/* 
 * GRO 302 - Conception d'un robot mobile
 * Code de démarrage
 * Auteurs: Roundnet
 * date: 24 juin 2021
*/

/*------------------------------ Librairies ---------------------------------*/
#include <LibS3GRO.h>
#include <ArduinoJson.h>
//#include <Tests.h> // Vos propres librairies
/*------------------------------ Constantes ---------------------------------*/
using namespace std;

#define BAUD                            115200    // Frequence de transmission serielle
#define UPDATE_PERIODE                  100       // Periode (ms) d'envoie d'etat general

#define MAGPIN                          32        // Port numerique pour electroaimant J-18
#define POTPIN                          A5        // Port analogique pour le potentiometre

#define PASPARTOUR                      64        // Nombre de pas par tour du moteur
#define RAPPORTVITESSE                  60        // Rapport de vitesse du moteur
#define RAYON_ROUE                      0.06      // Diamètres des roues
#define MOTOR_ID                        1         //Permet de choisir l'ID du moteur utiliser
#define GEAR_RATIO                      2         //Permet de choisir un gearRatio en fonction des engrenage choisit
#define FACTEUR_MAGIQUE                 1.1       //Ajoute 10% de distance pour compenser l'arondissement des ratio
#define DISTANCE_AVANT_OBSTACLE         0.22   //Permet de savoir la position que le robot doit prendre pour commencer son oscillation
#define PID_KP_LENT                     2.0
#define PID_KI_LENT                     0.25
#define PID_KD_LENT                     0.2
#define PID_KP_RAPIDE                   13
#define COMPTEUR                        4
#define BOUTON_PIN                      38         // Le id du bouton de demarage

//Différent cas pour la séquence du projet du sapin
#define START                           0
#define ATTENTE                         1
#define AVANCE_INITIAL                  2
#define OSCILLATION_DEBUT               3
#define ARRET_OSCILLATION               4
#define PASSE_OBSTACLE                  5
#define AVANCE_ALLER                    6
#define LACHE_SAPIN                     7
#define AVANCE_RETOUR                   8
#define PREND_SAPIN                     9
#define ARRET_TOTAL                     100

/*---------------------------- variables globales ---------------------------*/

ArduinoX AX_;                                     // objet arduinoX
MegaServo servo_;                                 // objet servomoteur
VexQuadEncoder vexEncoder_;                       // objet encodeur vex
IMU9DOF imu_;                                     // objet imu
PID pid_x;                                        // objet PID x
PID pid_q;                                        // objet PID q
//Tests tests;

volatile bool shouldSend_ =             false;    // drapeau prêt à envoyer un message
volatile bool shouldRead_ =             false;    // drapeau prêt à lire un message
volatile bool shouldPulse_ =            false;    // drapeau pour effectuer un pulse
volatile bool isInPulse_ =              false;    // drapeau pour effectuer un pulse

SoftTimer timerSendMsg_;                          // chronometre d'envoie de messages
SoftTimer timerPulse_;                            // chronometre pour la duree d'un pulse

uint16_t pulseTime_ =                   0;        // temps dun pulse en ms

int time =                              0;        //timer pour la loop
int32_t compteur_encodeur =             0;        //Encodeur du moteur

int choix =                             ATTENTE;  //sert pour le switch case

bool goal_position_atteint =            false;    //Permet de savoir si la positon est atteinte
bool goal_angle_atteint =               false;    //Permet de savoir si l'anlge du pendule est atteinte
bool prendre_sapin =                    false;    //Permet de savoir si à la fin de l'arrêt de l'oscillation, il faut prendre ou laisser le sapin
bool sapinLacher =                      false;    //Permet de savoir si le sapin à été laché
bool casZero =                          false;    //Permet de savoir si le cas START est actif
bool oscillation_finis =                false;
bool go =                               false;
bool go2 =                              false;
bool go3 =                              false;  

