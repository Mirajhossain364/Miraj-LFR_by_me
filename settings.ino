void setCheckpoint()
{
  int s_delay=60, anim_delay=20;
  while(DEBUGGING){oled.clear();oled.setCursor(0,0);oled.print("  Press the button   ");delay(250);oled.clear();delay(250);}
  oled.clear();
  while(1){
    int i=0;
    oled.setCursor(0,0);oled.print("     Start Zone      ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();  node=0;  return;}delay(10);}
    oled.setCursor(0,0);oled.print("    Checkpoint 1     ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();  node=1;  return;}delay(10);}
    oled.setCursor(0,0);oled.print("    Checkpoint 2     ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();  node=1; rCount=4;  return;}delay(10);}
    oled.setCursor(0,0);oled.print("    Checkpoint 3     ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();  node=1; rCount=8;  return;}delay(10);}
    oled.setCursor(0,0);oled.print("    Checkpoint 4     ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();  node=2;;return;}delay(10);}
  }
}
void menu()
{
  int s_delay=60, anim_delay=20;
  while(DEBUGGING){oled.clear();oled.setCursor(0,0);oled.print("  Press the button   ");delay(250);oled.clear();delay(250);}
  oled.clear();
  while(1){
    int i=0;
    oled.setCursor(0,0);oled.print("      Settings       ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();settings();break;}delay(10);}
    oled.setCursor(0,0);oled.print("        Run          ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
      break;}delay(10);}
    if(i<s_delay){return;}
  }
}
void settings()
{
  int s_delay=60, anim_delay=20, op_delay=60;
  while(1){
    int i=0;
    oled.clear();oled.setCursor(0,0);oled.print("      Set Speed      ");
    for(i=0; i<s_delay; i++){
      if(DEBUGGING){// SPD
        for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
        while(1){
          int k=0,n=4, op_count;
          int op[]={150,180,200,250};
          oled.setCursor(0,7);oled.print("   Current = "+String(EEPROM.read(ad_baseSpd))+"     ");
          for(op_count=0;op_count<n;op_count++){
            oled.setCursor(0,0);oled.print("       Set = "+String(op[op_count])+"     ");
            for(k=0; k<op_delay; k++){if(DEBUGGING){base_spd=op[op_count];EEPROM.update(ad_baseSpd,base_spd);break;}delay(10);}
            if(k<op_delay){oled.setCursor(0,1);oled.print("     ** SAVED **     ");delay(500);break;}
          }if(op_count<n)break;
        }while(DEBUGGING);continue;
      }delay(10);
    }
    
    oled.clear();oled.setCursor(0,0);oled.print("      Set Hand       ");
    for(i=0; i<s_delay; i++){
      if(DEBUGGING){// hand
        for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
        while(1){
          int k=0,n=2, op_count;
          char op[]={'R','L'};
          for(op_count=0;op_count<n;op_count++){
            oled.setCursor(0,0);oled.print("      Hand = "+String(op[op_count])+"       ");
            for(k=0; k<op_delay; k++){if(DEBUGGING){hand=op[op_count];break;}delay(10);}
            if(k<op_delay){oled.setCursor(0,1);oled.print("     ** SAVED **     ");delay(500);break;}
          }if(op_count<n)break;
        }while(DEBUGGING);continue;
      }delay(10);
    }
    
//    oled.clear();oled.setCursor(0,0);oled.print("      Set Line       ");
//    for(i=0; i<s_delay; i++){
//      if(DEBUGGING){// line
//        for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
//        while(1){
//          int k=0,n=2, op_count;
//          char op[]={'B','W'};
//          for(op_count=0;op_count<n;op_count++){
//            oled.setCursor(0,0);oled.print("      Line = "+String(op[op_count])+"       ");
//            for(k=0; k<op_delay; k++){if(DEBUGGING){max_spd=op[op_count];break;}delay(10);}
//            if(k<op_delay){oled.setCursor(0,1);oled.print("     ** SAVED **     ");delay(500);break;}
//          }if(op_count<n)break;
//        }while(DEBUGGING);continue;
//      }delay(10);
//    }

    oled.clear();oled.setCursor(0,0);oled.print("   Calibrate LineB    ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
      calibrate_line('B');while(DEBUGGING);break;}delay(10);}
      
    oled.clear();oled.setCursor(0,0);oled.print("  Re-Calibrate LineB  ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
      calibrate_line('B',true);while(DEBUGGING);break;}delay(10);}

    oled.clear();oled.setCursor(0,0);oled.print("   Calibrate LineW    ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
      calibrate_line('W');while(DEBUGGING);break;}delay(10);}
  
//    oled.clear();oled.setCursor(0,0);oled.print("   Calibrate Wall    ");
//    for(i=0; i<s_delay; i++){if(DEBUGGING){
//      for(int j=0;j<=15;j++){oled.setCursor(j,1);oled.print(">");delay(anim_delay);}oled.clear();
//      calibrate_wall();while(DEBUGGING);break;}delay(10);}
  
    oled.clear();oled.setCursor(0,0);oled.print("        Back         ");
    for(i=0; i<s_delay; i++){if(DEBUGGING){
      for(int j=15;j>=0;j--){oled.setCursor(j,1);oled.print("<");delay(anim_delay);}oled.clear();
      while(DEBUGGING);return;}delay(10);}
  }
}
