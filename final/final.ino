#include <Adafruit_MotorShield.h>


//set up pin mode
#define bp A1
#define l A2
#define tl 4
#define el 5
#define tr 2
#define er 3

//set up motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(4);

void setup() {
  AFMS.begin();
  // put your setup code here, to run once:
  pinMode(tl, OUTPUT);
  pinMode(el, INPUT);
  pinMode(tr, OUTPUT);
  pinMode(er, INPUT);
  pinMode(bp, OUTPUT);
  pinMode(l, INPUT);
  Serial.begin(9600);
}

//drive left and right motor at speed ls adn rs (negative means backword)
void drive(int ls, int rs) {
  if (ls > 0)
  { M1->setSpeed(ls);
    M1->run(FORWARD);
  } else {
    M1->setSpeed(-ls);
    M1->run(BACKWARD);
  }
  if (rs > 0)
  { M2->setSpeed(rs);
    M2->run(FORWARD);
  } else {
    M2->setSpeed(-rs);
    M2->run(BACKWARD);
  }

}
// Set the speed to start, from 0 (off) to 255 (max speed)


//Use ultrasonic sensor
long distance(bool side) {
  long duration, distance;
  if (side == 0) {
    digitalWrite(tl, LOW);
    delayMicroseconds(2);
    digitalWrite(tl, HIGH);
    delayMicroseconds(10);
    digitalWrite(tl, LOW);
    duration = pulseIn(el, HIGH);
    distance = (duration / 2) / 29.1;
    Serial.print("left ");
  } else {
    digitalWrite(tr, LOW);
    delayMicroseconds(2);
    digitalWrite(tr, HIGH);
    delayMicroseconds(10);
    digitalWrite(tr, LOW);
    duration = pulseIn(er, HIGH);
    distance = (duration / 2) / 29.1;
    Serial.print("right ");
  }
  Serial.println(distance);
  return distance;
}
//use photoresistor
int light() {
  long bright = analogRead(l);
  Serial.println(bright);
  return bright;
}
//use beeper
void beep() {
  digitalWrite(bp, HIGH);
  delay(100);
  digitalWrite(bp, LOW);
}

void loop() {
  if (light() < 500) { //if it is dark enough, drive normally
    drive(80, 80);
    if (distance(1) < 20) {// if left ultrasonic senses
      delay(5);
      if (distance(0) < 20) {// or if right ultrasonic senses
        drive(80, -80);  //turnn
        delay(1000);
      }
    }
  } else {  //if sees red light, stop and beep
    drive(0, 0);  
    beep();
  }
}
