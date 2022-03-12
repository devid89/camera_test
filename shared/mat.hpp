#pragma once
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <float.h>

// Обшие настройки.
const double GradCoef = 1.166;         // ij,ji=1.166  i,i=7
const int    RGBdiv   = 24;            // 16COOL  24SOSO
const int    RGB      = 256 / RGBdiv;
const int    AccX     = 11;  // work=11, replace=80
const int    AccY     = 11;  // work=11, replace=80
const int    Keys     = AccX * AccY;

// Настройки изображения.
const int  Width  = 200;
const int  Height = 200;

// Специфичные настройки распознавания.
static const double AccX1 = 0.2 * (AccX - 1);  // 0.2
static const double AccY1 = 0.2 * (AccY - 1);  // 0.2
static const double AccX2 = 0.8 * (AccX - 1);  // 0.8
static const double AccY2 = 0.2 * (AccY - 1);  // 0.2

// Веса ключевых точек.
static uint8_t wgh[AccX][AccY][RGB][RGB][RGB][RGB];

/// Расчет серого по цветам.
inline int gray(int r, int g, int b)
{
    return (r * 11 + g * 16 + b * 5) / 32;
}

/// Расчет градиента по двум разностям.
inline int grad(int di, int dj, int dij, int dji)
{
    int res = GradCoef * (3 * abs(di) + 3 * abs(dj) + abs(dij) + abs(dji));
    return ((res > 255) ? 255 : res);
}


/// Класс трансформации масштаба, поворота и смещения.
class Trans
{
    double co, si, dx, dy, scale, det;

public:

    /// Инициализируем преобразование нулями.
    Trans()
    {

    }

    /// Инициализируем преобразование координатами двух точек.
    Trans(int i1, int j1,
          int i2, int j2,
          double x1 = AccX1, double y1 = AccY1,
          double x2 = AccX2, double y2 = AccY2) {

        // Вектора масштаба-поворота шблона и изображения.
        double x = x1 - x2;
        double y = y1 - y2;
        scale = sqrt(x * x + y * y);
        if (fabs(scale) <= FLT_MIN) {
            si = co = 0.0;
            scale = 1.0;
//            std::cout << "ERR SCALE!!!" << std::endl;
        }

        // Направляющие синусы и косинусы с масштабом.
        int i = i1 - i2;
        int j = j1 - j2;
        co = (i * x + j * y) / scale;
        si = (j * x - i * y) / scale;
        det = si * si + co * co;

        // Если детерминант мал, то обнулим компоненты и переопределим детерминант.
        if (fabs(det) <= FLT_MIN) {
            si = co = 0.0;
            det = 1.0;
//            std::cout << "ERR DETERM!!!" << std::endl;
        }

        // Перенос СК.
        double avg_x = 0.5 * (x1 + x2);
        double avg_y = 0.5 * (y1 + y2);
        double avg_i = 0.5 * (i1 + i2);
        double avg_j = 0.5 * (j1 + j2);
        dx = avg_x - scale * (co * avg_i + si * avg_j) / det;
        dy = avg_y - scale * (co * avg_j - si * avg_i) / det;
    }

    /// Проверка валидности преобразований.
    bool valid()
    {
        return !((si == 0.0) && (co == 0.0));
    }

    /// Координаты точки в СК шаблона (ПРОВЕРЕНО).
    double x(int i, int j) {return scale * (co * i + si * j) / det + dx;}
    double y(int i, int j) {return scale * (co * j - si * i) / det + dy;}

    /// Координаты точки в СК изображения.
    int i(int x, int y) {return round((co * (x - dx) - si * (y - dy)) / scale);}
    int j(int x, int y) {return round((si * (x - dx) + co * (y - dy)) / scale);}
};
