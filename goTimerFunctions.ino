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
  byoYomiLines(periodsCounter);
  TCA9548A(7);
  u8g.firstPage();
    do {
      u8g.drawStr(0, 40, timer1.getCurrentTime());
      u8g.drawBox(0,55,lineLength,6);
      u8g.drawBox(startLine2,55,lineLength,6);
      u8g.drawBox(startLine3,55,lineLength,6);
      if (periodsCounter >= 4) {
        u8g.drawBox(startLine4,55,lineLength,6);      
        }
      if (periodsCounter >= 5) {
        u8g.drawBox(startLine5,55,lineLength,6);      
        }  
      } while (u8g.nextPage());
}

void refreshClock2() {
  byoYomiLines(periodsCounter);
  TCA9548A(2);
  u8g2.firstPage();
    do {
      u8g2.drawStr(0, 40, timer2.getCurrentTime());
      u8g.drawBox(0,55,lineLength,6);
      u8g.drawBox(startLine2,55,lineLength,6);
      u8g.drawBox(startLine3,55,lineLength,6);
      if (periodsCounter >= 4) {
        u8g.drawBox(startLine4,55,lineLength,6);      
        }
      if (periodsCounter >= 5) {
        u8g.drawBox(startLine5,55,lineLength,6);      
        } 
    } while (u8g2.nextPage());
}

void refreshClockBy1() {
  TCA9548A(7);
  u8g.firstPage();
    do {
      u8g.drawStr(0, 40, timerBy1.getCurrentTime());     
      if (periodsPlayer1 == 1){
        u8g.drawBox(0,57,lineLength,7);
      }
      if (periodsPlayer1 == 2){
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,57,lineLength,7);
      }

      if (periodsPlayer1 == 3){
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,49,lineLength,14);
        u8g.drawBox(startLine3,57,lineLength,7);        
      }
      if (periodsPlayer1 == 4) {
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,49,lineLength,14);
        u8g.drawBox(startLine3,49,lineLength,14);
        u8g.drawBox(startLine4,57,lineLength,7);      
        }
      if (periodsPlayer1 == 5) {
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,49,lineLength,14);
        u8g.drawBox(startLine3,49,lineLength,14);
        u8g.drawBox(startLine4,49,lineLength,14);
        u8g.drawBox(startLine5,57,lineLength,7);      
      }     
    } while (u8g.nextPage());
  
}

void refreshClockBy2() {
  TCA9548A(2);
  u8g2.firstPage();
    do {
      u8g2.drawStr(0, 40, timerBy2.getCurrentTime());
      if (periodsPlayer2 == 1){
        u8g.drawBox(0,57,lineLength,7);
      }
      if (periodsPlayer2 == 2){
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,57,lineLength,7);
      }

      if (periodsPlayer2 == 3){
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,49,lineLength,14);
        u8g.drawBox(startLine3,57,lineLength,7);        
      }
      if (periodsPlayer2 == 4) {
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,49,lineLength,14);
        u8g.drawBox(startLine3,49,lineLength,14);
        u8g.drawBox(startLine4,57,lineLength,7);      
        }
      if (periodsPlayer2 == 5) {
        u8g.drawBox(0,49,lineLength,14);
        u8g.drawBox(startLine2,49,lineLength,14);
        u8g.drawBox(startLine3,49,lineLength,14);
        u8g.drawBox(startLine4,49,lineLength,14);
        u8g.drawBox(startLine5,57,lineLength,7);      
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
    sprintf(buffer2, "periods: %01d", periodsCounter);
    u8g2.drawStr(0, 60, buffer2);
  }
  u8g.setFont(u8g_font_fub35n);
  u8g2.setFont(u8g_font_fub35n);
}





void byoYomiLines (int periods) {

  if (periods == 5){
    lengthOfASpace = 8;   
    }
  else {
    lengthOfASpace = 7;
    }
  
  totalSpace = ((periods - 1) * lengthOfASpace);
  
  lineLength = ((128 - totalSpace)/periods);
  startLine1 = 0;
  startLine2 = (lineLength + lengthOfASpace);
  startLine3 = (lineLength + lengthOfASpace) * 2;
  
  if (periods > 3) {
    startLine4 = (lineLength + 8) * 3;
  }
  
  if (periods > 4) {
    startLine5 = (lineLength + 8) * 4;
  }
}





void decideByoYomiPeriods(){
  if (settingsCounter == 1){
   // changePeriods = true;
    if (periodsCounter < 5) {
      periodsCounter++;
      }
    else {
      periodsCounter = 3;
      }  
    delay(200);
    Serial.println(periodsCounter);
  }
}




















void startByoYomi1 () {
  Serial.println(F("startByoYomi 1"));
  delay(70);
  periodsPlayer1--;
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
  periodsPlayer2--;
  tone(buzzer, 1000); 
    delay(120);      
    noTone(buzzer);
    digitalWrite(buzzer, HIGH);
    delay(60);
  lastByStart2 = millis() - 1000;
 }


void setupByoYomiTimer1() {
  if (periodsPlayer1 > 1) {
    timerBy1.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy1.COUNT_DOWN, startByoYomi1);
  }
  if (periodsPlayer1 == 1) {
    timerBy1.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy1.COUNT_DOWN, gameEnd);
  }
  timerBy1.setInterval(refreshClockBy1, 850);
  Serial.println(F("Setup Byo Yomi 1"));
  delay(20);
}

void setupByoYomiTimer2() {
  if (periodsPlayer2 > 1) {
    timerBy2.setCounter(timeHoursBy, timeMinutesBy, timeSecondsBy, timerBy2.COUNT_DOWN, startByoYomi2);
  }
  if (periodsPlayer2 == 1) {
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




void pauseTimer () {
  newPause = millis();
  if ((newPause - lastPause) > 600) {
    pauseCounter++;  
    lastPause = newPause;     
  } 
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
