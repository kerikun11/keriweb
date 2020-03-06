---
date: "2018-04-29T15:35:38+09:00"
title: "滑らかな加速の設計④ C++による実装例"
categories:
  - "説明記事"
tags:
  - "マイクロマウス"
thumbnail: "icon.png"
---

## 概要

[前回の記事](/posts/2018-04-29-accel-designer3/)の続きです．

今回の記事では，今までに示した設計法のC++による実装例を紹介します．

<!--more-->

### 滑らかな加速の設計 目次

- [滑らかな加速の設計① 台形加速と曲線加速](/posts/2018-04-29-accel-designer1/)
- [滑らかな加速の設計② 曲線加速の設計](/posts/2018-04-29-accel-designer2/)
- [滑らかな加速の設計③ 走行距離を考慮した速度設計](/posts/2018-04-29-accel-designer3/)
- [滑らかな加速の設計④ C++による実装例](/posts/2018-04-29-accel-designer4/)


## ソースコード

このライブラリに実体はなく，クラスの宣言が書かれたヘッダファイルのみです．

- ライブラリ [AccelDesigner.h](AccelDesigner.h)
- 使用例 [main.cpp](main.cpp)
- CSVをプロットするMATLABコード [plotout.m](plotout.m)

※使用する際は，これらのファイルはすべて同じディレクトリにおいてください．

## 設計1 曲線加速

以下のコードは，曲線加速クラスの宣言部分です．

~~~cpp
/**
 * @class 加速曲線を生成するクラス
 * @brief 引数に従って加速曲線を生成する
 */
class AccelCurve {
public:
  /**
   * @brief 初期化付きのコンストラクタ．
   * @param a_max   最大加速度 [mm/s/s]
   * @param v_start 始点速度   [mm/s]
   * @param v_end   終点速度   [mm/s]
   */
  AccelCurve(const float a_max, const float v_start, const float v_end);
  /**
   * @brief 空のコンストラクタ．あとで reset() により初期化すること．
   */
  AccelCurve();
  /**
   * @brief 引数の拘束条件から曲線を生成する．
   * この関数によって，すべての変数が初期化される．(漏れはない)
   * @param a_max   最大加速度 [mm/s/s]
   * @param v_start 始点速度 [mm/s]
   * @param v_end   終点速度 [mm/s]
   */
  void reset(const float a_max, const float v_start, const float v_end);
  /**
   * @brief 時刻$t$における躍度$j$
   * @param t 時刻[s]
   * @return 躍度[mm/s/s/s]
   */
  float j(const float t) const;
  /**
   * @brief 時刻$t$における加速度$a$
   * @param t 時刻[s]
   * @return 加速度[mm/s/s]
   */
  float a(const float t) const;
  /**
   * @brief 時刻$t$における速度$v$
   * @param t 時刻[s]
   * @return 速度[mm/s]
   */
  float v(const float t) const;
  /**
   * @brief 時刻$t$における位置$x$
   * @param t 時刻[s]
   * @return 位置[mm]
   */
  float x(const float t) const;
  /**
   * @brief 終端定数
   */
  float t_end() const;
  float v_end() const;
  float x_end() const;
  /**
   * @brief 曲線加速部分の時間を決定する関数
   * @param am 最大加速度の大きさ
   */
  static float calcTimeCurve(const float am);
  /**
   * @brief 走行距離から達しうる終点速度を算出する関数
   * @param am 最大加速度の大きさ [mm/s/s]
   * @param vs 始点速度 [mm/s]
   * @param vt 目標速度 [mm/s]
   * @param d 走行距離 [mm]
   * @return ve 終点速度 [mm/s]
   */
  static float calcVelocityEnd(float am, const float vs, const float vt,
                               const float d);
  /** @function calcVelocityMax
   *   @brief 走行距離から最大速度を算出する関数
   *   @param am 最大加速度の大きさ [mm/s/s]
   *   @param vs 始点速度 [mm/s]
   *   @param va 飽和速度 [mm/s]
   *   @param ve 終点速度 [mm/s]
   *   @param d 走行距離 [mm]
   *   @return vm 最大速度 [mm/s]
   */
  static float calcVelocityMax(const float am, const float vs, const float ve,
                               const float d);
  /**
   * @brief 速度差から変位を算出する関数
   *
   * @param am 最大加速度の大きさ [mm/s/s]
   * @param vs 始点速度 [mm/s]
   * @param vt 目標速度 [mm/s]
   * @return float d 変位 [mm/s]
   */
  static float calcMinDistance(float am, const float vs, const float vt);

private:
  float am;             //< 最大加速度 [m/s/s]
  float t0, t1, t2, t3; //< 境界点の時刻 [s]
  float v0, v1, v2, v3; //< 境界点の速度 [m/s]
  float x0, x1, x2, x3; //< 境界点の位置 [m]
  float tc;             //< 曲線加速の時間 [s]
  float tm;             //< 最大加速度の時間 [s]
};
~~~

## 設計2 曲線加減速

以下のコードは，走行距離の拘束条件を満たした曲線加減速クラスの宣言部分です．

~~~cpp
/**
 * @class 加減速曲線を生成するクラス
 * @brief 引数に従って速度計画をし，加減速曲線を生成する
 */
