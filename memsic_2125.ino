// Code for Memsic 2125 accelerometer from Radio Shack
// Accelerometer is made by Parallax
// Model: 28017
// Catalog #: 276-029 

// declare input pins for x and y accelerometer
int accel = 7;

// values for min/max accelerometer readings
float accel_raw = 0;
float accel_sample;
float accel_n = 30;  // number of samples taken to find a stable reading
float accel_range = 2490;    //Taken from test measure of +90 and -90, find diffrence 
float accel_low;
float accel_high;
float accel_offset = 2; //This to correct for inaccuries in acceleromiter.

void accel_setup(){
  pinMode(accel, INPUT);
}

void sample_accel(){
  float accel_total = 0;
  for (int i = 0; i < accel_n; i++){
    read_accel();
    accel_total += accel_raw;}
  accel_sample = (accel_total / accel_n);
  Serial.print("Total: ");
  Serial.println(accel_total);
  Serial.print("AVG: ");
  Serial.println(accel_sample);
  accel_low = accel_sample - (accel_range/2);
  accel_high = accel_sample + (accel_range/2);
  Serial.print("Accel Low:  ");
  Serial.println(accel_low);
  Serial.print("Accel High:  ");
  Serial.println(accel_high);
}

void read_accel(){
  // read the y axis of the accelerometer
  accel_raw = pulseIn(accel, HIGH);
  accel_angle = map (accel_raw, accel_low, accel_high, -90, 90);
  accel_angle = (accel_angle) + 0.00;
}
