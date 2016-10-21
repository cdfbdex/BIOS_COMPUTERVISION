#include "seleccionarroi.h"
#include "ui_seleccionarroi.h"

SeleccionarROI::SeleccionarROI(QWidget *parent)
    : QLabel(parent)
{
    setScaledContents(true);

    selectionStarted=false;

    QAction *addROIAction=contextMenu.addAction(tr("Agregar ROI"));
    QAction *saveAction=contextMenu.addAction(tr("Guardar"));

    connect(saveAction,SIGNAL(triggered()),this,SLOT(saveSlot()));
    connect(addROIAction,SIGNAL(triggered()),this,SLOT(addROISlot()));
}

SeleccionarROI::~SeleccionarROI()
{

}

void SeleccionarROI::setOriginalImageSize(QSize imgSize)
{
    // Establecer el tamaño original de la imagen a visualizar
    // con el objetivo de calcular posteriormente las proporciones
    // con el tamaño del Widget
    originalImageSize = imgSize;
    showedRect.setSize(QSize());
}

void SeleccionarROI::paintEvent(QPaintEvent *e)
{
    // Configuración del aspecto del rectángulo dibujado para delimitar el ROI
    QLabel::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,255,255,120)));

    painter.drawRect(showedRect);
    e->accept();
}

void SeleccionarROI::mousePressEvent(QMouseEvent *e)
{
    startPoint = e->pos();
    // Mostrar el menú contextual cuando el cursor se encuentre dentro del rectángulo seleccionado
    if (e->button()==Qt::RightButton)
    {
        if (showedRect.contains(startPoint))
            contextMenu.exec(this->mapToGlobal(startPoint));
    }
    else // Iniciar el dibujo de un nuevo rectángulo
    {
        selectionStarted=true;
    }
    e->accept();
}

void SeleccionarROI::mouseMoveEvent(QMouseEvent *e)
{
    if (selectionStarted)
    {
        showedRect.setTopLeft(startPoint);

        // Verificar si el cursor se encuentra dentro de los límites del Widget
        if(e->pos().x() >= 0 && e->pos().x() < this->size().width() &&
           e->pos().y() >= 0 && e->pos().y() < this->size().height() )
        {
            showedRect.setBottomRight(e->pos());
        }
        else    // Si el cursor está fuera de Widget
        {
            // Configurar X en los valores límites del Widget
            if(e->pos().x() >= this->size().width())
                showedRect.setRight(this->size().width()-1);
            else if(e->pos().x() < 0)
                showedRect.setRight(0);
            else
                showedRect.setRight(e->pos().x());

            // Configurar Y en los valores límites del Widget
            if(e->pos().y() >= this->size().height())
                showedRect.setBottom(this->size().height()-1);
            else if(e->pos().y() < 0)
                 showedRect.setBottom(0);
            else
                showedRect.setBottom(e->pos().y());
        }
        repaint();
    }
    e->accept();
}

void SeleccionarROI::mouseReleaseEvent(QMouseEvent *e)
{
    endPoint = e->pos();

    // Si se realizó click y no hubo un desplazamiento del cursor
    // Se borra el rectángulo creado anteriormente
    if(startPoint == endPoint)
    {
        // Verificar si el click se realizó dentro o fuera del rectángulo dibujado
        if(!showedRect.contains(endPoint))
        {
            // Borrar rectángulo cuando se presione click fuera del rectángulo
            showedRect = QRect();
            startPoint = QPoint();
            endPoint = QPoint();
            // Mapear los valores del rectángulo dibujado
            selectionStarted=false;
            originalSelectedRect = showedRect;  // Guardar las coordenadas dibujadas originalmente, en caso de que se reescale el Widget
            repaint();
        }
    }
    else
    {
        // Mapear los valores del rectángulo dibujado
        selectionStarted=false;
        originalSelectedRect = showedRect;  // Guardar las coordenadas dibujadas originalmente, en caso de que se reescale el Widget

        // Cálculo de proporciones entre el tamaño de visualización del Widget y el tamaño original de la imagen
        proportionWidth = float(originalImageSize.width()) / this->size().width();
        proportionHeight = float(originalImageSize.height()) / this->size().height();

        // Rectángulo que mapea el rectángulo seleccionado sobre la imagen de tamaño original
        QRect temp(originalSelectedRect.left()*proportionWidth, originalSelectedRect.top()*proportionHeight,
                   originalSelectedRect.width()*proportionWidth, originalSelectedRect.height()*proportionHeight);
        // Normalizar el rectángulo en caso de que el punto final del rect se encuentre n una posición menor al punto de origen.
        rectangleMappedOnImage = temp.normalized();
    }

    //qDebug() << "Proportions: " << proportionWidth << "/" << proportionHeight << " MAPPED RECT: " << rectangleMappedOnImage;
    e->accept();
}

void SeleccionarROI::mouseDoubleClickEvent(QMouseEvent *e)
{
//    // Borrar rectángulo con doble click dentro del rectángulo
//    if (showedRect.contains(e->pos()))
//    {
//        showedRect = QRect();
//        startPoint = QPoint();
//        endPoint = QPoint();
//        // Mapear los valores del rectángulo dibujado
//        selectionStarted=false;
//        originalSelectedRect = showedRect;  // Guardar las coordenadas dibujadas originalmente, en caso de que se reescale el Widget
//        repaint();
//    }
    e->accept();
}

void SeleccionarROI::resizeEvent(QResizeEvent *e)
{
    // Cuando se reescala la ventana (y el Widget) se dibuja nuevamente el rectángulo seleccionado
    // basado en las proporciones de la imagen original y el tamaño del Widget actual
    float newProportionWidth = float(e->size().width()) / originalImageSize.width();
    float newProportionHeight = float(e->size().height()) / originalImageSize.height();

    // Modificación del rectángulo mostrado basado en las nuevas medidas
    showedRect.setLeft(rectangleMappedOnImage.left() * newProportionWidth);
    showedRect.setWidth(rectangleMappedOnImage.width()*newProportionWidth);
    showedRect.setTop(rectangleMappedOnImage.top() * newProportionHeight);
    showedRect.setHeight(rectangleMappedOnImage.height()*newProportionHeight);
    e->accept();
}

///// SLOTS /////
void SeleccionarROI::saveSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save File"),
                                                    QDir::currentPath(),
                                                    QObject::tr("Images (*.jpg)"));
    this->pixmap()->copy(showedRect).save(fileName);
}

void SeleccionarROI::addROISlot()
{
    // Guardar un nuevo ROI basado en el rectángulo seleccionado por el usuario.
    emit setNewROI(rectangleMappedOnImage);

    //    qDebug() << "X: " << rectangleMappedOnImage.x();
    //    qDebug() << "Y: " << rectangleMappedOnImage.y();
    //    qDebug() << "W: " << rectangleMappedOnImage.width();
    //    qDebug() << "H: " << rectangleMappedOnImage.height();
}
