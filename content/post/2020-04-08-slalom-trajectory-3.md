---
title: "滑らかなスラロームの設計③ 設計結果の例"
date: "2020-04-08T13:00:00+09:00"
categories:
  - "説明記事"
tags:
  - "マイクロマウス"
  - "制御"
thumbnail: "icon.png"
draft: true
---

## はじめに

前回の記事の続きです．

今回の記事では，[マイクロマウスの走行パターン](/posts/2017-09-03-pattern-of-turn/)について，ターンの軌道の例を紹介します．

<!--more-->

## 軌道の例

### 最大角躍度，角加速度，角速度

これは設計パラメータのひとつです．
時間応答のグラフをみながら以下の値に設定しました．

| 項目         | 値                     | 備考                         |
| ------------ | ---------------------- | ---------------------------- |
| 最大角躍度   | 1200 $\pi$ [rad/s/s/s] | $\approx$ 216000 [deg/s/s/s] |
| 最大角加速度 | 36 $\pi$ [rad/s/s]     | $\approx$ 6480 [deg/s/s]     |
| 最大角速度   | 3 $\pi$ [rad/s]        | $\approx$ 540 [deg/s]        |

### #0 探索90度

![スラローム形状](shape/shape_0_xy.svg)

![スラローム軌道](shape/shape_0_t.svg)

### #1 最短45度

![スラローム形状](shape/shape_1_xy.svg)

![スラローム軌道](shape/shape_1_t.svg)

### #2 最短90度

![スラローム形状](shape/shape_2_xy.svg)

![スラローム軌道](shape/shape_2_t.svg)

### #3 最短135度

![スラローム形状](shape/shape_3_xy.svg)

![スラローム軌道](shape/shape_3_t.svg)

### #4 最短180度

![スラローム形状](shape/shape_4_xy.svg)

![スラローム軌道](shape/shape_4_t.svg)

### #5 最短V90度

![スラローム形状](shape/shape_5_xy.svg)

![スラローム軌道](shape/shape_5_t.svg)

## まとめ

![区画の中心から出る軌道](shape_1_stack.svg)

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
