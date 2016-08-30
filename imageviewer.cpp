#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    QImage image("/Users/Wojtek/Pictures/Three-eyed_crow.jpg");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}
