#include "mainWindowC.h"
#include "ui_mainwindow.h"



void MainWindow::parserP18()
{
    //    int monoTrackElapsedTime[MAXTOCKANUM];
    //    int monoTrackPointsCount[MAXTOCKANUM];
    //    int monoTrackTargetNoNo[MAXTOCKANUM];

    if(komKomC.rejimRabotiZRK == RejimRabotiZRK::TR_A ) // только для теста)
    {
        if(console2User.apyInfo.klmTransactionId == klmTransactionId_old)
            return;

        klmTransactionId_old = console2User.apyInfo.klmTransactionId;
    }

    for(int j = 0; j < MAXTOCKANUM; j++)
    {
        if(console2User.apyInfo.nextTocka[j].sourceId == SOURCEID::NOSOURCE)
            continue;

//        if(console2User.apyInfo.nextTocka[j].targetNo == 0)
//            continue;

        bool nextTochkaIsMatched = false;
        int matchedIndex = -1;

        //QString crdStr3 = "nextTocka j:" + QString::number(j) + " Nц:" + QString::number(shMemC2U.apyInfo.nextTocka[j].targetNo) + "    monoTrackTargetNoNo:";

        for(int i = 0; i < MAXTOCKANUM; i++)
        {
            if(monoTrackP18[i].head.sourceId == console2User.apyInfo.nextTocka[j].sourceId)                 //  совпадает источник информации
            {
                if(monoTrackP18[i].head.targetNo == console2User.apyInfo.nextTocka[j].targetNo)             //  совпадает номер цели в системе нумерации источника
                {                    
                    if(komKomC.rejimRabotiZRK == RejimRabotiZRK::TR_A &&                                    // в режиме тренажа с АРМа РТ добавляем информация только раз за оборот П-18
                            console2User.apyInfo.nextTocka[j].secsSinceStartOfDay - monoTrackP18[i].head.secsSinceStartOfDay > 8.0 ||
                    komKomC.rejimRabotiZRK != RejimRabotiZRK::TR_A && (monoTrackP18[i].head.groundX != console2User.apyInfo.nextTocka[j].groundX || // для реальной П-18
                       monoTrackP18[i].head.groundY != console2User.apyInfo.nextTocka[j].groundY))  //  будем добавлять информацию в монотрек только если она новая
                        matchedIndex = i;
                    nextTochkaIsMatched = true;             //  но, в любом случае, отождествили полностью и дальнейшие проверки не проводим
                    break;
                }
            }
        }

        //qDebug()<<crdStr3;

        if(nextTochkaIsMatched == false)                //  не был найден совпадающий номер цели совпадающего источника
        {
            for(int i = 0; i < MAXTOCKANUM; i++)        //  ищем первый свободный
            {
                if(monoTrackP18[i].head.sourceId == SOURCEID::NOSOURCE)
                {
                    matchedIndex = i;
                    nextTochkaIsMatched = true;
                    break;
                }
            }
        }

        if(nextTochkaIsMatched == false)                //  в списке nextTocka нет свободного места
        {
            int minElapsed = 1000000;

            for(int i = 0; i < MAXTOCKANUM; i++)        //  ищем самое старое сообщение
            {
                if(monoTrackP18[i].head.secsSinceStartOfDay < minElapsed)
                {
                    minElapsed = monoTrackP18[i].head.secsSinceStartOfDay;
                    matchedIndex = i;
                }
            }
            monoTrackP18[matchedIndex].tailSize = 0;
            //qDebug()<<"выбран устаревший ind:"<<ind<<"minElapsed:"<<minElapsed;
        }

        //qDebug()<<"ind:"<<ind;

        if(matchedIndex >= 0)
        {
            if(monoTrackP18[matchedIndex].tailSize >= MONOTRACKSIZE - 1)
                monoTrackP18[matchedIndex].tailSize = 0;

            if(monoTrackP18[matchedIndex].head.targetNo > 0)
            {
                monoTrackP18[matchedIndex].tail[monoTrackP18[matchedIndex].tailSize].radarRelativeAltitude = monoTrackP18[matchedIndex].head.radarRelativeAltitude;
                monoTrackP18[matchedIndex].tail[monoTrackP18[matchedIndex].tailSize].groundX = monoTrackP18[matchedIndex].head.groundX;
                monoTrackP18[matchedIndex].tail[monoTrackP18[matchedIndex].tailSize].groundY = monoTrackP18[matchedIndex].head.groundY;
                monoTrackP18[matchedIndex].tailSize ++;
            }

            memcpy(&monoTrackP18[matchedIndex].head, &console2User.apyInfo.nextTocka[j], sizeof(TargetParams));

            //memcpy(&monoTrack[matchedIndex][monoTrackPointsCount[matchedIndex]], &console2User.apyInfo.nextTocka[j], sizeof(TargetParams));
           // monoTrackTargetNoNo[matchedIndex] = console2User.apyInfo.nextTocka[j].targetNo;
            //monoTrackElapsedTime[matchedIndex] = programmStartTime.elapsed();
            //monoTrackPointsCount[matchedIndex] += 1;
        }
    }

    //QString crdStr2 = "Nцели: ";  for(int i = 0; i < MAXTOCKANUM; i++)  crdStr2 +=  QString::number(monoTrack[i][monoTrackPointsCount[i] - 1].targetNo) + " ";    qDebug()<<crdStr2;

    QString crdStr = "";    

    for(int i = 0; i < MAXTOCKANUM; i++)
    {
        //if(monoTrack2[i].tailSize > 0)
        {
            crdStr += "\nNц: " + QString::number(monoTrackP18[i].head.targetNo) +
                      " SrcId: " + QString::number(monoTrackP18[i].head.sourceId) +
                      " Nточек: " + QString::number(monoTrackP18[i].tailSize) +
                      " T: " + QString::number(awsCsecsSinceStartOfDay - monoTrackP18[i].head.secsSinceStartOfDay, 'f', 1) +
                      " Д: " + QString::number(monoTrackP18[i].head.groundRange_km, 'f', 3) + "km" +
                      " Аз:" + QString::number(monoTrackP18[i].head.azimuth360, 'f', 3) + "°" +
                      " H:" + QString::number(monoTrackP18[i].head.radarRelativeAltitude, 'f', 3) + "km" +
                      " V:" + QString::number(monoTrackP18[i].head.groundSpeed_ms, 'f', 3) + "м/c";
                      //" Курс :" + QString::number(monoTrackP18[i].head.kyrs, 'f', 3) + "°";
        }
        if(monoTrackP18[i].tailSize == 0)
            crdStr += " tailSize: 0";

    }

    crdStr += "\n\nсектор П18: " + QString::number(console2User.apyInfo.sector002, 'f', 0) + "°";

    ui->labelTargetCharacteristics->setText(crdStr);

    if(komKomC.useP18)
    {
        if(console2User.apyInfo.p18TransactionId > p18TransactionId_old)
        {
            p18TransactionId_old = console2User.apyInfo.p18TransactionId;
            ui->label_P18->setEnabled(true);
            //qDebug()<<"ui->label_P18->setEnabled(true);";
        }
        else
            ui->label_P18->setEnabled(false);
    }

}


