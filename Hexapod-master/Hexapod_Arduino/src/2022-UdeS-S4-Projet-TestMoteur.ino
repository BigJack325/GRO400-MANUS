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

// #include <Servo.h>
// #include <string.h>

// Servo servo_z,servo_x_sup, servo_x_inf; // create servo object to control a servo
// // twelve servo objects can be created on most boards

// String TestMoteur [1][3] = {"servotest_z","servotest_x_sup","servotest_x_inf"};

// int pos_z, pos_x_sup, pos_x_inf, new_pos;
// int angle_actuel, angle_initial;
// int delay_servo;

// //---------------------------------------------------------------
// //  Initialisation (Setup)
// //---------------------------------------------------------------

// void setup(){
// Serial.begin(9600);
// //myservo.attach(3);

// delay_servo = 30;
// pos_z = 0;
// pos_x_sup = 0;
// pos_x_inf = 0;
// new_pos = 0;

// angle_initial = 90;

// servo_z.attach(22);
// servo_x_sup.attach(24);// attaches the servo on pin 3 to the servo object
// servo_x_inf.attach(26);

// servo_z.write(angle_initial);// met la position du moteur à 90 deg
// delay(15);
// servo_x_sup.write(angle_initial);
// delay(15);
// servo_x_inf.write(angle_initial);
// angle_actuel = servo_x_inf.read();
// }

// //------------------------------------------------------------
// //  Boucle principale (Main)
// //------------------------------------------------------------

// void loop(){


// for (int i=0; i++; i<3)
// {
//   Servo TestMoteur[0][i];
//   TestMoteur[0][i].attach(22+2*i);
// }
// }
// //------------------------------------------------------------
// //  Test pour mouvement avec vitesse desiree
// //------------------------------------------------------------
// /*
// Serial.print("Angle actuel du servo: ");
// Serial.println(angle_actuel);
// Serial.print("Angle initial du servo: ");
// Serial.println(angle_initial);

// for (pos_z = 1; pos_z <= 15; pos_z += 1)
// //for (pos_z = 1; pos_z <= 180; pos_z += 1)
// {
//   if (pos_z<5)
//   {
//     angle_actuel += pos_z;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else if (servo_x_inf.read() < angle_initial+45)
//   {
//     angle_actuel += 5;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else{}
// }

// delay(5000);

// Serial.print("Angle actuel du servo: ");
// Serial.println(angle_actuel);
// Serial.print("Angle initial du servo: ");
// Serial.println(angle_initial);

// for (pos_z = 1; pos_z <= 15; pos_z += 1)
// //for (pos_z = 1; pos_z <= 180; pos_z += 1)
// {
//   if (pos_z<5)
//   {
//     angle_actuel -= pos_z;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else if (servo_x_inf.read() > angle_initial)
//   {
//     angle_actuel -= 5;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else{}
// }

// delay(5000);
// */
// //-------------------------------------------------------
// //  Test pour mouvement des moteurs
// //-------------------------------------------------------
// /*
// for (pos_2 = 90; pos_2 >= 0; pos_2 -= 5)
// {
//   servo_1.write(pos_2);
//   delay(15);
// }
// delay(1000);
// */

// /*
// for (pos = 0; pos <= 180; pos += 1) 
// { // goes from 0 degrees to 180 degrees in steps of 1 degree
//   myservo.write(pos); // tell servo to go to position in variable 'pos'
//   delay(15); // waits 15 ms for the servo to reach the position
// }
// for (pos = 180; pos >= 0; pos -= 1) 
// { // goes from 180 degrees to 0 degrees
//   myservo.write(pos); // tell servo to go to position in variable 'pos'
//   delay(15); // waits 15 ms for the servo to reach the position
// }
// */

// //------------------------------------------------------------
// //  Test pour mouvement avec vitesse desiree
// //------------------------------------------------------------
// /*
// Serial.print("Angle actuel du servo: ");
// Serial.println(angle_actuel);
// Serial.print("Angle initial du servo: ");
// Serial.println(angle_initial);

// for (pos_z = 1; pos_z <= 15; pos_z += 1)
// //for (pos_z = 1; pos_z <= 180; pos_z += 1)
// {
//   if (pos_z<5)
//   {
//     angle_actuel += pos_z;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else if (servo_x_inf.read() < angle_initial+45)
//   {
//     angle_actuel += 5;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else{}
// }

// delay(5000);

// Serial.print("Angle actuel du servo: ");
// Serial.println(angle_actuel);
// Serial.print("Angle initial du servo: ");
// Serial.println(angle_initial);

// for (pos_z = 1; pos_z <= 15; pos_z += 1)
// //for (pos_z = 1; pos_z <= 180; pos_z += 1)
// {
//   if (pos_z<5)
//   {
//     angle_actuel -= pos_z;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else if (servo_x_inf.read() > angle_initial)
//   {
//     angle_actuel -= 5;
//     servo_x_inf.write(angle_actuel);
//     delay(delay_servo);
//     Serial.println(angle_actuel);
//   }
//   else{}
// }

// delay(5000);
// */
// //-------------------------------------------------------
// //  Test pour mouvements des moteurs
// //-------------------------------------------------------
// /*
// for (pos_2 = 90; pos_2 >= 0; pos_2 -= 5)
// {
//   servo_1.write(pos_2);
//   delay(15);
// }
// delay(1000);
// */

// /*
// for (pos = 0; pos <= 180; pos += 1) 
// { // goes from 0 degrees to 180 degrees in steps of 1 degree
//   myservo.write(pos); // tell servo to go to position in variable 'pos'
//   delay(15); // waits 15 ms for the servo to reach the position
// }
// for (pos = 180; pos >= 0; pos -= 1) 
// { // goes from 180 degrees to 0 degrees
//   myservo.write(pos); // tell servo to go to position in variable 'pos'
//   delay(15); // waits 15 ms for the servo to reach the position
// }
// */
// }
