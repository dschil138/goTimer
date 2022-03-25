#include <EEPROM.h>
#include "Countimer.h"
#include <Wire.h>
#include "U8glib.h" 

//Initialize display.
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g2(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

#define CLK 11  //10 for breadboad, 11 for case
#define DT 12   //11 for breadboad, 12 for case
#define SW 10   //12 for breadboad, 10 for case

#define CLK2 A1
#define DT2 A2
#define SW2 A3

Countimer timer1;
Countimer timer2;
Countimer timerBy1;
Countimer timerBy2;

int eepMainTime;
int eepBYTime;

char buffer[10];
char buffer2[1];

int button1 = 2;
int rockerSwitch = 3;

const int buzzer = 9;

int timeSeconds = 17;
int timeMinutes = 0;
int timeHours = 0;

int timeSeconds2 = 17;
int timeMinutes2 = 0;
int timeHours2 = 0;

int timeSecondsBy = 6;
int timeMinutesBy = 0;
int timeHoursBy = 0;

int pauseCounter = 0;
int playerSwitchCounter = 1;
int byoYomiCounter1 = 0;
int byoYomiCounter2 = 0;
int byoYomiIncrementer1 = 0;
int byoYomiIncrementer2 = 0;
int gameEndCounter = 0;
unsigned long lastByStart1 = 0;
unsigned long lastByStart2 = 0;
unsigned long newStart1 = 0;
unsigned long newStart2 = 0;
unsigned long lastBeep = 0;
unsigned long newBeep = 0;

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int selectTime = EEPROM.read(10)*30; //600, 5 for testing
int selectTime2 = EEPROM.read(10)*30; //600, 5 for testing
int byoYomiTime = EEPROM.read(20); //30, 3 for testing

int byoYomiTimeSelectCounter = 1;
int settingsCounter = 1;

int currentStateCLK;
int lastStateCLK;
int currentStateCLK2;
int lastStateCLK2;
int currentEncoder2ButtonState;
unsigned long newPause = 0;
unsigned long lastPause = 0;

bool changePeriods;
int periodsCounter = EEPROM.read(30);
int periodsPlayer1;
int periodsPlayer2;
int periodsSelector = 3;
int spaces;
int totalSpace;
int lengthOfASpace;
int lineLength;
int startLine1;
int startLine2;
int startLine3;
int startLine4;
int startLine5;

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(bus);
}










/////////////////////////////////////////////////// -- SET-UP -- /////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(rockerSwitch, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  
  pinMode(CLK2,INPUT);
  pinMode(DT2,INPUT);
  pinMode(SW2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(rockerSwitch), playerSwitch, CHANGE); //2 for breadboard, 3 for case
  attachInterrupt(digitalPinToInterrupt(button1), pauseTimer, RISING);   //3 for breadboard, 2 for case

  digitalWrite(buzzer, HIGH);

  initScreen();
  
  lastStateCLK = digitalRead(CLK);
  lastStateCLK2 = digitalRead(CLK2);

  lastByStart1 = millis();
  lastByStart2 = millis();
  newStart1 = millis();
  newStart2 = millis();
  lastPause = millis();
  newPause = millis();

  u8g.setFont(u8g_font_fub35n);
  u8g2.setFont(u8g_font_fub35n);
}











