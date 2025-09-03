#include <NewPing.h>
#include <SharpIR.h>
#include <EEPROM.h>
#include <Wire.h>
#include "SSD1306AsciiAvrI2c.h"

#define halt_delay 50
#define tSpd 140

#define RPATT (!dr[0]&&!dr[1]&& (dr[3]||dr[4]||dr[5]) && (dr[9]||dr[10]) )
#define LPATT ( (dr[0]||dr[1]) && (dr[5]||dr[6]||dr[7]) &&!dr[9]&&!dr[10])
#define PPATT ( !dr[2] && (dr[4]||dr[5]||dr[6]) && !dr[8] )
#define YPATT ( (dr[0]||dr[1]) && (!dr[4]&&!dr[5]&&!dr[6]) && (dr[9]||dr[10]) )
#define ILPATT ( patt5=="11011"||patt5=="10011"||patt5=="11001" )

#define KP 1.4
#define KI 0.0001

int node=0, rCount=0;
int max_spd=200, base_spd=200;
int wallDist=16, objDist=12;

#define NUM_SENSORS 11
#define lpwm 3
#define rpwm 2
#define lmf 6
#define lmb 7
#define rmf 8
#define rmb 5
#define DEBUGGING !digitalRead(12)
#define ad_sminb 0
#define ad_smaxb 11
#define ad_sminw 22
#define ad_smaxw 33
#define ad_wallDist 44
#define ad_objDist 45
#define ad_baseSpd 46

int sminb[NUM_SENSORS]={1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023},smaxb[NUM_SENSORS]={0,0,0,0,0,0,0,0,0,0,0};
int sminw[NUM_SENSORS]={1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023},smaxw[NUM_SENSORS]={0,0,0,0,0,0,0,0,0,0,0};
int s[NUM_SENSORS]={A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15}, dr[NUM_SENSORS], ar[NUM_SENSORS];
int KD;
NewPing sonarL(42,42,400), sonarF(46,46,400), sonarR(44,44,400);
SharpIR irF( SharpIR::GP2Y0A21YK0F, A1);
SSD1306AsciiAvrI2c oled;
String patt, lastPatt = "", patt3, lastPatt3 = "010", patt5, lastPatt5 = "00100", patt7, lastPatt7 = "0001000", patt9, lastPatt9 = "000010000";
long pos,lastPos=0;
bool obFound=false;
char line='B', hand='R', dir='S';

void oled_print(int,int,String,int=1,bool=false);void oled_println(int,int,String,int=1,bool=false);
void oled_printBar(long*,int,int,int);
void indicator();
void calibrate_line(char,bool=false);void calibrate_wall();void calibrate_obj();
bool hasWall(long);char detectWall(char);void followWall(char,float);void followWallBoth();void checkSonar();
bool hasObject(long);void avoidObject(char,float);bool isRamp(long);void avoidObjectBlind(char, float, int=0);
void readLine();void extractPatterns();String getPattern(int=0);long getPos(int);void checkIR(bool=true);
void setMotors(int,int);void pid(bool=false);
void turn(char);void turnDeg(char,int=0,bool=false);void halt();void brake();void inch();
void menu();void settings();void setCheckpoint();

void setup() {
  
  pinMode(lmf,OUTPUT);pinMode(lmb,OUTPUT);pinMode(rmf,OUTPUT);pinMode(rmb,OUTPUT);pinMode(lpwm,OUTPUT);pinMode(rpwm,OUTPUT);
  pinMode(40,OUTPUT);pinMode(50,OUTPUT);pinMode(9,OUTPUT);pinMode(10,OUTPUT);
  digitalWrite(40,LOW);digitalWrite(50,HIGH);digitalWrite(9,HIGH);digitalWrite(10,HIGH);
  Serial.begin(9600);
  oled.begin(&SH1106_128x64, 0x3C);oled.setFont(System5x7);
  
  menu();
  setCheckpoint();

//  wallDist=EEPROM[ad_wallDist];
  base_spd=EEPROM.read(ad_baseSpd);
  for(int i=0; i<NUM_SENSORS; i++){
    sminb[i]=map(EEPROM[ad_sminb+i],0,255,0,1023);smaxb[i]=map(EEPROM[ad_smaxb+i],0,255,0,1023);
    sminw[i]=map(EEPROM[ad_sminw+i],0,255,0,1023);smaxw[i]=map(EEPROM[ad_smaxw+i],0,255,0,1023);
  }
  
  oled.clear();
  delay(500);
}

