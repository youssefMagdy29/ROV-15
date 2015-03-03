#ifndef RESIZABLE_LABEL
#define RESIZABLE_LABEL

#include <QLabel>

class ResizableLabel : public QLabel {
    Q_OBJECT

public:
    ResizableLabel(QWidget *parent = 0);
    void setImage(QImage *image);

private:
    QImage *image;

protected:
    void resizeEvent(QResizeEvent *e);
};


#endif // RESIZABLE_LABEL

