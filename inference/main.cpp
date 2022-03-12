#include <math.h>
#include <QImage>
#include <QFile>
#include <time.h>
#include <thread>
#include <vector>
#include "../shared/recognition.hpp"
#include "../shared/filtration.hpp"


/// Программа "улучшения" изображения лица.
int main()
{
    /// Массивы изображения.
    int  r[Width][Height];
    int  g[Width][Height];
    int  b[Width][Height];

    // Один раз инициализируем параметры распознавания.
    init();

    Filter filter;

    // Обработаем изображения.
    for (int f = -4;  f <= 21;  f++) {
        clock_t time = clock();

        // Считаем новый файл и создадим индексированные карты цветов.
        QImage in(QString("../_dataset/%1.png").arg(f));
        for (int i = 0; i < Width;  i++) {
        for (int j = 0; j < Height; j++) {
            QRgb rgb = in.pixel(i,j);
            r[i][j] = qRed  (rgb);
            g[i][j] = qGreen(rgb);
            b[i][j] = qBlue (rgb);
        }}

        // Распознаем изображение.
        Trans trans(in.width() / 2, 0,
                    in.width() / 2, in.height(),
                    0, 0,
                    0, AccY-1);
        int res_w = recognize(&trans, r, g, b);
        std::cout << "Image #" << f << " dt=" << (clock() - time) / 1e6 << " w=" << res_w << std::endl;

        // Фильтранем изображение в распознанной области.
        filter.filtrate(r, g, b, trans);

        // Сохраним изображение в файл.
        QImage out(Width, Height, QImage::Format_RGB888);
        for (int i = 0; i < Width;  i++) {
        for (int j = 0; j < Height; j++) {
            out.setPixel(i, j, qRgb(r[i][j], g[i][j], b[i][j]));
        }}
        out.save(QString("../_result/%1_.png").arg(f));
    }

    return 0;
}