void MainWindow::nextTochkaOptions()
{
/*
    if(shMemC2U.srvDrvInfo.klmTransactionId > klmTransactionId_old)
    {
        klmTransactionId_old = shMemC2U.srvDrvInfo.klmTransactionId;

//        int ind = -1;

//        for(int j = 0; j < MAXTOCKANUM; j++)
//        {
//            shMemC2U.apyInfo.nextTocka[j]
//        }


        if(monoTrackPointsCount[j] >= MONOTRACKSIZE)
            trackPointsCount = 0;

//        if(trackPointsCount > 1 && fabs(monoTrack[0][trackPointsCount - 2].groundRange_km - monoTrack[0][trackPointsCount - 1].groundRange_km) > 1.0)     // обнаружитель перезапуска траектории
//            trackPointsCount = 0;

        for(int j = 0; j < shMemC2U.srvDrvInfo.totalSimTargetNum; j++)
        {
            memcpy(&monoTrack[j][trackPointsCount], &shMemC2U.apyInfo.nextTocka[j], sizeof(TargetParams));
        }

        trackPointsCount++;

//       qDebug()<<"nextTochkaOptions: shMemC2U.srvDrvInfo.trackPointsNumber:"<<shMemC2U.srvDrvInfo.trackPointsNumber
//              <<"az:"<<monoTrack[shMemC2U.srvDrvInfo.trackPointsNumber].azimuth180<<"e:"<<monoTrack[shMemC2U.srvDrvInfo.trackPointsNumber].elevationAngle;
    }
            */
/*
    QString resStr = "<!DOCTYPE html><html><head> <style> p {line-height: 20%; white-space: pre-wrap;}  p.ex { margin-top: 18px; }</style></head><body>";
    //resStr += QString("<p>β</p>");

    //resStr += QString("<p>β<span style=\"vertical-align:sub;\">ц</span>:%1</p>").arg(shMemC2U.apyInfo.nextTocka.azimuth180, 3, 10, QChar('0'));

    resStr += QString("<p>β<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.azimuth360, 'f', 2) + QString::fromUtf8("°  ");
    //resStr += QString("<p>β<span style=\"vertical-align:sub;\">унв</span>: ") + QString::number(shMemC2U.srvDrvInfo.currentAzimuth, 'f', 2) + QString::fromUtf8("°</p>");

    resStr += QString("ε<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.elevationAngle, 'f', 2) + QString::fromUtf8("°</p>");
    resStr += QString("<p>H<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.seaLevelAltitude, 'f', 3) + QString::fromUtf8(" km   ");
    resStr += QString("V<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundSpeed, 'f', 1) + QString::fromUtf8(" м/с</p>");

    //resStr += QString("<p>x: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundX, 'f', 3) + " km   ";//QString("<span style=\"vertical-align:sub;\"> km</span>");
    //resStr += QString("y: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundY, 'f', 3) + " km</p>";
    resStr += QString("<p>Курс: ") + QString::number(shMemC2U.apyInfo.nextTocka.kyrs, 'f', 3) + QString::fromUtf8("°");
    resStr += QString("<p>Д<span style=\"vertical-align:sub;\">гор</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundRange_km, 'f', 3) + QString::fromUtf8(" km</p>");
    resStr += QString("<p>Д<span style=\"vertical-align:sub;\">накл</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.slantRange_km, 'f', 3) + QString::fromUtf8(" km   ");
    resStr += QString("Д<span style=\"vertical-align:sub;\">делта</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.slantRange_km - shMemC2U.apyInfo.nextTocka.groundRange_km, 'f', 3) + QString::fromUtf8(" km</p>");

    resStr += QString("<p> </p><p>β<span style=\"vertical-align:sub;\">УНВ</span>: ") + QString::number(shMemC2U.srvDrvInfo.currentAzimuth, 'f', 1) + QString::fromUtf8("°   ");
    resStr += QString("ε<span style=\"vertical-align:sub;\">УНВ</span>: ") + QString::number(shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle, 'f', 1) + QString::fromUtf8("°</p>");
    resStr += QString("<p>trackPointsNumber: ") + QString::number(shMemC2U.srvDrvInfo.trackPointsNumber) + "</p>";

    resStr += QString("<p>Id<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.transactionId);
    resStr += QString("   N<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.numTarget) + "</p>";
    resStr += QString("<p>R<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.distance) + " км";
    resStr += QString("   H<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(1000.0 * shMemC2U.srvDrvInfo.artSimData.height) + " м</p>";
    resStr += QString("<p>β<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.azimuth) + "°";
    resStr += QString("   V<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.speed) + " м/с</p>";
    resStr += QString("<p>K<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.kyrs) + "°";
    resStr += QString("   Отработать ЦУ<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.otrabCY) + "</p>";

//    resStr += QString("<p>Время<span style=\"vertical-align:sub;\">командира</span>: ") + shMemC2U.srvDrvInfo.bond2AWSG_27.sendStrTime.toString("hh:mm:ss.zzz")//.. QString::number(shMemC2U.srvDrvInfo.bondarchuk_27.getStrTime.hour()) + ":" +
//            //QString::number(shMemC2U.srvDrvInfo.bondarchuk_27.getStrTime.minute()) + ":" + QString::number(shMemC2U.srvDrvInfo.bondarchuk_27.getStrTime.second())
//                    + "</p>";

    resStr += QString("<p>l<span style=\"vertical-align:sub;\">LPH</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.soprovojdCoord.l) + "</p>";
    resStr += QString("<p>p<span style=\"vertical-align:sub;\">LPH</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.soprovojdCoord.p) + "</p>";
    resStr += QString("<p>h<span style=\"vertical-align:sub;\">LPH</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.soprovojdCoord.h) + "</p>";

    QString bitsStr = "";

    for(int i = 0; i < 48; i++)
    {
        //shMemC2U.srvDrvInfo.new_state_G.Buttons.KeyMap

                //bit = (number >> x) & 1U;
                int bit = (shMemC2U.new_state_C.Buttons.KeyMap >> i) & 1UL;

//                if(bit == 0)
//                    bitsStr += "0 ";
//                else

                if(i % 8 == 6 || i % 8 == 7)
                    bitsStr += " ";
                else
                    bitsStr += QString::number(bit);
    }

    resStr += QString("<p>") + bitsStr + "</p>";
    resStr += "</body></html>";
    ui->labelNextTochka->setText(resStr);
*/

    ///////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                       //
    //   С И Н Х Р О Н И З А Ц И Я   Э К Р А Н Н Ы Х   К Н О П О К   С   Ж Е Л Е З Н Ы М И   //
    //         И   Ф О Р М И Р О В А Н И Е   С О О Б Щ Е Н И Я   Н А   О Т П Р А В К У       //
    //                                                                                       //
    ///////////////////////////////////////////////////////////////////////////////////////////

    // Р Е Ж И М   Р А Б О Т Ы   З Р К
/*
    int bit_BR = (shMemC2U.new_state_C.Buttons.KeyMap >> 24) & 1UL;
    int bit_FK = (shMemC2U.new_state_C.Buttons.KeyMap >> 25) & 1UL;
    int bit_Tr = (shMemC2U.new_state_C.Buttons.KeyMap >> 26) & 1UL;
    int bit_RR = (shMemC2U.new_state_C.Buttons.KeyMap >> 27) & 1UL;

    if (bit_BR == 0 && bit_FK == 1 && bit_Tr == 1 && bit_RR == 1)
        ui->pushButtonRejimRabotiBR->setChecked(true);
    if (bit_BR == 1 && bit_FK == 0 && bit_Tr == 1 && bit_RR == 1)
        ui->pushButtonRejimRabotiFK->setChecked(true);
    if (bit_BR == 1 && bit_FK == 1 && bit_Tr == 0 && bit_RR == 1)
        ui->pushButtonRejimRabotiTren->setChecked(true);
    if (bit_BR == 1 && bit_FK == 1 && bit_Tr == 1 && bit_RR == 0)
        ui->pushButtonRejimRabotiRR->setChecked(true);
*/
    if(komKomC.useP18 !=  ui->pushButtonUseP18->isChecked())
    {
//        for(int i = 0; i < MAXTOCKANUM; i++)
//        {
//            monoTrackElapsedTime[i] = 0;
//            monoTrackPointsCount[i] = 1;
//            //monoTrackTargetNoNo[i] = 0;
//        }

        klmTransactionId_old = 0;
        p18TransactionId_old = 0;

        komKomC.useP18 = ui->pushButtonUseP18->isChecked();
        ui->groupBoxShowP18->setEnabled(komKomC.useP18);

        if(komKomC.useP18 == false)
        {
            ui->label_P18->setEnabled(false);
        }
    }

    awsCsecsSinceStartOfDay = 0.001 * (QTime::currentTime().msecsSinceStartOfDay());

    if(ui->pushButtonRejimRabotiBR->isChecked())
        komKomC.rejimRabotiZRK = RejimRabotiZRK::BR;
    if(ui->pushButtonRejimRabotiFK->isChecked())
        komKomC.rejimRabotiZRK = RejimRabotiZRK::FK;

    if(ui->pushButtonRejimRabotiTren->isChecked())
    {
        ui->groupBoxSimuliators->setEnabled(true);  // имитаторы доступны только в режиме тренажа

        if(komKomC.rejimRabotiZRK != RejimRabotiZRK::TR_A && komKomC.rejimRabotiZRK != RejimRabotiZRK::TR_K)    // при включении тренажа всегда сразу включаются имитаторы сервоприводов,
        {                                                                                                       // их можно выключить и работать с реальными (например, во время показа на выставке)
            ui->pushButtonSimulatorServoDrive->setChecked(true);
            komKomC.srv[AZIMUTH].delta = 0.0;
            komKomC.srv[ELEVANG].delta = 0.0;
        }

        komKomC.servoDriveSimulation = ui->pushButtonSimulatorServoDrive->isChecked();

        if(ui->pushButtonSimulatorKP->isChecked())
        {
            komKomC.rejimRabotiZRK = RejimRabotiZRK::TR_A;            
        }
        else
        {
            komKomC.rejimRabotiZRK = RejimRabotiZRK::TR_K;
            ///////////////////////shMemU2C.komKom.maxTockaCurrentNum = 3; // ПОКА ТРИ ЦЕЛИ КОМПЛЕКСНОГО ТРЕНАЖА
        }
    }
    else
    {
        ui->groupBoxSimuliators->setEnabled(false);
        ui->pushButtonSimulatorServoDrive->setChecked(false);
        komKomC.servoDriveSimulation = false;
    }

    if(ui->pushButtonRejimRabotiRR->isChecked())
    {
        komKomC.rejimRabotiZRK = RejimRabotiZRK::RR;
    }

    if(komKomC.rejimRabotiZRK != console2User.apyInfo.rejimRabotiZRK)   //  2018_08_07  это, возможно, перезапуск тренировки
        memset(&monoTrackP18, 0, sizeof(monoTrackP18));                 //  2018_08_07


    //  П У С К   Р А З Р Е Ш Е Н
/*
    int bit_Pysk_Razreshen  = (shMemC2U.new_state_C.Buttons.KeyMap >> 2) & 1UL;
    int bit_Pysk_Zapreschen = (shMemC2U.new_state_C.Buttons.KeyMap >> 3) & 1UL;

    if(bit_Pysk_Razreshen == 0 && bit_Pysk_Zapreschen == 1)
        ui->pushButtonPyskRazreshen->setChecked(true);
    if(bit_Pysk_Razreshen == 1 && bit_Pysk_Zapreschen == 0)
        ui->pushButtonPyskRazreshen->setChecked(false);
*/
    komKomC.pyskRazreshen = ui->pushButtonPyskRazreshen->isChecked();

    //  О Т О Б Р А Ж А Т Ь   И С Т И Н Н О Е   П О Л О Ж Е Н И Е   Ц Е Л И
    //  2018_07_02  komKomC.showSimulationTrack = ui->pushButtonShowTrack->isChecked();
    //  2018_07_02  komKomC.showSimulationTarget = ui->pushButtonShowTarget->isChecked();

    komKomC.remoteControlZRK = ui->pushButtonZRK_RemoteControl->isChecked();

    //  П Е Р Е К Л Ю Ч Е Н И Е   Я З Ы К О В
    komKomC.languageIndex = settings->languageIndex;

    if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT)
        komKomC.targetIndexCY = -1;

    //  С И Н Х Р О Н И З А Ц И Я   К Н О П О К   В К Л Ю Ч Е Н И Я   П О И С К А   К О М А Н Д И Р А   И   О Ф И Ц Е Р А   Н А В Е Д Е Н И Я

