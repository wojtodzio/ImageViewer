#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <qlabel>
#include <qscrollarea>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QScrollBar>
#include <QMouseEvent>
#include <QStatusBar>
#include <QVector>

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
    bool eventFilter(QObject* watched, QEvent* event);

    void on_actionCrop_triggered();
    void on_actionFullscreen_triggered();
    void on_actionPaintBlack_triggered();
    void on_actionOpen_triggered();
    void on_actionRotateLeft_triggered();
    void on_actionRotateRight_triggered();
    void on_actionSave_triggered();
    void on_actionShowToolbar_triggered(bool checked);
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();

private:
    Ui::ImageViewer *ui;

    QPoint croppingStart;
    QPoint croppingEnd;

    QLabel *imageLabel;

    QScrollArea *scrollArea;

    QString fileName;
    QImage image;

    QStatusBar *statusBar;

    QToolBar *mainToolBar;

    QVector<QImage> history;
    QVector<QImage> reverseHistory;

    QAction *actionCrop;
    QAction *actionOpen;
    QAction *actionPaintBlack;
    QAction *actionRotateLeft;
    QAction *actionRotateRight;
    QAction *actionSave;
    QAction *actionShowToolbar;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionZoomIn;
    QAction *actionZoomOut;

    double scaleFactor;
    bool croppingState;

    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void changeCroppingState(bool changeTo);
    void refreshLabel();
    void rotateImage(int angle);
    void saveToHistory(QImage imageToSave);
    void saveToHistoryWithClear(QImage imageToSave);
    void saveToReverseHistory(QImage imageToSave);
    void scaleImage(double factor);
    void updateActions(bool updateTo);
};

#endif // IMAGEVIEWER_H
