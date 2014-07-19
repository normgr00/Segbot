


void calculate_angle(){
  angle = (float)(gyro_weight * gyro_angle) + (accel_weight * accel_angle);
}

void time_stamp(){
  // check to make sure it has been exactly 50 milliseconds since the last recorded time-stamp
  while((millis() - last_cycle) < 60){
    delay(1);
  }
  // once the loop cycle reaches 60 mS, reset timer value and proceed
  cycle_time = millis() - last_cycle;
  last_cycle = millis();
}

void read_pots(){
  // Read and convert potentiometer values
  // Steering potentiometer
  steer_reading = analogRead(steeringPot); // We want to map this into a range between -1 and 1, and set that to steer_val
  steer_val = map(steer_reading, 0, 1023, steer_range, -steer_range);
  if (angle == 0.00){
  gain_val = 0;
  }
  // Gain potentiometer
  gain_reading = analogRead(gainPot);
  gain_val = map(gain_reading, 0, 1023, 32, 64);
}

void auto_level(){
  // enable auto-level turn On
  engage_state = digitalRead(engage_pin);
  if (engage_state == 1) {
    engage = false;
  } 
  else {
    if (engage_state == 0) {
      if (angle < 2 && angle > -2) {
        engage = false;
      } 
      else {
        engage = true;
      }
    }
  }
}

void update_motor_speed(){
  // Update the motors
  if (engage == true){
    if (angle < -65 || angle > 65){  //If segbot tips over disenguage
      motor_out = 0;
    }
    else {
      motor_out = map(angle, -30, 30, -gain_val, gain_val); //The angle between +-55 is mapped to a gain value between 32 and 64
    // assign steering bias
    motor_1_out = (motor_out + steer_val);
    motor_2_out = (motor_out - steer_val);
    // test for and correct invalid values
    if(motor_1_out > 64){
      motor_1_out = 64;
    }
    if(motor_1_out < -64){
      motor_1_out = -64;
    }
    if(motor_2_out > 64){
      motor_2_out = 64;
    }
    if(motor_2_out < -64){
      motor_2_out = -64;
    }
    // assign final motor output values
    m1_speed = 64 + motor_1_out;
    m2_speed = 192 + motor_2_out;
    //m1_speed = constrain(m1_speed, 1, 127);  //without constraining the speed can add up to 128 and 255 or 1 and 
    //m2_speed = constrain(m1_speed, 129, 255);
  }}
  else{
    m1_speed = 0;
    m2_speed = 0; 
  }
  // Serial speed values write here:  
  motorSerial.write(m1_speed);
  motorSerial.write(m2_speed);
}
  
 void serial_print_stuff(){
  // Debug with the Serial monitor
  //these values are commented out depending on testing needs
  /*
  Serial.print("steer_val: ");
  Serial.print(steer_val);
  Serial.print(" ");
  Serial.print("Gain_val: ");
  Serial.print(gain_val);
  Serial.print(" ");
  Serial.print("Engage: ");
  Serial.print(engage); // print the filtered angle
  Serial.print(" ");
 
  Serial.print("Accel: ");
  Serial.print(accel_angle); // print the accelerometer angle
  Serial.print(" ");
  Serial.print("Gyro: ");
  Serial.print(gyro_angle); // print the gyro angle
  Serial.print(" ");
  Serial.print(" ");

  Serial.print("Angle: ");
  Serial.print(angle); // print the filtered angle
  Serial.print(" ");

   Serial.print("Engage: ");
   Serial.print(engage); // print the filtered angle
   Serial.print(" ");   
 
  Serial.print("steer_val: ");
  Serial.print(steer_val);
  Serial.print(" ");
  Serial.print(motor_out);
  Serial.print(" ");
*/
   Serial.print("m1/m2: ");
   Serial.print(m1_speed);

   Serial.print("/");
   Serial.print(m2_speed);

   Serial.print(" time: ");
   Serial.print(cycle_time); // print the loop cycle time
   Serial.println(" ");

}

