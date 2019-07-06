# Status viewer

色んなモノを作っていく上で比較とかシミュレーションとかするために CGI スタイルで簡易ウェブサーバを作ります。
CSS Framework を適用させて見栄えもなんとなくちゃんとするつもり。

### 起動方法

```sh
$ python3 -m http.server --cgi 8080
```

`8080` はポート番号なので適当に。上記の起動方法の場合、例えば [http://localhost:8080/](http://localhost:8080/) で起動します。

### 構成

- `index.html`: メニュー。README と比較してちゃんと更新されるのはどっちか…
- `cgi-bin/`: python で書いた cgi というかデータベース。ほとんどここに入るはず。
  - `time.py`: 各ソルバーエンジンの素点での比較