//    qDebug()<<"        nextTochkaOptions: metka 1 shMemU2C.komKom.poiskState:"<<shMemU2C.komKom.poiskState
//           <<"shMemC2U.srvDrvInfo.rejimPoiskaSNR:"<<shMemC2U.srvDrvInfo.rejimPoiskaSNR;

    bool nadoPereklPoIniciativeOficeraNavedenia = false;

    if(ui->pushButtonBSP->isChecked())
    {
        //////////////////////ui->pushButtonRejimRabotiRR->setChecked(true);

        if(komKomC.poiskState < 200 || komKomC.poiskState > 290)
        {
            //qDebug()<<"nextTochkaOptions ui->pushButtonBSP->isChecked() командир, только переключил shMemU2C.komKom.poiskState:"<<shMemU2C.komKom.poiskState;
            komKomC.poiskState = 290;    //  инициатор переключения - командир, только переключил
        }
        else
        {
            if(komKomC.poiskState == 200)
            {
                if(console2User.apyInfo.rejimPoiskaSNR != POISK_BSP)
                {                    
                    //qDebug()<<"nextTochkaOptions ui->pushButtonBSP->isChecked() shMemU2C.komKom.poiskState == 200  shMemC2U.srvDrvInfo.rejimPoiskaSNR != POISK_BSP   ***   инициатор переключения - офицер наведения";
                    nadoPereklPoIniciativeOficeraNavedenia = true;  //  инициатор переключения - офицер наведения
                }
//                else
//                    qDebug()<<"nextTochkaOptions ui->pushButtonBSP->isChecked() shMemU2C.komKom.poiskState == 200  shMemC2U.srvDrvInfo.rejimPoiskaSNR == POISK_BSP      ничего не происходит";
            }
            else
            {
                if(console2User.apyInfo.rejimPoiskaSNR == POISK_BSP)
                {
                    //qDebug()<<"nextTochkaOptions ui->pushButtonBSP->isChecked() shMemU2C.komKom.poiskState != 200  shMemC2U.srvDrvInfo.rejimPoiskaSNR == POISK_BSP";
                    komKomC.poiskState = 200;
                }
                else
                {                    
                    komKomC.poiskState--;   //  инициатор переключения - командир, ждет подтверждения от AWSG
                }
            }
        }
    }

    if(ui->pushButtonMSP->isChecked())
    {
        if(komKomC.poiskState < 300 || komKomC.poiskState > 390)
        {
            komKomC.poiskState = 390;    //  инициатор переключения - командир, только переключил
        }
        else
        {
            if(komKomC.poiskState == 300)
            {
                if(console2User.apyInfo.rejimPoiskaSNR != POISK_MSP)
                {
                    nadoPereklPoIniciativeOficeraNavedenia = true;  //  инициатор переключения - офицер наведения
                }
            }
            else
            {
                if(console2User.apyInfo.rejimPoiskaSNR == POISK_MSP)
                {
                    komKomC.poiskState = 300;
                }
                else
                {
                    komKomC.poiskState--;   //  инициатор переключения - командир, ждет подтверждения от AWSG
                }
            }
        }
    }


    if(ui->pushButtonServoDrive_Off->isChecked())
    {
        if(komKomC.poiskState < 500 || komKomC.poiskState > 590)
        {            
            //qDebug()<<"nextTochkaOptions ui->pushButtonServoDrive_Off->isChecked() командир, только переключил shMemU2C.komKom.poiskState:"<<shMemU2C.komKom.poiskState;
            komKomC.poiskState = 590;    //  инициатор переключения - командир, только переключил
        }
        else
        {
            if(komKomC.poiskState == 500)
            {                
                if(console2User.apyInfo.rejimPoiskaSNR != POISK_VIKLUCHEN)
                {                    
                    //qDebug()<<"nextTochkaOptions ui->pushButtonServoDrive_Off->isChecked() shMemU2C.komKom.poiskState == 500  shMemC2U.srvDrvInfo.rejimPoiskaSNR != POISK_VIKLUCHEN   ***   инициатор переключения - офицер наведения";
                    nadoPereklPoIniciativeOficeraNavedenia = true;  //  инициатор переключения - офицер наведения
                }
                //else
                  //  qDebug()<<"nextTochkaOptions ui->pushButtonServoDrive_Off->isChecked() shMemU2C.komKom.poiskState != 500  shMemC2U.srvDrvInfo.rejimPoiskaSNR == POISK_VIKLUCHEN      ничего не происходит";
            }
            else
                if(console2User.apyInfo.rejimPoiskaSNR == POISK_VIKLUCHEN)
                {
                    //qDebug()<<"nextTochkaOptions ui->pushButtonServoDrive_Off->isChecked() shMemU2C.komKom.poiskState != 500  shMemC2U.srvDrvInfo.rejimPoiskaSNR == POISK_VIKLUCHEN";
                    komKomC.poiskState = 500;
                }
                else
                {                    
                    //qDebug()<<"nextTochkaOptions ui->pushButtonServoDrive_Off->isChecked() shMemU2C.komKom.poiskState != 500  shMemC2U.srvDrvInfo.rejimPoiskaSNR != POISK_VIKLUCHEN   ###   инициатор переключения - командир, ждет подтверждения от AWSG";
                    komKomC.poiskState--;   //  инициатор переключения - командир, ждет подтверждения от AWSG
                }
        }
    }

    if(nadoPereklPoIniciativeOficeraNavedenia)
    {
        //qDebug()<<"nadoPereklPoIniciativeOficeraNavedenia shMemC2U.srvDrvInfo.rejimPoiskaSNR:"<<shMemC2U.srvDrvInfo.rejimPoiskaSNR;
        switch(console2User.apyInfo.rejimPoiskaSNR)
        {
            case POISK_PEREKLYCHEN_V_ZAHVAT: case POISK_PEREKLYCHEN_V_SOPROVOJDENIE: ui->pushButtonZhvt->setChecked(true); komKomC.poiskState = 600; break;
            case POISK_VIKLUCHEN: ui->pushButtonServoDrive_Off->setChecked(true); komKomC.poiskState = 500; break;
            case POISK_BSP: ui->pushButtonBSP->setChecked(true); komKomC.poiskState = 200; break;
            case POISK_MSP: ui->pushButtonMSP->setChecked(true); komKomC.poiskState = 300; break;
            case POISK_Kryg: ui->pushButtonKrygSP->setChecked(true); komKomC.poiskState = 100;
        }

    }

}


