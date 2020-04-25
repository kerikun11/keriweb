---
title: "滑らかなスラロームの設計③ C++による実装"
date: "2020-04-25T13:00:00+09:00"
categories:
  - "説明記事"
tags:
  - "マイクロマウス"
  - "制御"
thumbnail: "icon.png"
draft: true
---

前回の記事の続きです．

<!--more-->

今回の記事では，滑らかなスラローム軌道の C++による実装例を紹介します．

## 実装

以下では，各クラスの抜粋を提示します．

### AccelDesigner

[以前の記事](/posts/2018-04-29-accel-designer1)で紹介した，拘束条件を満たす曲線加減速の軌道を生成するクラスです．

今回はこのクラスに対して，単位 [m] を [rad] とみなして使用します．

以下， [accel_designer.h](include/accel_designer.h) の抜粋:

```cpp
/**
 * @brief 拘束条件を満たす曲線加減速の軌道を生成するクラス
 *
 * - 移動距離の拘束条件を満たす曲線加速軌道を生成する
 * - 各時刻 $t$ における躍度 $j(t)$，加速度 $a(t)$，速度 $v(t)$，位置 $x(t)$
 * を提供する
 * - 最大加速度 $a_{\max}$ と始点速度 $v_s$
 * など拘束次第では目標速度 $v_t$ に達することができない場合があるので注意する
 */
class AccelDesigner {
public:
  /**
   * @brief 初期化付きコンストラクタ
   *
   * @param j_max     最大躍度の大きさ [m/s/s/s]，正であること
   * @param a_max     最大加速度の大きさ [m/s/s], 正であること
   * @param v_max     最大速度の大きさ [m/s]，正であること
   * @param v_start   始点速度 [m/s]
   * @param v_target  目標速度 [m/s]
   * @param dist      移動距離 [m]
   * @param x_start   始点位置 [m] (オプション)
   * @param t_start   始点時刻 [s] (オプション)
   */
  AccelDesigner(const float j_max, const float a_max, const float v_max,
                const float v_start, const float v_target, const float dist,
                const float x_start = 0, const float t_start = 0);
  /**
   * @brief 時刻 t [s] における躍度 j [m/s/s/s]
   * @param t 時刻 [s]
   */
  float j(const float t) const;
  /**
   * @brief 時刻 t [s] における加速度 a [m/s/s]
   * @param t 時刻 [s]
   */
  float a(const float t) const;
  /**
   * @brief 時刻 t [s] における速度 v [m/s]
   * @param t 時刻 [s]
   */
  float v(const float t) const;
  /**
   * @brief 時刻 t [s] における位置 x [m]
   * @param t 時刻 [s]
   */
  float x(const float t) const;
  /**
   * @brief 終点時刻 [s]
   */
  float t_end() const;
  /**
   * @brief 終点速度 [m/s]
   */
  float v_end() const;
  /**
   * @brief 終点位置 [m]
   */
  float x_end() const;
  /**
   * @brief std::ostream に軌道のcsvを出力する関数．
   */
  void printCsv(std::ostream &os, const float t_interval = 0.001f) const;
  /**
   * @brief 情報の表示
   */
  friend std::ostream &operator<<(std::ostream &os, const AccelDesigner &obj);

protected:
  float t0, t1, t2, t3; /**< @brief 境界点の時刻 [s] */
  float x0, x3;         /**< @brief 境界点の位置 [m] */
  AccelCurve ac, dc; /**< @brief 曲線加速，曲線減速オブジェクト */
};
```

### Pose

平面上の位置および姿勢を表現する座標．

以下， [pose.h](include/pose.h) の抜粋:

```cpp
/**
 * @brief 位置姿勢の座標
 */
struct Pose {
  float x, y, th; /*< (x, y, theta) 成分 */
};
```

### State

軌道生成などに使用する状態変数

以下， [state.h](include/state.h) の抜粋:

```cpp
/**
 * @brief 軌道制御の状態変数
 */
struct State {
  Pose q;
  Pose dq;
  Pose ddq;
  Pose dddq;
};
```

### Polar

並進と回転の座標を管理する構造体

以下， [polar.h](include/polar.h) の抜粋:

```cpp
/**
 * @brief 並進と回転の座標
 */
struct Polar {
  float tra; //< translation [m]
  float rot; //< rotation [rad]
};
```

### slalom::Shape

以下， [slalom.h](include/slalom.h) の抜粋:

