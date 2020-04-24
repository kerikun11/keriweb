---
title: "Manjaro Linux でデスクトップがしばしばフリーズする問題の解決方法"
date: "2020-04-24T12:05:29+09:00"
categories:
  - "備忘録"
tags:
  - "Linux"
---

今回の記事は，同じような不具合を抱えている方への情報共有です．

<!--more-->

## 症状

私が Manjaro Linux Cinnamon で体験したフリーズの症状は以下のとおりです．

- ウィンドウを動かしたときや，文字入力中などにデスクトップが突然フリーズしてしまう
- マウスは動くがクリックなどをしても反応がない
- キーボードを押しても反応がない
- `Ctrl+Alt+F2` を押すとターミナルが起動する
- そのターミナルで，コマンド `killall -HUP cinnamon; cinnamon --replace --display=:0` を実行すると，セッションが再起動されて復旧する
- ソフトウェアレンダリングセッションでは，デスクトップ全体はフリーズしないが，ウィンドウ単位でフリーズすることがある

## 環境

私のPC環境は以下のとおりです．

![システム情報](sysinfo.png)

`DELL XPS 13 (9380)` + `Manjaro Linux Lysia 20.0 Cinnamon` です．

## 解決策

以下のようにファイルを記述することで完全に治りました．

```sh
$ cat /etc/X11/xorg.conf.d/20-intel.conf
Section "Device"
   Identifier  "Intel Graphics"
   Driver      "intel"
   Option      "AccelMethod"  "uxa"
EndSection
```

どうやらグラフィックドライバのオプションの設定が必要だったようです．

## おわりに

これが正しい方法かはわかりませんが，治ったのでよしとしましょう．

同じような不具合を抱えている方は是非試してみてくだい．
