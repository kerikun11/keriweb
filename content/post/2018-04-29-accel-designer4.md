---
date: "2018-04-29T15:35:38+09:00"
title: "滑らかな加速の設計④ C++による実装例"
categories:
  - "説明記事"
tags:
  - "マイクロマウス"
  - "制御"
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

サンプルファイルは，[ここ](src.zip)からダウンロードできます．

このライブラリに実体はなく，クラスの宣言が書かれたヘッダファイルのみです．

- ライブラリ `accel_curve.h`, `accel_designer.h`
- 使用例 `simple.cpp`, `multi.cpp`
- 出力された CSV をプロットする MATLAB コード `plotout.m`

※使用する際は，これらのファイルはすべて同じディレクトリにおいてください．

## 設計1 曲線加速

まず，移動距離の拘束がない速度設計クラス `AccelCurve` を紹介します．

以下のコードは，
`accel_curve.h`
の抜粋です．

```cpp
/**
 * @brief 加速曲線を生成するクラス
 *
 * 引数に従って加速曲線を生成する
 */
class AccelCurve {
public:
  /**
   * @brief 初期化付きのコンストラクタ．
   *
   * @param j_max   最大躍度の大きさ [mm/s/s/s]
   * @param a_max   最大加速度の大きさ [mm/s/s]
   * @param v_start 始点速度   [mm/s]
   * @param v_end   終点速度   [mm/s]
   */
  AccelCurve(const float j_max, const float a_max, const float v_start, const float v_end);
  /**
   * @brief 時刻 $t$ における躍度 $j$
   * @param t 時刻 [s]
   * @return 躍度 [mm/s/s/s]
   */
  float j(const float t) const;
  /**
   * @brief 時刻 $t$ における加速度 $a$
   * @param t 時刻 [s]
   * @return 加速度 [mm/s/s]
   */
  float a(const float t) const;
  /**
   * @brief 時刻 $t$ における速度 $v$
   * @param t 時刻 [s]
   * @return 速度 [mm/s]
   */
  float v(const float t) const;
  /**
   * @brief 時刻 $t$ における位置 $x$
   * @param t 時刻 [s]
   * @return 位置 [mm]
   */
  float x(const float t) const;
  /**
   * @brief 終端定数
   */
  float t_end() const;
  float v_end() const;
  float x_end() const;
  /**
   * @brief std::ofstream に軌道のcsvを出力する関数．
   */
  void printCsv(std::ostream &os, const float t_interval = 0.001f) const;

public:
  /**
   * @brief 走行距離から達しうる終点速度を算出する関数
   *
   * @param j_max 最大躍度の大きさ [mm/s/s/s]
   * @param a_max 最大加速度の大きさ [mm/s/s]
   * @param vs 始点速度 [mm/s]
   * @param vt 目標速度 [mm/s]
   * @param d 走行距離 [mm]
   * @return ve 終点速度 [mm/s]
   */
  static float calcVelocityEnd(const float j_max, float a_max, const float vs, const float vt, const float d);
  /**
   * @brief 走行距離から最大速度を算出する関数
   *
   * @param j_max 最大躍度の大きさ [mm/s/s/s]
   * @param a_max 最大加速度の大きさ [mm/s/s]
   * @param vs 始点速度 [mm/s]
   * @param ve 終点速度 [mm/s]
   * @param d 走行距離 [mm]
   * @return vm 最大速度 [mm/s]
   */
  static float calcVelocityMax(const float j_max, const float a_max, const float vs, const float ve, const float d);
  /**
   * @brief 速度差から変位を算出する関数
   *
   * @param j_max   最大躍度の大きさ [mm/s/s/s]
   * @param a_max   最大加速度の大きさ [mm/s/s]
   * @param v_start 始点速度 [mm/s]
   * @param v_end   終点速度 [mm/s]
   * @return float d 変位 [mm/s]
   */
  static float calcMinDistance(const float j_max, const float a_max, const float v_start, const float v_end);

protected:
  float jm;             /**< @brief 最大躍度 [m/s/s/s] */
  float am;             /**< @brief 最大加速度 [m/s/s] */
  float t0, t1, t2, t3; /**< @brief 境界点の時刻 [s] */
  float v0, v1, v2, v3; /**< @brief 境界点の速度 [m/s] */
  float x0, x1, x2, x3; /**< @brief 境界点の位置 [m] */
};
```

