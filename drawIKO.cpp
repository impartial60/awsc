#include <QtGui>
#include "mainWindowC.h"
#include "ui_mainwindow.h"

//АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЪЭЮЯ
//абвгде'жзийклмнопрстуфхцчшщьъэюя

float zoomRanges[] =   {  30.0, 50.0, 70.0, 100.0, 150.0, 200.0, 300.0};

int numRangesRings[] =   {   6,   10,    7,    10,    15,     4,     6};

float ringsDelta[] =     { 5.0,  5.0, 10.0,  10.0,  10.0,  50.0,  50.0};

int fatLineDenominator[] = { 2,    2,    5,     5,     5,     2,     2};

int captionDenominator[] = { 2,    2,    1,     5,     5,     2,     2};


QColor gridColors[] = {
    QColor(0, 0, 0, 0),    QColor(16, 32, 24, 76),    QColor(24, 48, 36, 80),   QColor(28, 52, 40, 96), QColor(32, 56, 44, 112),
    QColor(32, 64, 48, 128), QColor(40, 72, 54, 132), QColor(48, 80, 64, 144),  QColor(56, 88, 72, 152), QColor(64, 112, 80, 160),

    QColor(64, 128, 96, 176), QColor(64, 128, 96, 192), QColor(64, 128, 96, 208),  QColor(64, 128, 96, 224), QColor(64, 128, 96, 240),
    QColor(64, 128, 96, 255), QColor(72, 136, 102, 255), QColor(80, 144, 104, 255), QColor(88, 152, 112, 255), QColor(96, 160, 120, 255),

    QColor(112, 176, 132, 255), QColor(144, 240, 208, 255), QColor(192, 255, 224, 255), QColor(255, 255, 255, 255)
};

//QPen penSoprKnot(QColor::fromRgbF(1, 0.25, 1, 1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//QPen penZavKnot(QColor::fromRgbF(0.75, 0.5, 0.5, 1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

//QPen penSoprTrassa(QColor::fromRgbF(1, 0.25, 1, 0.1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//QPen penZavTrassa(QColor::fromRgbF(0.75, 0.5, 0.5, 0.1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

//float redFi1 = 0.5;
//float bluFi1 = 0.8;

float cntrNetVibranihCelej = 0;

void MainWindow::IKOClick(QMouseEvent *ev)
{            
    if(ev->button() == Qt::LeftButton)  //  клик левой кнопкой
    {        
//        qDebug()<<"***   MouseClickSlot: komKomC.targetIndexCY:"<<komKomC.targetIndexCY
//                <<"target_IKO_MouseMove_Number:"<<target_IKO_MouseMove_Number<<"ev->x():"<<ev->x()<<"ev->y():"<<ev->y();

        float dAz;

        if(target_IKO_MouseMove_Number > - 1)   // команда захватить на сопровождение "отождествлённую" цель
        {
            if(console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_A || console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_K)
            {
                komKomC.targetIndexCY = simTrgt_IKO_MouseMove_Number;
                if(simTrgt_IKO_MouseMove_Number > - 1 && simTrgt_IKO_MouseMove_Number < 256)
                    dAz = console2User.apyInfo.nextTocka[simTrgt_IKO_MouseMove_Number].azimuth360 - console2User.srvDrvInfo.directrisaAzimuth;
                else
                    qDebug()<<"***   MouseClickSlot: simTrgt_IKO_MouseMove_Number:"<<simTrgt_IKO_MouseMove_Number;

            }
            else
            {
                komKomC.targetIndexCY = target_IKO_MouseMove_Number;
                dAz = console2User.primaryPlots[target_IKO_MouseMove_Number].azimuth360 - console2User.srvDrvInfo.directrisaAzimuth;
            }

            cntrNetVibranihCelej = 0;
        }
        else    // команда изменить только директрису УНВ
        {
            cntrNetVibranihCelej = 2;

            dAz = mouseCursorAzimuth - console2User.srvDrvInfo.directrisaAzimuth;

            ui->IKO->setCursor(Qt::WaitCursor);
        }

        if(dAz < -180.0)
            dAz += 360.0;

        if(dAz > 180.0)
            dAz -= 360.0;

        qDebug()<<"***   MouseClickSlot: dAz:"<<dAz<<"mouseCursorAzimuth:"<<mouseCursorAzimuth<<"console2User.srvDrvInfo.directrisaAzimuth:"<<console2User.srvDrvInfo.directrisaAzimuth;

    //if(ui->azimuthScale360->controlModifierIsHoldingDown == false && ui->azimuthScale3600->controlModifierIsHoldingDown == false)        {

        komKomC.srv[AZIMUTH].delta = dAz;
        if(komKomC.srv[AZIMUTH].executeCounter % 10 == 0)
            komKomC.srv[AZIMUTH].executeCounter += 17;

    }
    else    //  клик правой кнопкой - приклеить/отклеить положение электронной лупы
    {
        zoomIKOfreez = !zoomIKOfreez;

        if(zoomIKOfreez)
            ui->IKO->//setCursor(QCursor::shape()            );
                setCursor(Qt::PointingHandCursor);//::ArrowCursor);//::IBeamCursor);
        else
            ui->IKO->setCursor(Qt::ArrowCursor);//::IBeamCursor);
    }
}

