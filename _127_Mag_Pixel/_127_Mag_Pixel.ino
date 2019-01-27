/*
  Mag Minimums:  -3.64 -32.36  -79.49
  Mag RealTime:   38.55 14.45 6.73
  Mag Maximums:   54.27 57.00 10.71

*/
#include <Adafruit_NeoPixel.h>

#define NeoPCB 8
#define NeoRing 10

Adafruit_NeoPixel Neo8 = Adafruit_NeoPixel(1, NeoPCB, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Ring = Adafruit_NeoPixel(16, NeoRing, NEO_GRB + NEO_KHZ800);
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to these sensors */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float AccelMinX, AccelMaxX;
float AccelMinY, AccelMaxY;
float AccelMinZ, AccelMaxZ;

float MagMinX = -5;
float MagMaxX = 45;
float MagMinY = -14;
float MagMaxY = 42;
float MagMinZ;
float MagMaxZ;

long lastDisplayTime;

void setup(void)
{
  Ring.begin();
  Ring.setBrightness(255);
  Ring.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  Serial.println("LSM303 Calibration"); Serial.println("");

  /* Initialise the accelerometer */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  /* Initialise the magnetometer */
  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  lastDisplayTime = millis();
}

void loop(void)
{
  sensors_event_t magEvent;
  mag.getEvent(&magEvent);

  if (magEvent.magnetic.x < MagMinX) MagMinX = magEvent.magnetic.x;
  if (magEvent.magnetic.x > MagMaxX) MagMaxX = magEvent.magnetic.x;

  float spanX = MagMaxX - MagMinX;
  float centerX = MagMinX + spanX / 2;
  float X = (magEvent.magnetic.x - centerX) / (spanX / 2);

  if (magEvent.magnetic.y < MagMinY) MagMinY = magEvent.magnetic.y;
  if (magEvent.magnetic.y > MagMaxY) MagMaxY = magEvent.magnetic.y;

  float spanY = MagMaxY - MagMinY;
  float centerY = MagMinY + spanY / 2;
  float Y = (magEvent.magnetic.y - centerY) / (spanY / 2);

  if (magEvent.magnetic.z < MagMinZ) MagMinZ = magEvent.magnetic.z;
  if (magEvent.magnetic.z > MagMaxZ) MagMaxZ = magEvent.magnetic.z;


  float heading = atan2(Y, X) * 180 / 3.14159 + 180;

  float pixel = map(heading, 0, 360, 16, 0);

  for (int j = 0; j < 16; j++) {
    Ring.setPixelColor(j, 0, 0, 0);
  }
  Ring.setPixelColor(int(pixel)-1, 0, 0, 255);
  Ring.show();

  if ((millis() - lastDisplayTime) > 1000)  // display once/second
  {

    Serial.print("Mag Minimums: \t"); Serial.print(MagMinX); Serial.print("\t"); Serial.print(MagMinY); Serial.print("\t"); Serial.print(MagMinZ); Serial.println();
    Serial.print("Mag RealTime: \t"); Serial.print(magEvent.magnetic.x); Serial.print("\t"); Serial.print(magEvent.magnetic.y); Serial.print("\t"); Serial.print(magEvent.magnetic.z); Serial.println();
    Serial.print("Mag Maximums: \t"); Serial.print(MagMaxX); Serial.print("\t"); Serial.print(MagMaxY); Serial.print("\t"); Serial.print(MagMaxZ); Serial.println(); Serial.println();
    Serial.println("------------------------------------------------------------------");
    Serial.println(X);
    Serial.println(heading);
    Serial.println(pixel);
    lastDisplayTime = millis();
  }
}
