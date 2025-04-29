/*
   Function Name: Servo Range Check
   Purpose: This function checks if a servo angle is out of range, if it is,
            it corrects it to either the maximum or minimum allowable angle.
   Parameters: float - current angle value, float - maximum angle allowable, float - minimum angle allowable
   Return: Void
*/
void servo_range_check(int& angle, int max_angle, int min_angle)
{
  if (angle > max_angle)
    angle = max_angle;
    
  else if (angle < min_angle)
    angle = min_angle;
}


void servo_update(int& angle, int upper_deadzone, int lower_deadzone, int PWM, int increment)
{
  if (PWM > upper_deadzone)
    angle += increment;
    
  else if (PWM < lower_deadzone)
    angle -= increment;
}


void auto_servo_update(int& current_angle, int target_angle, int increment)
{
  if (abs(current_angle - target_angle) <= increment)
    current_angle = target_angle;
    
  else if (current_angle < target_angle)
    current_angle += increment;
    
  else if (current_angle > target_angle)
    current_angle -= increment;
}


void box_target_get()
{
  if ((ch[0] <= BOX_1_UPPER) and (ch[0] >= BOX_1_LOWER))
    box_target_angle = 1;
    
  else if ((ch[0] <= BOX_2_UPPER) and (ch[0] >= BOX_2_LOWER))
    box_target_angle = 2;
    
  else if ((ch[0] <= BOX_3_UPPER) and (ch[0] >= BOX_3_LOWER))
    box_target_angle = 3;
    
  else if ((ch[0] <= BOX_4_UPPER) and (ch[0] >= BOX_4_LOWER))
    box_target_angle = 4;
}
