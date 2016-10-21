#ifndef SELECCIONARROI_H
#define SELECCIONARROI_H

#include <QLabel>
#include <QMenu>
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>

class SeleccionarROI : public QLabel
{
    Q_OBJECT

public:
    SeleccionarROI(QWidget *parent = 0);
    ~SeleccionarROI();

    void setOriginalImageSize(QSize imgSize);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    QMenu contextMenu;

    bool selectionStarted;
    QPoint startPoint;
    QPoint endPoint;
    QRect showedRect;

    // Variables to control ROI rectangle when resizing window
    float proportionWidth,proportionHeight;
    QRect originalSelectedRect;
    QRect rectangleMappedOnImage;
    QSize originalImageSize;

signals:
    void setNewROI(QRect roi);

private slots:
    void saveSlot();
    void addROISlot();
};

#endif // SELECCIONARROI_H
