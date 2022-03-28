/* 
 * GRO 400 - Conception d'un robot articulé
 * Auteurs: MANUS
 * date: 25 janvier 2022
*/

/*------------------------------ Librairies ---------------------------------*/
#include <LibS3GRO.h>
#include <ArduinoJson.h>

/*----------------------------- CLASS ----------------------------*/
#define left                           1         // Variable to specify which side the leg is on for inverse cinematic calculation
#define right                          2         // Variable to specify which side the leg is on for inverse cinematic calculation


class SynchServo
{
  public:
     //constructor
    SynchServo();
    SynchServo(MegaServo* servo_1, MegaServo* servo_2,  MegaServo* servo_3, int A_B_or_C, int either_145_or_236);

    int cinematic_inverse(int left_or_right, float X, float Y, float Z);
    void move_angle(int angle_selon__patte_gauche);
    void move_position(float X, float Y, float Z);            // X Y and Z values must be according to left leg
    bool readall(int angle_check);

    MegaServo* servo1;
    MegaServo* servo2;
    MegaServo* servo3;

    // MegaServo* servo1 = & servo_1;
    // MegaServo* servo2 = & servo_2;
    // MegaServo* servo3 = & servo_3;

    int letter;
    int group;

  private:

    float L1x;
    float L1y;
    float L1z;
    float L2x;
    float L2y;
    float L2z;
    float L3x;
    float L3y;
    float L3z;
};

//constructor
SynchServo::SynchServo()
{
  letter = 0;
  group  = 0;

  //dimensions of the 3 pieces of the leg for inverse kinematics
   L1x = 0;
   L1y = 1;
   L1z = 0;
   L2x = 0;
   L2y = 1;
   L2z = 0;
   L3x = 0;
   L3y = 1;
   L3z = 0;
}

SynchServo::SynchServo(MegaServo *servo_1, MegaServo *servo_2, MegaServo *servo_3, int  A_B_or_C, int either_145_or_236)
{
  servo1 = servo_1;
  servo2 = servo_2;
  servo3 = servo_3;
  
  letter = A_B_or_C;
  group  = either_145_or_236;

  //dimensions of the 3 pieces of the leg for inverse kinematics
   L1x = 0;
   L1y = 1;
   L1z = 0;
   L2x = 0;
   L2y = 1;
   L2z = 0;
   L3x = 0;
   L3y = 1;
   L3z = 0;
}

int SynchServo::cinematic_inverse(int left_or_right, float X, float Y, float Z)
{  
  float theta1;
  float theta2;
  float theta3;
  
  // Inverse kinematics relationships
  float r1 = sqrt((X*X) + (Y*Y)) - L1y;
  float r2 = Z;
  float phi2 = atan(r2/r1);
  float r3 = sqrt((r1*r1) + (r2*r2));
  float phi1 = acos( (-(L3y*L3y) + (L2y*L2y) + (r3*r3)) / (2*L2y*r3));
  float phi3 = acos( ((L3y*L3y) + (L2y*L2y) - (r3*r3)) / (2*L2y*L3y));

  
  // Required angles left side
  if(left_or_right == 1)  //if left
  {
    theta1 = atan(Y/-X)     *360/(2*PI);
    if (X == 0) 
    {
      theta1 = 90;
    }
      
    if (X > 0)
    {
      theta1 =  180 - (-theta1);
    }
      
    theta2 =  90 - ((phi2 - phi1)   *360/(2*PI));
    theta3 =  (180 - phi3     *360/(2*PI));
  }

  // Required angles right side
  if(left_or_right == 2)  //if right
  {
    theta1 = atan(Y/-X)     *360/(2*PI);
    if (X == 0) 
    {
      theta1 = 90;
    }
      
    if (X > 0)
    {
      theta1 =  180 - (-theta1);
    }
      
    theta2 =  90 + ((phi2 - phi1)   *360/(2*PI));
    theta3 =  (phi3     *360/(2*PI));
  }

  //return correct angle
  if(letter == 1) //if A
  {
    return theta1;
  }

  if(letter == 2) // if B
  {
    return theta2;
  }

  if(letter == 3) // if C
  {
    return theta3;
  }
  return 0;
}


