# NSdaq (Neutron Sensor DAQ)

中性子センサーDAQシステム

## 使い方

ひとつのディレクトリ(ここでは/home/daq/NSdaq)の下にソースコード、コンパイルしたバイナリ、
設定ファイルを置く使い方を想定している。

### DAQ-Middlewareのセットアップ

OSはCentOS 7を使うことを想定する。
OSのセットアップは [開発マニュアル](https://daqmw.kek.jp/docs/DAQ-Middleware-1.4.4-DevManual.pdf)
のG.3節を見ること。

DAQ-Middlewareをセットする:

```
wget https://daqmw.kek.jp/src/daqmw-rpm
sudo sh daqmw-rpm install
```

https://daqmw.kek.jp/ と通信できる環境ではこれでDAQ-Middlewareのセットアップが
完了する。

### ソースコードの取得とコンパイル

/home/daq/NSdaqの所有者は作業する人のアカウントでもよいし、共通アカウントを作り
それでログイン、作業するのでもよい。
ここではdaqという名前のユーザーが、/home/daq/NSdaqで作業する場合のコマンドを書く。

```
sudo mkdir -p /home/daq
sudo chown daq:daq /home/daq

git clone https://github.com/h-sendai/NSdaq.git
```

これで/home/daq/NSdaq/というディレクトリができてその中にコードがはいっている。
