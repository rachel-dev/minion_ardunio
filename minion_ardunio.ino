#include <ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <Servo.h>
#include <math.h>

Servo servo;

ros::NodeHandle nh;

//motor control pins
//motor left front
const int enAPin = 6; //Left motor PWM speed control
const int in1Pin = 4; // Left motor Direction 1
const int in2Pin = 2; // Left motor Direction 2
//motor right front
const int in3Pin = 7; // Right motor Direction 1
const int in4Pin = 5; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control

float move_line, move_angular, move_line_turn;
enum Motor{LEFT,RIGHT};

// Set motor speed: 255 full ahead, −255 full reverse , 0 stop
void go( enum Motor m, int speed)
{
  digitalWrite (m == LEFT ? in1Pin : in3Pin , speed > 0 ? LOW : HIGH );
  digitalWrite (m == LEFT ? in2Pin : in4Pin , speed <= 0 ? LOW : HIGH );
  analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed );
}

void messageCb( const geometry_msgs::Twist& cmd_vel){
  move_line = cmd_vel.linear.x * 450 ;
  move_angular = cmd_vel.linear.x * 127 ;
  move_line_turn = cmd_vel.angular.z > 0 ? -move_line*(1+cmd_vel.angular.z): move_line*(-1-cmd_vel.angular.z);

  if(abs(move_line_turn) > 255){
    move_line_turn = move_line_turn > 0 ? 255:-255;
    }
    
  if (abs(cmd_vel.angular.z)< 0.0001){
    //forward or backward
    go(RIGHT,move_line);
    go(LEFT,move_line);
    }
    else if(cmd_vel.angular.z > 0.0001){
     //angular > 0 turn right
      go(RIGHT,move_line);
      go(LEFT,move_line_turn);  
      }
    else if(cmd_vel.angular.z < -0.0001){
      // turn left
      go(RIGHT,move_line_turn);
      go(LEFT,move_line);      
      }
}

//geometry_msgs::Twist motor_data;
ros::Subscriber<geometry_msgs::Twist> s("/minion/cmd_vel", &messageCb);
//ros::Publisher p("/minion/motor", &motor_data);


void setup() {
  Serial.begin(9600);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  go(LEFT, 0);
  go(RIGHT, 0);
  
  nh.initNode();
  nh.subscribe(s);
}

void loop() {
//  motor_data.linear.x = move_line;
//  motor_data.angular.z = move_line_turn;
  nh.spinOnce();
//  p.publish( &motor_data );
  delay(1);
}
