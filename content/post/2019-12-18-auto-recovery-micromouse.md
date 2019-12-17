---
date: "2019-12-18T00:00:00+09:00"
title: "新機能・クラッシュ後の自動復帰！"
categories:
  - "作品紹介"
tags:
  - "マイクロマウス"
  - "KERISE"
draft: true
---

この記事は，[Micro Mouse Advent Calendar](https://adventar.org/calendars/4007) 18日目の記事です．

昨日の記事は mako さんの機体紹介でした．  
<!-- 感想を書く -->

## はじめに

こんにちは．けりです．

今回の記事では，  
私のマイクロマウス [KERISE v4](/posts/2018-05-03-kerise-v4-coming) に搭載されている  
新機能「**クラッシュ後の自動復帰**」を紹介します．

<!--more-->

{{< postfig src="kerisev4.jpg" title="KERISE v4" width="240px" link="kerisev4.jpg" >}}

### 目指すは自律賞

マイクロマウス競技には，「自律賞」という賞があります．  
現行の競技規定では次のように記されています．

> [自律賞](https://www.ntf.or.jp/mouse/rule/hyouka_micro-JA.html): 持ち時間内に全走行が完了（最後にスタート地点まで戻る）するまで、ノータッチで走り切ったマイクロマウスの内、最短走行時間を記録したマイクロマウスに対する評価

つまり，競技の全行程を**完全自律**で終えるというものです．

### 32x32迷路での完全自律マウスはまだいない

実は，過去のマイクロマウス競技では，自律賞を獲得したマウスが毎年存在しました．  
しかし，ここ数年で自律賞を獲得したマウスはありません．

その理由は，競技ルールの改正があったからです．  
2015年までは，自律賞の規定は次のようになっていました．

> [自律賞(2015年まで)](http://www.ntf.or.jp/mouse/micromouse2015/hyouka_half.html): 1回目のスタートから持ち時間内に全走行が完了(最後にスタート地点まで戻る)するまで、ノータッチで走り切ったマウスの内、最短走行時間を記録したマウスに対する評価。**全走行を完了したマウスがない場合は、最初にタッチした時までの最短完走時間を記録したマウスを評価**する。

以前のルールでは，最後に追加の1文がありました．  
たとえ探索走行だったとしても，1回でもゴールに達したマウスがいれば**必ず受賞者が現れる**というルールでした．

それが，2016年からは，以下の規定の**ベストマウサー**という賞が追加されて，
自律賞は，**完全自律マウス**に限定されました．

> [自律賞(2016年より)](https://www.ntf.or.jp/mouse/rule/hyouka_micro-JA.html): 持ち時間内に全走行が完了（最後にスタート地点まで戻る）するまで、ノータッチで走り切ったマイクロマウスの内、最短走行時間を記録したマイクロマウスに対する評価  
> [ベストマウサー(2016年より)](http://www.ntf.or.jp/mouse/rule/hyouka_micro-JA.html): １回目のスタートから最初に操作者がロボットに触れた時までの最短完走時間を記録したマイクロマウスを評価する  

この**ルール改正後に自律賞を獲得したマウスはまだ存在しない**のです．  
僕はこの賞を目指しています！！

### 完全自律マウスの課題

現行の自律賞，つまり完全自律走行を達成するためには，

- 制限時間内に全行程を終えるための時間管理
- 状況に応じた走行パラメータの自動選択
- タイヤのホコリ耐性
- **絶対にクラッシュしない**

が必要となります．

### 最短記録と自律走行のトレードオフ

上記で挙げた**絶対にクラッシュしない**という条件ですが，これはかなり困難な条件です．

なぜなら，マイクロマウスは走行時間を競う競技なので，
いい記録を残すためには**クラッシュするギリギリ**を攻めなければなりません．

最短走行の記録と自律走行にはトレードオフがあるのです．

### 新機能・自動復帰による解決

そんな困難な**完全自律走行**ですが，別の視点の攻略法があります．

それが今回紹介する**クラッシュ後に自動復するマウス**です！

おそらく今までに自動復帰するマウスを作りたいと考えた人は少なくないと思います．  
ただ，実装がかなり複雑で，実現した人はいませんでした．

僕は**約1年半前**から取り組み始め，遂に今年，**自動復帰マウス**を実現しました！

## 自動復帰の様子

最短走行でクラッシュしてしまったとき，次の要領で自動復帰して走行に戻ります．

1. クラッシュしてしまい，姿勢が乱れる
2. 前壁補正などを用いて，位置と姿勢を整える
3. 既知の迷路と比較して，自己位置が一意に定まるまで周辺区画の探索を行う
4. 自己位置を特定したら，リタイアにならないようにゴール区画を訪問する
5. スタート区画に戻り，次の最短走行を行う

<blockquote class="twitter-tweet"><p lang="ja" dir="ltr">今年の新機能「クラッシュ後の自動復帰」の様子です！最短走行でクラッシュ → 前壁補正で姿勢を整える → しばらく周辺を探索 → 自己位置を特定 → 記録を残すために一度ゴールを訪問 → スタートへ戻る → 次の最短走行 <a href="https://t.co/1WFvqikmfg">pic.twitter.com/1WFvqikmfg</a></p>&mdash; けり (@kerikun11) <a href="https://twitter.com/kerikun11/status/1206570447044628481?ref_src=twsrc%5Etfw">December 16, 2019</a></blockquote> <script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

## 自己位置同定アルゴリズム

今回実装した自己位置同定アルゴリズムですが，かなり頑張りました．

- 探索済みの迷路ならば，迷路内のどの位置・方向からでも復帰可能！
- 自己位置復帰中に**スタート区画**を避ける機能を搭載
- いくつかの壁の読み間違いを許容

### 自己位置同定の概要

- 新たな迷路情報を用意して，周辺のマスを探索する
- 32x32マスのズレと東西南北4方向の回転に対してパターンマッチングする
- 未知の壁は無視して，既知の食い違いを数える

## クラッシュ後の姿勢復帰方法

壁が少ない場所での復帰は前壁補正による復帰が難しいという問題がありました．
そこで，ToF測距センサで辺りを一周走査して平らな壁を探す方法を取りました．

<blockquote class="twitter-tweet"><p lang="ja" dir="ltr">クラッシュ直後は、姿勢を整えるために1周回って平らな壁を探す。そして前壁補正によって区画の中央へ復帰。 <a href="https://t.co/jMbGSj47mu">pic.twitter.com/jMbGSj47mu</a></p>&mdash; けり (@kerikun11) <a href="https://twitter.com/kerikun11/status/1206577717228920833?ref_src=twsrc%5Etfw">December 16, 2019</a></blockquote> <script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

## シミュレータによる検証

(動画)

## おわりに

### 今後の展望

自律賞を獲得する！