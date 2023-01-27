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
// char str[50]; 
// char *sArr[5]; 
unsigned long pre_time = 0;
unsigned long cur_time = 0;
LiquidCrystal_I2C LCD(0x3f, 16,2);
volatile byte state = 1;
volatile byte Cstate = 1;
volatile byte Wstate = 1;
volatile byte LCDstate = 1;

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
  pinMode(2, INPUT);
  attachInterrupt(0, isr, FALLING);
  pinMode(10,OUTPUT);
  digitalWrite(10,1);
}

void loop() {
  cur_time = millis();
  int index = 0, i=0;
  char *ptr = NULL;
  
 if(cur_time - pre_time >= 1000)
 {
   slave.begin(9600);
   char str[50]; 
   char *sArr[5]; 
   if(state == 1){
    LCD.clear();
    LCD.setDelay(1000, 100);
   while(slave.available()){
    str[index++]= slave.read();
   }

    if(str != NULL)
    {
     
      //Serial.print("str : ");
      //Serial.println(str);
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
      LCD.setCursor(0,0);
      LCD.print(" Welcome to");
      LCD.setCursor(0,1);
      LCD.print("    Smart Home!");
      
    if(irrecv.decode(&results))
    {
      switch(results.value){
        case 16724175:
        Cstate = !Cstate;
        break;
        case 0xFF18E7:
        Wstate = !Wstate;
        break;
        case 0xFF42BD:
        LCD.clear();
        LCD.setCursor(0,0);
        LCD.print("Temp =  ");
        LCD.print(c);
        LCD.setCursor(0,1);
        LCD.print("Humid =  ");
        LCD.print(b);
        delay(3000);
        break;
        case 0xFF4AB5:
        LCD.clear();
        LCD.setCursor(0,0);
        LCD.print("Light =  ");
        LCD.print(a);
        if(a>110){
         LCD.setCursor(0,1);
         LCD.print("Curtain Open!");
        }
        delay(3000);
        break;
        case 0xFF52AD:
        LCDstate = !LCDstate;
        break;
        default:
        break;
      }
    Serial.println(results.value);
    irrecv.resume();
    }
    if(Cstate == 1){
      curtain.write(179);
      delay(50);
    }else{
      curtain.write(0);
      delay(50);
    }
    if(Wstate == 1){
      window.write(120);
      delay(50);
    }else{
      window.write(0);
      delay(50);
    }
    if(LCDstate == 1){
      LCD.backlight();
      LCD.display();
    }else{
      LCD.noBacklight();
      LCD.noDisplay();
    }
     slave.write(20);      
    //  Serial.print("light : "); 
    //  Serial.println(a);
    //  Serial.print("humid : ");
    //  Serial.println(b);
    //  Serial.print("temp : ");
    //  Serial.println(c);
     }
    }else{
     slave.end();
     LCD.clear();
     LCD.setCursor(0, 0);
     LCD.print("not connected");
    }
    pre_time = cur_time;
   
  }
 
}

void isr(){
  if(cur_time - pre_time >=500){ 
   
    digitalWrite(10,!state);
    Serial.println("interrupt");
    state = !state;        
    pre_time = cur_time;
  
}
}

//  void setColor(int red, int green, int blue)
// {
//   analogWrite(12, red);
//   analogWrite(11, green);
//   analogWrite(10, blue); 
// }

