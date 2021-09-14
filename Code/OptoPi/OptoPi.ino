#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
#include <elapsedMillis.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_AS7341.h>

// Light stimulation Settings
const byte red = 255;
const byte green = 0;
const byte blue = 0;
byte brightness = 20;
int BRIGHTNESS;
byte brightness_low = 0;

// Variables to enable equations to calculate the irradiance
bool red_selected = true;
bool green_selected = false;
bool blue_selected = false;

elapsedMillis timeElapsed;
// delay in milliseconds between blinks of the Matrices
unsigned int DELAY = 1000;
// delay befor starting the stimulation
unsigned int DELAY_0 = 10000;
bool state = LOW;
unsigned long previousMillis;
byte max_intensity = 255;

//MATRICES VARIABLES
#define DATAPIN_1    10
#define CLOCKPIN_1   11

#define DATAPIN_2    12
#define CLOCKPIN_2   13

#define DATAPIN_3    28
#define CLOCKPIN_3   26

#define DATAPIN_4    24
#define CLOCKPIN_4   22

Adafruit_DotStarMatrix matrix_1= Adafruit_DotStarMatrix(
                                  8, 8, DATAPIN_1, CLOCKPIN_1,
                                  DS_MATRIX_BOTTOM     + DS_MATRIX_LEFT +
                                  DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE,
                                  DOTSTAR_BGR);

Adafruit_DotStarMatrix matrix_2= Adafruit_DotStarMatrix(
                                  8, 8, DATAPIN_2, CLOCKPIN_2,
                                  DS_MATRIX_BOTTOM     + DS_MATRIX_LEFT +
                                  DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE,
                                  DOTSTAR_BGR);
                                  
Adafruit_DotStarMatrix matrix_3= Adafruit_DotStarMatrix(
                                  8, 8, DATAPIN_3, CLOCKPIN_3,
                                  DS_MATRIX_BOTTOM     + DS_MATRIX_LEFT +
                                  DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE,
                                  DOTSTAR_BGR);
                                  
Adafruit_DotStarMatrix matrix_4= Adafruit_DotStarMatrix(
                                  8, 8, DATAPIN_4, CLOCKPIN_4,
                                  DS_MATRIX_BOTTOM     + DS_MATRIX_LEFT +
                                  DS_MATRIX_ROWS + DS_MATRIX_PROGRESSIVE,
                                  DOTSTAR_BGR);

// ULTRASONIC SENSOR VARIABLES
int trigPin = 30;    // Trigger
int echoPin = 32;    // Echo
float duration, cm, inches;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

// ULTRASONIC SENSOR VARIABLES
int trigPin_2 = 34;   
int echoPin_2 = 36;    
float duration_2, cm_2;

int readings_2[numReadings];      
int readIndex_2 = 0;            
float total_2 = 0;                  
float average_2 = 0;      

int distance_camera;
int pot2;

// SERVO MOTORS VARIABLES

//Angle variable for the IR LED Arrays
int angle_1 = 0;

//Angle variable for the RGB Matrix
int angle_2 = 0;

//Angle corrected variable for the IR LED Arrays
int  angle_corrected_1 = 0;
int  angle_corrected_2 = 0;
int  angle_corrected_3 = 0;
int  angle_corrected_4 = 0;

int offset_1 = -15;
int offset_2 = -12;
int offset_3 = 0;
int offset_4 = 0;

////Angle corrected variable for the RGB Matrix
//int  angle_corrected_5 = 0;
int  angle_corrected_6 = 0;
int  angle_corrected_7 = 0;
//int  angle_corrected_8 = 0;

int offset_6 = 0;
int offset_7 = 0;

// Servo objects
Servo servo1; 
Servo servo2; 
Servo servo3; 
Servo servo4; 
Servo servo5; 
Servo servo6; 
Servo servo7; 
//Servo servo8; 

// TSL2561 Sensor motor configuration
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}

// AS7341 object declaration
Adafruit_AS7341 as7341;

void setup() 
{
  
  Serial.begin(9600);

  //Setup Servo motors
  servo1.attach(2); // Servo objects pin attachment
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);
  servo7.attach(8);
//  servo8.attach(9);
  
  //Setup Ultrasonic sensor 1
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
  {
    readings[thisReading] = 0;
  }
  
  //Setup Ultrasonic sensor 2
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  for (int thisReading_2 = 0; thisReading_2 < numReadings; thisReading_2++) 
  {
    readings_2[thisReading_2] = 0;
  }

  //Setup Matrices
  matrix_1.begin();

  matrix_2.begin();

  matrix_3.begin();

  matrix_4.begin();

  // Configure the light intensity sensor
  configureSensor();

  // Initialize the AS7341 spectral sensor and settings
  as7341.begin();

  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);
  
  //Check the values of the potentiometers preset and adjust once
  
  // CAMERA DISTANCE
  Camera();

  // IR ARRAYS ANGLE
  IR_Lights_angle();
    
  // RGB MATRICES ANGLE
  RGB_matrices_angle();
}



