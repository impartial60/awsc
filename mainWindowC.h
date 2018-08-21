#ifndef MAINWINDOWC_H
#define MAINWINDOWC_H

#include <QMainWindow>
#include <QThread>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QSignalMapper>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <unistd.h>

#include <math.h>

#include <EventHandler_IronButtons.h>
#include <sharedMem.h>
#include <scaleInfo.h>
#include "settingsC.h"
#include "qlabelm.h"

#define IKO_WIDTH_HEIGHT 840

#define DEST_ZOOM_RECT_WIDTH_HEIGHT 320


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KOMANDI_KOMANDIRA komKomC;
    MServer2UserC console2User;
    MonoTrack monoTrackP18[MAXTOCKANUM];

    float awsCsecsSinceStartOfDay;

    uint32_t klmTransactionId_old = 0;
    int32_t p18TransactionId_old = -1;

    int target_IKO_MouseMove_Number = -1;
    int simTrgt_IKO_MouseMove_Number = -1;
    float targetObsolTime[MAXTOCKANUM];

    QString strV[MAXTOCKANUM];
    QString strH[MAXTOCKANUM];
    QString strK[MAXTOCKANUM];
    QString strI[MAXTOCKANUM];

    float nextTocka_slantRange_km[MAXTOCKANUM];
    float nextTocka_azimuth360[MAXTOCKANUM];
    float nextTocka_elevAngle[MAXTOCKANUM];

    int greenBrightness[MAXTOCKANUM];
    int yellowBrightness[MAXTOCKANUM];

    struct sockaddr_in udpSockAddr;
    int udpClientSocketFD;
    socklen_t udpSockAddrLength;

    float redFi1;
    float bluFi1;
    float redFi2;
    float bluFi2;

    QPixmap ikoBMPFin;
    QPixmap ikoBMPSvetoFilter;

    QPixmap ikoBMP;
    QPixmap ikoZoomBMP;

    QPixmap mainIKOEchoBMP;
    QPixmap zoomIKOEchoBMP;

    float zoomFactor;
    int zoomSdvig;

    QRect destRect2DrawZoomIKO;
    QRect sourceRect2DrawZoomIKO;

    QPoint centerMainIKO;
    QPoint centerZoomIKO;
    float factorR_IKO_Main;
    float factorR_IKO_Zoom;
    float mouseCursorAzimuth;

    bool zoomIKOfreez = false;
    int zoomIKO_x = 0;
    int zoomIKO_y = 0;

    uchar  *imgDataMain;
    double *yarkostEchoMain;

    uchar  *imgDataZoom;
    double *yarkostEchoZoom;

    QPixmap graypix;
    QPixmap lightGreenPixmap;
    QPixmap darkGreenPixmap;
    QPixmap lightRedPixmap;
    QPixmap darkRedPixmap;

    QTimer *drawGraphicsTimer;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    int interchange2Console();
//    void interchange2ConsoleStart();

//    bool crc16BoolResult(unsigned char *pcBlock, unsigned int len, unsigned short int crc16Value);
//    QString crc16QStringResult(unsigned char *pcBlock, unsigned int len, unsigned short int crc16Value);
//    void assignCRC16Values(unsigned char *pcBlock, unsigned int len, unsigned char *lowByte , unsigned char *highByte);

//    unsigned char priznakGr();
    void nextTochkaOptions();
    void parserP18();

    void drawAzimuthScales(float azCur360);
    void drawScale360(float drctrAz, float azBlue);
    void drawScale3600(float drctrAz, float azBlue);
    void drawElevationScale87(float elevationAngleCur);
    void drawElevationScale870(float elevationAngleCur);

    void drawFormularC(QPainter *painter, bool _isZoomIKO, int plotInd);

    void drawTextHalo(QPainter *painter, QPen textPen, QFont textFont, int x, int y, QString text);

    void drawP18(bool isZoomIKO, QPainter *painter, QPoint centerIKO, float factorR_IKO, float radiusIKO);

    void drawEchoMainIKO(QPainter *painter);
    void drawEchoZoomIKO(QPainter *painter);

    void drawIKO(bool isZoomIKO, QPainter *painter);

    void drawMainIKO();//int plotTotalNumber);

    void drawZoomIKO();//int plotTotalNumber);

    void setShadowEffects();
    void setShadowEffect(QWidget *lbl);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void timerTickSlot();

    void MouseMoveIKO(int xx, int yy);
    void IKOClick(QMouseEvent *ev);


    void MouseMoveScales(int hovMain, int hovDelt);

    void MouseClickSlot(double deltaAzimHov);
    void MouseClickSlot_elevationScale(double deltaHov);
    //////////void MouseDoubleClickSlot_indicatorEB(int scrX, int scrY);

    void languageChange(int lng);

private slots:
    void on_buttonClose_clicked();

    void on_pushButtonFullScreen_clicked();

    int udpSocketReadWriteData();

    void on_iron_button_down(int key);
    void on_iron_button_up(int key);
    void on_iron_button_error();        

    void on_dblSpnBxTochnayShkala_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QSignalMapper *signalMapper;
    SETTINGSC *settings;    
    SCALE_INFO *scaleInfo;
    std::unique_ptr<EventHandler_IronButtons> p_iron_buttons;
};

#endif // MAINWINDOWC_H
