from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtWidgets import (QApplication, QGridLayout, QMainWindow, QLabel,
                             QLineEdit, QPushButton, QSpinBox, QWidget,
                              QFrame, QComboBox, QGraphicsView, QPlainTextEdit, QDoubleSpinBox, QTextBrowser)
from PyQt5.QtCore import QRect,QIODevice, QCoreApplication, pyqtSignal, Qt, QTimer
from PyQt5.QtGui import QPixmap, QFont
from PyQt5.QtChart import QChartView
import sys, os, json
# import cv2

SCRIPT_DIR = os.path.dirname(__file__)+os.path.sep
BAUD_RATE = 115200
SERIAL_UPDATE_RATE = 1000 # Ms

class Ui_MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.updateTimer = QTimer()
        self.msgBuffer_ = ""

        self.centralWidget = QWidget(self)
        self.gridLayout = QGridLayout(self.centralWidget)

        self.widget = QWidget(self.centralWidget)
        self.line = QFrame(self.widget)
        self.Hexapod_Pic = QLabel(self.widget)

        self.Servo_1 = QLineEdit(self.widget)
        self.Servo_2 = QLineEdit(self.widget)
        self.Servo_3 = QLineEdit(self.widget)
        self.Servo_4 = QLineEdit(self.widget)
        self.Servo_5 = QLineEdit(self.widget)
        self.Servo_6 = QLineEdit(self.widget)
        self.Servo_7 = QLineEdit(self.widget)
        self.Servo_8 = QLineEdit(self.widget)
        self.Servo_9 = QLineEdit(self.widget)
        self.Servo_10 = QLineEdit(self.widget)
        self.Servo_11 = QLineEdit(self.widget)
        self.Servo_12 = QLineEdit(self.widget)
        self.Servo_13 = QLineEdit(self.widget)
        self.Servo_14 = QLineEdit(self.widget)
        self.Servo_15 = QLineEdit(self.widget)
        self.Servo_16 = QLineEdit(self.widget)
        self.Servo_17 = QLineEdit(self.widget)
        self.Servo_18 = QLineEdit(self.widget)
        self.Servo_19 = QLineEdit(self.widget)

        self.RightButton = QPushButton(self.widget)
        self.LeftButton = QPushButton(self.widget)
        self.FrontButton = QPushButton(self.widget)
        self.BackButton = QPushButton(self.widget)
        self.ProneButton = QPushButton(self.widget)

        self.Port_label = QLabel(self.widget)
        self.comboBoxPort = QComboBox(self.widget)

        self.Map_label = QLabel(self.widget)
        self.Map = QGraphicsView(self.widget)

        self.Graph_label = QLabel(self.widget)
        self.JsonKey = QLineEdit(self.widget)
        self.graph = QChartView(self.widget)

        self.Donnees_label = QLabel(self.widget)
        self.pulseButton = QPushButton(self.widget)

        self.StartButton = QPushButton(self.widget)
        self.StopButton = QPushButton(self.widget)

        self.Distance_label = QLabel(self.widget)
        self.DistanceBox = QSpinBox(self.widget)

        self.Cam_label = QLabel(self.widget)
        self.Cam = QLabel(self.widget)
        self.CamTextEdit = QPlainTextEdit(self.widget)

        self.PWR_label = QLabel(self.widget)
        self.PWMBox = QDoubleSpinBox(self.widget)

        self.Json_label = QLabel(self.widget)
        self.Json_Browser = QTextBrowser(self.widget)

        self.setupUi()

    def setupUi(self):
        self.setObjectName("MainWindow")
        self.resize(983, 790)
        # MainWindow.setAcceptDrops(False)

        self.centralWidget.setObjectName("centralWidget")
        self.gridLayout.setContentsMargins(9, 9, 11, 9)
        self.gridLayout.setSpacing(6)
        self.gridLayout.setObjectName("gridLayout")

        self.widget.setObjectName("widget")

        self.line.setGeometry(QRect(600, 20, 20, 401))
        self.line.setFrameShape(QFrame.VLine)
        self.line.setFrameShadow(QFrame.Sunken)
        self.line.setObjectName("line")

        self.Hexapod_Pic.setGeometry(QRect(290, 410, 321, 361))
        self.Hexapod_Pic.setPixmap(QPixmap(SCRIPT_DIR+"hexapod.jpg"))
        self.Hexapod_Pic.setScaledContents(True)
        self.Hexapod_Pic.setObjectName("Hexapod_Pic")

        self.Servo_1.setGeometry(QRect(310, 520, 41, 25))
        self.Servo_1.setObjectName("Servo_1")
        self.Servo_2.setGeometry(QRect(350, 540, 41, 25))
        self.Servo_2.setObjectName("Servo_2")
        self.Servo_3.setGeometry(QRect(400, 550, 41, 25))
        self.Servo_3.setObjectName("Servo_3")
        self.Servo_4.setGeometry(QRect(450, 550, 41, 25))
        self.Servo_4.setObjectName("Servo_4")
        self.Servo_5.setGeometry(QRect(490, 530, 41, 25))
        self.Servo_5.setObjectName("Servo_5")
        self.Servo_6.setGeometry(QRect(530, 510, 41, 25))
        self.Servo_6.setObjectName("Servo_6")
        self.Servo_7.setGeometry(QRect(300, 580, 41, 25))
        self.Servo_7.setObjectName("Servo_7")
        self.Servo_8.setGeometry(QRect(350, 580, 41, 25))
        self.Servo_8.setObjectName("Servo_8")
        self.Servo_9.setGeometry(QRect(400, 580, 41, 25))
        self.Servo_9.setObjectName("Servo_9")
        self.Servo_10.setGeometry(QRect(450, 580, 41, 25))
        self.Servo_10.setObjectName("Servo_10")
        self.Servo_11.setGeometry(QRect(500, 580, 41, 25))
        self.Servo_11.setObjectName("Servo_11")
        self.Servo_12.setGeometry(QRect(550, 580, 41, 25))
        self.Servo_12.setObjectName("Servo_12")
        self.Servo_13.setGeometry(QRect(300, 640, 41, 25))
        self.Servo_13.setObjectName("Servo_13")
        self.Servo_14.setGeometry(QRect(350, 630, 41, 25))
        self.Servo_14.setObjectName("Servo_14")
        self.Servo_15.setGeometry(QRect(400, 620, 41, 25))
        self.Servo_15.setObjectName("Servo_15")
        self.Servo_16.setGeometry(QRect(450, 620, 41, 25))
        self.Servo_16.setObjectName("Servo_16")
        self.Servo_17.setGeometry(QRect(490, 640, 41, 25))
        self.Servo_17.setObjectName("Servo_17")
        self.Servo_18.setGeometry(QRect(530, 650, 41, 25))
        self.Servo_18.setObjectName("Servo_18")
        self.Servo_19.setGeometry(QRect(420, 520, 41, 25))
        self.Servo_19.setObjectName("Servo_19")

        self.RightButton.setGeometry(QRect(860, 510, 81, 61))
        self.RightButton.setObjectName("RightButton")
        self.LeftButton.setGeometry(QRect(660, 510, 81, 61))
        self.LeftButton.setObjectName("LeftButton")
        self.FrontButton.setGeometry(QRect(760, 430, 81, 61))
        self.FrontButton.setObjectName("FrontButton")
        self.BackButton.setGeometry(QRect(760, 590, 81, 61))
        self.BackButton.setObjectName("BackButton")
        self.ProneButton.setGeometry(QRect(760, 510, 81, 61))
        self.ProneButton.setObjectName("ProneButton")
        
        self.Port_label.setGeometry(QRect(20, 17, 94, 22))
        self.Port_label.setObjectName("Port_label")
        self.comboBoxPort.setGeometry(QRect(121, 17, 124, 22))
        self.comboBoxPort.setObjectName("comboBoxPort")

        self.Map_label.setGeometry(QRect(290, 170, 218, 22))
        self.Map_label.setObjectName("Map_label")
        self.Map.setEnabled(True)
        self.Map.setGeometry(QRect(290, 190, 311, 221))
        self.Map.setObjectName("Map")

        self.Graph_label.setGeometry(QRect(620, 10, 218, 22))
        self.Graph_label.setObjectName("Graph_label")
        self.JsonKey.setGeometry(QRect(810, 250, 101, 31))
        self.JsonKey.setObjectName("JsonKey")
        self.graph.setGeometry(QRect(620, 30, 341, 211))
        self.graph.setObjectName("graph")

        self.Donnees_label.setGeometry(QRect(660, 250, 121, 28))
        self.Donnees_label.setObjectName("Donnees_label")
        self.pulseButton.setGeometry(QRect(20, 110, 225, 28))
        self.pulseButton.setObjectName("pulseButton")

        self.StartButton.setGeometry(QRect(820, 330, 131, 31))
        self.StartButton.setObjectName("StartButton")
        self.StopButton.setGeometry(QRect(640, 330, 131, 31))
        font = QFont()
        font.setFamily("Sans Serif")
        font.setBold(True)
        self.StopButton.setFont(font)
        self.StopButton.setObjectName("Stop")

        self.Distance_label.setGeometry(QRect(20, 80, 94, 22))
        self.Distance_label.setObjectName("Distance_label")
        self.DistanceBox.setGeometry(QRect(121, 80, 124, 22))
        self.DistanceBox.setMinimum(0)
        self.DistanceBox.setMaximum(5000)
        self.DistanceBox.setSingleStep(1)
        self.DistanceBox.setObjectName("DistanceBox")

        self.Cam_label.setGeometry(QRect(10, 170, 218, 22))
        self.Cam_label.setObjectName("Cam_label")
        self.Cam.setGeometry(QRect(10, 200, 271, 201))
        # self.Cam.setAutoFillBackground(True)
        self.Cam.setObjectName("Cam")
        self.CamTextEdit.setGeometry(QRect(10, 410, 281, 171))
        self.CamTextEdit.setObjectName("plainTextEdit")

        self.PWR_label.setGeometry(QRect(20, 50, 94, 22))
        self.PWR_label.setObjectName("PWR_label")
        self.PWMBox.setGeometry(QRect(121, 50, 124, 22))
        self.PWMBox.setMinimum(-1.0)
        self.PWMBox.setMaximum(1.0)
        self.PWMBox.setSingleStep(0.1)
        self.PWMBox.setObjectName("PWMBox")

        self.Json_label.setGeometry(QRect(270, 10, 218, 22))
        self.Json_label.setObjectName("Json_label")
        self.Json_Browser.setGeometry(QRect(270, 30, 331, 131))
        font = QFont()
        font.setPointSize(9)
        self.Json_Browser.setFont(font)
        self.Json_Browser.setObjectName("Json_Browser")

        self.gridLayout.addWidget(self.widget, 0, 0, 1, 1)
        self.setCentralWidget(self.centralWidget)
        self.retranslateUi()
        

        self.portCensus()
        self.connectUpdateTimer(SERIAL_UPDATE_RATE)
        self.connectSerialComboBox()

        # QMetaObject.connectSlotsByName(Self)

    def retranslateUi(self):
        _translate = QCoreApplication.translate
        self.setWindowTitle(_translate("MainWindow", "MANUS"))
        self.JsonKey.setText(_translate("MainWindow", "potVex"))
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

    def portCensus(self):
        self.comboBoxPort.clear
        available_ports = QSerialPortInfo.availablePorts()

        for port in available_ports:
            self.port_name = port.portName()
            self.comboBoxPort.addItem(self.port_name)
  
    def connectUpdateTimer(self, updateTime):
        self.updateTimer.timeout.connect(self.OnPeriodicEvent)
        self.updateTimer.start(updateTime)
    
    def OnPeriodicEvent(self):
        print('*')

    def connectSerialComboBox(self):
        self.comboBoxPort.activated.connect(lambda: self.startSerialCom(self.comboBoxPort.currentText()))
    
    def startSerialCom(self,portName):

        self.serialCom_ = SerialProtocol(portName)
        self.connectSerialPortRead()
        print(self.connectSerialPortRead())

    def connectSerialPortRead(self):
        self.serialCom_.newMessage.connect(self.receiveFromSerial)

    def receiveFromSerial(self,msg):
        
        self.msgBuffer_ += msg
        print(self.msgBuffer_)
        # if self.msgBuffer_.endswith('\n'):
            # print(self.msgBuffer_)
        data = json.load(self.msgBuffer_())

    #     // Passage ASCII vers structure Json
        # self.jsonResponse.fromJson(self.msgBuffer_.encode('utf-8'))


    #     // Analyse du message Json
        # if not self.jsonResponse.isEmpty():
 
    #         QJsonObject jsonObj = jsonResponse.object();
    #         QString buff = jsonResponse.toJson(QJsonDocument::Indented);

    #         // Affichage des messages Json
    #         ui->textBrowser->setText(buff.mid(2,buff.length()-4));
    #         //ui->Etat->setText(jsonObj["Etat"].toString());

    #         positionVoiture = covertisseurMagique*jsonObj["cur_pos"].toDouble();
    #         anglePendule = -1*(jsonObj["cur_angle"].toDouble()+45);
    #         angleSapin = -1*(jsonObj["cur_angle"].toDouble());
    #         sapinLacher = jsonObj["sapin_lacher"].toBool();
    #         etat = jsonObj["Etat"].toDouble();
    #         casZero     = jsonObj["casZero"].toBool();
    #         vitesse_angulaire = jsonObj["vitesse_angulaire"].toDouble();
    #         son = jsonObj["son"].toDouble();

    #         if(game_on == true)
    #         {
    #             this->moveMario();
    #         }

    #         if(etat == 100.0)
    #         {
    #             game_on = true;
    #         }

    #         // Affichage des donnees dans le graph
    #         if(jsonObj.contains(JsonKey_)){
    #             double time = jsonObj["time"].toDouble();
    #             series_.append(time, jsonObj[JsonKey_].toDouble());
    #             // Mise en forme du graphique (non optimal)
    #             chart_.removeSeries(&series_);
    #             chart_.addSeries(&series_);
    #             chart_.createDefaultAxes();
    #         }

    #         // Fonction de reception de message (vide pour l'instant)
    #         msgReceived_ = msgBuffer_;
    #         onMessageReceived(msgReceived_);

    #         // Si les donnees doivent etre enregistrees
    #         if(record){
    #             writer_->write(jsonObj);
    #         }
    #     }
    #     // Reinitialisation du message tampon
    #     msgBuffer_ = "";
    # }

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
            print("Serial Ready")
        else:
            raise IOError("Cannot connect to device on port {}".format(portName))


    def sendMessage(self,msg):
        if self.serial_.isOpen:
            self.serial_.write(msg.decode('utf-8', 'ignore'))

    def readReceivedMsg(self):

        self.newMessage.emit(str(self.serial_.readAll()))
    
    def serialQuit(self):
        if self.serial_ != None:
            del self.serial_



if __name__ == "__main__":

    app = QApplication(sys.argv)
    ui = Ui_MainWindow()
    ui.show()

    app.aboutToQuit.connect(ui.cleanUp)

    sys.exit(app.exec_())
