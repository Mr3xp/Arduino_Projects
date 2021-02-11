#include <AFMotor.h>    //Library that is needed to use the L293D motor shield
#include<SoftwareSerial.h>   //Gives you the abillity to transforms digitals pins to be able to have serial comunication 
#include<Servo.h>   //Library for servo motor
#include<NewPing.h>  //It's an automating approach to find the distcane threw ultra sonic sensors
#define upright A4
#define upleft A5
#define downright 5
#define downleft 6

 //Assign its motor to the write position and frequency
AF_DCMotor DCMotor1(1,MOTOR12_64KHZ); 
AF_DCMotor DCMotor2(2,MOTOR12_64KHZ);
AF_DCMotor DCMotor3(3,MOTOR34_64KHZ);
AF_DCMotor DCMotor4(4,MOTOR34_64KHZ);


SoftwareSerial  BT(2,3); //Assign the pins that are connected to the bleutooth module 
Servo myServo;
NewPing sonar1(A0,A1,400); 
NewPing sonar2(A2,A3,400);

int motorspeed = 0;
int  LeftDistance = 0;
int  RightDistance = 0;
int FrontDistance = 0;
int flordistance = 0;
int Distance = 0;
char BtMode,Command; 

void setup() {
// Set the mode of the pins for the infared sensors
pinMode(upright,INPUT);
pinMode(upleft,INPUT);
pinMode(downright,INPUT);
pinMode(downleft,INPUT);

myServo.attach(9);
Serial.begin(9600);
BT.begin(9600);  //Begin serial communication with the bleutooth module and self-phone(via app)


  DCMotor1.setSpeed(120);
  DCMotor2.setSpeed(120);
  DCMotor3.setSpeed(120);
  DCMotor4.setSpeed(120); 
}

void loop() {
  Serial.println(BtMode);
  if(BT.available())
  {
    BtMode = BT.read();   //stores the values send from app in order to choose its mode
    if(BtMode=='1')   //user controls the vehicle
    {
      while(BtMode=='1')  // put's the programm in loop until it's given a different value
      {
        if(BT.available())  
        {
          Command = BT.read();  
          if(Command=='F')
          {   dontfall();      
            MoveForward();
            dontfall();
          }
          else if(Command=='R')
          {
            MoveRight();
            
          }
          else if(Command=='L')
          {
            MoveLeft();
           
          }
          else if(Command=='B')
          {
            MoveBackwards();
            
          }
          else if(Command=='Q')   //exit from the while loop so you can choose an other mode
          {
            StopMoving();
            break;
          }
        }
      }
    }
    else if(BtMode=='2')   //obstacle avoidance mode
    {
      while(BtMode=='2')
      {
        Scan(); 
        
        Navigate();
        if(BT.available())
        {
          Command = BT.read();
          if(Command=='Q')
          {
            StopMoving();
            break;
          }
        }
      }
    }
    else if(BtMode=='3')    //Following an object by infared sensors
    {
      while(BtMode=='3')
      {
       // Serial.println(BtMode);
        Command = BT.read();
        int right_value = digitalRead(upright);
        int left_value = digitalRead(upleft);
        if ((right_value ==0)&&(left_value==0))
        {
          MoveForward();
          Serial.println("Forward");
         
        }
        else if ((right_value==1)&&(left_value==0))
        {
          MoveRight();
           Serial.println(right_value);
        }  
        else if ((right_value==0)&&(left_value==1))
        {
          MoveLeft();
           Serial.println("left");
        }
        else if ( ((right_value==1)&&(left_value==1))) /*|| (distance >1 && distance<10))*/
        {
          StopMoving();
        }
   
        
        if(Command=='Q')
          {
           StopMoving();
           break;
          }
          
      }
    }
    /*else if(BtMode=='4')  //line following mode(it's commented because of a burned sensor)
    {
      while(BtMode=='4')
      {
        if(digitalRead(downright)==1 && digitalRead(downleft)==1)
        {
          MoveForward();  
        }
        else if(digitalRead(downright)==1 && digitalRead(downleft)==0)
        {
          MoveLeft();
        }
        else if(digitalRead(downright)==0 && digitalRead(downleft)==1)
        {
          MoveRight();
        }
        else if(digitalRead(downright)==0 && digitalRead(downleft)==0)
        {
          StopMoving();
        }
        if(BT.available())
          {
            Command = BT.read();
            if(Command=='Q')
            {
              StopMoving();
              break;
            }
          }
        }
      }
    }*/
  }
}

