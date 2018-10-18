ICFPC 2007 Endo ([Site](http://save-endo.cs.uu.nl/))
=====

## 問題概要
宇宙人 Endo の DNA が文字列で与えられている。これに適当な prefix を与えて処理することで生存確率を上げよう。
`ICFP` の 4 つの塩基で表現される DNA は適当な規則でもって RNA に変換される。 そして RNA はまた別の規則でもって画像に変換される。
元々の DNA を処理すると

![元の画像](http://save-endo.cs.uu.nl/source.png "元画像")

こんな画像が生成されるが、最終的にターゲット画像

![ターゲット画像](http://save-endo.cs.uu.nl/target.png "ターゲット画像") 

に近い画像を生成する prefix を与えることでスコアが得られる。


## 感想
入り口に立つまでが辛いので結構プログラム初心者殺しだと思う。具体的には DNA → RNA の変換で

> It seems particularly important to ensure that performing skips and appending unquoted references perform better than in linear time.
> (変換処理は線形時間より速いオーダーで処理ができることが超重要だよ)

とか書かれているので、与えられた仕様をそのまま実装するのもそこそこ重い実装なのにつらい。
C++ だと今では（このコンテストのおかげで）よく知られている `rope` やそれを実装したライブラリに関する知識を持っているというハードルが本質的にあるからだ。そこを超えてもまぁいろいろ大変なんだけど。
後からのんびりやるならおすすめ度☆☆☆☆★という感じだけど、リアルタイム参加に関してはあまりオススメできなかった。
