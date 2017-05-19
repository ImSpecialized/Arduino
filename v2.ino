#include <DFPlayer_Mini_Mp3.h>

#include <EEPROM.h>

#include <LiquidCrystal.h> 

#include <SPI.h>
#include <MFRC522.h>

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0); // (RS, E, DB4, DB5, DB6, DB7)

//RTOS
unsigned long tim, previousMillis_10, previousMillis_100, previousMillis_1000, previousMillis_10000 ; 

int mod = 0, room = 0;

int brightmax, brightmin, vol, scrolldel = 500, scrollt, tplay;

int key = -1;

int menu, menupos;

#define ledon 10

#define SS_PIN 9
#define RST_PIN A6

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
byte nuidPICC[4];

byte memoroom[4];

void setup(){ 

 pinMode(2, INPUT_PULLUP);
 pinMode(3, INPUT_PULLUP);
 pinMode(4, INPUT_PULLUP);
 pinMode(5, INPUT_PULLUP);

 pinMode(6, OUTPUT);
 pinMode(7, OUTPUT);
 pinMode(8, OUTPUT);

  //value = EEPROM.read(address);
  brightmin = EEPROM.read(0);
  brightmax = EEPROM.read(1);
  vol = EEPROM.read(2);
  scrollt = EEPROM.read(3);
  tplay = EEPROM.read(4);

 memoroom[0]  = EEPROM.read(10);
 memoroom[1]  = EEPROM.read(11);
 memoroom[2]  = EEPROM.read(12);
 memoroom[3]  = EEPROM.read(13);


  
  lcd.begin(16, 2);                  


  Serial.begin (9600);
  mp3_set_serial (Serial); 

  mp3_set_volume (vol);
  analogWrite(ledon, brightmin);

   SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
}

void loop(){
tim = millis();

   if (tim - previousMillis_10 >= 10)
   {
       previousMillis_10 = tim;

       

   }
      
   if (tim - previousMillis_100 >= 100)
   {
       previousMillis_100 = tim;
       button(); 

        switch (mod)
       {

        case 0:
        lcd0();
        break;

        case 1:
        lcd1();
        roomset();
        break;

        case 5:
        menuset();
        break;

       }
    
    switch (key) 
    {

    case 13:
    analogWrite(ledon, brightmax);
    mod = 1;
    key = 0;
    break;

    case 15:
    if (mod == 1)
    { 
     scrolltext();
    }
      break; 

    } 

     }
  
   if (tim - previousMillis_1000 >= 1000)
   {
       previousMillis_1000 = tim;

       if (menu == 5){
         mod = 5;
         menu = 0;

       }
scan();


if (nuidPICC[0] == memoroom[0]) {
 
mp3_play (); 
lcd3();

}

if (nuidPICC[0] != memoroom[0]) {
 
mp3_stop ();

}


   }

   if (tim - previousMillis_10000 >= 10000)
   {
       previousMillis_10000 = tim;

       
   }

}


void button(){
  label:
digitalWrite(6, LOW);
digitalWrite(7, HIGH);
digitalWrite(8, HIGH);

if (digitalRead(2) == LOW){
  key = 1;
  goto label;
}

if (digitalRead(3) == LOW){
  key = 4;
}

if (digitalRead(4) == LOW){
  key = 7;
  goto label;
}

if (digitalRead(5) == LOW){
  key = 13;
  goto label;
}

digitalWrite(6, HIGH);
digitalWrite(7, LOW);
digitalWrite(8, HIGH);

if (digitalRead(2) == LOW){
  key = 2;
  goto label;
}

if (digitalRead(3) == LOW){
  key = 5;
}

if (digitalRead(4) == LOW){
  key = 8;
  goto label;
}

if (digitalRead(5) == LOW){
  key = 0;
  goto label;
}

digitalWrite(6, HIGH);
digitalWrite(7, HIGH);
digitalWrite(8, LOW);

if (digitalRead(2) == LOW){
  key = 3;
  goto label;
}

if (digitalRead(3) == LOW){
  key = 6;
}

if (digitalRead(4) == LOW){
  key = 9;
  goto label;
}

if (digitalRead(5) == LOW){
  key = 15;
  goto label;
}


}




void lcd0(){
  lcd.clear();
  lcd.setCursor(1, 0);                         
  lcd.print("Welcome To KPI");        
}

void lcd1(){
  lcd.clear();
  lcd.setCursor(1, 0);                         
  lcd.print("Welcome To KPI");  
  lcd.setCursor(0, 1);              
  lcd.print("Room:");        
  lcd.setCursor(11, 1);              
  lcd.print(room);        
}

