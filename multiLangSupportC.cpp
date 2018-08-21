#include <QtGui>
#include <QCheckBox>
#include <QRadioButton>
#include "mainWindowC.h"
#include "ui_mainwindow.h"

#include "../../aws125CommonInclude/externCommon.h"
#include "../../aws125CommonInclude/commonEnum.h"


//enum WidgetTypes{GroupBoxWidget, PushButtonWidget, CheckBoxWidget, RadioButtonWidget, LabelWidget, WindowWidget};

//void MainWindow::setCaption(QWidget *widget, int widgetType, int lngInd, QString textR, QString textE, QString textP)
//{
//    QString caption;

//    switch(lngInd)
//    {
//        case english: caption = textE; break;
//        //case russian: caption = textR; break;
//        case russian: caption = QString::fromUtf8(textR.toStdString().c_str()); break;
//        case portuguese: caption = textP; break;
//        default: caption = "unKnownCaption";
//    }

//    switch(widgetType)
//    {
//        case GroupBoxWidget: ((QGroupBox*)widget)->setTitle(caption); break;
//        case PushButtonWidget: ((QPushButton*)widget)->setText(caption); break;
//        case CheckBoxWidget: ((QCheckBox*)widget)->setText(caption); break;
//        case RadioButtonWidget: ((QRadioButton*)widget)->setText(caption); break;
//        case LabelWidget: ((QLabel*)widget)->setText(caption); break;
//        case WindowWidget: ((QMainWindow*)widget)->setWindowTitle(caption); break;

//        default: qDebug()<<"setCaption unKnownwidgetType:"<<widgetType;
//    }
//}



