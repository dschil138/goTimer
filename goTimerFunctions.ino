void computeTime(int totalTime) {
  timeMinutes = totalTime / 60;
  timeHours = timeMinutes / 60;
  timeSeconds = totalTime % 60;
}

void computeTime2(int totalTime) {
  timeMinutes2 = totalTime / 60;
  timeHours2 = timeMinutes2 / 60;
  timeSeconds2 = totalTime % 60;
}

void computeTimeBy(int totalTime) {
  timeMinutesBy = totalTime / 60;
  timeHoursBy = timeMinutesBy / 60;
  timeSecondsBy = totalTime % 60;
}




void initScreen() {

  TCA9548A(7);
  u8g.setFont(u8g_font_unifont);
  u8g.firstPage(); 
    do {     
    u8g.drawStr(0, 60, "select main time time");
    } while (u8g.nextPage());
    
  TCA9548A(2);
  u8g2.setFont(u8g_font_unifont); 
  u8g2.firstPage();
    do {     
    u8g2.drawStr(0, 60, "select byo yomi time");
    } while (u8g2.nextPage()); 
}


void refreshClock1() {
  TCA9548A(7);
  u8g.firstPage();
    do {
      u8g.drawStr(0, 40, timer1.getCurrentTime());
      u8g.drawBox(0,55,36,6);
      u8g.drawBox(45,55,35,6);
      u8g.drawBox(90,55,36,6);     
      } while (u8g.nextPage());
}

void refreshClock2() {
  TCA9548A(2);
  u8g2.firstPage();
    do {
      u8g2.drawStr(0, 40, timer2.getCurrentTime());
      u8g.drawBox(0,55,36,6);
      u8g.drawBox(45,55,35,6);
      u8g.drawBox(90,55,36,6); 
    } while (u8g2.nextPage());
}

void refreshClockBy1() {
  TCA9548A(7);
  u8g.firstPage();
    do {
      u8g.drawStr(0, 40, timerBy1.getCurrentTime());     
      if (byoYomiIncrementer1 == 3){
        u8g.drawBox(0,56,38,7);
      }
      if (byoYomiIncrementer1 == 2){
        u8g.drawBox(0,49,38,14);
        u8g.drawBox(45,57,38,7);
      }

      if (byoYomiIncrementer1 == 1){
        u8g.drawBox(0,49,38,14);
        u8g.drawBox(44,49,38,14);
        u8g.drawBox(88,57,38,7);        
      }     
    } while (u8g.nextPage());
  
}

void refreshClockBy2() {
  TCA9548A(2);
  u8g2.firstPage();
    do {
      u8g2.drawStr(0, 40, timerBy2.getCurrentTime());
      if (byoYomiIncrementer2 == 3){
        u8g2.drawBox(0,56,38,7);
      }
      if (byoYomiIncrementer2 == 2){
        u8g2.drawBox(0,49,38,14);
        u8g2.drawBox(45,57,38,7);
      }

      if (byoYomiIncrementer2 == 1){
        u8g2.drawBox(0,49,38,14);
        u8g2.drawBox(44,49,38,14);
        u8g2.drawBox(88,57,38,7);       
      }     
    } while (u8g2.nextPage());
}

void displaySelectingTime(int player, int screen) {
  TCA9548A(screen);
  computeTime(player);
  sprintf(buffer, "%02d:%02d", timeMinutes, timeSeconds);
  if (screen == 7) {
    u8g.drawStr(0, 40, buffer);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 60, "select main time");
  }
  if (screen == 2) {
    u8g2.drawStr(0, 40, buffer);
    u8g2.setFont(u8g_font_unifont);
    u8g2.drawStr(0, 60, "select byo yomi time");
  }
  u8g.setFont(u8g_font_fub35n);
  u8g2.setFont(u8g_font_fub35n);
}





















void startByoYomi1 () {
  Serial.println(F("startByoYomi 1"));
  delay(70);
  byoYomiCounter1 = 1;
  byoYomiIncrementer1++;
  tone(buzzer, 1000); 
    delay(120);      
    noTone(buzzer);
    digitalWrite(buzzer, HIGH);
    delay(60);
  lastByStart1 = millis() -1000;
 }

 void startByoYomi2 () {
  Serial.println(F("startByoYomi 2"));
  delay(70);
  byoYomiCounter2 = 1;
  byoYomiIncrementer2++;
  tone(buzzer, 1000); 
    delay(120);      
    noTone(buzzer);
    digitalWrite(buzzer, HIGH);
    delay(60);
  lastByStart2 = millis() - 1000;
 }


void setupByoYomiTimer1() {
  if (byoYomiIncrementer1 < 3) {
    timerBy1.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy1.COUNT_DOWN, startByoYomi1);
  }
  if (byoYomiIncrementer1 == 3) {
    timerBy1.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy1.COUNT_DOWN, gameEnd);
  }
  timerBy1.setInterval(refreshClockBy1, 850);
  Serial.println(F("Setup Byo Yomi 1"));
  delay(20);
}

void setupByoYomiTimer2() {
  if (byoYomiIncrementer2 < 3) {
    timerBy2.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy2.COUNT_DOWN, startByoYomi2);
  }
  if (byoYomiIncrementer2 == 3) {
    timerBy2.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy2.COUNT_DOWN, gameEnd);
  }
  timerBy2.setInterval(refreshClockBy2, 850);
  Serial.println(F("Setup Byo Yomi 2"));
  delay(20);
}









void playerSwitch() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    playerSwitchCounter++;
  }
  last_interrupt_time = interrupt_time;
}


void bzzz(void) {
  TCA9548A(7);
  u8g.firstPage();
  do {
    u8g.drawStr(20, 20, "BZ");
  } while (u8g.nextPage());
  Serial.println(F("bzzz."));

  TCA9548A(2);
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 40, "BZ");
  } while (u8g2.nextPage());
   
}




void gameEnd() {
  TCA9548A(7);
  u8g.firstPage();
  do {
    u8g.drawStr(20, 20, "BZ");
  } while (u8g.nextPage());
  Serial.println(F("bzzz."));
  
  TCA9548A(2);
  u8g2.firstPage();
  do {
    u8g2.drawStr(0, 40, "BZ");
  } while (u8g2.nextPage());
    
  for (int i = 0; i <= 3; i++) {
    tone(buzzer, 600); 
    delay(120);      
    noTone(buzzer);
    digitalWrite(buzzer, HIGH);
    delay(40);
  }
  gameEndCounter ++;
}
