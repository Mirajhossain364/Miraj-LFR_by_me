void calibrate_line(char l,bool again)
{
  bool flag=false;
  line=l;
  delay(200);
  oled_print(0,0,"Calibrating Line"+l,true);
  int a[NUM_SENSORS], calSpd = 60, calPeriod = 340;
  if(again){
    for(int i=0; i<NUM_SENSORS; i++){
      sminb[i]=map(EEPROM[ad_sminb+i],0,255,0,1023);smaxb[i]=map(EEPROM[ad_smaxb+i],0,255,0,1023);
      sminw[i]=map(EEPROM[ad_sminw+i],0,255,0,1023);smaxw[i]=map(EEPROM[ad_smaxw+i],0,255,0,1023);
    }
  }
  for (int count = 0; count < calPeriod; count+=1) {
    setMotors(50, -50);
    for (int i = 0; i < NUM_SENSORS; i++) {
      a[i] = analogRead(s[i]);
      if(l=='B'||l=='b'){
        if(a[i] < sminb[i]) sminb[i] = a[i];
        if(a[i] > smaxb[i]) smaxb[i] = a[i];
      }else{
        if(a[i] < sminw[i]) sminw[i] = a[i];
        if(a[i] > smaxw[i]) smaxw[i] = a[i];
      }
    }
    oled.setCursor(count/10,1);oled.write(136);
    delay(10);
  }
  setMotors(0, 0);
  while(!DEBUGGING);while(DEBUGGING){oled.clear();oled.print(String(l));readLine();getPos(11);extractPatterns();}
  while(1){
    int k=0;
    oled_print(0,0,"  Ignore?  ");
    for(k=0; k<100; k++){if(DEBUGGING){flag=false;break;}delay(10);}if(k<100)break;
    oled_print(0,0,"  Update?  ");
    for(k=0; k<100; k++){if(DEBUGGING){flag=true;break;}delay(10);}if(k<100)break;
  }
  for (int i = 0; i < NUM_SENSORS; i++) {
    if(flag){
      if(l=='B'){
        EEPROM.update(ad_sminb+i, map(sminb[i],0,1023,0,255));
        EEPROM.update(ad_smaxb+i, map(smaxb[i],0,1023,0,255));
      }else{
        EEPROM.update(ad_sminw+i, map(sminw[i],0,1023,0,255));
        EEPROM.update(ad_smaxw+i, map(smaxw[i],0,1023,0,255)); 
      }
    }
  }
  oled_print(0,0," * Done! * ");
  delay(1000);
}

void calibrate_wall()
{
  bool flag=false;
  delay(200);
  oled.clear();
  oled_print(10,30,"Calibrating Wall");
  if(detectWall(hand)=='R'){
    for(int i=0; i<16;i++){
      wallDist=sonarR.ping_cm();
      oled.setCursor(i,1);oled.write(8);
      delay(100);
    }
  }else{
    for(int i=0; i<16;i++){
      wallDist=sonarL.ping_cm();
      oled.setCursor(i,1);oled.write(8);
      delay(100);
    }
  }
  while(!DEBUGGING);while(DEBUGGING){oled_print(0,4,String(wallDist)+"cm",1,true);}
  while(1){
    int k=0;
    oled_print(0,2,"  Ignore?  ");
    for(k=0; k<100; k++){if(DEBUGGING){flag=false;break;}delay(10);}if(k<100)break;
    oled_print(0,2,"  Update?  ");
    for(k=0; k<100; k++){if(DEBUGGING){flag=true;break;}delay(10);}if(k<100)break;
  }
  if(flag)EEPROM.update(ad_wallDist,wallDist*10);
  oled_print(0,0," * Saved * ",true);
  delay(1000);
}