class AccelDesigner {
public:
  /**
   * @param a_max 最大加速度 [mm/s/s]
   * @param v_start 始点速度 [mm/s]
   * @param v_end 終点速度 [mm/s]
   */
  AccelDesigner(const float a_max, const float v_start, const float v_sat,
                const float v_target, const float distance,
                const float x_start = 0, const float t_start = 0);
  /**
   * @brief 空のコンストラクタ．あとで reset() により初期化すること．
   */
  AccelDesigner();
  /**
   * @brief 引数の拘束条件から曲線を生成する．
   * この関数によって，すべての変数が初期化される．(漏れはない)
   */
  void reset(const float a_max, const float v_start, const float v_sat,
             const float v_target, float distance, const float x_start = 0,
             const float t_start = 0);
  /**
   * @brief 時刻$t$における躍度$j$
   * @param t 時刻[s]
   * @return 躍度[mm/s/s/s]
   */
  float j(const float t) const;
  /**
   * @brief 時刻$t$における加速度$a$
   * @param t 時刻[s]
   * @return 加速度[mm/s/s]
   */
  float a(const float t) const;
  /**
   * @brief 時刻$t$における速度$v$
   * @param t 時刻[s]
   * @return 速度[mm/s]
   */
  float v(const float t) const;
  /**
   * @brief 時刻$t$における位置$x$
   * @param t 時刻[s]
   * @return 位置[mm]
   */
  float x(const float t) const;
  /**
   * @brief 終端xx
   */
  float t_end() const;
  float v_end() const;
  float x_end() const;
  /**
   * @brief stdoutに軌道のcsvを出力する関数．
   */
  void printCsv(const float t_interval = 0.001f) const;
  /**
   * @brief std::ofstream に軌道のcsvを出力する関数．
   */
  void printCsv(std::ofstream &of, const float t_interval = 0.001f) const;

private:
  float t0, t1, t2, t3; /**< 境界点の時刻 [s] */
  float x0, x3;         /**< 境界点の位置 [mm] */
  AccelCurve ac, dc;    /**< 曲線加速，曲線減速オブジェクト */
};
~~~

## 使用例

### シンプル

一番簡単な例として以下のC++コードをご覧ください．
実行すると，標準出力にCSVが表示されます．

~~~cpp
#include "AccelDesigner.h"
#include <cstdio>
#include <fstream>
#include <iostream>

int main(void) {
  AccelDesigner sd;

  // パラメータを設定
  const float a_max = 3000;
  const float v_start = 0;
  const float v_sat = 2400;
  const float v_target = 600;
  const float distance = 720;
  // 曲線を生成
  sd.reset(a_max, v_start, v_sat, v_target, distance);
  // CSV出力
  for (float t = 0; t < sd.t_end(); t += 0.001f) {
    printf("%f,%f,%f,%f,%f\n", t, sd.j(t), sd.a(t), sd.v(t), sd.x(t));
  }

  return 0;
}
~~~

### 連続使用

加減速をいくつか繰り返す例です．

前回の終点の速度や位置を，次回の始点速度，位置に使っています．

~~~cpp
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
~~~
### CSVをMATLABでプロット

以下のMATLABコードで，出力された[out.csv](out.csv)ファイルをプロットできます．

~~~m
%% cleaning
clear;
set(groot, 'DefaultTextInterpreter', 'Latex');
set(groot, 'DefaultLegendInterpreter', 'Latex');
set(groot, 'DefaultLineLineWidth', 2);

%% load
rawdata = csvread('out.csv');
t = rawdata(:, 1);
j = rawdata(:, 2);
a = rawdata(:, 3);
v = rawdata(:, 4);
x = rawdata(:, 5);

%% plot
ylabels= {'$j$ [mm/s/s/s]', '$a$ [mm/s/s]', '$v$ [mm/s]', '$x$ [mm]'};
titles= {'Jerk', 'Acceleration', 'Velocity', 'Position'};
xlabelstr = '$t$ [s]';

figure(1);
data = [j a v x];
for i = 1 : 4
    subplot(4, 1, i);
    hold off; plot(nan, nan); % clean
    hold on;
    ax = gca; ax.ColorOrderIndex = i;
    plot(t, data(:, i));
    grid on;
    xlabel(xlabelstr);
    ylabel(ylabels(i));
    title(titles(i));
end
~~~

上記の連続使用のコード例をプロットしたのが以下の図です．

{{< postfig src="out.png" title="CSVをMATLABでプロット" width="480px" >}}

## 実装上の工夫

### 使いやすさ

今回作ったライブラリでは，コンストラクタや `reset()` 関数でパラメータを設定してあげれは
あとは $v(t)$ のように時間の関数として使えるようにしました．

複雑な場合分けは内部で行われていて，使い方はとてもシンプルになっています．

### 移植性の考慮

今回のクラス設計では，

- 走行距離$d$を考慮して終点速度$v_e$を求める関数`AccelCurve::calcVelocityEnd()`
- 走行距離$d$を考慮して最大速度$v_m$を求める関数`AccelCurve::calcVelocityMax()`

は，設計1の`AccelCurve`クラスに実装しました．

というのも，曲線生成部分がすべて`AccelCurve`クラスにまとまっているので，
今後，曲線部分を2次関数ではない別の関数に変えたくなったら，`AccelCurve`クラスの中身を変更するだけで済むようになっています．

## まとめ

さて，滑らかな加速について考えてきました．

今回の設計によって，拘束条件を満たす走行軌道を生成することができました．

あとは，機体がこれを追従するコードを書くだけですね！

といっても，それもまた難しいですが...

### 免責

設計には欠陥やミスがあるかもしれません．くれぐれも自己責任ご使用ください．

もしミスや改良点を見つけたら教えてください！

### 追記: 2019年1月29日

ソースコードに飽和速度を超過するバグがあったので修正しました．

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
