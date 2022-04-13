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
    SynchServo(MegaServo* servo_1, MegaServo* servo_2,  MegaServo* servo_3, int A_B_or_C, int either_145_or_236, int offset_1, int offset_2, int offset_3);

    int direct_kinematics(int output_dimension, int angleA, int angleB, int angleC);
    int inverse_kinematics(int left_or_right, float X, float Y, float Z);
    void move_angle(int angle_selon__patte_gauche);
    void move_position(float X, float Y, float Z);            // X Y and Z values must be according to left leg
    bool readall(int angle_check);

    MegaServo* servo1;
    MegaServo* servo2;
    MegaServo* servo3;

    int letter;
    int group;

    int offset1;
    int offset2;
    int offset3;

    float L1x;
    float L1y;
    float L1z;
    float L2x;
    float L2y;
    float L2z;
    float L3x;
    float L3y;
    float L3z;

  private:

 
};

//constructor
SynchServo::SynchServo()
{
  letter = 0;
  group  = 0;

  L1x = 1.6;
  L1y = 5.55;
  L1z = 4.72;
  L2x = 0;
  L2y = 7.2;
  L2z = 0;
  L3x = 0;
  L3y = 11.47;
  L3z = -0.5;
}

SynchServo::SynchServo(MegaServo *servo_1, MegaServo *servo_2, MegaServo *servo_3, int  A_B_or_C, int either_145_or_236, int offset_1, int offset_2, int offset_3)
{
  servo1 = servo_1;
  servo2 = servo_2;
  servo3 = servo_3;
  
  letter = A_B_or_C;
  group  = either_145_or_236;

  offset1 = offset_1;
  offset2 = offset_2;
  offset3 = offset_3;

  L1x = 1.6;
  L1y = 5.55;
  L1z = 4.72;
  L2x = 0;
  L2y = 7.2;
  L2z = 0;
  L3x = 0;
  L3y = 11.47;
  L3z = -0.5;
}

int SynchServo::direct_kinematics(int output_dimension, int angleA, int angleB, int angleC)
{
  //cinematic_direct based on left side 
  float theta1 = (angleA)  *2 *PI /360;
  float theta2 = (angleB)  *2 *PI /360;
  float theta3 = (angleC)  *2 *PI /360;

  float length_from_top = L1y + L2y*sin(theta2) + L3y*sin(theta2 + (PI - theta3)); 

  float x_pos = sin(theta1 - PI/2) * length_from_top;
  float y_pos = cos(theta1 - PI/2) * length_from_top;

  if(output_dimension == 1)
  {
    return x_pos;
  }
  if (output_dimension == 2)
  {
    return y_pos;
  }
  

}

int SynchServo::inverse_kinematics(int left_or_right, float X, float Y, float Z)
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
  if (group == 1)
  {
    servo1->write(angle_selon_patte_gauche + offset1);
    servo2->write(180 - (angle_selon_patte_gauche + offset2)); //2400
    servo3->write(angle_selon_patte_gauche + offset3);
  }
  
  if (group == 2)
  {
    servo1->write(180 - (angle_selon_patte_gauche + offset1));
    servo2->write(angle_selon_patte_gauche + offset2);
    servo3->write(180 - (angle_selon_patte_gauche + offset3));
  }
}

