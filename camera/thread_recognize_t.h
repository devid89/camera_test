#pragma once

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include "../shared/filtration.hpp"
#include "../shared/recognition.hpp"
#include <unistd.h>

class thread_recognize_t : public QObject {
    Q_OBJECT
private:
    Trans _trans;
    QMutex _mutex;
public:
    thread_recognize_t()
        : _trans(Width / 2, 0,
                 Width / 2, Height,
                 0, 0,
                 0, AccY-1)
    {}

public slots:
    void slot_capture(int, const QImage& image)
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
        QImage _image = image.copy(x,y,w,h).scaled(Width, Height);

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


        // Распознаем изображение.
        Trans trans(Width / 2, 0,
                    Width / 2, Height,
                    0, 0,
                    0, AccY-1);
        int res_w = recognize(&trans, r, g, b);

        qDebug() << "recognize = " <<  res_w;

        _mutex.lock();
        _trans = trans;
        _mutex.unlock();

        /// специально сделал задержку, чтобы было видно что это работает в другом потоке
        sleep(1);

    }

    Trans get_trans() {
        QMutexLocker mutex_lock(&_mutex);
        return _trans;
    }

};
