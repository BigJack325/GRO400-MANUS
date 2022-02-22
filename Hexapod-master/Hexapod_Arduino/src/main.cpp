/* 
 * GRO 400 - Agile Conception in Robotics
 * Auteurs: MANUS
 * date: 25 janvier 2022
*/

/*------------------------------ LIBRAIRIES ---------------------------------*/
#include <LibS3GRO.h>
#include <ArduinoJson.h>

/*------------------------------ CONSTANTS ---------------------------------*/
using namespace std;

#define BAUD                           115200    // Seriral transmission Frequence 
#define UPDATE_PERIOD                  100       // Update period (ms) to send general state

#define MANUEL                         1         // Used to set robot in manuel mode
#define AUTOMATIC                      2         // Used to set robot in automatic mode

#define initial_angle_A                90         //Angle of servo A in initialized state
#define initial_angle_B                90         //Angle of servo B in initialized state
#define initial_angle_C                90         //Angle of servo C in initialized state

#define angle_forward                  10         // Angle of servo A used in forward movements
#define angle_backward                 10         // Angle of servo A used in backward movements
#define angle_rotation                 10         // Angle of servo A used in spinning movements

#define forward                        1
#define backward                       2
#define spin                           3

#define arena_sizex                    200                //Width of the arena (cm)
#define arena_sizey                    200                //length of the arena (cm)
#define initial_position_x             arena_sizex/2      //Initial position of the robot on the x axis (cm)
#define initial_position_y             arena_sizey/2      //Initial position of the robot on the y-axis (cm)

//--- CASE NUMBERS ----
#define INITIALIZATION                 1         // Initial case of the robot when turned on
#define WAIT                           2         // Case when robot is waiting for a command
#define MOVE_FORWARD                   3         // Case for robot to move forward
#define MOVE_BACKWARD                  4         // Case for robot to move backward
#define MOVE_LEFT                      5         // Case for robot to move sideways left
#define MOVE_RIGHT                     6         // Case for robot to move sideways right
#define TURN_LEFT                      7         // Case for robot to spin on itself left
#define TURN_RIGHT                     8         // Case for robot to spin on itself right
#define PICKUP                       9         // Case for robot to pick up object
#define DROP                           10        // Case for robot to drop object
#define SHUTDOWN                       20        // Case for robot to start shutdown sequence

//--- SERVOMOTOR NUMBERS ----       Possibly useless may need to delete later
#define A1                             1         
#define B1                             2       
#define C1                             3         
#define A2                             4         
#define B2                             5        
#define C2                             6         
#define A3                             7        
#define B3                             8         
#define C3                             9     
#define A4                             10     
#define B4                             11       
#define C4                             12
#define A5                             13      
#define B5                             14       
#define C5                             15        
#define A6                             16         
#define B6                             17      
#define C6                             18
#define D1                             19  

//----------------------------- PINS USED -----------------------------------

                          //Must Change pin numbers
#define A1_Pin                          22  //Digital Port for Servo A1
#define B1_Pin                          22  //Digital Port for Servo B1
#define C1_Pin                          22  //Digital Port for Servo C1

#define A2_Pin                          22  //Digital Port for Servo A2
#define B2_Pin                          22  //Digital Port for Servo B2
#define C2_Pin                          22  //Digital Port for Servo C2

#define A3_Pin                          22  //Digital Port for Servo A3
#define B3_Pin                          22  //Digital Port for Servo B3
#define C3_Pin                          22  //Digital Port for Servo C3

#define A4_Pin                          22  //Digital Port for Servo A4
#define B4_Pin                          22  //Digital Port for Servo B4
#define C4_Pin                          22  //Digital Port for Servo C4

#define A5_Pin                          22  //Digital Port for Servo A5
#define B5_Pin                          22  //Digital Port for Servo B5
#define C5_Pin                          22  //Digital Port for Servo C5

#define A6_Pin                          22  //Digital Port for Servo A6
#define B6_Pin                          22  //Digital Port for Servo B6
#define C6_Pin                          22  //Digital Port for Servo C6

#define D1_Pin                          22  //Digital Port for Servo D1

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
int command =                          INITIALIZATION;    //Set first command as initialisation

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

/*------------------------- Prototypes de fonctions -------------------------*/
void timerCallback();
void startPulse();
void endPulse();
void sendMsg(); 
void readMsg();
void serialEvent();
void digitalWrite(uint8_t pin, uint8_t val);


// Caller les fonctions ICI 
//ex:
// double Calculangle();
// float reduce_angle();
// double vitesse();

float futureposition();                                          //Calculate robot coordinates if movement is made
bool isinarena();                                                //Checks if robot will be in arena if movement is made
void initialize(); 
void forwardleftleg(int leg_identification,int angle_A);         //Move a single left side leg forward
void forwardrightleg(int leg_identification,int angle_A);        //Move a single right side leg forward
void backwardleftleg(int leg_identification,int angle_A);        //Move a single left side leg backward
void backwardrightleg(int leg_identification,int angle_A);       //Move a single right side leg backward
void sidewaysreach(int leg_identification);                      //Move a single leg out for sidways motion
void sidewaysretract(int leg_identification);                    //Move a single leg in for sidways motion



