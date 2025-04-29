void pickup()
{
  if ((pickup_dropoff_box > 12) or (pickup_dropoff_box < 1))
    pickup_dropoff_stage = 1;

  if (pickup_dropoff_stage < 8)
    box_target_get();

  if (abs(ch[0] - 1500) > 100)  //checks if joystick is out of deadzone
    pickup_dropoff_box(box_target_angle, false);
  else if ((abs(ch[0] - 1500) <= 100) && (pickup_dropoff_stage < 4))
    pickup_dropoff_box(box_target_angle, true);
}


void dropoff()
{
  if ((pickup_dropoff_box > 12) or (pickup_dropoff_box < 1))
    pickup_dropoff_stage = 11;

  if (pickup_dropoff_stage > 9)
    box_target_get();

  if (abs(ch[0] - 1500) > 100)  //checks if joystick is out of deadzone
    pickup_dropoff_box(box_target_angle, true);
  else if ((abs(ch[0] - 1500) <= 100) && (pickup_dropoff_stage > 8))
    pickup_dropoff_box(box_target_angle, false);
}


void pickup_dropoff_box(int box_angle, bool reverse)
{
  if (pickup_dropoff_stage == 1)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_CLOSED, -1, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: void, claw: closed
  else if (pickup_dropoff_stage == 2)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_OPEN, SERVO_4_CENTER, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: center, claw: open
  else if (pickup_dropoff_stage == 3)
  {
    if (reverse == false)
      move_to(SERVO_1_EXTENDED, SERVO_2_EXTENDED, SERVO_3_OPEN, SERVO_4_CENTER, pickup_dropoff_stage, reverse);  //arm: extended, rotation: center, claw: open
    else if (reverse == true)
      move_to(SERVO_1_Retracted, SERVO_2_Retracted, SERVO_3_OPEN, -1, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: void, claw: open
  }
  else if (pickup_dropoff_stage == 4)
  {
    if (pickup_dropoff_stage_last != pickup_dropoff_stage)
      claw_delay_timer = millis();
    if ((millis() - claw_delay_timer) >= claw_delay)  //adds a pause before claw is closed to allow user to switch into manual for fine adjustments
    {
      if (reverse == false)
        move_to(-1, -1, SERVO_3_CLOSED, -1, pickup_dropoff_stage, reverse);  //arm: void, rotation: void, claw: closed
      else if (reverse == true)
        move_to(-1, -1, SERVO_3_OPEN, -1, pickup_dropoff_stage, reverse);  //arm: void, rotation: void, claw: open
    }
  }
  else if (pickup_dropoff_stage == 5)
  {
    if (reverse == false)
      move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_CLOSED, -1, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: void, claw: closed
    else if (reverse == true)
      move_to(SERVO_1_EXTENDED, SERVO_2_EXTENDED, SERVO_3_CLOSED, SERVO_4_CENTER, pickup_dropoff_stage, reverse);  //arm: extended, rotation: center, claw: closed
  }
  else if (pickup_dropoff_stage == 6)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_CLOSED, SERVO_4_CENTER, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: center, claw: closed
  else if (pickup_dropoff_stage == 7)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_CLOSED, box_target_angle, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: holder, claw: closed
  else if (pickup_dropoff_stage == 8)   
    move_to(SERVO_1_EXTENDED, SERVO_2_EXTENDED, SERVO_3_CLOSED, box_target_angle, pickup_dropoff_stage, reverse);  //arm: extended, rotation: holder, claw: closed
  else if (pickup_dropoff_stage == 9)
    move_to(SERVO_1_EXTENDED, SERVO_2_EXTENDED, SERVO_3_OPEN, box_target_angle, pickup_dropoff_stage, reverse);  //arm: extended, rotation: holder, claw: open
  else if (pickup_dropoff_stage == 10)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_OPEN, box_target_angle, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: holder, claw: open
  else if (pickup_dropoff_stage == 11)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_CLOSED, -1, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: void, claw: closed
  else if (pickup_dropoff_stage == 12)
    move_to(SERVO_1_RETRACTED, SERVO_2_RETRACTED, SERVO_3_CLOSED, SERVO_4_CENTER, pickup_dropoff_stage, reverse);  //arm: retracted, rotation: center, claw: closed

  pickup_dropoff_stage_last = pickup_dropoff_stage;
}


void move_to(int servo_1_target, int servo_2_target, int servo_3_target, int servo_4_target, int& arm_stage, bool reverse)
{
  if ((millis() - servo_timer) >= servo_time_space)  //update the servo angle if time since last update >= servo_time_space
  {
    /*check if step is complete*/
    if ((servo_1_target == current_servo_1_angle || servo_1_target == -1) && (servo_2_target == current_servo_2_angle || servo_2_target == -1) &&
        (servo_3_target == current_servo_3_angle || servo_3_target == -1) && (servo_4_target == current_servo_4_angle || servo_4_target == -1))
    {
      if (reverse == false)
        arm_stage++;  //jumps to the next stage
      else if (reverse == true)
        arm_stage--;  //jumps to the previous stage
      return;
    }

    /*updating angles for the servos*/
    if (servo_1_target != -1)
    {
      auto_servo_update(current_servo_1_angle, servo_1_target, servo_angle_increment);
      servo1.writeMicroseconds(current_servo_1_angle);
    }

    if (servo_2_target != -1)
    {
      auto_servo_update(current_servo_2_angle, servo_2_target, servo_angle_increment);
      servo2.writeMicroseconds(current_servo_2_angle);
    }

    if (servo_3_target != -1)
    {
      auto_servo_update(current_servo_4_angle, servo_4_target, servo_angle_increment);
      if (PWM3 > 1600 || PWM3 < 1400)
      {
        servo3.attach(SERVO3_PIN, MIN_SERVO, MAX_SERVO);
        servo3.writeMicroseconds(current_servo_3_angle);
      }
      else
      {
        servo3.detach();
      }
    }

    if (servo_4_target != -1)
    {
      auto_servo_update(current_servo_3_angle, servo_3_target, servo_angle_increment);
      if (PWM4 > 1600 || PWM4 < 1400)
      {
        servo4.attach(SERVO4_PIN);
        servo4.writeMicroseconds(current_servo_4_angle);
      }
      else
      {
        servo4.detach();
      }
    }

    servo_timer = millis();
  }
}
