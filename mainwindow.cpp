#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_camera(QCameraInfo::defaultCamera())
    , m_imageCapture(&m_camera)
    , work(true)
{
    ui->setupUi(this);

    frams = 0;
    connect(&timer, &QTimer::timeout, this, &MainWindow::slot_timeout);
    timer.start(1000);

    connect(this, &MainWindow::signal_start, this, &MainWindow::slot_start);

    connect(&m_imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::slot_capture);

    m_camera.setCaptureMode(QCamera::CaptureVideo);

    m_camera.start();
    m_imageCapture.setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    label = new QLabel();
    ui->statusBar->addWidget(label);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    emit signal_start();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    /// Отрисовка кадра
    painter.drawImage(0, 0, _image);

    /// Рисовать на кадре
    QPen pen = painter.pen();
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(10);
    painter.setPen(pen);

    /// рисуем круг
    painter.drawEllipse(this->width()/2 - 50, this->height()/2 - 50, 100, 100);

}

void MainWindow::closeEvent(QCloseEvent *)
{
    exit(0);
}

void MainWindow::slot_capture(int id, const QImage &image)
{
    _image = image;

    frams++;
    repaint();
}

void MainWindow::slot_start()
{
    while(work)
    {
        m_imageCapture.capture();
        QApplication::processEvents();
    }
}

void MainWindow::slot_timeout()
{
    label->setText(QString("%1 fps").arg(frams));
    frams = 0;
}

