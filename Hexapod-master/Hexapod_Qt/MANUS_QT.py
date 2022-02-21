
from cmath import cos, pi, sin
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtWidgets import (QApplication, QGridLayout, QMainWindow, QLabel,
                             QLineEdit, QPushButton, QSpinBox, QWidget,
                              QFrame, QComboBox, QGraphicsView, QGraphicsItem, QGraphicsScene,
                               QGraphicsPixmapItem, QPlainTextEdit, QDoubleSpinBox, QTextBrowser, QCheckBox)
from PyQt5.QtCore import QRect,QIODevice, QCoreApplication, pyqtSignal, Qt, QTimer
from PyQt5.QtMultimedia import QMediaPlayer
from PyQt5.QtGui import QPixmap, QFont, QBrush, QImage, QKeyEvent
from PyQt5.QtChart import QChart,QChartView,QLineSeries
import sys, os, json, math
# import cv2

SCRIPT_DIR = os.path.dirname(__file__)+os.path.sep
SCRIPT_IMAGES = SCRIPT_DIR + "Images"+os.path.sep
BAUD_RATE = 115200
UI_UPDATE_RATE = 1000# Ms
NUM_SERVOS = 19
MANUAL_SIDE_MOVEMENT  = 10 #pixels
MANUAL_VERTICAL_MOVEMENT = 10 #pixels
MANUAL_ROTATION_ANGLE = 20


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
        self.RotateRightButton = QPushButton(self.widget)
        self.LeftButton = QPushButton(self.widget)
        self.RotateLeftButton = QPushButton(self.widget)
        self.FrontButton = QPushButton(self.widget)
        self.BackButton = QPushButton(self.widget)
        self.ProneButton = QPushButton(self.widget)

        self.Port_label = QLabel(self.widget)
        self.comboBoxPort = QComboBox(self.widget)

        self.Map_label = QLabel(self.widget)
        self.MapView = Map(self.widget)

        self.Graph_label = QLabel(self.widget)
        self.JsonKey = QLineEdit(self.widget)
        self.graph = QChart()
        self.graphView = QChartView(self.widget)
        self.series_ = QLineSeries()

        self.Donnees_label = QLabel(self.widget)
        self.pulseButton = QPushButton(self.widget)

        self.StartButton = QPushButton(self.widget)
        self.StopButton = QPushButton(self.widget)

        self.Distance_label = QLabel(self.widget)
        self.DistanceBox = QSpinBox(self.widget)

        self.Cam_label = QLabel(self.widget)
        self.Cam = QLabel(self.widget)
        self.CamDistance_label = QLabel(self.widget)
        self.CamDistanceText = QPlainTextEdit(self.widget)

        self.PWR_label = QLabel(self.widget)
        self.PWMBox = QDoubleSpinBox(self.widget)

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

        self.Hexapod_Pic.setGeometry(QRect(285, 410, 321, 361))
        self.Hexapod_Pic.setPixmap(QPixmap(SCRIPT_IMAGES+"hexapod.jpg"))
        self.Hexapod_Pic.setScaledContents(True)

        self.Servo[1].setGeometry(QRect(400, 550, 41, 25))
        self.Servo[2].setGeometry(QRect(350, 540, 41, 25))
        self.Servo[3].setGeometry(QRect(310, 520, 41, 25))
        self.Servo[4].setGeometry(QRect(450, 550, 41, 25))
        self.Servo[5].setGeometry(QRect(490, 530, 41, 25))
        self.Servo[6].setGeometry(QRect(530, 510, 41, 25))
        self.Servo[7].setGeometry(QRect(400, 580, 41, 25))
        self.Servo[8].setGeometry(QRect(350, 580, 41, 25))
        self.Servo[9].setGeometry(QRect(300, 580, 41, 25))       
        self.Servo[10].setGeometry(QRect(450, 580, 41, 25))
        self.Servo[11].setGeometry(QRect(500, 580, 41, 25))
        self.Servo[12].setGeometry(QRect(550, 580, 41, 25))
        self.Servo[13].setGeometry(QRect(400, 620, 41, 25))
        self.Servo[14].setGeometry(QRect(350, 630, 41, 25))
        self.Servo[15].setGeometry(QRect(300, 640, 41, 25))        
        self.Servo[16].setGeometry(QRect(450, 620, 41, 25))
        self.Servo[17].setGeometry(QRect(490, 640, 41, 25))
        self.Servo[18].setGeometry(QRect(530, 650, 41, 25))
        self.Servo[19].setGeometry(QRect(420, 520, 41, 25))

        for idx in range(1,NUM_SERVOS+1):
            self.Servo[idx].setReadOnly(True)        

        self.RightButton.setGeometry(QRect(860, 510, 81, 61))
        self.RotateRightButton.setGeometry(QRect(860, 430, 81, 61))
        self.LeftButton.setGeometry(QRect(660, 510, 81, 61))
        self.RotateLeftButton.setGeometry(QRect(660, 430, 81, 61))
        self.FrontButton.setGeometry(QRect(760, 430, 81, 61))
        self.BackButton.setGeometry(QRect(760, 590, 81, 61))
        self.ProneButton.setGeometry(QRect(760, 510, 81, 61))
        
        self.Port_label.setGeometry(QRect(20, 17, 94, 22))
        self.comboBoxPort.setGeometry(QRect(121, 17, 124, 22))

        self.Map_label.setGeometry(QRect(0, 440, 218, 22))
        self.MapView.setEnabled(True)
        self.MapView.setGeometry(QRect(0, 460, 261, 261))


        self.Graph_label.setGeometry(QRect(620, 10, 218, 22))
        self.JsonKey.setGeometry(QRect(810, 250, 101, 31))
        self.graphView.setGeometry(QRect(620, 30, 341, 211))

        self.Donnees_label.setGeometry(QRect(660, 250, 121, 28))
        self.pulseButton.setGeometry(QRect(20, 110, 225, 28))

        self.StartButton.setGeometry(QRect(820, 330, 131, 31))
        self.StopButton.setGeometry(QRect(640, 330, 131, 31))
        font = QFont()
        font.setBold(True)
        self.StopButton.setFont(font)

        self.Distance_label.setGeometry(QRect(20, 80, 94, 22))
        self.DistanceBox.setGeometry(QRect(121, 80, 124, 22))
        self.DistanceBox.setMinimum(0)
        self.DistanceBox.setMaximum(5000)
        self.DistanceBox.setSingleStep(1)

        self.Cam_label.setGeometry(QRect(0, 150, 218, 22))
        self.Cam.setGeometry(QRect(10, 170, 261, 261))
        self.CamDistance_label.setGeometry(QRect(280, 230, 218, 22))
        self.CamDistanceText.setGeometry(QRect(280, 250, 311, 151))

        self.PWR_label.setGeometry(QRect(20, 50, 94, 22))
        self.PWMBox.setGeometry(QRect(121, 50, 124, 22))
        self.PWMBox.setMinimum(-1.0)
        self.PWMBox.setMaximum(1.0)
        self.PWMBox.setSingleStep(0.1)

        self.Json_label.setGeometry(QRect(270, 10, 218, 22))
        self.Json_Browser.setGeometry(QRect(280, 30, 311, 201))
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
        self.Distance_label.setText(_translate("MainWindow", "Distance(m)"))
        self.Map_label.setText(_translate("MainWindow", "Map:"))
        self.BackButton.setText(_translate("MainWindow", "BACK"))
        self.ProneButton.setText(_translate("MainWindow", "PRONE"))
        self.RightButton.setText(_translate("MainWindow", "RIGHT"))
        self.Cam_label.setText(_translate("MainWindow", "Cam:"))
        self.StartButton.setText(_translate("MainWindow", "Start"))
        self.Port_label.setText(_translate("MainWindow", "Port:"))
        self.LeftButton.setText(_translate("MainWindow", "LEFT"))
        self.pulseButton.setText(_translate("MainWindow", "Commande de pulse"))
        self.Donnees_label.setText(_translate("MainWindow", "Donnees brutes:"))
        self.StopButton.setText(_translate("MainWindow", "Stop"))
        self.PWR_label.setText(_translate("MainWindow", "Tension [-1,1]"))
        self.Json_label.setText(_translate("MainWindow", "Messages Json de l\'Arduino:"))
        self.FrontButton.setText(_translate("MainWindow", "FRONT"))
        self.Graph_label.setText(_translate("MainWindow", "Graphique:"))
        self.CamDistance_label.setText(_translate("MainWindow", "Distance Cam:"))
        self.RotateLeftButton.setText(_translate("MainWindow", "ROTATE L"))
        self.RotateRightButton.setText(_translate("MainWindow", "ROTATE R"))
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

        self.StartButton.clicked.connect(lambda: self.serialCom_.sendMessage("START"))
        self.StopButton.clicked.connect(lambda: self.serialCom_.sendMessage("STOP"))

        # self.RightButton.clicked.connect(lambda: self.serialCom_.sendMessage("RIGHT"))
        self.RightButton.clicked.connect(lambda: self.MapView.manual_map_movement("RIGHT"))

        # self.LeftButton.clicked.connect(lambda: self.serialCom_.sendMessage("LEFT"))
        self.LeftButton.clicked.connect(lambda: self.MapView.manual_map_movement("LEFT"))

        # self.FrontButton.clicked.connect(lambda: self.serialCom_.sendMessage("FRONT"))
        self.FrontButton.clicked.connect(lambda: self.MapView.manual_map_movement("FRONT"))

        # self.BackButton.clicked.connect(lambda: self.serialCom_.sendMessage("BACK"))
        self.BackButton.clicked.connect(lambda: self.MapView.manual_map_movement("BACK"))

        # self.RotateLeftButton.clicked.connect(lambda: self.serialCom_.sendMessage("RLEFT"))
        self.RotateLeftButton.clicked.connect(lambda: self.MapView.manual_map_movement("RLEFT"))

        # self.RotateRightButton.clicked.connect(lambda: self.serialCom_.sendMessage("RRIGHT"))
        self.RotateRightButton.clicked.connect(lambda: self.MapView.manual_map_movement("RRIGHT"))

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
        if self.counter == 1:
            self.msgBuffer_ += msg
        # print(msg)

        if self.msgBuffer_.endswith("\n") and self.msgBuffer_.startswith("{"):
            self.jsondata = json.loads(self.msgBuffer_)
            jsondataString = json.dumps(self.jsondata,indent=2)
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
        self.counter = 1

    def cleanUp(self):

        print("Exiting program...")
        self.serialCom_.serialQuit()
        self.updateTimer.stop()
        print("END")