void MainWindow::languageChange(int lng)
{
    settings->languageIndex = lng;

    //qDebug()<<"languageChange languageChange lng:"<<lng;
    if(lng < 0 || lng > 2)
    {
        qDebug()<<"languageChange lng:"<<lng;
        return;
    }

//    if(diagnostics != NULL)
//    {
////       int langInd = 0;
////       if(settings->languageIndex == russian)
////           langInd = 1;
//        diagnostics->setIndexLanguage(settings->languageIndex);
//    }

    setCaption(this, WindowWidget, lng, "АРМ командира",
                                        "Commander AWS",
                                        "");

    setCaption(ui->labelWindowTitle, LabelWidget, lng,  "Автоматизированное рабочее место командира",
                                                        "Commander Automated Workstation",
                                                        "");

    // В Е Р Х Н Я Я   П А Н Е Л Ь      В Е Р Х Н Я Я   П А Н Е Л Ь      В Е Р Х Н Я Я   П А Н Е Л Ь

    setCaption(ui->groupBoxCY, GroupBoxWidget, lng, "Целеуказание", "Target Designation", "Designação de alvos");
    setCaption(ui->pushButtonVneshneeCY, PushButtonWidget, lng, "Внешнее", "External", "Exterior");
    setCaption(ui->pushButtonMestnoeCY, PushButtonWidget, lng, "Местное", "Internal", "Interna");

//    setCaption(ui->groupBoxPrivoda, GroupBoxWidget, lng, "Питание приводов", "Actuator Power", "Alimentação de acionamentos");
//    setCaption(ui->pushButtonSwinging, PushButtonWidget, lng, "Качание", "Swinging", "Oscilação");
//    setCaption(ui->pushButtonRotation, PushButtonWidget, lng, "Вращение", "Rotation", "Rotação");

//    setCaption(ui->groupBoxPRF, GroupBoxWidget, lng, "Запуск", "Pulse Repet.Freq.", "   P R F   ");
//    setCaption(ui->pushButton400Hz, PushButtonWidget, lng, " Редкий    ", " L o w        ", " B a i x o  ");
//    setCaption(ui->pushButton800Hz, PushButtonWidget, lng, " Частый    ", " H i g h      ", "   A l t o    ");
//    setCaption(ui->pushButtonVneshnZapusk, PushButtonWidget, lng, " Внешний ", " External  ", "  Exterior ");


//    setCaption(ui->groupBoxAntEcv, GroupBoxWidget, lng, "Режим излучения", "Antenna-Dummy Selector", "Seleção Ant.-Carga Fant.");
//    setCaption(ui->pushButtonAnt, PushButtonWidget, lng, "Ант.", "Ant.", "Ant.");
//    setCaption(ui->pushButtonEcv, PushButtonWidget, lng, "Экв.", "Dum.", "Fant.");
//    setCaption(ui->pushButtonRestrictedSectors, PushButtonWidget, lng, " По команде ", " By order  ", "  Por ordem   ");
//    if(lng ==  english)
//        ui->pushButtonRestrictedSectors->setGeometry(44, 90, 106, 32);
//    else
//        ui->pushButtonRestrictedSectors->setGeometry(34, 90, 126, 32);


//    setCaption(ui->pushButtonDetect, PushButtonWidget, lng, " Обнаружение", "     Detection     ", "      Deteção      ");
//    setCaption(ui->labelPorog, LabelWidget, lng, "порог обнаружения", "detection threshold", "limite de deteção");
//    setCaption(ui->labelMaxPlots, LabelWidget, lng, "макс.количество отметок", "max plots number", "número máximo de plots");
//    setCaption(ui->labelStrobWidth, LabelWidget, lng, "ширина строба обнаружения", "detection strobe width", "largura de estrobo de deteção");
//    setCaption(ui->labelPorogPeleng, LabelWidget, lng, "порог обн. пеленга", "bearing detection threshold", "limite de deteção de rolamento");

//    setCaption(ui->groupBoxTransmitter, GroupBoxWidget, lng, "Передающее устройство", " Transmitter ", " Transmissor ");
//    setCaption(ui->labelNakal, LabelWidget, lng, "накал", "heating", "aquecimento");
//    setCaption(ui->pushButtonTransmitter, PushButtonWidget, lng, "Высокое", "High Voltage", "Voltagem alta");
//    if(lng != russian)
//        ui->pushButtonTransmitter->setGeometry(16, 104, 114, 32);
//    else
//        ui->pushButtonTransmitter->setGeometry(26, 104, 94, 32);

//    setCaption(ui->groupBoxFreq, GroupBoxWidget, lng, "Перестройка частоты          ", "Frequency Tuning         ", "Redefinição da frequência         ");

//    if(lng == english)
//        ui->labelFreqLock->mapFromParent(QPoint(128, 4));
//    else
//        ui->labelFreqLock->mapFromParent(QPoint(228, 4));

//    if(lng == english)
//        ui->labelFreqLock->setGeometry(208, 4, 22, 22);
//    else
//        ui->labelFreqLock->setGeometry(226, 4, 22, 22);

    setCaption(ui->groupBoxDateTime, GroupBoxWidget, lng, "Дата, время", "Date, time", "   ");


//    setCaption(ui->groupBoxPassword, GroupBoxWidget, lng, "Права доступа", "Permissions", "Direitos de acesso");
//    passwordLanguageChange(lng);

//    setCaption(ui->pushButtonMore, PushButtonWidget, lng, "Промотр", "View", "  Ver ");

    // Л Е В А Я   П А Н Е Л Ь     Л Е В А Я   П А Н Е Л Ь     Л Е В А Я   П А Н Е Л Ь

    setCaption(ui->groupBoxMashtabD, GroupBoxWidget, lng, "Масштаб Д, км", "Range Scale, km", "Escala D");

    setCaption(ui->groupBoxMetki, GroupBoxWidget, lng, "Метки", "Grids", "Marcas");
    setCaption(ui->pushButtonGridAz, PushButtonWidget, lng, "  Азимут ", " Azimuth", " Azimuth");
    setCaption(ui->pushButtonGridR, PushButtonWidget, lng, "  Дальн. ", "   Range  ", " Distânc.");
    setCaption(ui->pushButtonZoom, PushButtonWidget, lng, "    Лупа   ", "    Zoom   ", "Elevação");
//    setCaption(ui->pushButtonGridH, PushButtonWidget, lng, "Высоты ", "Altitude", " Altitude");
//    setCaption(ui->pushButtonStrobCY, PushButtonWidget, lng, " Строб   ", "  Strobe ", " Estrobo ");
    setCaption(ui->pushButtonFormular, PushButtonWidget, lng, "  Форм.  ", "    Label   ",  "    Label    ");

    setCaption(ui->groupBoxEcho, GroupBoxWidget, lng, "Яркость", "Brightness", "Luminosidade");
    setCaption(ui->pushButtonEcho, PushButtonWidget, lng, "  Эхо  ", "  Echo  ", "  Eco  ");

    // Н И Ж Н Я Я   П А Н Е Л Ь     Н И Ж Н Я Я   П А Н Е Л Ь     Н И Ж Н Я Я   П А Н Е Л Ь

//    setCaption(ui->groupBoxPLC, GroupBoxWidget, lng, "ПЛК", "PLC", "PLC");
//    setCaption(ui->labelPLCConnection, LabelWidget, lng, "соединение", "connection", "conexão");
//    setCaption(ui->groupBoxPLCupr, GroupBoxWidget, lng, "Режим упр.", "Control Mode", "Regime de controlo");
//    setCaption(ui->labelPLCManual, LabelWidget, lng, "ручной", "manual", "manual");
//    setCaption(ui->labelPLCProgramm, LabelWidget, lng, "прогр.", "progr.", "progr.");

    //  З А К Л А Д К И     З А К Л А Д К И     З А К Л А Д К И     З А К Л А Д К И     З А К Л А Д К И
//    if(lng == english)
//    {
//        ui->tabWidget->setTabText(0, QString("Settings"));
//        ui->tabWidget->setTabText(1, QString("Diagnostics"));
//        ui->tabWidget->setTabText(2, QString("Data Exchange"));
//        ui->tabWidget->setTabText(3, QString("Additional"));
//    }
//    if(lng == russian)
//    {
//        ui->tabWidget->setTabText(0, QString::fromUtf8("Настройки"));
//        ui->tabWidget->setTabText(1, QString::fromUtf8("Диагностика"));
//        ui->tabWidget->setTabText(2, QString::fromUtf8("Обмен АСУ"));
//        ui->tabWidget->setTabText(3, QString::fromUtf8("Дополн."));
//    }
//    if(lng == portuguese)
//    {
//        ui->tabWidget->setTabText(0, QString("  Ajuste  "));
//        ui->tabWidget->setTabText(1, QString("Diagnóstico"));
//        ui->tabWidget->setTabText(2, QString("Troca de dados"));
//        ui->tabWidget->setTabText(3, QString(" Auxiliar "));
//    }

    //  З А К Л А Д К А   "Н А С Т Р О Й К И"

//    setCaption(ui->pushButtonPBL, PushButtonWidget, lng, "  ПБЛ   ", "  SLS   ", "  SLS   ");
//    setCaption(ui->labelAltitude_5, LabelWidget, lng, "весовой коэфф.", "weight ratio", "relação de peso");

//    setCaption(ui->groupBoxLuft, GroupBoxWidget, lng, "Люфт цели", "Slack", "Folga de alvo");

//    setCaption(ui->pushButtonVARY, PushButtonWidget, lng, "  ВАРУ   ", "  TAGC   ", "  CAGT   ");

    // И М И Т А Т О Р   И М И Т А Т О Р   И М И Т А Т О Р   И М И Т А Т О Р   И М И Т А Т О Р   И М И Т А Т О Р   И М И Т А Т О Р

//    setCaption(ui->pushButtonSimulationOnOff, PushButtonWidget, lng, " Имитатор ", "  Simulator ", "  Simulador ");

//    setCaption(ui->pushButtonPAPOn, PushButtonWidget, lng, "  Акт. помеха  ", "    Jamming    ", " Interfer.Ativas");
//    setCaption(ui->labelTargetH, LabelWidget, lng, "высота цели", "flight altitude", "altitude de alvo");

//    ui->dblSpnBxSimulationAltitude->setSuffix(hectoMeters[settings->languageIndex]);

//    setCaption(ui->labelSignalAmpl, LabelWidget, lng, "сигнал цели", "target echo", "sinal de alvo");

//    setCaption(ui->pushButtonTargetOn, PushButtonWidget, lng, "  Цель  ", " Target ", "   Alvo   ");
//    setCaption(ui->labelPAP_E, LabelWidget, lng, "угол места", "elevation angle", "ângulo de elevação");
//    setCaption(ui->labelPAP_Value, LabelWidget, lng, "среднее значение", "average value", "valor médio");

//    setCaption(ui->labelNoise, LabelWidget, lng, "внутр.шумы приёмника", "receiver noise", "ruído de recetor");

//    setCaption(ui->pushButtonASYSimulation, PushButtonWidget, lng, " Команды АСУ->ПРВ", "  CP->HF Commands  ", "Comandos PCA->PRV");
//    setCaption(ui->pushButtonServoDriveSimulation, PushButtonWidget, lng, " Сервопривод ант. ", " Antenna Servodrive", "  Servo  de  antena  ");

//    setCaption(ui->groupBoxGeo, GroupBoxWidget, lng, "Топопривязка и горизонтирование", "Location and leveling", "Localização e posição horizontal");

//    setCaption(ui->groupBoxOrient, GroupBoxWidget, lng, "Ориентирование        ", "Orientation        ", "Orientação       ");
//    if(lng == russian)
//        ui->labelOrientLock->setGeometry(144, 2, 22, 22);
//    else
//        ui->labelOrientLock->setGeometry(124, 4, 22, 22);


//    setCaption(ui->labelTrimbleAzNewValue, LabelWidget, lng, "Новый азимут", "New azimuth", "Azimute novo");
    //setCaption(ui->pushButtonMeters, PushButtonWidget, lng, "    метры", "    meters");
//    setCaption(ui->pushButtonLockTrimbleTracking, PushButtonWidget, lng, "Отслеживать азимут", "Azimuth traking", " Seguimento de azim. ");//ute");
//    if(lng == english)
//        ui->pushButtonLockTrimbleTracking->setGeometry(23, 90, 132, 26);
//    else
//        ui->pushButtonLockTrimbleTracking->setGeometry(8, 90, 166, 26);

//    setCaption(ui->pushButtonApplyNewAzTrimble, PushButtonWidget, lng, "Применить", "Apply", "Aplicar");

    //  З А К Л А Д К А   "Д И А Г Н О С Т И К А"    
//    switch (lng)
//    {
//        case english:       ui->treeWidget->setHeaderLabels(QStringList() << "Rack, unit, element(parameter)" << "Diagnostic message"<< "");  break;
//        case russian:       ui->treeWidget->setHeaderLabels(QStringList() << "Шкаф, блок, элемент(параметр)" << "Диагностическое сообщение"<< "");  break;
//        case portuguese:    ui->treeWidget->setHeaderLabels(QStringList() << "Armário, unidade, elemento(parâmetro)" << "Mensagem diagnóstica"<< "");
//    }

    //  З А К Л А Д К А   "О б м е н   А С У"

//    setCaption(ui->groupBoxHDLC, GroupBoxWidget, lng, "Тип  канала  обмена         ", "Exchange channel type         ", "Tipo de canal de troca         ");

//    setCaption(ui->labelASY2PRV, LabelWidget, lng, "АСУ -> ПРВ________________________________________",
//               "Command post -> Height finder________________________", "PCA -> PRV________________________________________");
//    setCaption(ui->labelPRV2ASY, LabelWidget, lng, "ПРВ -> АСУ________________________________________",
//               "Height finder -> Command post________________________", "PRV -> PCA________________________________________");


//    setCaption(ui->pushButtonFromASYPause, PushButtonWidget, lng, "Пауза", "Pause", "Pausa");
//    setCaption(ui->pushButtonFromPRVPause, PushButtonWidget, lng, "Пауза", "Pause", "Pausa");

    //  З А К Л А Д К А   "Д О П О Л Н И Т Е Л Ь Н О"

    //setCaption(ui->groupBoxIndicatorMode, GroupBoxWidget, lng, "Вид индикатора", "Indicator Mode", "Modo de exibição");
    //setCaption(ui->pushButtonIndicatorE, PushButtonWidget, lng, "Угол м.- Д", " El.angle-R", "Elevação-D");
    //setCaption(ui->pushButtonEncoder, PushButtonWidget, lng, "Угол м.(t)", "El.angle(t)", "Elevação(t)");

//    setCaption(ui->groupBoxUnits, GroupBoxWidget, lng, "Единицы измерения", "Measurment Units", "Unidades de medição");
//    setCaption(ui->pushButtonMeters, PushButtonWidget, lng, "Метры", "Meters", "Metros");
//    setCaption(ui->pushButtonMiles, PushButtonWidget, lng, "Футы-мили", "Feet-Miles", "Pés-milhas");

//    setCaption(ui->groupBoxUPS, GroupBoxWidget, lng, "ИБП", "UPS", "UPS");
//    setCaption(ui->labelUPSConnection, LabelWidget, lng, "соединение", "connection", "conexão");
//    setCaption(ui->labelUPS220V, LabelWidget, lng, "сеть 220V", "input AC 220V", "rede 220V");
//    setCaption(ui->labelCharge, LabelWidget, lng, "Заряд:", "Charge:", "Carga");

    //qDebug()<<"languageChange: "<<this->windowState();

//    if(this->windowState() == Qt::WindowFullScreen || this->windowState() == Qt::WindowNoState)
//    {
//        setCaption(ui->pushButtonFullScreen, PushButtonWidget, lng, "Оконный режим", "Window mode", "Modo de janela");
//    }
//    else
//    {
//        setCaption(ui->pushButtonFullScreen, PushButtonWidget, lng, "Полный экран", "Full screen", "Modo de tela cheia");
//    }
}

