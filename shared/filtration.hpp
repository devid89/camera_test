/** Библиотека фильтрации лица с целью его (лица) "улучшения":) **/
#pragma once
#include <math.h>
#include <QImage>
#include <QFile>
#include <time.h>
#include <thread>
#include <vector>
#include "mat.hpp"

// Данные подмены лица на "идеальное".
static const int X = 80;
static const int Y = 80;
static uint8_t ideal[X][Y][4];


/// Класс фильтрации изображения для "улучшения" лица.
class Filter {


public:

    /// Инициализационные процедуры.
    Filter()
    {
        std::ifstream in("../_filters/ideal.bin", std::ios_base::binary);
        in.read((char*)&ideal, sizeof ideal);
        in.close();
    }

    /// Фильтрация изображения.
    void filtrate(int     r[Width][Height],
                  int     g[Width][Height],
                  int     b[Width][Height],
                  Trans   trans,
                  double  alfa_ideal  = 0.5,
                  double  alfa_border = 0.5)
    {
//        filter_color(color);
//        filter_blur (blur );
//        filter_shape(shape);

        for (int i = 0; i < Width;  i++) {
        for (int j = 0; j < Height; j++) {

            // Перевод в СК шаблона.
            int x = (trans.x(i, j) * (X - 1)) / (AccX - 1);
            int y = (trans.y(i, j) * (Y - 1)) / (AccY - 1);

            // Замена на идеальное лицо.
            if ((x >= 0) && (x < X) &&
                (y >= 0) && (y < Y)) {
                double w = alfa_ideal * ideal[x][y][3] / 255.0;
                r[i][j] = (1.0 - w) * r[i][j] + alfa_ideal * ideal[x][y][0] * w;
                g[i][j] = (1.0 - w) * g[i][j] + alfa_ideal * ideal[x][y][1] * w;
                b[i][j] = (1.0 - w) * b[i][j] + alfa_ideal * ideal[x][y][2] * w;
            }

            // Отрисовка рамочки.
            if (((x == -1) && (y >= -1) && (y <= Y))
             || ((x ==  X) && (y >= -1) && (y <= Y))
             || ((y == -1) && (x >= -1) && (x <= X))
             || ((y ==  Y) && (x >= -1) && (x <= X))) {
                r[i][j] = (1 - alfa_border) * r[i][j] + alfa_border * 0;
                g[i][j] = (1 - alfa_border) * g[i][j] + alfa_border * 127;
                b[i][j] = (1 - alfa_border) * b[i][j] + alfa_border * 0;
            }
        }}
    }


};
