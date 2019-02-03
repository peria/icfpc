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
| MSI | 20 | 博物館へ移動する |
| CON | 20 | [console](./console.txt) を解読する |

コマンド
-------

- `help` は簡単なコマンドリストと説明。
- `go` は `n`、`e`、`s`、`w` を引数に取り東西南北へ移動する。ちなみに `go` が無くても四方の文字だけでも移動できる。
- `take`、`get`、`grab` は手元にある重なった荷物の一番上のものを取る
- `drop` は持ってる荷物から1つ指定して足元の荷物スタックに置く。ただし改造したものは置けない。
- `incinerate`、`inc` アイテムをゴミ箱に入れる。物語が進めばゴミ箱から取り出すことも可能になる。
- `combine`、`c` はアイテムの修復。 `combine radio with transistor` という感じに使う。
- `use` はアイテムの機能があれば使う。
- `examine`、`ex`、`x`、`look`、`l` はアイテムやら周りの状況やらを見る
- `inventory`、`show`、`inv`、`i` は持ってる荷物を表示する
- `quit`、`exit` はゲーム終了
- (隠し) `break` はアイテムを破壊する。戻ってこない。
- (隠し) `whistle` 笛を吹いて自分を応援。
- (隠し) `hum` 鼻歌を歌って自分を応援。
- (隠し) `speak` 引数にした文字列を喋る。周囲の反応があるかはどうなんだろう。

解いていく流れ
-------------
まずは Room With a Door に [pamphlet](./pamphlet.txt) と [manifesto](./manifesto.txt) があるが、manifesto は `[_REDACTED_]`(検閲済み) とあり読めない。
とりあえずそれらを無視して北の Junk Room へ移動し、荷物を組み合わせて keypad を作る。
この keypad を Room With a Door に戻って使うとドアを開けて外に出ることができる(戻れない)。

### 世界地図
ちなみに外の世界の地図と、各エリア(実は全部部屋)で作れるものリスト。

|Street\Avenue| |Ridgewood |Dorchester| Blackstone | Harper |
|-------------|-|--------- |----------| ---------- | ------ |
|52nd Street  | | X | progress bar | manual | LED |
|53th Street  | | X | display | battery | EPROM burner |
|54th Street  | | downloader / uploader | USB cable | textbook | RS232 |
|54th place   | | X | power cord | jumper shunt | MOSFET |

出たところ (54th Street and Ridgewood Avenue) に [note](./note.txt) があるので読んでみると

- 2つ便利なものを残してるらしい。多分 downloader と uploader。
- 科学博物館 (the Museum of Science and Industry) で blueprint を見つけなければならない。
- 読めない場合は Censory Engine が行動ではなく視覚を邪魔してるらしい。

ということが分かる。

まずは上の地図に従って display, progress bar, USB cable, power cord, jumper shunt を作りそれぞれ downloader と合成することで downloader を修復できる。これを使うと [gc.rml](./gc.rml) というプログラムをダウンロードすることができる。
プログラムを読むとこの adventure のプログラム (の一部) であることが分かる。また、`break`、`whistle`、`hum`、`speak` といった隠しコマンドがあることがわかる。

次に一旦 downloader を無かったことにして同様に EPROM burner, MOSFET, RS232, status LED を合成して uploader を修復し、[gc.rml](./gc.rml) を改造した [gc2.rml](./gc2.rml) を upload する。[変更点](https://github.com/peria/icfpc/commit/586790aec2f23910e181d3f7e53621d37043d34e#diff-c1d34f318491eb9eb36c38a67b382491)は

- 持てる荷物を増やす
- `hum` で博物館へ移動する様にする

というあたり。博物館に移動すると、その南に `blueprint` があるので読んで見るが検閲済み。
頑張って `blueprint` を読みたいが、直接情報を読むことはできないし、検閲済みデータを使った演算結果も検閲済みになるので、[kinabaさんの当時のblog](http://www.kmonos.net/wlog/63.html#_0214060725)を参考に副作用を持つプログラムをuploadして解読していく。
今回は「`speak` する文字が、現在いる部屋の一番上に積んでいるアイテムの n 文字目(プログラム内 `index` で管理)であれば uploader を置く」というプログラムを作り、適当な順番に文字を `speak` する形にした。ヒットするまでは検閲済みな反応が返るが、ヒットすると uploader がプログラムのチェック対象になるので「youは x を叫んだが…」みたいな普通の反応になるので特定できる、という形にした。なお細かいことではあるが大文字は直接判定できなさそうなことが書いてあったので大文字用プログラムでは `string_charat()` に 32 (0x20) を足して小文字化するプログラムに変更した。

出来上がったものが [decrypt.py](decrypt.py) と [decrypt.rml](./decrypt.rml)。すると "Machine Room M4" という名前の部屋があることがわかる。

Machine Room M4 に行くと `console` というやはり検閲済みのものがあるのでこれも[解読してみる](./console.txt)。(ちなみに `note` に `knr` のパスワードが書かれているが、そのままでは通らない。)
するとこの世界 (adventure) の外 (umix) のメールやり取りが見え、`adventure` に `sequent-sequel` というオプションをつけて起動できることが分かる。

# 後編
`sequent-sequel` をつけて立ち上げると後編に入る。後編は前編で入れなかった博物館の中から始まる。
