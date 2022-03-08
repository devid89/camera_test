#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QDebug>
#include <QPainter>
#include <QDateTime>
#include <QLabel>
#include <QTimer>

#include "mat.hpp"
#include "filters.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void    start();

    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

    QCamera m_camera;
    QCameraImageCapture m_imageCapture ;

    Filters* filters;

    QImage _image;

    bool    work;
    uint16_t frams;
    QLabel  *label;
    QTimer  timer;

signals:
    void signal_start();

private slots:
    void slot_capture(int, const QImage& image);

    void    slot_start();

    void    slot_timeout();
};
#endif // MAINWINDOW_H
