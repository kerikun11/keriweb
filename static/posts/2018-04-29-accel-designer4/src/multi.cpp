#include <fstream>
#include <iostream>

#include "accel_designer.h"

int main(void) {
  std::ofstream of("out.csv"); //< 出力ファイル名
  ctrl::AccelDesigner ad;

  const float j_max = 120000;
  const float a_max = 9000;
  ad.reset(j_max, a_max, ad.v_end(), 2400, 1200, 1080, ad.x_end(),
           ad.t_end()); //< 曲線の生成
  ad.printCsv(of);      //< CSVファイル出力
  ad.reset(j_max, a_max, ad.v_end(), 2400, 600, 360, ad.x_end(),
           ad.t_end()); //< 曲線の生成
  ad.printCsv(of);      //< CSVファイル出力
  ad.reset(j_max, a_max, ad.v_end(), 2400, 0, 720, ad.x_end(),
           ad.t_end()); //< 曲線の生成
  ad.printCsv(of);      //< CSVファイル出力

  return 0;
}
