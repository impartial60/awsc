#ifndef QLABELM_H
#define QLABELM_H

//#include <QString>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QTabletEvent>

class qLabelM : public QLabel
{
    Q_OBJECT
public:
    explicit qLabelM(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void tabletEvent(QTabletEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent * e);
    void leaveEvent(QEvent * e);
    void mouseMoveHandler(int mouse_x,int mouse_y);
    bool isMouseHover()const;

    int x, y, doubleClickX;
    bool pressEvent;
    bool visibleMarker;
    float speed;
    bool controlModifierIsHoldingDown;

    QString parentName;
    int cursorHotPointX;
    int cursorHotPointY;

signals:
    void Mouse_Pos(int, int);
    void Mouse_Click();
    void Mouse_Click(double);
    void Mouse_Click(QMouseEvent *ev);
    void doubleClicked(int, int);

public slots:

    
};

#endif // QLABELM_H
