`gardener` (`ANTWO`)
====================

パターンマッチングによるアリの動きのプログラム化でパズルを解いていく問題。

| 小問題 | 得点 | 備考 |
| ----- | --- | ---- |
| 001 | 5 | |


### 概要
入力問題の `*` の部分を自由に設定できる。対応する番号のアリプログラムと周辺状況の組み合わせで次のターンの状況が決まる。
東西南北はアリの向きが合うようにパターンを見たときの相対的な方角なので、文字では NESW (東西南北) でも左右、直進、反転、のように捉えるのが良い。
パターンマッチは方角を固定せずに一致確認を行い、優先度の高いパターンから適用させる。
