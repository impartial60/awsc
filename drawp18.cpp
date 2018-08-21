#include <QtGui>
#include "mainWindowC.h"
#include "ui_mainwindow.h"

QColor newTrackColor(48,176,192, 176);
QColor newKnotColor(48,176,192, 192);

//QColor oldTrackColor(48,176,192, 64);
//QColor oldKnotColor(48,176,192, 80);

//QColor oldTrackColor(32,128,144,128);
//QColor oldKnotColor(32,128,144,160);

QColor selectedTargetColor(192, 255, 160, 255);// = Qt::yellow;


void MainWindow::drawP18(bool isZoomIKO, QPainter *painter, QPoint centerIKO, float factorR_IKO, float radiusIKO)
{
    //        QString crdStr = "";

    //        for(int i = 0; i < MAXTOCKANUM; i++)
    //        {
    //            int ii = monoTrackPointsCount[i] - 1;

    //            if(ii >= 0 && ii < MONOTRACKSIZE)
    //            {
    //                crdStr += "\nчисло точек: " + QString::number(ii) +
    //                        " Nцели: " + QString::number(monoTrack[i][ii].targetNo) +
    //                " Д: " + QString::number(monoTrack[i][ii].groundRange_km, 'f', 3) + "km" +
    //                " Аз:" + QString::number(monoTrack[i][ii].azimuth360, 'f', 3) + "°" +
    //                " V:" + QString::number(monoTrack[i][ii].groundSpeed_ms, 'f', 3) + "м/c" +
    //                " Курс :" + QString::number(monoTrack[i][ii].kyrs, 'f', 3) + "°";
    //            }
    //        }

    //qDebug()<<"drawIKO:"<<shMemC2U.srvDrvInfo.maxTockaCurrentNum;//crdStr;

    //painter->setPen(newTailMainIKOPen);

    // Р А З В Ё Р Т К А   П - 1 8
    float azRadians = console2User.apyInfo.sector002 / Rad2Grad;
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawEllipse(centerIKO + QPoint(radiusIKO * sin(azRadians), - radiusIKO * cos(azRadians)), 1, 1);

    float oldAge = ui->dblSpnBxAge->value();

    for(int j = 0; j < MAXTOCKANUM; j++)    //  рисование хвостов
    {
        float age = awsCsecsSinceStartOfDay - monoTrackP18[j].head.secsSinceStartOfDay;

        if(age > oldAge)
            continue;

        float dXTrgt_old =   monoTrackP18[j].head.groundX * factorR_IKO;
        float dYTrgt_old = - monoTrackP18[j].head.groundY * factorR_IKO;
        QPoint trgtPointIKO_old = centerIKO + QPoint(dXTrgt_old, dYTrgt_old);

        int minNumber = monoTrackP18[j].tailSize - ui->dblSpnBxPlotNumber->value() + 1;
        if(minNumber < 0)
            minNumber = 0;

        for(int i = monoTrackP18[j].tailSize - 1; i >= minNumber; i--)
        {
            float dXTrgt =   monoTrackP18[j].tail[i].groundX * factorR_IKO;
            float dYTrgt = - monoTrackP18[j].tail[i].groundY * factorR_IKO;

            QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

//            if(age > oldAge)
//                painter->setPen(oldTrackColor);
//            else
                painter->setPen(newTrackColor);

            ////////////////////painter->drawLine(trgtPointIKO_old, trgtPointIKO);

//            if(age > oldAge)
//                painter->setPen(oldKnotColor);
//            else
//                painter->setPen(newKnotColor);

            int alphaValue = 192 - (monoTrackP18[j].tailSize - 1 - i) * 16;
            if(alphaValue < 48)
                alphaValue = 48;

            //qDebug()<<"j:"<<j<<"i:"<<i<<"alphaValue:"<<alphaValue;

//            painter->pen().color().setAlpha(0);//alphaValue);//.setColor(painter->pen().color.red(), painter->pen().color.green(), painter->pen().color.blue(), newTrackColor.alpha() );
//            painter->pen().color().setRed(255);
//            painter->pen().color().setGreen(0);
//            painter->pen().color().setBlue(0);
            //painter->setPen(QColor(Qt::red));

            //QColor clr = painter->pen().color();

            painter->setPen(QColor(painter->pen().color().red(), painter->pen().color().green(), painter->pen().color().blue(), alphaValue));

            if(isZoomIKO)
                painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, painter->pen().color());//newKnotColor);
            else
                painter->drawPoint(trgtPointIKO);

            trgtPointIKO_old = trgtPointIKO;
        }
    }

    painter->setPen(newTrackColor);

    for(int j = 0; j < MAXTOCKANUM; j++)    //  рисование головок и формуляров невыбранных целей
    {
        if(monoTrackP18[j].head.targetNo <= 0 || j == target_IKO_MouseMove_Number)
            continue;

        float age = awsCsecsSinceStartOfDay - monoTrackP18[j].head.secsSinceStartOfDay;

        if(age > oldAge)
            continue;

        float dXTrgt =   monoTrackP18[j].head.groundX * factorR_IKO;
        float dYTrgt = - monoTrackP18[j].head.groundY * factorR_IKO;

        QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

        if(isZoomIKO)
        {
             painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, painter->pen().color());
        }
        else
        {
            painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, painter->pen().color());
        }

        //str_V = QString::number(shMemC2U.apyInfo.nextTocka[i].groundSpeed_ms, 'f', 2) + QString::fromUtf8("м/c");
        //str_H = QString::number(shMemC2U.apyInfo.nextTocka[i].seaLevelAltitude * 1000, 'f', 0) + QString::fromUtf8("м");    // .radarRelativeAltitude
        //str_K = QString::number(shMemC2U.apyInfo.nextTocka[i].kyrs, 'f', 1) + QString::fromUtf8("°");

        QString str_I = "Nц " + QString::number(monoTrackP18[j].head.targetNo);
        if(monoTrackP18[j].head.sourceId == SOURCEID::P18)
            str_I += "☼";
        if(monoTrackP18[j].head.sourceId == SOURCEID::ARMTREN)
            str_I += "☺";//
        drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y(), str_I);

        //QString str_T = " T: " + QString::number(awsCsecsSinceStartOfDay - monoTrack2[j].head.secsSinceStartOfDay, 'f', 1) + QString::fromUtf8("c");

        if(isZoomIKO)
        {
            painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, painter->pen().color());//newKnotColor);

            QString str_V = QString::number(monoTrackP18[j].head.groundSpeed_ms, 'f', 1) + QString::fromUtf8("м/c");
            drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y() + 15, str_V);
            //drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y() + 30, str_T);
        }
        else
        {
            painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, painter->pen().color());//newKnotColor);
            //drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y() + 15, str_T);
        }
    }

    if(target_IKO_MouseMove_Number > - 1)   //  рисование головки и формуляра выбранной цели, если она есть
    {
        //QColor selectedTargetColor = Qt::yellow;
        painter->setPen(selectedTargetColor);

        float dXTrgt =   monoTrackP18[target_IKO_MouseMove_Number].head.groundX * factorR_IKO;
        float dYTrgt = - monoTrackP18[target_IKO_MouseMove_Number].head.groundY * factorR_IKO;

        QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

        //str_V = QString::number(shMemC2U.apyInfo.nextTocka[i].groundSpeed_ms, 'f', 2) + QString::fromUtf8("м/c");
        //str_H = QString::number(shMemC2U.apyInfo.nextTocka[i].seaLevelAltitude * 1000, 'f', 0) + QString::fromUtf8("м");    // .radarRelativeAltitude
        //str_K = QString::number(shMemC2U.apyInfo.nextTocka[i].kyrs, 'f', 1) + QString::fromUtf8("°");

        QString str_I = "Nц " + QString::number(monoTrackP18[target_IKO_MouseMove_Number].head.targetNo);

        //qDebug()<<"target_IKO_MouseMove_Number:"<<target_IKO_MouseMove_Number<<str_I;

        if(monoTrackP18[target_IKO_MouseMove_Number].head.sourceId == SOURCEID::P18)
            str_I += "☼";
        if(monoTrackP18[target_IKO_MouseMove_Number].head.sourceId == SOURCEID::ARMTREN)
            str_I += "☺";//
        drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y(), str_I);

        //QString str_T = " T: " + QString::number(awsCsecsSinceStartOfDay - monoTrack2[target_IKO_MouseMove_Number].head.secsSinceStartOfDay, 'f', 1) + QString::fromUtf8("c");

        if(isZoomIKO)
        {
            painter->fillRect(trgtPointIKO.x() - 2, trgtPointIKO.y() - 2, 4, 4, selectedTargetColor);

            QString str_V = QString::number(monoTrackP18[target_IKO_MouseMove_Number].head.groundSpeed_ms, 'f', 1) + QString::fromUtf8("м/c");
            drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y() + 15, str_V);
            //drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y() + 30, str_T);
        }
        else
        {
            painter->fillRect(trgtPointIKO.x() - 1, trgtPointIKO.y() - 1, 2, 2, selectedTargetColor);
            //drawTextHalo(painter, painter->pen(), painter->font(), trgtPointIKO.x() + 10, trgtPointIKO.y() + 15, str_T);
        }
    }
}
