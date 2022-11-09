#include <DS3231.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//나의 아이디어: 0~3에 initT 저장. 4~39에 4초이전부터 4초이후(-4-3-2-1,0,1,2,3,4) 초에 대한 x값 저장 
//initT가 현재시간보다 4초 초과 차이나면, initT에 1초 더해가면서 x -1칸씩 shift, until initT == 현재시간. 

RF24 radio(9, 10);
const byte address[6] = "00001";
boolean button_state = 0;
RTClib myRTC;
long int initT;
double x[9];
int Reset = 12;
char buf [64];


void setup() {
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address); 
  radio.setPALevel(RF24_PA_MIN);       
  radio.startListening();      
  EEPROM.get(0,initT);
  EEPROM.get(4,x);
  digitalWrite(Reset, HIGH);
  delay(200);
  pinMode(Reset, OUTPUT);
  delay(200);
  Wire.begin();
  Serial.begin(57600);
  delay(600);
  DateTime now = myRTC.now();
  int t=1;
  if ((now.unixtime()-initT)>=5){
    sprintf(buf, "Current time: %ld , initT: %ld \n",now.unixtime(),initT);
    Serial.println(buf);
    for(t=1; t<101 && (initT+t)<=now.unixtime(); t++){
      for(int i=0; i<8; i++){
          x[i]=x[i+1];
      }
      x[8]=3.998*x[7]*(1-x[7]);
      if(t%20==1){
        Serial.println(t);
        Serial.println("x[i] is:");
        for(int i=0; i<9; i++){
          Serial.println(String(x[i],6));
        }
        Serial.print("\n");
      }
    }
    initT=initT+t-1;
    EEPROM.put(0,(long int)initT);
    EEPROM.put(4,x);
    digitalWrite(Reset, LOW);
  }

}

void loop() {
  if (radio.available()){
  char text[8] = "";                 
  radio.read(&text, sizeof(text)); 
  Serial.println(text);
  String receive_val = String(text);
  Serial.println(receive_val);
  DateTime now = myRTC.now();
  sprintf(buf, "(Main)Current time: %ld , initT: %ld \n",now.unixtime(),initT);
  Serial.println(buf);
  for(int i=1; i<101; i++){
    DateTime now = myRTC.now();
    int a=now.unixtime()-initT;
    String sendval_s=String(x[a+4], 4);
    Serial.println("(M)x[i] is:");
    for(int i=0; i<9; i++){
      Serial.println(String(x[i],6));
      if (receive_val.equals(String(x[i],4))){
        //open();
      }
    }
    Serial.println(now.unixtime());
    Serial.println(initT);
    Serial.println("a is:");
    Serial.println(a);
    Serial.println("correct sendval is:");
    Serial.println(sendval_s);
    Serial.print("\n");
    for(int t=1; t<=(a+1); t++){
      for(int i=0; i<8; i++){
          x[i]=x[i+1];
      }
      x[8]=3.998*x[7]*(1-x[7]);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
      initT++;
    }
    
    delay(1000-a);
  }
 
  EEPROM.put(0,(long int)initT);
  EEPROM.put(4,x);
  digitalWrite(Reset, LOW);
  }
}
