---
date: "2019-11-01T13:00:00+09:00"
title: "nRF5マイコンのコマンドライン開発"
categories:
  - "説明記事"
tags:
  - "Nordic"
  - "nRF5"
draft: true
---

<!--more-->

以下のツールをインストールする．

- [GNU Arm Embedded Toolchain 4.9-2015-q3-update](https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update)
  - マイコンのコンパイラを含むツールチェーン
  - 下記のパスにインストールする．
    - Windows    : `C:/Program Files (x86)/GNU Tools ARM Embedded/4.9 2015q3` (デフォルトのインストール場所)
    - Linux / OSX: `/usr/local/gcc-arm-none-eabi-4_9-2015q3` (圧縮ファイルをダウンロードしてここに解凍する)
- [nRF Command Line Tools](https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools/Download)
  - 書き込みに使うJLinkやhexファイルのマージをするツール群を含む
- [Visual Studio Code](https://code.visualstudio.com/)
  - 次の拡張機能をインストールする．(拡張機能タブで検索してインストール)
    - `Cortex-Debug`
    - `C/C++`
