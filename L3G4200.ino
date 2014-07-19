// This code is to get angle readings from the L3G4200 gyroscope from Radio Shack using I2C
// Gyroscope is made by Parallax
// Model: 27911-RT
// Catalog #: 276-119 

// I2C setup for gyroscope
#include <Wire.h>
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
int Addr = 105;                 // I2C address of gyro
float gyro_rate;

// scale variable for Gyro
float gyro_scale = 1; //0.0782 Adjust for inaccuries in sensitivity. 

void gyro_setup(){
  Wire.begin();
  writeI2C(CTRL_REG1, 0x1F);    // Turn on all axes, disable power down
  writeI2C(CTRL_REG3, 0x08);    // Enable control ready signal
  writeI2C(CTRL_REG4, 0x80);    // Set scale (500 deg/sec) 500=0x48 250=0x80 2000=0xA0
  delay(100);                   // Wait to synchronize  
}

void read_gyroscope(){
  // Get new values
  getGyroValues();
  // sum gyro angle
  gyro_angle = angle + (gyro_rate * gyro_scale * 0.07); // multiplying gyro_rate by 0.07 seconds = degries rotated since last update then adds this movement to the last position = current position 
  
}

void getGyroValues () {
  byte MSB, LSB;
  MSB = readI2C(0x29);
  LSB = readI2C(0x28);
  gyro_rate = ((MSB << 8) | LSB) * 0.00875;  
  /* multiply gyro readout by the sensitivity in dps/digit 
  sensivity derived from the selected Full Scale range selected, FS = 250,500 or 2000)
  FS = 250 dps - dps/digit = 0.00875
  FS = 500 dps - dps/digit = 0.0175
  FS = 2000 dps - dps/digit = 0.07   
  */
  
}

int readI2C (byte regAddr) {
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);                // Register address to read
  Wire.endTransmission();             // Terminate request
  Wire.requestFrom(Addr, 1);          // Read a byte
  while(!Wire.available()) {};        // Wait for receipt
  return(Wire.read());                // Get result
}

void writeI2C (byte regAddr, byte val) {
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);
  Wire.write(val);
  Wire.endTransmission();
}


