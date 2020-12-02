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
ROOTのセットアップは[Install ROOT](https://root.cern/install/)
に書かれています。
CentOS 7の場合だと、ROOTが使っている依存ライブラリをインストールし、
ROOTバイナリtarballを取得し、展開します。具体的作業は以下のようにすると
よいと思います。

#### 依存ライブラリのインストール

```
root# rpm --quiet -q epel-release || yum -y install epel-release
root# vi /etc/yum.repos.d/epel.repo
enabled=1となっている箇所が１か所あるので
enalbed=0に変更（不用意にepelからパッケージをダウンロード、あるいは
既存パッケージをアップデートしないようにするため）

yum install --enablerepo=epel git cmake3 gcc-c++ gcc binutils libX11-devel libXpm-devel libXft-devel libXext-devel openssl-devel
```

#### ROOTのダウンロードと設定

[ROOT Releases](https://root.cern/install/all_releases/)から最新の
日付をクリックし、CentOS 7用バイナリtar.gzファイルをダウンロードする。
ダウンロード後 /usr/local 以下に展開する:

```
root# tar xf root_v6.XX.YY.Linux-centos7-x86_64-gcc4.8.tar.gz -C /usr/local
```

/etc/ld.so.conf.d/root.confを作成する:

```
root# echo /usr/local/root/lib > /etc/ld.so.conf.d/root.conf
root# ldconfig
```

最後のldconfigはroot.confの設定を有効にするために実行します。
リブート時には自動で実行されるので再起動後はldconfigを実行する必要は
ありません。

さらにROOTは、ROOTSYS環境変数が必要な場合があります(NSMonitorもそうなっています)。
各自のshellでROOTSYS環境変数の値を/usr/local/rootに設定しておきます。
ログインシェルにbashを使っているユーザー全員で設定していい場合は
/etc/profile.d/root.shに

```
ROOTSYS=/usr/local/root
export ROOTSYS
```

と書いてログインしなおします。

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
