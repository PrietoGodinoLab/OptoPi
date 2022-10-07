#include <Servo.h>

Servo servo;  // create servo object to control a servo


void setup() {
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  servo.write(180);                  // sets the servo position according to the scaled value
                         // waits for the servo to get there
}