void loop() 
{
   String command = Serial.readString();
   while (command == "begin")
   {
   //Setting functions
   Settings();

   //RGB MATRICES STIMULATION
   Set_Matrix_Color_Brightness(0, 0, 0, 0);

   //Intensity ramp behaviural function
   //intensity_ramp(max_intensity,DELAY);
  
   //Blinking matrices behaviural function
   //blinking(DELAY_0, DELAY);

   //Read irradiance
   //read_irradiance();
   }
}

////////////////////////////////////FUNCTIONS////////////////////////////////////////////////////////
void Settings()
{
  //  //READ DISTANCE ULTRASONIC SENSOR 1(Top)
  Read_distance_sensor_1();
//  
//  //READ DISTANCE ULTRASONIC SENSOR 2(Bottom)
  Read_distance_sensor_2();
//  
//  // CAMERA DISTANCE
  Camera();
//  
//  // IR ARRAYS ANGLE
  IR_Lights_angle();
//
//  // RGB MATRICES ANGLE
  RGB_matrices_angle();
  //
  Set_Matrix_Brightness();

  //Bottom distance 
   Serial.print(" Db: ");
   Serial.print(average_2);
   //Top distance 
   Serial.print(" Dt: ");
   Serial.print(average);
   //IR angle
   Serial.print(" IR: ");
   Serial.print(angle_1);
   //RGB angle
   Serial.print(" RGB: ");
   Serial.print(angle_2);
   //Camera distance
   Serial.print(" Cd: ");
   Serial.print(distance_camera);
   //RGB brightness
   Serial.print(" Bright: ");
   Serial.print(BRIGHTNESS);
   Serial.println();
}
void IR_Lights_angle()
  {
   //Reading from the first potentiometer and converting to an angle from 0-180 deg
   angle_1 = analogRead(A0)*0.176;
   
   //Send position to Servo 1
   angle_corrected_1 = angle_1 + offset_1;
   servo1.write(angle_corrected_1);
 
   //Send position to Servo 2
   angle_corrected_2 = angle_1 + offset_2 ;
   servo2.write(angle_corrected_2);
 
   //Send position to Servo 3
   angle_corrected_3 = angle_1 + offset_3;
   servo3.write(angle_corrected_3);

   //Send position to Servo 4
   angle_corrected_4 = angle_1 + offset_4;
   servo4.write(angle_corrected_4);
  }

void RGB_matrices_angle()
  {
   //Reading from the first potentiometer and converting to an angle from 0-180 deg
   angle_2 = analogRead(A1)*0.176;
     
   //Send position to Servo 6
   angle_corrected_6 = angle_2 + offset_6;
   servo6.write(angle_corrected_6);

   //Send position to Servo 7
   angle_corrected_7 = angle_2 + offset_7;
   servo7.write(angle_corrected_7);
  }


void Camera()
  {
   pot2 = analogRead(A3);
   distance_camera = map(pot2, 0,1023,0,180);
   servo5.write(distance_camera); 
  }

void Read_distance_sensor_1()
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
   
    // Convert the time into a distance
    cm = (duration/2) / 29.1;     
  
    // subtract the last reading
    total = total - readings[readIndex];
    // read from the sensor
    readings[readIndex] = cm;
    // add the reading to the total
    total = total + readings[readIndex];
    // advance to the next position in the array
    readIndex = readIndex + 1;
  
    // if we're at the end of the array...
    if (readIndex >= numReadings) 
    {
      // ...wrap around to the beginning
      readIndex = 0;
    }
    // calculate the average
    average = total / numReadings;
  }

void Read_distance_sensor_2()
  {
    digitalWrite(trigPin_2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_2, LOW);
    pinMode(echoPin_2, INPUT);
    duration_2 = pulseIn(echoPin_2, HIGH);
   
    // Convert the time into a distance
    cm_2 = (duration_2/2) / 29.1;     
  
    // subtract the last reading
    total_2 = total_2 - readings_2[readIndex_2];
    // read from the sensor
    readings_2[readIndex_2] = cm_2;
    // add the reading to the total
    total_2 = total_2 + readings_2[readIndex_2];
    // advance to the next position in the array
    readIndex_2 = readIndex_2 + 1;
  
    // if we're at the end of the array...
    if (readIndex_2 >= numReadings) 
    {
      // ...wrap around to the beginning
      readIndex_2 = 0;
    }
    // calculate the average
    average_2 = total_2 / numReadings;

  }

