Readerで256kB読むことにする。

ROOTヒストグラムデータ: 1 fill 10ns
256kB 1event 2バイト。fillするデータは1バイトなので
256kBでは128k回fillすることが必要。
256kBではfillにかかる時間は
10*10^(-9)*128*10^(3)
= 1280*10^(-6) sec
= 1.28 msec

256kB
------- = 0.0125 = 12.5 ms に１回データがくる。
20MiB/s

Histogram Draw() + TCanvas Updateに10ms程度かかる。

MonitorにはバッファがあるのでTCanvas Updateで
ヒストグラムデータをアップデートできないあいだは
データはバッファにたまり、前段を待たせることはない。
