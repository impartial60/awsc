#ifndef SETTINGSC_H
#define SETTINGSC_H
#include <QGraphicsDropShadowEffect>
#include <QSettings>
#include <QDebug>
#include <QDir>

#include "mainWindowC.h"
#include "ui_mainwindow.h"
#include <pwd.h>
//#include <sys/types.h>

//enum language {english, russian, portuguese};

class SETTINGSC
{
    public:
        QSettings *factorySettings;
        QSettings *userSettings;

        QString awsC125absolutePath;
        int languageIndex;
        int zoomIndex;
        int labelBrightnessIndex;

        char ipAddress_awsDispatcher[255];
        ushort port_awsDispatcher;

        SETTINGSC(Ui::MainWindow *ui)
        {
            QDir dir = QDir(QCoreApplication::applicationDirPath());//::applicationFilePath());
            awsC125absolutePath = dir.absolutePath() + "/";
            qDebug()<<"user SETTINGSC awsC125absolutePath:"<<awsC125absolutePath;

            //factorySettings = new QSettings(awsC125absolutePath + "launcherSettings.conf", QSettings::NativeFormat);

            factorySettings = new QSettings(awsC125absolutePath + "awsC125FactorySettings.conf", QSettings::NativeFormat);
            userSettings = new QSettings(awsC125absolutePath + "awsC125UserSettings.conf", QSettings::NativeFormat);

            //password = factorySettings->value("Application/password", "prv16").toString();
            //enterPasswordOnStrart = factorySettings->value("Application/enterPasswordOnStrart", false).toBool();

            assignSettings(ui);

            qDebug()<<"SETTINGSC: languageIndex:"<<languageIndex<<"zoomIndex:"<<zoomIndex<<"labelBrightnessIndex:"<<labelBrightnessIndex
            <<"ipAddress_awsMain:"<<ipAddress_awsDispatcher<<"port_awsMain:"<<port_awsDispatcher;
        }

        void assignSettings(Ui::MainWindow *ui)
        {
            languageIndex = userSettings->value("Application/languageIndex", 0).toInt();
            zoomIndex = userSettings->value("Application/zoomIndex", 0).toInt();
            if(zoomIndex < 0)
                zoomIndex = 0;
            else
                if(zoomIndex > 6)
                    zoomIndex = 6;

            QString strIpAddressAndPortDispatcher = factorySettings->value("IPAddressesAndPorts/ipAddressAndPort_mainServer", "127.0.0.1:30016").toString();
            QStringList strListIpAddrPortDispatcher = strIpAddressAndPortDispatcher.split(":");

            if(strListIpAddrPortDispatcher.size() == 2)
            {
                strcpy(ipAddress_awsDispatcher,  strListIpAddrPortDispatcher[0].toStdString().c_str());
                bool ok;
                port_awsDispatcher = strListIpAddrPortDispatcher[1].toUShort(&ok, 10);
            }


            ui->sliderZoom->setValue(zoomIndex);

            labelBrightnessIndex = userSettings->value("Application/labelBrightnessIndex", 7).toInt();
            if(labelBrightnessIndex < 0)
                labelBrightnessIndex = 0;
            else
                if(labelBrightnessIndex > 23)
                    labelBrightnessIndex = 23;

            ui->sliderGridBrightness->setValue(labelBrightnessIndex);

            switch (languageIndex)
            {
                case 0: ui->pushButtonEnglish->setChecked(true);  break;
                case 1: ui->pushButtonRussian->setChecked(true);  break;
                case 2: ui->pushButtonPortug->setChecked(true);
            }
        }

        void saveToFile(Ui::MainWindow *ui)
        {
            qDebug()<<"saveToFile: languageIndex:"<<languageIndex;

            userSettings->setValue("Application/labelBrightnessIndex", QString::number(labelBrightnessIndex));
            userSettings->setValue("Application/languageIndex", QString::number(languageIndex));
            userSettings->setValue("Application/zoomIndex", QString::number(zoomIndex));

//            userSettings->setValue("Application/indicatorEchoBrightnessIndex", QString::number(ui->sliderEchoBrightness->value()));
//            userSettings->setValue("Application/indicatorGridBrightnessIndex", QString::number(ui->sliderGridBrightness->value()));

//            userSettings->setValue("Application/isHDLCUser", ui->pushButtonHDLC->isChecked());

//            userSettings->setValue("Application/isMetric", ui->pushButtonMeters->isChecked());

//            userSettings->setValue("Application/mainIndicatorScaleIndex", QString::number(ui->sliderZoom->value()));

//            userSettings->setValue("Application/mainIndicatorZoomH1", QString::number(ui->sliderZoomH1->value()));
//            userSettings->setValue("Application/mainIndicatorZoomH2", QString::number(ui->sliderZoomH2->value()));
//            userSettings->setValue("Application/mainIndicatorZoomH3", QString::number(ui->sliderZoomH3->value()));
//            userSettings->setValue("Application/mainIndicatorZoomH4", QString::number(ui->sliderZoomH4->value()));

//            userSettings->setValue("Application/targetLuft", QString::number(ui->dblSpnBxLuft->value()));
//            userSettings->setValue("Application/trimbleAzimuthCorrection", QString::number(popravkaAzimutaTrimble));

//            userSettings->setValue("Detection/Threshold", QString::number(ui->dblSpnBxPorog->value()));
//            userSettings->setValue("Detection/JammingThreshold", QString::number(ui->dblSpnBxPorogPAP->value()));
//            userSettings->setValue("Detection/TargetTotalNumber", QString::number(ui->dblSpnBxMaxTargetNumber->value()));
//            userSettings->setValue("Detection/PBLisOn", ui->pushButtonPBL->isChecked());
//            userSettings->setValue("Detection/PBLValue", QString::number(ui->dblSpinBoxPBL->value()));
//            userSettings->setValue("Detection/StrobWidth", QString::number(ui->dblSpnBxStrob->value()));

            userSettings->sync();
        }


};

#endif // SETTINGSC_H
