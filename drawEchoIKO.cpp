#include <QtGui>
#include "mainWindowC.h"
#include "ui_mainwindow.h"

void MainWindow::drawEchoZoomIKO(QPainter *painter)
{
    float postLightFactor = 0.99;

    for(int j = 0; j < DEST_ZOOM_RECT_WIDTH_HEIGHT; j ++)                 // ПОСЛЕСВЕЧЕНИЕ
    {
        for(int i = 0; i < DEST_ZOOM_RECT_WIDTH_HEIGHT; i ++)
        {
            *(yarkostEchoZoom + j * DEST_ZOOM_RECT_WIDTH_HEIGHT + i) *= postLightFactor;
        }
    }

    QPainter painterEcho(&zoomIKOEchoBMP);
    painterEcho.fillRect(0, 0, zoomIKOEchoBMP.width(), zoomIKOEchoBMP.height(), QBrush(QColor(0, 0, 0, 255)));

    painterEcho.setRenderHint(QPainter::Antialiasing, true);

    // П С Е В Д О Р А З В Ё Р Т К А
    painterEcho.setPen(QPen(QColor::fromRgb(0, 32, 0, 255), 15, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));

    //float radiusIKO = centerZoomIKO.x() - 120;
    //factorR_IKO_Zoom = 1.0 / zoomRanges[ui->sliderZoom->value()] * radiusIKO;
    //factorR_IKO = factorR_IKO_Zoom;
    //fnt.setPointSize(10);
    //QPoint centerIKO = centerZoomIKO;

    //QPoint centerIKO(zoomFactor * zoomIKO_x - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2 - centerZoomIKO.x(), zoomFactor * zoomIKO_y - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2 - centerZoomIKO.y());
    //QPoint centerIKO(zoomFactor * zoomIKO_x - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2 + centerZoomIKO.x(), zoomFactor * zoomIKO_y - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2 + centerZoomIKO.y());

    QPoint centerIKO(centerZoomIKO.x() - (zoomFactor * zoomIKO_x - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2), centerZoomIKO.y() - (zoomFactor * zoomIKO_y - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2));

//    qDebug()<<"drawEchoZoomIKO: dest:"<<zoomFactor * zoomIKO_x - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2<<"centerZoomIKO.x():"<<centerZoomIKO.x()<<"="
//            <<centerZoomIKO.x() - (zoomFactor * zoomIKO_x - DEST_ZOOM_RECT_WIDTH_HEIGHT / 2);

    float azRadians = console2User.srvDrvInfo.currentAzimuth / Rad2Grad;
    float radiusIKO = centerZoomIKO.x() - 30;
    float dXIKO =    radiusIKO * sin(azRadians);
    float dYIKO =  - radiusIKO * cos(azRadians);
    QPoint borderPointIKO = centerIKO + QPoint(dXIKO / 1, dYIKO / 1);
    painterEcho.drawLine(centerIKO, borderPointIKO);

    if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
    {
        // МДО-1:   ПРОРИСОВКА РАДИАЛЬНЫХ СЕЧЕНИЙ ДУЖЕК ЦЕЛЕЙ

        for(int j = 0; j < MAX_MDO1_TARGETS_COUNT; j++)
        {
            if(console2User.joinedTrgtData[j].range == 0)
                break;

            float greenBrghtns = console2User.joinedTrgtData[j].sredEnF;
            if(console2User.joinedTrgtData[j].sredEnF > 1.0)
            {
                qDebug()<<"drawMainIKO: greenBrghtns:"<<greenBrghtns;
                greenBrghtns = 1.0;
            }

            if(console2User.joinedTrgtData[j].sredEnF < 0.0)
            {
                qDebug()<<"drawMainIKO: greenBrghtns:"<<greenBrghtns;
                greenBrghtns = 0.0;
            }

            //painter->setPen(QPen(QColor::fromRgbF(0, console2User.joinedTrgtData[j].sredEnF, 0.0, 1.0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painterEcho.setPen(QPen(QColor::fromRgbF(0.0, greenBrghtns, 0.0, 1.0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            float scrTrgtR1 = (console2User.joinedTrgtData[j].range_km - 0.1) * factorR_IKO_Zoom;
            float scrTrgtR2 = (console2User.joinedTrgtData[j].range_km + 0.1) * factorR_IKO_Zoom;
            QPointF scr1 = QPointF(scrTrgtR1 * sin(azRadians), - scrTrgtR1 * cos(azRadians)) + centerIKO;
            QPointF scr2 = QPointF(scrTrgtR2 * sin(azRadians), - scrTrgtR2 * cos(azRadians)) + centerIKO;
            painterEcho.drawLine(scr1, scr2);
        }

        QImage chip = zoomIKOEchoBMP.toImage();

        for (int j = 0; j < DEST_ZOOM_RECT_WIDTH_HEIGHT; j++)
        {
            for (int i = 0; i < DEST_ZOOM_RECT_WIDTH_HEIGHT; i++)
            {
                int addrYarkost = j * DEST_ZOOM_RECT_WIDTH_HEIGHT + i;
                int addrChip = addrYarkost * 4 + 1;

                if(*(chip.bits() + addrChip) > *(yarkostEchoZoom + addrYarkost))
                {
                    *(yarkostEchoZoom + addrYarkost) = *(chip.bits() + addrChip);
                }

                *(imgDataZoom + addrYarkost * 3 + 1) = (unsigned char)(*(yarkostEchoZoom + addrYarkost));
            }
        }
    }

    QPixmap pixmapEcho = QPixmap::fromImage(QImage((unsigned char *) imgDataZoom, DEST_ZOOM_RECT_WIDTH_HEIGHT, DEST_ZOOM_RECT_WIDTH_HEIGHT, QImage::Format_RGB888));
    //painter->drawPixmap(0, 0, pixmapEcho);
    painter->drawPixmap(sourceRect2DrawZoomIKO, pixmapEcho);
    //painter->drawPixmap(destRect2DrawZoomIKO, pixmapEcho);

    //painter->setPen(QPen(QColor::fromRgb(255, 32, 0, 128), 30, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
//    painter->drawLine(sourceRect2DrawZoomIKO.left(), sourceRect2DrawZoomIKO.top(),
//                      sourceRect2DrawZoomIKO.left() + DEST_ZOOM_RECT_WIDTH_HEIGHT, sourceRect2DrawZoomIKO.top() + DEST_ZOOM_RECT_WIDTH_HEIGHT);
}


