#pragma once
#include "mat.hpp"
#include <stdint.h>

/// Класс фильтрации изображения для "улучшения" видимости себя любимого:)
class Filters
{
    class MyPixel {
    private:
        uint8_t* _alfa;
        uint8_t* _red;
        uint8_t* _green;
        uint8_t* _blue;

    public:
        MyPixel(uint8_t* image_pixels)
        {
            _blue  = image_pixels;
            _green = image_pixels + 1;
            _red   = image_pixels + 2;
            _alfa  = image_pixels + 3;

        }

        /// Получить значения
        uint8_t a() {return *_alfa;}
        uint8_t r() {return *_red;}
        uint8_t g() {return *_green;}
        uint8_t b() {return *_blue;}

        /// Установить значения
        void a(uint8_t val) {*_alfa  = val;}
        void r(uint8_t val) {*_red   = val;}
        void g(uint8_t val) {*_green = val;}
        void b(uint8_t val) {*_blue  = val;}

        /// Установить значения
        void rgb(uint8_t r, uint8_t g, uint8_t b) {*_red = r, *_green = g, *_blue = b, *_alfa = 255;}
    };

    class MyImage {
    private:
        uint8_t* image_pixels;  ///< Указатель на 1 пиксель изображения
        uint16_t _width;        ///< Ширина изображения в пикселях
        uint16_t _height;       ///< Ширина изображения в пикселях

    public:
        /**
         * @brief Конструктор
         * @param image_pixels  Указатель на 1 пиксель
         * @param widht         Ширина изображения в пикселях
         */
        MyImage(uint8_t* image_pixels, uint16_t width, uint16_t height)
            : image_pixels(image_pixels)
            , _width(width)
            , _height(height)
        {}

        /// Вернем размеры изображения.
        int width()  {return _width;}
        int height() {return _height;}

        /**
         * @brief Получить пиксель
         * @param row   Номер строки
         * @param col   Номер столбца
         * @return
         */
        MyPixel pixel(uint16_t col, uint16_t row)
        {
            uint64_t index = (row * _width + col) * 4;
            return MyPixel(image_pixels + index);
        }
        MyPixel px(uint16_t col, uint16_t row, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        {
            uint64_t index = (row * _width + col) * 4;
            MyPixel  pixel = (image_pixels + index);
            pixel.r(r);
            pixel.g(g);
            pixel.b(b);
            pixel.a(a);
            return pixel;
        }

    };

   // Ссылки на обрабатываемое изображение.
//   uint8_t r[SrcI][SrcJ];
//   uint8_t g[SrcI][SrcJ];
//   uint8_t b[SrcI][SrcJ];
    uint16_t width;
    uint16_t height;

   // Последнее обновленное преобразование.
   int x1;
   int y1;
   int x2;
   int y2;

   // // Маски фильтров.
   // uint8_t lips   [SrcI][SrcJ];
   // uint8_t shadow [SrcI][SrcJ];
   // uint8_t blur   [SrcI][SrcJ];
   // uint8_t shape_w[SrcI][SrcJ];
   // uint8_t shape_h[SrcI][SrcJ];

   // /// Служебная функция насыщения.
   // int sat255(int val)
   // {
   //    if (val <   0) return   0;
   //    if (val > 255) return 255;
   //    return val;
   // }

   // Фильтры изображений:

   /// Тестовый фильтр рисования красного квадрата.
   void test(MyImage& img)
   {
       printf("width=%d\n", img.width());

      for (int i = x1;  i < img.width()  / 2;  ++i) {
      for (int j = y1;  j < img.height() / 2;  ++j) {
//          MyPixel pixel = img.pixel(i, j);
//          pixel.r(0);
//          pixel.g(255);
//          pixel.b(0);
//          pixel.a(12);

//          img.px(i, j, 0, 127, 255, 64);

          MyPixel px = img.pixel(i, j);
          px.rgb(px.r() + 50, px.g() + 50, px.b() + 50);
      }}
   }

public:

   /// Инициализируем фильтры один раз после запуска программы.
//   Filters(uint8_t red[SrcI][SrcJ], uint8_t green[SrcI][SrcJ], uint8_t blue[SrcI][SrcJ])
   /**
    * @brief Конструктор
    * @param image_size     Размер изображения в байтах
    * @param pixel_size     Размер пикселя (1,2,3,4 байта)
    * @param widht          Ширина изображения
    * @param height         Высота изображения
    */
   Filters(uint16_t widht, uint16_t height)
       : width(widht)
       , height(height)
   {
//      r = red;
//      g = green;
//      b = blue;
//      x1 = y1 = x2 = y2 = 0;
//    std::ifstream in;
//    in.load("lips.bin",    std::ios_base::binary);  in.read((char*)&lips,    sizeof lips);     in.close();
//    in.load("shadow.bin",  std::ios_base::binary);  in.read((char*)&shadow,  sizeof shadow);   in.close();
//    in.load("blur.bin",    std::ios_base::binary);  in.read((char*)&blur,    sizeof blur);     in.close();
//    in.load("shape_w.bin", std::ios_base::binary);  in.read((char*)&shape_w, sizeof shape_w);  in.close();
//    in.load("shape_h.bin", std::ios_base::binary);  in.read((char*)&shape_h, sizeof shape_h);  in.close();
   }

   /// Обновление области фильтрации после распознавания ключевого кадра.
   void update(int _x1, int _y1, int _x2, int _y2)
   {
      x1 = _x1;
      y1 = _y1;
      x2 = _x2;
      y2 = _y2;
      // tr = transform;
   }

   /**
    * @brief Модификация (фильтрация) лица, которую будем делать на каждом кадре.
    * @note: Будем делать на каждом кадре.
    * @param image_pixels   Указатель на 1 пиксель изображения
    * @param image_size     Размер изображения в байтах
    * @param pixel_size     Размер пикселя (1,2,3,4 байта)
    * @param widht          Ширина изображения
    * @param height         Высота изображения
    */
   void filtration(uint8_t* image_pixels)
   {
       MyImage img(image_pixels, width, height);
      test   (img);
//    filter_lips   ();
//    filter_shadow ();
//    filter_blur   ();
//    filter_shape_w();
//    filter_shape_h();
   }
};
