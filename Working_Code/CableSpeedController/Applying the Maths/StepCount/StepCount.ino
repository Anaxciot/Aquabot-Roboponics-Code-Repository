//button pins
int buttonRight = A2;
int buttonLeft = A3;
int buttonUp = A1;
int buttonDown = A0;

//motor driver pins

//left is motor 4
int ENA_left = 11;
int DIR_left = 12;
int PUL_left = 13;

//right is motor 1
int ENA_right = 7;
int DIR_right = 6;
int PUL_right = 5;

//up is motor 2
int ENA_up = 10;
int DIR_up = 9;
int PUL_up = 8;

//down is motor 3
int ENA_down = 2;
int DIR_down = 3;
int PUL_down = 4;

//Global Variables
int MC = 32;        //Microstep setting
int dia = 2;        //Gear diameter in inches
int base = 500;     //Base workspace dimension in inches
int height = 500;   //Height workspace dimension in inches

double x = 0;             //Current x-position
double y = 0;             //Current y-position
double stepCountHor = 0;  //Horizontal step counter
double stepCountVer = 0;  //Vertical step counter

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //declaring pins for buttons as inputs. Input_pullup makes it read high when not connected to ground(when button is pressed)
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);

  //declaring all pins for motors as outputs. On Arduinos, digitial pins default to input
  pinMode(PUL_left, OUTPUT);
  pinMode(PUL_right, OUTPUT);
  pinMode(PUL_up, OUTPUT);
  pinMode(PUL_down, OUTPUT);

  pinMode(ENA_left, OUTPUT);
  pinMode(ENA_right, OUTPUT);
  pinMode(ENA_up, OUTPUT);
  pinMode(ENA_down, OUTPUT);

  pinMode(DIR_left, OUTPUT);
  pinMode(DIR_right, OUTPUT);
  pinMode(DIR_up, OUTPUT);
  pinMode(DIR_down, OUTPUT);

  //enabling all motors. LOW is enabled. HIGH is disabled
  digitalWrite(ENA_left,LOW);
  digitalWrite(ENA_right,LOW);
  digitalWrite(ENA_up,LOW);
  digitalWrite(ENA_down,LOW);

/*
  //setting motor direction
  digitalWrite(DIR_left,HIGH);
  digitalWrite(DIR_right,HIGH);
  digitalWrite(DIR_up,HIGH);
  digitalWrite(DIR_down,HIGH);
*/


  Serial.println("Setup Complete");
  Serial.println("Beginning loop, press a button to move that motor");
}

