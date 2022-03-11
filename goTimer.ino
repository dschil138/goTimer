
#include "Countimer.h"
#include <Wire.h>
#include "U8glib.h" 

//Initialize display.
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g2(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

#define CLK A1
#define DT A2
#define SW A3

#define CLK2 10
#define DT2 11
#define SW2 3

Countimer timer1;
Countimer timer2;
Countimer timerBy1;
Countimer timerBy2;

char buffer[10];
char buffer2[1];

bool changePeriods = false;

int button1 = 7;
int button3 = 5;

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

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int selectTime = 10;
int selectTime2 = 10;
int byoYomiTime = 10;

int byoYomiTimeSelectCounter = 1;
int settingsCounter = 1;

int currentStateCLK;
int lastStateCLK;
int currentStateCLK2;
int lastStateCLK2;

int periodsCounter = 5;
int spaces;
int lineLength;
int startLine1;
int startLine2;
int startLine3;
int startLine4;
int startLine5;

int pauseCounter = 0;

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
  //pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  
  pinMode(CLK2,INPUT);
  pinMode(DT2,INPUT);
  pinMode(SW2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(2), playerSwitch, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), decideByoYomiPeriods, RISING);

  digitalWrite(buzzer, HIGH);

  initScreen();
  
  lastStateCLK = digitalRead(CLK);
  lastStateCLK2 = digitalRead(CLK2);

  lastByStart1 = millis();
  lastByStart2 = millis();
  newStart1 = millis();
  newStart2 = millis();

  u8g.setFont(u8g_font_fub35n);
  u8g2.setFont(u8g_font_fub35n);
  byoYomiLines(periodsCounter);
  byoYomiIncrementer1 = periodsCounter;
  byoYomiIncrementer2 = periodsCounter;
}












////////////////////////////////////////////////// ---- LOOP ---- ///////////////////////////////////////////////////////////////////////////
void loop() {

//------------------------------------------------ START WHILE LOOP --------------------------------------------------------------------------
  while(settingsCounter == 1){

    buttonState1 = digitalRead(button1);   
    currentStateCLK = digitalRead(CLK);  
    currentStateCLK2 = digitalRead(CLK2); 

    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
      
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

    if ((currentStateCLK2 != lastStateCLK2  && currentStateCLK2 == 1) || changePeriods == true){
      if (changePeriods == false) {
        if (digitalRead(DT2) != currentStateCLK2) {
          byoYomiTime += 1;
        } else {
          byoYomiTime -= 1;
        }
      }
      u8g2.firstPage();
      do {
        displaySelectingTime(byoYomiTime, 2);
      } while (u8g2.nextPage());
    }
    
    changePeriods = false;
    lastStateCLK2 = currentStateCLK2;
  
    if (buttonState1 == LOW) {
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
      settingsCounter++;
    }
  }  
//------------------------------------------------ END WHILE LOOP ---------------------------------------------------------------------------------------
  while (gameEndCounter == 0) {
    
    if (playerSwitchCounter % 2 != 0) {
      if (byoYomiCounter1 < 1){
        timer1.run();
        if (!timer1.isCounterCompleted()) {
          timer1.start();
        }
      }

   
      if (byoYomiCounter1 >= 1) {        
        newStart1 = millis();    
        if ((newStart1 - lastByStart1) > 600) {
          setupByoYomiTimer1();       
        }
        lastByStart1 = millis();
        timerBy1.run();
        timerBy1.start();   
        Serial.println(F("Byo Yomi 1 Running"));                  
      }
    }



  
    
    
    if (playerSwitchCounter % 2 == 0) {
      if (byoYomiCounter2 < 1){
        timer2.run();
        if (!timer2.isCounterCompleted()) {
          timer2.start();
        }
      }

      
      if (byoYomiCounter2 >= 1) {  
        newStart2 = millis(); 
        if ((newStart2 - lastByStart2) > 600) {
          setupByoYomiTimer2();           
        }
        lastByStart2 = millis();
        timerBy2.run();
        timerBy2.start();   
        Serial.println(F("Byo Yomi 2 Running"));                 
      }
    }  
  } 
  bzzz;
}
