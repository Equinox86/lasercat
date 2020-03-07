#include <Servo.h> 
Servo laser_head; 

#define MAX_ANGLE 120
int lpos =0;
int sweep_time = 15; 
void setup() {
  // put your setup code here, to run once:
laser_head.attach(9);
Serial.begin(9600);
pinMode(5,OUTPUT);
randomSeed(analogRead(0));

}

void run_sweep(unsigned long stop_time) { 
  //Turn on the Laser
  digitalWrite(5,HIGH);

  while(millis() < stop_time) {
    sweep_time= random(10)+5;
    for(lpos=0; lpos <= MAX_ANGLE; lpos++){ 
      laser_head.write(lpos);
      delay(sweep_time); 
      Serial.println(lpos);
      printf(lpos);
    }
    sweep_time= random(10)+5;
    for(lpos=MAX_ANGLE; lpos >=0;lpos--){ 
      laser_head.write(lpos);
      Serial.println(lpos);
      delay(sweep_time); 
    }
  }  

  //Turn laser off 
  digitalWrite(5,LOW);
}

void loop() {
  //Set the next start time; 
  unsigned long next_start = 10000; //10 second test
  delay(next_start); 
  run_sweep(millis() + (1000 * 180)); // Run for 3 minutes
}
