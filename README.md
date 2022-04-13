# GRO400-MANUS
4th Semester Upgraded Hexapod Project for Robotique Engineering Sherbrooke University

<div id="ManUS" align="center">
    <h1>ManUS</h1>
    <h3>Hexapod Robot : Upgraded Version | Promo66-UdeS-GRO</h3>
</div>

**This academic project is based on an older academic project from robotic engineering undergraduates at Université de Sherbrooke. "Manu" is an hexapod robot able to interact with his environments. Along with 6 legs comes a camera mounted on a 3D printed head, elevating Manu to a whole new level of robotic hexapod. It can walk, look around and chase targets all around him in a restrained area/arena.**

## Table of Contents
- [Setup](#Setup)
    - [Setup for Users](#SetupUsers)
    - [Setup for Repository Contributors](#SetupContrib)
- [Documentation](#Documentation)
    - [Servos Classification](#Servosclassification)
    - [Assembly Guide](#assemblyguide)
    - [Electrical Schematic](#electricalschematic)
    - [Electrical Diagram of the Shield](#electrical)
    - [Code Overview](#Codeexplication)
- [Robot Operation](#Operation)
    - [Robot Movement](#Movement)
    - [Robot Vision](#Vision)
    - [Robot Mods](#Mods)
    - [HMI And Robot Controls](#Controls)
- [External Resources](#Ressources)
    - [Battery, Power Source, and Charger](#Battery)
    - [Microcontroller Board](#Controller)
    - [On-Board Computer](#Computer)
    - [Servomotor for the legs](#Servo1)
    - [Servomotor for the mandibles](#Servo2)
    - [Current Sensor](#Current)
    - [DC/DC Converter](#DCDC)
    - [Fuse 10 Amp](#Fuse)
 

## <a id="Setup"></a>Setup

#### <a id="SetupUsers"></a>Setup for Users

- Clone or download the repository;
- Connect the hexapod's microcontroller (Arduino) to your computer;
- Open the file ```main.cpp``` in the *./Hexapod-Master/Hexapod_Arduino/src* folder, and build the code into the microcontroller with your favorite IDE;
- Disconnect the microcontroller from the computer and connect to the on-board computer (Raspberry Pi);
- Verify your Python version (3.5 and above) and verify if the OpenCV and PyQt5 librairies are present in the Raspberry Pi with ```Import``` command ;
- Run the file ```MANUS_QT``` in the *./Hexapod-Master/Hexapod_Qt* folder;
- Initialize the serial communication.

#### <a id="SetupContrib"></a>Setup for Contributors

## <a id="Documentation"></a>Documentation
#### <a id = "Servosclassification">Classification of the robot servos:</a>

![photo patte hexapod](https://user-images.githubusercontent.com/48064854/162976618-52e0769e-27ba-48a6-97b3-df437f16bacd.PNG)

#### <a id= "assemblyguide">Assembly Guide</a>
You need to 3D print the necessary parts that you can find in the *./CAD* folder.   
We have a complete assembly guide [here](https://github.com/EDP325/GRO400-MANUS/wiki):

#### <a id = "electricalschematic"> Electrical Schematic</a>
Photo to be inserted here.
#### <a id= "electrical">Electrical Diagram of the Shield: </a>
![image](https://user-images.githubusercontent.com/48064854/163028957-0d5c8229-0cb9-4d3b-a6ca-b984c7e7bd09.png)

#### <a id="Codeexplication">Code Overview</a>

The code is divided in two main sections.  The first one is all about the robot movment and is directly written in the Arduino.  This code is written in switch case depending one the manuel/automatic mode and which movment to do.
The second section  includes the vision algorithm and the HMI.  It is written in the Raspberry Pi.  It is made to detect the chosen images and display some variable on the HMI, a screen connected to the Pi.  We chose to use VNCviewer to have a remote connection to the hexapod.

## <a id="Operation"></a>Robot Operation

#### <a id="Movement"></a>Robot Movement
The robot can move forward, backward and sideways.  It can also rotate clockwise and counterclockwise.  The head can rotate and the mandibles can open cand close.  

#### <a id="Vision"></a>Robot Vision
The robot has a vision algorithm that detects these two images: 

![happy](https://user-images.githubusercontent.com/48064854/163003461-6ebe9cd6-cc70-48df-82ff-0c2a40982960.jpg)![angry](https://user-images.githubusercontent.com/48064854/163003538-dd7039e3-41dc-485b-952a-bf78a2d1049d.jpg)



#### <a id="Mods"></a>Robot Mods

#### <a id="Controls"></a>HMI And Robot Controls
Our HMI shows:
- the current view of the cam
- the position of all the servos;
- a map of the arena with the position of the hexapod;
- informations for the camera and messages from the Arduino
- a graphic that shows the the value of a variable over time
- buttons to move the robots

![image](https://user-images.githubusercontent.com/48064854/163049332-fb5af359-9492-4097-8c6c-b21095a80209.png)


## <a id="Ressources"></a>External ressources

Under this section, you will find external links and descriptions of the project's external resources.

#### <a id="Battery"></a>Battery

- Brand Name: Makita
- Model:
    - Battery: BL1041B
    - Power Source: PE00000028
    - Charger: DC10SB
- ASIN:
    - Battery: B017GQJDVM
    - Charger: B01FY4XCB0
- Information / Resources:
    - [Makita Battery Webpage](https://www.makitatools.com/products/details/BL1041B)
    - [Makita Power Source Webpage](https://www.makitatools.com/products/details/PE00000028)
    - [Makita Charger Webpage](https://www.makitatools.com/products/details/DC10SB)

#### <a id="Controller"></a>Microcontroller board

- Brand Name: Arduino
- Model: Mega 2560 R3 (A000067)
- ASIN: B0046AMGW0
- Information / Resources: [Arduino Webpage](https://store.arduino.cc/mega-2560-r3)

#### <a id="Computer"></a>Onboard computer

- Brand Name: Raspberry Pi
- Model: 3B
- ASIN: B01LPLPBS8
- Information / Resources: [Raspberry Pi Webpage](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)

#### <a id="Servo1"></a>Servomotor for the legs

- Brand Name : VIPMOON
- Model : MG996R
- Information / Resources : [Amazon Webpage](https://www.amazon.ca/-/fr/gp/product/B07VT5T9JC/ref=ewc_pr_img_1?smid=A2FI4MJY3VQVX5&psc=1)

#### <a id="Servo2"></a>Servomotor for the mandibles

- Model : MG90S
- Information / Resources : [Amazon Webpage](https://www.amazon.ca/servomoteur-h%C3%A9licopt%C3%A8re-m%C3%A9tallique-intelligent-commandes/dp/B08W26FNQD/)
#### <a id="Current"></a>Current sensor

- Brand Name : ROBOJAX
- Model : ACS712
- ASIN :B07YP4693C
- Information / Resources : [Amazon Webpage](https://www.amazon.ca/gp/product/B07YP4693C/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)

#### <a id="DCDC"></a>DC/DC Converter

- Brand Name : Tyenaza
- Model : Tyenazad03xwg94zo
- ASIN :
- Information / Resources : [Amazon Webpage](https://www.amazon.ca/gp/product/B097QT54C7/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&amp&psc=1)

#### <a id="Camera"></a>Pi Camera

- Brand Name : Keyestudio
- Model : 8541582798
- ASIN :
- Information / Resources : [Amazon Webpage](https://www.amazon.ca/gp/product/B073RCXGQS/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)
    
fuse 10 AMP
#### <a id="Fuse">Fuse 10 AMP 250V</a>

- Brand Name : Littelfuse
- Model : Littelfuse 5x15MM. FAST ACTING 10A.250V.
- Information / Resources :You can buy fuse at any electronics store near your house.  We bought ours [here](https://electro5.com/fr/product/231621/littlefuse-5x15mm-fast-acting-10a250v/) 
