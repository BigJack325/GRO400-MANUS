/*
Exemple de librairie pouvant etre ajoute au projet
*/
#include <Tests.h>

// Class constructor
Tests::Tests(){
    
}

// Class desstructor
Tests::~Tests(){
    
}

// Public Functions
void Tests::Tests_unitaire(){

  /*
  if(shouldRead_){
    readMsg();
  }
  if(shouldSend_){
    sendMsg();
  }
  if(shouldPulse_){
    startPulse();
  }
  */

  // mise a jour des chronometres
  timerSendMsg_.update();
  timerPulse_.update();

  //----------------------------------SECTION MOTEUR-------------------------------------------//

  //Allume les moteur
  //shouldPulse_ = false;

  //Éteindre les moteurs
  //shouldPulse_ = true;

  //-------------------------------SECTION ÉLECTRO-AIMANT-------------------------------------//

  //TEST : Allumer et éteindre l'électro-aimant
  /*
  pinMode(MAGPIN, HIGH); // Activation electroAimant
  if(time > 10000)
  {
      pinMode(MAGPIN, LOW); // Desactivation electroAimant
      time = 0;
  }
  else
  {
    time += 1;
    //delay(1000); //Délai de 1 secondes
  }
  */


  //----------------------------------SECTION ENCODEUR------------------------------------------//

  //compteur_encodeur = AX_.readEncoder(0);
  //Serial.println();


  //--------------------------SECTION CAPTEUR TENSION/COURANT----------------------------------//

  //Voir section void sendMsg();


  //------------------------------SECTION ENCODEUR OPTIQUE-------------------------------------//

  //vexEncoder_.init(2,3); // initialisation de l'encodeur VEX
  // attache de l'interruption pour encodeur vex
  //attachInterrupt(vexEncoder_.getPinInt(),[]{vexEncoder_.isr();},FALLING);
  //Voir section void sendMsg(); pour la fonction en dessous 
  //doc["encVex"] = vexEncoder_.getCount();


  //-----------------------------SECTION POTENTIOMÈTRE GROVE-----------------------------------//
  //retourne des valeurs de 0-1023
  //117-1010
  //Serial.println(analogRead(POTPIN));
  /*
  if(rentree == false)
  {
      Potentio_zero = analogRead(POTPIN);
      rentree = true;
  }

  deg = (analogRead(POTPIN)-Potentio_zero)*(180.0/880.0);
  */
  //Serial.println(deg);

  //etat initiale du pendule = 
  //à 90 degree (vers l'avant du robot) = 
  ////à 90 degree (vers l'arrière du robot) = 


  //--------------------------SECTION CENTRALE INERTIELLE GROVE--------------------------------//

  //Voir section void sendMsg();  pour pour les fonctions en dessous
  //doc["accelZ"] = imu_.getAccelZ();
  //doc["gyroX"] = imu_.getGyroX();

  //Serial.println(((imu_.getAccelY()*((millis()/1000.0)*(millis()/1000.0)))/2.0));
  //distance = distance + ((imu_.getAccelY()*((millis()/1000.0)*(millis()/1000.0)))/2.0);
  //Serial.println(distance);
  //-------------------------------------SECTION PID-------------------------------------------//
  //pid_x.run();
}

// Private Functions
void Tests::myPrivateFunction(){

}

// Protected Functions
void Tests::myProtectedFunction(){

}