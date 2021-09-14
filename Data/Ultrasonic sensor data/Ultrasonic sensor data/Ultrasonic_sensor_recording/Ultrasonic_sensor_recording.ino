
// ULTRASONIC SENSOR VARIABLES
int trigPin = 4;    // Trigger
int echoPin = 2;    // Echo
float duration, cm, inches;

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average


void setup() 
{
  //Setup Ultrasonic sensor 1
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
  {
    readings[thisReading] = 0;
  }
  Serial.begin(9600);
  Serial.println("CLEARDATA");
 Serial.println("LABEL,Time,average,raw");

}

void loop() 
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
    Serial.print("DATA,TIME,");
    Serial.print(average);
    Serial.print(",");
    Serial.print(cm);
    Serial.print(",");
    Serial.println(millis());
}