void SynchServo::move_angle(int angle_selon_patte_gauche)
{
  //int micro_selon_patte_gauche = angle_selon_patte_gauche * 2400 / 544;
  //float old_percent = .99;
  //float new_percent = 1- old_percent;

  if (group == 1)
  {
    // float angle_servo1 = (micro_selon_patte_gauche * new_percent) + (servo1->readMicroseconds() *old_percent);
    // float angle_servo2 = (micro_selon_patte_gauche * new_percent) + ((2400 - servo2->readMicroseconds()) *old_percent);
    // float angle_servo3 = (micro_selon_patte_gauche * new_percent) + (servo3->readMicroseconds() *old_percent);

    servo1->write(angle_selon_patte_gauche);
    servo2->write(180 - angle_selon_patte_gauche); //2400
    servo3->write(angle_selon_patte_gauche);
  }
  
  if (group == 2)
  {
    // float angle_servo1 = (micro_selon_patte_gauche* new_percent) + ((2400 - servo1->readMicroseconds()) *old_percent);
    // float angle_servo2 = (micro_selon_patte_gauche* new_percent) + ( servo2->readMicroseconds() *old_percent);
    // float angle_servo3 = (micro_selon_patte_gauche* new_percent) + ((2400 - servo3->readMicroseconds()) *old_percent);

    servo1->write(180 - angle_selon_patte_gauche);
    servo2->write(angle_selon_patte_gauche);
    servo3->write(180 - angle_selon_patte_gauche);
  }
}

void SynchServo::move_position(float X, float Y, float Z)
{
  if(group == 1) //group145
  {
    servo1->write(        cinematic_inverse(left,   X, Y, Z)       );
    servo2->write(        cinematic_inverse(right, -X, Y, Z)       );
    servo3->write(        cinematic_inverse(left,   X, Y, Z)       );

  }

  if(group == 2) //group236
  {
    servo1->write(        cinematic_inverse( right, -X, Y, Z)       );
    servo2->write(        cinematic_inverse( left ,  X, Y, Z)       );
    servo3->write(        cinematic_inverse( right, -X, Y, Z)       );
  }
}

bool SynchServo::readall(int angle_check)
{
  if(group == 1) //group145
  {
    if (servo1->read() == angle_check && servo2->read()== 180 - angle_check && servo3->read() == angle_check)
    {
      return true;
    }
  }

  if(group == 2) //group236
  {
    if (servo1->read() == 180 - angle_check && servo2->read()== angle_check && servo3->read() == 180 - angle_check)
    {
      return true;
    }
  }
  return false;
}


/*------------------------------ Constantes ---------------------------------*/
using namespace std;

#define BAUD                            115200    // Frequence de transmission serielle
#define UPDATE_PERIODE                  100     // Periode (ms) d'envoie d'etat general

#define MANUEL                         1         // Used to set robot in manuel mode
#define AUTOMATIC                      2         // Used to set robot in automatic mode

#define initial_angle_A                90         //Angle of servo A in initialized state
#define initial_angle_B                180         //Angle of servo B in initialized state
#define initial_angle_C                145       //Angle of servo C in initialized state

#define standing_angle_B               140        // Angle for B servos in standing position
#define standing_angle_C               140        // Angle for C servos in standing position

#define walking_angle_B                160        // Angle to lift up B when walking
#define Walking_angle_A_increase       10         // Angle of servo A used in forward movements

#define sidestep_angle                 10         // Angle used in sidestepping
#define turn_angle                     10         // angle used in turning

#define angle_backward                 10         // Angle of servo A used in backward movements
#define angle_rotation                 10         // Angle of servo A used in spinning movements

