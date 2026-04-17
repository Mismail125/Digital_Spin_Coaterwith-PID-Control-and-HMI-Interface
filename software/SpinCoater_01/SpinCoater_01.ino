#include <elapsedMillis.h>
#include <FreqMeasure.h>
#include <ServoTimer2.h>
#include <PID_v1.h>
#include "EasyNextionLibrary.h"
const int motorPin = 5; 
EasyNex myNex(Serial);
int flag = 1;
elapsedMillis timeElapsed;
int progress;
double Kp = 0.0;  // Proportional gain 0.024
double Ki = 0.33;  // Integral gain
double Kd = 0.00; // Derivative gain

ServoTimer2 motorESC;
double setpoint = 0;
double input = 0.0;
double output = 0.0;
double sum = 0;
int count = 0;
float rpm = 0;
float targetrpm;
long ramptime,coatingtime;
int state = 0;

PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  motorESC.attach(motorPin);
  myNex.begin(115200);
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(1000, 2000);
  motorESC.write(1000);
  delay(3000);
 
 }

void loop() {
  input = 0.0;
  myNex.NextionListen(); 
  
}

//______________________Ramping Up____________________________//
void Start(){
   pid.SetMode(AUTOMATIC);
   targetrpm =  myNex.readNumber("n0.val");
   ramptime =  myNex.readNumber("n1.val")*1000;
   coatingtime = myNex.readNumber("n2.val")*1000;
   myNex.NextionListen();
   FreqMeasure.begin();
   delay(500);
   myNex.writeStr("page 1");
   myNex.writeStr("p1t2.txt","Ramp Up");
   myNex.writeStr("p1t0.txt","Setpoint");
   myNex.writeNum("p1n0.val",targetrpm );
   timeElapsed = 0;

   while (rpm<targetrpm&flag ==1)//(timeElapsed<ramptime)
      {
      setpoint = map(timeElapsed,0,ramptime,300,targetrpm);
      progress = map(timeElapsed,0,ramptime,0,100);
      if (progress<0){progress=0;}
      if (progress>100){progress=100;}
      myNex.writeNum("j1.val",progress );
      calculateRpm();
      pid.Compute();
      motorESC.write(output);    
      myNex.writeNum("p1n1.val",rpm );
      delay(5);
      myNex.NextionListen();
     }
    if (flag ==1){coat();}
     }
    //__________________Coating___________________//
  
     void coat(){
      myNex.writeStr("p1t2.txt","Coating...");
      myNex.writeStr("p1t0.txt","Coating Time(Sec)");
      myNex.writeNum("p1n0.val",coatingtime/1000 );
      myNex.writeStr("p1t1.txt","Elapsed Time(Sec)");
      timeElapsed = 0; 
      while(timeElapsed<coatingtime&flag==1)
      {
      motorESC.write(output); 
      progress = map(timeElapsed,0,coatingtime,0,100);
      myNex.writeNum("p1n1.val",timeElapsed/1000);
      if (progress<0){progress=0;}
      if (progress>100){progress=100;}
      myNex.writeNum("j1.val",progress);
      myNex.NextionListen();
      }
      if (flag ==1){ramp();}
      }
      //_________________Ramping Down___________________//
      void ramp()
      {
      myNex.writeStr("p1t2.txt","Ramp Down");
      myNex.writeStr("p1t0.txt","Target RPM");
      myNex.writeNum("p1n0.val",0);
      myNex.writeStr("p1t1.txt","CurrentRPM");
      myNex.writeNum("p1n1.val",rpm);
      timeElapsed = 0; 
      while(timeElapsed<ramptime&flag==1)
      {
      setpoint = map(timeElapsed,ramptime,0,250,targetrpm);
      progress = map(timeElapsed,ramptime,0,100,0);
      if (progress<0){progress=0;}
      if (progress>100){progress=100;}
      myNex.writeNum("j1.val",progress);
      calculateRpm();
      pid.Compute();
      motorESC.write(output);    
      myNex.writeNum("p1n1.val",rpm );
      delay(5);
      myNex.NextionListen();
      }           
      Stop();
      }

//__________________________________________________//
 void calculateRpm()
 {   
   if (FreqMeasure.available()) 
   {
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
     // Serial.println(“measuring”);
      if (count > 5) 
       {
        float frequency = FreqMeasure.countToFrequency(sum / count);
        rpm = (frequency /14) *60;
        sum = 0;
        count = 0;
       }
    }
   input = rpm;
}
//___________________________________________________//
void Stop()
{

  while (output> 1000)
  {output = output-50;
  motorESC.write(output);
  //pid.Compute();
  delay(60);
}

motorESC.write(1000);
pid.SetMode(MANUAL);
output = 1000;
flag = 0;
rpm=0;
myNex.writeStr("page 0");
}


//___________________________________________________//
   
   
   void trigger0(){

        flag = 1;
        Start(); 
        //page1.show();  

        }
   void trigger1(){

        Stop();   
       
        }
