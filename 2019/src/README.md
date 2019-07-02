# アルゴリズムなどのメモ

# 問題セットアップ
## desc ファイルのパース

[9cc](https://www.sigbus.info/compilerbook) の入り口の所だけ読んだのを何となく思い出しつつ、tokenizer と parser とをゴッチャにした形で実装。ただし地図データは Polygon (`vector<Point>') で出力される。

## Polygon to grid

探索やらシミュレーションやらでは Polygon 形式の地図は使いづらいので Grid 状のデータに変換します。色んな条件から

- 横線だけ抽出する
  - 各横線の各 x 座標に y 座標を stack していく
- 各 x 座標に stack された y 座標のうち 2n 番目と 2n+1 番目の間を塗る

ということでやっています。各線の左側が内側、という設定は見てない。400x400 までだからまぁこれでも支障はない。

## Grid 地図

Wrapper が居る所や Booster がある所という情報とともに wrap 済かどうかの判定も欲しいので Bitset の 2 次元 vector として管理しています。([map.h](src/map.h)) また、wrap 済を flag にすると wrap していないところを探すのが大変になった経験から、non-wrapped を flag にしてます。

# 解法

TBD