double fonction =                       0.0;      //fonction de tests dans la loop
double goal_voulu_angle =               0.0;      //Permet de dire l'angle voulue
double position_depart =                0.03;      //Permet de savoir la position initial du robot
double position_obstacle =              0.75;      //Permet de savoir la position de l'obstacle
double position_depot =                 1.2;      //Permet de savoir la position du dépot du sapin
double distance_ins =                   0.0;      //Permet de savoir la distance instantanné du véhicule pour calculer la vitesse
double hauteur_obstacle =               0.0;      //Permet de savoir la hauteur de l'obstacle
double distance_old =                   0.0;      //Permet de savoir la distance précédente pour le calcul de la vitesse
double temps_ins =                      0.0;      //Permet de savoir le temps instantanné du véhicule pour calculer la vitesse
double temps_old =                      0.0;      //Permet de savoir le temps précédente pour le calcul de la vitesse

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
float cur_angle =                       0.0;      //Permet de savoir l'angle en temps réelle du pendule
float pwm_correction  =                 0.0;
int i =                                   0;      


float old_angle;
float new_angle;
float vitesse_angle;
float old_temps;
float new_temps;

/*------------------------- Prototypes de fonctions -------------------------*/
void timerCallback();
void startPulse();
void endPulse();
void sendMsg(); 
void readMsg();
void serialEvent();
void digitalWrite(uint8_t pin, uint8_t val);

// Fonctions pour le PID de position
double PIDmeasurement();
void PIDcommand(double cmd);
void PIDgoalReached();

// Fonctions pour le PID de l'angle
double PIDmeasurement_angle();
void PIDcommand_angle(double cmd);
void PIDgoalReached_angle();

double Calculangle();
float reduce_angle();
double vitesse();



