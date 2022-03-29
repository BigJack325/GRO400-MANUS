from cmath import cos, pi, sin
import encodings
import cv2
import re
import time
import numpy as np
import imutils
from tflite_runtime.interpreter import Interpreter
from imutils.video import VideoStream
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtWidgets import (QApplication, QGridLayout, QMainWindow, QLabel,
                             QLineEdit, QPushButton, QWidget,
                              QFrame, QComboBox, QGraphicsView, QGraphicsItem, QGraphicsScene,
                               QGraphicsPixmapItem, QPlainTextEdit, QDoubleSpinBox, QTextBrowser, QCheckBox, QSlider)
from PyQt5.QtCore import QRect,QIODevice, QCoreApplication, pyqtSignal, Qt, QTimer, QSize
from PyQt5.QtGui import QPixmap, QFont, QBrush, QImage, QKeyEvent, QIcon
from PyQt5.QtChart import QChart,QChartView,QLineSeries
import sys, os, json, math

BAUD_RATE = 115200
UI_UPDATE_RATE = 1000# Ms
CAM_UPDATE_RATE = 10
NUM_SERVOS = 19
MANUAL_SIDE_MOVEMENT  = 10 #pixels
MANUAL_VERTICAL_MOVEMENT = 10 #pixels
CAMERA_WIDTH = 320
CAMERA_HEIGHT= 320

CUSTOM_MODEL_NAME = 'Ant' 
LABEL_MAP_NAME = 'labelmap.txt'


paths = {
    'DISPLAY_IMAGE_PATH': os.path.join('Qt_Images','Display'),
    'BUTTON_IMAGE_PATH': os.path.join('Qt_Images', 'Buttons'),
    'TFLITE_PATH': os.path.join('Tensorflow', 'workspace','models',CUSTOM_MODEL_NAME,'tfliteexport'),
 }


