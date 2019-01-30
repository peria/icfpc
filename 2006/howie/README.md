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
| JMP | 20 | jumper shunt を合成する |
| DNL | 5 | downloader を使う |
| BTU | 20 | battery を合成する |
| 232 | 20 | RS232 adapber を合成する |
| MOS | 20 | MOSFET を合成する |
| EPM | 20 | EPROM burner を合成する |
| LED | 20 | status LED を合成する |
| UPL | 5 | uploader でプログラムを upload する |

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

|Street\Avenue| |Ridgewood |Dorchester| Blackstone | Harper |
|-------------|-|--------- |----------| ---------- | ------ |
|52nd Street  | | X | progress bar | manual | LED |
|53th Street  | | X | display | battery | EPROM burner |
|54th Street  | | downloader / uploader | USB cable | textbook | RS232 |
|54th place   | | X | power cord | jumper shunt | MOSFET |

downloader を作るときは display を最初に作ってから downloader を取らないと持てる荷物の枠が足りなくなる。

解いていく流れ
-------------
まずは Room With a Door に [pamphlet](./pamphlet.txt) と [manifesto](./manifesto.txt) があるが、manifesto は `[_READACTED_]`(検閲済み) とあり読めない。
とりあえずそれらを無視して北の Junk Room へ移動し、荷物を組み合わせて keypad を作る。
この keypad を Room With a Door に戻って使うとドアを開けて外に出ることができる(戻れない)。

出たところ 54th Street and Ridgewood Avenue の足元に [note](./note.txt) があるので読んでみると
- 2つ便利なもの (downloader, uploader) を残してるらしい
- 科学博物館 (the Museum of Science and Industry) で blueprint を見つけなければならないっぽい
- 読めない場合は Censory Engine が行動ではなく視覚を邪魔してるらしい (この微妙な違いが後で重要になる)

という方針が分かる。

まずは 53th Street and Dorchester Avenue まで行き display を作る。
そして戻って downloader を拾い合成する。あとは上の地図に従って progress bar, USB cable, power cord, jumper shunt を作りそれぞれ downloader と合成することで downloader が治るのでこれを使うと [gc.rml](./gc.rml) というプログラムをダウンロードすることができる。

プログラムを読むとこの adventure のプログラムであることが分かる。コメントを読んでいくと `incinerate` は実はゴミ箱に荷物を送り込んでいて破壊してない(復旧できるはず)ということや `break`、`whistle`、`hum`、`speak` といった隠しコマンドがあることがわかる。

次に一旦 downloader を無かったことにして uploader を作り、[gc.rml](./gc.rml) を改造した [gc2.rml](./gc2.rml) を upload する。変更点は

- 持てる荷物を増やす
- `hum` で博物館へ移動する様にする

というあたり。