/*---------------------------- fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // initialisation de la communication serielle
  AX_.init();                       // initialisation de la carte ArduinoX 
  imu_.init();                      // initialisation de la centrale inertielle
  vexEncoder_.init(2,3);            // initialisation de l'encodeur VEX
  // attache de l'interruption pour encodeur vex
  attachInterrupt(vexEncoder_.getPinInt(), []{vexEncoder_.isr();}, FALLING);
  
  // Chronometre envoie message
  timerSendMsg_.setDelay(UPDATE_PERIODE);
  timerSendMsg_.setCallback(timerCallback);
  timerSendMsg_.enable();

  // Chronometre duration pulse
  timerPulse_.setCallback(endPulse);
  
  // Initialisation du PID de position
  pid_x.setGains(PID_KP_LENT, PID_KI_LENT ,PID_KD_LENT);
  // Attache des fonctions de retour
  pid_x.setMeasurementFunc(PIDmeasurement);
  pid_x.setCommandFunc(PIDcommand);
  pid_x.setAtGoalFunc(PIDgoalReached);
  pid_x.setEpsilon(0.015);
  pid_x.setPeriod(100);
  //pid_x.enable();

  // Initialisation du PID d'angle
  pid_q.setGains(5, 0 ,0);
  // Attache des fonctions de retour
  pid_q.setMeasurementFunc(PIDmeasurement_angle);
  pid_q.setCommandFunc(PIDcommand_angle);
  pid_q.setAtGoalFunc(PIDgoalReached_angle);
  pid_q.setEpsilon(5);
  pid_q.setPeriod(100);
  //pid_q.enable();


  //Defenition du IO pour l'ÉLECTRO-AIMANT
  pinMode(MAGPIN, OUTPUT); 
  //digitalWrite(MAGPIN, LOW);

//definition du IO du bouton
  pinMode(BOUTON_PIN,INPUT);                                        // JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ  Pinmode pour le bouton de demarrage

  //Initialise l'état initiale du pendule comme étant 0 degree
  Potentio_zero = analogRead(POTPIN);
}


// Boucle principale (infinie) 
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
  

  

  // Mise à jour des chronometres
  timerSendMsg_.update();
  timerPulse_.update();


  switch(choix) 
  {
    case ATTENTE : //Cas pour l'activation de l'électroaimant et attente du commencement de la séquence
    //Serial.println("Case 1");
      AX_.setMotorPWM(MOTOR_ID, 0);
      digitalWrite(MAGPIN, HIGH);
      //Serial.println("attente");
      if(digitalRead(38))
      {
          choix = START;                                        //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj   ajout d'un bouton pour faciliter les testage
      }                                                  //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj

    break;

    case START: //Cas pour l'initialisation des variables
    //Serial.println("Case 2");
      //Serial.println("start");
      casZero = true;
      goal_voulu_angle = -55;
      goal_position_atteint = false;
      goal_angle_atteint = false;
      oscillation_finis = false;
      go = false;
      go2 = false;
      
      
      pid_x.setGoal(position_obstacle-DISTANCE_AVANT_OBSTACLE);
      //digitalWrite(MAGPIN, HIGH);
      
      sapinLacher = false;
      
      choix = AVANCE_INITIAL;
      AX_.setMotorPWM(MOTOR_ID, 0.0);  
      pid_x.enable();
    
    break;

    case AVANCE_INITIAL: //Cas pour aller  la position initiale avant d'osciller
    //Serial.println("Case 3");
      //Serial.println("avance_initial");
      casZero = false;
      pid_x.setEpsilon(0.02);
      pid_x.setGains(PID_KP_LENT, PID_KI_LENT ,PID_KD_LENT);
      
      
      
      AX_.setMotorPWM(MOTOR_ID, pulsePWM_);
      //Serial.println("case = pid_x.getGoal()");
      //Serial.println(pid_x.getGoal());


     if(goal_position_atteint)
      {
        choix = OSCILLATION_DEBUT;
        goal_position_atteint = false;
        pid_x.setEpsilon(0.015);
        pid_x.setGoal(position_depot);                                // depot + distance
        //pid_q.enable(); 
        pid_x.enable();
        //pid_q.setGoal(goal_voulu_angle);
        temps2 = millis();
        //Serial.println("avance initial fini");
      }

    break;

    case OSCILLATION_DEBUT: //Cas pour osciller le pendule a environ 60 degree pour passer par dessus l'obstacle
    //Serial.println("Case 4");
      //Serial.println("oscillation_debut");
      if(!oscillation_finis)
            {
              temps1= millis()-temps2;
        
              fonction = -0.9*sin(5.0*(temps1/1000.0));
              AX_.setMotorPWM(MOTOR_ID,fonction);
              pid_x.enable();
              oscillation_finis = false;
            }
      
      if(cur_angle<goal_voulu_angle&&!go2)
      {
        oscillation_finis = true;
        go = true;
        goal_voulu_angle = 0;
        AX_.setMotorPWM(MOTOR_ID,0.7);
        }


      if((cur_angle>goal_voulu_angle&&go == true)||go2)
      {
        go2 = true;
        pid_x.setGains(PID_KP_RAPIDE,0,0);
        AX_.setMotorPWM(MOTOR_ID, 1);

        if(cur_pos > (position_obstacle+0.35))
        {
          oscillation_finis = false;
          goal_angle_atteint = false;
          choix = AVANCE_ALLER;
          goal_position_atteint = false;
          pid_x.setEpsilon(0.05);
          pid_x.enable();
        }
      }
      
     

    break;

    case AVANCE_ALLER : //Cas pour se rendre au dessus du panier à sapin
    //Serial.println("Case 5");
      //Serial.println("avance_aller");
    
        // Serial.println("pos depot");
        // Serial.println(position_depot);
        // Serial.println(" pos cur");
        // Serial.println(cur_pos);

      pid_x.setGoal(position_depot);
      pid_x.setGains(PID_KP_LENT, PID_KI_LENT ,PID_KD_LENT);

      AX_.setMotorPWM(MOTOR_ID, 1);

      

      if(cur_pos > (position_depot-0.1))                                //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj  condition changer parce que celle davant avait pas de sens
      {                                                                 
        choix = ARRET_OSCILLATION;         
        goal_position_atteint = false;
        pid_x.enable();
        pid_x.setEpsilon(0.03);
        pid_x.setGains(3,PID_KI_LENT,PID_KD_LENT);
        pid_x.setGoal(position_depot);
        old_angle = Calculangle();
        old_temps = millis();
        goal_voulu_angle = 0;
        go3 = false;
      }

    break;

    case ARRET_OSCILLATION: //Cas pour arreter le pendule vers 0 degree au dessus du panier
      //On fait un sinus qui tend vers 0 ???---------------------------------------------------------------
      //problème avec le case 
                //pid_q.setGains(5,0,0);
                //pid_q.setGoal(0);
                //AX_.setMotorPWM(MOTOR_ID,pulsePWM_angle);       //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
      if(!go3)
      {
      pwm_correction = reduce_angle();
      AX_.setMotorPWM(MOTOR_ID,(pwm_correction/abs(pwm_correction)));             // 50*pwm_correction);              //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
     
      
        if((abs(vitesse_angle) < 45.0 && abs(new_angle) < 2.0))//||go3==true //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
        {
            choix = ATTENTE;
        } 
      }
      
                     // mis en comentaire pour tester
     if((abs(vitesse_angle) < 0.2 && abs(new_angle) < 10.0))//||go3==true  
      {
        go3 = true;
        AX_.setMotorPWM(MOTOR_ID, pulsePWM_);

          if (goal_position_atteint) //&&(abs(vitesse_angle) < 0.2 && abs(new_angle+old_angle) < 10)) //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
          {
          goal_position_atteint = false;
          pid_x.enable();
          pid_x.setEpsilon(0.02);
          choix = LACHE_SAPIN;
          temps2 = millis();
        }
      }
     
    break;

    case LACHE_SAPIN: //Cas pour lacher le pendule dans le panier
      //Serial.println("lache_sapin");
      sapinLacher = true;

      AX_.setMotorPWM(MOTOR_ID,0);
      digitalWrite(MAGPIN, LOW);
      temps1 = millis();
      
      if( (temps1- temps2) >= 3000 )
      {
       sapinLacher = false;
       choix = AVANCE_RETOUR;
        pid_x.enable();
        goal_position_atteint = false;
      }
      break;

    case AVANCE_RETOUR: //Cas de passer par dessus l'obstacle pour le retour
      //Serial.println("avance retour");
      pid_x.setGoal(position_depart);
      pid_x.setGains(1.6, PID_KI_LENT ,PID_KD_LENT);
      AX_.setMotorPWM(MOTOR_ID,pulsePWM_);
      if(goal_position_atteint)
      {
        goal_position_atteint = false;
        prendre_sapin = true;
        pid_x.enable();
        choix = ATTENTE;
      }
    break;

    case PREND_SAPIN: //Cas d'arret du pendule au dessus du sapin = 0 degree
      //Serial.println("prend sapin");
        choix = ATTENTE;
        goal_angle_atteint = false;
        prendre_sapin = false;
        // AX_.resetEncoder(MOTOR_ID);
        digitalWrite(MAGPIN, HIGH);

    break;

    case ARRET_TOTAL: //Cas pour mettre les moteurs à 0. Donc, arret du moteur et de l'électroaimant
      //Serial.println("arret_total");
      AX_.setMotorPWM(MOTOR_ID,0);
      digitalWrite(MAGPIN, LOW);
      AX_.resetEncoder(MOTOR_ID);
    break;


    

  }//Fin du switch case
 
 //PID update
  pid_x.run();
  pid_q.run();

  Calculangle();
  vitesse();
  PIDmeasurement();

}

/*---------------------------Definition de fonctions ------------------------*/