class Ui_MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.updateTimer = QTimer()
        self.msgBuffer_ = ""
        self.counter = 0
        self.jsondata = None
        self.serialCom_ = None

        self.centralWidget = QWidget(self)
        self.gridLayout = QGridLayout(self.centralWidget)

        self.widget = QWidget(self.centralWidget)
        self.line = QFrame(self.widget)
        self.Hexapod_Pic = QLabel(self.widget)
        self.Servo = dict()

        for idx in range(1,NUM_SERVOS+1):
            self.Servo[idx] = QLineEdit(self.widget)

        self.RightButton = QPushButton(self.widget)
        self.RightButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Right.png")))
        self.RightButton.setIconSize(QSize(61,61))
        self.RightButton.setAutoRepeat(True)
        self.RightButton.setAutoRepeatDelay(UI_UPDATE_RATE )#mseconds
        self.RightButton.setAutoRepeatInterval(1000)#mseconds
        self.RotateRightButton = QPushButton(self.widget)
        self.RotateRightButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"RotateR.png")))
        self.RotateRightButton.setIconSize(QSize(61,61))
        self.RotateRightButton.setAutoRepeat(True)
        self.RotateRightButton.setAutoRepeatDelay(UI_UPDATE_RATE )#mseconds
        self.RotateRightButton.setAutoRepeatInterval(1000)#mseconds
        self.LeftButton = QPushButton(self.widget)
        self.LeftButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Left.png")))
        self.LeftButton.setIconSize(QSize(61,61))
        self.LeftButton.setAutoRepeat(True)
        self.LeftButton.setAutoRepeatDelay(UI_UPDATE_RATE )#mseconds        
        self.LeftButton.setAutoRepeatInterval(1000)#mseconds
        self.RotateLeftButton = QPushButton(self.widget)
        self.RotateLeftButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"RotateL.png")))
        self.RotateLeftButton.setIconSize(QSize(61,61))
        self.RotateLeftButton.setAutoRepeat(True)
        self.RotateLeftButton.setAutoRepeatDelay(UI_UPDATE_RATE )#mseconds
        self.RotateLeftButton.setAutoRepeatInterval(1000)#mseconds
        self.FrontButton = QPushButton(self.widget)
        self.FrontButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Front.png")))
        self.FrontButton.setIconSize(QSize(61,61))
        self.FrontButton.setAutoRepeat(True)
        self.FrontButton.setAutoRepeatDelay(UI_UPDATE_RATE )#mseconds  
        self.FrontButton.setAutoRepeatInterval(1000)#mseconds
        self.BackButton = QPushButton(self.widget)
        self.BackButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Back.png")))
        self.BackButton.setIconSize(QSize(61,61))
        self.BackButton.setAutoRepeat(True)
        self.BackButton.setAutoRepeatDelay(UI_UPDATE_RATE )#mseconds  
        self.BackButton.setAutoRepeatInterval(1000)#mseconds
        self.ProneButton = QPushButton(self.widget)

        self.Port_label = QLabel(self.widget)
        self.comboBoxPort = QComboBox(self.widget)

        self.Map_label = QLabel(self.widget)
        self.MapView = Map(self.widget)

        self.Graph_label = QLabel(self.widget)
        self.graph = QChart()
        self.graphView = QChartView(self.widget)
        self.series_ = QLineSeries()

        self.Donnees_label = QLabel(self.widget)
        self.JsonKey = QLineEdit(self.widget)

        self.StartButton = QPushButton(self.widget)
        self.StopButton = QPushButton(self.widget)

        self.Cam_label = QLabel(self.widget)
        self.Cam = VideoTracking(self.widget)
        self.CamDistance_label = QLabel(self.widget)
        self.CamDistanceText = QPlainTextEdit(self.widget)

        self.Angle_label = QLabel(self.widget)
        self.AngleBox = QDoubleSpinBox(self.widget)

        self.Json_label = QLabel(self.widget)
        self.Json_Browser = QTextBrowser(self.widget)

        self.Manual_mode = QCheckBox(self.widget)

        self.setupUi()

    def setupUi(self):
        self.resize(983, 790)

        self.gridLayout.setContentsMargins(9, 9, 11, 9)
        self.gridLayout.setSpacing(6)

        self.line.setGeometry(QRect(600, 20, 20, 751))
        self.line.setFrameShape(QFrame.VLine)
        self.line.setFrameShadow(QFrame.Sunken)

        self.Hexapod_Pic.setGeometry(QRect(285, 400, 321, 361))
        self.Hexapod_Pic.setPixmap(QPixmap(os.path.join(paths['DISPLAY_IMAGE_PATH'],"hexapod.png")))
        self.Hexapod_Pic.setScaledContents(True)

        self.Servo[1].setGeometry(QRect(400, 560, 41, 25))
        self.Servo[2].setGeometry(QRect(350, 540, 41, 25))
        self.Servo[3].setGeometry(QRect(300, 520, 41, 25))
        self.Servo[4].setGeometry(QRect(450, 560, 41, 25))
        self.Servo[5].setGeometry(QRect(500, 540, 41, 25))
        self.Servo[6].setGeometry(QRect(550, 520, 41, 25))
        self.Servo[7].setGeometry(QRect(400, 610, 41, 25))
        self.Servo[8].setGeometry(QRect(350, 610, 41, 25))
        self.Servo[9].setGeometry(QRect(300, 610, 41, 25))       
        self.Servo[10].setGeometry(QRect(450, 610, 41, 25))
        self.Servo[11].setGeometry(QRect(500, 610, 41, 25))
        self.Servo[12].setGeometry(QRect(550, 610, 41, 25))
        self.Servo[13].setGeometry(QRect(400, 660, 41, 25))
        self.Servo[14].setGeometry(QRect(350, 680, 41, 25))
        self.Servo[15].setGeometry(QRect(300, 700, 41, 25))        
        self.Servo[16].setGeometry(QRect(450, 660, 41, 25))
        self.Servo[17].setGeometry(QRect(500, 680, 41, 25))
        self.Servo[18].setGeometry(QRect(550, 700, 41, 25))
        self.Servo[19].setGeometry(QRect(425, 520, 41, 25))

        for idx in range(1,NUM_SERVOS+1):
            self.Servo[idx].setReadOnly(True)        

        self.RightButton.setGeometry(QRect(840, 510, 61, 61))
        self.RotateRightButton.setGeometry(QRect(840, 430, 61, 61))
        self.LeftButton.setGeometry(QRect(680, 510, 61, 61))
        self.RotateLeftButton.setGeometry(QRect(680, 430, 61, 61))
        self.FrontButton.setGeometry(QRect(760, 430, 61, 61))
        self.BackButton.setGeometry(QRect(760, 590, 61, 61))
        self.ProneButton.setGeometry(QRect(760, 510, 61, 61))
        
        self.Port_label.setGeometry(QRect(660, 300, 94, 22))
        self.comboBoxPort.setGeometry(QRect(800, 300, 124, 22))

        self.Map_label.setGeometry(QRect(0, 440, 218, 22))
        self.MapView.setEnabled(True)
        self.MapView.setGeometry(QRect(0, 460, 261, 261))

        self.Donnees_label.setGeometry(QRect(660, 250, 121, 28))
        self.JsonKey.setGeometry(QRect(810, 250, 101, 31))

        self.Graph_label.setGeometry(QRect(620, 10, 218, 22))
        self.graphView.setGeometry(QRect(620, 30, 341, 211))

        self.StartButton.setGeometry(QRect(820, 360, 131, 31))
        self.StopButton.setGeometry(QRect(640, 360, 131, 31))
        font = QFont()
        font.setBold(True)
        self.StopButton.setFont(font)

        self.Cam_label.setGeometry(QRect(0, 170, 50, 22))
        self.Cam.setGeometry(QRect(0, 160, 320, 320))
        self.CamDistance_label.setGeometry(QRect(388, 230, 218, 22))
        self.CamDistanceText.setGeometry(QRect(388, 250, 200, 151))

        self.Angle_label.setGeometry(QRect(615, 680, 94, 22))
        self.AngleBox.setGeometry(QRect(710, 680, 124, 22))
        self.AngleBox.setMinimum(0)
        self.AngleBox.setMaximum(180)
        self.AngleBox.setDecimals(0)
        self.AngleBox.setValue(10)
        self.AngleBox.setSingleStep(1)

        self.Json_label.setGeometry(QRect(388, 10, 218, 22))
        self.Json_Browser.setGeometry(QRect(388, 30, 200, 201))
        font = QFont()
        font.setPointSize(9)
        self.Json_Browser.setFont(font)

        self.Manual_mode.setGeometry(QRect(850, 670, 120, 41))

        self.gridLayout.addWidget(self.widget, 0, 0, 1, 1)
        self.setCentralWidget(self.centralWidget)
        self.retranslateUi()

        self.connectUpdateTimer(UI_UPDATE_RATE)
        self.connectSerialComboBox()
        self.connectButtons()

        # QMetaObject.connectSlotsByName(Self)

    def retranslateUi(self):
        _translate = QCoreApplication.translate
        self.setWindowTitle(_translate("MainWindow", "MANUS"))
        self.JsonKey.setText(_translate("MainWindow", "time"))
        self.Map_label.setText(_translate("MainWindow", "Map:"))
        self.ProneButton.setText(_translate("MainWindow", "PRONE"))
        self.Cam_label.setText(_translate("MainWindow", "Cam:"))
        self.StartButton.setText(_translate("MainWindow", "Start"))
        self.Port_label.setText(_translate("MainWindow", "Port:"))
        self.Donnees_label.setText(_translate("MainWindow", "Donnees brutes:"))
        self.StopButton.setText(_translate("MainWindow", "Stop"))
        self.Angle_label.setText(_translate("MainWindow", "Angle[0,360]"))
        self.Json_label.setText(_translate("MainWindow", "Messages Json de l\'Arduino:"))
        self.Graph_label.setText(_translate("MainWindow", "Graphique:"))
        self.CamDistance_label.setText(_translate("MainWindow", "Distance Cam:"))
        self.Manual_mode.setText(_translate("MainWindow", "Manual Mode"))


    def portCensus(self):
        self.comboBoxPort.clear()
        available_ports = QSerialPortInfo.availablePorts()
        if available_ports == []:
            self.counter = 0

        for port in available_ports:
            self.port_name = port.portName()
            self.comboBoxPort.addItem(self.port_name)

    def connectUpdateTimer(self, updateTime):
        self.updateTimer.timeout.connect(self.OnPeriodicEvent)
        self.updateTimer.start(updateTime)
    
    
    def OnPeriodicEvent(self):
        self.portCensus()
        self.connectMotorLabels()
        self.checkManual()
        if self.Manual_mode.checkState() == 0:
            self.MapView.auto_map_movement(self.jsondata)

        print('*')

    def connectButtons(self):

        #Send message to Arduino for manual movement
        self.StartButton.pressed.connect(lambda: self.ManualMessage("START"))
        self.StopButton.pressed.connect(lambda: self.ManualMessage("STOP"))
        self.RightButton.pressed.connect(lambda: self.ManualMessage("RIGHT"))
        self.LeftButton.pressed.connect(lambda: self.ManualMessage("LEFT"))
        self.FrontButton.pressed.connect(lambda: self.ManualMessage("FRONT"))
        self.BackButton.pressed.connect(lambda: self.ManualMessage("BACK"))
        self.RotateLeftButton.pressed.connect(lambda: self.ManualMessage("RLEFT"))
        self.RotateRightButton.pressed.connect(lambda: self.ManualMessage("RRIGHT"))


        # Change button image when pressed
        self.RightButton.pressed.connect(lambda: self.changeButtonIcon("RIGHT",1))
        self.LeftButton.pressed.connect(lambda: self.changeButtonIcon("LEFT",1))
        self.FrontButton.pressed.connect(lambda: self.changeButtonIcon("FRONT",1))
        self.BackButton.pressed.connect(lambda: self.changeButtonIcon("BACK",1))
        self.RotateLeftButton.pressed.connect(lambda: self.changeButtonIcon("RLEFT",1))
        self.RotateRightButton.pressed.connect(lambda: self.changeButtonIcon("RRIGHT",1))

        # Change button image when released
        self.RightButton.released.connect(lambda: self.changeButtonIcon("RIGHT",0))
        self.LeftButton.released.connect(lambda: self.changeButtonIcon("LEFT",0))
        self.FrontButton.released.connect(lambda: self.changeButtonIcon("FRONT",0))
        self.BackButton.released.connect(lambda: self.changeButtonIcon("BACK",0))
        self.RotateLeftButton.released.connect(lambda: self.changeButtonIcon("RLEFT",0))
        self.RotateRightButton.released.connect(lambda: self.changeButtonIcon("RRIGHT",0))

        
        self.RightButton.pressed.connect(lambda: self.MapView.manual_map_movement("RIGHT",0))
        self.LeftButton.pressed.connect(lambda: self.MapView.manual_map_movement("LEFT",0))
        self.FrontButton.pressed.connect(lambda: self.MapView.manual_map_movement("FRONT",0))
        self.BackButton.pressed.connect(lambda: self.MapView.manual_map_movement("BACK",0))
        self.RotateLeftButton.pressed.connect(lambda: self.MapView.manual_map_movement("RLEFT",int(self.AngleBox.text())))
        self.RotateRightButton.pressed.connect(lambda: self.MapView.manual_map_movement("RRIGHT",int(self.AngleBox.text())))

    def changeButtonIcon(self,button,state):

        

        if button == "RIGHT" and state == 1:
            self.RightButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Right_pressed.png")))
            self.RightButton.setIconSize(QSize(61,61))
        elif button == "RIGHT" and state == 0:
            self.RightButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Right.png")))
            self.RightButton.setIconSize(QSize(61,61))
        elif button == "LEFT" and state == 1:
            self.LeftButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Left_pressed.png")))
            self.LeftButton.setIconSize(QSize(61,61))
        elif button == "LEFT" and state == 0:
            self.LeftButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Left.png")))
            self.LeftButton.setIconSize(QSize(61,61))
        elif button == "FRONT" and state == 1:
            self.FrontButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Front_pressed.png")))
            self.FrontButton.setIconSize(QSize(61,61))
        elif button == "FRONT" and state == 0:
            self.FrontButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Front.png")))
            self.FrontButton.setIconSize(QSize(61,61))
        elif button == "BACK" and state == 1:
            self.BackButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Back_pressed.png")))
            self.BackButton.setIconSize(QSize(61,61))
        elif button == "BACK" and state == 0:
            self.BackButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"Back.png")))
            self.BackButton.setIconSize(QSize(61,61))
        elif button == "RLEFT" and state == 1:
            self.RotateLeftButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"RotateL_pressed.png")))
            self.RotateLeftButton.setIconSize(QSize(61,61))
        elif button == "RLEFT" and state == 0:
            self.RotateLeftButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"RotateL.png")))
            self.RotateLeftButton.setIconSize(QSize(61,61))
        elif button == "RRIGHT" and state == 1:
            self.RotateRightButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"RotateR_pressed.png")))
            self.RotateRightButton.setIconSize(QSize(61,61))
        elif button == "RRIGHT" and state == 0:
            self.RotateRightButton.setIcon(QIcon(os.path.join(paths['BUTTON_IMAGE_PATH'],"RotateR.png")))
            self.RotateRightButton.setIconSize(QSize(61,61))
        


    def ManualMessage(self,msg):
        if msg == "START":
            msg_array = {"CASE":10}
        elif msg == "STOP":
            msg_array = {"CASE":1}
        elif msg == "RIGHT":
            msg_array = {"CASE":5}
        elif msg == "LEFT":
            msg_array = {"CASE":4}
        elif msg == "FRONT":
            msg_array = {"CASE":2}
        elif msg == "BACK":
            msg_array = {"CASE":3}
        elif msg == "RLEFT":
            msg_array = {"CASE":6}
        elif msg == "RRIGHT":
            msg_array = {"CASE":7}
        
        
        data_out = json.dumps(msg_array)
        self.serialCom_.sendMessage(data_out)

        self.RightButton.released.connect(lambda: self.ManualMessage("WAIT"))
        self.LeftButton.released.connect(lambda: self.ManualMessage("WAIT"))
        self.FrontButton.released.connect(lambda: self.ManualMessage("WAIT"))
        self.BackButton.released.connect(lambda: self.ManualMessage("WAIT"))
        self.RotateLeftButton.released.connect(lambda: self.ManualMessage("WAIT"))
        self.RotateRightButton.released.connect(lambda: self.ManualMessage("WAIT"))


    def connectMotorLabels(self):
        if self.jsondata is not None:
            self.Servo[1].setText(str(self.jsondata["Servo_1"]))
            self.Servo[2].setText(str(self.jsondata["Servo_2"]))
            self.Servo[3].setText(str(self.jsondata["Servo_3"]))
            self.Servo[4].setText(str(self.jsondata["Servo_4"]))
            self.Servo[5].setText(str(self.jsondata["Servo_5"]))
            self.Servo[6].setText(str(self.jsondata["Servo_6"]))
            self.Servo[7].setText(str(self.jsondata["Servo_7"]))
            self.Servo[8].setText(str(self.jsondata["Servo_8"]))
            self.Servo[9].setText(str(self.jsondata["Servo_9"]))
            self.Servo[10].setText(str(self.jsondata["Servo_10"]))
            self.Servo[11].setText(str(self.jsondata["Servo_11"]))
            self.Servo[12].setText(str(self.jsondata["Servo_12"]))
            self.Servo[13].setText(str(self.jsondata["Servo_13"]))
            self.Servo[14].setText(str(self.jsondata["Servo_14"]))
            self.Servo[15].setText(str(self.jsondata["Servo_15"]))
            self.Servo[16].setText(str(self.jsondata["Servo_16"]))
            self.Servo[17].setText(str(self.jsondata["Servo_17"]))
            self.Servo[18].setText(str(self.jsondata["Servo_18"]))
            self.Servo[19].setText(str(self.jsondata["Servo_19"]))

    def checkManual(self):
        if self.Manual_mode.checkState() == 0:
            self.RightButton.setEnabled(False)
            self.LeftButton.setEnabled(False)
            self.FrontButton.setEnabled(False)
            self.BackButton.setEnabled(False)
            self.RotateLeftButton.setEnabled(False)
            self.RotateRightButton.setEnabled(False)
            self.ProneButton.setEnabled(False)
        elif self.Manual_mode.checkState() == 2 and  self.serialCom_ is not None:
            self.RightButton.setEnabled(True)
            self.LeftButton.setEnabled(True)
            self.FrontButton.setEnabled(True)
            self.BackButton.setEnabled(True)
            self.RotateLeftButton.setEnabled(True)
            self.RotateRightButton.setEnabled(True)
            self.ProneButton.setEnabled(True)

        if self.serialCom_ is None:
            self.StartButton.setEnabled(False)
            self.StopButton.setEnabled(False)
        else:
            self.StartButton.setEnabled(True)
            self.StopButton.setEnabled(True)

    def connectSerialComboBox(self):
        self.comboBoxPort.activated.connect(lambda: self.startSerialCom(self.comboBoxPort.currentText()))
    
    def startSerialCom(self,portName):

        self.serialCom_ = SerialProtocol(portName)
        self.connectSerialPortRead()

    def connectSerialPortRead(self):
        self.serialCom_.newMessage.connect(self.receiveFromSerial)

    def receiveFromSerial(self,msg):

        for word in msg:
            if word != '}':
                self.msgBuffer_ += word
            if word == '}':
                self.msgBuffer_ += '}\n'
                break
        # print(msg)
        # print(self.msgBuffer_)

        if not self.msgBuffer_.startswith("{"):
            self.msgBuffer_ = ""

        if self.msgBuffer_.endswith('\n') and self.msgBuffer_.startswith("{"):
            self.jsondata = json.loads(self.msgBuffer_)
            jsonBrowserText = json.loads(self.msgBuffer_)

            for key in list(self.jsondata):
                if key == "Servo_1" or key=="Servo_2" or key=="Servo_3" or key=="Servo_4" or key=="Servo_5" or key=="Servo_6" or key=="Servo_7" or key=="Servo_8" or key=="Servo_9"or key=="Servo_10" or key=="Servo_11" or key=="Servo_12" or key=="Servo_13" or key=="Servo_14" or key=="Servo_15" or key=="Servo_16" or key=="Servo_17" or key=="Servo_18" or key=="Servo_19" :
                    del jsonBrowserText[key]
            
            jsondataString = json.dumps(jsonBrowserText,indent=2)

            self.Json_Browser.setText(jsondataString)
     
            for key in self.jsondata.keys():
                if self.JsonKey.text() == key:
                    self.series_.append(self.jsondata['time'], float(self.jsondata[key]))
                    self.graph.removeSeries(self.series_)
                    self.graph.addSeries(self.series_)
                    self.graph.legend().hide()
                    self.graph.setTitle(str(key))
                    self.graph.createDefaultAxes()
                    self.graphView.setChart(self.graph)
                
            if self.JsonKey.text() not in self.jsondata.keys():
                self.series_.clear()
            
            self.msgBuffer_ = ""


    def cleanUp(self):

        print("Exiting program...")
        self.serialCom_.serialQuit()
        self.updateTimer.stop()
        self.Cam.capwebcam.stop()
        print("END")

