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

起動方法

```
./cmdrbcp.py3
```
説明については
https://github.com/h-sendai/SitcpRbcp/blob/master/py3/README.ja.md
をご覧ください。

## GUIプログラム

unreleased yet
