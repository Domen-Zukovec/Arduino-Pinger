#include <ICMPPing.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//Primer vseh funkcij za LCD zaslon:
/*###########################################
  # Methods and Functions (KEYWORD2)
  ###########################################
  init  KEYWORD2
  begin KEYWORD2
  clear KEYWORD2
  home  KEYWORD2
  noDisplay KEYWORD2
  display KEYWORD2
  noBlink KEYWORD2
  blink KEYWORD2
  noCursor  KEYWORD2
  cursor  KEYWORD2
  scrollDisplayLeft KEYWORD2
  scrollDisplayRight  KEYWORD2
  leftToRight KEYWORD2
  rightToLeft KEYWORD2
  shiftIncrement  KEYWORD2
  shiftDecrement  KEYWORD2
  noBacklight KEYWORD2
  backlight KEYWORD2
  autoscroll  KEYWORD2
  noAutoscroll  KEYWORD2
  setCursor KEYWORD2
  print KEYWORD2
  blink_on  KEYWORD2
  blink_off KEYWORD2
  cursor_on KEYWORD2
  cursor_off  KEYWORD2
  load_custom_character KEYWORD2
  printstr  KEYWORD2
*/

LiquidCrystal_I2C lcd(0x27, 20, 4); //0x27 je I2C naslov, 20 je stevilo znakov v vrstici, 4 stevilo vrstic

byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x56, 0x5B}; // mac address for ethernet shield
byte ip[] = {192, 168, 8, 32}; // ip address for ethernet shield
byte pingAddr[] = {8, 8, 8, 8}; // ip google


SOCKET pingSocket = 0;


int reset = A15;
bool nadaljuj = 0;
long prev2 = 0;
int buttonState2 = 0;
long prev3 = 0;
int buttonState3 = 0;
long prev4 = 0;
int buttonState4 = 0;
long prev = 0;
int buttonState = 0;// 0 = not pressed   --- 1 = long pressed --- 2 short pressed
int YOUR_RESET_BUTTON_PIN = 2;
int YOUR_RESET_BUTTON_PIN2 = 3;
int YOUR_RESET_BUTTON_PIN3 = 4;
int YOUR_RESET_BUTTON_PIN4 = 5;
int DURATION_IN_MILLIS = 2000;
int DURATION_IN_MILLIS2 = 2000;
int DURATION_IN_MILLIS3 = 2000;
int DURATION_IN_MILLIS4 = 2000;
int google_pritisk = 200;

int delayMS = 4 * 1000; // delay between successive pings (60 * 1000 = 60 seconds)







int rebootEthShield()
{ lcd.clear();

  //  Serial.println("Rebooting ethernet shield...");
  pinMode(7, OUTPUT);

  digitalWrite(7, HIGH);

  delay(50);

  digitalWrite(7, LOW);

  delay(50);

  digitalWrite(7, HIGH);

  delay(100);

  

  byte macData[] = {0xaa, 0xbb, 0xcc, 0x12, 0x34, mac};
  lcd.setCursor(0, 0);
  lcd.print("--------------------");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");
  lcd.setCursor(0, 2);
  lcd.print("|                  |");
  lcd.setCursor(0, 1);
  lcd.print("|   Pridobivam IP  |");// <-- Fill in your Ethernet shield's MAC address here.
  Ethernet.begin(macData);

  // delay(500);
  // client.setTimeout(500);
  //  wdt_reset(); //resetiram watchdog timer
  lcd.clear();
IPAddress Gateway = Ethernet.gatewayIP();
  if (Gateway[0] == 192){
      
   
  lcd.setCursor(0, 0);
  lcd.print("--------------------");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");
  lcd.setCursor(0, 2);
  lcd.print("|                  |");
  lcd.setCursor(0, 1);
  lcd.print("|     Internet     |");
  delay(2000);

    
  }else{ 
    
   lcd.setCursor(0, 0);
  lcd.print("--------------------");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");
  lcd.setCursor(0, 2);
  lcd.print("|                  |");
  lcd.setCursor(0, 1);
  lcd.print("|    Voice/Video   |");
  delay(2000);}
}