void serialEvent(){shouldRead_ = true;}

void timerCallback(){shouldSend_ = true;}

void startPulse(){
  /* Demarrage d'un pulse */
  timerPulse_.setDelay(pulseTime_);
  timerPulse_.enable();
  timerPulse_.setRepetition(1);
  AX_.setMotorPWM(0, pulsePWM_);
  AX_.setMotorPWM(1, pulsePWM_);
  shouldPulse_ = false;
  isInPulse_ = true;
}

void endPulse(){
  /* Rappel du chronometre */
  AX_.setMotorPWM(0,0);
  AX_.setMotorPWM(1,0);
  timerPulse_.disable();
  isInPulse_ = false;
}

void sendMsg(){
  /* Envoit du message Json sur le port seriel */
  StaticJsonDocument<500> doc;
  // Elements du message

  doc["time"]      = (millis()/1000.0);
  doc["goal"]      = pid_x.getGoal();
  doc["pulsePWM"]  = pulsePWM_;
  doc["cur_vel"]   = cur_vel;
  doc["cur_pos"]   = cur_pos;
  doc["cur_angle"] = cur_angle;
  doc["Etat"]      = choix;
  doc["actualTime"] = pid_x.getActualDt();
  doc["position_obstacle"] = position_obstacle;
  doc["hauteur_obstacle"] = hauteur_obstacle;
  doc["position_depot"] = position_depot;
  doc["sapin_lacher"] = sapinLacher;
  doc["casZero"] = casZero;
  doc["vitesse_angulaire"] = reduce_angle();

  //doc["potVex"] = analogRead(POTPIN);
  //doc["encVex"] = vexEncoder_.getCount();
  //doc["measurements"] = PIDmeasurement();
  //doc["voltage"] = AX_.getVoltage();
  //doc["current"] = AX_.getCurrent(); 
  //doc["measurements"] = PIDmeasurement();
  //doc["voltage"] = AX_.getVoltage();
  //doc["current"] = AX_.getCurrent(); 
  //doc["pulseTime"] = pulseTime_;
  //doc["inPulse"] = isInPulse_;
  //doc["accelX"] = imu_.getAccelX();
  //doc["accelY"] = imu_.getAccelY();
  //doc["accelZ"] = imu_.getAccelZ();
  //doc["gyroX"] = imu_.getGyroX();
  //doc["gyroY"] = imu_.getGyroY();
  //doc["gyroZ"] = imu_.getGyroZ();
  //doc["temp"] = imu_.getTemp(); //Température
  //doc["isGoal"] = pid_x.isAtGoal();

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
  
  //À LAISSER DANS LA PREMIÈRE FENETRE QT------------------------------------------------------------À FAIRE-------------------------------------

  // Analyse des éléments du message message
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

  parse_msg = doc["setGoal"];
  if(!parse_msg.isNull()){
    pid_x.disable();
    pid_x.setGains(doc["setGoal"][0], doc["setGoal"][1], doc["setGoal"][2]);
    pid_x.setEpsilon(doc["setGoal"][3]);
    pid_x.setGoal(doc["setGoal"][4]);
    pid_x.enable();
  }

  parse_msg = doc["Start"];
  if(!parse_msg.isNull())
  {
    choix = START;
  }

  parse_msg = doc["Stop"];
  if(!parse_msg.isNull())
  {
    //pinMode(MAGPIN, LOW);
    //AX_.setMotorPWM(MOTOR_ID, 0);
    choix = ARRET_TOTAL;
  }

  parse_msg = doc["Distance"];
  if(!parse_msg.isNull())
  {
    position_obstacle = (doc["Distance"][1]);
  }

  parse_msg = doc["Distance"];
  if(!parse_msg.isNull())
  {
    position_depot = (doc["Distance"][2]);
  }  
  //--------------------------------------------------------------------------------------------------------------
}