//Function that controlls the car by checking the downward fessing ultrasonic sensor so it dont fall.
void dontfall ()
{
  int flordistance = sonar2.ping_cm();    
  if (flordistance > 7 )
  {StopMoving();
    MoveBackwards();
    delay(20);
    StopMoving();
  }   
}

//Function that controlls the car by checking the forward fessing ultrasonic sensor so it check and returns the distance.
int Scan()
{
   
  Distance = sonar1.ping_cm();
  return Distance;
}


//
void Navigate ()   // fuction that control the car accordingly by assessting the values from the ultra sonic sensors
{Serial.println(Distance);
  if(Distance<=14)
    {
      StopMoving();
      delay(100);
      MoveBackwards2();
      delay(300);
      StopMoving();
      delay(100);
      servo(30);
      Scan();
      RightDistance=Distance;
      delay(100);
      servo(150);
      Scan();
      LeftDistance=Distance;
      servo(90);
      if(LeftDistance<=RightDistance)
      {
        MoveRight2();
        delay(400);
        StopMoving();
      }
      else 
      {
        MoveLeft2();
        delay(400);
        StopMoving();
      }
    }
    do
    {
      MoveForward2();
      
      Scan();
      Serial.println(Distance);
    }while(Distance>14);
  StopMoving();
  Scan();
}

void servo(int thita)
{
  myServo.write(thita);
  delay(1000); 
}


//Function that controlls the motors so the car move forward
void MoveForward()
{
  DCMotor1.run(FORWARD);
  DCMotor2.run(FORWARD);
  DCMotor3.run(FORWARD);
  DCMotor4.run(FORWARD);
   delay(45);
   StopMoving();
}
void MoveForward2()
{
  DCMotor1.run(FORWARD);
  DCMotor2.run(FORWARD);
  DCMotor3.run(FORWARD);
  DCMotor4.run(FORWARD);  
}
void MoveBackwards2()
{
  

  DCMotor1.run(BACKWARD);
  DCMotor2.run(BACKWARD);
  DCMotor3.run(BACKWARD);
  DCMotor4.run(BACKWARD);
  
   
}
//Function that controlls the motors so the car move forward
void MoveBackwards()
{
  

  DCMotor1.run(BACKWARD);
  DCMotor2.run(BACKWARD);
  DCMotor3.run(BACKWARD);
  DCMotor4.run(BACKWARD);
  delay(45);
   StopMoving();
}
void MoveRight2()
{
  DCMotor1.run(FORWARD);
  DCMotor2.run(FORWARD);
  DCMotor3.run(BACKWARD);
  DCMotor4.run(BACKWARD);
}
//Function that controlls the motors so the car move right
void MoveRight()
{
  DCMotor1.run(FORWARD);
  DCMotor2.run(FORWARD);
  DCMotor3.run(BACKWARD);
  DCMotor4.run(BACKWARD);
  delay(50);
  StopMoving();
}
void MoveLeft2()
{
  DCMotor1.run(BACKWARD);
  DCMotor2.run(BACKWARD);
  DCMotor3.run(FORWARD);
  DCMotor4.run(FORWARD);
  
}
//Function that controlls the motors so the car move left
void MoveLeft()
{
  DCMotor1.run(BACKWARD);
  DCMotor2.run(BACKWARD);
  DCMotor3.run(FORWARD);
  DCMotor4.run(FORWARD);
  delay(50);
  StopMoving();
}

//Function that stops the car
void StopMoving()
{
  DCMotor1.run(RELEASE);
  DCMotor2.run(RELEASE);
  DCMotor3.run(RELEASE);
  DCMotor4.run(RELEASE);
}
