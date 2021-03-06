---
date: "2016-08-14"
title: "IR-Stationの赤外線ダウンロード機能"
categories:
  - "作品紹介"
tags:
  - "IR-Station"
  - "ESP-WROOM-02"
  - "ESP8266"
  - "赤外線学習リモコン"
thumbnail: "icon.png"
---

## IR-Stationとは

自作のWiFi接続の赤外線学習リモコンです．IR-Stationを使えば，スマホやPCから家電製品をコントロールすることができます．

詳しくは，[IR-Stationについての記事一覧](/tags/ir-station)をご覧ください．

## IR-Stationの新機能

夏休みになり，IR-Stationを本格的に開発しています．今回は，いくつかの新機能を紹介します．

<!--more-->

### IR-Stationの操作画面
画面下方の設定やInfoの配置が少し変わり，見やすくなりました．

{{< postfig src="display.png" title="画面" >}}

### 設定画面
画面下方の設定画面では，以下のことができます．

  1. 赤外線信号を新規登録(Recode a new Signal)
  1. 既存の赤外線信号の名前を変更(Rename a Signal)
  1. 赤外線信号ファイルをアップロードして新規登録(Upload a new Signal)
  1. 既存の赤外線信号ファイルを端末にダウンロード(Download a Signal)
  1. 既存の赤外線信号を削除(Clear a Signal)
  1. すべての赤外線信号を削除(Clear All Signals)
  1. WiFiを変更(初期化)(Change WiFi)

{{< postfig src="action.png" title="設定" >}}

### 赤外線データファイルのダウンロード機能

赤外線信号を選んで`Submit`ボタンを押せばダウンロードできます．ダウンロードされるのはJSONファイルです．

{{< postfig src="download.png" title="ダウンロード画面" >}}

### 赤外線データファイルのアップロード機能

信号を割り当てたいチャンネルを選び，上でダウンロードした赤外線データファイルを選択します．任意で名前を入力して，`Submit`ボタンを押すと，赤外線が登録されます．

{{< postfig src="upload.png" title="アップロード画面" >}}

## 赤外線データファイルはJSONファイル

赤外線データファイルはJSON形式に従っており，以下のフォーマットになっています．

### JSONフォーマット

赤外線データは，単位がマイクロ秒の生データです．

0,2,4,6,...番目の数値が，38kHzの赤外線が存在する(High状態の)マイクロ秒時間  
1,3,5,7,...番目の数値が，赤外線が存在しない(Low状態の)マイクロ秒時間

です．

```
[
	"赤外線のHigh,Lowをマイクロ秒で表したuint16_tのリスト"
]
```


### JSONファイルの例

これに従った形式なら，自分で作ったファイルでもアップロード可能です．

```
[
	3300,1786,363,1305,390,473,364,476,317,548,289,577,288,501,364,474,390,475,290,550,289,1403,290,551,314,1377,290,552,313,502,362,1379,289,1378,289,1378,314,1377,389,1304,364,476,312,552,290,553,312,474,391,1305,362,501,364,474,361,504,289,553,312,477,363,500,364,476,287,578,289,1377,314,551,290,502,363,476,388,474,391,451,313,551,313,553,288,500,363,1304,385,1362,310,475,389,475,364,477,313,552,288,1380,317,50215,3305,3489,314
]
```

### 注意

JSONデータのフォーマットは，今後のアップデートで変更される可能性があります．最新情報は，[Githab/kerikun11/IR-Station](http://github.com/kerikun11/IR-station.git)にあるのでそちらをご覧下さい．

## まとめ

IR-Stationは，日々進化しています．今後のアップデートにもご期待を！


