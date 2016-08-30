#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <qlabel>
#include <qscrollarea>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::ImageViewer *ui;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QAction *actionOpen;
};

#endif // IMAGEVIEWER_H