void MainWindow::drawEchoMainIKO(QPainter *painter)
{
    float postLightFactor = 0.99;

    for(int j = 0; j < IKO_WIDTH_HEIGHT; j ++)                 // ПОСЛЕСВЕЧЕНИЕ
    {
        for(int i = 0; i < IKO_WIDTH_HEIGHT; i ++)
        {
            *(yarkostEchoMain + j * IKO_WIDTH_HEIGHT + i) *= postLightFactor;
            ///////////////////*(imgDataEB + (j * IKO_WIDTH_HEIGHT + i) * 3 + 1) =  (unsigned char)(*(yarkostEB + j * IKO_WIDTH_HEIGHT + i));
        }
    }

    QPainter painterEcho(&mainIKOEchoBMP);
    painterEcho.fillRect(0, 0, mainIKOEchoBMP.width(), mainIKOEchoBMP.height(), QBrush(QColor(0, 0, 0, 255)));//::fromRgbF(1.0, 1.0, 0.0, 1.0)));//Qt::black);

    painterEcho.setRenderHint(QPainter::Antialiasing, true);

    // П С Е В Д О Р А З В Ё Р Т К А
    painterEcho.setPen(QPen(QColor::fromRgb(0, 32, 0, 255), 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));


    float azRadians = console2User.srvDrvInfo.currentAzimuth / Rad2Grad;
    float radiusIKO = centerMainIKO.x() - 30;
    float dXIKO =    radiusIKO * sin(azRadians);
    float dYIKO =  - radiusIKO * cos(azRadians);
    QPoint borderPointIKO = centerMainIKO + QPoint(dXIKO / 1, dYIKO / 1);
    painterEcho.drawLine(centerMainIKO, borderPointIKO);

    if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
    {
        // МДО-1:   ПРОРИСОВКА РАДИАЛЬНЫХ СЕЧЕНИЙ ДУЖЕК ЦЕЛЕЙ

        for(int j = 0; j < MAX_MDO1_TARGETS_COUNT; j++)
        {
            if(console2User.joinedTrgtData[j].range == 0)
                break;

            float greenBrghtns = console2User.joinedTrgtData[j].sredEnF;
            if(console2User.joinedTrgtData[j].sredEnF > 1.0)
            {
                qDebug()<<"drawMainIKO: greenBrghtns:"<<greenBrghtns;
                greenBrghtns = 1.0;
            }

            if(console2User.joinedTrgtData[j].sredEnF < 0.0)
            {
                qDebug()<<"drawMainIKO: greenBrghtns:"<<greenBrghtns;
                greenBrghtns = 0.0;
            }

            //painter->setPen(QPen(QColor::fromRgbF(0, console2User.joinedTrgtData[j].sredEnF, 0.0, 1.0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painterEcho.setPen(QPen(QColor::fromRgbF(0.0, greenBrghtns, 0.0, 1.0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            float scrTrgtR1 = (console2User.joinedTrgtData[j].range_km - 0.1) * factorR_IKO_Main;   //factorR_IKO;
            float scrTrgtR2 = (console2User.joinedTrgtData[j].range_km + 0.1) * factorR_IKO_Main;
            QPointF scr1 = QPointF(scrTrgtR1 * sin(azRadians), - scrTrgtR1 * cos(azRadians)) + centerMainIKO;// centerIKO;
            QPointF scr2 = QPointF(scrTrgtR2 * sin(azRadians), - scrTrgtR2 * cos(azRadians)) + centerMainIKO;
            painterEcho.drawLine(scr1, scr2);
        }

        QImage chip = mainIKOEchoBMP.toImage();

        for (int j = 0; j < IKO_WIDTH_HEIGHT; j++)
        {
            for (int i = 0; i < IKO_WIDTH_HEIGHT; i++)
            {
                int addrYarkost = j * IKO_WIDTH_HEIGHT + i;
                int addrChip = addrYarkost * 4 + 1;

                if(*(chip.bits() + addrChip) > *(yarkostEchoMain + addrYarkost))
                {
                    *(yarkostEchoMain + addrYarkost) = *(chip.bits() + addrChip);
                }

                *(imgDataMain + addrYarkost * 3 + 1) = (unsigned char)(*(yarkostEchoMain + addrYarkost));
            }
        }
    }

    QPixmap pixmapEcho = QPixmap::fromImage(QImage((unsigned char *) imgDataMain, IKO_WIDTH_HEIGHT, IKO_WIDTH_HEIGHT, QImage::Format_RGB888));
    painter->drawPixmap(0, 0, pixmapEcho);
}

