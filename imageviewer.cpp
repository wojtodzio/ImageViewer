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

    updateActions(false);

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
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
         QImage image(fileName);
         if (image.isNull()) {
             QMessageBox::information(this,
                                      tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(fileName));

             return;
         }

         imageLabel->setPixmap(QPixmap::fromImage(image));
         scaleFactor = 1.0;

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