void googleKratek() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;


  int sensorVal = digitalRead(2);
  int sensorVal2 = digitalRead(3);
  int naprej1 = digitalRead(2);
  int naprej2 = digitalRead(3);
  int neki = digitalRead(4);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;
  lcd.clear();

  //KRATKI!


  Serial.println("TODO button is pressed ");
  IPAddress Lokalni_ip = Ethernet.localIP();
  IPAddress Gateway = Ethernet.gatewayIP();



  Serial.println(Lokalni_ip[1]);
  //byte pingAddr[] = {Gateway[0],Gateway[1],Gateway[2],Gateway[3]};

  bool pingRet; // pingRet stores the ping() success (true/false)
  //startPing();
  ICMPPing ping(pingSocket);
  pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
  delay(250);
  //endPing();


  Serial.println(buffer);
  //lcd.print(buffer);




  if (pingRet && ping_time < 2000 && ping_time >= 0 ) // Failure
  { digitalWrite(9, HIGH);
    // pingSuccess();
  //  neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja == 0) {
      neki = digitalRead(3);
      lcd.setCursor(0, 0);
      lcd.print("IP: " );
      lcd.print(Lokalni_ip);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Povezava JE ok");

      reset = analogRead(A15);
      //Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();
          ja = 1;


        }
      }
    }
    digitalWrite(9, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
  }
  else
  { digitalWrite(8, HIGH);
    //pingFail();
    lcd.setCursor(0, 0);
    lcd.print("IP: " );
    lcd.print(Lokalni_ip);
    lcd.setCursor(0, 1);
    sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
    Serial.println(buffer);
    lcd.print(buffer);
  //  neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja2 == 0) {
      neki = digitalRead(3);
      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = // ");
      //lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Povezava NI ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();
          ja2 = 1;

        }
      }
    }
    digitalWrite(8, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
    //TODO button is pressed short
  }
}


void localKratek() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;


  int sensorVal = digitalRead(2);
  int sensorVal2 = digitalRead(3);
  int naprej1 = digitalRead(2);
  int naprej2 = digitalRead(3);
  int neki = digitalRead(4);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;
  lcd.clear();

  //KRATKI!


  Serial.println("TODO button is pressed ");
  IPAddress Lokalni_ip = Ethernet.localIP();
  IPAddress Gateway = Ethernet.gatewayIP();



  Serial.println(Lokalni_ip[1]);
  byte pingAddr[] = {Gateway[0], Gateway[1], Gateway[2], Gateway[3]};

  bool pingRet; // pingRet stores the ping() success (true/false)
  //startPing();
  ICMPPing ping(pingSocket);
  pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
  delay(250);
  //endPing();


  Serial.println(buffer);
  //lcd.print(buffer);




  if (pingRet && ping_time < 2000 && ping_time >= 0 ) // Failure
  { digitalWrite(9, HIGH);
    // pingSuccess();
    neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja == 0) {
      neki = digitalRead(2);
      lcd.setCursor(0, 0);
      lcd.print("IP: " );
      lcd.print(Lokalni_ip);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Lokal povezava JE ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();
          ja = 1;


        }
      }
    }
    digitalWrite(9, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
  }
  else
  { digitalWrite(8, HIGH);
    //pingFail();
    lcd.setCursor(0, 0);
    lcd.print("IP: " );
    lcd.print(Lokalni_ip);
    lcd.setCursor(0, 1);
    sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
    Serial.println(buffer);
    lcd.print(buffer);
    neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja2 == 0) {
      neki = digitalRead(2);
      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = // ");
      //lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Lokal povezava NI ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          lcd.clear();
          rebootEthShield();
          ja2 = 1;

        }
      }
    }
    digitalWrite(8, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
    //TODO button is pressed short
  }
}











