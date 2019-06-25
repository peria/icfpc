# ICFPC 2019 : Wrappy & Lambda-chain ([Site](https://icfpcontest2019.github.io/))

# 問題概要

今回のコンテストの問題は 2 問。1 問は全時間対象の Wrappy、もう 1 問は開始後 38 時間後開始、69 時間後終了の Lamda-chain。

## Wrappy

与えられた部屋を塗っていくお仕事を行う。2 次元グリッドでのスプラトゥーンという感じ。部屋内にある Booster を利用するなどして短時間で全マスを塗ることを目的とする。

Booster には下記のようなものがある

- Manipulator: 一度に塗る領域を増やせる
- Fast Whee: 50 ステップの間、移動速度が 2 倍に。複数回使うと有効時間が 50 ステップ ~~になる~~ 増える
- Drill: 30 ステップの間、壁を突き抜けられるようになる
- Beacon: 一旦 Beacon を設置したあとは任意のタイミングでそこにテレポートできる
- Cloning: Spawn の場所で使うとクローンできる。クローンでできた Wrapper は場所以外は初期状態。


## Lamda-chain

Wrappy を使いつつ、もう 1 つ別のパズルを解いて block-chain もどきを行う。本来 Lightening 終了のあと、4 時間後から開始する予定だったが対応できたチームが少なすぎたせいで開始を 10 時間遅らせることに。

各ブロックでは (puzzle, task) が問題として与えられる。task は Wrappy で解ける問題、puzzle は次の task を生成するための制約である。task をこなした solution のうち時間が短い 25 チームにコインが配布される。また、上位 10 チームの中から抽選で 1 チームの提出にある task が次のブロックでの task として使われる。

```
|   block 0       |    block 1        |      block 2      |   ...
| 出題   | 提出    |   出題    |  提出 |   出題    |  提出  |   ...
|-----------------|-------------------|-------------------|-------
| puzzle => task -+   puzzle => task -+   puzzle => task -+   puzzle
| task   => sol   +-> task   => sol   +-> task   => sol   +-> task
```

