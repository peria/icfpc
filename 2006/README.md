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
| [`guest`](guest/) | _なし_ | INTRO | 立ち上がった時点で教えてくれる。UMIX 入門 |
| [`ohmega`](ohmega/) | bidirectional | | |
| [`howie`](howie/) | xyzzy | | |
|ftd||||
|knr||||
|gardener||||
|yang||||
|hmonk||||
|bbarker||||