void videoKratek() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;


  int sensorVal = digitalRead(4);
  int sensorVal2 = digitalRead(5);
  int neki = digitalRead(6);
  int naprej2 = digitalRead(5);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;
  lcd.clear();

  //KRATKI!


  Serial.println("TODO button is pressed ");
  IPAddress Lokalni_ip = Ethernet.localIP();
  IPAddress Gateway = Ethernet.gatewayIP();



  Serial.println(Lokalni_ip[1]);
  byte pingAddr[] = {10, 253, 3, 120};
  //byte pingAddr[] = {10,253,47,115}; REZERVA

  bool pingRet; // pingRet stores the ping() success (true/false)
  //startPing();
  ICMPPing ping(pingSocket);
  pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
  delay(250);
  //endPing();


  Serial.println(buffer);
  //lcd.print(buffer);




  if (pingRet && ping_time < 2000 && ping_time >= 0 ) // Failure
  { digitalWrite(9, HIGH);
    // pingSuccess();
    neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja == 0) {
      neki = digitalRead(5);
      lcd.setCursor(0, 0);
      lcd.print("IP: " );
      lcd.print(Lokalni_ip);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Video povezava JE ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();
          ja = 1;


        }
      }
    }
    digitalWrite(9, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
  }
  else
  { digitalWrite(8, HIGH);
    //pingFail();
    lcd.setCursor(0, 0);
    lcd.print("IP: " );
    lcd.print(Lokalni_ip);
    lcd.setCursor(0, 1);
    sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
    Serial.println(buffer);
    lcd.print(buffer);
    neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja2 == 0) {
      neki = digitalRead(5);
      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = // ");
      //lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Video povezava NI ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          lcd.clear();
          rebootEthShield();
          ja2 = 1;

        }
      }
    }
    digitalWrite(8, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
    //TODO button is pressed short
  }
}














void voiceKratek() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;


  int sensorVal = digitalRead(4);
  int sensorVal2 = digitalRead(5);
  int neki = digitalRead(6);
  int naprej2 = digitalRead(5);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;
  lcd.clear();

  //KRATKI!


  Serial.println("TODO button is pressed ");
  IPAddress Lokalni_ip = Ethernet.localIP();
  IPAddress Gateway = Ethernet.gatewayIP();



  Serial.println(Lokalni_ip[1]);
  byte pingAddr[] = {10, 253, 0, 1};

  bool pingRet; // pingRet stores the ping() success (true/false)
  //startPing();
  ICMPPing ping(pingSocket);
  pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
  delay(250);
  //endPing();


  Serial.println(buffer);
  //lcd.print(buffer);




  if (pingRet && ping_time < 2000 && ping_time >= 0 ) // Failure
  { digitalWrite(9, HIGH);
    // pingSuccess();
    neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja == 0) {
      neki = digitalRead(4);
      lcd.setCursor(0, 0);
      lcd.print("IP: " );
      lcd.print(Lokalni_ip);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Voice povezava JE ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();
          ja = 1;


        }
      }
    }
    digitalWrite(9, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
  }
  else
  { digitalWrite(8, HIGH);
    //pingFail();
    lcd.setCursor(0, 0);
    lcd.print("IP: " );
    lcd.print(Lokalni_ip);
    lcd.setCursor(0, 1);
    sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
    Serial.println(buffer);
    lcd.print(buffer);
    neki = LOW; //tezava z gumbi moral restit programsko, saj je gumb vezan na pulup in ne dobi dost hitr low in se loop sploh ne zacne
    while (neki == LOW && ja2 == 0) {
      neki = digitalRead(4);
      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = // ");
      //lcd.print(ping_time);
      lcd.print("ms");

      lcd.setCursor(0, 3);
      lcd.print("Voice povezava NI ok");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          lcd.clear();
          rebootEthShield();
          ja2 = 1;

        }
      }
    }
    digitalWrite(8, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("1. Ping -> loklani");
    lcd.setCursor(1, 1);
    lcd.print("2. Ping -> google");
    lcd.setCursor(1, 2);
    lcd.print("3. Ping -> Voice");
    lcd.setCursor(1, 3);
    lcd.print("4. Ping -> Video");
    delay(100);
    //TODO button is pressed short
  }
}



















