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
        uint8_t alfa()  {return *_alfa;}
        uint8_t red()   {return *_red;}
        uint8_t green() {return *_green;}
        uint8_t blue()  {return *_blue;}

        /// Установить значения
        void alfa (uint8_t val) {*_alfa  = val;}
        void red  (uint8_t val) {*_red   = val;}
        void green(uint8_t val) {*_green = val;}
        void blue (uint8_t val) {*_blue  = val;}
    };

    class MyImage {
    private:
        uint8_t* image_pixels;  ///< Указатель на 1 пиксель изображения
        uint16_t widht;         ///< Ширина изображения в пикселях

    public:
        /**
         * @brief Конструктор
         * @param image_pixels  Указатель на 1 пиксель
         * @param widht         Ширина изображения в пикселях
         */
        MyImage(uint8_t* image_pixels, uint16_t widht)
            : image_pixels(image_pixels)
            , widht(widht)
        {}

        /**
         * @brief Получить пиксель
         * @param row   Номер строки
         * @param col   Номер столбца
         * @return
         */
        MyPixel pixel(uint16_t row, uint16_t col)
        {
            uint64_t index = (row * widht + col) * 4;
            return MyPixel(image_pixels + index);
        }

    };

   // Ссылки на обрабатываемое изображение.
//   uint8_t r[SrcI][SrcJ];
//   uint8_t g[SrcI][SrcJ];
//   uint8_t b[SrcI][SrcJ];
    uint16_t widht;
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
      for (int c = x1;  c <= x2;  ++c) {
      for (int r = y1;  r <= y2;  ++r) {
          MyPixel pixel = img.pixel(r, c);
          pixel.red(255);
          pixel.green(0);
          pixel.blue(0);
          pixel.alfa(255);
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
       : widht(widht)
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
       MyImage img (image_pixels, widht);
      test   (img);
//    filter_lips   ();
//    filter_shadow ();
//    filter_blur   ();
//    filter_shape_w();
//    filter_shape_h();
   }
};
