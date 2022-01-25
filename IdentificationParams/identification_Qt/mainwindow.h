#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QCloseEvent>
#include <QDebug>
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSerialPortInfo>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts>
#include <QGraphicsView>
#include <QPixmap>
#include <QObject>
#include <QSound>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QSoundEffect>
#include <QMediaPlayer>

// Propres librairies
#include "csvwriter.h"
#include "serialprotocol.h"

#include "car.h"
#include "pipe.h"
#include "flag.h"
#include "pendule.h"
#include "sapin.h"

// Classe definissant l'application
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const qint32 BAUD_RATE = 115200;

    explicit MainWindow(int updateRate, QWidget *parent = nullptr);
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override;
    void closeEvent(QCloseEvent *event) override;

    void sendMessage(QString msg);
    void setUpdateRate(int rateMs);

    void onPeriodicUpdate();
    void onMessageReceived(QString);

private slots:
    void receiveFromSerial(QString);
    void sendPulseSetting();
    void sendPulseStart();
    void manageRecording(int);
    void changeJsonKeyValue();
    void startSerialCom(QString);
    void sendPID();
    void sendStart();
    void sendStop();
    void sendPosition();


private:
    void connectTimers(int updateRate);
    void connectButtons();
    void connectSerialPortRead();
    void connectSpinBoxes();
    void startRecording();
    void stopRecording();
    void connectTextInputs();
    void connectComboBox();
    void moveMario();
    void addFormesInitial();
    void portCensus();
    void showPopUp();
    void showGIF();
    void startMarioLetsGo();
    void startItsMeMario();
    void startIMfastAsF();
    void startMarioDeath();
    void gameover();

    bool record = false;
    CsvWriter* writer_;
    QTimer updateTimer_;
    QString msgReceived_{""};
    QString msgBuffer_{""};
    SerialProtocol* serialCom_=nullptr;

    QString JsonKey_;
    QLineSeries series_;
    QChart chart_;
    QGraphicsScene *scene;    //Création Scene
    QMovie *movie = new QMovie(":/image/WeDidIt.gif");
    QLabel *label;

    QColor colorRed = Qt::red;
    QColor colorBlack = Qt::black;
    QColor colorBlue = Qt::darkGray;
    QColor colorGreen = Qt::green;

    QBrush brushRed = Qt::SolidPattern;
    QBrush brushBlack = Qt::SolidPattern;
    QBrush brushBlue = Qt::SolidPattern;
    QBrush brushGreen = Qt::SolidPattern;

    QGraphicsPixmapItem * pixItem;
    QGraphicsPixmapItem * GameOver;

    QRectF rail;
    QRectF panierGauche;
    QRectF panierMillieu;
    QRectF panierDroite;

    QSound *ohYeah = new QSound(":/sound/WeDidIt.wav");
    QSound *IMfastAsFuck = new QSound(":/sound/Im_fast_as_F_boi.wav");
    QSound *letsGoMario = new QSound(":/sound/mario_lets_a_go.wav");
    QSound *ItsMeMario = new QSound(":/sound/Its_a_me_Mario.wav");
    QSound *Mario_Death = new QSound(":/sound/Mario_Death.wav");
    //    QSound *SapinSound = new QSound(":/sound/Never Gonna Give You Up.wav");

    QTimer *timer = new QTimer();
    QTimer *sapintimer;


    CarItem * camion;
    PipeItem * pipe;
    FlagItem * flag;
    PenduleItem * pendule;
    SapinItem * sapin;

    int sapin_dropped =          0;

    double etat =                100;

    double distance_obstacle =   100;
    //double distance_depot =      500;
    // double lastposvoiture =      0;
    double positionVoiture =     0;//peut peut etre enlever et garder dans la classe max = 602
    double positionObstacle =    300;//same 350,-18 a 620
    double hauteur_obstacle =    -0.01;
    double longeurPendule =      40;
    double positionDepot =       500;
    //double lastanglePendule =    0;
    double anglePendule =        -45;
    double angleSapin =          0;
    double largeurRobot =        50;
    double hauteurRobot =        10;
    double diametreRoue =        12;
    double afficher     =        0;
    double distanceRouePendule = 0.217;
    double covertisseurMagique = (602.0/1.68);
    double vitesse_angulaire =   0;
    double son =                  0;

    double distance_obstacle_anim = 0;
    double positionObstacle_anim = 0;
    double positionDepot_anim = 0;
    double hauteur_obstacle_anim = 0;

    bool casZero     =           false;
    bool sapinLacher =           false;
    bool distance_envoyer =      false;
    bool scene_initialise = false;
    bool game_on = true;
    bool go_anim = false;

protected:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
