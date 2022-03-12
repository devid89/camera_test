/** Библиотека разпознавания лица с целью его (лица) "улучшения":) **/

#pragma once
#include <math.h>
#include <QImage>
#include <QFile>
#include <time.h>
#include <thread>
#include <vector>
#include "mat.hpp"

// Настройки распознавания.
static const double WidthMin = 0.3;  // Минимальная ширина шаблона на изображении.
static const double WidthMax = 1.0;  // Мaксимальная ширина шаблона на изображении.

// Глобальные переменные большого и не очень размера.
static int  key_w[Keys][RGB][RGB][RGB][RGB];
static int  key_x[Keys];
static int  key_y[Keys];

// Параметры объединения точек на изображении:
static const int Union  = 5;
static const int UnionI = Width  / Union;
static const int UnionJ = Height / Union;

// Отобранные на изображении точки шаблона:
static int pat_w[Keys];
static int pat_i[Keys];
static int pat_j[Keys];

// Накопители точек на изображении шаблона:
static int acc_w [Keys][UnionI][UnionJ];
static int acc_wi[Keys][UnionI][UnionJ];
static int acc_wj[Keys][UnionI][UnionJ];

// Массив градиентов.
static int img_r[Width][Height];
static int img_g[Width][Height];
static int img_b[Width][Height];
static int img_a[Width][Height];

/// Инициализационные процедуры.
inline void init()
{
    // Считаем статистику распределения весов.
    std::ifstream in("../_pattern/wgh.bin", std::ios_base::binary);
    in.read((char*)&wgh, sizeof wgh);
    in.close();

    // Создадим массивы координат ключей.
    int k = 0;
    for (int x = 0; x < AccX; x++) {
    for (int y = 0; y < AccY; y++) {
        for (int r = 0;  r < RGB;  r++) {
        for (int g = 0;  g < RGB;  g++) {
        for (int b = 0;  b < RGB;  b++) {
        for (int a = 0;  a < RGB;  a++) {
            key_w[k][r][g][b][a] = wgh[x][y][r][g][b][a];
        }}}}
        key_x[k] = x;
        key_y[k] = y;
        k++;
    }}
}


// Распознавание изображения.
inline int recognize(Trans *solution,
              int   r[Width][Height],
              int   g[Width][Height],
              int   b[Width][Height])
{
    // Создадим массив градиентов.
    for (int i = 1;  i < (Width  - 1);  ++i) {
    for (int j = 1;  j < (Height - 1);  ++j) {
        img_r[i][j] = r[i][j] / RGBdiv;
        img_g[i][j] = g[i][j] / RGBdiv;
        img_b[i][j] = b[i][j] / RGBdiv;
        int ai  = gray(r[i-1][j  ], g[i-1][j  ], b[i-1][j  ])
                - gray(r[i+1][j  ], g[i+1][j  ], b[i+1][j  ]);
        int aj  = gray(r[i  ][j-1], g[i  ][j-1], b[i  ][j-1])
                - gray(r[i  ][j+1], g[i  ][j+1], b[i  ][j+1]);
        int aij = gray(r[i-1][j-1], g[i-1][j-1], b[i-1][j-1])
                - gray(r[i+1][j+1], g[i+1][j+1], b[i+1][j+1]);
        int aji = gray(r[i-1][j+1], g[i-1][j+1], b[i-1][j+1])
                - gray(r[i+1][j-1], g[i+1][j-1], b[i+1][j-1]);
        img_a[i][j] = grad(ai, aj, aij, aji) / RGBdiv;
    }}

    // Найдем наилучшие совпадения по ключевым точкам.
    memset(pat_w,  0, sizeof pat_w );
    memset(pat_i,  0, sizeof pat_i );
    memset(pat_j,  0, sizeof pat_j );
    memset(acc_w,  0, sizeof acc_w );
    memset(acc_wi, 0, sizeof acc_wi);
    memset(acc_wj, 0, sizeof acc_wj);
    for (int k = 0;  k < Keys;  ++k) {
        for (int i = 1;  i < (Width  - 1);  ++i) {
        for (int j = 1;  j < (Height - 1);  ++j) {
            int w = key_w[k][img_r[i][j]]
                            [img_g[i][j]]
                            [img_b[i][j]]
                            [img_a[i][j]];
            int u = i / Union;
            int v = j / Union;
            acc_w [k][u][v] += w;
            acc_wi[k][u][v] += w * i;
            acc_wj[k][u][v] += w * j;
            if (pat_w[k] < acc_w[k][u][v]) {
                pat_w[k] = acc_w[k][u][v];
                pat_i[k] = u;
                pat_j[k] = v;
            }
        }}
        if (pat_w[k] > 0) {
            int u = pat_i[k];
            int v = pat_j[k];
            pat_i[k] = acc_wi[k][u][v] / pat_w[k];
            pat_j[k] = acc_wj[k][u][v] / pat_w[k];
        }
    }

    // Переберем все пары ключевых точек и определим параметры преобразования.
    int best_w = 0;
    for (int k1 = 0;         k1 < (Keys - 1);  ++k1) {
    for (int k2 = (k1 + 1);  k2 < Keys;        ++k2) {
        Trans trans(pat_i[k1], pat_j[k1],   // Инициализируем преобразование
                    pat_i[k2], pat_j[k2],   // координатами двух точек шаблона на изображении
                    key_x[k1], key_y[k1],   // и соответствующим им координатам
                    key_x[k2], key_y[k2]);  // двух точек в шаблона.
        if (!trans.valid()) {               // Если невозможно рассчитать преобразование,
            continue;                       // то следующий тик.
        }
        double di = trans.i(0, 0) - trans.i(AccX-1, 0);
        double dj = trans.i(0, 0) - trans.i(AccX-1, 0);
        double dr = sqrt(di * di + dj *dj);
        if ((dr < (WidthMin * Width))
         || (dr > (WidthMax * Width))) {
            continue;
        }
        int w = 0;
        for (int k = 0;  k < Keys;  ++k) {
            int i = trans.i(key_x[k], key_y[k]);
            int j = trans.j(key_x[k], key_y[k]);
            if (i >= 0 && i < Width &&
                j >= 0 && j < Height) {
                w += key_w[k][img_r[i][j]]
                             [img_g[i][j]]
                             [img_b[i][j]]
                             [img_a[i][j]];
            }
        }
        if (w > best_w) {
            best_w    = w;
            *solution = trans;
        }
    }}

    // Вернем вес распознавания.
    return best_w;
}