class SerialProtocol(QComboBox):
    newMessage = pyqtSignal(str)

    def __init__(self,portName):
        super().__init__()
        self.serial_ = QSerialPort(self)
        self.serial_.setPortName(portName)


        if self.serial_.open(QIODevice.ReadWrite):
            self.serial_.setBaudRate(BAUD_RATE)
            while self.serial_.waitForReadyRead(100):
                self.serial_.clear()
            self.serial_.readyRead.connect(self.readReceivedMsg)
            print("Serial Ready")
            self.serial_.clear()
            
        else:
            raise IOError("Cannot connect to device on port {}".format(portName))


    def sendMessage(self,msg):
        if self.serial_.isOpen:
            self.serial_.write(msg.encode())

    def readReceivedMsg(self):

        self.newMessage.emit(str(self.serial_.readAll(), encoding='utf-8', errors='ignore'))

    
    def serialQuit(self):
        if self.serial_ != None:
            del self.serial_

class Map(QGraphicsView):
    def __init__(self,parent):
        super().__init__(parent)
        self.updateTimer = QTimer()

        # Create scene
        self.scene = QGraphicsScene()
        self.scene.setSceneRect(0, 0, 320,320)
        self.scene.setBackgroundBrush(QBrush(QImage(os.path.join(paths['DISPLAY_IMAGE_PATH'],"Arena.png"))))
        self.setScene(self.scene)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.hexapod = Robot()
        self.hexapod.setFlag(QGraphicsItem.ItemIsFocusable)
        self.hexapod.setFocus()
        self.scene.addItem(self.hexapod)

        self.show

        #CREATE TARGET OBJECT WHEN DETECTED

    def auto_map_movement(self,jsondata):
        if jsondata is not None:
            self.hexapod.move(jsondata["cur_x"],jsondata["cur_y"])
            self.hexapod.rotate(jsondata["cur_angle"])

    def manual_map_movement(self,key,angle):

        if key == "RIGHT":
            self.hexapod.move(MANUAL_SIDE_MOVEMENT,0)
        if key == "LEFT":
            self.hexapod.move(-MANUAL_SIDE_MOVEMENT,0)
        if key == "FRONT":
            self.hexapod.move(0,-MANUAL_VERTICAL_MOVEMENT)
        if key == "BACK":
            self.hexapod.move(0,MANUAL_VERTICAL_MOVEMENT)
        if key == "RLEFT":
            self.hexapod.rotate(-angle)

        if key == "RRIGHT":
            self.hexapod.rotate(angle)

