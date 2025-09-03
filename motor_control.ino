void inch()
{
  setMotors(100, 100);
  delay(100);
}
void turn(char dir)
{  
  if (dir == 'l' || dir == 'L')setMotors(-tSpd, tSpd); else setMotors(tSpd, -tSpd);
  
  while(getPattern(7) == "0000000");
  
  setMotors(0, 0);
  delay(100);
}
void turnDeg(char dir, int deg, bool adjust)
{
  if (dir == 'l' || dir == 'L')setMotors(-tSpd, tSpd); else setMotors(tSpd, -tSpd);

  if(deg==45){
    delay(120);
  }else if(deg==90){
    delay(170);
  }else if(deg==135){
    delay(200);
  }else if(deg==180){
    delay(200);
  }
  if(adjust==true){while(getPattern(5) == "00000");}
  setMotors(0,0);
  delay(100);
}
void setMotors(int spdL, int spdR)
{
  spdL = max(min(spdL, 255), -255); spdR = max(min(spdR, 255), -255);

  if (spdL < 0) {analogWrite(lpwm, -spdL);digitalWrite(lmf, LOW);digitalWrite(lmb, HIGH);}
  else {analogWrite(lpwm, spdL);digitalWrite(lmf, HIGH);digitalWrite(lmb, LOW);}
  if (spdR < 0) {analogWrite(rpwm, -spdR);digitalWrite(rmf, LOW);digitalWrite(rmb, HIGH);}
  else {analogWrite(rpwm, spdR);digitalWrite(rmf, HIGH);digitalWrite(rmb, LOW);}
}
void brake()
{
  setMotors(-150,-150);
  delay(100);
  setMotors(0,0);;
}
void halt()
{
  setMotors(0, 0);
}

void pid(bool reverse) {
  static long P, I = 0, D, err, lasterr = 0, spd;
  if(reverse){KD=max_spd/25;}
  else{KD=max_spd/10;}
  err = pos - max_spd;
  P = err;
  I+=err;
  if(err==0)I=0;
  D = err - lasterr;
  lasterr = P;
  spd = KP * P + KI * I + KD * D;
  if (reverse)setMotors(min(max(max_spd+spd, -max_spd), max_spd), min(max(max_spd-spd, -max_spd), max_spd));
  else setMotors(min(max(max_spd+spd, -(max_spd/3)), 255), min(max(max_spd-spd, -(max_spd/3)), 255));
}
