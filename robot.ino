

#include <Servo.h> 

int pinLB=14;     // Define a 14 Pin
int pinLF=15;     // Define a 15 Pin

int pinRB=16;    // Define a 16 Pin
int pinRF=17;    // Define a 17 Pin

//int MotorLPWM=5;  //Define a 5 Pin
//int MotorRPWM=6;  //Define a 6 Pin

int inputPin = 9;  // Define the ultrasound signal receiving a Pin
int outputPin = 8;  //Define the ultrasound signal emission Pin

int Fspeedd = 0;      // go
int Rspeedd = 0;      // The right to
int Lspeedd = 0;      // Turn left to
int directionn = 0;   // After the former = 8 = 2 left = 4 right = 6 

Servo myservo;        // Set up the myservo

int delay_time = 250; // After the servo motor to the stability of the time
const int LOOK_FORWARD = 125;
const int LOOK_RIGHT = 185;
const int LOOK_LEFT = 65;


int Fgo = 8;         // go
int Rgo = 6;         // The right to
int Lgo = 4;         // Turn left to
int Bgo = 2;         // astern


void setup()
{
  Serial.begin(9600);     // Initialize 
  pinMode(pinLB,OUTPUT); // Define 14 pin for the output (PWM)
  pinMode(pinLF,OUTPUT); // Define 15 pin for the output (PWM)
  pinMode(pinRB,OUTPUT); // Define 16 pin for the output (PWM) 
  pinMode(pinRF,OUTPUT); // Define 17 pin for the output (PWM)



  //pinMode(MotorLPWM,  OUTPUT);  // Define 5 pin for PWM output 
  // pinMode(MotorRPWM,  OUTPUT);  // Define 6 pin for PWM output

  pinMode(inputPin, INPUT);    // Define the ultrasound enter pin
  pinMode(outputPin, OUTPUT);  // Define the ultrasound output pin   

  myservo.attach(10);    // Define the servo motor output 10 pin(PWM)
}

void advance(int a)     // go
{
  digitalWrite(pinRB,HIGH);  // 16 feet for high level
  digitalWrite(pinRF,LOW);   //17 feet for low level

  //analogWrite(MotorRPWM,180);//Set the output speed(PWM)

  digitalWrite(pinLB,HIGH);  // 14 feet for high level
  digitalWrite(pinLF,LOW);   //15 feet for high level

  //analogWrite(MotorLPWM,180);//Set the output speed(PWM)
  delay(a * 1);     
}



void right(int b)        //right
{
  digitalWrite(pinRB,LOW);   
  digitalWrite(pinRF,HIGH);

  //analogWrite(MotorRPWM,250);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,LOW);
  delay(b * 100);
}

void left(int c)         //left
{
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,LOW);   
  digitalWrite(pinLF,HIGH);

  //analogWrite(MotorLPWM,250);
  delay(c * 100);
}

void turnR(int d)        //right
{
  digitalWrite(pinRB,HIGH);  
  digitalWrite(pinRF,LOW);

  //analogWrite(MotorRPWM,250);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,HIGH);  

  //analogWrite(MotorLPWM,250);
  delay(d * 50);
}

void turnL(int e)        //left
{
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);   
  //analogWrite(MotorRPWM,220);
  digitalWrite(pinLB,HIGH);   
  digitalWrite(pinLF,LOW);
  //analogWrite(MotorLPWM,220);
  delay(e * 50);
}    

void stopp(int f)         //stop
{
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,LOW);
  delay(f * 100);
}

void back(int g)          //back
{
  digitalWrite(pinRB,LOW);  
  digitalWrite(pinRF,HIGH);
  //analogWrite(MotorRPWM,0);
  digitalWrite(pinLB,LOW);  
  digitalWrite(pinLF,HIGH);
  //analogWrite(MotorLPWM,230);
  delay(g * 500);     
}



void detection()        //Measuring three angles(0.90.179)
{      
  int delay_time = 200;   // After the servo motor to the stability of the time
  ask_pin(LOOK_FORWARD, Fspeedd, "Front");            // Read in front of the distance  

  if(Fspeedd < 10)         // If the front distance less than 10 cm
  {
    stopp(1);               // Remove the output data 
    back(2);                // The back two milliseconds
  }

  if(Fspeedd < 25)         // If the front distance less than 25 cm
  {
    stopp(1);               // Remove the output data
    ask_pin(LOOK_LEFT, Lspeedd, "Left");            // Read the left distance
    delay(delay_time);      // Waiting for the servo motor is stable
    ask_pin(LOOK_RIGHT, Rspeedd, "Right");            // Read the right distance  

    delay(delay_time);      //  Waiting for the servo motor is stable  
    if(Lspeedd > Rspeedd)   //If the distance is greater than the right distance on the left
    {
      directionn = Lgo;      //Left
    }

    if(Lspeedd <= Rspeedd)   //If the distance is less than or equal to the distance on the right
    {
      directionn = Rgo;      //right
    } 

    if (Lspeedd < 15 && Rspeedd < 15)   /*If the left front distance and distance and the right distance is less than 15 cm */
    {
      directionn = Bgo;      //Walk backwards        
    }          
  }
  else                      //If the front is not less than 25 cm (greater than)    
  {
    directionn = Fgo;        //Walk forward    
  }

}    

void ask_pin(int servo_pos, int &dir, const char* pinDir)   // Measure the distance on the left 
{
  Serial.print("servo_pos passed:::  ");
  Serial.println(servo_pos);
  myservo.write(servo_pos);
  delay(delay_time);
  digitalWrite(outputPin, LOW);   // For low voltage 2 us ultrasonic launch
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);  // Let ultrasonic launch 10 us high voltage, there is at least 10 us
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);    // Maintaining low voltage ultrasonic launch
  float dist = pulseIn(inputPin, HIGH);  // Read the time difference
  dist= dist/5.8/10;       // Will be time to distance distance (unit: cm)
  Serial.print(pinDir);
  Serial.print(" distance: ");  //The output distance (unit: cm)
  Serial.println(dist);         //According to the distance
  dir = dist;              // Will reading Lspeedd distance
}

void loop()
{
  myservo.write(100);  /*Make the servo motor ready position Prepare the next measurement */
  detection();        //Measuring Angle And determine which direction to go to
  if(directionn == 2)  //If directionn (direction) = 2 (back)          
  {
    back(8);                    //  back
    turnL(2);                   //Move slightly to the left (to prevent stuck in dead end lane)
    Serial.print(" Reverse ");   //According to the direction (reverse)
  }

  if(directionn == 6)           //If directionn (direction) = 6 (right)   
  {
    back(1); 
    turnR(6);                   // right
    Serial.print(" Right ");    //According to the direction (Right)
  }

  if(directionn == 4)          //If directionn (direction) = 4 (left)   
  {  
    back(1);      
    turnL(6);                  // left
    Serial.print(" Left ");     //According to the direction (Left)  
  }  

  if(directionn == 8)          //If directionn (direction) = 8 (forward)      
  { 
    advance(1);                 // go 
    Serial.print(" Advance ");   //According to the direction (Advance)
    Serial.print("   ");    
  }

}

