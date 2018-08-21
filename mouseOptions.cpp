#include <QtGui>
#include "mainWindowC.h"
#include "ui_mainwindow.h"

extern float convert360angle2PlusMinus180(float dltAz360);

void MainWindow::MouseMoveIKO(int xx, int yy)
{
    int target_IKO_MouseMove_Number_Temp = -1;    
    float minNeviazka = 1000.0;

    int xxTemp = xx;
    int yyTemp = yy;
    float factorR = factorR_IKO_Main;
    QPoint centerIKO = centerMainIKO;

    //qDebug()<<"MouseMoveIKO xx:"<<xx<<"yy:"<<yy;

    //float oldAge = ui->dblSpnBxAge->value();

    if(ui->pushButtonZoom->isChecked() && //ui->IKO->visibleMarker && //|| arm->zoomIKOfreez) &&                      //  если внутри лупы
       (destRect2DrawZoomIKO.left() < xx && xx < destRect2DrawZoomIKO.left() + destRect2DrawZoomIKO.width() &&  //  если внутри лупы
        destRect2DrawZoomIKO.top()  < yy && yy < destRect2DrawZoomIKO.top()  + destRect2DrawZoomIKO.height()))  //  если внутри лупы
    {
        xxTemp = xx - destRect2DrawZoomIKO.left() + sourceRect2DrawZoomIKO.left();
        yyTemp = yy - destRect2DrawZoomIKO.top()  + sourceRect2DrawZoomIKO.top();
        factorR = factorR_IKO_Zoom;
        centerIKO = centerZoomIKO;        


        //qDebug()<<"внутри лупы xxTemp:"<<xxTemp<<"centerIKO:"<<centerIKO.x();//<<"yyTemp:"<<yyTemp;
//        qDebug()<<"внутри лупы xxTemp:"<<xxTemp<<" = xx:"<<xx<<" - destRect2DrawZoomIKO.left():"<<destRect2DrawZoomIKO.left()
//               <<" + sourceRect2DrawZoomIKO.left():"<<sourceRect2DrawZoomIKO.left();//<<"yyTemp:"<<yyTemp;
//        qDebug()<<"внутри лупы yy:"<<yy;
    }

    if(xxTemp == centerIKO.x() && yyTemp == centerIKO.y()) // маловероятно, но возможно - попали точно в центр ИКО
    {
        mouseCursorAzimuth = console2User.srvDrvInfo.directrisaAzimuth;    // куда поворачиваться непонятно - оставляем всё, как есть
    }
    else
    {
        //float newAzGrad = atan2(ev->x() - centerMainIKO.x(), centerMainIKO.y() - ev->y()) * Rad2Grad;
        //dAz = mouseCursorAzimuth - console2User.srvDrvInfo.directrisaAzimuth;
        mouseCursorAzimuth = atan2(xxTemp - centerIKO.x(), centerIKO.y() - yyTemp) * Rad2Grad;

        if(mouseCursorAzimuth < 0)
            mouseCursorAzimuth += 360.0;
    }


#if 0                   //  вообще не будем отождествлять цели П-18
    if(komKomC.useP18)
    {
        for(int i =  MAXTOCKANUM - 1; i >= 0; i--)
        {
            float age = awsCsecsSinceStartOfDay - console2User.apyInfo.nextTocka[i].secsSinceStartOfDay;

            if(age > oldAge)
                continue;

            //if(nextTocka_elevAngle[i] < shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle - 5 || nextTocka_elevAngle[i] > shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle + 5)
            //    continue;   //      цель за пределами сектора сканирования по углу

            float dXTrgt =   monoTrackP18[i].head.groundX * factorR;
            float dYTrgt = - monoTrackP18[i].head.groundY * factorR;

            QPoint trgtPointIKO = centerIKO + QPoint(dXTrgt, dYTrgt);

            float nevX = trgtPointIKO.x() - xxTemp;
            float nevY = trgtPointIKO.y() - yyTemp;

            float nevAbs = sqrt(nevX * nevX + nevY * nevY);

            if(nevAbs < minNeviazka)
            {
                minNeviazka = nevAbs;
                target_IKO_MouseMove_Number_Temp = i;
            }
            //qDebug()<<"                               MouseMoveIndicatorEB: j:"<<j<<"nevAbs:"<<nevAbs<<"minNeviazka:"<<minNeviazka;
        }
    }
#endif

    for(int j = 0; j < MAX_MDO1_PRIMARY_PLOTS_COUNT; j++)
    {
        if(console2User.primaryPlots[j].packageWeight == 0)
            break;

        float azRad = console2User.primaryPlots[j].azimuth360 / Rad2Grad;
        float scrX =  centerIKO.x() + console2User.primaryPlots[j].range_km * factorR * sin(azRad);
        float scrY =  centerIKO.y() - console2User.primaryPlots[j].range_km * factorR * cos(azRad);

        float nevX = scrX - xxTemp;
        float nevY = scrY - yyTemp;

        float nevAbs = sqrt(nevX * nevX + nevY * nevY);

        if(nevAbs < minNeviazka)
        {
            minNeviazka = nevAbs;
            target_IKO_MouseMove_Number_Temp = j;
        }
    }

    if(minNeviazka < 6.0)
    {
        target_IKO_MouseMove_Number = target_IKO_MouseMove_Number_Temp; // цель отождествлена
        //qDebug()<<"target_IKO_MouseMove_Number:"<<target_IKO_MouseMove_Number<<"minNeviazka:"<<minNeviazka<<"targetNo:"<<shMemC2U.apyInfo.nextTocka[target_IKO_MouseMove_Number].targetNo;
    }
    else
    {
        target_IKO_MouseMove_Number = -1;                               // цель не отождествлена
        //qDebug()<<"target_IKO_MouseMove_Number:"<<target_IKO_MouseMove_Number<<"minNeviazka:"<<minNeviazka;
    }

    //  в режиме тренажа отождествление с номером списка имитированных целей
    if(target_IKO_MouseMove_Number > -1 && (console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_A ||
                                            console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_K))
    {
        int simTrgt_IKO_MouseMove_Number_Temp = - 1;
        float simMinNeviazka = 1000.0;

        float azRad1 = console2User.primaryPlots[target_IKO_MouseMove_Number].azimuth360 / Rad2Grad;
        float xs1 =  console2User.primaryPlots[target_IKO_MouseMove_Number].range_km * sin(azRad1);
        float ys1 =  console2User.primaryPlots[target_IKO_MouseMove_Number].range_km * cos(azRad1);

        for(int i = 0; i < console2User.apyInfo.totalSimTargetNum; i++)
        {
            float azRad2 = console2User.apyInfo.nextTocka[i].azimuth360 / Rad2Grad;
            float xs2 =  console2User.apyInfo.nextTocka[i].slantRange_km * sin(azRad2);
            float ys2 =  console2User.apyInfo.nextTocka[i].slantRange_km * cos(azRad2);

            float nevX = xs2 - xs1;
            float nevY = ys2 - ys1;

            float nevAbs = sqrt(nevX * nevX + nevY * nevY);

            if(nevAbs < simMinNeviazka)
            {
                simMinNeviazka = nevAbs;
                simTrgt_IKO_MouseMove_Number_Temp = i;
            }
        }

        //target_IKO_MouseMove_Number = simTrgt_IKO_MouseMove_Number;

        if(simMinNeviazka < 6.0)
        {
            simTrgt_IKO_MouseMove_Number = simTrgt_IKO_MouseMove_Number_Temp;; // цель отождествлена
        }
        else
        {
            simTrgt_IKO_MouseMove_Number = -1;                               // цель не отождествлена
        }
    }

    //ui->labelPorog->setText(QString("hovMainX: %1").arg(hovMainX));

    if(zoomIKOfreez == false)
    {
        zoomIKO_x = xx;
        zoomIKO_y = yy;
    }

    //ui->labelAltitude->setText();
}


