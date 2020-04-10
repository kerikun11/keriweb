---
title: "C++によるフィードバック制御器の実装"
date: "2020-04-07T23:33:05+09:00"
categories:
  - "作品紹介"
tags:
  - "マイクロマウス"
  - "制御"
thumbnail: ""
draft: true
---



<!--more-->

## 機体のモデルを同定

### 機体の伝達関数

左右のモーターの平均電圧から，機体の並進速度への伝達関数を1次系の線形モデルで近似します．

$$
y(s) = \frac{K_1}{T_1s+1} u(s)
$$

ただし，$T_1$は立ち上がり時間，$K_1$は定常ゲインです．

これは，モーター電圧から速度へのステップ応答を取ることで得ることができます．

## 逆モデルを導出

### 入力を出力で表す

機体のモデルを式変形して， $u(s)$ について解くと，

$$
\begin{align}
T_1 s y(s) + y(s) &= K_1 u(s)
\newline
\Longleftrightarrow \quad u(s) &= \frac{T_1}{K_1} s y(s) + \frac{1}{K_1} y(s)
\end{align}
$$

と表せます．

### 逆ラプラス変換

これを逆ラプラス変換すると，時間の関数として $u(t)$ が得られます．

$$
u(t) = \frac{T_1}{K_1} \dot{y}(t) + \frac{1}{K_1} y(t)
$$

<script type="text/x-mathjax-config">
    MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});
</script>
