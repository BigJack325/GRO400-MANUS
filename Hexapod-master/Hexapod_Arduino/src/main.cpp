/* 
 * GRO 400 - Conception d'un robot articulé
 * Auteurs: MANUS
 * date: 25 janvier 2022
*/

/*------------------------------ Librairies ---------------------------------*/
#include <LibS3GRO.h>
#include <ArduinoJson.h>

/*------------------------------ Constantes ---------------------------------*/
using namespace std;

#define BAUD                            115200    // Frequence de transmission serielle
#define UPDATE_PERIODE                  100     // Periode (ms) d'envoie d'etat general

//-----------------------------PIN SERVO-----------------------------------
#define MAGPIN                          32        // Port numerique pour electroaimant J-18

/*---------------------------- variables globales ---------------------------*/

//MegaServo servo_;                                 // objet servomoteur
//IMU9DOF imu_;                                     // objet imu central inertielle

volatile bool shouldSend_ =             false;    // drapeau prêt à envoyer un message
volatile bool shouldRead_ =             false;    // drapeau prêt à lire un message
volatile bool shouldPulse_ =            false;    // drapeau pour effectuer un pulse
volatile bool isInPulse_ =              false;    // drapeau pour effectuer un pulse

SoftTimer timerSendMsg_;                          // chronometre d'envoie de messages
SoftTimer timerPulse_;                            // chronometre pour la duree d'un pulse

uint16_t pulseTime_ =                   0;        // temps dun pulse en ms

int time =                              0;        //timer pour la loop
int32_t compteur_encodeur =             0;        //Encodeur du moteur

//------------------------------ VARIABLES-----------------------------------------------

// double fonction =                       0.0;      //fonction de tests dans la loop
// double goal_voulu_angle =               0.0;      //Permet de dire l'angle voulue
// double position_depart =                0.03;      //Permet de savoir la position initial du robot
// double position_obstacle =              0.75;      //Permet de savoir la position de l'obstacle
// double position_arrive =                 1.2;      //Permet de savoir la position du dépot du sapin
// double distance_ins =                   0.0;      //Permet de savoir la distance instantanné du véhicule pour calculer la vitesse
// double hauteur_obstacle =               0.0;      //Permet de savoir la hauteur de l'obstacle
// double distance_old =                   0.0;      //Permet de savoir la distance précédente pour le calcul de la vitesse
// double temps_ins =                      0.0;      //Permet de savoir le temps instantanné du véhicule pour calculer la vitesse
// double temps_old =                      0.0;      //Permet de savoir le temps précédente pour le calcul de la vitesse

double temps1 =                         0.0;
double temps2 =                         0.0;
float pulsePWM_ =                       0.1;      // Amplitude de la tension au moteur pour la position[-1,1]
float pulsePWM_angle =                  0.1;      //Amplitude de la tension au moteur pour l'angle [-1,1]
float Axyz[3];                                    // tableau pour accelerometre
float Gxyz[3];                                    // tableau pour giroscope
float Mxyz[3];                                    // tableau pour magnetometre
float Potentio_zero =                   0.0;      //permet de savoir la valeur initiale du pendule
float angle_pendule =                   0.0;      //Permet de savoir l'agle actuelle du pendule
float cur_pos =                         0.0;      //Permet de savoir la position en temps réelle du pendule
float cur_vel =                         0.0;      //Permet de savoir la vitesse en temps réelle du pendule
int cur_angle =                       0;      //Permet de savoir l'angle en temps réelle du pendule
float pwm_correction  =                 0.0;
int i =                                   0;   
int cur_x = 0;   
int cur_y = 0;
String receivedMessage_;

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