class Robot(QGraphicsPixmapItem):
    def __init__(self):
        super().__init__()
        pixmap = QPixmap(os.path.join(paths['DISPLAY_IMAGE_PATH'],"hexapod.png"))
        pixmap_resized = pixmap.scaled(66,73,Qt.KeepAspectRatio)
        self.setPixmap(pixmap_resized)
        self.setPos(130, 120)
        self.setTransformOriginPoint(33,51.5)#image is 66 by 103 pixel
        self.angle = 0

    def move(self,xpos,ypos):

        if self.angle == 0:
            self.setPos(xpos+self.x(),ypos+self.y())
        elif self.angle > 0:
            x = xpos*cos(math.radians(self.angle))-ypos*sin(math.radians(self.angle))
            y = xpos*sin(math.radians(self.angle))+ypos*cos(math.radians(self.angle))
            self.setPos(self.x()+ x.real,self.y()+y.real)
        elif self.angle < 0:
            x = xpos*cos(-math.radians(self.angle))+ypos*sin(-math.radians(self.angle))
            y = -xpos*sin(-math.radians(self.angle))+ypos*cos(-math.radians(self.angle))
            self.setPos(self.x()+ x.real,self.y()+y.real)

    def rotate(self,angle):
        self.angle += angle
        self.setRotation(self.angle)

    # def keyPressEvent(self, event:QKeyEvent):
    #     # print('lol')

    #     if event.key() == Qt.Key_Left:
    #         if self.pos().x()>0:
    #             self.setPos(self.x()-MANUAL_SIDE_MOVEMENT, self.y())
    #     elif event.key() == Qt.Key_Right:
    #         if self.pos().x() < 780:
    #             self.setPos(self.x()+MANUAL_SIDE_MOVEMENT, self.y())

    #     return super().keyPressEvent(event)