void SynchServo::move_position(float X, float Y, float Z)
{
  if(group == 1) //group145
  {
    servo1->write(        inverse_kinematics(left,   X, Y, Z)       );
    servo2->write(        inverse_kinematics(right, -X, Y, Z)       );
    servo3->write(        inverse_kinematics(left,   X, Y, Z)       );

  }

  if(group == 2) //group236
  {
    servo1->write(        inverse_kinematics( right, -X, Y, Z)       );
    servo2->write(        inverse_kinematics( left ,  X, Y, Z)       );
    servo3->write(        inverse_kinematics( right, -X, Y, Z)       );
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

#define VIN                            A0
#define BAUD                           115200  // Frequence de transmission serielle
#define UPDATE_PERIODE                 100    // Periode (ms) d'envoie d'etat general

#define MODE_MANUEL                    1         // Used to set robot in manuel mode
#define MODE_AUTOMATIC                 2         // Used to set robot in automatic mode

#define initial_angle_A                90         //Angle of servo A in initialized state
#define initial_angle_B                180         //Angle of servo B in initialized state
#define initial_angle_C                145       //Angle of servo C in initialized state
#define initial_angle_D1               83         //Angle of the neck

#define standing_angle_B               140        // Angle for B servos in standing position
#define standing_angle_C               140        // Angle for C servos in standing position

#define walking_angle_B_increase       20         // Angle to lift up B when walking
#define Walking_angle_A_increase       20         // Angle of servo A used in forward movements

#define sidestep_angle                 10         // Angle used in sidestepping
#define turn_angle                     20         // angle used in turning

#define angle_backward                 20         // Angle of servo A used in backward movements
#define angle_rotation                 20         // Angle of servo A used in spinning movements

#define angle_laydown_B                160          // Angle used to lay robot on ground

#define mandible_open_angle            97         //Servo angle for mandibles to be open
#define mandible_close_angle           130        //Servo angle for mandibles to be closed for grabbing

#define head_turn_angle                45         // Angle the head turns by when calling head to turn

#define arena_sizex                    200                //Width of the arena (cm)
#define arena_sizey                    200                //length of the arena (cm)
#define initial_position_x             arena_sizex/2      //Initial position of the robot on the x axis (cm)
#define initial_position_y             arena_sizey/2      //Initial position of the robot on the y-axis (cm)
#define initial_pos_x_pixel            127                //Initial position x in pixels in hmi
#define initial_pos_y_pixel            123.5              //Initial position y in pixels in hmi

#define map_limit_top_y                43.5               // arena limit position in pixels on hmi
#define map_limit_bottom_y             193.5              // arena limit position in pixels on hmi
#define map_limit_left_x               57.5               // arena limit position in pixels on hmi
#define map_limit_right_x              197                // arena limit position in pixels on hmi


// 43.5 en haut en y ||    193.5 en bas y ||  gauche en x 57.5 || 197 a droite en x || centre 127 en x || centre 123.5 an y

#define turn_left_drift_error_factor   1                  // Error factor when calculating change in orientatio when turning left

#define offset_front_back              25                 // Angle of offset from middle legs of front and back legs
#define offset_A1                      3  + offset_front_back   //ok
#define offset_A2                      20 + offset_front_back //ok
#define offset_A3                      2    //ok
#define offset_A4                      -5   //ok
#define offset_A5                      0  - offset_front_back //ok
#define offset_A6                      -6 - offset_front_back //ok  
#define offset_B1                      0    //ok
#define offset_B2                      -2   //ok
#define offset_B3                      8    //ok
#define offset_B4                      -2   //ok
#define offset_B5                      4    //ok
#define offset_B6                      2    //ok
#define offset_C1                      4    //ok
#define offset_C2                      7    //
#define offset_C3                      4    //ok
#define offset_C4                      7    //
#define offset_C5                      -1   //ok 
#define offset_C6                      13   //ok
#define offset_D1                      0
#define offset_D2                      0

#define step_delay                     300               // delay between each steps

#define max_current_b4_forced_stop     9.8
#define min_battery_voltage            9.1

#define dropoff_angle_tolerance        10
#define dropoff_x_tolerance            5
#define dropoff_y_tolerance            5

//--- CASE NUMBERS ----
#define INITIALIZATION                 0         // Initial case of the robot when turned on
#define WAIT                           1         // Case when robot is waiting for a command
#define MOVE_FORWARD                   2         // Case for robot to move forward
#define MOVE_BACKWARD                  3         // Case for robot to move backward
#define SIDESTEP_LEFT                  4         // Case for robot to move sideways left
#define SIDESTEP_RIGHT                 5         // Case for robot to move sideways right
#define TURN_LEFT                      6         // Case for robot to spin on itself left
#define TURN_RIGHT                     7         // Case for robot to spin on itself right
#define PICKUP                         8         // Case for robot to pick up object
#define DROP                           9         // Case for robot to drop object
#define STAND                          10        // Case for robot to get up off the ground
#define LAYDOWN                        11        // Case for robot to gently drop to ground
#define HEAD_LEFT                      12        // Case to turn the head to the left
#define HEAD_RIGHT                     13        // Case to turn the head to the right
#define HEAD_CENTRE                    14        // Case to turn head back tostraight position
#define AUTOMATIC                      15        // Case for when the robot is in automatic mode

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
#define A1_Pin                         42  //Digital Port for Servo A1
#define B1_Pin                         41  //Digital Port for Servo B1
#define C1_Pin                         40  //Digital Port for Servo C1

#define A2_Pin                         25  //Digital Port for Servo A2
#define B2_Pin                         26  //Digital Port for Servo B2
#define C2_Pin                         27  //Digital Port for Servo C2

#define A3_Pin                         39  //Digital Port for Servo A3
#define B3_Pin                         38  //Digital Port for Servo B3
#define C3_Pin                         37  //Digital Port for Servo C3

#define A4_Pin                         28  //Digital Port for Servo A4
#define B4_Pin                         29  //Digital Port for Servo B4
#define C4_Pin                         30  //Digital Port for Servo C4

#define A5_Pin                         36  //Digital Port for Servo A5
#define B5_Pin                         35  //Digital Port for Servo B5
#define C5_Pin                         34  //Digital Port for Servo C5

#define A6_Pin                         31  //Digital Port for Servo A6
#define B6_Pin                         32  //Digital Port for Servo B6
#define C6_Pin                         33  //Digital Port for Servo C6

#define D1_Pin                         23  //Digital Port for Servo D1 (head)
#define D2_Pin                         44  //Digital Port for Servo D2 (mandibules)

#define voltage_pin                    A3
#define pin_voltage_LED                10

/*---------------------------- GLOBAL VARIABLES ---------------------------*/

volatile bool shouldSend_ =            false;    // Ready to send message flag
volatile bool shouldRead_ =            false;    // Ready to read message flag
volatile bool shouldPulse_ =           false;    // Generate a pulse flag
volatile bool isInPulse_ =             false;    // Currently in a pulse flag

SoftTimer timerSendMsg_;                          // Send message timer
SoftTimer timerPulse_;                            // Duration of a pulse timer

uint16_t pulseTime_ =                  0;        // Pulse time in ms

int time =                             0;        // Loop timer
uint32_t Time;                                   // Timer for SmoothMovementWhileV2

int operation_mode =                   MODE_MANUEL;       //Determines whether robot is in automatic or manuel mode
int command =                          INITIALIZATION;    //Variable to give command (inicate which case to do)
bool robot_is_standing =               false;             //Varibale indicates if robot is standing or lying down

bool current_overload =                false;
bool insufficient_voltage =            false;
bool electrical_shutdown =             false;

float current_position_x =               arena_sizex/2;                 //Current position of the robot on the x axis (cm)
float current_position_y =               arena_sizey/2;                 //Current position of the robot on the y-axis (cm)
int current_orientation =              0;                //Current angle of orientation of robot (deg) in a counterclockwise rotation of x-axis
float current_orientation_rad =          0;                //Current angle of orientation of robot (rad) in a counterclockwise rotation of x-axis
float cur_position_x_pixel =             initial_pos_x_pixel;           //Current position of the robot on the x axis (cm)
float cur_position_y_pixel =             initial_pos_y_pixel;           //Current position of the robot on the y-axis (cm)

int future_position_x =                0;                 //Calculated future position of the robot on the x axis (cm)
int future_position_y =                0;                 //Calculated future of the robot on the y-axis (cm)
int future_orientation =               0;                 //Calculated future angle of orientation of robot (deg) in a counterclockwise rotation of x-axis

bool mouvement_ok =                    false;             //Indicates if mouvement will make robot leave designated arena

float distance_from_target =           300;               //Distance robot must cover to reach target object (cm)
float drop_off1x =                      0;                //Coordinate on x-axis of object 1 drop location
float drop_off1y =                      0;                //Coordinate on y-axis of object 1 drop location
float drop_off2x =                      0;                //Coordinate on x-axis of object 2 drop location
float drop_off2y =                      0;                //Coordinate on y-axis of object 2 drop location


int leg_identification =               0;                 //Leg number to be used in movement

double time1 =                         0.0;               //Variable used if a dt needs to be calculated
double time2 =                         0.0;               //Variable used if a dt needs to be calculated

int step =                             1;                 //To organize motion in each case
float t;                                                  //Variable to use as timer for steps

float real_current;                                       //Current consumption in amps
float real_voltage;                                       //Present battery voltage

float step_distance =                  0;                 // Theoretical distance made by current case

bool in_possession =                   false;             // Variable to indicate if target object is being grabbed by robot (Automatic mode)
bool object_detected =                 false;             // Variable to indicate if camera is identifying the target object (automatic mode)
int  object_aim =                      0;                 // Indicates if robot is to the left (1), dead center (2), to the right (3) of the camera, undetected (0) (mode automatic)
bool in_grab_range =                   false;             // Indicates if robot is close enough to grab object
float grab_range =                     5;                 // How close robot has to be to be able to grab object   
float target_distance =                1000.0;            // The distance the camera detects the object to be from the robot 
int which_image =                      2;                 // Inidcates which of the images is being seen (0= happy 1=angry 2 = nothing)
int automatic_search_count =           0;                 // Indicates which process to do when seraching for object in automatic mode
int head_orientation =                 2;                 // Indicates if the head it turned or not (1 = left) (2 = centre) (3 = right)
float go_to_angle =                    0;                 // Indicate to which angle reach when searching in automatic mode
int spin_until_object_aligned =        0;                 // Indicates if robot is aligning itself towards the detected object (1 = left) (2 = right)
bool automatic_done =                  false;             // Indicates if the automatic mode has drop the target at the dropoff point

bool test_variable =                   0;                 // This variable can be used to conduct tests while debugging. It should not be in the code if not presently using it in a test
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



SynchServo A145_(A1_p, A4_p, A5_p, A, group145, offset_A1, offset_A4, offset_A5);
SynchServo B145_(B1_p, B4_p, B5_p, B, group145, offset_B1, offset_B4, offset_B5);
SynchServo C145_(C1_p, C4_p, C5_p, C, group145, offset_C1, offset_C4, offset_C5);

SynchServo A236_(A2_p, A3_p, A6_p, A, group236, offset_A2, offset_A3, offset_A6);
SynchServo B236_(B2_p, B3_p, B6_p, B, group236, offset_B2, offset_B3, offset_B6);
SynchServo C236_(C2_p, C3_p, C6_p, C, group236, offset_C2, offset_C3, offset_C6);



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
void SmoothMovementWhileV2(MegaServo servo, int Speed, int Angle, int DelayTime);

float current();
float battery_voltage();
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

  //pinMode(current_pin, INPUT);
  pinMode(voltage_pin, INPUT);
  pinMode(VIN, INPUT);
  pinMode(pin_voltage_LED, OUTPUT);

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
  D2_.attach(D2_Pin);

  command = INITIALIZATION;
  t = millis();
  Time = millis();
}


// Main loop (infinite) 
void loop() {

  
  if(shouldRead_){
    readMsg();
  }
  if(shouldSend_){
    sendMsg();
  }


  real_current = current();
  real_voltage = battery_voltage();


  if(operation_mode == MODE_MANUEL) // if in manuel mode reset mode automatic
  {
    automatic_done = false;
  } 

  if(which_image != 2)
  {
    object_detected = true;
  }
  else
  {
    object_detected = false;
  }

  // if(real_current >= max_current_b4_forced_stop)     // If current reaches limit 
  // { 
  //   current_overload = true;
  // }

  if(real_voltage <= min_battery_voltage)             // If voltage reaches lower limit
  { 
    insufficient_voltage = true;
  }

  // if(current_overload == true || insufficient_voltage == true)  // if "voltage or current error" trigger shutdown
  // {
  //   electrical_shutdown = true;
  //   command = WAIT;
  // }
  

  if(robot_is_standing == false && (command != STAND && command != INITIALIZATION))
  {
    command = WAIT;
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
        D1_.write(initial_angle_D1);  
        D2_.write(mandible_open_angle);  

          if (step == 7)
          {
            step = 1;
            command = WAIT;
          }

        break;

        case WAIT :                // Waiting for a command

            t = millis();

            if(operation_mode == MODE_AUTOMATIC && automatic_done == false)
            {
              command = AUTOMATIC;
            }
            
            if (electrical_shutdown == true)  //If electrical problem stop moving
            {
            }

        break;

        case MOVE_FORWARD :                // Move one step forward sequence

          step_distance = A145_.direct_kinematics(1,initial_angle_A + turn_angle, standing_angle_B, standing_angle_C);
          current_orientation_rad = current_orientation /360 * 2* PI;
          
          if(current_position_x + 2*step_distance * cos(current_orientation_rad) < arena_sizex && current_position_x + step_distance * cos(current_orientation_rad) > 0)
          {
            if(current_position_y + 2*step_distance * sin(current_orientation_rad) < arena_sizey && current_position_y + step_distance * sin(current_orientation_rad) > 0)
            {
              mouvement_ok = true;
            }
          }

          if(mouvement_ok)
          {
            stepsequence(1, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
            stepsequence(2, step_delay, &A145_, initial_angle_A + Walking_angle_A_increase);
            stepsequence(3, step_delay, &B145_, standing_angle_B);
            stepsequence(4, step_delay, &B236_, standing_angle_B + walking_angle_B_increase);
            stepsequence(5, step_delay, &A145_, initial_angle_A);
            stepsequence(6, step_delay, &A236_, initial_angle_A + Walking_angle_A_increase);
            stepsequence(7, step_delay, &B236_, standing_angle_B);
            stepsequence(8, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
            stepsequence(9, step_delay, &A236_, initial_angle_A);
            stepsequence(10, step_delay, &B145_, standing_angle_B);

            if (step == 11)
            {
              step = 1;
              current_position_x = current_position_x + 2* step_distance * cos(current_orientation_rad);
              current_position_y = current_position_y + 2*step_distance * sin(current_orientation_rad);
              mouvement_ok = false;
              command = WAIT;
            }
          }
          

        break;

        case MOVE_BACKWARD :                // Move one step backward sequence
          stepsequence(1, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
          stepsequence(2, step_delay, &A145_, initial_angle_A - Walking_angle_A_increase);
          stepsequence(3, step_delay, &B145_, standing_angle_B);
          stepsequence(4, step_delay, &B236_, standing_angle_B + walking_angle_B_increase);
          stepsequence(5, step_delay, &A145_, initial_angle_A);
          stepsequence(6, step_delay, &A236_, initial_angle_A - Walking_angle_A_increase);
          stepsequence(7, step_delay, &B236_, standing_angle_B);
          stepsequence(8, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
          stepsequence(9, step_delay, &A236_, initial_angle_A);
          stepsequence(10,step_delay, &B145_, standing_angle_B);

          if (step == 11)
          {
            step = 1;
            command = WAIT;
          }

        break;

        case SIDESTEP_LEFT :                // Lateral step left sequence
          stepsequence(1, step_delay, &B236_, standing_angle_B + walking_angle_B_increase);
          sidestepsequence(2, step_delay, &C145_, sidestep_angle);
          stepsequence(3, step_delay, &B236_, standing_angle_B);
          stepsequence(4, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
          sidestepsequence(5, step_delay, &C145_, 0);
          stepsequence(6, step_delay, &B145_, standing_angle_B);

      
          if (step == 7)
          {
            step = 1;
            command = WAIT;
          }
        break;

        case SIDESTEP_RIGHT :                // Lateral step right sequence

          stepsequence(1, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
          sidestepsequence(2, step_delay, &C236_, sidestep_angle);
          stepsequence(3, step_delay, &B145_, standing_angle_B);
          stepsequence(4, step_delay, &B236_, standing_angle_B + walking_angle_B_increase);
          sidestepsequence(5, step_delay, &C236_, 0);
          stepsequence(6, step_delay, &B236_, standing_angle_B);
      
          if (step == 7)
          {
            step = 1;
            command = WAIT;
          }
        break;

        case TURN_LEFT :                // Pivot counter-clockwise sequence
          stepsequence(1, step_delay, &B236_, standing_angle_B + walking_angle_B_increase);
          turnstepsequence(2, step_delay, &A236_, -turn_angle);
          stepsequence(3, step_delay, &B236_, standing_angle_B);
          stepsequence(4, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
          turnstepsequence(5, step_delay, &A236_, 0);
          stepsequence(6, step_delay, &B145_, standing_angle_B);

           if (step == 7)
          {
            step = 1;
            current_orientation = current_orientation + (turn_angle * turn_left_drift_error_factor);
            command = WAIT;
          }
    
        break;

        case TURN_RIGHT :                // Pivot clockwise sequence
          stepsequence(1, step_delay, &B236_, standing_angle_B + walking_angle_B_increase);
          turnstepsequence(2, step_delay, &A236_, turn_angle);
          stepsequence(3, step_delay, &B236_, standing_angle_B);
          stepsequence(4, step_delay, &B145_, standing_angle_B + walking_angle_B_increase);
          turnstepsequence(5, step_delay, &A236_, 0);
          stepsequence(6, step_delay, &B145_, standing_angle_B);

           if (step == 7)
          {
            step = 1;
            current_orientation = current_orientation - (turn_angle * turn_left_drift_error_factor);
            command = WAIT;
          }
        break;

        case PICKUP :                // Pickup object sequence
          D2_.write(mandible_close_angle); 
          if(millis() > (t + step_delay))
            {
              t = millis();
              command = WAIT; 
            }      
        break;

        case DROP :                // Drop object sequence
          D2_.write(mandible_open_angle);  
          if(millis() > (t + step_delay))
            {
              t = millis();
              command = WAIT; 
            } 
        break;

        case STAND :               // Get up off ground
          stepsequence(1, 0, &C145_, standing_angle_C );
          stepsequence(2, step_delay, &C236_, standing_angle_C );
          stepsequence(3, 0, &B145_, standing_angle_B );
          stepsequence(4, step_delay, &B236_, standing_angle_B );
          
            if (step == 5)
            {
              step = 1;
              robot_is_standing = true;
              command = WAIT;
            }
        break;

        case LAYDOWN :                // Gently lands body on the ground and sets legs into transport position
          stepsequence(1, 0, &B145_, angle_laydown_B );
          stepsequence(2, step_delay, &B236_, angle_laydown_B );
          stepsequence(3, 0, &C145_, standing_angle_C );
          stepsequence(4, step_delay, &C236_, standing_angle_C );
          
            if (step == 5)
            {
              step = 1;
              robot_is_standing = false;
              command = WAIT;
            }
        break;

        case HEAD_LEFT :
          D1_.write(initial_angle_D1 + head_turn_angle);  
          if(millis() > (t + 5* step_delay))
            {
              head_orientation = 1;
              command = WAIT; 
            } 
        break;

        case HEAD_RIGHT :
          D1_.write(initial_angle_D1 - head_turn_angle);  
          if(millis() > (t + 5* step_delay))
            {
              head_orientation = 3;
              command = WAIT; 
            } 
        break;

        case HEAD_CENTRE :
          D1_.write(initial_angle_D1);  
          if(millis() > (t + step_delay))
            {
              t = millis();
              head_orientation = 2;
              command = WAIT; 
            } 
        break;

        case AUTOMATIC :
        
          //if object is not in robots possession
          if(in_possession == false)
          {
              // if object not detected 
              if(object_detected == false && spin_until_object_aligned == 0)
              {
                if (automatic_search_count == 6)
                {
                    command = MOVE_FORWARD;
                    automatic_search_count = 0;
                    
                }
                if (automatic_search_count == 5)
                {
                    command = TURN_LEFT;
                    if( abs(current_orientation - go_to_angle) < 5 )
                    {
                      automatic_search_count++;
                    }  
                }
                
                if (automatic_search_count == 4)
                {
                    go_to_angle = (current_orientation + 90) % 360;
                    automatic_search_count++;
                }
                if (automatic_search_count == 3)
                {
                  command = MOVE_FORWARD;
                  if ( (arena_sizex - current_position_x) < (arena_sizex/4) || current_position_x < (arena_sizex/4) || (arena_sizey - current_position_y) < (arena_sizey/4) || current_position_y < (arena_sizey/4) )
                  {
                    automatic_search_count++;
                  }
                }
                if (automatic_search_count == 2)
                {
                  command = HEAD_CENTRE;
                  automatic_search_count++;
                }
                
                if (automatic_search_count == 1)
                {
                  command = HEAD_RIGHT;
                  automatic_search_count++;
                }
                if (automatic_search_count == 0)
                {
                  command = HEAD_LEFT;
                  automatic_search_count++;
                }
            }


              // if object detected and head is not looking forward
              if ((object_detected == true && head_orientation != 2) || spin_until_object_aligned != 0)
              {
                    if (head_orientation == 1)
                    {
                      spin_until_object_aligned = 1;
                    }
                    if (head_orientation == 3)
                    {
                      spin_until_object_aligned = 2;
                    }

                    if (spin_until_object_aligned == 1)
                    {
                      command = TURN_LEFT;
                      if(object_detected ==true)
                      {
                        spin_until_object_aligned = 0;
                      }
                    }
                     if (spin_until_object_aligned == 2)
                    {
                      command = TURN_RIGHT;
                      if(object_detected ==true)
                      {
                        spin_until_object_aligned = 0;
                      }
                    }

                     if(head_orientation !=2)
                    {
                      command = HEAD_CENTRE;
                    }
              }
              
               
              // if object detected and head is looking forward
              if(object_detected == true && head_orientation == 2 && spin_until_object_aligned == 0)
              {
                automatic_search_count = 0;
                    //check if object is left, right or center of camera
                    //if left turn left until center
                    if(object_aim == 1)
                    {
                      command = TURN_LEFT;
                    }

                    //if right turn right until center
                    if(object_aim == 3)
                    {
                      command = TURN_RIGHT;
                    }

                    //if center move forward
                    if(object_aim == 2 && in_grab_range == false)
                    {
                      command = MOVE_FORWARD;
                    }

                    //check if object is in pickup distance                
                    if (target_distance < grab_range)
                    {
                      in_grab_range = true;
                    }
                    else        // ------------might be a problem when comes time to grab and too close to see image
                    {
                      in_grab_range = false;
                    }
                    

                    //if in grab distance grab target
                    if(in_grab_range == true)
                    {
                      command = PICKUP;
                      in_possession = true;
                    }
              }
          }

          //if object is in robots possession 
          if(in_possession == true)
          {
            //if happy
              if(which_image == 0)
              {
               
              float distancex_from_dropoff = drop_off1x - current_position_x; 
              float distancey_from_dropoff = drop_off1y - current_position_y; 
              float angle_to_dropoff = tan(distancey_from_dropoff/distancex_from_dropoff) *360/(2*PI);

                //if not at drop off position
                if( abs(current_position_x - drop_off1x) > dropoff_x_tolerance && abs(current_position_y - drop_off1y) > dropoff_y_tolerance)
                {
                    //if not in correct orrientation to reach target: turn
                    if (abs(angle_to_dropoff - current_orientation) > dropoff_angle_tolerance)
                    {
                      if ( (angle_to_dropoff - current_orientation) < 180 && (angle_to_dropoff - current_orientation) > 0)
                      {
                        command = TURN_LEFT;
                      }
                      else
                      {
                        command = TURN_RIGHT;
                      }
                    }
                    else // If in right orientation: walk forward
                    {
                      command = MOVE_FORWARD;
                    }
                }
                else // If at drop off position
                {
                  command = DROP;
                  automatic_done = true;
                }
              }

              //if angry
              if(which_image == 1)
              {
               
              float distancex_from_dropoff = drop_off2x - current_position_x; 
              float distancey_from_dropoff = drop_off2y - current_position_y; 
              float angle_to_dropoff = tan(distancey_from_dropoff/distancex_from_dropoff) *360/(2*PI);

                //if not at drop off position
                if( abs(current_position_x - drop_off2x) > dropoff_x_tolerance && abs(current_position_y - drop_off2y) > dropoff_y_tolerance)
                {
                    //if not in correct orrientation to reach target: turn
                    if (abs(angle_to_dropoff - current_orientation) > dropoff_angle_tolerance)
                    {
                      if ( (angle_to_dropoff - current_orientation) > 0 && (angle_to_dropoff - current_orientation) < 180 )
                      {
                        command = TURN_LEFT;
                      }
                      else
                      {
                        command = TURN_RIGHT;
                      }
                    }
                    else // If in right orientation: walk forward
                    {
                      command = MOVE_FORWARD;
                    }
                }
                else // If at drop off position
                {
                  command = DROP;
                  automatic_done = true;
                  operation_mode = MODE_MANUEL;
                }
              }
          }
        break;
    }

    cur_position_x_pixel = map_limit_left_x + (current_position_x/arena_sizex) * (map_limit_right_x - map_limit_left_x);
    cur_position_y_pixel = map_limit_bottom_y - (current_position_y/arena_sizey) * (map_limit_bottom_y - map_limit_top_y);

  timerSendMsg_.update();
}

/*---------------------------Function definition communication ------------------------*/

void serialEvent(){shouldRead_ = true;}

void timerCallback(){shouldSend_ = true;}

void sendMsg(){

  /* Envoit du message Json sur le port seriel */
  StaticJsonDocument<500> doc;
  // Elements du message

  doc["time"]      = (millis()/1000.0);
  doc["cur_x"]  = current_position_x; 
  doc["cur_y"]  = current_position_y;
  doc["cur_angle"]  = current_orientation;
  doc["Case"] = command;
  doc["current"] = real_current;
  doc["voltage"] = real_voltage;
  // doc["VISION_DIS"] = target_distance;
  // doc["VISION_OBJ"] = which_image;
  doc["Mode"] = operation_mode;
  doc["cur_x_map"] = cur_position_x_pixel;
  doc["cur_y_map"] = cur_position_y_pixel;

  doc["Servo_A1"]  = A1_.read();
  doc["Servo_B1"]  = B1_.read();
  doc["Servo_C1"]  = C1_.read();
  doc["Servo_A2"]  = A2_.read();
  doc["Servo_B2"]  = B2_.read();
  doc["Servo_C2"]  = C2_.read();
  doc["Servo_A3"]  = A3_.read();
  doc["Servo_B3"]  = B3_.read();
  doc["Servo_C3"]  = C3_.read();
  doc["Servo_A4"]  = A4_.read();
  doc["Servo_B4"]  = B4_.read();
  doc["Servo_C4"]  = C4_.read();
  doc["Servo_A5"]  = A5_.read();
  doc["Servo_B5"]  = B5_.read();
  doc["Servo_C5"]  = C5_.read();
  doc["Servo_A6"]  = A6_.read();
  doc["Servo_B6"]  = B6_.read();
  doc["Servo_C6"]  = C6_.read();
  doc["Servo_D1"]  = D1_.read();

  
 
  // Serialisation
  serializeJson(doc, Serial);
  // Envoit
  Serial.print("*");
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

  parse_msg = doc["MODE"];

   if(!parse_msg.isNull()){
     operation_mode = doc["MODE"].as<int>();
  }
  parse_msg = doc["CASE"];

   if(!parse_msg.isNull()){
     command = doc["CASE"].as<int>();
  }

  parse_msg = doc["VISION_DIS"];
  if(!parse_msg.isNull()){
     target_distance = doc["VISION_DIS"].as<float>();
  }

  parse_msg = doc["VISION_OBJ"];
  if(!parse_msg.isNull()){
     which_image = doc["VISION_OBJ"].as<float>();
  }

  parse_msg = doc["VISION_MOVE"];
  if(!parse_msg.isNull()){
     object_aim = doc["VISION_MOVE"].as<float>();
  }

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
 if((0 < current_position_x) && (current_position_x < arena_sizex) && (0 < current_position_y) && (current_position_y < arena_sizey))
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
            servos->servo1->write(standing_angle_C + angle + servos->offset1 );
            servos->servo2->write(180 - standing_angle_C + angle - servos->offset2 );
            servos->servo3->write(standing_angle_C + angle + servos->offset3 );
          }

          if(servos->group == 2)
          {
            servos->servo1->write(180 - standing_angle_C - angle - servos->offset1);
            servos->servo2->write(standing_angle_C - angle + servos->offset2);
            servos->servo3->write(180 - standing_angle_C - angle - servos->offset3);
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
            servos->servo1->write(initial_angle_A + angle + servos->offset1 );
            servos->servo2->write(180 - initial_angle_A + angle + servos->offset2);
            servos->servo3->write(initial_angle_A + angle + servos->offset3 );
          }

          if(servos->group == 2)
          {
            servos->servo1->write(180 - initial_angle_A + angle - servos->offset1 );
            servos->servo2->write(initial_angle_A + angle + servos->offset2 );
            servos->servo3->write(180 - initial_angle_A + angle - servos->offset3 );
          }
          
          if(millis() > (t + delay_microseconds))
            {
              t = millis();
              step ++;
            }
        }
  return;
}

void SmoothMovementWhileV2(MegaServo servo, int Speed, int Angle, int DelayTime)
{
  int GapToAngle = abs(Angle-servo.read());
  int GoToAngle = 0;
  int Direction = Angle-servo.read();
  int ElseCount = 0;
  while(GapToAngle!=0)
  {
    if(millis()>=Time+DelayTime)
    {
      if(GapToAngle <= Speed)
      {
        GoToAngle = Angle;
        servo.write(GoToAngle);
      }
      else if(Direction>0 && GapToAngle!=0)
      {
        GoToAngle = servo.read()+Speed;
        servo.write(GoToAngle);
      }
      else if(Direction<0 && GapToAngle!=0)
      {
        GoToAngle = servo.read()-Speed;
        servo.write(GoToAngle);
      }
      else
      {
        servo.write(Angle);
        break;
      }
      Time = millis();
      GapToAngle = abs(Angle-GoToAngle);
    }
    else if (ElseCount>=1 && millis()>=Time+DelayTime)
    {
      if(Direction>0)
      {
        servo.write(GoToAngle+Speed);
        GapToAngle = abs(Angle-servo.read());
        Direction = Angle-servo.read();
      }
      else if(Direction<0)
      {
        servo.write(GoToAngle-Speed);
        GapToAngle = abs(Angle-servo.read());
        Direction = Angle-servo.read();
      }
      ElseCount = 0;
    }
    else
    {
      ElseCount += 1;
    }
  }
}

float current(){
  const float VCC = 5.0;  
  const float QVt = 0.5*VCC;
  float sensitivity = 0.100;
  float voltage_raw;
  voltage_raw = abs((5.0/1023.0) * analogRead(VIN));
  float voltage; 
  voltage = voltage_raw - QVt + 0.012;
  float current;
  current = voltage/sensitivity;
  return current;
}

// LED pin 10 et 11

float battery_voltage()
{
    
    int sensor_value = analogRead(voltage_pin);
    float voltage_analog = sensor_value * (5.0 / 1023.0);
    float voltage = voltage_analog * 3;

    if (voltage < min_battery_voltage) // If battery voltage is getting too close to 9V
    {
        digitalWrite(pin_voltage_LED, HIGH);
    }
    else
    {
        digitalWrite(pin_voltage_LED, LOW);
    }

    return voltage;
}