void MainWindow::MouseMoveScales(int hovMain, int hovDelt)
{        
    scaleInfo->detailScaleHoverX = hovDelt;
    scaleInfo->mainScaleHoverX = hovMain;

    //  2018_06_25  ui->labelPorog->setText(QString("e: %1° [%2°]").arg(hovMain).arg(hovDelt));
}


void MainWindow::MouseClickSlot_elevationScale(double deltaHov)//, bool cntrlModifierIsPressed)
{
    if(deltaHov < -180.0)
        deltaHov += 360.0;

    if(deltaHov > 180.0)
        deltaHov -= 360.0;

    if(ui->elevationScale87->controlModifierIsHoldingDown == false)
    {
        komKomC.srv[ELEVANG].delta = deltaHov;
        if(komKomC.srv[ELEVANG].executeCounter % 10 == 0)
            komKomC.srv[ELEVANG].executeCounter += 17;

//        qDebug()<<"*************************************    MouseClickSlot_eleavationScale: shMemU2C.userCmds.executeNewElevationAngleManual:"
//               <<shMemU2C.userCmds.executeNewElevationAngleManual<<"shMemU2C.userCmds.deltaElevationAngle:"<<shMemU2C.userCmds.deltaElevationAngle;
    }
}


void MainWindow::MouseClickSlot(double deltaAzimHov)//, bool cntrlModifierIsPressed)
{
    //////////////////////////////if(ui->pushButtonVneshneeCY->isChecked() || executeNewAzManual || deltaAzimHov == 0)
        //////////////////////////////return;

    //ui->labelTest_2->setText(QString("deltaAzimHov: %1").arg(deltaAzimHov));

    //qDebug()<<"MouseClickSlot: deltaAzimHov:"<<deltaAzimHov;

    //arm->newAzCY =  arm->azimuthPRV_CURRENT + deltaAzimHov;


    //if(ui->azimuthScale360->controlModifierIsHoldingDown == false && ui->azimuthScale3600->controlModifierIsHoldingDown == false)
    {
        komKomC.srv[AZIMUTH].delta =  convert360angle2PlusMinus180(deltaAzimHov);
        if(komKomC.srv[AZIMUTH].executeCounter % 10 == 0)
            komKomC.srv[AZIMUTH].executeCounter += 17;

//        qDebug()<<"*************************************    MouseClickSlot: komKomC.srv[AZIMUTH].delta:"<<komKomC.srv[AZIMUTH].delta
//                <<"executeCounter:"<<komKomC.srv[AZIMUTH].executeCounter;
    }
}

