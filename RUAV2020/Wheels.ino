int FL, FR, BL, BR;

int drive(){
  PWM1 = map(ch[1], MIN_SERVO, MAX_SERVO, 90, -90);//the wheels are flipped,
  PWM2 = map(ch[2], MIN_SERVO, MAX_SERVO, -90, 90);
  PWM4 = map(ch[4], MIN_SERVO, MAX_SERVO, 90, -90);
  
  FL = 0 - PWM1 - PWM2 - PWM4;
  FR = 0 - PWM1 + PWM2 - PWM4;
  BL = 0 + PWM1 - PWM2 - PWM4;
  BR = 0 + PWM1 + PWM2 - PWM4;

  FL = map(FL, -90, 90, 0, 180);
  FR = map(FR, -90, 90, 0, 180);
  BL = map(BL, -90, 90, 0, 180);
  BR = map(BR, -90, 90, 75, 115);
  if((PWM1 + PWM2 + PWM4) > 3350 && (PWM1 + PWM2 + PWM4) < 3650){
    //detach wheels if no input. needed due to drifting. 
    wheel_FL.detach();
    wheel_FR.detach();
    wheel_BL.detach();
    wheel_BR.detach();
  }else{//dead zone
    wheel_FL.attach(wheelFL_PIN, MIN_SERVO, MAX_SERVO);
    wheel_FR.attach(wheelFR_PIN, MIN_SERVO, MAX_SERVO);
    wheel_BL.attach(wheelBL_PIN, MIN_SERVO, MAX_SERVO);
    wheel_BR.attach(wheelBR_PIN, MIN_SERVO, MAX_SERVO);
    wheel_FL.write(FL);
    wheel_FR.write(FR);
    wheel_BL.write(BL);
    wheel_BR.write(BR);  
  }


  //*****************debug********************
  Serial.print(" FL:");Serial.print(FL);
  Serial.print(" FR:");Serial.print(FR);
  Serial.print(" BL:");Serial.print(BL);
  Serial.print(" BR:");Serial.println(BR);
}