void googleDolgi() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;

  bool dela = 1;
  int sensorVal = digitalRead(2);
  int sensorVal2 = digitalRead(3);
  int naprej1 = digitalRead(2);
  int naprej2 = digitalRead(3);
  int neki = digitalRead(4);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;
  int ena = 0;
  int dva = 0;
  lcd.clear();

  //DOLGI!!!!
  while (ja == 0) {
    neki = digitalRead(3);
    IPAddress Lokalni_ip = Ethernet.localIP();
    IPAddress Gateway = Ethernet.gatewayIP();

    //GLEDA KER GUMB SI KLIKNU
    buttonState2 = 0;

    if(digitalRead(YOUR_RESET_BUTTON_PIN2) && ne==0){}else{ne=1;}
         
          
       if (digitalRead(YOUR_RESET_BUTTON_PIN2) && ne==1) {
      prev2 = millis();
      buttonState2 = 1;
      while ((millis() - prev2) <= DURATION_IN_MILLIS2) {
        if (!(digitalRead(YOUR_RESET_BUTTON_PIN2))) {
          buttonState2 = 2;
          break;
          ne=1;
        }
      }
    }
      
    //GLEDA KER GUMB SI KLIKNU

    //PREVRERJA!
    if (!buttonState2)
    {
      //NIČ NI PRITISNENO;
    }
    else if (buttonState2 == 1)
    {
      ja = 1;
    }
    else if (buttonState2 == 2)
    {
      // localKratek();
    }
    neki = digitalRead(2);




    Serial.println(Lokalni_ip[1]);
    //byte pingAddr[] = {Gateway[0],Gateway[1],Gateway[2],Gateway[3]};

    bool pingRet; // pingRet stores the ping() success (true/false)
    //startPing();
    ICMPPing ping(pingSocket);
    pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
    delay(250);
    //endPing();

    if (pingRet && ping_time < 2000 && ping_time >= 0 ) {

      Serial.println(buffer);
      //lcd.print(buffer);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");
      ping_time = 0;
      dela = !dela;

      if (dela == 1) {
        lcd.setCursor(8, 3);
        lcd.print(" .");
      }
      if (dela == 0) {
        lcd.setCursor(8, 3);
        lcd.print(". ");
      }

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();

        }
      }

    }
    else {

      lcd.setCursor(0, 2);
      lcd.print("...Ping neuspesen...");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }

    }


  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("1. Ping -> loklani");
  lcd.setCursor(1, 1);
  lcd.print("2. Ping -> google");
  lcd.setCursor(1, 2);
  lcd.print("3. Ping -> Voice");
  lcd.setCursor(1, 3);
  lcd.print("4. Ping -> Video");
}

void setup()
{
  bool dela = 1;
  int sensorVal = digitalRead(2);
  int sensorVal2 = digitalRead(3);
  int naprej1 = digitalRead(2);
  int naprej2 = digitalRead(3);
  int neki = digitalRead(4);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;
  int ena = 0;
  int dva = 0;
  Ethernet.hostName("arduinoping");

  pinMode(4, INPUT);



  pinMode(YOUR_RESET_BUTTON_PIN, INPUT);
  pinMode(YOUR_RESET_BUTTON_PIN2, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  lcd.begin();
  lcd.home ();

  Serial.begin(9600);
  Serial.println("Starting ethernet connection");




  reset = analogRead(A15);
  Serial.println(reset);
  if (reset < 380 || reset > 460) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("--------------------");
    lcd.setCursor(0, 1);
    lcd.print("|Kabel ni vkljucen |");
    lcd.setCursor(0, 3);
    lcd.print("--------------------");
    int lala = 0;
    while (reset < 380 || reset > 460) {
      lcd.setCursor(0, 2);
      lcd.print("|    Vkljuci ga!   |");
      reset = analogRead(A15);
      Serial.println(reset);

    }
    if (reset > 380 && reset < 460 && lala == 0)
    {
      lala++;
      lcd.clear();
      rebootEthShield();
      ja2 = 1;

    }
  }


  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("1. Ping -> loklani");
  lcd.setCursor(1, 1);
  lcd.print("2. Ping -> google");
  lcd.setCursor(1, 2);
  lcd.print("3. Ping -> Voice");
  lcd.setCursor(1, 3);
  lcd.print("4. Ping -> Video");



  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    lcd.setCursor(0, 0);
    lcd.print("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);

  }
}

