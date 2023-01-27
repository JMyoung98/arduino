#include <SoftwareSerial.h>
#include <Servo.h>


Servo curtain; 
Servo window;
SoftwareSerial slave(6,7);
char str[50]; 
char *sArr[5]; 
unsigned long pre_time = 0;
unsigned long cur_time = 0;


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  slave.begin(9600);
  curtain.attach(8);
  window.attach(9);
  pre_time = millis();
}

void loop() {
  cur_time = millis();
  int index = 0, i=0;
  char *ptr = NULL;
if(cur_time - pre_time>=1000 )  {
  Serial.println(cur_time-pre_time);
  pre_time = cur_time ;
 while(slave.available()){
  str[index++]= slave.read();
 }
  str[index] = '\0';
  ptr = strtok(str,"#");
  while(ptr != NULL){
    sArr[i++] = ptr;
    ptr = strtok(NULL, "#"); 
  }  
 
  int a= atoi(sArr[0]);
  int b= atoi(sArr[1]);
  int c= atoi(sArr[2]);
  
  Serial.print("light : "); 
  Serial.println(a);
  Serial.print("humid : ");
  Serial.println(b);
  Serial.print("temp : ");
  Serial.println(c);
  if(a>150){
    curtain.write(179);
    //setColor(0,0,0);
    delay(20);
  }else{
    curtain.write(0);
      //setColor(130,1,255);//파랑,초록,빨강
    delay(20);
  }
  
  if(b>24 && c<30){
    window.write(179);
    delay(20);
  }
  else{
   window.write(0);  
   delay(20);
 }
}
}

//  void setColor(int red, int green, int blue)
// {
//   analogWrite(12, red);
//   analogWrite(11, green);
//   analogWrite(10, blue); 
// }

