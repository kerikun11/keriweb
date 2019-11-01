#include <cstdio>
#include <fstream>
#include <iostream>

#include "AccelDesigner.h"

int main(void) {
  std::ofstream of("out.csv"); //< ファイル名
  AccelDesigner sd;

  sd.reset(12000, sd.v_end(), 2400, 1200, 1080, sd.x_end(), sd.t_end()); //< 曲線の生成
  sd.printCsv(of); //< CSVファイル出力
  sd.reset(12000, sd.v_end(), 2400, 600, 360, sd.x_end(), sd.t_end()); //< 曲線の生成
  sd.printCsv(of); //< CSVファイル出力
  sd.reset(12000, sd.v_end(), 2400, 0, 720, sd.x_end(), sd.t_end()); //< 曲線の生成
  sd.printCsv(of); //< CSVファイル出力

  return 0;
}