/*---------------------------- MAIN -----------------------------*/

void setup() {
  Serial.begin(BAUD);                                            // Initialisation of serial communication
  Serial.flush();

  // Send message timer
  timerSendMsg_.setDelay(UPDATE_PERIOD);
  timerSendMsg_.setCallback(timerCallback);
  timerSendMsg_.enable();

  // Pulse duration timer
  timerPulse_.setCallback(endPulse);

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
}


// Main loop (infinite) 
void loop() {

  if(shouldRead_){
    readMsg();
  }
  if(shouldSend_){
    sendMsg();
  }
  if(shouldPulse_){
    startPulse();
  }

//---------------------- SWITCH CASE -------------------------------
 switch(command)
    {
        case INITIALIZATION :      // Sets all servomotors to initial angles
          initialize();
        break;

        case WAIT :                // Waiting for a command
        break;

        case MOVE_FORWARD :                // Move one step forward sequence
        break;

        case MOVE_BACKWARD :                // Move one step backward sequence
        break;

        case MOVE_LEFT :                // Lateral step left sequence
        break;

        case MOVE_RIGHT :                // Lateral step right sequence
        break;

        case TURN_LEFT :                // Pivot counter-clockwise sequence
        break;

        case TURN_RIGHT :                // Pivot clockwise sequence
        break;

        case PICKUP :                // Pickup object sequence
        break;

        case DROP :                // Drop object sequence
        break;

        case SHUTDOWN :                // Gently lands body on the ground and sets legs into transport position
        break;



    }

  // Mise à jour des chronometres
  timerSendMsg_.update();
  timerPulse_.update();


  
}

/*---------------------------Definition de fonctions ------------------------*/

void serialEvent(){shouldRead_ = true;}

void timerCallback(){shouldSend_ = true;}

void startPulse(){
  /* Demarrage d'un pulse */
  timerPulse_.setDelay(pulseTime_);
  timerPulse_.enable();
  timerPulse_.setRepetition(1);
  shouldPulse_ = false;
  isInPulse_ = true;
}

void endPulse(){
  /* Rappel du chronometre */
  timerPulse_.disable();
  isInPulse_ = false;
}

void sendMsg(){
  /* Envoit du message Json sur le port seriel */
  StaticJsonDocument<500> doc;
  // Elements du message

  doc["time"]      = (millis()/1000.0);
  doc["cur_pos_x"]  = current_position_x; 
  doc["cur_pos_y"]  = current_position_y;
  doc["cur_orientation"]  = current_orientation;
  /*
  doc["cur_angle_A1"]  = A1_.Read();
  doc["cur_angle_B1"]  = B1_.Read();
  doc["cur_angle_C1"]  = C1_.Read();
  doc["cur_angle_A2"]  = A2_.Read();
  doc["cur_angle_B2"]  = B2_.Read();
  doc["cur_angle_C2"]  = C2_.Read();
  doc["cur_angle_A3"]  = A3_.Read();
  doc["cur_angle_B3"]  = B3_.Read();
  doc["cur_angle_C3"]  = C3_.Read();
  doc["cur_angle_A4"]  = A4_.Read();
  doc["cur_angle_B4"]  = B4_.Read();
  doc["cur_angle_C4"]  = C4_.Read();
  doc["cur_angle_A5"]  = A5_.Read();
  doc["cur_angle_B5"]  = B5_.Read();
  doc["cur_angle_C5"]  = C5_.Read();
  doc["cur_angle_A6"]  = A6_.Read();
  doc["cur_angle_B6"]  = B6_.Read();
  doc["cur_angle_C6"]  = C6_.Read();
  doc["cur_angle_D1"]  = D1_.Read();
  */

  //doc["pulsePWM"]  = pulsePWM_;                          //add variables
    

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
    //Serial.print("deserialize() failed: ");
    //Serial.println(error.c_str());
    return;
  }
  
  //TO BE LET IN FIRST QT WINDOW------------------------------------------------------------TO DO-------------------------------------

  // Analyse des éléments du message message
  /*
  parse_msg = doc["pulsePWM"];
  if(!parse_msg.isNull()){
     pulsePWM_ = doc["pulsePWM"].as<float>();
  }

  parse_msg = doc["pulseTime"];
  if(!parse_msg.isNull()){
     pulseTime_ = doc["pulseTime"].as<float>();
  }

  parse_msg = doc["pulse"];
  if(!parse_msg.isNull()){
     shouldPulse_ = doc["pulse"];
  }
*/
  //-----------------------AJOUTER FONCTIONS DE MOUVEMENT ICI-----------------------------------------------------
}

void initialize(){  //TO DO




  return;
}

//Calculate the coordinate and orientation of the robot if the commanded action is taken
void futureposition(int movement)   //    TO DO
{

if(movement == forward)
 {
  future_position_x = current_position_x;
  future_position_y = current_position_y;
  future_orientation = current_orientation;
 } 

if(movement == backward)
 {
  current_position_x = current_position_x;
  current_position_y = current_position_y;
  current_orientation = current_orientation;
 } 

if(movement == spin)
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