#define arena_sizex                    200                //Width of the arena (cm)
#define arena_sizey                    200                //length of the arena (cm)
#define initial_position_x             arena_sizex/2      //Initial position of the robot on the x axis (cm)
#define initial_position_y             arena_sizey/2      //Initial position of the robot on the y-axis (cm)

#define step_delay                     500               // delay between each steps

//--- CASE NUMBERS ----
#define INITIALIZATION                 1         // Initial case of the robot when turned on
#define WAIT                           2         // Case when robot is waiting for a command
#define MOVE_FORWARD                   3         // Case for robot to move forward
#define MOVE_BACKWARD                  4         // Case for robot to move backward
#define SIDESTEP_LEFT                  5         // Case for robot to move sideways left
#define SIDESTEP_RIGHT                 6         // Case for robot to move sideways right
#define TURN_LEFT                      7         // Case for robot to spin on itself left
#define TURN_RIGHT                     8         // Case for robot to spin on itself right
#define PICKUP                         9         // Case for robot to pick up object
#define DROP                           10        // Case for robot to drop object
#define STAND                          20        // Case for robot to get up off the ground
#define LAYDOWN                        21        // Case for robot to gently drop to ground

// --- Class constants
#define A                              1         // Variable to identify which motors is associated with object synchservo
#define B                              2         // Variable to identify which motors is associated with object synchservo
#define C                              3         // Variable to identify which motors is associated with object synchservo
#define group145                       1         // Variable to identify which set of motors is associated with object synchservo
#define group236                       2         // Variable to identify which set of motors is associated with object synchservo
#define left                           1         // Variable to specify which side the leg is on for inverse cinematic calculation
#define right                          2         // Variable to specify which side the leg is on for inverse cinematic calculation

//----------------------------- PINS USED -----------------------------------

                          //Must Change pin numbers
#define A1_Pin                          42  //Digital Port for Servo A1
#define B1_Pin                          41  //Digital Port for Servo B1
#define C1_Pin                          40  //Digital Port for Servo C1

#define A2_Pin                          25  //Digital Port for Servo A2
#define B2_Pin                          26  //Digital Port for Servo B2
#define C2_Pin                          27  //Digital Port for Servo C2

#define A3_Pin                          39  //Digital Port for Servo A3
#define B3_Pin                          38  //Digital Port for Servo B3
#define C3_Pin                          37  //Digital Port for Servo C3

#define A4_Pin                          28  //Digital Port for Servo A4
#define B4_Pin                          29  //Digital Port for Servo B4
#define C4_Pin                          30  //Digital Port for Servo C4

#define A5_Pin                          36  //Digital Port for Servo A5
#define B5_Pin                          35  //Digital Port for Servo B5
#define C5_Pin                          34  //Digital Port for Servo C5

#define A6_Pin                          31  //Digital Port for Servo A6
#define B6_Pin                          32  //Digital Port for Servo B6
#define C6_Pin                          33  //Digital Port for Servo C6

#define D1_Pin                          23  //Digital Port for Servo D1 (head)
#define D2_Pin                          44  //Digital Port for Servo D2 (mandibules)

/*---------------------------- GLOBAL VARIABLES ---------------------------*/

volatile bool shouldSend_ =             false;    // Ready to send message flag
volatile bool shouldRead_ =             false;    // Ready to read message flag
volatile bool shouldPulse_ =            false;    // Generate a pulse flag
volatile bool isInPulse_ =              false;    // Currently in a pulse flag

SoftTimer timerSendMsg_;                          // Send message timer
SoftTimer timerPulse_;                            // Duration of a pulse timer

uint16_t pulseTime_ =                   0;        // Pulse time in ms

int time =                              0;        // Loop timer


int operation_mode =                   MANUEL;            //Determines whether robot is in automatic or manuel mode
int command =                          0;                 //Variable to give command (inicate which case to do)

int current_position_x =               0;                 //Current position of the robot on the x axis (cm)
int current_position_y =               0;                 //Current position of the robot on the y-axis (cm)
int current_orientation =              90;                //Current angle of orientation of robot (deg) in a counterclockwise rotation of x-axis