//void MainWindow::passwordLanguageChange(int lng)
//{
//    ui->labelPassword->setAlignment(Qt::AlignVCenter);

//    if(fullAccess)
//    {
//        setCaption(ui->pushButtonLogInLogOut, PushButtonWidget, lng, "Выйти", "Log Out", "Sair");
//        setCaption(ui->labelPassword, LabelWidget, lng, "     полный доступ включён", "     full access permitted", "                 acesso permitido");
//    }
//    else
//    {
//        setCaption(ui->pushButtonLogInLogOut, PushButtonWidget, lng, "Войти", "Login", "Login");
//        setCaption(ui->labelPassword, LabelWidget, lng, "    пароль:", "   password:", "palavra-passe:");
//    }
//}



//void MainWindow::on_pushButtonFullScreen_clicked()
//{
//    //this->setWindowState(Qt::WindowFullScreen);
//    //this->setWindowState(Qt::window::WindowMaximized);
//    QString strNormalWin[3] = {"Window mode", "Оконный режим", "Modo de janela"};
//    QString strFullScreen[3] = {"Full screen", "Полный экран", "Modo de tela cheia"};

//    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
//    if(this->windowState() == Qt::WindowFullScreen)
//    {
//        ui->frameWindowTitle->move(0, 0);
//        ui->frameMain->move(0, 28);
//        ui->pushButtonFullScreen->setText(QString::fromUtf8(strNormalWin[settings->languageIndex].toStdString().c_str()));
//    }
//    else
//    {
//        ui->frameWindowTitle->move(0, -30);
//        ui->frameMain->move(0, 0);
//        ui->pushButtonFullScreen->setText(QString::fromUtf8(strFullScreen[settings->languageIndex].toStdString().c_str()));
//    }
//}