void LocalDolg() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;

  bool dela = 1;
  int sensorVal = digitalRead(2);
  int sensorVal2 = digitalRead(3);
  int naprej1 = digitalRead(2);
  int naprej2 = digitalRead(3);
  int neki = digitalRead(4);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;




  lcd.clear();

  //DOLGI!!!!
  while (ja == 0) {
    neki = digitalRead(3);
    IPAddress Lokalni_ip = Ethernet.localIP();
    IPAddress Gateway = Ethernet.gatewayIP();

       //GLEDA KER GUMB SI KLIKNU
    buttonState = 0;

    if(digitalRead(YOUR_RESET_BUTTON_PIN) && ne==0){}else{ne=1;}
         
          
       if (digitalRead(YOUR_RESET_BUTTON_PIN) && ne==1) {
      prev = millis();
      buttonState = 1;
      while ((millis() - prev) <= DURATION_IN_MILLIS) {
        if (!(digitalRead(YOUR_RESET_BUTTON_PIN))) {
          buttonState = 2;
          break;
          ne=1;
        }
      }
    }
      
    //GLEDA KER GUMB SI KLIKNU

    //PREVRERJA!
    if (!buttonState)
    {
      //NIČ NI PRITISNENO;
    }
    else if (buttonState == 1)
    {
      ja = 1;
    }
    else if (buttonState == 2)
    {
      // localKratek();
    }

    Serial.println(Lokalni_ip[1]);
    byte pingAddr[] = {Gateway[0], Gateway[1], Gateway[2], Gateway[3]};

    bool pingRet; // pingRet stores the ping() success (true/false)
    //startPing();
    ICMPPing ping(pingSocket);
    pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
    delay(250);
    //endPing();


    if (pingRet && ping_time < 2000 && ping_time >= 0 ) {
      Serial.println(buffer);
      //lcd.print(buffer);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");
      ping_time = 0;
      dela = !dela;

      if (dela == 1) {
        lcd.setCursor(8, 3);
        lcd.print(" .");
      }
      if (dela == 0) {
        lcd.setCursor(8, 3);
        lcd.print(". ");
      }

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }
    }
    else {

      lcd.setCursor(0, 2);
      lcd.print("...Ping neuspesen...");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }

    }
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("1. Ping -> loklani");
  lcd.setCursor(1, 1);
  lcd.print("2. Ping -> google");
  lcd.setCursor(1, 2);
  lcd.print("3. Ping -> Voice");
  lcd.setCursor(1, 3);
  lcd.print("4. Ping -> Video");
}

















void videoDolg() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;

  bool dela = 1;
  int sensorVal = digitalRead(4);
  int sensorVal2 = digitalRead(5);
  int naprej1 = digitalRead(4);
  int naprej2 = digitalRead(5);
  int neki = digitalRead(6);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;




  lcd.clear();

  //DOLGI!!!!
  while (ja == 0) {
    neki = digitalRead(5);
    IPAddress Lokalni_ip = Ethernet.localIP();
    IPAddress Gateway = Ethernet.gatewayIP();

    //GLEDA KER GUMB SI KLIKNU
    buttonState4 = 0;

    if(digitalRead(YOUR_RESET_BUTTON_PIN4) && ne==0){}else{ne=1;}
         
          
       if (digitalRead(YOUR_RESET_BUTTON_PIN4) && ne==1) {
      prev4 = millis();
      buttonState4 = 1;
      while ((millis() - prev4) <= DURATION_IN_MILLIS4) {
        if (!(digitalRead(YOUR_RESET_BUTTON_PIN4))) {
          buttonState4 = 2;
          break;
          ne=1;
        }
      }
    }
      
    //GLEDA KER GUMB SI KLIKNU

    //PREVRERJA!
    if (!buttonState4)
    {
      //NIČ NI PRITISNENO;
    }
    else if (buttonState4 == 1)
    {
      ja = 1;
    }
    else if (buttonState4 == 2)
    {
      // localKratek();
    }

    Serial.println(Lokalni_ip[1]);
    byte pingAddr[] = {10, 253, 3, 120};
    //byte pingAddr[] = {10,253,47,115}; REZERVA

    bool pingRet; // pingRet stores the ping() success (true/false)
    //startPing();
    ICMPPing ping(pingSocket);
    pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
    delay(250);
    //endPing();


    if (pingRet && ping_time < 2000 && ping_time >= 0 ) {
      Serial.println(buffer);
      //lcd.print(buffer);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");
      ping_time = 0;
      dela = !dela;

      if (dela == 1) {
        lcd.setCursor(8, 3);
        lcd.print(" .");
      }
      if (dela == 0) {
        lcd.setCursor(8, 3);
        lcd.print(". ");
      }


      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }
    }
    else {

      lcd.setCursor(0, 2);
      lcd.print("...Ping neuspesen...");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }

    }
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("1. Ping -> loklani");
  lcd.setCursor(1, 1);
  lcd.print("2. Ping -> google");
  lcd.setCursor(1, 2);
  lcd.print("3. Ping -> Voice");
  lcd.setCursor(1, 3);
  lcd.print("4. Ping -> Video");
}






















