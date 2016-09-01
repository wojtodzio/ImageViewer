#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    actionOpen = ui->actionOpen;
    actionZoomIn = ui->actionZoomIn;
    actionZoomOut = ui->actionZoomOut;
    actionRotateLeft = ui->actionRotateLeft;
    actionRotateRight = ui->actionRotateRight;
    actionCrop = ui->actionCrop;
    actionSave = ui->actionSave;
    statusBar = ui->statusBar;

    updateActions(false);

    setMouseTracking(true);

    imageLabel = new QLabel;
    imageLabel->setMouseTracking(true);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->installEventFilter(this);

    scrollArea = new QScrollArea;
    scrollArea->setMouseTracking(true);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    setWindowTitle(tr("Image Viewer"));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::updateActions(bool updateTo)
{
    actionZoomIn->setEnabled(updateTo);
    actionZoomOut->setEnabled(updateTo);
    actionRotateLeft->setEnabled(updateTo);
    actionRotateRight->setEnabled(updateTo);
    actionCrop->setEnabled(updateTo);
    actionSave->setEnabled(updateTo);
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    actionZoomIn->setEnabled(scaleFactor < 3.0);
    actionZoomOut->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    int newValue = factor * scrollBar->value() + (factor - 1) * scrollBar->pageStep() / 2;
    scrollBar->setValue(newValue);
}

void ImageViewer::on_actionOpen_triggered()
{
    QString lastFileName = fileName.isEmpty() ? QDir::currentPath() : fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open File"),
                                                 lastFileName);
    if (!fileName.isEmpty()) {
         image = QImage(fileName);
         if (image.isNull()) {
             QMessageBox::information(this,
                                      tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(fileName));

             return;
         }

         imageLabel->setPixmap(QPixmap::fromImage(image));
         scaleFactor = 1.0;
         croppingImage = false;
         setCursor(Qt::ArrowCursor);
         updateActions(true);

         imageLabel->adjustSize();
    }
}

void ImageViewer::on_actionZoomIn_triggered()
{
    scaleImage(1.25);
}

void ImageViewer::on_actionZoomOut_triggered()
{
    scaleImage(0.80);
}

void ImageViewer::rotateImage(int angle)
{
    QPixmap pixmap(*imageLabel->pixmap());
    QMatrix rm;
    rm.rotate(angle);
    pixmap = pixmap.transformed(rm);
    image = pixmap.toImage();
    imageLabel->setPixmap(pixmap);
}

void ImageViewer::on_actionRotateLeft_triggered()
{
    rotateImage(-90);
}

void ImageViewer::on_actionRotateRight_triggered()
{
    rotateImage(90);
}

void ImageViewer::on_actionCrop_triggered()
{
    changeCroppingImage(true);
}

void ImageViewer::changeCroppingImage(bool changeTo)
{
    croppingImage = changeTo;
    actionCrop->setDisabled(changeTo);

    if (changeTo)
    {
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

bool ImageViewer::eventFilter(QObject* watched, QEvent* event)
{
    if (watched != imageLabel)
        return false;

    switch (event->type())
    {
        case QEvent::MouseButtonPress:
        {
            if (!croppingImage) break;
            const QMouseEvent* const me = static_cast<const QMouseEvent*>(event);
            croppingStart = me->pos() / scaleFactor;
            break;
        }

        case QEvent::MouseButtonRelease:
        {
            if (!croppingImage) break;
            const QMouseEvent* const me = static_cast<const QMouseEvent*>(event);
            croppingEnd = me->pos() / scaleFactor;

            QRect rect(croppingStart, croppingEnd);
            image = image.copy(rect);
            imageLabel->setPixmap(QPixmap::fromImage(image));
            imageLabel->adjustSize();
            scaleImage(1.0);

            changeCroppingImage(false);
            break;
        }

        case QEvent::MouseMove:
        {
            const QMouseEvent* const me = static_cast<const QMouseEvent*>(event);
            const QPoint position = me->pos();
            statusBar->showMessage(QString("(x,y) coordinates: (%1,%2)").arg(position.x()).arg(position.y()));
        }

        default:
            break;
    }

    return false;
}

void ImageViewer::on_actionSave_triggered()
{
    QString imagePath = QFileDialog::getSaveFileName(this,
                                                     tr("Save File"),
                                                     "",
                                                     tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));

    image.save(imagePath);
}