```cpp
/**
 * @brief slalom::Shape スラロームの形状を表す構造体
 *
 * メンバー変数は互いに依存して決定されるので，個別に数値を変更することは許されない，
 * スラローム軌道を得るには slalom::Trajectory を用いる．
 */
struct Shape {
  Pose total; /**< @brief 前後の直線を含めた移動位置姿勢 */
  Pose curve; /**< @brief カーブ部分の移動位置姿勢 */
  float straight_prev; /**< @brief カーブ前の直線の距離 [m] */
  float straight_post; /**< @brief カーブ後の直線の距離 [m] */
  float v_ref;         /**< @brief カーブ部分の基準速度 [m/s] */
  float dddth_max;     /**< @brief 最大角躍度の大きさ [rad/s/s/s] */
  float ddth_max;      /**< @brief 最大角加速度の大きさ [rad/s/s] */
  float dth_max;       /**< @brief 最大角速度の大きさ [rad/s] */

public:
  /**
   * @brief 生成済みスラローム形状を代入するコンストラクタ
   */
  Shape(const Pose total, const Pose curve, float straight_prev,
        const float straight_post, const float v_ref, const float dddth_max,
        const float ddth_max, const float dth_max);
  /**
   * @brief 拘束条件からスラローム形状を生成するコンストラクタ
   *
   * @param total 前後の直線を含めた移動位置姿勢
   * @param y_curve_end
   * $y$方向(進行方向に垂直な方向)の移動距離，
   * カーブの大きさを決めるもので，設計パラメータとなる
   * @param x_adv $x$方向(進行方向)の前後の直線の長さ．180度ターンなどでは
   * y_curve_end で調節できないので，例外的にこの値で調節する．
   * @param dddth_max 最大角躍度の大きさ [rad/s/s/s]
   * @param ddth_max 最大角加速度の大きさ [rad/s/s]
   * @param dth_max 最大角速度の大きさ [rad/s]
   */
  Shape(const Pose total, const float y_curve_end, const float x_adv = 0,
        const float dddth_max = 1200 * M_PI, const float ddth_max = 36 * M_PI,
        const float dth_max = 3 * M_PI);
  /**
   * @brief 軌道の積分を行う関数．ルンゲクッタ法を使用して数値積分を行う．
   *
   * @param ad 角速度分布
   * @param s 状態変数
   * @param v 並進速度
   * @param t 時刻
   * @param Ts 積分時間
   */
  static void integrate(const AccelDesigner &ad, State &s, const float v,
                        const float t, const float Ts);
  /**
   * @brief 情報の表示
   */
  friend std::ostream &operator<<(std::ostream &os, const Shape &obj);
};
```

### slalom::Trajectory

以下， [slalom.h](include/slalom.h) の抜粋:

```cpp
/**
 * @brief slalom::Trajectory スラローム軌道を生成するクラス
 *
 * スラローム形状 Shape と並進速度をもとに，各時刻における位置や速度を提供する．
 */
class Trajectory {
public:
  /**
   * @brief コンストラクタ
   *
   * @param shape スラローム形状
   * @param mirror_x スラローム形状を$x$軸反転(進行方向に対して左右反転)する
   */
  Trajectory(const Shape &shape, const bool mirror_x = false);
  /**
   * @brief 並進速度を設定して軌道を初期化する関数
   *
   * @param velocity 並進速度 [m/s]
   * @param th_start 初期姿勢 [rad] (オプション)
   * @param t_start 初期時刻 [s] (オプション)
   */
  void reset(const float velocity, const float th_start = 0,
             const float t_start = 0);
  /**
   * @brief 軌道の更新
   *
   * @param state 次の時刻に更新する現在状態
   * @param t 現在時刻 [s]
   * @param Ts 積分時間 [s]
   */
  void update(State &state, const float t, const float Ts) const;
  /**
   * @brief 並進速度を取得
   */
  float getVelocity() const;
  /**
   * @brief ターンの合計時間を取得
   */
  float getTimeCurve() const;
  /**
   * @brief スラローム形状を取得
   */
  const Shape &getShape() const;
  /**
   * @brief 角速度設計器を取得
   */
  const AccelDesigner &getAccelDesigner() const;

protected:
  Shape shape;      /**< @brief スラロームの形状 */
  AccelDesigner ad; /**< @brief 角速度用の曲線加速生成器 */
  float velocity;   /**< @brief 並進速度 */
};
```

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
