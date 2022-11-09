#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>

RTClib myRTC;
long int current_T;
long int initT;
double x[9];
char buf [32];

void printlist(double before_move[], int len){
    for(int i=0; i<len; i++){
      Serial.println(String(x[i],6));
    }
}

void setup (void)
{
  x[0]=0;
  x[1]=0;
  x[2]=0;
  x[3]=0;
  x[4]=0.384235;
  for(int i=5; i<9; i++){
    x[i]=3.998*x[i-1]*(1-x[i-1]);
  }
  Wire.begin();
  Serial.begin(57600);
  DateTime now = myRTC.now();
  current_T=(long int)now.unixtime();
  //EEPROM.put(0,(long int)1667998800);
  EEPROM.put(0,current_T);
  EEPROM.put(4,x);
  printlist(x,9);
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