void voiceDolg() {
  char buffer [256];
  int TTL = 0;
  int ping_time = 0;

  bool dela = 1;
  int sensorVal = digitalRead(4);
  int sensorVal2 = digitalRead(5);
  int naprej1 = digitalRead(4);
  int naprej2 = digitalRead(5);
  int neki = digitalRead(6);
  bool ja = 0;
  bool ja2 = 0;
  bool ne = 0;
  bool ne2 = 0;




  lcd.clear();

  //DOLGI!!!!
  while (ja == 0) {
    neki = digitalRead(4);
    IPAddress Lokalni_ip = Ethernet.localIP();
    IPAddress Gateway = Ethernet.gatewayIP();

    //GLEDA KER GUMB SI KLIKNU
    buttonState3 = 0;

    if(digitalRead(YOUR_RESET_BUTTON_PIN3) && ne==0){}else{ne=1;}
         
          
       if (digitalRead(YOUR_RESET_BUTTON_PIN3) && ne==1) {
      prev3 = millis();
      buttonState3 = 1;
      while ((millis() - prev3) <= DURATION_IN_MILLIS3) {
        if (!(digitalRead(YOUR_RESET_BUTTON_PIN3))) {
          buttonState3 = 2;
          break;
          ne=1;
        }
      }
    }
      
    //GLEDA KER GUMB SI KLIKNU

    //PREVRERJA!
    if (!buttonState3)
    {
      //NIČ NI PRITISNENO;
    }
    else if (buttonState3 == 1)
    {
      ja = 1;
    }
    else if (buttonState3 == 2)
    {
      // localKratek();
    }

    Serial.println(Lokalni_ip[1]);
    byte pingAddr[] = {10, 253, 0, 1};

    bool pingRet; // pingRet stores the ping() success (true/false)
    //startPing();
    ICMPPing ping(pingSocket);
    pingRet = ping(4, pingAddr, buffer, &ping_time, &TTL);
    delay(250);
    //endPing();


    if (pingRet && ping_time < 2000 && ping_time >= 0 ) {
      Serial.println(buffer);
      //lcd.print(buffer);
      lcd.setCursor(0, 1);
      sprintf(buffer, "Ping: %d.%d.%d.%d", pingAddr[0], pingAddr[1], pingAddr[2], pingAddr[3]);
      Serial.println(buffer);
      lcd.print(buffer);

      lcd.setCursor(0, 2);
      lcd.print("Odzivni cas = ");
      lcd.print(ping_time);
      lcd.print("ms");
      ping_time = 0;
      dela = !dela;

      if (dela == 1) {
        lcd.setCursor(8, 3);
        lcd.print(" .");
      }
      if (dela == 0) {
        lcd.setCursor(8, 3);
        lcd.print(". ");
      }

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }
    }
    else {

      lcd.setCursor(0, 2);
      lcd.print("...Ping neuspesen...");

      reset = analogRead(A15);
      Serial.println(reset);
      if (reset < 380 || reset > 460) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--------------------");
        lcd.setCursor(0, 1);
        lcd.print("|Kabel ni vkljucen |");
        lcd.setCursor(0, 3);
        lcd.print("--------------------");
        int lala = 0;
        while (reset < 380 || reset > 460) {
          lcd.setCursor(0, 2);
          lcd.print("|    Vkljuci ga!   |");
          reset = analogRead(A15);
        }
        if (reset > 380 && reset < 460 && lala == 0)
        {
          lala++;
          rebootEthShield();


        }
      }

    }
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("1. Ping -> loklani");
  lcd.setCursor(1, 1);
  lcd.print("2. Ping -> google");
  lcd.setCursor(1, 2);
  lcd.print("3. Ping -> Voice");
  lcd.setCursor(1, 3);
  lcd.print("4. Ping -> Video");
}




















