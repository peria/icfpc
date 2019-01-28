`howie` (`ADVTR`)
==================

適当にコマンドを叩いていくアドベンチャーゲーム。いくつか中間ゴール的なものが設定されていて

1. 周囲に散らばったアイテムを集めて何か鍵的なものを作る(チュートリアル)
2. 各地を巡ってアイテム合成をし downloader を作る
3. 同様に uploader を組み立てる

といった手順を取ることになる。


| 小問題 | 得点 | 備考 |
| ----- | --- | ---- |
| INC | 5 | アイテムを破壊する |
| CMB | 5 | アイテム合成する |
| KEY | 20 | 最初の部屋を突破する |
| USB | 20 | USB cable を合成する |
| DSP | 20 | display を合成する |
| PGB | 5 | progress bar を入手する(合成の必要なし) |
| PWR | 20 | power cord を合成する |
| JMP | 20 | jumper shunt を作る |

コマンド
-------

- `help` は簡単なコマンドリストと説明
- `go` は移動。`n`、`e`、`s`、`w` を引数に取るが、`go` が無くても四方の文字だけで移動できる。
- `take`、`get`、`grab` は手元にある重なった荷物の一番上のものを取る
- `drop` は持ってる荷物から1つ指定して足元の荷物スタックに置く。ただし改造したものは置けない。
- `incinerate`、`inc` アイテムを破壊する。state-of-the-art DVNUL-9000 incinerator という装備をしているらしい。
- `combine`、`c` はアイテムの修復。 `combine radio with transistor` という感じに使う。
- `use` はアイテムの機能があれば使う
- `examine`、`ex`、`x`、`look`、`l` はアイテムやら周りの状況やらを見る
- `inventory`、`show`、`inv`、`i` は持ってる荷物を表示する
- `quit`、`exit` はゲーム終了

世界地図
-------
最初の部屋を出ると世界中で色々ものを作るのでマップと作れるものリスト。

|Street\Avenue| |Ridgewood |Dorchester| Blackstone |
|-------------|-|--------- |----------| ---------- |
|52nd Street  | | X | progress bar | |
|53rd Street  | | X | display | |
|54th Street  | | downloader uploader | USB cable | textbook |
|54th place   | | X | power cord | jumper shunt |

downloader を作るときは display を最初に作ってから downloader を取らないと持てる荷物の枠が足りなくなる。
