/*
Projet S3 2021
@author Natael Laroche-Latulippe
@version 1.0 15/07/2021
*/

#ifndef TESTS_H_
#define TESTS_H_

#include <LibS3GRO.h>
#include <ArduinoJson.h>

#define BAUD            115200      // Frequence de transmission serielle
#define UPDATE_PERIODE  100         // Periode (ms) d'envoie d'etat general

#define MAGPIN          32         // Port numerique pour electroaimant J-16
#define POTPIN          A5          // Port analogique pour le potentiometre

#define PASPARTOUR      64          // Nombre de pas par tour du moteur
#define RAPPORTVITESSE  50          // Rapport de vitesse du moteur

class Tests
{
    public:
    Tests();
    ~Tests();
    void Tests_unitaire();

/*---------------------------- variables globales ---------------------------*/

ArduinoX AX_;                       // objet arduinoX
MegaServo servo_;                   // objet servomoteur
VexQuadEncoder vexEncoder_;         // objet encodeur vex
IMU9DOF imu_;                       // objet imu
PID pid_x;                           // objet PID x
PID pid_q;                           // objet PID q

volatile bool shouldSend_ = false;  // drapeau prêt à envoyer un message
volatile bool shouldRead_ = false;  // drapeau prêt à lire un message
volatile bool shouldPulse_ = false; // drapeau pour effectuer un pulse
volatile bool isInPulse_ = false;   // drapeau pour effectuer un pulse

SoftTimer timerSendMsg_;            // chronometre d'envoie de messages
SoftTimer timerPulse_;              // chronometre pour la duree d'un pulse

uint16_t pulseTime_ = 0;            // temps dun pulse en ms
float pulsePWM_ = 0.1;              // Amplitude de la tension au moteur pour la position[-1,1]
float pulsePWM_angle = 0.1;         //Amplitude de la tension au moteur pour l'angle [-1,1]

float Axyz[3];                      // tableau pour accelerometre
float Gxyz[3];                      // tableau pour giroscope
float Mxyz[3];                      // tableau pour magnetometre

int time = 0;                       //Timer pour la loop
int32_t compteur_encodeur = 0;      //Encodeur du moteur

int choix = 10;                      //Srt pour le switch case
double fonction = 0;                 //Fonction de tests dans la loop
bool goal_position_atteint = false;  //Permet de savoir si la positon est atteinte
bool goal_angle_atteint = false;     //Permet de savoir si l'anlge du pendule est atteinte
int goal_voulu_position = 0;         //Permet de dire la distance voulue
int goal_voulu_angle = 0;            //Permet de dire l'angle voulue
float Potentio_zero = 0;             //permet de savoir la valeur initiale du pendule
float deg = 0;                       //Permet de savoir l'anlge du pendule
bool rentree = false;                //Permet de calculer Potentio_zero une seule fois
float distance = 0;                  //Permet de savoir la distance en temps réelle du robot

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
    
    protected:
    void myProtectedFunction();
    
    private:
    void myPrivateFunction();
    int myInt = 0;
    bool myBool = false;
};
#endif // LibExample_H_