void lcd3(){
  analogWrite(ledon, brightmax);
  lcd.clear();
  lcd.setCursor(1, 0);                         
  lcd.print("Welcome HOME");  
  delay(tplay*1000);
  mp3_stop ();
  analogWrite(ledon, brightmin);
  nuidPICC[0] = 0;
}


void scrolltext(){


  lcd.clear();
  lcd.setCursor(0, 0);                         
  lcd.print("Please Wait");  

  mp3_play (); 

for (int i=0; i<room; i++)
      {
          mp3_next (); 
      }

for (int i=0; i<scrollt; i++)
      {
        lcd.scrollDisplayRight();
        delay(scrolldel);
      }

     mp3_stop ();
     room = 0;
     mod = 0; 
     key = 0;
     analogWrite(ledon, brightmin);

}


void roomset(){

  if (key == 1)  if (mod == 0)  {
  key = 0;
  menu ++;
  }

if (key == 13){
	mod = 0; 
	key = 0;
    analogWrite(ledon, brightmin);
    menu ++;
	goto end;
}

if (key == 15){
	goto end;
}

if (room > 0) if (key >= 0){
	room = key+(room*10);
	key = -1;
}

if (room < 1) if (key > 0){
	room = key;
	key = -1;
}

end:

if (room > 100){
	room = 0;
}


}


void menuset(){

	 if (key == 2){
         	menupos ++;
         	key = 0;
         	delay(500);
         }

          if (key == 5){
         	menupos --;
         	key = 0;
         	delay(500);
         }

  lcd.clear();
  lcd.setCursor(1, 0);                         
  lcd.print("Setting");
  lcd.setCursor(12, 0);                         
  lcd.print(menupos);

menupos = constrain(menupos, 0, 6);

switch (menupos)
    {

        case 0:
         lcd.setCursor(0, 1);                         
         lcd.print("Admin MODE");

        break;

        case 1:
         lcd.setCursor(0, 1);                         
         lcd.print("Brig max");
         lcd.setCursor(12, 1);                         
         lcd.print(brightmax);

         if (key == 3){
         	brightmax ++;
         	key = 0;
         }

          if (key == 1){
         	brightmax --;
         	key = 0;
         }
        brightmax = constrain(brightmax, 0, 255);
        analogWrite(ledon, brightmax);
        break;

        case 2:
         lcd.setCursor(0, 1);                         
         lcd.print("Brig min");
         lcd.setCursor(12, 1);                         
         lcd.print(brightmin);

         if (key == 3){
         	brightmin ++;
         	key = 0;
         }

          if (key == 1){
         	brightmin --;
        	key = 0;
         }	
            brightmin = constrain(brightmin, 0, 255);
       	    analogWrite(ledon, brightmin);
        break;  


        case 3:
         lcd.setCursor(0, 1);                         
         lcd.print("Vol");
         lcd.setCursor(12, 1);                         
         lcd.print(vol);

         if (key == 3){
         	vol ++;
         	key = 0;
         }

          if (key == 1){
         	vol --;
        	key = 0;
         }	
            vol = constrain(vol, 0, 20);
       	    mp3_set_volume (vol);
        break;  

         case 4:
         lcd.setCursor(0, 1);                         
         lcd.print("scrollt");
         lcd.setCursor(12, 1);                         
         lcd.print(scrollt);

         if (key == 3){
         	scrollt ++;
         	key = 0;
         }

          if (key == 1){
         	scrollt --;
        	key = 0;
         }	
            scrollt = constrain(scrollt, 0, 200);
        break;  

        case 5:
         lcd.setCursor(0, 1);                         
         lcd.print("tplay");
         lcd.setCursor(12, 1);                         
         lcd.print(tplay);

         if (key == 3){
          tplay ++;
          key = 0;
         }

          if (key == 1){
          tplay --;
          key = 0;
         }  
            tplay = constrain(tplay, 0, 200);
        break;  

        case 6:
         lcd.setCursor(0, 1);                         
         lcd.print("Save card");

         if (key == 3){
          EEPROM.write(10, nuidPICC[0]);
          key = 0;
          lcd.setCursor(12, 1);                         
         lcd.print("OK");
         delay(500);

         }
        break;  


    }


if (key == 8){

	    EEPROM.write(0, brightmin);
      EEPROM.write(1, brightmax);
      EEPROM.write(2, vol);
      EEPROM.write(3, scrollt);
      EEPROM.write(4, tplay);



        memoroom[0]  = EEPROM.read(10);
        nuidPICC[0] = 0;

      key = 0;
      menupos = 0;
}


}

void scan() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

 
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
   
  }
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


