//slave
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#define IRsenser A0

Servo curtain; 
Servo window;
SoftwareSerial slave(6,7);
char str[50]; 
char *sArr[5]; 
unsigned long pre_time = 0;
unsigned long cur_time = 0;
LiquidCrystal_I2C LCD(0x3f, 16,2);

IRrecv irrecv(IRsenser);
decode_results results;


void setup() {
  // put your setup code here, to run once:
  LCD.init();
  LCD.backlight();
  Serial.begin(9600);
  slave.begin(9600);
  curtain.attach(8);
  window.attach(4);
  pre_time = millis();
  irrecv.enableIRIn();
}

void loop() {
  cur_time = millis();
  int index = 0, i=0;
  char *ptr = NULL;
 if(cur_time - pre_time >= 1000)
 {
   while(slave.available()){
    str[index++]= slave.read();
   }
   
    if(str != -1)
    {
     str[index] = '\0';
     ptr = strtok(str,"#");
     while(ptr != NULL)
     {
       sArr[i++] = ptr;
       ptr = strtok(NULL, "#"); 
     }  
 
     int a= atoi(sArr[0]);
     int b= atoi(sArr[1]);
     int c= atoi(sArr[2]);
  
    //  Serial.print("light : "); 
    //  Serial.println(a);
    //  Serial.print("humid : ");
    //  Serial.println(b);
    //  Serial.print("temp : ");
    //  Serial.println(c);
      if(a>150)
      {
        curtain.write(179);
        //setColor(0,0,0);
        delay(20);
      }else
      {
        curtain.write(0);
        //setColor(130,1,255);//파랑,초록,빨강
        delay(20);
      }
  
      if(b>24 && c<30)
      {
        window.write(179);
        delay(20);
      }else
      {
        window.write(0);  
        delay(20);
      }

      if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
      }
      LCD.setCursor(0,0);
      LCD.print("Temp =  ");
      LCD.print(c);
      LCD.setCursor(0,1);
      LCD.print("Humid =  ");
      LCD.print(b);
      slave.write('A');      
    }
    pre_time = cur_time;
   
  }
 
}


//  void setColor(int red, int green, int blue)
// {
//   analogWrite(12, red);
//   analogWrite(11, green);
//   analogWrite(10, blue); 
// }

