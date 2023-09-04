#include<Servo.h> // include server library

#define enA 8
#define in1 6
#define in2 7


#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5


Servo ser, ser1; // create servo object to control a servo
int poser, pos_2; // initial position of server pos_2 for the second servo
int val; // initial value of input
int pos_lim;

long duration;
int distance, inches;

int MAX_ANGLE = 106;
int MIN_ANGLE = 10;

int motor_speed=0;
int i_r, i_l;

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600); // Serial comm begin at 9600bps
ser.attach(9);// servo is connected at pin 9 
//ser1.attach(10); //Servo 2 at pin 10 

pinMode(enA, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);

// Set initial rotation direction
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

poser=44;

}



void loop() {
  // put your main code here, to run repeatedly:

if (Serial.available()){
  
val = Serial.read();// then read the serial value
 
  if (val == 'c') //if value input is equals to c Center steering command

    {
      //center steering
      poser = 44; //0 Steering
      //poser=position_err(poser,MAX_ANGLE, MIN_ANGLE );
      ser.write(poser);// the servo will move according to position
      delay(15);//delay for the servo to get to the position

    }

    if (val == 'd') //if value input is equals to d

    {
    //steer right
    poser += 2; //than position of servo motor increases by 1 ( anti clockwise)
    //poser=position_err(poser,MAX_ANGLE, MIN_ANGLE );
    ser.write(poser);// the servo will move according to position
    delay(15);//delay for the servo to get to the position
      
    }

   if (val == 'a') //if value input is equals to a

    {
    //steer left
    poser -= 2; //than position of servo motor decreases by 1 (clockwise)
    //poser=position_err(poser,MAX_ANGLE, MIN_ANGLE );
    ser.write(poser);// the servo will move according to position
    delay(15);//delay for the servo to get to the position
    
    }

    //*****MAIN MOTOR******//
}      
      
     
      if(val=='w'){//mptor fwd
        
      motor_speed=355;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
        
      }

     if(val=='t'){//motor increment
        
      motor_speed+=5;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
        
      }

      if(val=='s'){//motor back
        
      motor_speed=355;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);      
  
      }

      if(val=='v'){//motor stop
        
      motor_speed=0;
      analogWrite(enA, motor_speed);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
        
      }
   

Serial.print("\nSteer Value read ");
Serial.print(poser);

Serial.print("\nmotor Value read ");
Serial.print(motor_speed);

//Serial.print("\nservo Value read pos_2 ");
//Serial.print(pos_2);






}
////******END OF MAIN LOOP********///////
/*

list of funtions



*/

int position_err(int val, int max, int min)
{
  int val_lim;

  if(val >= max)
  {  
   val_lim=max; 
  }
  else if(val <= min)
  {
   val_lim=min;
  }
  else
  {
   val_lim=val;  
  }
  
  return val_lim;
  
}