void Set_Matrix_Brightness()
  {
     //SET THE LEVEL OF BRIGHTNESS Warning: The maximum level is set at 200 because more will exceed the limit of 2.5A of the power supply
     
     BRIGHTNESS = map(analogRead(A2),0,1023,0,200);
     //BRIGHTNESS = 80; // This value ranges from 0-255 but more than 100 is not recommended because the current will be too high.
     matrix_1.setBrightness(BRIGHTNESS);
     matrix_2.setBrightness(BRIGHTNESS);

     matrix_1.fillScreen(matrix_1.Color(red, green, blue));
     matrix_1.show();
     matrix_2.fillScreen(matrix_1.Color(red, green, blue));
     matrix_2.show();
     
  }

  void display_values()
  {
     //Bottom distance 
     Serial.print(" Db: ");
     Serial.print(average_2);
     //Top distance 
     Serial.print(" Dt: ");
     Serial.print(average);
     //IR angle
     Serial.print(" IR: ");
     Serial.print(angle_1);
     //RGB angle
     Serial.print(" RGB: ");
     Serial.print(angle_2);
     //Camera distance
     Serial.print(" Cd: ");
     Serial.print(distance_camera);
     //RGB brightness
     Serial.print("Bright: ");
     Serial.print(BRIGHTNESS);
     Serial.println();
  }

  void Set_Matrix_Color_Brightness(byte Red_int, byte Green_int, byte Blue_int, byte brightness)
  {
     //Update Matrix 1
     matrix_1.setBrightness(brightness);  
     matrix_1.fillScreen(matrix_1.Color(Red_int, Green_int, Blue_int));
     matrix_1.show();
  
     //Update Matrix 2
     matrix_2.setBrightness(brightness);
     matrix_2.fillScreen(matrix_2.Color(Red_int, Green_int, Blue_int));
     matrix_2.show();
  }

  void read_irradiance()
  {
    sensors_event_t event;
    tsl.getEvent(&event);
    float lux = event.light;
    if (red_selected == true)
    {
      float irradiance = 0.0034*lux - 0.0133;
      Serial.println(irradiance);
    }
    if (green_selected == true)
    {
      float irradiance = 0.002*lux + 0.0874;
      Serial.println(irradiance);
    }
    if (blue_selected == true)
    {
      float irradiance = 0.0022*lux + 0.0921;
      Serial.println(irradiance);
    }
    
  }

  void intensity_ramp(byte max_value,unsigned int interval)
  {
    byte i = 0;
    while (i <= max_value)
    {
      if (timeElapsed > interval)
      {
        Set_Matrix_Color_Brightness(red, green, blue, i);
        Serial.println(i);
        timeElapsed = 0;
        i++;
      }
    }
  }
  
  void blinking(unsigned int interval_low, unsigned int interval)
  {
  unsigned long currentMillis = millis();    
  // RGB MATRICES SET COLOR NO COLOR
  Set_Matrix_Color_Brightness(red, green, blue, brightness_low);
  
  // Print intensity 0
  Serial.println(state);

  while (currentMillis > interval_low)
  {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) 
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
  
      // if the state is off turn it on and vice-versa:
      if (state == LOW) 
        {
          state = HIGH;
          // RGB matrices set high color
          Set_Matrix_Color_Brightness(red, green, blue, brightness);
        } 
      else 
        {
          state = LOW;
          // RGB matrices set low color
          Set_Matrix_Color_Brightness(red, green, blue, brightness_low);
        }
      Serial.println(state);
      }  
    Serial.println(state);
    }
  }

  void read_spectral_sensor()
  {
    uint16_t readings[12];
    float counts[12];

    if (!as7341.readAllChannels(readings))
    {
      Serial.println("Error reading all channels!");
      return;
    }

    for(uint8_t i = 0; i < 12; i++) 
    {
      if(i == 4 || i == 5) continue;
      // we skip the first set of duplicate clear/NIR readings
      // (indices 4 and 5)
      counts[i] = as7341.toBasicCounts(readings[i]);
    }

  Serial.print("F1 415nm : ");
  Serial.println(counts[0]);
  Serial.print("F2 445nm : ");
  Serial.println(counts[1]);
  Serial.print("F3 480nm : ");
  Serial.println(counts[2]);
  Serial.print("F4 515nm : ");
  Serial.println(counts[3]);
  Serial.print("F5 555nm : ");
  // again, we skip the duplicates  
  Serial.println(counts[6]);
  Serial.print("F6 590nm : ");
  Serial.println(counts[7]);
  Serial.print("F7 630nm : ");
  Serial.println(counts[8]);
  Serial.print("F8 680nm : ");
  Serial.println(counts[9]);
  Serial.print("Clear    : ");
  Serial.println(counts[10]);
  Serial.print("NIR      : ");
  Serial.println(counts[11]);
  Serial.println();
  
  delay(500);
  }
  
