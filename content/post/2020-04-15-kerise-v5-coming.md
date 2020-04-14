---
date: "2020-04-15T14:53:42+09:00"
title: "New KERISE v5，登場"
categories:
  - "作品紹介"
tags:
  - "マイクロマウス"
  - "ロボット"
  - "KERISE"
thumbnail: "icon.jpg"
draft: "true"
---

こんにちは．けりです．

今年の新作「KERISE v5」を紹介します．

<!--more-->

{{< postfig src="kerise-v5.jpg" title="KERISE v5" width="360px" >}}

## KERISE v5

KERISE v5は小型化を追求しました．

{{< postfig src="kerise-v5-1.jpg" title="KERISE v5" width="360px" >}}
{{< postfig src="kerise-v5-2.jpg" title="KERISE v5" width="360px" >}}
{{< postfig src="kerise-v5-3.jpg" title="KERISE v5" width="360px" >}}
<!-- {{< postfig src="kerise-v5-4.jpg" title="KERISE v5" width="360px" >}} -->
{{< postfig src="kerise-v5-6.jpg" title="KERISE v5" width="360px" >}}
{{< postfig src="kerise-v5-7.jpg" title="KERISE v5" width="360px" >}}

## KERISE v4 との比較

4輪吸引機構の KERISE v4 とは形がかなり異なり，
KERISE v5 は非吸引の2輪です．

{{< postfig src="kerise-v4-v5.jpg" title="KERISE v4 と KERISE v5" width="360px" >}}

## スペック

{{< postfig src="kerise-v5-8.jpg" title="KERISE v5" width="360px" >}}

| スペック         | KERISE v5                     | 備考                                 |
| ---------------- | ----------------------------- | ------------------------------------ |
| 縦 x 横 x 高     | 30 [mm] x 32 [mm] x 12.6 [mm] |                                      |
| 重量             | 8.72 [g] / 6.08 [g]           | バッテリー あり/なし                 |
| マイコン         | ESP32-PICO-D4                 | 240MHz dual core                     |
| モーター         | Φ6 [mm] x 10 [mm]             | AliExpress ノーブランド              |
| モータードライバ | DRV8835                       | 1.5A x 2ch                           |
| エンコーダ       | MA730 + Φ3 x 1 [mm] 磁石      | 自作磁気式エンコーダ                 |
| IMU              | ICM-20602                     | 3軸加速度 + 3軸ジャイロ              |
| ToF              | VL6180X                       | 赤外線レーザー測距センサ             |
| フォトリフレクタ | SFH4045N + SFH3015FA          | 赤外線LED + 赤外線フォトトランジスタ |
| バッテリー       | 100 [mAh] /                   | AliExpress ノーブランド              |

{{< postfig src="kerise-v5-weight.jpg" title="KERISE v5 の重量" width="240px" >}}

1.6 こじまうす程度です．こじまうすおかしい...

### 3D設計

例によってFusion 360で設計しています．個人用なら無料で使えるのはとてもありがたいです．

<iframe src="https://myhub.autodesk360.com/ue2805ff3/shares/public/SH56a43QTfd62c1cd9680714eff12a91d83a?mode=embed" width="800" height="600" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"  frameborder="0"></iframe>

## 特徴

### 小型，低重心，低慣性

{{< postfig src="kerise-v5-9.jpg" title="KERISE v5" width="360px" >}}
{{< postfig src="kerise-v5-5.jpg" title="KERISE v5" width="360px" >}}

### フォトリフレクタの小型化

{{< postfig src="photo-reflector-v4.jpg" title="KERISE v4" width="360px" >}}
{{< postfig src="photo-reflector-v5.jpg" title="KERISE v5" width="360px" >}}

## まとめ

4輪のマウスの制御の難しさを痛感していたので，
久々に2輪のマウスに戻ってきました．

これをもとにいろいろな制御を試してみたいと思っています．