class Target(QGraphicsPixmapItem):
    pass

class VideoTracking(QLabel):
    def __init__(self,parent):
        super().__init__(parent)

        self.capwebcam = VideoStream(src=0,usePiCamera=True).start()
        self.camTimer = QTimer()
        
        self.new_width = 320
        self.font = cv2.FONT_HERSHEY_COMPLEX

        self.labels = self.load_labels()
        self.interpreter = Interpreter(model_path=os.path.join(paths['TFLITE_PATH'],"detect.tflite"))
        self.interpreter.allocate_tensors()
        self.input_height= self.interpreter.get_input_details()[0]['shape'][1]
        self.input_width= self.interpreter.get_input_details()[0]['shape'][2]

        time.sleep(1)
        self.camTimer.timeout.connect(self.OnPeriodicEvent)
        self.camTimer.start(CAM_UPDATE_RATE)

    def OnPeriodicEvent(self):
        frame = self.capwebcam.read()
        self.vision(frame)

    def load_labels(self,path=os.path.join(paths['TFLITE_PATH'],"labelmap.txt")):

        with open(path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            labels = {}
            for row_number, content in enumerate(lines):
                pair = re.split(r'[:\s]+', content.strip(), maxsplit=1)
                if len(pair) == 2 and pair[0].strip().isdigit():
                   labels[int(pair[0])] = pair[1].strip()
                else:
                    labels[row_number] = pair[0].strip()
        
        return labels
    
    def set_input_tensor(self,interpreter, image):

        tensor_index = interpreter.get_input_details()[0]['index']
        input_tensor = interpreter.tensor(tensor_index)()[0]
        input_tensor[:, :] = np.expand_dims((image-255)/255, axis=0)

    
    def get_output_tensor(self,interpreter, index):

        output_details = interpreter.get_output_details()[index]
        tensor = np.squeeze(interpreter.get_tensor(output_details['index']))
        return tensor
    
    def detect_objects(self,interpreter, image, threshold):
        self.set_input_tensor(interpreter, image)
        interpreter.invoke()
        # Get all output details
        boxes = self.get_output_tensor(interpreter, 1)
        classes = self.get_output_tensor(interpreter, 3)
        scores = self.get_output_tensor(interpreter, 0)

        results = []
        for i in range(len(scores)):
            if scores[i] >= threshold:
                result = {
                    'bounding_box': boxes[i],
                    'class_id': classes[i],
                    'score': scores[i]
                }
                results.append(result)
        return results
        

    def vision(self,frame):

        img = cv2.resize(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB), (320,320))

        res = self.detect_objects(self.interpreter, img, 0.5)
        # print(res)

        for result in res:
            ymin, xmin, ymax, xmax = result['bounding_box']
            xmin = int(max(1,xmin * 320))
            xmax = int(min(320, xmax * 320))
            ymin = int(max(1, ymin * 200))
            ymax = int(min(200, ymax * 250))

            label = '%s: %d%%' % (self.labels[int(result['class_id'])], int(result['score']*100))
            
            cv2.rectangle(frame,(xmin, ymin),(xmax, ymax),(0,255,0),3)
            cv2.putText(frame,label,(xmin, ymin-7), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(255,255,255),2,cv2.LINE_AA) 


        imageframe = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        imageframe = QImage(imageframe,imageframe.shape[1],imageframe.shape[0],imageframe.strides[0],QImage.Format_RGB888)

        self.setPixmap(QPixmap.fromImage(imageframe))



if __name__ == "__main__":

    app = QApplication(sys.argv)
    
    ui = Ui_MainWindow()
    ui.show()

    app.aboutToQuit.connect(ui.cleanUp)

    sys.exit(app.exec_())
