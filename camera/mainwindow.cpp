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

    _thread.start();
    thread_recognize.moveToThread(&_thread);

    frams = 0;
    connect(&timer, &QTimer::timeout, this, &MainWindow::slot_timeout, Qt::QueuedConnection);
    timer.start(1000);

    connect(this, &MainWindow::signal_start, this, &MainWindow::slot_start, Qt::QueuedConnection);

    connect(&m_imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::slot_capture, Qt::QueuedConnection);
    connect(&m_imageCapture, &QCameraImageCapture::imageCaptured, &thread_recognize, &thread_recognize_t::slot_capture, Qt::QueuedConnection);

    m_camera.setCaptureMode(QCamera::CaptureMode::CaptureStillImage);  // CaptureStillImage

    m_camera.start();
    m_imageCapture.setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    label = new QLabel();
    ui->statusBar->addWidget(label);

    init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    emit signal_start();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    if(_image.width() == 0) return;
    QPainter painter(this);

    /// Разтянть на весь экран
    QImage img = _image.scaled(this->width(), this->height());
    /// Отрисовка кадра
    painter.drawImage(0, 0, img);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    work = false;
    _thread.exit();
    QMainWindow::closeEvent(event);
}

void MainWindow::slot_capture(int, const QImage &image)
{
    if(image.width() == 0) return;
    /// Изменить размер фото на 200х200, наверно потом это нужно как изменить

    int x = 0;
    int y = 0;
    int w = image.width();
    int h = image.height();
    if(w>h) {
        x = (w-h) / 2;
        w = h;
    } else if(h> w) {
        y = (h-w) / 2;
        h = w;
    }
    _image = image.copy(x,y,w,h).scaled(Width, Height);

    /// Массивы изображения.
    static int  r[Width][Height];
    static int  g[Width][Height];
    static int  b[Width][Height];

    for (int i = 0; i < Width;  i++) {
            for (int j = 0; j < Height; j++) {
                QRgb rgb = _image.pixel(i,j);
                r[i][j] = qRed  (rgb);
                g[i][j] = qGreen(rgb);
                b[i][j] = qBlue (rgb);
            }}


//    // Распознаем изображение.
//    Trans trans(Width / 2, 0,
//                Width / 2, Height,
//                0, 0,
//                0, AccY-1);
//    int res_w = recognize(&trans, r, g, b);
//    std::cout << "Image #" << f << " dt=" << (clock() - time) / 1e6 << " w=" << res_w << std::endl;

    Trans trans = thread_recognize.get_trans();

    // Фильтранем изображение в распознанной области.
    filter.filtrate(r, g, b, trans, (double)ui->horizontalSlider->value() / 100.0, (double)ui->horizontalSlider_2->value() / 100.0);

    // Сохраним изображение
    for (int i = 0; i < Width;  i++) {
        for (int j = 0; j < Height; j++) {
            _image.setPixel(i, j, qRgb(r[i][j], g[i][j], b[i][j]));
        }}
qDebug() << "new image";
    frams++;    /// Расчет FPS
    repaint();    /// Отрисовка на форме
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

