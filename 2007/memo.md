# やったこと

- endo.dna をそのまま実行する → no_input
- Endo.pdf の最後にある prefix を利用 → verify
- 0 prefix での RNA をはじめの 1300 だけ使って絵をdump → no_input_first.png
- no_input_first で出てきた prefix を利用 → repair_guide
- ガイドの下にある prefix → light
- ガイドのカタログ用と言われてる prefix → catalog_1337
  I=0 と C=1 を使って数字をビット表現してページ指定ができるらしい。並び順が逆なのに注意。
  1337=0b10100111001 → CIICCCIICIC (最後のCが無い？)
- ガイド&カタログのprefixを見比べる
  ガイド:
```
 IIPIFFCPICFPPICIICC IICIPPP FIIC
   ( cCIF PCFF P  )I   /ln00 C  /  <- Pattern&Template
```
  カタログ：

```
 IIPIFFCPICFPPICIICCCIICIPPPCFIIC
   ( cCIF PCFF P  )II  /ln00IC  /  <- Pattern&Template
```
  Template での I/C がページ数に該当？とすると1337ページは
```
  IIPIFFCPICFPPICIICCCCCCCCCCCCIICIPPPFCCFFFCCFCFIIC
    ( cCIF PCFF P  )IIIIIIIIIII  /ln00CIICCCIICIC  /  <- Pattern&Template
```
- ビンゴ。以降メモの詳細は各prefixの2行目以降に書くことに。
- 目次をもとにいくつか生成 ([structure](input/structure.md), [encodings](input/encodings.md), [list](input/list.md), [lookout](input/lookout.md), [charset](input/charset.md), [field](input/field.md), [notes](input/notes.md), [lsystems](input/lsystems.md), [security](input/security.md), [history](input/history.md))
- [field](input/field.md) と [list](input/list.md) から apple のお絵かきを呼び出すやり方が何となくわかった気がするのであとで試す。引数無しで該当位置＆長さを呼び出す様に数値変換。
- [appletree](input/apple.md)を使うと原点ベースで林檎の木が描画される。ただし最後に適当な文字を付けてfinishさせないと通常のRed/Green Zoneに入って黒塗りされるので消えてしまう。
- その他色んな所からのメタ情報込みだけど、Red Zoneにmainルーチンが書かれていて、Green Zoneに書かれている各種関数を呼び出している、と。そしてBlue Zoneは変数やらスタック情報やらの保存領域になってる。問題になっているprefixはその前処理を行うことでRed Zoneが目的の絵を描くよう調整しましょう、と。なのでBlue Zoneだけじゃなくて多分RedもGreenも対象に変更することがあるはず。
- Green Zone の関数は `IFPICFPPCCC` で区切られている。リスト化すべき？
- Green Zone の関数呼び出しは
```
IIPIFFCPICCFPICICFFFIIPIFFCPICCFPICICFFFIICIICIICIPPPxxxxxxICyyyICIPPCPIIC
```
で行える。`xxxx`がGreen Zoneのoffset位置で`yyy`が関数のサイズ。引数を取る関数については呼び出す前に
```
IIPIFFCPICCFPICICFPCICICIICIICIPPPxxxxxxxICIIC
```
で数(24bit確定)を、
```
IIPIFFCPICCFPICICFPCICICIICIICIPPPxIIC
```
でboolean（`P`:false、?:true）をセットすることができる。結果が返ってくる関数の場合はBlue Zone (`IFPICFPPCFIPP`)の後ろに置かれるらしい。のでどうにかしてコピーしてくるかそのまま別関数に投げるかできるはず。
- 位置の調整や[明るくなるprefix](input/light.md) はどう説明されるんだろ？
