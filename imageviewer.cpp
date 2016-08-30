#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    actionOpen = ui->actionOpen;

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

void ImageViewer::on_actionOpen_triggered()
{
    qDebug() << "open()";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QDir::currentPath());
    if (!fileName.isEmpty()) {
         QImage image(fileName);
         if (image.isNull()) {
             QMessageBox::information(this,
                                      tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(fileName));

             return;
         }
         ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    }
}
