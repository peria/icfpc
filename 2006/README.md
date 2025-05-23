ICFPC 2006 : Umix  ([Site](http://www.boundvariable.org/index.shtml))
====

概要
----

1. 与えられた仕様で VM (Umix machine ; um) を作成する
2. 事前にもらえるファイル codex.umz を um に食わせて参加登録するともらえる description key を入力すると謎の OS (Umix) をダンプできる
3. um に umix を食わせると umix os が走り、あとは Unix 風な使い勝手で色々与えられる問題を解く

UMIX 内部
----
UMIX 内部に用意されているアカウント毎に問題が用意されていて、問題を解いていくと点数をもらえたり別のユーザーアカウントをハックできたりする。点数を貰える場合は

```
INTRO.LOG=200@999999|35e6f52e9bc951917c73af391e35e1d
```

という感じの文字列が現れるので本番のときはこの文字列をsubmitした。ちなみに `大問題`.`小問題`=`点数`@`チームID`|`確認用hash` というフォーマットになっている。
で、各ユーザー(大問題ジャンル)は以下の通り。

| User | Password | Code | 備考 |
| ----:| --------:| ---- | ---- |
| [`guest`](guest/) | _なし_ | INTRO | UMIX 入門 |
| [`ohmega`](ohmega/) | bidirectional | CIRCS | 2D プログラム |
| [`howie`](howie/) | xyzzy | ADVTR | 壮大なテキストベース冒険プログラム…か？ |
| [`ftd`](ftd/) | falderal90 | BASIC | `guest`のBASICができたか確認など。|
| [`hmonk`](hmonk/) | COMEFROM | ADVIS | O'Cult の実装 |
| [`gardener`](gardener/) | mathemantica | ANTWO | Antomaton で実装 |
| [`bbarker`](bbarker/) | plinko | BLACK | 条件付きあみだくじを作る |
| [`yang`](yang/) | U+262F | BLNCE | 極小 VM アセンブラ BALANCE で実装 |
| [`knr`](knr/) | X3.159-1989 |||

ちなみにそれぞれのユーザーアカウントをハックできる条件をいくつか。

- `guest` : ログイン画面に書いてる
- `ohmega` : `guest` のプログラムを走らせる
- `howie` : `guest` のプログラムを走らせる、`bbarekr` で 000 を解いてランキングを見る
- `ftd` : `guest` のプログラムを完成させて走らせる
- `hmonk` : `ftd` の `TODO` に書いている、`howie` で部屋を出る
- `gardener` : `hmonk` で 1 問解くとメールが来る
- `bbarker` : `gardener` の `.history` に書いている
- `yang` : `bbarekr` で 000 を解いてランキングを見る、`howie` で部屋を出る
- `knr` : `howie` で ”Machine Room M4" のnoteを見る 
