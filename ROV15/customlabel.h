#ifndef CUSTOMLABEL
#define CUSTOMLABEL

#include <QLabel>

class customLabel : public QLabel {
    Q_OBJECT

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

signals:
    void lineFinished(int startX, int startY,
                      int endX  , int endY);
};

#endif // CUSTOMLABEL