void lop(){
  setMotors(base_spd,base_spd);
//  checkIR();
//  turnDeg('R',135);delay(1000);
//  followWall('R',0.3);
}

void loop() {
  int tCount = 0, count = 0, gap_loop_count = 0;
  bool flagS = false, flagR = true, flagIL = false, flagObj = false;
  bool wallPassed = false, wlinePassed = false;
  long prevMillis = 0;
  
  while(1){
    if(DEBUGGING){
      oled_print(0, 0, String(hand) + " " + String(line) + " " + String(dir) + " " + String(count));
      oled_print(0,1,"sonarF="+String(sonarF.ping_cm())+"  ");
    }

    flagObj=hasObject(16);
    readLine();
    extractPatterns();
    pos=getPos(7);
    indicator();
    if (dr[1] && !dr[9]) {
      dir = 'L';
      count=0;
    } else if (!dr[1] && dr[9]) {
      dir = 'R';
      count=0;
    } else {
      if(count>30){
//        if(dir!='S'){halt();oled.clear();oled.print("reset dir");delay(3000);}
        dir='S';
      }else count++;
    }
    
    if(DEBUGGING)continue;

    if(node==0){
      for(int i=0;i<260;i++){readLine();pos=getPos(7);pid(1);}
      halt();delay(2000);
      node++;
    }else if(node==2){
      wallPassed=true;
      flagIL=true;
      node++;
    }

    if(patt=="11111111111"){ // black box
      halt();
      oled.clear();oled_print(2,0,"MISSION\n COMPLETE! ");
      while(!DEBUGGING); 
      oled.clear();oled.print(" Restarting...\nPlace the Robot");
      while(DEBUGGING);
      dir='S';
      oled.clear();
      delay(1000);continue;
    }

    

    if(flagR && dr[10]){
      rCount++;
      if(rCount==8){
        rCount++;
        flagR=false;
      }
      halt();oled.clear();oled.print(String(hand)+" int");delay(50);
      turnDeg('R',90,true);
      for(int i=0;i<30;i++){readLine();pos=getPos(7);pid(1);}
      continue;
    }

    if(!wallPassed && patt=="00000000000"){
      followWall('R',0.4);
      halt();oled.clear();oled.print("wallpassed");delay(2000);
      wallPassed=true;
      flagIL=true;
    }
    
    if (flagIL && ILPATT) { // inverse line or Y
      inch();
      patt5=getPattern(5);
      if(ILPATT){ // IL
        line=='B' ? line='W' : line='B';
        halt();oled.clear();oled.print("inverse line");delay(1000);
        for(int i=0;i<700;i++){readLine();pos=getPos(7);pid(1);}
        flagS=true;
      }
      continue;
    }
    
    if (flagS && patt9=="000000000" && lastPatt7!="1000000" && lastPatt7!="0000001") {
      if(dir=='S'){
      }else{ // angle
        halt();oled.clear();oled.print(dir);delay(50);
        turn(dir);
        for(int i=0;i<500;i++){readLine();pos=getPos(7);pid(1);}
      }
      dir='S';continue;
    }

    max_spd=base_spd;
    pid(1);
    

    lastPatt = patt;
    lastPatt3 = patt3;
    lastPatt5 = patt5;
    lastPatt7 = patt7;
    lastPatt9 = patt9;
  } // endwhile;
}