void loop() {
  /*
   * Current thinking of logic:
   * Starting at a specified position (x,y), if a button (up, down, left, or right), the cart will move in that direction.
   * The speed it moves will be based off of the speed of the main set of cables (the left and right for horizontal and the up and down for vertical)
   * A step of the main pairs will occur one per time interval (whatever delay between pulse seems to give smoothest operation)
   * Since the number of steps the secondary pair will take will be less than 1, a value is needed to count the partial steps, once this value reached above 1, a step will be taken and the value will decrease by one
   * 
   * Needs:
   * speed variable
   * position variables
   * secondary cable partial step count
   * 
   * Pseudo Code:
   * if (left button pressed) {
   *   moveCartLeft()
   * }
   * 
   * void moveCartLeft() {
   *   setDirleft(HIGH);setDirright(LOW); //set the direction of the primary motors to whatever would move the cart the correct direction
   *   stepMotor(left);stepMotor(right); //step the primary cables once
   *   nbrOfSteps = calcSecondaryStep(x,y,dir); //calculate the cooresponding nbr of steps the secondary need to take, the direction the cart is moving with impact if the number is negative of positive
   *   stepCountVer = stepCountVer + nbrOfSteps; //add the nbr of steps to the step count, this should be a global variable
   *   
   *   //stepCountVer will be positive if needs to move slightly out (loosen) and negative if needs to move slightly in (tighened)
   *   
   *   if (stepCountVer > 1) {
   *     setDir(DIR_up, HIGH);setDir(DIR_down,HIGH); //set direction that would tighten the cables
   *     stepMotor(up);stepMotor(down); //step both motors
   *     stepCountVer = stepCountVer - 1;
   *   }
   *   
   *   if (stepCountVer < -1) {
   *     setDir(DIR_up, LOW);setDir(DIR_down,LOW); 
   *     stepMotor(up);stepMotor(down);
   *     stepCountHor = stepCountHor + 1;
   *   }
   * }
   * 
   *void moveCartRight() {
   *   setDirleft(LOW);setDirright(HIGH); 
   *   stepMotor(left);stepMotor(right);
   *   nbrOfSteps = calcSecondaryStep(x,y,dir);
   *   stepCountVer = stepCountVer + nbrOfSteps;
   *   
   *   if (stepCountVer > 1) {
   *     setDir(DIR_up, HIGH);setDir(DIR_down,HIGH);
   *     stepMotor(up);stepMotor(down);
   *     stepCountVer = stepCountVer - 1;
   *   }
   *   
   *   if (stepCountHor < -1) {
   *     setDir(DIR_up, LOW);setDir(DIR_down,LOW);
   *     stepMotor(up);stepMotor(down);
   *     stepCountVer = stepCountVer + 1;
   *   }
   * }
   * 
   * void setDir(int motor, bool dir) {
   *    digitalWrite(motor, dir);
   * }
   * 
   */
  
  if (digitalRead(buttonLeft) == LOW) {
    Serial.println("left button pressed");
       
    digitalWrite(PUL_left,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_left,HIGH);
    delayMicroseconds(500);
  }

  if (digitalRead(buttonRight) == LOW) {
    Serial.println("right button pressed");
    
    digitalWrite(PUL_right,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_right,HIGH);
    delayMicroseconds(500);
  }

  if (digitalRead(buttonUp) == LOW) {
    Serial.println("up button pressed");

    digitalWrite(PUL_up,LOW);
    delayMicroseconds(500);
    digitalWrite(PUL_up,HIGH);
    delayMicroseconds(500);
  }

  if (digitalRead(buttonDown) == LOW) {
    Serial.println("down button pressed");

    stepMotor(PUL_down,500);
  }
}

//I think since the motion is based on step count, the delayTime will stay constant, so this argument could be removed
void stepMotor(int PUL, int delayTime){
  digitalWrite(PUL,LOW);
  delayMicroseconds(delayTime);
  digitalWrite(PUL,HIGH);
  delayMicroseconds(delayTime);
}

//delta returns the change of length in the top cable over the change in length of the horizontal cable
double delta(double x, double y) {
  double dx = (dia * PI) / (200 * mc); double dy = dx;
  double ACD = sqrt(x ^ 2 + (H - y) ^ 2) + sqrt((B - x) ^ 2 + (H - y) ^ 2) - sqrt((x + dx) ^ 2 + (H - y - dy) ^ 2) - sqrt((B - x - dx) ^ 2 + (H - y - dy) ^ 2);
  double BCD = sqrt(x ^ 2 + y ^ 2) + sqrt(x ^ 2 + (H - y) ^ 2) - sqrt((x + dx) ^ 2 + (y + dy) ^ 2) - sqrt((x + dx) ^ 2 + (H - y - dy) ^ 2);
  double del = ACD / BCD;
  return del;
}

//Calculates the number of secondary steps that need to be taken.
//Dir: 1 is up, 2 is down, 3 is left, 4 is right
double calcSecondarySteps(double x, double y, int dir) {
  switch (dir) {
    case 1:
      return 1/delta(x, y);
      break;
    case 2:
      return 1/delta(x, y);
      break;
    case 3:
      return delta(x, y);
      break;
    case 4:
      return delta(x, y);
      break;
  }
}
