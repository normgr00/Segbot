// www.prototyperobotics.com  -  JD Warren 2013
// Arduino Uno
// This code is for using the accelerometer and gyroscope sensors sold at Radio Shack
// Gyroscope - Parallax 27911-RT 3-Axis Gyroscope Module
// Accelerometer - Memsic 2125 Dual-axis Accelerometer
// This code is a work-in-progress and will be labeled as version 4 (v4), once completed

/*
Next problem is to get gyro and acceleromiter to find level even if it powers up unlevel.
*/

#include <SoftwareSerial.h>
SoftwareSerial motorSerial(2, 3); // RX, TX

// Name Digital I/O pins
// Pin 12 decides whether the Arduino goes into debug mode or not. if pin 12 is not connected to anything, the segbot will operate normally. if pin 12 is grounded when booting up, the segbot will boot into debug mode and will print sensor values to the serial monitor instead of motor values to the sabertooth.
boolean debug = false;
int debug_pin = 12;   // if pin 12 is connected to GND while the Seg-bot is turned On, it will boot into Debug mode and the sensor values will be sent to the serial monitor
int debug_led = 13;

// Name Analog input pins
int steeringPot = 3; // connect the steering potentiometer to Analog input 3
int gainPot = 2; // connect the gain potentiometer to Analog input 2

// Floats for angles
int angle = 0;
float gyro_angle = 0.0;
float accel_angle = 0.0;

// set variable weights
float gyro_weight = 0.95;
float accel_weight = 0.05;

// timer variables
int last_update;
int cycle_time;
long last_cycle = 0;

// engage button variables
int engage_pin = 4;
boolean engage = false;
int engage_state = 1;
// boolean start_up = false;

// motor speed variables
int motor_out = 0;
int motor_1_out = 0;
int motor_2_out = 0;
byte m1_speed = 0;
byte m2_speed = 0;
int output =0;

// potentiometer variables
int steer_val;
int steer_range = 7;
int steer_reading;
int gain_reading;
int gain_val;

// end of variable declaration

void setup(){
  Serial.begin(9600);
  // set the engage_switch pin as an Input
  motorSerial.begin(9600);
  pinMode(engage_pin, INPUT);
  // enable the Arduino internal pull-up resistor on the engage_switch pin.
  digitalWrite(engage_pin, HIGH);
   // Tell Arduino to use the Aref pin for the Analog voltage, don't forget to connect 3.3v to Aref!
  //analogReference(EXTERNAL);
  accel_setup();
  gyro_setup();
  sample_accel();
  // create input for debug_pin to enable user to boot into debug mode if needed by grounding pin 12
  pinMode(debug_pin, INPUT);
  pinMode(debug_led, OUTPUT);
  // enable the Arduino's internal pull-up resistor on pin D12
  digitalWrite(debug_pin, HIGH);
  // let pin voltage settle
  delay(100);
  // check pin 12 state: if left alone (not connected to anything), the Seg-bot will operate normally and the motor output values will be sent to the Sabertooth
  if (digitalRead(debug_pin) == LOW){
    debug = true;
    digitalWrite(debug_led, HIGH);
  }
  // if pin 12 is connected to GND while the Seg-bot is turned On, it will boot into Debug mode and the sensor values will be sent to the serial monitor
  else{
    debug = false;
    digitalWrite(debug_led, LOW);
  }
}

void loop(){
  read_accel();
  read_gyroscope();
  calculate_angle();  
  read_pots();
  update_motor_speed(); 
  auto_level();
    if (debug == false){
    update_motor_speed();    // update the motors with the new values
    time_stamp();     // check the loop cycle time and add a delay as necessary
    }
  
    else {

  // print values
  serial_print_stuff();
  // check the loop cycle time and add a delay as necessary
  time_stamp();
    }
}


