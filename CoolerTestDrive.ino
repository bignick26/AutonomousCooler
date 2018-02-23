//Nicholas Restivo

#include <Servo.h>

Servo motorForward;
Servo motorTurn;

int motorValForward = 93;
int motorValTurn = 93;
bool count = 0;
int currentRead;

const int numReadings = 30;
float readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

float currentValue = 0;

//RCin
byte PWM_PIN3 = 3;
byte PWM_PIN5 = 5;
 
float pwm_value3;
float pwm_value5;

bool RCon = 0;

void setup() {
  Serial.begin(9600);
  motorForward.attach(9); //analog pin A1
  motorTurn.attach(10); //analog pin A2

  pinMode(PWM_PIN3, INPUT);
  pinMode(PWM_PIN5, INPUT);

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   

  attachInterrupt(1, rising1, RISING);
}

void loop() {

  if (RCon) {
    pwm_value3 = pulseIn(PWM_PIN3, HIGH);
    pwm_value5 = pulseIn(PWM_PIN5, HIGH);  
    
    pwm_value3 = round(pwm_value3/15.6);
    pwm_value5 = round(pwm_value5/15.7);
    //Serial.print(int(pwm_value3));
    //Serial.print(", ");
    //Serial.println(int(pwm_value5));
    if (pwm_value3 != 0 && pwm_value5 != 0) {
      motorValForward = pwm_value5;
      motorValTurn = pwm_value3;
    } 
  }
  else {
    if (Serial.available() > 0 && !count) {
      float input = Serial.parseFloat(); // needs to be around 50-200!! 
      motorValForward = input;
      Serial.println(motorValForward);
      count = 1;
    }
  
    if (Serial.available() > 0 && count) {
      float input = Serial.parseFloat(); // needs to be around 50-200!! 
      motorValTurn = input;
      Serial.println(motorValTurn);
      count = 0;
    }
  }

  if (pwm_value3 == 0 || pwm_value5 == 0) {
    RCon = 0;
    attachInterrupt(1, rising1, RISING);
  }

  currentRead = getCurrent();
  //Serial.println(currentRead);
  if (currentRead >= 20) {
    motorValForward = 93;
    motorValTurn = 93;
  }

  motorForward.write(motorValForward);
  delay(10);
  motorTurn.write(motorValTurn);
  Serial.print(int(motorValForward));
  Serial.print(", ");
  Serial.println(int(motorValTurn));
  
}

int getCurrent()
{   
    total= total - readings[index];          
    readings[index] = analogRead(0); //Raw data reading
//Data processing:510-raw data from analogRead when the input is 0;
// 5-5v; the first 0.04-0.04V/A(sensitivity); the second 0.04-offset val;
    readings[index] = (readings[index]-512)*5/1024/0.04-0.12;
                                                       
    total= total + readings[index];       
    index = index + 1;                    
    if (index >= numReadings)              
      index = 0;                           
    average = total/numReadings;   //Smoothing algorithm (http://www.arduino.cc/en/Tutorial/Smoothing)    
    currentValue= average;
    return currentValue;
}

void rising1() {
  RCon = 1;
  detachInterrupt(1);
}