/*---------------------------- fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);              // initialisation de la communication serielle
  //imu_.init();                      // initialisation de la centrale inertielle
  // Chronometre envoie message
  timerSendMsg_.setDelay(UPDATE_PERIODE);
  timerSendMsg_.setCallback(timerCallback);
  timerSendMsg_.enable();

  // Chronometre duration pulse
  timerPulse_.setCallback(endPulse);

}


// Boucle principale (infinie) 
void loop() {

  Serial.flush();
  
  if(shouldRead_){
    readMsg();
  }
  if(shouldSend_){
    sendMsg();
  }
  if(shouldPulse_){
    startPulse();
  }

//----------------------FAIRE SWITCH CASE ICI-------------------------------


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
  doc["pulsePWM"]  = pulsePWM_;
  doc["cur_x"] = cur_x;
  doc["cur_y"] = cur_y;
  doc["cur_angle"] = cur_angle;
  doc["cur_vel"]   = cur_vel;
  doc["Servo_1"] = 101;
  doc["Servo_2"] = 102;
  doc["Servo_3"] = 103;
  doc["Servo_4"] = 104;
  doc["Servo_5"] = 105;
  doc["Servo_6"] = 106;
  doc["Servo_7"] = 107;
  doc["Servo_8"] = 108;
  doc["Servo_9"] = 109;
  doc["Servo_10"] = 110;
  doc["Servo_11"] = 111;
  doc["Servo_12"] = 112;
  doc["Servo_13"] = 113;
  doc["Servo_14"] = 114;
  doc["Servo_15"] = 115;
  doc["Servo_16"] = 116;
  doc["Servo_17"] = 117;
  doc["Servo_18"] = 118;
  doc["Servo_19"] = 119;
 
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
  // Analyse des éléments du message
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
}
  //-----------------------AJOUTER FONCTIONS DE MOUVEMENT ICI-----------------------------------------------------
/*

class LowerBody
{
  public:
  LowerBody(int NumberOfLegs);
  ~LowerBody();
  int forward();
  int backward();
  int rotate();
}

class Leg
{
  public:
  Leg(string MotorName_z, string MotorName_x_sup, string MotorName_x_inf, int PinNumber_z, int PinNumber_x_sup, int PinNumber_x_inf);
  ~Leg();
  string DataMotor [1][3]; //Contient tous les noms de moteurs
  string DataPin [1][3]; //Contient tous les numeros de pins
  int DataAngle[2][3]; //Contient, en ordre de lignes, les angles initiaux et actuels
  virtual int avancer();
};
 
class FrontLeg : public Motor
{
  public:
  Patte(int MotorNumber_z, int MotorNumber_x_sup, int MotorNumber_x_inf, int Pin_z, int Pin_x_sup, int Pin_x_inf);
  ~Patte();
  int avancer(int Direction, int Speed);
};

class BackLeg : public Motor
{
  public:
  Patte(int MotorNumber_z, int MotorNumber_x_sup, int MotorNumber_x_inf, int Pin_z, int Pin_x_sup, int Pin_x_inf);
  ~Patte();
  int avancer(int Direction, int Speed);
};

int BackLeg::avancer(int Direction, int Speed)
{
  
}

Leg::Leg(string MotorName_z, string MotorName_x_sup, string MotorName_x_inf, int Pin_z, int Pin_x_sup, int Pin_x_inf)
{
  for (int i=0; i++; i<3)
  {
    Pin_z = DataPin [0][0];
    Pin_x_sup = DataPin [0][1];
    Pin_x_inf = DataPin [0][2];
    Motor_z = DataMotor [0][0];
    Motor_x_sup = DataMotor [0][1];
    Motor_x_inf = DataPin [0][2];
    for (int i=0; i++; i<3)
    {
      Servo DataMotor[0][i];
      DataMotor[0][i].attach(DataPin[0][i]);
    }
  }
}

int Leg::avancer(int Direction, int Speed)
{
  for(int i=0; i++; <3)
    angle_initial_z = DataMotor[0][i].read();
    angle_initial_x_inf = Motor_z.read();
    angle_initial_z = Motor_z.read();
}

*/
//---------------------------------------------------------------
//  Variables et librairies
//---------------------------------------------------------------

#include <Servo.h>
#include <string.h>

Servo servo_z,servo_x_sup, servo_x_inf; // create servo object to control a servo
// twelve servo objects can be created on most boards

String TestMoteur [1][3] = {"servotest_z","servotest_x_sup","servotest_x_inf"};

int pos_z, pos_x_sup, pos_x_inf, new_pos;
int angle_actuel, angle_initial;
int delay_servo;

//---------------------------------------------------------------
//  Initialisation (Setup)
//---------------------------------------------------------------

void setup(){
Serial.begin(9600);
//myservo.attach(3);

delay_servo = 30;
pos_z = 0;
pos_x_sup = 0;
pos_x_inf = 0;
new_pos = 0;

angle_initial = 90;

servo_z.attach(22);
servo_x_sup.attach(24);// attaches the servo on pin 3 to the servo object
servo_x_inf.attach(26);

servo_z.write(angle_initial);// met la position du moteur à 90 deg
delay(15);
servo_x_sup.write(angle_initial);
delay(15);
servo_x_inf.write(angle_initial);
angle_actuel = servo_x_inf.read();
}

