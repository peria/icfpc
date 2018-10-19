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
