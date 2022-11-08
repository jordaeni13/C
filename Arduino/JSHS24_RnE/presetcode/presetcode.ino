#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>

RTClib myRTC;
long int current_T;
long int initT;
double x[8];
char buf [32];

void printlist(double before_move[], int len){
    for(int i=0; i<len; i++){
      Serial.println(String(x[i],6));
    }
}

void setup (void)
{
  x[0]=0.384235;
  for(int i=1; i<8; i++){
    x[i]=3.998*x[i-1]*(1-x[i-1]);
  }
  Wire.begin();
  Serial.begin(57600);
  DateTime now = myRTC.now();
  current_T=(long int)now.unixtime();
  EEPROM.put(0,(long int)current_T);
  EEPROM.put(4,x);
  printlist(x,8);
  EEPROM.get(0,initT);
  Serial.println(initT);
  Serial.println(current_T);
}

void loop (void)
{
  //DateTime now = myRTC.now();
  //current_T=now.unixtime();
  //Serial.println(current_T);
}