//------------------------------------------------------------
//  Boucle principale (Main)
//------------------------------------------------------------

void loop(){


for (int i=0; i++; i<3)
{
  Servo TestMoteur[0][i];
  TestMoteur[0][i].attach(22+2*i);
}
}
//------------------------------------------------------------
//  Test pour mouvement avec vitesse desiree
//------------------------------------------------------------
/*
Serial.print("Angle actuel du servo: ");
Serial.println(angle_actuel);
Serial.print("Angle initial du servo: ");
Serial.println(angle_initial);

for (pos_z = 1; pos_z <= 15; pos_z += 1)
//for (pos_z = 1; pos_z <= 180; pos_z += 1)
{
  if (pos_z<5)
  {
    angle_actuel += pos_z;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else if (servo_x_inf.read() < angle_initial+45)
  {
    angle_actuel += 5;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else{}
}

delay(5000);

Serial.print("Angle actuel du servo: ");
Serial.println(angle_actuel);
Serial.print("Angle initial du servo: ");
Serial.println(angle_initial);

for (pos_z = 1; pos_z <= 15; pos_z += 1)
//for (pos_z = 1; pos_z <= 180; pos_z += 1)
{
  if (pos_z<5)
  {
    angle_actuel -= pos_z;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else if (servo_x_inf.read() > angle_initial)
  {
    angle_actuel -= 5;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else{}
}

delay(5000);
*/
//-------------------------------------------------------
//  Test pour mouvement des moteurs
//-------------------------------------------------------
/*
for (pos_2 = 90; pos_2 >= 0; pos_2 -= 5)
{
  servo_1.write(pos_2);
  delay(15);
}
delay(1000);
*/

/*
for (pos = 0; pos <= 180; pos += 1) 
{ // goes from 0 degrees to 180 degrees in steps of 1 degree
  myservo.write(pos); // tell servo to go to position in variable 'pos'
  delay(15); // waits 15 ms for the servo to reach the position
}
for (pos = 180; pos >= 0; pos -= 1) 
{ // goes from 180 degrees to 0 degrees
  myservo.write(pos); // tell servo to go to position in variable 'pos'
  delay(15); // waits 15 ms for the servo to reach the position
}
*/

//------------------------------------------------------------
//  Test pour mouvement avec vitesse desiree
//------------------------------------------------------------
/*
Serial.print("Angle actuel du servo: ");
Serial.println(angle_actuel);
Serial.print("Angle initial du servo: ");
Serial.println(angle_initial);

for (pos_z = 1; pos_z <= 15; pos_z += 1)
//for (pos_z = 1; pos_z <= 180; pos_z += 1)
{
  if (pos_z<5)
  {
    angle_actuel += pos_z;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else if (servo_x_inf.read() < angle_initial+45)
  {
    angle_actuel += 5;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else{}
}

delay(5000);

Serial.print("Angle actuel du servo: ");
Serial.println(angle_actuel);
Serial.print("Angle initial du servo: ");
Serial.println(angle_initial);

for (pos_z = 1; pos_z <= 15; pos_z += 1)
//for (pos_z = 1; pos_z <= 180; pos_z += 1)
{
  if (pos_z<5)
  {
    angle_actuel -= pos_z;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else if (servo_x_inf.read() > angle_initial)
  {
    angle_actuel -= 5;
    servo_x_inf.write(angle_actuel);
    delay(delay_servo);
    Serial.println(angle_actuel);
  }
  else{}
}

delay(5000);
*/
//-------------------------------------------------------
//  Test pour mouvements des moteurs
//-------------------------------------------------------
/*
for (pos_2 = 90; pos_2 >= 0; pos_2 -= 5)
{
  servo_1.write(pos_2);
  delay(15);
}
delay(1000);
*/

/*
for (pos = 0; pos <= 180; pos += 1) 
{ // goes from 0 degrees to 180 degrees in steps of 1 degree
  myservo.write(pos); // tell servo to go to position in variable 'pos'
  delay(15); // waits 15 ms for the servo to reach the position
}
for (pos = 180; pos >= 0; pos -= 1) 
{ // goes from 180 degrees to 0 degrees
  myservo.write(pos); // tell servo to go to position in variable 'pos'
  delay(15); // waits 15 ms for the servo to reach the position
}
*/
}
