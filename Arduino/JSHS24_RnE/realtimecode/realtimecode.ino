#include <DS3231.h>
#include <Wire.h>
#include <EEPROM.h>
//나의 아이디어: 0~3에 initT 저장. 4~35에 xhistory[8] 저장. x는 initT기준 0초이후, 1,2,3,4,5,6,7 초이후에 대한 x 값들을 저장해둠.
//initT가 현재시간보다 8초 초과 차이나면, initT에 1초 더해가면서 x -1칸씩 shift, until initT == 현재시간. 
/* (ex. 현재시간 9초, initT: 1초였다가, 현재 init 1+6까지 끌어올린 상태에서
 *  x=[0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8]이라고 함. 그러면 t=6에서 t=7로 t++가 돼서 현재 9, init+t=8
 *  x=[0.2,0.3,0.4,0.5,0.6,0.7,0.8, 0.9]가 됨. t++, 현재9, init+t=9. t는 8이 되고, 
 *  x=[0.3,0.4,0.5,0.6,0.7,0.8, 0.9, 1.0]가 됨. t++, 현재9, init+t=10. t는 9이 되고, 프로그렘종료. 이후 t=k였기에 init+k=init.
 * 
 */

RTClib myRTC;
long int initT;
double x[8];
int Reset = 12;
char buf [64];


void setup() {
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
  if ((now.unixtime()-initT)>=8){
    sprintf(buf, "Current time: %ld , initT: %ld \n",now.unixtime(),initT);
    Serial.println(buf);
    for(t=1; t<101 && (initT+t)<=now.unixtime(); t++){
      for(int i=0; i<7; i++){
          x[i]=x[i+1];
      }
      x[7]=3.998*x[6]*(1-x[6]);
      if(t%20==1){
        Serial.println(t);
        Serial.println("x[i] is:");
        for(int i=0; i<8; i++){
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
  DateTime now = myRTC.now();
  sprintf(buf, "(Main)Current time: %ld , initT: %ld \n",now.unixtime(),initT);
  Serial.println(buf);
  for(int i=1; i<101; i++){
    DateTime now = myRTC.now();
    int a=now.unixtime()-initT;
    String sendval_s=String(x[a], 4);
    Serial.println("(M)x[i] is:");
    for(int i=0; i<8; i++){
      Serial.println(String(x[i],6));
    }
    Serial.println(now.unixtime());`
    Serial.println(initT);
    Serial.println("a is:");
    Serial.println(a);
    Serial.println("sendval is:");
    Serial.println(sendval_s);
    Serial.print("\n");
    for(int t=1; t<=(a+1); t++){
      for(int i=0; i<7; i++){
          x[i]=x[i+1];
      }
      x[7]=3.998*x[6]*(1-x[6]);
      initT++;
    }
    
    delay(1000-a);
  }
 
  EEPROM.put(0,(long int)initT);
  EEPROM.put(4,x);
  digitalWrite(Reset, LOW);
}
