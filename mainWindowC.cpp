#include <QtGui>
#include "mainWindowC.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>


//tcpdump -i enp2s0 -vvv -X             просмотр пакетов в бинарном виде

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), p_iron_buttons{new EventHandler_IronButtons}
{
    //qDebug()<<"sizeof(monoTrackP18):"<<sizeof(monoTrackP18)<<"MAXTOCKANUM:"<<MAXTOCKANUM<<"sizeof(MonoTrack):"<<sizeof(MonoTrack);

                           //MonoTrack monoTrackP18[MAXTOCKANUM];

    redFi2 = 0.5;
    bluFi2 = 0.8;
    redFi1 = redFi2;//0.8;
    bluFi1 = bluFi2;//0.0;

    ui->setupUi(this);

    if( p_iron_buttons->is_valid() )
    {
        connect(p_iron_buttons.get(),&EventHandler_IronButtons::onKeyDown,     this,&MainWindow::on_iron_button_down);
        connect(p_iron_buttons.get(),&EventHandler_IronButtons::onKeyUp,       this,&MainWindow::on_iron_button_up);
        connect(p_iron_buttons.get(),&EventHandler_IronButtons::onKeyPressFail,this,&MainWindow::on_iron_button_error);
    }

    settings = new SETTINGSC(ui);

    // ИНИЦИАЛИЗАЦИЯ КЛИЕНТСКОГО UDP-СОКЕТА
    udpSockAddrLength = sizeof(udpSockAddr);
    if ((udpClientSocketFD=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"udpSrv: socket failed";  //connectStatus = false;//die("socket");
    }

    fcntl(udpClientSocketFD, F_SETFL, O_NONBLOCK);

    udpSockAddr.sin_family = AF_INET;
    udpSockAddr.sin_port = htons(settings->port_awsDispatcher);
    udpSockAddr.sin_addr.s_addr = inet_addr(settings->ipAddress_awsDispatcher);

    qDebug()<<"ИНИЦИАЛИЗАЦИЯ КЛИЕНТСКОГО UDP-СОКЕТА settings->port_awsMain:"<<settings->port_awsDispatcher
              <<"udpSockAddr.sin_addr.s_addr:"<<udpSockAddr.sin_addr.s_addr<<"udpSockAddr.sin_port:"<<udpSockAddr.sin_port;

    ui->azimuthScale360 ->controlModifierIsHoldingDown = false;
    ui->azimuthScale3600->controlModifierIsHoldingDown = false;

    for(int i = 0; i < MAXTOCKANUM; i++)
    {
        greenBrightness[i] = 0;
        yellowBrightness[i] = 0;
    }

    scaleInfo = new SCALE_INFO();//arm = new SCALE_INFO();

    setShadowEffects();

    komKomC.poiskState = 500;
    komKomC.targetIndexCY = -1;

    //trackPointsCount = 0;

    graypix.load(":/InfoInd/resgui/infoind/ind_gray7.png");
    lightGreenPixmap.load(":/InfoInd/resgui/infoind/ind_green72.png");
    darkGreenPixmap.load(":/InfoInd/resgui/infoind/ind_darkGreen01.png");
    lightRedPixmap.load(":/InfoInd/resgui/infoind/ind_red7.png");
    darkRedPixmap.load(":/InfoInd/resgui/infoind/ind_darkRed01.png");

    ikoBMP = QPixmap(ui->IKO->width(), ui->IKO->height());

    mainIKOEchoBMP = QPixmap(ui->IKO->width(), ui->IKO->height());
    zoomIKOEchoBMP = QPixmap(DEST_ZOOM_RECT_WIDTH_HEIGHT, DEST_ZOOM_RECT_WIDTH_HEIGHT);

#if 0

    //QExplicitlySharedDataPointer *dptr = ikoEchoBMP.data_ptr();
    //DataPtr *dptr = ikoEchoBMP.data_ptr();

    QPainter painter(&ikoEchoBMP);
    //painter.setBrush(QBrush(QColor::fromRgbF(1.0, 1.0, 0.0, 1.0)));//painter.setBrush(QBrush(QColor::fromRgbF(0, 0, 0, 0.03)));

    //painter.fillRect(0, 0, ikoEchoBMP.width(), ikoEchoBMP.height(), QBrush(QColor::fromRgbF(1.0, 1.0, 0.0, 1.0)));//Qt::black);
    //painter.fillRect(0, 0, ikoEchoBMP.width(), ikoEchoBMP.height(), QBrush(QColor(128, 64, 32, 16)));//::fromRgbF(1.0, 1.0, 0.0, 1.0)));//Qt::black);
    painter.fillRect(0, 0, ikoEchoBMP.width(), ikoEchoBMP.height(), QBrush(QColor(0, 0, 0, 255)));//::fromRgbF(1.0, 1.0, 0.0, 1.0)));//Qt::black);

    painter.setPen(QColor::fromRgb(0, 255, 0, 255));//painter.setBrush(QBrush(QColor::fromRgbF(0, 0, 0, 0.03)));
    painter.drawLine(10,10, 830, 830);

    //ikoEchoBMP.detach();

    QImage chip = ikoEchoBMP.toImage();

    for (int i = 0; i < chip.width(); ++i)
    {
        for (int j = 0; j < chip.height(); ++j)
        {
            //QColor clr = chip.pi
            //col = chip.pixel(i, j);
            //gray = qGray(col);
            //if(i % 5 == 0)

            *(chip.bits() + (chip.width() * j + i) * 4 + 2) = *(chip.bits() + (chip.width() * j + i) * 4 + 1);
            *(chip.bits() + (chip.width() * j + i) * 4 + 1) = 0;

                //chip.setPixel(i, j, qRgb(255, 255, 255));
            //chip.setPixelColor(i, j, QColor(255, 255, 0));
                //chip.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }


//    for (int i = 0; i < chip.width(); ++i)
//    {
//        for (int j = 0; j < chip.height(); ++j)
//        {
//            //col = chip.pixel(i, j);
//            //gray = qGray(col);
//            if(i % 5 == 0)
//                 *(chip.bits() + chip.width() * j * 4 + i) = 255;
//                //chip.setPixel(i, j, qRgb(255, 255, 255));
//            //chip.setPixelColor(i, j, QColor(255, 255, 0));
//                //chip.setPixel(i, j, qRgb(gray, gray, gray));
//        }
//    }    


//    for(int i = 0; i < 800; i++)
//        //chip.setPixel(i, i, 0xFFFFFFFF);// RGB.setColor(.bits() + 1400000 + i) = 255;
//        chip.setPixelColor(i, i, Qt::white);

    qDebug()<<"chip:"<<chip.width()<<chip.height()<<"chip.byteCount():"<<chip.byteCount()
            <<"    "<<*(chip.bits() + 0)<<*(chip.bits() + 1)<<*(chip.bits() + 2)<<*(chip.bits() + 3)
            <<"    "<<*(chip.bits() + 4)<<*(chip.bits() + 5)<<*(chip.bits() + 6)<<*(chip.bits() + 7)
            <<"    "<<*(chip.bits() + 840 * 4 + 4)<<*(chip.bits() + 840 * 4 + 5)<<*(chip.bits() + 840 * 4 + 6)<<*(chip.bits() + 840 * 4 + 7);

//    qDebug()<<"chip:"<<chip.width()<<chip.height()<<"chip.byteCount():"<<chip.byteCount()
//           <<"    "<<*(chip.bits() + 1400000 + 0)<<*(chip.bits() + 1400000 + 1)<<*(chip.bits() + 2)<<*(chip.bits() + 3)
//           <<"    "<<*(chip.bits() + 1400000 + 4)<<*(chip.bits() + 5)<<*(chip.bits() + 6)<<*(chip.bits() + 7);

    QPixmap ikoEchoBMP2 = QPixmap::fromImage(chip, Qt::AutoColor);
    //ikoEchoBMP.fromImage(chip, Qt::AutoColor);


    //qDebug()<<"ikoEchoBMP:"<<ikoEchoBMP.data.data().. chip.width()<<chip.height()<<"chip.byteCount():"<<chip.byteCount()


    ui->IKO->setPixmap(ikoEchoBMP2);

    //ikoEchoBMP = chip.t
#endif

    centerMainIKO = QPoint(ikoBMP.width() / 2, ikoBMP.height() / 2);

    ikoZoomBMP = QPixmap(ui->IKO->width() * 4, ui->IKO->height() * 4);
    centerZoomIKO = QPoint(ikoZoomBMP.width() / 2, ikoZoomBMP.height() / 2);

    yarkostEchoMain = (double *)calloc(IKO_WIDTH_HEIGHT * IKO_WIDTH_HEIGHT, sizeof(double));
    imgDataMain = (unsigned char*)calloc(IKO_WIDTH_HEIGHT * IKO_WIDTH_HEIGHT * 3, sizeof(unsigned char));

    yarkostEchoZoom = (double *)calloc(DEST_ZOOM_RECT_WIDTH_HEIGHT * DEST_ZOOM_RECT_WIDTH_HEIGHT, sizeof(double));
    imgDataZoom = (unsigned char*)calloc(DEST_ZOOM_RECT_WIDTH_HEIGHT * DEST_ZOOM_RECT_WIDTH_HEIGHT * 3, sizeof(unsigned char));

    zoomFactor = 4.0;
    zoomSdvig = 8;

    destRect2DrawZoomIKO = QRect(zoomSdvig, zoomSdvig, DEST_ZOOM_RECT_WIDTH_HEIGHT, DEST_ZOOM_RECT_WIDTH_HEIGHT);

    ikoBMPFin = QPixmap(ui->IKO->width(), ui->IKO->height());

    ikoBMPSvetoFilter = QPixmap(ui->IKO->width(), ui->IKO->height());

    /////////////////////interchange2ConsoleStart();

    connect(ui->azimuthScale360,  SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
    connect(ui->azimuthScale3600, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
    connect(ui->elevationScale87, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
    connect(ui->elevationScale870, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));

    connect(ui->azimuthScale360, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot(double)));
    connect(ui->azimuthScale3600, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot(double)));
    connect(ui->elevationScale87, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot_elevationScale(double)));
    connect(ui->elevationScale870, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot_elevationScale(double)));

    connect(ui->IKO, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveIKO(int, int)));
    connect(ui->IKO, SIGNAL(Mouse_Click(QMouseEvent *)),this,SLOT(IKOClick(QMouseEvent *)));

    signalMapper = new QSignalMapper(this);

    connect(ui->pushButtonEnglish, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonPortug, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonRussian, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->pushButtonEnglish, 0);
    signalMapper->setMapping(ui->pushButtonPortug, 2);
    signalMapper->setMapping(ui->pushButtonRussian, 1);

    connect(signalMapper, SIGNAL(mapped(const int &)), this, SLOT(languageChange(const int &)));

    languageChange(settings->languageIndex);

    drawScale360(0, 0);
    drawScale3600(0, 0);
    drawElevationScale87(0);
    drawElevationScale870(0);

    drawGraphicsTimer =  new QTimer(this);
    connect(drawGraphicsTimer, SIGNAL(timeout()), this, SLOT(timerTickSlot()));    

    drawGraphicsTimer->start(TIMER_INTERVAL);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//int cntr = 120;

void MainWindow::timerTickSlot()
{    
    udpSocketReadWriteData();

    //qDebug()<<"timerTickSlot:"<<cntr++;

//    qDebug()<<QString("MainWindow::timerTickSlot: 0x%1 0x%2 0x%3").arg(local_memory[SERVICE_TOTAL_LENGTH + 16], 2, 16, QChar('0'))
//              .arg(local_memory[SERVICE_TOTAL_LENGTH + 17], 2, 16, QChar('0')).arg(local_memory[SERVICE_TOTAL_LENGTH + 18], 2, 16, QChar('0'));

/*
    int n = interchange2Console();  //qDebug()<<"timerTickSlot n:"<<n;

    // К О М А Н Д Ы   О Т   В К П

    QString vkpStr = "К О М А Н Д Ы   О Т   В К П\n";

    vkpStr += "SAC:" + QString::number(shMemC2U.vkp2zrk.sac) +
            "  SIC:" + QString::number(shMemC2U.vkp2zrk.sic) + "  Nзрк:" + QString::number(shMemC2U.vkp2zrk.Nzrk);

    vkpStr += "\nназн.сектор отв: " + QString(shMemC2U.vkp2zrk.sectorOtvetstvennostiNaznachit ? "да" : "нет") +
            QString("   от: ") + QString::number(shMemC2U.vkp2zrk.sectorOtvetstvennostiStart, 'f', 1) +
            "   до: "  + QString::number(shMemC2U.vkp2zrk.sectorOtvetstvennostiEnd, 'f', 1);

    //if((shMemC2U.vkp2zrk.fspec1 & 0x10) == 0x10) //  если есть Команда на ЗРК
    {
        //vkpStr += "\nКоманда b1" + strByte125(shMemC2U.vkp2zrk.KomandaYprZRK030[0], "запр.изл", "М100%", "упрВКП", "#", "#", "#", "-", "-");// Байт 1

        vkpStr += "\n\nКоманда на ЗРК:\nзапр.изл: " + QString(((shMemC2U.vkp2zrk.komandaYprZRK030[0] & 0x80) == 0x80) ? "да" : "нет");
        vkpStr += "\nМ100%: " + QString(((shMemC2U.vkp2zrk.komandaYprZRK030[0] & 0x40) == 0x40) ? "да" : "нет");
        vkpStr += "\nперейти на упрВКП: " + QString(((shMemC2U.vkp2zrk.komandaYprZRK030[0] & 0x20) == 0x20) ? "да" : "нет");
        vkpStr += "\nперевести в резерв: " + QString(((shMemC2U.vkp2zrk.komandaYprZRK030[0] & 0x20) == 0x20) ? "да" : "нет");

        uint8_t kk = shMemC2U.vkp2zrk.komandaYprZRK030[0] & 0b1111;
        vkpStr +=  QString("\nКК: 0b%1 - ").arg(kk, 4, 2, QChar('0'));

        switch(kk)
        {
            case 0b0011: vkpStr += "привести ЗРК в  готовность 	3"; break;
            case 0b0010: vkpStr += "привести ЗРК в  готовность 	2"; break;
            case 0b0001: vkpStr += "привести ЗРК в  готовность 	1"; break;
            case 0b0100: vkpStr += "привести ЗРК в  готовность 	1 ускоренно"; break;
            case 0b0101: vkpStr += "провести автономный тренаж (АТ)"; break;
            case 0b0110: vkpStr += "провести комплексный тренаж (АТ)"; break;
            default : vkpStr += "код команды не распознан";
        }

        vkpStr += "\nзапр.изл.сек: " + QString(((shMemC2U.vkp2zrk.komandaYprZRK030[1] & 0x40) == 0x40) ? "да" : "нет");
        vkpStr += QString("\nрегл.целерасп: 0b%1").arg((shMemC2U.vkp2zrk.komandaYprZRK030[1] & 0b00111000)>>3, 3, 2, QChar('0'));
    }

    //if((fspec1 & 0x08) == 0x08) //  если есть I125/040 Управление ракетами
    {
        vkpStr += "\n\nУпр.ракетами М1:\nогран.расход:";
        vkpStr += ((shMemC2U.vkp2zrk.yprRaketami040[1] & 0x80) == 0x80) ? "да" : "нет";

        vkpStr += QString("\nготовить непр.: %1").arg((shMemC2U.vkp2zrk.yprRaketami040[1] & 0b01110000) >> 4, 2, 10, QChar('0'));
        vkpStr += QString("  готовить разово: %1").arg(shMemC2U.vkp2zrk.yprRaketami040[1] & 0b00001111, 2, 10, QChar('0'));

        vkpStr += QString("\nотменить непр.: %1").arg((shMemC2U.vkp2zrk.yprRaketami040[4] & 0b01110000) >> 4, 2, 10, QChar('0'));
        vkpStr += QString("  отменить разово: %1").arg(shMemC2U.vkp2zrk.yprRaketami040[4] & 0b00001111, 2, 10, QChar('0'));

    }

    QString crdStr = "\n\nСевер: " + QString::number(shMemC2U.vkp2zrk.coordYHX.y_km, 'f', 3) +
            "  Высота: " + QString::number(shMemC2U.vkp2zrk.coordYHX.h_km, 'f', 3) +
            "  Восток: " + QString::number(shMemC2U.vkp2zrk.coordYHX.x_km, 'f', 3) +
            "\nVсев :" + QString::number(shMemC2U.vkp2zrk.coordYHX.vy_ms, 'f', 3) +
            "  Vвыс :" + QString::number(shMemC2U.vkp2zrk.coordYHX.vh_ms, 'f', 3) +
            "  Vвост:" + QString::number(shMemC2U.vkp2zrk.coordYHX.vx_ms, 'f', 3);


    for(int i = 0; i < 3; i++)
    {
        crdStr +=     "\n\nЦель  N " + QString::number(i + 1) +
                        "  компл.тренажа\nСевер: " + QString::number(shMemC2U.vkp2zrk.cmplxYHX[i].y_km, 'f', 3) +
                        "  Высота: " + QString::number(shMemC2U.vkp2zrk.cmplxYHX[i].h_km, 'f', 3) +
                        "  Восток: " + QString::number(shMemC2U.vkp2zrk.cmplxYHX[i].x_km, 'f', 3) +
                        "\nVсев :" + QString::number(shMemC2U.vkp2zrk.cmplxYHX[i].vy_ms, 'f', 3) +
                        "  Vвыс :" + QString::number(shMemC2U.vkp2zrk.cmplxYHX[i].vh_ms, 'f', 3) +
                        "  Vвост:" + QString::number(shMemC2U.vkp2zrk.cmplxYHX[i].vx_ms, 'f', 3);
    }

    crdStr +=     "\n\nrejimPoiskaSNR: " + QString::number(shMemC2U.srvDrvInfo.rejimPoiskaSNR);//crdStr +=     "\n\ncyStatus: " + QString::number(shMemC2U.srvDrvInfo.cyStatus);
    crdStr +=     "\nЦУ: " + QString::number(shMemC2U.vkp2zrk.komandiPoCelevomyKanaly1.CU);
    crdStr +=     "\nотмена ЦУ: " + QString::number(shMemC2U.vkp2zrk.komandiPoCelevomyKanaly1.OCU);
*/
    //QString vkpStr = "К О М А Н Д Ы   О Т   В К П\n";

//    QString crdStr = "\n\nNцели: " + QString::number(shMemC2U.apyInfo.nextTocka[0].targetNo) +
//            " Д: " + QString::number(shMemC2U.apyInfo.nextTocka[0].groundRange_km, 'f', 3) + "km" +
//            " Аз:" + QString::number(shMemC2U.apyInfo.nextTocka[0].azimuth360, 'f', 3) + "°" +
//            " V:" + QString::number(shMemC2U.apyInfo.nextTocka[0].groundSpeed_ms, 'f', 3) + "м/c" +
//            " Курс :" + QString::number(shMemC2U.apyInfo.nextTocka[0].kyrs, 'f', 3) + "°";


    //if(trackPointsCount >= MONOTRACKSIZE)
      //  trackPointsCount = 0;

    // Р А З Б О Р   И Н Ф О Р М А Ц И И   О Т   П - 1 8
    if(console2User.apyInfo.p18TransactionId > -1)
    {
        ui->groupBoxP18->setEnabled(true);
    }
    else
    {
        ui->groupBoxP18->setEnabled(false);        
    }

    if(komKomC.useP18 || komKomC.rejimRabotiZRK == RejimRabotiZRK::TR_A)
        parserP18();

//    for(int j = 0; j < shMemC2U.srvDrvInfo.maxTockaCurrentNum; j++)
//    {
//        memcpy(&monoTrack[j][trackPointsCount], &shMemC2U.apyInfo.nextTocka[j], sizeof(TargetParams));
//    }

//    trackPointsCount++;

    nextTochkaOptions();
/*
    for(int i = 0; i < MAXTOCKANUM; i++)
    {
        QString crdStr = "i:" + QString::number(i) + " size:" + QString::number(monoTrack2[i].tailSize) + " T: " + QString::number(0.001 * monoTrack2[i].head.elapsedTime, 'f', 3) + " xx: ";

        for(int j = 0; j < monoTrack2[i].tailSize; j++)
        {
            crdStr += " " + QString::number(monoTrack2[i].tail[j].groundX, 'f', 2);
        }

        crdStr += " head:" + QString::number(monoTrack2[i].head.groundX, 'f', 2);

        qDebug()<<crdStr;
    }
*/
    drawMainIKO();

    //ui->labelMouseMoveIndicatorEB->setText(QString("MouseMoveIndicatorEB: nextTargetX: %1° xx: %2° ").arg(s nextTargetX).arg(xx));

    drawAzimuthScales(console2User.srvDrvInfo.currentAzimuth);// arm->azimuthPRV_CURRENT);
    drawElevationScale87(console2User.srvDrvInfo.currentDirectrisaElevationAngle);// arm->azimuthPRV_CURRENT);
    drawElevationScale870(console2User.srvDrvInfo.currentDirectrisaElevationAngle);// arm->azimuthPRV_CURRENT);

    // Д А Т А   И   В Р Е М Я
    QDateTime now =  QDateTime::currentDateTime();
    if(now.time().msec() < 500)
        ui->qLabelTime->setText(now.toString("hh:mm:ss"));
    else
        ui->qLabelTime->setText(now.toString("hh mm ss"));
    ui->qLabelDate->setText(now.toString("yyyy.MM.dd"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //commander2Console.stopConsolePrgrm = 0xFF;

    //usleep(1300 * 1000);

    settings->saveToFile(ui);

    usleep(300 * 1000);

//    if(awsUserMode == AWS_UserMode::Local)
//    {
//        on_buttonStop_clicked();

//        diagnostics->saveDiagnosticReport(settings->aws16AbsolutPath);
//    }


    //sleep(3);

//    qDebug()<<3121212;
//    QMessageBox msgBox;
//    msgBox.setText("The document has been modified.");
//    msgBox.setInformativeText(QString::number(arm->settings->seaLevel));
//    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//    msgBox.setDefaultButton(QMessageBox::Save);
//    int ret = msgBox.exec();
}


void MainWindow::on_buttonClose_clicked()
{
    //commander2Console.stopConsolePrgrm = 0xFF;
    //usleep(300 * 1000);

    close();
}

void MainWindow::setShadowEffect(QWidget *lbl)
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
       effect->setBlurRadius(1);
       effect->setXOffset(-1);
       effect->setYOffset(-1);
       effect->setColor(qRgba(0,0,0,255));

    lbl->setGraphicsEffect(effect);
}

void MainWindow::setShadowEffects()
{
    setShadowEffect(ui->labelWindowTitle);
    setShadowEffect(ui->groupBoxScanSector);
    //setShadowEffect(ui->groupBoxZahvat);
    setShadowEffect(ui->groupBoxRejRabotiZRK);

    //setShadowEffect(ui->labelPlusMinusAzScale);
}

void MainWindow::on_pushButtonFullScreen_clicked()
{
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
    this->setWindowState(this->windowState() ^ Qt::WindowMaximized);
}

int udpSrvCounter = 0;


int MainWindow::udpSocketReadWriteData()
{
    udpSrvCounter++;

    ssize_t totalReadBytes = 0;
    //ssize_t totalSentBytes = 0;

    if(console2User.srvDrvInfo.komJustExecuteCounter[AZIMUTH] == komKomC.srv[AZIMUTH].executeCounter / 10)
        komKomC.srv[AZIMUTH].executeCounter = 10 * console2User.srvDrvInfo.komJustExecuteCounter[AZIMUTH];
    else
        if(komKomC.srv[AZIMUTH].executeCounter % 10 > 0)
            komKomC.srv[AZIMUTH].executeCounter--;

    if(console2User.srvDrvInfo.komJustExecuteCounter[ELEVANG] == komKomC.srv[ELEVANG].executeCounter / 10)
        komKomC.srv[ELEVANG].executeCounter = 10 * console2User.srvDrvInfo.komJustExecuteCounter[ELEVANG];
    else
        if(komKomC.srv[ELEVANG].executeCounter % 10 > 0)
            komKomC.srv[ELEVANG].executeCounter--;

    //if(connectStatus)
    {
        //totalSentBytes =
        //sendto(udpClientSocketFD, &udpSrvCounter, sizeof(int), 0, (struct sockaddr *) &udpSockAddr, udpSockAddrLength);        
        //qDebug()<<"udpSocketReadWriteData udpSrvCounter:"<<udpSrvCounter<<"komKomC.rejimRabotiZRK:"<<komKomC.rejimRabotiZRK;

        sendto(udpClientSocketFD, &komKomC, sizeof(KOMANDI_KOMANDIRA), 0, (struct sockaddr *) &udpSockAddr, udpSockAddrLength);

        //totalReadBytes = recvfrom(udpClientSocketFD, &shMemC2U, sizeof(LauncherConsole2User), 0, (struct sockaddr *) &udpSockAddr, &udpSockAddrLength);    //  2017-12-29

        ssize_t totalReadBytes = 0;
        ssize_t nextReadBytes;
        int readCirclCounter = 0;
        char readByteArray[sizeof(MServer2UserC)];

        while (totalReadBytes < abs(sizeof(MServer2UserC)) && readCirclCounter++ < 10)
        {
            nextReadBytes = recvfrom(udpClientSocketFD, readByteArray + totalReadBytes, sizeof(MServer2UserC) - totalReadBytes, 0, (struct sockaddr *) &udpSockAddr, &udpSockAddrLength);

            if(nextReadBytes < 0)
                nextReadBytes = 0;

            totalReadBytes += nextReadBytes;
        }

        //   2018_06_16 if(totalReadBytes == sizeof(SNR_INFO))
        //   2018_06_16     memcpy(&console2User.srvDrvInfo, readByteArray, sizeof(SNR_INFO));
        if(totalReadBytes == sizeof(MServer2UserC))
            memcpy(&console2User, readByteArray, sizeof(MServer2UserC));

        //qDebug()<<"udpSocketReadWriteData totalReadBytes:"<<totalReadBytes<<"currentAzimuth:"<<console2User.srvDrvInfo.currentAzimuth;
                  //<<"missileStatus:"<<shMemC2U.srvDrvInfo..launcherInfo[1].missileInfo[0].missileStatus;

    }

    return totalReadBytes;
}

void MainWindow::on_iron_button_down(int key)
{
    qWarning() << "on_iron_button_down " << key;

    switch(key)
    {
        case 25: ui->pushButtonRejimRabotiBR->setChecked(true); break;
        case 26: ui->pushButtonRejimRabotiFK->setChecked(true); break;
        case 27: ui->pushButtonRejimRabotiTren->setChecked(true); break;
        case 28: ui->pushButtonRejimRabotiRR->setChecked(true); break;

        case  5: ui->pushButtonZRK_RemoteControl->setChecked(true); break;
        case 41: ui->pushButtonZRK_LocalControl->setChecked(true);
    }

}

void MainWindow::on_iron_button_up(int key)
{
    qWarning() << "on_iron_button_up   " << key;
}

void MainWindow::on_iron_button_error()
{
    qWarning() << "::Too many keys pressed...";
}

//ui->dblSpnBxAzimuthCorrection->setValue(6.0 * arg1 );

void MainWindow::on_dblSpnBxTochnayShkala_valueChanged(double arg1)
{
    ui->dblSpnBxAzimuthCorrection->setValue(arg1 * 0.06);
}