## 設計2 曲線加減速

次は，移動距離などの拘束条件を満たす加減速設計クラス `AccelDesigner` を紹介します．

以下のコードは，
`accel_designer.h`
の抜粋です．

```cpp
/**
 * @brief 加減速曲線を生成するクラス
 *
 * 引数の拘束条件に従って速度計画をし，加減速曲線を生成する
 */
class AccelDesigner {
public:
  /**
   * @brief 初期化付きコンストラクタ
   *
   * @param j_max     最大躍度の大きさ [mm/s/s/s]，正であること
   * @param a_max     最大加速度の大きさ [mm/s/s], 正であること
   * @param v_start   始点速度 [mm/s]
   * @param v_sat     飽和速度の大きさ [mm/s]，正であること
   * @param v_target  目標速度 [mm/s]
   * @param v_end     終点速度 [mm/s]
   * @param dist      移動距離の大きさ [mm], 非負であること
   * @param x_start   始点位置 [mm]
   * @param t_start   始点時刻 [t]
   */
  AccelDesigner(const float j_max, const float a_max, const float v_start, const float v_sat, const float v_target, const float dist, const float x_start = 0, const float t_start = 0);
  /**
   * @brief 空のコンストラクタ．あとで reset() により初期化すること．
   */
  AccelDesigner();
  /**
   * @brief 引数の拘束条件から曲線を生成する．
   * この関数によって，すべての変数が初期化される．(漏れはない)
   *
   * @param j_max     最大躍度の大きさ [mm/s/s/s]，正であること
   * @param a_max     最大加速度の大きさ [mm/s/s], 正であること
   * @param v_start   始点速度 [mm/s]
   * @param v_sat     飽和速度の大きさ [mm/s]，正であること
   * @param v_target  目標速度 [mm/s]
   * @param v_end     終点速度 [mm/s]
   * @param dist      移動距離の大きさ [mm], 非負であること
   * @param x_start   始点位置 [mm]
   * @param t_start   始点時刻 [t]
   */
  void reset(const float j_max, const float a_max, const float v_start, const float v_sat, const float v_target, const float dist, const float x_start = 0, const float t_start = 0);
  /**
   * @brief 時刻 $t$ における躍度 $j$
   * @param t 時刻[s]
   * @return j 躍度[mm/s/s/s]
   */
  float j(const float t) const;
  /**
   * @brief 時刻 $t$ における加速度 $a$
   * @param t 時刻 [s]
   * @return a 加速度 [mm/s/s]
   */
  float a(const float t) const;
  /**
   * @brief 時刻 $t$ における速度 $v$
   * @param t 時刻 [s]
   * @return v 速度 [mm/s]
   */
  float v(const float t) const;
  /**
   * @brief 時刻 $t$ における位置 $x$
   * @param t 時刻 [s]
   * @return x 位置 [mm]
   */
  float x(const float t) const;
  /**
   * @brief 終端xx
   */
  float t_end() const;
  float v_end() const;
  float x_end() const;
  /**
   * @brief 端点の時刻
   */
  float t_0() const;
  float t_1() const;
  float t_2() const;
  float t_3() const;
  /**
   * @brief stdoutに軌道のcsvを出力する関数．
   */
  void printCsv(const float t_interval = 0.001f) const;
  /**
   * @brief std::ofstream に軌道のcsvを出力する関数．
   */
  void printCsv(std::ostream &os, const float t_interval = 0.001f) const;
  /**
   * @brief 情報の表示
   */
  friend std::ostream &operator<<(std::ostream &os, const AccelDesigner &obj);

private:
  float t0, t1, t2, t3; /**< @brief 境界点の時刻 [s] */
  float x0, x3;         /**< @brief 境界点の位置 [mm] */
  AccelCurve ac, dc; /**< @brief 曲線加速，曲線減速オブジェクト */
};
```

