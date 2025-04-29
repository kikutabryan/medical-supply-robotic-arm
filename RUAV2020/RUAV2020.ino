/*
grabber twitch 
POT:  black center. A6, A5 and A4 are 5v and ground. 
*/

//methods, give_to_box1 though box4
//methods, Get_from_box1 through box4

//50hz = 20ms
//1000 to 2000 millis();
//each singal generates 2 interups each cycle 

//claw max PWM = 1225 (old/new/sand)
// = 0<  < 180,  1000 < 1225 < 2000

#include <Servo.h>
Servo wheel_FL, wheel_FR, wheel_BL, wheel_BR;//wheels
Servo servo1, servo2, servo3, servo4;//claw

#define pot_pin_ADC A4// rot sensor
#define SERVO1_PIN A3 //1
#define SERVO2_PIN A2 //2
#define SERVO3_PIN A1 //claw
#define SERVO4_PIN A0 //swivel 

#define wheelFL_PIN 3 
#define wheelFR_PIN 4
#define wheelBL_PIN 5
#define wheelBR_PIN 6

#define MAX_SERVO 2000
#define MIN_SERVO 1000
#define claw_open 180 
#define claw_close 45 

#define box1_rotation -140//servo1_pos=108 servo2_pos=58
#define box2_rotation -95// servo1_pos=96 servo2_pos=18
#define box3_rotation 140// servo1_pos=108 servo2_pos=58
#define box4_rotation 95// servo1_pos=96 servo2_pos=18
#define box_get 1
#define box_give 0


//AutoPID arm_PID(&temperature, &setPoint, &outputVal, 0, 180, KP, KI, 0);


int servo1_pos = 0, servo2_pos = 180, servo3_pos = claw_open, servo4_pos = 90;//0 to 180
int rotate_to = 0, rotate_current = 0, rotate_speed = 0;
long int PWM1, PWM2, PWM3, PWM4, PWM5, PWM6;
long int time_current, time_last1, time_last2;
int degree_sec1 = 0, degree_sec2 = 0;
int claw_closed = 0;

int dis = 0;
int dis_1[] = {0,0};//servo1 degree for distance
int dis_2[] = {0,0};//servo2 degree for distance
//rate 1500 1600 (1 degree/sec)
//rate 1600 1700 (2 degree/sec)
//rate 1700 1800 (3 degree/sec)
//rate 1800 1900 (4 degree/sec)
//rate 1900 2000 (5 degree/sec)

int ch[9];
unsigned long long int rise_time, current_time, last_time = 0; 
unsigned int ch_select = 0;

//int MODE = 0;
int nob_ch = 7;
int sw_ch = 6;


//********************************USER NEEDS TO SET*****************************
#define SERVO_1_MAX 2250//max servo angle position if the servo is restriceted by the arm, in degrees!
#define SERVO_1_MIN 500  //min servo angle position
#define SERVO_2_MAX 2500
#define SERVO_2_MIN 1000
#define SERVO_3_MAX 2000
#define SERVO_3_MIN 1000
#define SERVO_4_MAX 2000
#define SERVO_4_MIN 1000

int current_servo_1_angle = 1100;  //variable for the current assigned angle of the first servo
int current_servo_2_angle = 2300;  //variable for the current assigned angle of the second servo
int current_servo_3_angle = 1500;
int current_servo_4_angle = 1500;

//END OF "USER NEEDS TO SET" SECTION
long unsigned int servo_timer = 0;  //variable to hold the previous update time
long unsigned int update_timer = 0;

//USER NEEDS TO SET
int servo_time_space = 10;  //time interval between servo position updates
int update_time_space = 10;

//auto_arm
#define SERVO_1_RETRACTED 1500
#define SERVO_1_EXTENDED 1500
#define SERVO_2_RETRACTED 1500
#define SERVO_2_EXTENDED 1500
#define SERVO_3_CLOSED 1500
#define SERVO_3_OPEN 1500
#define SERVO_4_CENTER 1500

#define BOX_1_UPPER 1500
#define BOX_1_LOWER 1500
#define BOX_2_UPPER 1500
#define BOX_2_LOWER 1500
#define BOX_3_UPPER 1500
#define BOX_3_LOWER 1500
#define BOX_4_UPPER 1500
#define BOX_4_LOWER 1500

int pickup_dropoff_stage = 0;
int pickup_dropoff_stage_last = 0;
int box_target_angle;
int servo_angle_increment = 1;

long unsigned int claw_delay_timer = 0;
int claw_delay = 2000;
//***********************************************************************************

void setup() {
  Serial.begin(115200);

  pinMode(pot_pin_ADC, INPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), PPM, CHANGE);// enabling interrupt at pin 2

}

void loop() {
  if (ch[sw_ch] < 1500){ //enable switch
    if (ch[nob_ch] > 1333 && ch[nob_ch] <= 1666){
      Serial.print("ARM ");
      arm_setup();
      arm();
      wheel_FL.detach();
      wheel_FR.detach();
      wheel_BL.detach();
      wheel_BR.detach();
      
    }else if (ch[nob_ch] > 1666){
      Serial.print("DRV ");
      drive();
      servo1.detach();
      servo2.detach();
      servo3.detach();
      servo4.detach();
    }else{
      Serial.print("OF1 ");
    }
  }else{
    Serial.print("OF2 ");
    servo1.detach();
    servo2.detach();
    servo3.detach();
    servo4.detach();
    wheel_FL.detach();
    wheel_FR.detach();
    wheel_BL.detach();
    wheel_BR.detach();
  }

  if(PWM1 == -90 && PWM2 == 90 && PWM3 == 90 && PWM4 == -90 && PWM5 == 1000 ){}//failsafe
  
                    //Serial.print(ch[0]);
  Serial.print(" ");Serial.print(ch[1]);
  Serial.print(" ");Serial.print(ch[2]);
  Serial.print(" ");Serial.print(ch[3]);
  Serial.print(" ");Serial.print(ch[4]);
  Serial.print(" ");Serial.print(ch[5]);
  Serial.print(" ");Serial.print(ch[6]);
  Serial.print(" ");Serial.print(ch[7]);
  Serial.print(" ");Serial.println(ch[8]);
  delay(10);
}

long int max_range(long int pwm_current){
  if (pwm_current > MAX_SERVO){//input range limited to min/max
    return MAX_SERVO;
  }
  if (pwm_current < MIN_SERVO){
    return MIN_SERVO;
  }
  return pwm_current;
}


////////////////////////////PPM/////////////////////////
void PPM(){
  current_time = micros();
  if(digitalRead(2) == LOW){
    if(last_time < current_time - 4000){
      ch_select = 0;
    }
      ch[ch_select] = current_time - rise_time + 400;//600 is min for PPM
      ch_select++;
  }
  else{
    rise_time = current_time;
  }
  last_time = current_time;
}
