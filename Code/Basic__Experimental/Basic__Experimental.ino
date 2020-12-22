#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#include <Servo.h>


// delay in milliseconds
unsigned int interval = 1000;
unsigned long previousMillis;

bool state = LOW;

//MATRICES VARIABLES
#define DATAPIN_1    10
#define CLOCKPIN_1   11

#define DATAPIN_2    12
#define CLOCKPIN_2   13

#define DATAPIN_3    28
#define CLOCKPIN_3   26

#define DATAPIN_4    24
#define CLOCKPIN_4   22


#define SHIFTDELAY 100
int BRIGHTNESS = 80;

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

////Angle corrected variable for the RGB Matrix
//int  angle_corrected_5 = 0;
int  angle_corrected_6 = 0;
int  angle_corrected_7 = 0;
//int  angle_corrected_8 = 0;

// Servo objects
Servo servo1; 
Servo servo2; 
Servo servo3; 
Servo servo4; 
Servo servo5; 
Servo servo6; 
Servo servo7; 
//Servo servo8; 

int serialRead;

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
  // matrix_1.setBrightness(BRIGHTNESS);

  matrix_2.begin();
  //matrix_2.setBrightness(BRIGHTNESS);

  matrix_3.begin();
  //matrix_3.setBrightness(BRIGHTNESS);

  matrix_4.begin();
  //matrix_4.setBrightness(BRIGHTNESS);

  // CAMERA DISTANCE
  Camera();

  // IR ARRAYS ANGLE
  IR_Lights_angle();
    
  // RGB MATRICES ANGLE
  RGB_matrices_angle();
    
  // RGB MATRICES SET BRIGHTNESS
  Set_Matrix_Brightness();
}

void loop() 
{

unsigned long currentMillis = millis();
      
// RGB MATRICES SET COLOR NO COLOR
Set_Matrix_Color_0();

// If there is no color it will print 0
Serial.println(state);
//Serial.print(";");
//Serial.println(millis());

  while (currentMillis > 10000)
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
          // RGB MATRICES SET COLOR RED
          Set_Matrix_Color_Red();
        } 
      else 
        {
          state = LOW;
          // RGB MATRICES SET COLOR NO COLOR
          Set_Matrix_Color_0();
        }
      Serial.println(state);
  //    Serial.print(";");
  //    Serial.println(millis());
      }  
    Serial.println(state);
  //  Serial.print(";");
  //  Serial.println(millis()); 
  } 
}

void Read_serial()
  {
    if (Serial.available() > 0) 
      {
        serialRead  = Serial.parseInt(); // Convert the string read from the serial to integer
        if (serialRead  == 1) 
          {
            //Do something
          }
      }
  }
void IR_Lights_angle()
  {
   //Reading from the first potentiometer and converting to an angle from 0-180 deg
   angle_1 = analogRead(A0)*0.176;
   
   //Send position to Servo 1
   angle_corrected_1 = angle_1;
   servo1.write(angle_corrected_1);
 
   //Send position to Servo 2
   angle_corrected_2 = angle_1 ;
   servo2.write(angle_corrected_2);
 
   //Send position to Servo 3
   angle_corrected_3 = angle_1 -10;
   servo3.write(angle_corrected_3);

   //Send position to Servo 4
   angle_corrected_4 = angle_1;
   servo4.write(angle_corrected_4);
  }

void RGB_matrices_angle()
  {
   //Reading from the first potentiometer and converting to an angle from 0-180 deg
   angle_2 = analogRead(A2)*0.176;
     
   //Send position to Servo 6
   angle_corrected_6 = angle_2;
   servo6.write(angle_corrected_6);

   //Send position to Servo 7
   angle_corrected_7 = angle_2;
   servo7.write(angle_corrected_7);
  }


void Camera()
  {
   pot2 = analogRead(A1);
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
     //SET THE LEVEL OF BRIGHTNESS Warning: The maximum level is set at 100 because more will exceed the limit of 2.5A of the power supply
     
     //BRIGHTNESS = map(analogRead(A2),0,1023,0,100);

     //BRIGHTNESS = 80; // This value ranges from 0-255 but more than 100 is not recommended because the current will be too high.
     matrix_1.setBrightness(BRIGHTNESS);
     matrix_2.setBrightness(BRIGHTNESS);
     //matrix_3.setBrightness(BRIGHTNESS);
     //matrix_4.setBrightness(BRIGHTNESS);
  }

void Set_Matrix_Color_Red()
  {
    //Red color Matrix 1
   matrix_1.fillScreen(matrix_1.Color(255, 0, 0));
   matrix_1.show();
  
   //Red color Matrix 2
   matrix_2.fillScreen(matrix_2.Color(255, 0, 0));
   matrix_2.show();
  
//   //Red color Matrix 3
//   matrix_3.fillScreen(matrix_3.Color(255, 0, 0));
//   matrix_3.show();
//  
//   //Red color Matrix 4
//   matrix_4.fillScreen(matrix_4.Color(255, 0, 0));
//   matrix_4.show();
  }

void Set_Matrix_Color_Blue()
  {
   //Blue color Matrix 1
   matrix_1.fillScreen(matrix_1.Color(0, 255, 225));
   matrix_1.show();

   //Blue color Matrix 2
   matrix_2.fillScreen(matrix_2.Color(0, 255, 225));
   matrix_2.show();

   //Blue color Matrix 3
   //matrix_3.fillScreen(matrix_3.Color(0, 255, 225));
   //matrix_3.show();

   //Blue color Matrix 4
   //matrix_4.fillScreen(matrix_4.Color(0, 255, 225));
   //matrix_4.show();
  }

 void Set_Matrix_Color_0()
  {
   //Blue color Matrix 1
   matrix_1.fillScreen(matrix_1.Color(0, 0, 0));
   matrix_1.show();

   //Blue color Matrix 2
   matrix_2.fillScreen(matrix_2.Color(0, 0, 0));
   matrix_2.show();

   //Blue color Matrix 3
   //matrix_3.fillScreen(matrix_3.Color(0, 0, 0));
   //matrix_3.show();

   //Blue color Matrix 4
   //matrix_4.fillScreen(matrix_4.Color(0, 0, 0));
   //matrix_4.show();
  }
