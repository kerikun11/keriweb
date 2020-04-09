---
title: "滑らかなスラロームの設計① 滑らかなスラローム概要"
date: "2020-04-08T11:00:00+09:00"
categories:
  - "説明記事"
tags:
  - "マイクロマウス"
  - "制御"
thumbnail: "icon.png"
draft: true
---

## はじめに

今回はマイクロマウスのスラローム走行に関するお話です．

<!--more-->

### 目次

1. スラローム軌道の構成
2. 各パターンの生成結果
3. C++ライブラリの実装

### スラロームターンの種類

以前，
[マイクロマウスのターン一覧](/posts/2017-09-03-pattern-of-turn/)
という記事を書きました．

今回の記事では，そのスラロームターンを実現する軌道を紹介します．

### スラロームターンの構成

    直線 → 緩和曲線 → 円弧 → 緩和曲線 → 直線

### 並進速度が可変である必要性

ちなみに，速度は一定．
その一定速度が決め打ちなのか，走行時に可変なのかということ．

補正などをした結果，減速しきれない場合は，そのままの速度でスラロームに差し掛かる．

## 方法

直線 → 緩和曲線 → 円弧 → 緩和曲線 → 直線

![スラローム形状](shape/shape_2_xy.svg)

![スラローム軌道](shape/shape_2_t.svg)