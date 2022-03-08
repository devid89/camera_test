#include "mat.hpp"


/// Класс фильтрации изображения для "улучшения" видимости себя любимого:)
class Filters
{
   // Ссылки на обрабатываемое изображение.
   uint8_t r[SrcI][SrcJ];
   uint8_t g[SrcI][SrcJ];
   uint8_t b[SrcI][SrcJ];

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
   void test(uint8_t red[SrcI][SrcJ], uint8_t green[SrcI][SrcJ], uint8_t blue[SrcI][SrcJ])
   {
      for (int i = x1;  i <= x2;  ++i) {
      for (int j = y1;  j <= y2;  ++j) {
         red[i][j] = 255;
      }}
   }

public:

   /// Инициализируем фильтры один раз после запуска программы.
   Filters(uint8_t red[SrcI][SrcJ], uint8_t green[SrcI][SrcJ], uint8_t blue[SrcI][SrcJ])
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
   void update(int i1, int j1, int i2, int j2)
   {
      x1 = i1;
      y1 = j1;
      x2 = i2;
      y2 = j2;
      // tr = transform;
   }

   /// Модификация (фильтрация) лица, которую будем делать на каждом кадре.
   /// @note: Будем делать на каждом кадре.
   void filtration(uint8_t red[SrcI][SrcJ], uint8_t green[SrcI][SrcJ], uint8_t blue[SrcI][SrcJ])
   {
      test   (red, green, blue);
//    filter_lips   ();
//    filter_shadow ();
//    filter_blur   ();
//    filter_shape_w();
//    filter_shape_h();
   }
};