## 使用例

次に， `AccelDesigner` の使用例を紹介します．

### シンプル

一番簡単な例として以下の C++コードをご覧ください．
実行すると，標準出力に CSV が表示されます．

`simple.cpp`

```cpp
#include <cstdio>
#include <fstream>
#include <iostream>

#include "accel_designer.h"

int main(void) {
  ctrl::AccelDesigner ad;

  // パラメータを設定
  const float j_max = 120000;
  const float a_max = 9000;
  const float v_start = 0;
  const float v_sat = 1800;
  const float v_target = 600;
  const float distance = 720;
  // 曲線を生成
  ad.reset(j_max, a_max, v_start, v_sat, v_target, distance);
  // CSV出力
  for (float t = 0; t < ad.t_end(); t += 0.001f) {
    printf("%f,%f,%f,%f,%f\n", t, ad.j(t), ad.a(t), ad.v(t), ad.x(t));
  }

  return 0;
}
```

### 連続使用

加減速をいくつか繰り返す例です．

前回の終点の速度や位置を，次回の始点速度，位置に使っています．

`multi.cpp`

```cpp
#include <fstream>
#include <iostream>

#include "accel_designer.h"

int main(void) {
  std::ofstream of("out.csv"); //< 出力ファイル名
  ctrl::AccelDesigner ad;

  // パラメータを設定
  const float j_max = 120000;
  const float a_max = 9000;
  // 曲線を生成 & CSVを作成 の連続
  ad.reset(j_max, a_max, ad.v_end(), 2400, 1200, 1080, ad.x_end(), ad.t_end());
  ad.printCsv(of);
  ad.reset(j_max, a_max, ad.v_end(), 2400, 600, 360, ad.x_end(), ad.t_end());
  ad.printCsv(of);
  ad.reset(j_max, a_max, ad.v_end(), 2400, 0, 720, ad.x_end(), ad.t_end());
  ad.printCsv(of);

  return 0;
}
```

### CSV を MATLAB でプロット

以下の MATLAB コードで，生成された `out.csv` をプロットできます．

```m
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
titles= {'Jerk', 'Acceleration', 'Velocity', 'Position'};
ylabels= {'$j$ [mm/s/s/s]', '$a$ [mm/s/s]', '$v$ [mm/s]', '$x$ [mm]'};
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
```

上記の連続使用のコード例をプロットしたのが以下の図です．

{{< postfig src="main.png" title="CSVをMATLABでプロット" width="480px" >}}

滑らかな曲線加速になっていることがわかります．

## 実装上の工夫

### 使いやすさ

今回作ったライブラリでは，コンストラクタや `reset()` 関数でパラメータを設定してあげれは
あとは $v(t)$ のように時間の関数として使えるようにしました．

複雑な場合分けは内部で行われていて，使い方はとてもシンプルになっています．

### 移植性の考慮

今回のクラス設計では，

- 走行距離 $d$ を考慮して終点速度 $v_e$ を求める関数 `AccelCurve::calcVelocityEnd()`
- 走行距離 $d$ を考慮して最大速度 $v_m$ を求める関数 `AccelCurve::calcVelocityMax()`

は，設計1の `AccelCurve` クラスに実装しました．

というのも，曲線生成部分がすべて `AccelCurve` クラスにまとまっているので，
今後，曲線部分を2次関数ではない別の関数に変えたくなったら， `AccelCurve` クラスの中身を変更するだけで済むようになっています．

## まとめ

さて，滑らかな加速について考えてきました．

今回の設計によって，拘束条件を満たす走行軌道を生成することができました．

あとは，機体がこれを追従するコードを書くだけですね！

といっても，それもまた難しいですが...

### 免責

設計には欠陥やミスがあるかもしれません．くれぐれも自己責任ご使用ください．

もしミスや改良点を見つけたら是非教えてください！[@kerikun11](https://twitter.com/kerikun11)

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
