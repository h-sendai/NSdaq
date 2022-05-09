# Monitor Draw(), Update()

ヒストグラムを画面に描画するのに10ミリ秒程度かかる。
その間もデータは上流からやってきて、Monitorコンポーネントの
InPortバッファに蓄積される。

InPortバッファ数はデフォルトでは256個になっている。この数で
十分かどうか、daq_run()のたびにまだ読んでいないバッファ数が
何個あるかログに出力してみた。

/usr/include/rtm/InPort.hの修正が必要。

unread bufが1個以上あるなら時刻とともにその数をログに出力
されてみた。

画面はローカルのデスクトップではなく、他ホストの画面にssh X11 Forwardを使って
表示させた（ローカルデスクトップに出すより不利な条件）。

いずれの行もヒストグラムの再描画のタイミングででてきていた。
最大6個なので256あればとりあえず十分か?

バッファ数を増やすにはconfig.xmlを編集する。

```
m_event_byte_size: 262144

17:16:20.582 unread_buf: 6
17:16:20.584 unread_buf: 5
17:16:20.585 unread_buf: 4
17:16:20.586 unread_buf: 4
17:16:20.587 unread_buf: 3
17:16:20.589 unread_buf: 2
17:16:20.590 unread_buf: 1

m_event_byte_size: 262144

17:16:23.187 unread_buf: 5
17:16:23.188 unread_buf: 4
17:16:23.189 unread_buf: 3
17:16:23.191 unread_buf: 2
17:16:23.192 unread_buf: 1

m_event_byte_size: 262144

17:16:25.796 unread_buf: 4
17:16:25.797 unread_buf: 3
17:16:25.798 unread_buf: 2
17:16:25.800 unread_buf: 1

m_event_byte_size: 262144

17:16:28.422 unread_buf: 4
17:16:28.424 unread_buf: 3
17:16:28.425 unread_buf: 3
17:16:28.426 unread_buf: 2
17:16:28.427 unread_buf: 1

m_event_byte_size: 262144

17:16:31.034 unread_buf: 4
17:16:31.035 unread_buf: 3
17:16:31.036 unread_buf: 2
17:16:31.037 unread_buf: 1

m_event_byte_size: 262144

17:16:33.657 unread_buf: 4
17:16:33.658 unread_buf: 3
17:16:33.659 unread_buf: 2
17:16:33.661 unread_buf: 1

m_event_byte_size: 262144

17:16:36.283 unread_buf: 4
17:16:36.285 unread_buf: 3
17:16:36.286 unread_buf: 2
17:16:36.287 unread_buf: 1
17:16:36.288 unread_buf: 1
```