// Fonctions pour le PID
double vitesse(){

  double distancetot = 0;
  double dt = 0;

  temps_ins = millis()/1000.0;//temps en sec

  PIDmeasurement();// calcul de la variable distance_ins
  distancetot = cur_pos-distance_old;
  dt = temps_ins-temps_old;
  cur_vel = distancetot /dt;

  //Serial.println("vitesse:");
  //Serial.println(cur_vel);

  distance_old = cur_pos;
  temps_old = temps_ins;

  return cur_vel;
}

// Fonctions pour le PID de position
double PIDmeasurement(){
  double pulse;
  double distance;
  double tour;

  pulse = AX_.readEncoder(MOTOR_ID);

  tour = pulse/(3200.0*GEAR_RATIO);
  distance = (tour * RAYON_ROUE * 2.0 * PI ) / FACTEUR_MAGIQUE;

  cur_pos = distance;

  return distance;
}

void PIDcommand(double cmd){
  pulsePWM_ = cmd;
}

void PIDgoalReached(){
  goal_position_atteint = true;
}


/////////////////////////////////////////////////////////

void PIDAngle()
{
  
}

// Fonctions pour le PID d'angle
double PIDmeasurement_angle(){
  
  //angle_pendule = (analogRead(POTPIN)-Potentio_zero)*(180.0/880.0);

  //cur_angle = angle_pendule;

  //Serial.print("angle_pendule");
  //Serial.println(angle_pendule);

  return Calculangle();
}

double Calculangle()
{
  angle_pendule = (analogRead(POTPIN)-Potentio_zero)*(180.0/880.0);

  cur_angle = angle_pendule;

  return angle_pendule;
}


void PIDcommand_angle(double cmd_angle){
  pulsePWM_angle = cmd_angle;
  //Serial.println("cmd_angle");
  //Serial.println(cmd_angle);
}


void PIDgoalReached_angle(){
  goal_angle_atteint = true;
}


float reduce_angle(){
  new_angle = Calculangle();
  new_temps = millis();
  vitesse_angle = 1000.0 * (new_angle - old_angle) / (new_temps - old_temps);                      //JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ
  old_angle = new_angle;
  old_temps = new_temps;
  return vitesse_angle;
}


