---
title: "滑らかなスラロームの設計③ C++による実装"
date: "2020-04-18T13:00:00+09:00"
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

以下では，各クラスの概要を提示します．

```cpp
struct Position {
  float x, y, th; /*< 成分 */
};
```

```cpp
/**
 * @brief 軌道制御の状態変数
 */
struct State {
  Position q;
  Position dq;
  Position ddq;
  Position dddq;
};
```

```cpp
/**
 * @brief slalom::Shape スラロームの形状を表す構造体
 *
 * メンバー変数は互いに依存して決定されるので，個別に数値を変更することは許されない，
 * スラローム軌道を得るには slalom::Trajectory を用いる．
 */
struct Shape {
  Position total;      /**< 前後の直線を含めた移動位置姿勢 */
  Position curve;      /**< カーブ部分の移動位置姿勢 */
  float straight_prev; /**< カーブ前の直線の距離 [m] */
  float straight_post; /**< カーブ後の直線の距離 [m] */
  float v_ref;         /**< カーブ部分の基準速度 [m/s] */
  float m_dddth;       /**< 最大角躍度の大きさ [rad/s/s/s] */
  float m_ddth;        /**< 最大角加速度の大きさ [rad/s/s] */
  float m_dth;         /**< 最大角速度の大きさ [rad/s] */

public:
  /**
   * @brief 拘束条件からスラローム形状を生成するコンストラクタ
   *
   * @param total 前後の直線を含めた移動位置姿勢
   * @param y_curve_end
   * $y$方向(進行方向に垂直な方向)の移動距離，
   * カーブの大きさを決めるもので，設計パラメータとなる
   * @param x_adv $x$方向(進行方向)の前後の直線の長さ．180度ターンなどでは
   * y_curve_end で調節できないので，例外的にこの値で調節する．
   * @param m_dddth
   * @param m_ddth
   * @param m_dth
   */
  Shape(const Position total, const float y_curve_end, const float x_adv = 0,
        const float m_dddth = 1200 * M_PI, const float m_ddth = 36 * M_PI,
        const float m_dth = 3 * M_PI)
      : total(total), m_dddth(m_dddth), m_ddth(m_ddth), m_dth(m_dth) {
    /* 生成準備 */
    const float Ts = 1e-3; /**< シミュレーションの積分周期 */
    float v = 600.0f;      /**< 初期値 */
    State s;               /**< シミュレーションの状態 */
    AccelDesigner ad;
    ad.reset(m_dddth, m_ddth, 0, m_dth, 0, total.th);
    /* 複数回行って精度を高める */
    for (int i = 0; i < 3; ++i) {
      s.q.x = s.q.y = 0;
      /* シミュレーション */
      float t = 0;
      while (t + Ts < ad.t_end())
        integrate(ad, s, v, t, Ts), t += Ts;
      integrate(ad, s, v, t, ad.t_end() - t); //< 残りの半端分を積分
      /* 結果を用いて更新 */
      v *= y_curve_end / s.q.y;
    }
    curve = s.q;
    v_ref = v;
    const float sin_th = std::sin(total.th);
    const float cos_th = std::cos(total.th);
    /* 前後の直線の長さを決定 */
    if (std::abs(sin_th) < 0.001f) {
      /* 180度ターン */
      straight_prev = x_adv;
      straight_post = x_adv;
      curve = total;
    } else {
      /* 180度ターン以外 */
      straight_prev = total.x - s.q.x - cos_th / sin_th * (total.y - s.q.y);
      straight_post = 1 / sin_th * (total.y - s.q.y);
    }
  }
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
                        const float t, const float Ts) {
    /* Calculation */
    const std::array<float, 3> th{{ad.x(t), ad.x(t + Ts / 2), ad.x(t + Ts)}};
    std::array<float, 3> cos_th;
    std::array<float, 3> sin_th;
    for (int i = 0; i < 3; ++i) {
      cos_th[i] = std::cos(th[i]);
      sin_th[i] = std::sin(th[i]);
    }
    /* Runge-Kutta Integral */
    s.q.x += v * Ts * (cos_th[0] + 4 * cos_th[1] + cos_th[2]) / 6;
    s.q.y += v * Ts * (sin_th[0] + 4 * sin_th[1] + sin_th[2]) / 6;
    /* Result */
    s.dq.x = v * cos_th[2];
    s.dq.y = v * sin_th[2];
    s.q.th = ad.x(t + Ts);
    s.dq.th = ad.v(t + Ts);
    s.ddq.th = ad.a(t + Ts);
    s.dddq.th = ad.j(t + Ts);
    s.ddq.x = -s.dq.y * s.dq.th;
    s.ddq.y = +s.dq.x * s.dq.th;
    s.dddq.x = -s.ddq.y * s.dq.th - s.dq.y * s.ddq.th;
    s.dddq.y = +s.ddq.x * s.dq.th + s.dq.x * s.ddq.th;
  }
};
```

```cpp
/**
 * @brief slalom::Trajectory スラローム軌道を生成するクラス
 *
 * スラローム形状 Shape と並進速度をもとに，各時刻における位置や速度を提供する．
 */
class Trajectory {
public:
  Trajectory(const Shape &shape) : shape(shape) {}
  void reset(const float velocity, const float th_start = 0,
             const float t_start = 0) {
    this->velocity = velocity;
    const float gain = velocity / shape.v_ref;
    ad.reset(gain * gain * gain * shape.m_dddth, gain * gain * shape.m_ddth, 0,
             gain * shape.m_dth, 0, shape.total.th, th_start, t_start);
  }
  void update(State &s, const float t, const float Ts) const {
    return Shape::integrate(ad, s, velocity, t, Ts);
  }
  float getVelocity() const { return velocity; }
  float getTimeCurve() const { return ad.t_end(); }
  const Shape &getShape() const { return shape; }
  const AccelDesigner &getAccelDesigner() const { return ad; }

protected:
  const Shape &shape; /**< スラロームの形状 */
  AccelDesigner ad;   /**< 角速度用の曲線加速生成器 */
  float velocity;     /**< 並進速度 */
};
```

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
