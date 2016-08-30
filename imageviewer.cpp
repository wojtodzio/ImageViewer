#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    QImage image("/Users/Wojtek/Pictures/Three-eyed_crow.jpg");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    setWindowTitle(tr("Image Viewer"));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
