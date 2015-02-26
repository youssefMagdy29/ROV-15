#ifndef CUSTOMLABEL
#define CUSTOMLABEL

#include <QLabel>

class customLabel : public QLabel {

public:
    customLabel(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    int startX, startY;
    int endX  , endY;
    bool clicked;
};

#endif // CUSTOMLABEL