int future_position_x =                0;                 //Calculated future position of the robot on the x axis (cm)
int future_position_y =                0;                 //Calculated future of the robot on the y-axis (cm)
int future_orientation =               0;                //Calculated future angle of orientation of robot (deg) in a counterclockwise rotation of x-axis

float distance_from_target =           300;               //Distance robot must cover to reach target object (cm)
float drop_offx =                      0;                 //Coordinate on x-axis of object-drop location
float drop_offy =                      0;                 //Coordinate on y-axis of object-drop location

int leg_identification =               0;                 //Leg number to be used in movement

double time1 =                         0.0;               //Variable used if a dt needs to be calculated
double time2 =                         0.0;               //Variable used if a dt needs to be calculated

int step =                             1;                 //To organize motion in each case
float t;                                                  //Variable to use as timer for steps

int input_command =                    WAIT;       // command recu du HMI

/*---------------------------- Objects ---------------------------*/

//Create an object for each servo 
MegaServo A1_;       
MegaServo B1_;       
MegaServo C1_;         
MegaServo A2_;            
MegaServo B2_;          
MegaServo C2_;            
MegaServo A3_;            
MegaServo B3_;             
MegaServo C3_;        
MegaServo A4_;         
MegaServo B4_;              
MegaServo C4_;      
MegaServo A5_;      
MegaServo B5_;             
MegaServo C5_;                
MegaServo A6_;           
MegaServo B6_;         
MegaServo C6_;
MegaServo D1_; 
MegaServo D2_;

//-----Create pointers for each servo

MegaServo* A1_p =  &A1_; 
MegaServo* A2_p =  &A2_; 
MegaServo* A3_p =  &A3_; 
MegaServo* A4_p =  &A4_; 
MegaServo* A5_p =  &A5_; 
MegaServo* A6_p =  &A6_; 
MegaServo* B1_p =  &B1_; 
MegaServo* B2_p =  &B2_; 
MegaServo* B3_p =  &B3_; 
MegaServo* B4_p =  &B4_; 
MegaServo* B5_p =  &B5_; 
MegaServo* B6_p =  &B6_;
MegaServo* C1_p =  &C1_; 
MegaServo* C2_p =  &C2_; 
MegaServo* C3_p =  &C3_; 
MegaServo* C4_p =  &C4_; 
MegaServo* C5_p =  &C5_; 
MegaServo* C6_p =  &C6_; 



SynchServo A145_(A1_p, A4_p, A5_p, A, group145);
SynchServo B145_(B1_p, B4_p, B5_p, B, group145);
SynchServo C145_(C1_p, C4_p, C5_p, C, group145);

SynchServo A236_(A2_p, A3_p, A6_p, A, group236);
SynchServo B236_(B2_p, B3_p, B6_p, B, group236);
SynchServo C236_(C2_p, C3_p, C6_p, C, group236);



/*------------------------- Prototypes de fonctions -------------------------*/
void timerCallback();
//void startPulse();
//void endPulse();
void sendMsg(); 
void readMsg();
void serialEvent();
void digitalWrite(uint8_t pin, uint8_t val);

float futureposition();                                          //Calculate robot coordinates if movement is made
bool isinarena();                                                //Checks if robot will be in arena if movement is made
void stepsequence(int step_number, int delay_microseconds, SynchServo* servos , int angle);       //Move a synchservo object with timer
void sidestepsequence(int step_number, int delay_microseconds, SynchServo* servos , int angle);   //Move synchservo for sidestepping with timer
void turnstepsequence(int step_number, int delay_microseconds, SynchServo* servos , int angle);    //Move synchservo for turning with timer


