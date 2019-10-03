#include <Servo.h>
Servo servo;

//motor control pins
const int enAPin = 6; //Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control
int vel = 50; //initial speed
int command = 'o'; // initial command

enum Motor{LEFT,RIGHT};

// Set motor speed: 255 full ahead, −255 full reverse , 0 stop
void go( enum Motor m, int speed)
{
  digitalWrite (m == LEFT ? in1Pin : in3Pin , speed > 0 ? HIGH : LOW );
  digitalWrite (m == LEFT ? in2Pin : in4Pin , speed <= 0 ? HIGH : LOW );
  analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed );
}

// Initial motor test :
// left motor forward then back
// right motor forward then back
void testMotors ()
{
  static int speed[8] = { 50, 100, 128, 0 ,-50, -100, -28, 0};
  go(RIGHT, 0);
  
  for (unsigned char i = 0 ; i < 8 ; i++)
    go(LEFT, speed[i ]), delay (200);

  for (unsigned char i = 0 ; i < 8 ; i++)
    go(RIGHT, speed[i ]), delay (200);
}

// Initial configuration
//Configure the input and output pins
//Center the servo
//Turn off the motors
//Test the motors
//Scan the surroundings once

void setup () {

  Serial.begin(9600);

  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  go(LEFT, 0);
  go(RIGHT, 0);
  
  testMotors ();
}

// Main loop:

void loop() {

  if (Serial.available()>0)  {
    command = Serial.read();
    }

  if(command == 'u'){ // Forward
    Serial.println("forward");
    go(LEFT, 60);
    go(RIGHT, 60);
  }

 
  if(command =='r'){ // right
    Serial.println("right");
    go(LEFT, 70);
    go(RIGHT, 30);
  }
  
  if(command =='s'){ // Stop
    Serial.println("stop");
    go(LEFT, 0);
    go(RIGHT, 0);
   }
   
  if(command =='l'){ // left
    Serial.println("left");
    go(LEFT, 30);
    go(RIGHT, 70);
  }
  
  if(command =='d'){ // Reverse
    Serial.println("backward");
    go(LEFT, -50);
    go(RIGHT, -50);
  } 
  
  if (command =='f'){ // Boton ON se mueve sensando distancia
  }
  
  if (command =='o'){ // Boton OFF, detiene los motores no hace nada
  }
}
