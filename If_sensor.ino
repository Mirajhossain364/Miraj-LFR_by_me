void readLine()
{
  
  for(int i=0; i<NUM_SENSORS; i++){
    if(line=='B'||line=='b'){
      ar[i]=constrain(map(analogRead(s[i]),sminb[i],smaxb[i],-30,100), 0,100);
    }else{
      ar[i]=constrain(map(analogRead(s[i]),sminb[i],smaxb[i],100,-30), 0,100);
    }
    dr[i]=ar[i] < 30?0:1;
  }
  
}
void extractPatterns()
{
  String pattern="";
  patt3=pattern+dr[4]+dr[5]+dr[6];
  patt5=pattern+dr[3]+dr[4]+dr[5]+dr[6]+dr[7];
  patt7=pattern+dr[2]+dr[3]+dr[4]+dr[5]+dr[6]+dr[7]+dr[8];
  patt9=pattern+dr[1]+dr[2]+dr[3]+dr[4]+dr[5]+dr[6]+dr[7]+dr[8]+dr[9];
  patt =pattern+dr[0]+dr[1]+dr[2]+dr[3]+dr[4]+dr[5]+dr[6]+dr[7]+dr[8]+dr[9]+dr[10];
  
  if(DEBUGGING) {
    oled_print(5,7,String(patt[0]));
    oled_print(5,6,String(patt[1]));
    for(int i=2;i<=8;i++)oled_print(5+i,5,String(patt[i]));
    oled_print(15,6,String(patt[9]));
    oled_print(15,7,String(patt[10]));
  }
}
String getPattern(int mode)
{
  String pattern="";
  int d[NUM_SENSORS];
  for(int i=0; i<NUM_SENSORS; i++){
    if(line=='B'){
      d[i]=constrain(map(analogRead(s[i]),sminb[i],smaxb[i],-30,100), 0,100) < 30?0:1;
    }else{
      d[i]=constrain(map(analogRead(s[i]),sminw[i],smaxw[i],100,-30), 0,100) < 30?0:1;
    }
  }
  
  if(mode==3){
    return (pattern+d[2]+d[5]+d[8]);
  }else if(mode==5){
    return (pattern+d[3]+d[4]+d[5]+d[6]+d[7]);
  }else if(mode==7){
    return (pattern+d[2]+d[3]+d[4]+d[5]+d[6]+d[7]+d[8]);
  }else if(mode==9){
    return (pattern+d[1]+d[2]+d[3]+d[4]+d[5]+d[6]+d[7]+d[8]+d[9]);
  }else{
    return (pattern+d[0]+d[1]+d[2]+d[3]+d[4]+d[5]+d[6]+d[7]+d[8]+d[9]+d[10]);
  }
}
long getPos(int n)
{
  long pos, sum=0,w_sum=0, weight, count=0;
  weight=max_spd;
  if(DEBUGGING) oled_printBar(ar,5-(n/2),5+(n/2),100);
  for(int i=5-(n/2); i<=5+(n/2); i++){sum+=ar[i];}
  if(n==5){
    w_sum=ar[3]*weight*0+ar[4]*weight*0.7+ar[5]*weight+ar[6]*weight*1.3+ar[7]*weight*2;
  }else if(n==7){
    w_sum=ar[2]*0+ar[3]*weight*0.4+ar[4]*weight*0.75+ar[5]*weight+ar[6]*weight*1.25+ar[7]*weight*1.6+ar[8]*weight*2;
  }else if(n==9){
    w_sum=ar[1]*0+ar[2]*0+ar[3]*weight*0.4+ar[4]*weight*0.75+ar[5]*weight+ar[6]*weight*1.25+ar[7]*weight*1.6+ar[8]*weight*2+ar[9]*weight*2;
  }
  pos=w_sum/sum;
  
  if(sum==0)pos=lastPos;
  if(DEBUGGING) {oled.clearField(9,3,3);oled_print(9,3, String(pos));}
  lastPos=pos;
  return pos;
}

bool straight_line(String p)
{
  if(p=="1000000"||p=="1100000"||p=="0100000"||p=="0110000"||p=="0010000"||p=="0011000"||p=="0001000"||p=="0001100"||p=="0000100"||p=="0000110"||p=="0000010"||p=="0000011"||p=="0000001"){
    return true;
  }else{
    return false;
  }
}
void checkIR(bool raw){
  int k=2;
  if(raw){
    for(int i=0,j=0; i<11; i+=4,j++){
      oled_print(0,j, String(analogRead(s[i])/4)+"  "+String(analogRead(s[i+1])/4)+"  "+String(analogRead(s[i+2])/4)+"  "+(i<8?String(analogRead(s[i+3])/4)+"   ":"   "));
    }
  }else{
    readLine();
    for(int i=0,j=0; i<11; i+=4,j++){
      oled_print(0,j, String(ar[i])+"  "+String(ar[i+1])+"  "+String(ar[i+2])+"  "+(i<8?String(ar[i+3])+"   ":"  "));
    }
  }
}
