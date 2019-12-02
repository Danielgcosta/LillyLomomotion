/*
 * Daniel Guimarães Costa
 * The British School Rio de Janeiro Barra Site
 * 07 Sept 2019
 * Controlled robot 4 wheeled robot for parents meeting on 09 Sept 2019
 */

// Motor configuration
#define motor1Port1 53
#define motor1Port2 52
#define motor2Port1 51
#define motor2Port2 50
#define motor3Port1 49
#define motor3Port2 48
#define motor4Port1 47
#define motor4Port2 46

#define motor1Enable 2
#define motor2Enable 3
#define motor3Enable 4
#define motor4Enable 5

int mot1MaxPower = 100;  
int mot2MaxPower = 100;   
int mot3MaxPower = 100;   
int mot4MaxPower = 100;

// Radio control configuration
#define channel1 A0
#define channel2 A2
int stickThreshold = 10;
int motorPower;
int leftActivation;
int rightActivation;

void setup() {
  pinMode(channel1,INPUT);  // Radio control inputs
  pinMode(channel2,INPUT);
  
  pinMode(motor1Port1,OUTPUT);  // Motors
  pinMode(motor1Port2,OUTPUT);
  pinMode(motor2Port1,OUTPUT);
  pinMode(motor2Port2,OUTPUT);
  pinMode(motor3Port1,OUTPUT);
  pinMode(motor3Port2,OUTPUT);
  pinMode(motor4Port1,OUTPUT);
  pinMode(motor4Port2,OUTPUT);
  pinMode(motor1Enable,OUTPUT);
  pinMode(motor2Enable,OUTPUT);
  pinMode(motor3Enable,OUTPUT);
  pinMode(motor4Enable,OUTPUT);  
}

void activateMotor(int enablePin, int motorPort1, int motorPort2, int
enable, byte motora, byte motorb, int delayTime = 0){
  if(enable >120){
    enable=120;  //Protection against overvoltage
  }
  analogWrite(enablePin,enable);
  digitalWrite(motorPort1,motora);
  digitalWrite(motorPort2,motorb);
  delay(delayTime);
}

void walkForward(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,mot1MaxPower,HIGH,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,mot2MaxPower,HIGH,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,mot3MaxPower,HIGH,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,mot4MaxPower,HIGH,LOW);
}

void stopCar(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,0,LOW,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,0,LOW,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,0,LOW,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,0,LOW,LOW);
}

void radioControlActivation(){
  int left = leftActivation*motorPower/100;
  int right = rightActivation*motorPower/100;
  Serial.print("M:");
  Serial.print(motorPower);
  Serial.print(" L:");  
  Serial.print(leftActivation);
  Serial.print(" R:");  
  Serial.print(rightActivation);
  Serial.print(" 1:");  
  Serial.print(left*mot1MaxPower/100);
  Serial.print(" 2:");  
  Serial.print(right*mot2MaxPower/100);
  Serial.print(" 3:");  
  Serial.print(left*mot3MaxPower/100);
  Serial.print(" 4:");  
  Serial.print(right*mot4MaxPower/100);
  Serial.print(" ");  
  Serial.print(left*mot1MaxPower/100);
  Serial.println(" ");  
  
  if(left>=0){
    activateMotor(motor1Enable,motor1Port1,motor1Port2,left*mot1MaxPower/100,HIGH,LOW); // front left
    activateMotor(motor3Enable,motor3Port1,motor3Port2,left*mot3MaxPower/100,HIGH,LOW); // rear left
  }
  else{    
    activateMotor(motor1Enable,motor1Port1,motor1Port2,abs(left)*mot1MaxPower/100,LOW,HIGH);
    activateMotor(motor3Enable,motor3Port1,motor3Port2,abs(left)*mot3MaxPower/100,LOW,HIGH);
  }
  if(right>=0){
    activateMotor(motor2Enable,motor2Port1,motor2Port2,right*mot2MaxPower/100,HIGH,LOW);  //front right
    activateMotor(motor4Enable,motor4Port1,motor4Port2,right*mot4MaxPower/100,HIGH,LOW);
  }
  else{
    activateMotor(motor2Enable,motor2Port1,motor2Port2,abs(right)*mot2MaxPower/100,LOW,HIGH);  
    activateMotor(motor4Enable,motor4Port1,motor4Port2,abs(right)*mot4MaxPower/100,LOW,HIGH);
  }
}

void radioControl(){  
  long pulse1 = pulseIn(channel1,HIGH);
  long pulse2 = pulseIn(channel2,HIGH);

  // Channel 1 right stick
  // < 1450 to 1230 left
  // > 1500 to 1730 right

  if(pulse1<1430){
    leftActivation = map(pulse1,1280,1430,0,100);
    if(leftActivation > 100){
      leftActivation= 100;
    }
    else if(pulse1 < 1280){
      leftActivation = map(pulse1,1230,1280,-100,0);
      if(leftActivation < -100){        
        leftActivation = -100;
      }
    }
    rightActivation = 100;
  }
  else if(pulse1>1500){
    rightActivation = map(pulse1,1500,1650,100,0); 
    if(rightActivation > 100){
      rightActivation = 100; 
    }
    else if(pulse1 >1650){
      rightActivation = map(pulse1,1650,1750,0,-100);
      if(rightActivation < -100){
        rightActivation = -100;
      }
    } 
    leftActivation = 100;
  }
  /*
  Serial.print("1: ");
  Serial.print(pulse1);
  Serial.print(" - 2: ");
  Serial.print(pulse2);*/

  // Channel 2 acceleration
  // 2150 to 850

  stickThreshold = 20;
  motorPower = map(pulse2,2150,850,-100,100);
  if((motorPower < stickThreshold)&&(motorPower >-stickThreshold)){
    motorPower = 0;
  }
  else if(motorPower>=stickThreshold){
    motorPower = (motorPower-stickThreshold)*100/(100-stickThreshold);
  }
  else{    
    motorPower = (motorPower+stickThreshold)*100/(100-stickThreshold);
  }/*
  Serial.print(" motor power = ");
  Serial.print(motorPower);
  Serial.print(" left motor: ");
  Serial.print(leftActivation*motorPower/100);
  Serial.print(" right motor: ");
  Serial.println(rightActivation*motorPower/100);*/
  radioControlActivation();
}

void loop() {/*
  walkForward();
  delay(5000);
  stopCar();
  delay(1000);
  */
  radioControl();
}