class SerialProtocol(QComboBox):
    newMessage = pyqtSignal(str)

    def __init__(self,portName):
        super().__init__()
        self.serial_ = QSerialPort(self)
        self.serial_.setPortName(portName)

        if self.serial_.open(QIODevice.ReadWrite):
            self.serial_.setBaudRate(BAUD_RATE)
            self.serial_.readyRead.connect(self.readReceivedMsg)
            self.clear()

            print("Serial Ready")
        else:
            raise IOError("Cannot connect to device on port {}".format(portName))


    def sendMessage(self,msg):
        if self.serial_.isOpen:
            pass
            # self.serial_.write(msg.decode('utf-8', 'ignore'))

    def readReceivedMsg(self):
        self.newMessage.emit(str(self.serial_.readAll(), encoding='utf-8', errors='ignore'))
    
    def serialQuit(self):
        if self.serial_ != None:
            del self.serial_

class Robot(QGraphicsPixmapItem):
    def __init__(self):
        super().__init__()
        self.setPixmap(QPixmap(SCRIPT_IMAGES+"hexapod_map.png"))
        self.setPos(120, 120)
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

    def keyPressEvent(self, event:QKeyEvent):
        print('lol')

        if event.key() == Qt.Key_Left:
            if self.pos().x()>0:
                self.setPos(self.x()-MANUAL_SIDE_MOVEMENT, self.y())
        elif event.key() == Qt.Key_Right:
            if self.pos().x() < 780:
                self.setPos(self.x()+MANUAL_SIDE_MOVEMENT, self.y())

        return super().keyPressEvent(event)

