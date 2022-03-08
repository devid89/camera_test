#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_camera(QCameraInfo::defaultCamera())
    , m_imageCapture(&m_camera)
    , filters(nullptr)
    , work(true)
{
    ui->setupUi(this);

    frams = 0;
    connect(&timer, &QTimer::timeout, this, &MainWindow::slot_timeout);
    timer.start(1000);

    connect(this, &MainWindow::signal_start, this, &MainWindow::slot_start);

    connect(&m_imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::slot_capture);

    m_camera.setCaptureMode(QCamera::CaptureStillImage);

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

    /// Разтянть на весь экран
    QImage img = _image.scaled(this->width(), this->height());
    /// Отрисовка кадра
    painter.drawImage(0, 0, img);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    exit(0);
}

void MainWindow::slot_capture(int, const QImage &image)
{
    /// Создать объект Filters
    /// Создаем тут, так как нужен размер изображения, а получаем мы его только в этой функции
    if(!filters)
        filters = new Filters(image.width(), image.height());

    /// Сохранить полученное изображение
    _image = image;

    /// Обновить координаты квадрата
    filters->update(100, 100, 200, 150);

    /// Нарисовать квадрат
    filters->filtration(_image.bits());

    /// Расчет FPS
    frams++;

    /// Отрисовка на форме
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