void MainWindow::drawTextHalo(QPainter *painter, QPen textPen, QFont textFont, int x, int y, QString text)
{
    painter->setFont(textFont);
    //QFontMetrics fMetrics = painter.fontMetrics();
    //QSize sz = fMetrics.size(Qt::TextSingleLine, text);

    //QPen blackPen((Qt::black);
    //painter->setPen(blackPen);
    painter->setPen(QColor(0,0,0,255));

    int d = 1;
    painter->drawText(x - d, y - d, text);
    painter->drawText(x + d, y - d, text);
    painter->drawText(x - d, y + d, text);
    painter->drawText(x + d, y + d, text);

    painter->drawText(x    , y - d, text);
    painter->drawText(x    , y - d, text);
    painter->drawText(x - d, y    , text);
    painter->drawText(x + d, y    , text);

    painter->setPen(textPen);
    painter->drawText(x,y,text);
}


void MainWindow::drawFormularC(QPainter *painter, bool _isZoomIKO, int plotInd)
{
    float radiusIKO = centerMainIKO.x() - 30;
    QPoint centerIKO = centerMainIKO;

    float factorR_IKO;

    if(_isZoomIKO)
    {
        radiusIKO = centerZoomIKO.x() - 120;
        factorR_IKO_Zoom = 1.0 / zoomRanges[ui->sliderZoom->value()] * radiusIKO;
        factorR_IKO = factorR_IKO_Zoom;
        centerIKO = centerZoomIKO;
    }
    else
    {
        factorR_IKO_Main = 1.0 / zoomRanges[ui->sliderZoom->value()] * radiusIKO;
        factorR_IKO = factorR_IKO_Main;
    }

    float trgtR = console2User.primaryPlots[plotInd].range_km * factorR_IKO;

    //strD[j] = QString::number(console2User.primaryPlots[j].range_km, 'f', 2) + QString::fromUtf8("км");
    strI[plotInd] = QString::number(plotInd + 1);

    int rgValue = 128;
    if(plotInd == target_IKO_MouseMove_Number)
        rgValue = 224;

    painter->setPen(QPen(QColor(rgValue, rgValue, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    //drawFormularEB(painter, j);

    // С Т Р О Б   Ц Е Л И   И   Ф О Р М У Л Я Р
    //if(yellowBrightness[i] > 128)
    {
        //QPen strobPen(QColor(yellowBrightness[i] / 4, yellowBrightness[i] / 4, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        //QPen strobPen(QColor(160, 160, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);

        //painter->setPen(strobPen);

        int screenStrobDeltaR = 8;
        if(_isZoomIKO)
            screenStrobDeltaR = 16;

        float rangeTrgt1 = trgtR - screenStrobDeltaR;
        float rangeTrgt2 = trgtR + screenStrobDeltaR;

        float plt_nev_delta_b = 3.0;
        if(_isZoomIKO)
            plt_nev_delta_b = 2.0;

        float azTrgtRad1 = (console2User.primaryPlots[plotInd].azimuth360 + plt_nev_delta_b) / Rad2Grad;
        float azTrgtRad2 = (console2User.primaryPlots[plotInd].azimuth360 - plt_nev_delta_b) / Rad2Grad;

        float dX1Trgt =  centerIKO.x() + rangeTrgt1 * sin(azTrgtRad1);
        float dY1Trgt =  centerIKO.y() - rangeTrgt1 * cos(azTrgtRad1);

        float dX2Trgt =  centerIKO.x() + rangeTrgt2 * sin(azTrgtRad1);
        float dY2Trgt =  centerIKO.y() - rangeTrgt2 * cos(azTrgtRad1);

        painter->drawLine(dX1Trgt , dY1Trgt, dX2Trgt , dY2Trgt);

        dX1Trgt =  centerIKO.x() + rangeTrgt1 * sin(azTrgtRad2);
        dY1Trgt =  centerIKO.y() - rangeTrgt1 * cos(azTrgtRad2);

        dX2Trgt =  centerIKO.x() + rangeTrgt2 * sin(azTrgtRad2);
        dY2Trgt =  centerIKO.y() - rangeTrgt2 * cos(azTrgtRad2);

        painter->drawLine(dX1Trgt , dY1Trgt, dX2Trgt , dY2Trgt);

        float azTrgtGrad = 90.0 - (console2User.primaryPlots[plotInd].azimuth360 + plt_nev_delta_b);

        if(azTrgtGrad < 0.0)
            azTrgtGrad += 360.0;

        if(azTrgtGrad >= 360.0)
            azTrgtGrad -= 360.0;

        azTrgtGrad *= 16;

        float spanAngle = 2 * plt_nev_delta_b * 16;

        painter->drawArc(QRectF(centerIKO.x() - rangeTrgt1, centerIKO.y() - rangeTrgt1, 2 * rangeTrgt1, 2 * rangeTrgt1), azTrgtGrad, spanAngle);
        painter->drawArc(QRectF(centerIKO.x() - rangeTrgt2, centerIKO.y() - rangeTrgt2, 2 * rangeTrgt2, 2 * rangeTrgt2), azTrgtGrad, spanAngle);

        if(plotInd != target_IKO_MouseMove_Number)
        {
            int yelBr = 80;//yellowBrightness[i] / 4;

//                    if(target_IKO_MouseMove_Number > -1)
//                        yelBr = yellowBrightness[i] / 6;

            QPen pen(QColor(yelBr, yelBr, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            float nextTocka_azimuthRad = console2User.primaryPlots[plotInd].azimuth360 / Rad2Grad;

            int formularX =  centerIKO.x() + trgtR * sin(nextTocka_azimuthRad) + 20; // 20 - смещение
            int formularY =  centerIKO.y() - trgtR * cos(nextTocka_azimuthRad);

            drawTextHalo(painter, pen, painter->font(), formularX, formularY - 10, strI[plotInd]);
            //drawTextHalo(painter, pen, painter->font(), formularX, formularY +  5, strH[i]);
            //drawTextHalo(painter, pen, painter->font(), formularX, formularY + 20, strK[i]);
            //drawTextHalo(painter, pen, painter->font(), formularX, formularY + 35, strV[i]);
        }

    }

}


void MainWindow::drawIKO(bool isZoomIKO, QPainter *painter)
{        
    float radiusIKO = centerMainIKO.x() - 30;
    QPoint centerIKO = centerMainIKO;

    painter->setBrush(QBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 1.0)));//painter.setBrush(QBrush(QColor::fromRgbF(0, 0, 0, 0.03)));

    QFont fnt = painter->font();
    float factorR_IKO;

    if(isZoomIKO)
    {
        radiusIKO = centerZoomIKO.x() - 120;
        factorR_IKO_Zoom = 1.0 / zoomRanges[ui->sliderZoom->value()] * radiusIKO;
        factorR_IKO = factorR_IKO_Zoom;
        fnt.setPointSize(10);
        centerIKO = centerZoomIKO;
        //centerMainIKO = QPoint(ikoZoomBMP.width() / 2, ikoZoomBMP.height() / 2);

        if(console2User.apyInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_BSP && console2User.apyInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_MSP)
            painter->fillRect(0, 0, ikoZoomBMP.width(), ikoZoomBMP.height(), Qt::black);
    }
    else
    {
        factorR_IKO_Main = 1.0 / zoomRanges[ui->sliderZoom->value()] * radiusIKO;
        factorR_IKO = factorR_IKO_Main;
        fnt.setPointSize(8);

        if(console2User.apyInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_BSP && console2User.apyInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_MSP)
            painter->fillRect(0, 0, ikoBMP.width(), ikoBMP.height(), Qt::black);
    }


    painter->setFont(fnt);

    painter->setBrush(Qt::NoBrush);

    int echoBrightnessInd = ui->sliderGridBrightness->value();

    float textRadiusIKO = radiusIKO + 12.0;

    if(ui->pushButtonGridAz->isChecked())
    {
        for(int i = 0; i < 36; i++)
        {
            painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

            float azRadians = i * 10 / Rad2Grad;
            float dXIKO =    radiusIKO * sin(azRadians);
            float dYIKO =  - radiusIKO * cos(azRadians);
            QPoint borderPointIKO = centerIKO + QPoint(dXIKO, dYIKO);

            float tdXIKO =    textRadiusIKO * sin(azRadians);
            float tdYIKO =  - textRadiusIKO * cos(azRadians);

            QString strAz = QString::fromUtf8("           ") + QString::number(i * 10) + QString::fromUtf8("°         ");
            if(i == 0)
                strAz = QString::fromUtf8("0");

            QFontMetrics fMetrics = painter->fontMetrics();
            QSize sz = fMetrics.size(Qt::TextSingleLine, strAz);

            QPoint textPointIKO = centerIKO + QPoint(tdXIKO - sz.width() / 2, tdYIKO + 5);// - sz.height() / 2);

            painter->drawText(textPointIKO, strAz);

            if(i % 3 != 0)
                painter->setPen(QPen(QColor (gridColors[echoBrightnessInd].red() / 2, gridColors[echoBrightnessInd].green() / 2, gridColors[echoBrightnessInd].blue() / 2), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

            painter->drawLine(centerIKO, borderPointIKO);
        }
    }

    if(ui->pushButtonGridR->isChecked())
    {
        for(int i = 1; i <= numRangesRings[ui->sliderZoom->value()]; i++)//for(int i = 0; i < 10; i++)
        {
            float radiusNext = factorR_IKO * ringsDelta[ui->sliderZoom->value()] * i; //float radiusNext = factorR * 10.0 * i;

            //int fatLineDenominator = { 2,    2,    5,     5,     1,     2,     2};
            //int captionDenominator = { 2,    2,    1,     1,     1,     2,     2};

            if(i % fatLineDenominator[ui->sliderZoom->value()] == 0)
                painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            else
                painter->setPen(QPen(QColor (gridColors[echoBrightnessInd].red() / 2, gridColors[echoBrightnessInd].green() / 2, gridColors[echoBrightnessInd].blue() / 2), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

              //  painter.setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

//             QColor clr(gridColors[echoBrightnessInd]);
//             clr.setAlpha(128);

            //QColor clr2(gridColors[echoBrightnessInd].red() / 2, gridColors[echoBrightnessInd].green() / 2, gridColors[echoBrightnessInd].blue() / 2);

             //if(i % fatLineDenominator[ui->sliderZoom->value()] != 0)
               //  painter.setPen(QPen(clr2, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                 //painter.pen().color().setAlpha(32);

            //qDebug()<<"drawIKO: i:"<<i<<"radiusNext:"<<radiusNext;
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(centerIKO, (int)radiusNext, (int)radiusNext);

            if(i % captionDenominator[ui->sliderZoom->value()] == 0)// > 0)
            {
                QString strR = " " + QString::number(ringsDelta[ui->sliderZoom->value()] * i);// + QString::fromUtf8(" km");
                if(i == numRangesRings[ui->sliderZoom->value()])
                    strR = " " + QString::number(ringsDelta[ui->sliderZoom->value()] * i) + QString::fromUtf8(" km");

                QFontMetrics fMetrics = painter->fontMetrics();
                QSize sz = fMetrics.size(Qt::TextSingleLine, strR);

                float azRadians = 45.0 / Rad2Grad;

                float dXIKO =    radiusNext * sin(azRadians);
                float dYIKO =  - radiusNext * cos(azRadians);

                int intX = centerIKO.x() + dXIKO - sz.width() / 2;
                int intY = centerIKO.y() + dYIKO + 2;

                painter->setBrush(QBrush(Qt::black));
                painter->setPen(Qt::NoPen);
                painter->drawRect(intX, intY - 10, sz.width(), 12);

                //painter.setBrush(QBrush() ::NoBrush);
                painter->setBrush(QBrush(QColor::fromRgb(0, 0, 0)));

                painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter->drawText(intX, intY, strR);
                painter->drawText(intX, intY, strR);

                //drawTextHalo(&painter, painter.pen(), fontE, intX, intY, strR);
            }
        }
    }

    painter->setRenderHint(QPainter::Antialiasing, false);

    // Д И Р Е К Т Р И С А
    painter->setPen(QPen(QColor::fromRgbF(0, 0.25, 1.0, 0.51), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    float azRadians = console2User.srvDrvInfo.currentAzimuth / Rad2Grad;//float azRadians = arm->azimuthPRV_CURRENT / Rad2Grad;
    float dXIKO =    radiusIKO * sin(azRadians);
    float dYIKO =  - radiusIKO * cos(azRadians);
    QPoint borderPointIKO = centerIKO + QPoint(dXIKO / 1, dYIKO / 1);
    painter->drawLine(centerIKO, borderPointIKO);

    // П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18   П - 18
    if(ui->pushButtonUseP18->isChecked())
        drawP18(isZoomIKO, painter, centerIKO, factorR_IKO, radiusIKO);

    //   И С Т И Н Н А Я   Т Р А С С А   Ц Е Л И          И С Т И Н Н А Я   Т Р А С С А   Ц Е Л И          И С Т И Н Н А Я   Т Р А С С А   Ц Е Л И
    if(ui->pushButtonShowTrack->isChecked())
    {
        painter->setPen(QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.9));

        for(int j = 0; j < console2User.apyInfo.totalSimTargetNum; j++)
        {
            for(int i = 0; i < monoTrackP18[j].tailSize; i++)
            {
                float dXTrgt =   monoTrackP18[j].tail[i].groundX * factorR_IKO;
                float dYTrgt = - monoTrackP18[j].tail[i].groundY * factorR_IKO;

                QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

                if(isZoomIKO)
                {
//                    if(i == monoTrackP18[j].tailSize - 1)//if(i == monoTrackPointsCount[j] - 1)
//                        painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, QColor::fromRgbF(redFi1, 0.8, bluFi1, 0.9));
//                    else
                        painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.9));
                }
                else
                {
//                    if(i == monoTrackP18[j].tailSize - 1)//if(i == monoTrackPointsCount[j] - 1)
//                        painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, QColor::fromRgbF(redFi1, 0.8, bluFi1, 0.9));
//                    else
                        painter->drawPoint(trgtPointIKO);
                }
            }
        }
    }

    if(ui->pushButtonShowTarget->isChecked())
    {
        for(int j = 0; j < console2User.apyInfo.totalSimTargetNum; j++)
        {            
            float azTrgtRadians1 = 90 - (monoTrackP18[j].head.azimuth360 + 1); //  float azTrgtRadians1 = 90 - (monoTrack[j][monoTrackPointsCount[j] - 1].azimuth360 + 1);

            if(azTrgtRadians1 < 0)
                azTrgtRadians1 += 360;
            azTrgtRadians1 *= 16;

            float dXTrgt =   monoTrackP18[j].head.groundX * factorR_IKO;
            float dYTrgt = - monoTrackP18[j].head.groundY * factorR_IKO;

            QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

            if(isZoomIKO)
            {
                 painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, QColor::fromRgbF(redFi1, 0.8, bluFi1, 0.9));
            }
            else
            {
                painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, QColor::fromRgbF(redFi1, 0.8, bluFi1, 0.9));
            }
        }
    }

//по новому   (c  М Д О - 1)
    if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
    {
        //  Ф О Р М У Л Я Р Ы   Ц Е Л Е Й   по новому   (c  М Д О - 1)
        //int rgValue = 128;

        for(int j = 0; j < MAX_MDO1_PRIMARY_PLOTS_COUNT; j++)
        {
            if(console2User.primaryPlots[j].packageWeight == 0)
                break;

            if(j == target_IKO_MouseMove_Number)
                continue;

            drawFormularC(painter, isZoomIKO, j);
        }
        if(target_IKO_MouseMove_Number > -1)
        {
            drawFormularC(painter, isZoomIKO, target_IKO_MouseMove_Number);

//            QPen pen(QColor(Qt::yellow), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

//            float nextTocka_azimuthRad = nextTocka_azimuth360[target_IKO_MouseMove_Number] / Rad2Grad;

//            float trgtR = nextTocka_slantRange_km[target_IKO_MouseMove_Number] * factorR_IKO;

//            int formularX =  centerIKO.x() + trgtR * sin(nextTocka_azimuthRad) + 20; // 20 - смещение
//            int formularY =  centerIKO.y() - trgtR * cos(nextTocka_azimuthRad);

//            drawTextHalo(painter, pen, painter->font(), formularX, formularY - 10, strI[target_IKO_MouseMove_Number]);
//            drawTextHalo(painter, pen, painter->font(), formularX, formularY +  5, strH[target_IKO_MouseMove_Number]);
//            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 20, strK[target_IKO_MouseMove_Number]);
//            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 35, strV[target_IKO_MouseMove_Number]);
        }

    }
    else
        if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE && console2User.apyInfo.targetIndexCY >=0 && console2User.apyInfo.targetIndexCY < MAXTOCKANUM)
        {
            float azTrgtRadians1 = 90 - (console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].azimuth360 + 1);

            if(azTrgtRadians1 < 0)
                azTrgtRadians1 += 360;
            azTrgtRadians1 *= 16;

            float dXTrgt =   console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].groundX * factorR_IKO;
            float dYTrgt = - console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].groundY * factorR_IKO;

            QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

            if(isZoomIKO)
                 painter->fillRect(trgtPointIKO.x() - 3, trgtPointIKO.y() - 3, 5, 5, QColor::fromRgbF(1.0, 1.0, 0, 1.0));
            else
                painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, QColor::fromRgbF(1.0, 1.0, 0, 1.0));

            QPen pen(QColor(255, 255, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            float nextTocka_azimuthRad = nextTocka_azimuth360[console2User.apyInfo.targetIndexCY] / Rad2Grad;
            float trgtR = nextTocka_slantRange_km[console2User.apyInfo.targetIndexCY] * factorR_IKO;
            int formularX =  centerIKO.x() + trgtR * sin(nextTocka_azimuthRad) + 20; // 20 - смещение
            int formularY =  centerIKO.y() - trgtR * cos(nextTocka_azimuthRad);

            drawTextHalo(painter, pen, painter->font(), formularX, formularY - 10, strI[console2User.apyInfo.targetIndexCY]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY +  5, strH[console2User.apyInfo.targetIndexCY]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 20, strK[console2User.apyInfo.targetIndexCY]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 35, strV[console2User.apyInfo.targetIndexCY]);
        }


#if 0   //по старому   (c   АРМ руководителя тренировки)
    if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
    {
        for(int i = console2User.apyInfo.totalSimTargetNum - 1; i >= 0; i--)
        {
            if(nextTocka_elevAngle[i] < console2User.srvDrvInfo.currentDirectrisaElevationAngle - 5 || nextTocka_elevAngle[i] > console2User.srvDrvInfo.currentDirectrisaElevationAngle + 5)
                continue;   //      цель за пределами сектора сканирования по углу

            float trgtR = nextTocka_slantRange_km[i] * factorR_IKO;

            //  Д У Ж К А   П А Ч К И   Ц Е Л И
            if(greenBrightness > 0)
            {
                painter->setPen(QPen(QColor(0, greenBrightness[i], 0)));

                float azTrgtRadians1 = 90 - (nextTocka_azimuth360[i] + 0.5);// + 1);

                if(azTrgtRadians1 < 0)
                    azTrgtRadians1 += 360;
                azTrgtRadians1 *= 16;

                painter->drawArc(QRectF(centerIKO.x() - trgtR, centerIKO.y() - trgtR, 2 * trgtR, 2 * trgtR), azTrgtRadians1, 1 * 16);
            }

            // С Т Р О Б   Ц Е Л И   И   Ф О Р М У Л Я Р
            if(yellowBrightness[i] > 128)
            {
                QPen strobPen(QColor(yellowBrightness[i] / 4, yellowBrightness[i] / 4, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);

                painter->setPen(strobPen);

                int screenStrobDeltaR = 8;
                if(isZoomIKO)
                    screenStrobDeltaR = 16;

                float rangeTrgt1 = trgtR - screenStrobDeltaR;
                float rangeTrgt2 = trgtR + screenStrobDeltaR;

                float plt_nev_delta_b = 3.0;
                if(isZoomIKO)
                    plt_nev_delta_b = 2.0;

                float azTrgtRad1 = (nextTocka_azimuth360[i] + plt_nev_delta_b) / Rad2Grad;
                float azTrgtRad2 = (nextTocka_azimuth360[i] - plt_nev_delta_b) / Rad2Grad;

                float dX1Trgt =  centerIKO.x() + rangeTrgt1 * sin(azTrgtRad1);
                float dY1Trgt =  centerIKO.y() - rangeTrgt1 * cos(azTrgtRad1);

                float dX2Trgt =  centerIKO.x() + rangeTrgt2 * sin(azTrgtRad1);
                float dY2Trgt =  centerIKO.y() - rangeTrgt2 * cos(azTrgtRad1);

                painter->drawLine(dX1Trgt , dY1Trgt, dX2Trgt , dY2Trgt);

                dX1Trgt =  centerIKO.x() + rangeTrgt1 * sin(azTrgtRad2);
                dY1Trgt =  centerIKO.y() - rangeTrgt1 * cos(azTrgtRad2);

                dX2Trgt =  centerIKO.x() + rangeTrgt2 * sin(azTrgtRad2);
                dY2Trgt =  centerIKO.y() - rangeTrgt2 * cos(azTrgtRad2);

                painter->drawLine(dX1Trgt , dY1Trgt, dX2Trgt , dY2Trgt);

                float azTrgtGrad = 90.0 - (nextTocka_azimuth360[i] + plt_nev_delta_b);

                if(azTrgtGrad < 0.0)
                    azTrgtGrad += 360.0;

                if(azTrgtGrad >= 360.0)
                    azTrgtGrad -= 360.0;

                azTrgtGrad *= 16;

                float spanAngle = 2 * plt_nev_delta_b * 16;

                painter->drawArc(QRectF(centerIKO.x() - rangeTrgt1, centerIKO.y() - rangeTrgt1, 2 * rangeTrgt1, 2 * rangeTrgt1), azTrgtGrad, spanAngle);
                painter->drawArc(QRectF(centerIKO.x() - rangeTrgt2, centerIKO.y() - rangeTrgt2, 2 * rangeTrgt2, 2 * rangeTrgt2), azTrgtGrad, spanAngle);

                if(i != target_IKO_MouseMove_Number)
                {
                    int yelBr = yellowBrightness[i] / 4;

                    if(target_IKO_MouseMove_Number > -1)
                        yelBr = yellowBrightness[i] / 6;

                    QPen pen(QColor(yelBr, yelBr, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

                    float nextTocka_azimuthRad = nextTocka_azimuth360[i] / Rad2Grad;

                    int formularX =  centerIKO.x() + trgtR * sin(nextTocka_azimuthRad) + 20; // 20 - смещение
                    int formularY =  centerIKO.y() - trgtR * cos(nextTocka_azimuthRad);

                    drawTextHalo(painter, pen, painter->font(), formularX, formularY - 10, strI[i]);
                    drawTextHalo(painter, pen, painter->font(), formularX, formularY +  5, strH[i]);
                    drawTextHalo(painter, pen, painter->font(), formularX, formularY + 20, strK[i]);
                    drawTextHalo(painter, pen, painter->font(), formularX, formularY + 35, strV[i]);
                }
            }
        }
        if(target_IKO_MouseMove_Number > -1)
        {
            QPen pen(QColor(Qt::yellow), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            float nextTocka_azimuthRad = nextTocka_azimuth360[target_IKO_MouseMove_Number] / Rad2Grad;

            float trgtR = nextTocka_slantRange_km[target_IKO_MouseMove_Number] * factorR_IKO;

            int formularX =  centerIKO.x() + trgtR * sin(nextTocka_azimuthRad) + 20; // 20 - смещение
            int formularY =  centerIKO.y() - trgtR * cos(nextTocka_azimuthRad);

            drawTextHalo(painter, pen, painter->font(), formularX, formularY - 10, strI[target_IKO_MouseMove_Number]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY +  5, strH[target_IKO_MouseMove_Number]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 20, strK[target_IKO_MouseMove_Number]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 35, strV[target_IKO_MouseMove_Number]);
        }
    }
    else
        if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE && console2User.apyInfo.targetIndexCY >=0 && console2User.apyInfo.targetIndexCY < MAXTOCKANUM)
        {
            float azTrgtRadians1 = 90 - (console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].azimuth360 + 1);

            if(azTrgtRadians1 < 0)
                azTrgtRadians1 += 360;
            azTrgtRadians1 *= 16;

            float dXTrgt =   console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].groundX * factorR_IKO;
            float dYTrgt = - console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].groundY * factorR_IKO;

            QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

            if(isZoomIKO)
                 painter->fillRect(trgtPointIKO.x() - 3, trgtPointIKO.y() - 3, 5, 5, QColor::fromRgbF(1.0, 1.0, 0, 1.0));
            else
                painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, QColor::fromRgbF(1.0, 1.0, 0, 1.0));

            QPen pen(QColor(255, 255, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            float nextTocka_azimuthRad = nextTocka_azimuth360[console2User.apyInfo.targetIndexCY] / Rad2Grad;
            float trgtR = nextTocka_slantRange_km[console2User.apyInfo.targetIndexCY] * factorR_IKO;
            int formularX =  centerIKO.x() + trgtR * sin(nextTocka_azimuthRad) + 20; // 20 - смещение
            int formularY =  centerIKO.y() - trgtR * cos(nextTocka_azimuthRad);

            drawTextHalo(painter, pen, painter->font(), formularX, formularY - 10, strI[console2User.apyInfo.targetIndexCY]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY +  5, strH[console2User.apyInfo.targetIndexCY]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 20, strK[console2User.apyInfo.targetIndexCY]);
            drawTextHalo(painter, pen, painter->font(), formularX, formularY + 35, strV[console2User.apyInfo.targetIndexCY]);
        }
#endif



    // Ц Е Л И   К О М П Л Е К С Н О Г О   Т Р Е Н А Ж А
    if(komKomC.rejimRabotiZRK == RejimRabotiZRK::TR_K)
    {
        int scrVKP_K1_x = centerIKO.x() + console2User.vkp2zrk.coordYHX.x_km * factorR_IKO;
        int scrVKP_K1_y = centerIKO.y() - console2User.vkp2zrk.coordYHX.y_km * factorR_IKO;

        int scrVKP_T1_x = centerIKO.x() + console2User.vkp2zrk.cmplxYHX[0].x_km * factorR_IKO;
        int scrVKP_T1_y = centerIKO.y() - console2User.vkp2zrk.cmplxYHX[0].y_km * factorR_IKO;

        int scrVKP_T2_x = centerIKO.x() + console2User.vkp2zrk.cmplxYHX[1].x_km * factorR_IKO;
        int scrVKP_T2_y = centerIKO.y() - console2User.vkp2zrk.cmplxYHX[1].y_km * factorR_IKO;

        int scrVKP_T3_x = centerIKO.x() + console2User.vkp2zrk.cmplxYHX[2].x_km * factorR_IKO;
        int scrVKP_T3_y = centerIKO.y() - console2User.vkp2zrk.cmplxYHX[2].y_km * factorR_IKO;

        painter->fillRect(scrVKP_K1_x - 4, scrVKP_K1_y - 4, 8, 8, QColor::fromRgbF(1.0, 0.5, 0, 1.0));

        painter->fillRect(scrVKP_T1_x - 2, scrVKP_T1_y - 2, 4, 4, QColor::fromRgbF(1.0, 0, 0.5, 1.0));
        painter->fillRect(scrVKP_T2_x - 2, scrVKP_T2_y - 2, 4, 4, QColor::fromRgbF(1.0, 0, 0.5, 1.0));
        painter->fillRect(scrVKP_T3_x - 2, scrVKP_T3_y - 2, 4, 4, QColor::fromRgbF(1.0, 0, 0.5, 1.0));

        drawTextHalo(painter, QPen(QColor::fromRgbF(1.0, 0, 0.5, 1.0), 1), painter->font(), scrVKP_T1_x + 12, scrVKP_T1_y - 2, "1: " + QString::number(console2User.vkp2zrk.cmplxTrgtN[0]));
        drawTextHalo(painter, QPen(QColor::fromRgbF(1.0, 0, 0.5, 1.0), 1), painter->font(), scrVKP_T2_x + 12, scrVKP_T2_y - 2, "2: " + QString::number(console2User.vkp2zrk.cmplxTrgtN[1]));
        drawTextHalo(painter, QPen(QColor::fromRgbF(1.0, 0, 0.5, 1.0), 1), painter->font(), scrVKP_T3_x + 12, scrVKP_T3_y - 2, "3: " + QString::number(console2User.vkp2zrk.cmplxTrgtN[2]));
    }
}


void MainWindow::drawMainIKO()
{
    settings->zoomIndex = ui->sliderZoom->value();
    settings->labelBrightnessIndex = ui->sliderGridBrightness->value();

    QPainter painter(&ikoBMP);

    QFont fontE("Arial");
    //fontE.setPointSize(8);
    fontE.setBold(false);
    fontE.setStyleStrategy(QFont::NoAntialias);
    painter.setFont( fontE );

    int echoBrightnessInd = ui->sliderGridBrightness->value();
    painter.setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    painter.setRenderHint(QPainter::Antialiasing, false);

    //  П Р О Р И С О В К А   Э Х О
    if(ui->pushButtonEcho->isChecked())    
        drawEchoMainIKO(&painter);

#if 1
    // цели с имитатора        информация обновляется после прохода луча по цели
    for(int i = 0; i < console2User.apyInfo.totalSimTargetNum; i++)
    {
        if(console2User.apyInfo.rejimPoiskaSNR != POISK_VIKLUCHEN  && fabs(console2User.srvDrvInfo.currentAzimuth - console2User.apyInfo.nextTocka[i].azimuth360) < 0.2)
        {
            nextTocka_slantRange_km[i] = console2User.apyInfo.nextTocka[i].slantRange_km;
            nextTocka_azimuth360[i] = console2User.apyInfo.nextTocka[i].azimuth360;
            nextTocka_elevAngle[i] = console2User.apyInfo.nextTocka[i].elevationAngle;

            greenBrightness[i] = 255;

            yellowBrightness[i] = 640;

            //qDebug()<<"drawIKO: nextTocka.slantRange_km"<<shMemC2U.apyInfo.nextTocka.slantRange_km<<"nextTocka.azimuth360:"<<shMemC2U.apyInfo.nextTocka.azimuth360;

            strV[i] = QString::number(console2User.apyInfo.nextTocka[i].groundSpeed_ms, 'f', 2) + QString::fromUtf8("м/c");
            strH[i] = QString::number(console2User.apyInfo.nextTocka[i].seaLevelAltitude * 1000, 'f', 0) + QString::fromUtf8("м");    // .radarRelativeAltitude
            strK[i] = QString::number(console2User.apyInfo.nextTocka[i].kyrs, 'f', 1) + QString::fromUtf8("°");
            strI[i] = QString::number(console2User.apyInfo.nextTocka[i].targetNo);
        }

        if(greenBrightness[i]-- <= 0)
            greenBrightness[i] = 0;

        if(yellowBrightness[i]-- <= 0)
            yellowBrightness[i] = 0;
    }
#endif

    drawIKO(false, &painter);//, centerIKO);

    //qDebug()<<"drawIKO: azTrgtGrad:"<<azTrgtGrad<<"dX1Trgt:"<<dX1Trgt<<"dY1Trgt:"<<dY1Trgt;

    QPainter painterFin(&ikoBMPFin);
    painterFin.drawPixmap(0, 0, ikoBMP);

    if(ui->pushButtonZoom->isChecked())
    {
        QPainter painter(&ikoZoomBMP);

        QFont fontE("Arial");
        //fontE.setPointSize(10);
        fontE.setBold(false);
        fontE.setStyleStrategy(QFont::NoAntialias);

        painter.setFont( fontE );

        painter.setBrush(Qt::NoBrush);

        //drawIKO(true, &painter);

        if(ui->IKO->isMouseHover() || zoomIKOfreez)//if(arm->zoomIKO_x > 20 && arm->zoomIKO_x < ikoBMP.width() - 20 && arm->zoomIKO_y > 20 && arm->zoomIKO_y < ikoBMP.height() - 20)
        {
            QPainter painterSvetoFilter(&ikoBMPSvetoFilter);
            painterSvetoFilter.fillRect(0, 0, ikoBMP.width(),  ikoBMP.height(), QColor::fromRgbF(0.25, 0.25, 0.25, 1));
            int deltaWH = (int)(1.0 / zoomFactor * destRect2DrawZoomIKO.width()); // квадрат - высота и ширина одинаковы

            QRect okno(zoomIKO_x - deltaWH / 2, zoomIKO_y - deltaWH / 2, deltaWH, deltaWH);



            painterSvetoFilter.fillRect(okno, QColor::fromRgbF(0.0, 0.0, 0.0, 1.0));

            painterFin.setOpacity(0.15);
            painterFin.drawPixmap(0, 0, ikoBMPSvetoFilter);
            painterFin.setOpacity(1);

            destRect2DrawZoomIKO.setWidth(DEST_ZOOM_RECT_WIDTH_HEIGHT);
            destRect2DrawZoomIKO.setHeight(DEST_ZOOM_RECT_WIDTH_HEIGHT);

            if(zoomIKO_x < ikoBMPFin.width() / 2)
                destRect2DrawZoomIKO.setLeft(ikoBMPFin.width() - destRect2DrawZoomIKO.width() - zoomSdvig);
            else
                destRect2DrawZoomIKO.setLeft(zoomSdvig);

            if(zoomIKO_y < ikoBMPFin.height() / 2)
                destRect2DrawZoomIKO.setTop(ikoBMPFin.height() - destRect2DrawZoomIKO.height() - zoomSdvig);
            else
                destRect2DrawZoomIKO.setTop(zoomSdvig);

            sourceRect2DrawZoomIKO.setLeft(zoomFactor * zoomIKO_x - destRect2DrawZoomIKO.width() / 2);
            sourceRect2DrawZoomIKO.setTop(zoomFactor * zoomIKO_y - destRect2DrawZoomIKO.height() / 2);
            sourceRect2DrawZoomIKO.setWidth(destRect2DrawZoomIKO.width());
            sourceRect2DrawZoomIKO.setHeight(destRect2DrawZoomIKO.height());


            //QPixmap pixmapEchoZoom = QPixmap::fromImage(QImage((unsigned char *) imgDataMain, IKO_WIDTH_HEIGHT, IKO_WIDTH_HEIGHT, QImage::Format_RGB888));

            if(zoomIKOfreez)
            {
                if(ui->pushButtonEcho->isChecked())
                    drawEchoZoomIKO(&painter);

//                QPixmap pixmapEchoZoom = QPixmap::fromImage(QImage((unsigned char *) imgDataZoom, DEST_ZOOM_RECT_WIDTH_HEIGHT, DEST_ZOOM_RECT_WIDTH_HEIGHT, QImage::Format_RGB888));

//                painter.drawPixmap(sourceRect2DrawZoomIKO, pixmapEchoZoom, QRect(0, 0, DEST_ZOOM_RECT_WIDTH_HEIGHT, DEST_ZOOM_RECT_WIDTH_HEIGHT));  //  2018_08_03
            }
            else
            {
                QPixmap pixmapEchoZoom = QPixmap::fromImage(QImage((unsigned char *) imgDataMain, IKO_WIDTH_HEIGHT, IKO_WIDTH_HEIGHT, QImage::Format_RGB888));

                painter.drawPixmap(sourceRect2DrawZoomIKO, pixmapEchoZoom, okno);  //  2018_08_02
            }

            drawIKO(true, &painter);

            painterFin.drawPixmap(destRect2DrawZoomIKO, ikoZoomBMP, sourceRect2DrawZoomIKO);
        }
    }

    /////////////////////////////////////drawPriborPyska(&painterFin);

    if(cntrNetVibranihCelej > 0.25)  // КОМАНДИР НЕ ПОПАЛ ТРЕКБОЛОМ В ФОРМУЛЯР ЦЕЛИ
    {
        QFont fnt = painter.font();
        fnt.setPointSize(10);
        fnt.setBold(true);
        painterFin.setFont(fnt);

        cntrNetVibranihCelej -= 0.001 * drawGraphicsTimer->interval();

        painterFin.setPen(QColor::fromRgbF(1.0, 0.25, 0.25, cntrNetVibranihCelej / 2.0));

        painterFin.drawText(10, 30, QString::fromUtf8("Цель не выбрана"));

        if(cntrNetVibranihCelej < 1.0)
            ui->IKO->setCursor(Qt::ArrowCursor);//::IBeamCursor);
    }

    ui->IKO->setPixmap(ikoBMPFin);
}


