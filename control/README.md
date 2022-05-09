# NSdaq用レジスタコントロールプログラム

## バッチジョブ用コマンドラインプログラム

各レジスタに値を設定するプログラム。コマンドラインから実行する。

設定内容はcsvファイルから読みだす。
csvファイルのフォーマットは次のとおり

```
レジスタ名,アドレス,レジスタ長,設定する値
```

数値は先頭に0xがあれば16進数と解釈する。
0xで始まっていなければ10進数と解釈する。

コマンド起動方法

```
set-registers csvファイル IPアドレス
```

起動例
```
./set-registers try00.csv 192.168.10.16
```

``-v``オプションを付けると進捗状況を表示します。
例:

```
% ./set-registers -v sample.csv
---> trying user_area_0 0xffffff3c 0x10 (16) ... done
---> trying user_area_1 0xffffff3d 0x20 (32) ... done
---> trying user_area_2 0xffffff3e 0x30 (48) ... done
---> trying user_area_3 0xffffff3f 0x40 (64) ... done
```

## コマンドラインインタラクティブジョブ用プログラム

SiTCP RBCP汎用のプログラムを
https://github.com/h-sendai/SitcpRbcp/
においてあります。これを利用してください。

python3を使うプログラム cmdrbcp.py3 は
https://github.com/h-sendai/SitcpRbcp/blob/master/py3/README.ja.md
に説明があります。

## GUIプログラム

ボタンを押すとSiTCP RBCPでレジスタ値をセットできる
プログラム(LibreOffice Calcを使用)が
https://github.com/h-sendai/SitcpRbcp/tree/master/LibreOffice-Calc
にあります。セットアップ、使い方もこのページに
書いてありますのでご利用ください。
