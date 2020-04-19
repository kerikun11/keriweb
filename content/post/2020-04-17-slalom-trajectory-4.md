---
title: "滑らかなスラロームの設計④ 設計結果の例"
date: "2020-04-18T14:00:00+09:00"
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

今回の記事では，[マイクロマウスの走行パターン](/posts/2017-09-03-pattern-of-turn/)について，ターンの軌道の例を紹介します．

### 対称性

![スラローム形状](shape/shape_1_xy.svg)

## 軌道の例

ここでは，1区画が 90 [mm] のマイクロマウス競技のスラローム走行軌道を紹介します．

### 最大角躍度，角加速度，角速度

これは設計パラメータのひとつです．
時間応答のグラフをみながら以下の値に設定しました．

| 項目         | 値                     | 備考                         |
| ------------ | ---------------------- | ---------------------------- |
| 最大角躍度   | 1200 $\pi$ [rad/s/s/s] | $\approx$ 216000 [deg/s/s/s] |
| 最大角加速度 | 36 $\pi$ [rad/s/s]     | $\approx$ 6480 [deg/s/s]     |
| 最大角速度   | 3 $\pi$ [rad/s]        | $\approx$ 540 [deg/s]        |

### #0 探索 90 度

![スラローム形状](shape/shape_0_xy.svg)

![スラローム軌道](shape/shape_0_t.svg)

### #1 最短 45 度

![スラローム形状](shape/shape_1_xy.svg)

![スラローム軌道](shape/shape_1_t.svg)

### #2 最短 90 度

![スラローム形状](shape/shape_2_xy.svg)

![スラローム軌道](shape/shape_2_t.svg)

### #3 最短 135 度

![スラローム形状](shape/shape_3_xy.svg)

![スラローム軌道](shape/shape_3_t.svg)

### #4 最短 180 度

![スラローム形状](shape/shape_4_xy.svg)

![スラローム軌道](shape/shape_4_t.svg)

### #5 最短斜め 90 度

![スラローム形状](shape/shape_5_xy.svg)

![スラローム軌道](shape/shape_5_t.svg)

### #6 最短斜めロング 90 度

![スラローム形状](shape/shape_6_xy.svg)

![スラローム軌道](shape/shape_6_t.svg)

### #1 〜 #4 まとめ

![区画の中心から出る軌道](shape_1_stack.svg)

## おわりに

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
