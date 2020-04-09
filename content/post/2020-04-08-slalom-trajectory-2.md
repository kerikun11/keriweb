---
title: "滑らかなスラロームの設計② 設計方法"
date: "2020-04-08T12:00:00+09:00"
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

<!--more-->

## 入出力

### 拘束条件

- 移動位置，角度
- 飽和角躍度，角加速度，角速度
- 調整用の直線距離

### 走行時に得られる情報

- 各時刻 $t$ における角度 $\theta(t)$，角速度 $\dot{\theta}(t)$，角加速度 $\ddot{\theta}(t)$，角躍度 $\dddot{\theta}(t)$
- 各時刻 $t$ における位置 $x(t)$, $y(t)$
- 並進速度 $v(t)$ (ターン中は一定値なのであまり関係ない)

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