/*---------------------------- fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // Initialisation of serial communication
  //Serial.flush();

  // Send message timer
  timerSendMsg_.setDelay(UPDATE_PERIODE);
  timerSendMsg_.setCallback(timerCallback);
  timerSendMsg_.enable();

  // Pulse duration timer
  //timerPulse_.setCallback(endPulse);

  //Assign each servo to their object 
  A1_.attach(A1_Pin);         
  B1_.attach(B1_Pin);       
  C1_.attach(C1_Pin);         
  A2_.attach(A2_Pin);            
  B2_.attach(B2_Pin);          
  C2_.attach(C2_Pin);            
  A3_.attach(A3_Pin);            
  B3_.attach(B3_Pin);             
  C3_.attach(C3_Pin);        
  A4_.attach(A4_Pin);         
  B4_.attach(B4_Pin);              
  C4_.attach(C4_Pin);      
  A5_.attach(A5_Pin);      
  B5_.attach(B5_Pin);             
  C5_.attach(C5_Pin);                
  A6_.attach(A6_Pin);           
  B6_.attach(B6_Pin);         
  C6_.attach(C6_Pin);
  D1_.attach(D1_Pin); 

  command = INITIALIZATION;
  input_command = WAIT;
  t = millis();
}


// Main loop (infinite) 
void loop() {
  
  if(shouldRead_){
    readMsg();
  }
  if(shouldSend_){
    sendMsg();
  }
  
//---------------------- SWITCH CASE -------------------------------
 switch(command)
    {
        case INITIALIZATION :      // Sets all servomotors to initial angles

        stepsequence(1, step_delay, &A145_, initial_angle_A);
        stepsequence(2, step_delay, &A236_, initial_angle_A);
        stepsequence(3, step_delay, &B145_, initial_angle_B);
        stepsequence(4, step_delay, &B236_, initial_angle_B);
        stepsequence(5, step_delay, &C145_, initial_angle_C);
        stepsequence(6, step_delay, &C236_, initial_angle_C);

          if (step == 7)
          {
            step = 1;
            command = WAIT;
          }
        break;

        
        case WAIT :                // Waiting for a command
          //Do nothing
          if (input_command != WAIT)
          {
            t = millis();
            command = input_command;
          }
        break;

        case MOVE_FORWARD :                // Move one step forward sequence
          stepsequence(1, step_delay, &B145_, walking_angle_B);
          stepsequence(2, step_delay, &A145_, initial_angle_A + Walking_angle_A_increase);
          stepsequence(3, step_delay, &B145_, standing_angle_B);
          stepsequence(4, step_delay, &B236_, walking_angle_B);
          stepsequence(5, 0, &A145_, initial_angle_A);
          stepsequence(6, step_delay, &A236_, initial_angle_A + Walking_angle_A_increase);
          stepsequence(7, step_delay, &B236_, standing_angle_B);
          stepsequence(8, step_delay, &B145_, walking_angle_B);
          stepsequence(9, step_delay, &A236_, initial_angle_A);
          stepsequence(10, step_delay, &B145_, standing_angle_B);

          if (step == 11)
          {
            step = 1;
            command = WAIT;
          }

        break;

        case MOVE_BACKWARD :                // Move one step backward sequence
          stepsequence(1, step_delay, &B145_, walking_angle_B);
          stepsequence(2, step_delay, &A145_, initial_angle_A - Walking_angle_A_increase);
          stepsequence(3, step_delay, &B145_, standing_angle_B);
          stepsequence(4, step_delay, &B236_, walking_angle_B);
          stepsequence(5, 0, &A145_, initial_angle_A);
          stepsequence(6, step_delay, &A236_, initial_angle_A - Walking_angle_A_increase);
          stepsequence(7, step_delay, &B236_, standing_angle_B);
          stepsequence(8, step_delay, &B145_, walking_angle_B);
          stepsequence(9, step_delay, &A236_, initial_angle_A);
          stepsequence(10, step_delay, &B145_, standing_angle_B);

          if (step == 11)
          {
            step = 1;
            command = WAIT;
          }

        break;

        case SIDESTEP_LEFT :                // Lateral step left sequence
          stepsequence(1, step_delay, &B236_, walking_angle_B);
          sidestepsequence(2, step_delay, &C145_, sidestep_angle);
          stepsequence(3, step_delay, &B236_, standing_angle_B);
          stepsequence(4, step_delay, &B145_, walking_angle_B);
          sidestepsequence(5, step_delay, &C145_, 0);
          stepsequence(6, step_delay, &B145_, standing_angle_B);

      
          if (step == 7)
          {
            step = 1;
            command = WAIT;
          }
        break;

        case SIDESTEP_RIGHT :                // Lateral step right sequence

          stepsequence(1, step_delay, &B145_, walking_angle_B);
          sidestepsequence(2, step_delay, &C236_, sidestep_angle);
          stepsequence(3, step_delay, &B145_, standing_angle_B);
          stepsequence(4, step_delay, &B236_, walking_angle_B);
          sidestepsequence(5, step_delay, &C236_, 0);
          stepsequence(6, step_delay, &B236_, standing_angle_B);

      
          if (step == 7)
          {
            step = 1;
            command = WAIT;
          }
        break;

        case TURN_LEFT :                // Pivot counter-clockwise sequence

          stepsequence(1, step_delay, &B236_, walking_angle_B);
          turnstepsequence(2, step_delay, &A236_, turn_angle);
          stepsequence(3, step_delay, &B236_, standing_angle_B);
          stepsequence(4, step_delay, &B145_, walking_angle_B);
          sidestepsequence(5, step_delay, &A236_, 0);
          stepsequence(6, step_delay, &B145_, standing_angle_B);

           if (step == 7)
          {
            step = 1;
            command = WAIT;;
          }

        break;

        case TURN_RIGHT :                // Pivot clockwise sequence
        break;

        case PICKUP :                // Pickup object sequence
          D2_.write(0); 
          command = WAIT;       
        break;

        case DROP :                // Drop object sequence
          D2_.write(100);  
          command = WAIT;
        break;

        case STAND :               // Get up off ground
          stepsequence(1, 0, &C145_, standing_angle_C );
          stepsequence(2, step_delay, &C236_, standing_angle_C );
          stepsequence(3, 0, &B145_, standing_angle_B );
          stepsequence(4, step_delay, &B236_, standing_angle_B );
          
            if (step == 5)
            {
              step = 1;
              command = WAIT;
            }
        break;

        case LAYDOWN :                // Gently lands body on the ground and sets legs into transport position
          stepsequence(1, 0, &B145_, initial_angle_B );
          stepsequence(2, step_delay, &B236_, initial_angle_B );
          stepsequence(3, 0, &C145_, standing_angle_C );
          stepsequence(4, step_delay, &C236_, standing_angle_C );
          
            if (step == 5)
            {
              step = 1;
              command = WAIT;
            }
        break;
    }

}

/*---------------------------Function definition communication ------------------------*/