void loop()
{

  IPAddress Lokalni_ip = Ethernet.localIP();
  IPAddress Gateway = Ethernet.gatewayIP();


  //GLEDA KER GUMB SI KLIKNU
  buttonState = 0;
  if (digitalRead(YOUR_RESET_BUTTON_PIN)) {
    prev = millis();
    buttonState = 1;
    while ((millis() - prev) <= DURATION_IN_MILLIS) {
      if (!(digitalRead(YOUR_RESET_BUTTON_PIN))) {
        buttonState = 2;
        break;
      }
    }
  }
  //GLEDA KER GUMB SI KLIKNU


  //GLEDA KER GUMB SI KLIKNU2
  buttonState2 = 0;
  if (digitalRead(YOUR_RESET_BUTTON_PIN2)) {
    prev2 = millis();
    buttonState2 = 1;
    while ((millis() - prev2) <= DURATION_IN_MILLIS2) {
      if (!(digitalRead(YOUR_RESET_BUTTON_PIN2))) {
        delay(100);
        buttonState2 = 2;
        break;
      }
    }
  }
  //GLEDA KER GUMB SI KLIKNU2


  //GLEDA KER GUMB SI KLIKNU3
  buttonState3 = 0;
  if (digitalRead(YOUR_RESET_BUTTON_PIN3)) {
    prev3 = millis();
    buttonState3 = 1;
    while ((millis() - prev3) <= DURATION_IN_MILLIS3) {
      if (!(digitalRead(YOUR_RESET_BUTTON_PIN3))) {
        buttonState3 = 2;
        break;
      }
    }
  }
  //GLEDA KER GUMB SI KLIKNU3


  //GLEDA KER GUMB SI KLIKNU4
  buttonState4 = 0;
  if (digitalRead(YOUR_RESET_BUTTON_PIN4)) {
    prev4 = millis();
    buttonState4 = 1;
    while ((millis() - prev4) <= DURATION_IN_MILLIS4) {
      if (!(digitalRead(YOUR_RESET_BUTTON_PIN4))) {
        buttonState4 = 2;
        break;
      }
    }
  }
  //GLEDA KER GUMB SI KLIKNU4



  reset = analogRead(A15);
  Serial.println(reset);
  if (reset < 380 || reset > 460) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("--------------------");
    lcd.setCursor(0, 1);
    lcd.print("|Kabel ni vkljucen |");
    lcd.setCursor(0, 3);
    lcd.print("--------------------");
    int lala = 0;
    while (reset < 380 || reset > 460) {
      lcd.setCursor(0, 2);
      lcd.print("|    Vkljuci ga!   |");
      Serial.println(reset);
      reset = analogRead(A15);
    }
    if (reset > 380 && reset < 460 && lala == 0)
    {
      lala++;
      rebootEthShield();
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");

    }
  }


  //PREVRERJA!
  if (!buttonState)
  {
    //NIČ NI PRITISNENO;
  }
  else if (buttonState == 1)
  {
    if (Gateway[0] == 192) {
      LocalDolg();
      delay(1000);
    }
    else
    { lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }


  }
  else if (buttonState == 2)
  { Serial.println(Gateway[0]);
    if (Gateway[0] == 192)
    {
      localKratek();
    }
    else
    { lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }




  //PREVRERJA2!
  if (!buttonState2)
  {
    //NIČ NI PRITISNENO;
  }
  else if (buttonState2 == 1)
  { if (Gateway[0] == 192) {
      googleDolgi();
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }
  else if (buttonState2 == 2)
  {
    if (Gateway[0] == 192) {
      googleKratek();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }


  //PREVRERJA3!
  if (!buttonState3)
  {
    //NIČ NI PRITISNENO;
  }
  else if (buttonState3 == 1)
  {
    if (Gateway[0] == 10) {
      voiceDolg();
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }
  else if (buttonState3 == 2)
  {
    if (Gateway[0] == 10) {
      voiceKratek();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }

  //PREVRERJA4!
  if (!buttonState4)
  {
    //NIČ NI PRITISNENO;
  }
  else if (buttonState4 == 1)
  {
    if (Gateway[0] == 10) {
      videoDolg();
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }
  else if (buttonState4 == 2)
  {
    if (Gateway[0] == 10) {
      videoKratek();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      lcd.setCursor(0, 2);
      lcd.print("|                  |");
      lcd.setCursor(0, 1);
      lcd.print("|  Napacna mreza!  |");
      delay(1500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("1. Ping -> loklani");
      lcd.setCursor(1, 1);
      lcd.print("2. Ping -> google");
      lcd.setCursor(1, 2);
      lcd.print("3. Ping -> Voice");
      lcd.setCursor(1, 3);
      lcd.print("4. Ping -> Video");
    }
  }
}
