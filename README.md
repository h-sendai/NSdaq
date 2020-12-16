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

### ROOTのセットアップ

ヒストグラムを作るのに[ROOT](https://root.cern/)を使っています。
ROOTのパッケージはFedoraが運営するEPEL (Extra Packages for Enterprise Linux)
リポジトリにありますのでこれを利用するのが簡単です。
以下のように作業します。
```
root# yum install epel-release
root# yum install root
```
なんらかの理由でこの方法が使えない場合は[Install ROOT](https://root.cern/install/)
を見てください。

ROOTは、ROOTSYS環境変数が必要な場合があります(NSMonitorもそうなっています)。
各自のshellでROOTSYS環境変数の値を/usr/local/rootに設定しておきます。
ログインシェルにbashを使っているユーザー全員で設定していい場合は
/etc/profile.d/root.shに

```
ROOTSYS=/usr
export ROOTSYS
```

と書いてログインしなおします。

このままの状態ではEPELリポジトリが有効になっており、パッケージの更新
yum updateなどでDAQ-Middlewareで必要としているomniORBについて、より
新しいものに更新しようとしてエラーになることがあります。omniORBはパッケージ
アップデート対象からはずします。設定は/etc/yum.confの最後に次の1行を追加します。
```
exclude=omniORB*
```

### ソースコードの取得とコンパイル

/home/daq/NSdaqの所有者は作業する人のアカウントでもよいし、共通アカウントを作り
それでログイン、作業するのでもよい。
ここではdaqという名前のユーザーが、/home/daq/NSdaqで作業する場合のコマンドを書く。

```
sudo mkdir -p /home/daq
sudo chown daq:daq /home/daq

cd /home/daq
git clone https://github.com/h-sendai/NSdaq.git
```

これで/home/daq/NSdaq/というディレクトリができてその中にコードがはいっている。