void serialEvent(){shouldRead_ = true;}

void timerCallback(){shouldSend_ = true;}

void sendMsg(){

  /* Envoit du message Json sur le port seriel */
  StaticJsonDocument<500> doc;
  // Elements du message

  doc["time"]      = (millis()/1000.0);
  doc["cur_pos_x"]  = current_position_x; 
  doc["cur_pos_y"]  = current_position_y;
  doc["cur_orientation"]  = current_orientation;
  
  // doc["cur_angle_A1"]  = A1_.Read();
  // doc["cur_angle_B1"]  = B1_.Read();
  // doc["cur_angle_C1"]  = C1_.Read();
  // doc["cur_angle_A2"]  = A2_.Read();
  // doc["cur_angle_B2"]  = B2_.Read();
  // doc["cur_angle_C2"]  = C2_.Read();
  // doc["cur_angle_A3"]  = A3_.Read();
  // doc["cur_angle_B3"]  = B3_.Read();
  // doc["cur_angle_C3"]  = C3_.Read();
  // doc["cur_angle_A4"]  = A4_.Read();
  // doc["cur_angle_B4"]  = B4_.Read();
  // doc["cur_angle_C4"]  = C4_.Read();
  // doc["cur_angle_A5"]  = A5_.Read();
  // doc["cur_angle_B5"]  = B5_.Read();
  // doc["cur_angle_C5"]  = C5_.Read();
  // doc["cur_angle_A6"]  = A6_.Read();
  // doc["cur_angle_B6"]  = B6_.Read();
  // doc["cur_angle_C6"]  = C6_.Read();
  // doc["cur_angle_D1"]  = D1_.Read();
  
 
  // Serialisation
  serializeJson(doc, Serial);
  // Envoit
  Serial.println();
  shouldSend_ = false;
}

