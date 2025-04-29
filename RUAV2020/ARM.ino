int center = 82;
int center_count = 0;
int offset = 15;
long int count_center = 0, count_plus = 0, count_neg =0;
unsigned long long int time_start = 0, time_end_offset = 1000; 

int claw_pos, rot_pos, arm1_pos, arm2_pos;//keeps track of arm servos current pos

void arm_setup(){
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
}

int arm()
{
    PWM1 = ch[1];//1000 to 2000
    PWM2 = ch[2];//1000 to 2000;
    PWM3 = ch[3];//1000 to 2000;
    PWM4 = ch[4];//1000 to 2000;
    
    if ((millis() - update_timer) >= update_time_space)
    {
      servo_update(current_servo_1_angle, 1600, 1400, PWM1, 1);
      servo_update(current_servo_2_angle, 1600, 1400, PWM2, 1);
      servo_update(current_servo_3_angle, 1600, 1400, PWM3, 1);
      servo_update(current_servo_4_angle, 1600, 1400, PWM4, 1);
      
      servo_range_check(current_servo_1_angle, SERVO_1_MAX, SERVO_1_MIN);
      servo_range_check(current_servo_2_angle, SERVO_2_MAX, SERVO_2_MIN);
      servo_range_check(current_servo_3_angle, SERVO_3_MAX, SERVO_3_MIN);
      servo_range_check(current_servo_4_angle, SERVO_4_MAX, SERVO_4_MIN);
            
      update_timer = millis();
    }
    
    if ((millis() - servo_timer) >= servo_time_space){      
      servo1.writeMicroseconds(current_servo_1_angle);  //write the servo position to the servo
      servo2.writeMicroseconds(current_servo_2_angle);

      if(PWM4 > 1600 || PWM4 < 1400)
      {
        servo4.attach(SERVO4_PIN);
        servo4.writeMicroseconds(current_servo_4_angle);
      }
      else
      {
        servo4.detach();
      }

      if(PWM3 > 1600 || PWM3 < 1400)
      {
        servo3.attach(SERVO3_PIN, MIN_SERVO, MAX_SERVO);
        servo3.writeMicroseconds(current_servo_3_angle);
      }
      else
      {
        servo3.detach();
      } 
      
      servo_timer = millis();
    }  
}
