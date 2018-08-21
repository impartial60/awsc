#include "qlabelm.h"
#include <QApplication>
#include <QDebug>
//#include <QObject>

qLabelM::qLabelM(QWidget *parent) :
    QLabel(parent)
{
    visibleMarker = false;
    cursorHotPointX = 2;
    cursorHotPointY = 0;//27;
    controlModifierIsHoldingDown = false;
    //qDebug()<<"qLabelM::controlModifierIsHoldingDown:"<<parent->objectName()<<controlModifierIsHoldingDown;
}

void qLabelM::mouseReleaseEvent(QMouseEvent *ev)
{
    controlModifierIsHoldingDown = false;
}


void qLabelM::mouseMoveEvent(QMouseEvent *ev)
{
    mouseMoveHandler(ev->x(),ev->y());
}

void qLabelM::tabletEvent(QTabletEvent *ev)
{
    if(ev->type() == QEvent::TabletMove)
    {
        mouseMoveHandler(ev->x(),ev->y());
    }
}

void qLabelM::mousePressEvent(QMouseEvent *ev)
{
    double delitel = 3.0;

    if(objectName() == "azimuthScale3600" || objectName() == "elevationScale870")
    {
        delitel = 30.0;
    }

    int scrX = ev->x() - width() / 2 - cursorHotPointX;
    double deltaAzimHvr = 1 / delitel * scrX;

    if(objectName() == "elevationScale87" || objectName() == "elevationScale870")
    {
        scrX = - ev->y() + height() / 2 + cursorHotPointY;
        deltaAzimHvr = 1 / delitel * scrX;
    }

    //////////////////////////qDebug()<<"qLabelM::mousePressEvent:"<<ev->modifiers();

    //controlModifierIsPressed = false;
    if(Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        controlModifierIsHoldingDown = true;
        speed = 0.1 * deltaAzimHvr;
    }

    //if (ev->button() == Qt::LeftButton)

    if(objectName() == "mainIndicator" || objectName() == "IKO")
    {
        emit Mouse_Click(ev);//ev->x(), ev->y());
        //qDebug()<<"qLabelM::mousePressEvent ev->button():"<<ev->button(); //Qt::MouseButton::LeftButton;
    }
    else
        if(objectName() == "indicatorEB")
        {
            emit Mouse_Click();
        }
        else
        {
            emit Mouse_Click(deltaAzimHvr);//, controlModifierIsPressed);
        }
}

void qLabelM::mouseDoubleClickEvent(QMouseEvent* ev)
{
    this->doubleClickX = ev->x();
    if (ev->button() == Qt::LeftButton)
    {
        emit doubleClicked(ev->x() - cursorHotPointX , ev->y() - cursorHotPointY);
    }
}

void qLabelM::leaveEvent(QEvent *ev)
{
    if (ev->type() == QEvent::Leave)
        visibleMarker = false;

    //qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}

void qLabelM::mouseMoveHandler(int mouse_x, int mouse_y)
{
    this->x  = mouse_x;
    this->y  = mouse_y;

    int scrX = mouse_x - width() / 2 - cursorHotPointX;
    int scrY = - mouse_y + height() / 2 + cursorHotPointY;

    int hovMain = scrX;
    int hovDelt = scrX * 10;

    if(objectName() == "azimuthScale3600")
    {
        hovDelt = scrX;
        hovMain = scrX / 10;
    }

    if(objectName() == "elevationScale87")
    {
        hovMain = scrY;
        hovDelt = scrY * 10;

        //qDebug()<<"qLabelM::mouseMoveEvent: elevationScale360:"<<"hovMain:"<<hovMain;
    }

    if(objectName() == "elevationScale870")
    {
        hovDelt = scrY;
        hovMain = scrY / 10;
    }


    if(objectName() == "indicatorEB")
    {
        hovMain = mouse_x;
        hovDelt = mouse_y;
    }

    if(controlModifierIsHoldingDown)
    {
        double delitel = 30.0;

        if(objectName() == "azimuthScale360" || objectName() == "elevationScale87")
        {
            delitel = 3.0;
        }

        int scrX = mouse_x - width() / 2 - cursorHotPointX;
        int scrY = mouse_y - height() / 2 - cursorHotPointY;

        double deltaAzimHvr = 1 / delitel * scrX;

        if(objectName() == "elevationScale87" || objectName() == "elevationScale870")
            deltaAzimHvr = 1 / delitel * scrY;

        speed = 0.1 * deltaAzimHvr;
    }

    if(objectName() == "IKO")
    {
        hovMain = mouse_x;
        hovDelt = mouse_y;
    }


    visibleMarker = true;

//    if(objectName() == "mainIndicator" && ev->modifiers().testFlag(Qt::ShiftModifier)) //if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
//        this->setCursor(Qt::CrossCursor);
//    else
//        this->setCursor(Qt::ArrowCursor);

    emit Mouse_Pos(hovMain, hovDelt);//objectName());//parent()->objectName());
}

static bool is_point_belong_widget(const QWidget & widget,const QPoint & point )
{
    QRect box(widget.mapToGlobal(QPoint(0,0)),widget.size());
    return box.contains(point);
}

bool qLabelM::isMouseHover() const
{
    return is_point_belong_widget(*this,QCursor::pos());
}