class Map(QGraphicsView):
    def __init__(self,parent):
        super().__init__(parent)
        self.updateTimer = QTimer()

        # Create scene
        self.scene = QGraphicsScene()
        self.scene.setSceneRect(0, 0, 320,320)
        self.scene.setBackgroundBrush(QBrush(QImage(SCRIPT_IMAGES+"Arena.png")))
        self.setScene(self.scene)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.hexapod = Robot()
        self.hexapod.setFlag(QGraphicsItem.ItemIsFocusable)
        self.hexapod.setFocus()
        self.scene.addItem(self.hexapod)

        self.show

        #CREATE OBJECT WHEN DETECTED

    def auto_map_movement(self,jsondata):
        if jsondata is not None:
            self.hexapod.move(jsondata["cur_x"],jsondata["cur_y"])
            self.hexapod.rotate(jsondata["cur_angle"])

    def manual_map_movement(self,key):

        if key == "RIGHT":
            self.hexapod.move(MANUAL_SIDE_MOVEMENT,0)
        if key == "LEFT":
            self.hexapod.move(-MANUAL_SIDE_MOVEMENT,0)
        if key == "FRONT":
            self.hexapod.move(0,-MANUAL_VERTICAL_MOVEMENT)
        if key == "BACK":
            self.hexapod.move(0,MANUAL_VERTICAL_MOVEMENT)
        if key == "RLEFT":
            self.hexapod.rotate(-MANUAL_ROTATION_ANGLE)

        if key == "RRIGHT":
            self.hexapod.rotate(MANUAL_ROTATION_ANGLE)

class Target(QGraphicsPixmapItem):
    pass


if __name__ == "__main__":

    app = QApplication(sys.argv)
    
    ui = Ui_MainWindow()
    ui.show()

    app.aboutToQuit.connect(ui.cleanUp)

    sys.exit(app.exec_())