void readMsg(){

  // Lecture du message Json
  StaticJsonDocument<500> doc;
  JsonVariant parse_msg;

  // Lecture sur le port Seriel
  DeserializationError error = deserializeJson(doc, Serial);
  shouldRead_ = false;


  // Si erreur dans le message
  if (error) {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return;
  }
  // Analyse des éléments du message
  // parse_msg = doc["pulsePWM"];
  // if(!parse_msg.isNull()){
  //    pulsePWM_ = doc["pulsePWM"].as<float>();
  // }

}


/* ------------------Fonction Definition ----------------------------*/
//Calculate the coordinate and orientation of the robot if the commanded action is taken
void futureposition(int movement)   //    TO DO
{

if(movement == 1) //forward
 {
  future_position_x = current_position_x;
  future_position_y = current_position_y;
  future_orientation = current_orientation;
 } 

if(movement == 1) //backward
 {
  current_position_x = current_position_x;
  current_position_y = current_position_y;
  current_orientation = current_orientation;
 } 

if(movement == 1) //turn left
 {
  current_position_x = current_position_x;
  current_position_y = current_position_y;
  current_orientation = current_orientation;
 } 

 if(movement == 1) //turn right
 {
  current_position_x = current_position_x;
  current_position_y = current_position_y;
  current_orientation = current_orientation;
 } 
  
return;
}

// If robot is still within arena limits return true else return false
bool isinarena(){
 if((-arena_sizex < current_position_x) && (current_position_x < arena_sizex) && (-arena_sizey < current_position_y) && (current_position_y < arena_sizey))
  {
    return true;
  }

 return false;
}

void stepsequence(int step_number, int delay_microseconds, SynchServo* servos , int angle)
{
  if(step == step_number)
        {
          servos->move_angle(angle);
          if(millis() > (t + delay_microseconds))
            {
              t = millis();
              step ++;
            }
        }
  return;
}

void sidestepsequence(int step_number, int delay_microseconds, SynchServo* servos , int angle)
{
  if(step == step_number)
        {
          if(servos->group == 1)
          {
            servos->servo1->write(standing_angle_C + angle);
            servos->servo2->write(180 - standing_angle_C + angle);
            servos->servo3->write(standing_angle_C + angle);
          }

          if(servos->group == 2)
          {
            servos->servo1->write(180 - standing_angle_C - angle);
            servos->servo2->write(standing_angle_C - angle);
            servos->servo3->write(180 - standing_angle_C - angle);
          }
          
          if(millis() > (t + delay_microseconds))
            {
              t = millis();
              step ++;
            }
        }
  return;
}

void turnstepsequence(int step_number, int delay_microseconds, SynchServo* servos , int angle)
{
  if(step == step_number)
        {
          if(servos->group == 1)
          {
            servos->servo1->write(initial_angle_A + angle);
            servos->servo2->write(180 - initial_angle_A - angle);
            servos->servo3->write(initial_angle_A + angle);
          }

          if(servos->group == 2)
          {
            servos->servo1->write(180 - initial_angle_A + angle);
            servos->servo2->write(initial_angle_A - angle);
            servos->servo3->write(180 - initial_angle_A + angle);
          }
          
          if(millis() > (t + delay_microseconds))
            {
              t = millis();
              step ++;
            }
        }
  return;
}