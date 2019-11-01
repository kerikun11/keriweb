---
date: "2018-04-29T13:35:38+09:00"
title: "滑らかな加速の設計② 曲線加速の設計"
categories:
  - "説明記事"
tags:
  - "マイクロマウス"
thumbnail: "icon.png"
---

## 設計1・加速曲線の設計

[前回の記事](/posts/2018-04-29-accel-designer1/)の続きです．

この記事では，設計1の曲線加速の設計について考えます．

<!--more-->

### 滑らかな加速の設計 目次

- [滑らかな加速の設計① 台形加速と曲線加速](/posts/2018-04-29-accel-designer1/)
- [滑らかな加速の設計② 曲線加速の設計](/posts/2018-04-29-accel-designer2/)
- [滑らかな加速の設計③ 走行距離を考慮した速度設計](/posts/2018-04-29-accel-designer3/)
- [滑らかな加速の設計④ C++による実装例](/posts/2018-04-29-accel-designer4/)

## 設計内容

ここでは，下図のように，始点速度$v_s$から最大加速度$a_m$で加速して終点速度$v_e$に達するような場合を考えます．

注意点として，
最大加速度$a_m$で加速する時間を$t_m$としたとき，
始点速度$v_s$と終点速度$v_e$の差がある程度小さいと，
算出される$t_m$が負になる場合があります．

したがって，$t_m$の符号によって場合分けをして考える必要があります．

{{< postfig src="design1_cmp.png" title="加速曲線" width="480px" >}}

## 拘束条件
与える情報は以下の通りです．

- 最大加速度 $a_m$
- 始点速度 $v_s$
- 終点速度 $v_e$
- 加速度が曲線である時間 $t_c$

$t_c$は，実機の特性や生成した波形を見ながら経験的に適当に決めます．

## 求めた関数
求めた関数，値は以下の通りです．

- 任意の時刻 $t$ における，加速度 $a(t)$，速度 $v(t)$ および位置 $x(t)$ を返す関数

### 関数

任意の時刻$t$における各関数は，

$$
\\begin{align}
&
a(t) :=
\\left\\{ \\begin{array}{ll}
  0 & (\\hspace{2.2em}t \\le t_0) \\newline
  \\frac{a_m}{t_c}(t-t_0) & (t_0 < t \\le t_1) \\newline
  a_m & (t_1 < t \\le t_2) \\newline
  -\\frac{a_m}{t_c}(t-t_3) & (t_2 < t \\le t_3) \\newline
  0 & (t_3 < t \\hspace{2.2em})
\\end{array} \\right.
\\newline
&
v(t) :=
\\left\\{ \\begin{array}{ll}
  v_0                               & (\\hspace{2.2em} t \\le t_0) \\newline
  v_0 + \\frac{a_m}{2t_c}(t-t_0)^2  & (t_0 < t \\le t_1) \\newline
  v_1 + a_m(t-t_1)                  & (t_1 < t \\le t_2) \\newline
  v_3 - \\frac{a_m}{2t_c}(t-t_3)^2  & (t_2 < t \\le t_3) \\newline
  v_3                               & (t_3 < t \\hspace{2.2em})
\\end{array} \\right.
\\newline
&
x(t) :=
\\left\\{ \\begin{array}{ll}
  x_0 + v_0(t-t_0) & (\\hspace{2.2em} t \\le t_0) \\newline
  x_0 + v_0(t-t_0) + \\frac{a_m}{6t_c}(t-t_0)^3 & (t_0 < t \\le t_1) \\newline
  x_1 + v_1(t-t_1) + \\frac{a_m}{2}(t-t_1)^2 & (t_1 < t \\le t_2) \\newline
  x_3 + v_3(t-t_3) - \\frac{a_m}{6t_c}(t-t_3)^3 & (t_2 < t \\le t_3) \\newline
  x_3 + v_3(t-t_3) & (t_3 < t \\hspace{2.2em})
\\end{array} \\right.
\\end{align}
$$

と表すことができます．ただし，各定数は以下の通りです．

### 境界点の時刻

まずは，各境界点における時刻の定義です．

等加速度運動となる時間 $ t_m := \\frac{1}{a_m}(v_e - v_s) - t_c $ に対し，

$$
\\begin{array}{ll}
  \\mathrm{if}~(t_m > 0) &
  \\left\\{ \\begin{array}{l}
    t_0 := 0\\newline
    t_1 := t_0 + t_c\\newline
    t_2 := t_1 + t_m\\newline
    t_3 := t_2 + t_c
  \\end{array} \\right.
   \\newline
  \\mathrm{else}~(t_m \\le 0) &
  \\left\\{ \\begin{array}{l}
    t_0 := 0\\newline
    t_1 := t_0 + \\sqrt{\\frac{t_c}{a_m}(v_e-v_s)} \\newline
    t_2 := t_1\\newline
    t_3 := t_2 + (t_1-t_0)
  \\end{array} \\right.
\\end{array}
$$

### 境界点における定数
次に，境界点の各定数は，

$$
\\begin{align}
  &\\left\\{ \\begin{array}{l}
  v_0 := v_s \\newline
  v_1 := v(t_1) \\newline
  v_2 := v(t_2) \\newline
  v_3 := v_e
  \\end{array} \\right.
  \\newline
  &\\left\\{ \\begin{array}{l}
  x_0 := 0 \\newline
  x_1 := x(t_1) \\newline
  x_2 := x(t_2) \\newline
  x_3 := x_0 + \\frac{1}{2}(v_0 + v_3)(t_3-t_0)
  \\end{array} \\right.
  \\end{align}
$$

となります．

### 加速度の符号

ちなみに，最大加速度 $a_m$ の符号を適切に設定すれば，減速の場合にもそのまま使用できることに注意しましょう．つまり，

$$
a_m = \\mathrm{sign}(v_e-v_s) \\times|a_m|
$$

のように$a_m$の符号を設定すると上手くいきます．

## 導出過程

簡単に流れを書きます．

1. とりあえず最大加速度の時間を$t_m$と置いて，境界点の時刻$t_0,\\cdots,t_3$を$t_m$で表す．
1. 図から加速度$a(t)$を求める(1次関数)．
1. 加速度の積分$\\int_{t_0}^{t_3}a(t)dt = v_e$の方程式を解いて，$t_m$の値を求める．
1. $t_c\\le 0$なら，図より$t_1=t_2$であり，$v_1=v_2=\\frac{1}{2}(v_s+v_e)$(←中点)なので，$ v_1 = v_s+\\int\_{t_0}^{t_1}a(t)dt = \\frac{1}{2}(v_s+v_e)$の方程式を解いて$t_1,t_2$を再定義する．
1. 図より，面積である$x_3$を求める．
1. 対称性を考えながら，$a(t)$を積分して，$v(t)$を求める．
1. 対称性を考えながら，$v(t)$を積分して，$x(t)$を求める．

ポイントは，積分をする際に，$t_0$から単純に積分していくのではなく，$t_0$と$t_3$の双方から対称性を使って積分しているところです．

これは，$t_m$の符号による場合分けを最小限に抑えるためです．その結果，$t_m$による場合分けは境界点の定数のみで，関数の方には現れなくなりました．

## まとめ

今回の記事では，始点速度と終点速度を滑らかにつなぐ曲線加速の設計を行いました．

[次回の記事](/posts/2018-04-29-accel-designer3/)では，走行距離の拘束条件を満たすように，今回設計した曲線加速を組み合わせていきます．
