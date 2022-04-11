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

レジスタの値を変更してテスト、を繰り返す場合、コマンドラインからでは
作業効率が上がらないので、ボタンを押せるGUIをLibreOffice Calcを使って
実装してみた。

LibreOfficeはOpenOfficeから2010年に派生したオフィスソフトで、
最近のLinuxディストリビューションではLibreOfficeを配布していることが
多い(CentOSもLibreOfficeを配布している）。

サンプルCalcファイルは
[../README.md](../README.md)にあるようにgit cloneした場合
/home/daq/NSdaq/control/calc/ns-registers-sample.ods
にあるのでこれをコピーして使う。

```
cd
cp NSdaq/control/calc/ns-registers-sample.ods ns-registers.ods
```

``ns-registers.ods``以外の名前にするときはこのファイル中にある
マクロを変更すること。

### ディレクトリ構成

- /home/daq/ns-registers.ods<br>
  LibreOffice Calcファイル
- /home/daq/NSdaq/control/calc/set-registers-calc<br>
  ns-registers.odsファイル中に含まれるマクロが呼び出すシェルスクリプト。

### LibreOffice Calcの準備

CentOS 7: yum -y install libreoffice

CentOS Stream 8: yum -y install libreoffice-calc

### LibreOffice Calcの起動方法

デスクトップのメニューからでも起動することができるが、
エラーログなどは端末にでる場合もあるので端末を開き（右クリックで
[端末を開く]を選ぶ）、

```
cd
libreoffice --calc ns-registers.ods
```

で起動する。
いったん起動したら
Tools -> Options -> LibreOffice -> Security -> Macro Security ...
でMediumか、Lowにセットする。Mediumにセットした場合、Calcファイル
を開いたときにマクロを有効にするかどうかのダイアログがでるようになる。

### セルフォーマット

A列: 名前、 B列: アドレス、C列: レジスタ長（バイト長）、D列: レジスタに
セットする値。F列以降は適当に使ってよい。設定プログラムではD列までしか
みない。

C列の値は0xが先頭にあれば16進数、0bが先頭にあればビット列と解釈する
(Pythonの整数プレフィックスと同じ)。
それ以外は10進数と解釈する。

例: 0x10 = 16 (10進数）、0b11110000 = 0xF0 = 240 （10進数）

### ターゲットIPアドレス

SiTCP機器のIPアドレスは192.168.10.16を使うようにしてある。
変更する場合は``calc/set-registers-calc``中の
``set-registers``呼び出しを
``set-registers 192.168.10.20``のように変更するIPアドレスを指定して
呼び出すようにする。
``

### マクロの中身

ns-registers.ods中のマクロを
[calc/macro.txt](calc/macro.txt)
においてある（もっといい書き方があれば教えてください）。

### ボタンを押したときの動作

1. まず``thisCompnent.store()``でCalcファイルを上書き保存する
2. /home/daq/NSDaq/control/set-registers-calcをマクロ関数shell()
を使って起動する。第4引数にTrueを指定しているのでCalcはset-registers-calc
が終了するまでCalcは停止する。set-registers-calcの中身:
    1. LibreOffice Calcを使ってCSVファイルに変換する
    2. control/set-registersでCSVファイルを読んでレジスタ値を設定する
3. set-registers-calcが終了したら、ステータスバーに"set registers: done"と表示する。1秒後に表示は消える。

### ToDo

set-registers-calc内でCSV変換にCalcを使っているが、LibreOfficeは
起動時にオプションを付けないとひとつのプロセスが全部の処理を行うようになる。
上記マクロ処理中でshell("command_path", , , True)としてcommand_pathが終了
するまでLibreOfficeは動作を停止する。よってCSVに変換する
Calcプロセスもそのままでは動作しなくなる（LibreOffice Calcのデッドロック）。
今回は起動時に
``-env:UserInstallation=file:///$HOME/.another-libreoffice``
オプションを付ければ複数のLibreOfficeプロセスが起動できるという
ワークアラウンドを使った（set-registers-calcを見よ）。
これによりCalcファイル -> CSVへの変換プログラムは書かなくて済んだが
Calcプロセスを新たに起動するのに遅いPCだと1秒程度はかかるのでボタンを
押してからレジスタセット完了まで1秒程度かかることになっている。
Calcファイル -> CSVへの変換をLibreOffice Calc自体で実行するのではなく
軽量別プログラムにすると待ち時間が短くなるかもしれない。