////////////////////////////////////////////////// ---- LOOP ---- ///////////////////////////////////////////////////////////////////////////
void loop() {

//------------------------------------------------ START WHILE LOOP - CHOOSING SETTINGS --------------------------------------------------------------------------
  while(settingsCounter == 1){

    buttonState1 = digitalRead(button1);   
    currentStateCLK = digitalRead(CLK);  
    currentStateCLK2 = digitalRead(CLK2);
    currentEncoder2ButtonState = digitalRead(SW2);

    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){ //ENCODER 1 CHOOSES MAIN TIME
      if (digitalRead(DT) != currentStateCLK) {
        selectTime += 30;
      } else {
        selectTime -= 30;
      }
      
      u8g.firstPage();
      do {
        displaySelectingTime(selectTime, 7);
      } while (u8g.nextPage());
    }

    lastStateCLK = currentStateCLK;

    if (currentStateCLK2 != lastStateCLK2  && currentStateCLK2 == 1){ // ENCODER 2 CHOOSES BYO YOMI TIME
      if (digitalRead(DT2) != currentStateCLK2) {
        byoYomiTime += 1;
      } else {
        byoYomiTime -= 1;
      }
      u8g2.firstPage();
      do {
        displaySelectingTime(byoYomiTime, 2);
      } while (u8g2.nextPage());
    }

    lastStateCLK2 = currentStateCLK2;

    if (currentEncoder2ButtonState == LOW){
      decideByoYomiPeriods(); 
      u8g2.firstPage();
      do {
        displaySelectingTime(byoYomiTime, 2);
      } while (u8g2.nextPage());
      }
  
    if (buttonState1 == LOW) { // HITTING START/PAUSE BUTTON INITIATES ALL THE CLOCKS AND MOVES TO GAME MODE
      int currentEncoder1ButtonState = digitalRead(SW);
      if (currentEncoder1ButtonState == LOW){
        eepMainTime = selectTime/30;
        EEPROM.write(10, eepMainTime);
        EEPROM.write(20, byoYomiTime);
        EEPROM.write(30, periodsCounter);
        }
      int startingPlayer = digitalRead(2);
      if (startingPlayer == LOW) {
        playerSwitchCounter = 2;
      }
      computeTime(selectTime);
      computeTime2(selectTime);
      computeTimeBy(byoYomiTime);    
      timer1.setCounter(timeHours, timeMinutes, timeSeconds, timer1.COUNT_DOWN, startByoYomi1);
      timer2.setCounter(timeHours2, timeMinutes2, timeSeconds2, timer2.COUNT_DOWN, startByoYomi2);
      timer1.setInterval(refreshClock1, 850);
      timer2.setInterval(refreshClock2, 850);
      periodsPlayer1 = periodsCounter + 1;
      periodsPlayer2 = periodsCounter + 1;
      settingsCounter++;
      pauseCounter++;
      
    }
  }  
//------------------------------------------------ END: SETINGS MODE ---------------------------------------------------------------------------------------
//------------------------------------------------ BEGIN: GAME MODE ---------------------------------------------------------------------------------------
  while (gameEndCounter == 0) {

    if ((pauseCounter % 2) == 0) {

    
      if (playerSwitchCounter % 2 != 0) {  // CONTROLLING PLAYER 1 CLOCK
        if (periodsPlayer1 > periodsCounter){
          timer1.run();
          if (!timer1.isCounterCompleted()) {
            timer1.start();
          }
        }
  
     
        if (periodsPlayer1 <= periodsCounter) {        
          newStart1 = millis();    
          if ((newStart1 - lastByStart1) > 600) {  
            setupByoYomiTimer1();                  //resets Byo Yomi clock at beginning of each turn
          }
          lastByStart1 = millis();
          timerBy1.run();
          timerBy1.start();  
          if (timerBy1.getCurrentSeconds() == 5){
            newBeep = millis();
            if (newBeep - lastBeep > 1200) {
              tone(buzzer, 1000); 
              delay(100);      
              noTone(buzzer);
              delay(30);
              tone(buzzer, 1000); 
              delay(70);      
              noTone(buzzer);
              digitalWrite(buzzer, HIGH);
              lastBeep = millis();
            }
          }
          Serial.println(F("Byo Yomi 1 Running"));                  
        }
      }
  
  
  
    
      
      
      if (playerSwitchCounter % 2 == 0) {    // CONTROLLING PLAYER 2 CLOCK
        if (byoYomiCounter2 < 1){
          timer2.run();
          if (!timer2.isCounterCompleted()) {
            timer2.start();
          }
        }
  
        
        if (periodsPlayer2 <= periodsCounter) {  
          newStart2 = millis(); 
          if ((newStart2 - lastByStart2) > 600) {
            setupByoYomiTimer2();                  //resets Byo Yomi clock at beginning of each turn
          }
          lastByStart2 = millis();
          timerBy2.run();
          timerBy2.start();
          if (timerBy2.getCurrentSeconds() == 5){
            newBeep = millis();
            if (newBeep - lastBeep > 1200) {
              tone(buzzer, 1000); 
              delay(100);      
              noTone(buzzer);
              delay(30);
              tone(buzzer, 1000); 
              delay(70);      
              noTone(buzzer);
              digitalWrite(buzzer, HIGH);
              lastBeep = millis();
            }
          }  
          Serial.println(F("Byo Yomi 2 Running"));                 
        }
      }
    }
    else {
      Serial.println("paused");
    }      
  } 
  bzzz();
}
