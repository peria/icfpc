# ICFPC 2024 School of the Bound Variable ([Site](https://icfpcontest2024.github.io/))

## 問題概要

まずは HTTP の POST でやり取りを開始すると ICFP 表現と呼ばれる特殊プログラム言語で返事が来るので、
それを評価して情報を集める。すると段階的に4種のパズルが解けるようになる。

パズル1 `lambdaman` は迷路の中で点をすべて回収する。解くだけであればあまり難しくはないが、
評価である ICFP 表現を以下に短くするかが鍵。

パズル2 `spaceship` はターン制の加速度を調整して指定された座標を巡る問題。
難易度が上がると行動数が増えるので、入力サイズ上限にかからないよう上手く ICFP 表現を短くする必要がある。

パズル3 `3d` は問題内で定義されたプログラム言語 `3D` を使って与えられた課題を解く。

パズル4 `efficiency` は、結果が数になる ICFP 表現のプログラムを与えられるので評価して結果となる数を答える。
仕様に従って素直に評価するとマシンがどんなに高速でも1000年単位で終わらないので、
色々な情報から結果だけ上手く導く必要がある。

## 感想

過去の ICFP コンテストへの参照が盛り沢山(過去のコンテストを知っていることで今回のコンテストが有利になることは特に無い)
であった。まず全体の枠として複数の小問があるスタイルは [2006 年の UMIX](../2006) を踏襲し、
[3D](./3d) は UMIX 内の問題 2D を、[lambdaman](./lambdaman) は 2012 年のコンテストを、[spaceship](./spaceship) は [2020 年の問題](../2020)
を元ネタにしている。それぞれの問題文にもチラホラ過去のコンテストへの言及がある。

2006 年以来？途中で問題のアップデートが行われず(正確には24時間経過時点で `3d` や `efficiency` をunlockしていない
チームにも開放された)、72 時間同じ条件の問題であった。

## メモ

復習する際には[公式](https://github.com/icfpcontest2024/icfpc2024) で運営側のサーバーを立てる方法も
紹介されているので、手元の PC で試すことができる。

問題が複数種類あるので分業はやりやすかったし、マンパワーがあるチームは有利だったかもしれない一方で、
逆に問題の種類があまり多くないこととアップデートが無いことで、行き詰まった時に気晴らしに外の問題へ移る事が
難しかったのは今回のコンテストでの数少ない欠点だったと思う。

上位は上位で、おそらく解法が近い中での最適化ゲームになってたのではないかと懸念している。

## Note
```
$ python3 -m venv .venv
$ source .venv/bin/activate
$ pip3 install requests z3-solver
```