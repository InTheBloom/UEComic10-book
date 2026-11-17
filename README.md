# UEComic10-book

UEComic10向けの本の原稿・掲載コード・組版設定を管理するプロジェクトです。
Re:VIEW Starterを使用します。

## フォルダ構成

- `contents/`: 本文原稿（章の掲載順は `catalog.yml`）
- `code/`: 掲載するソースコード。本全体と同じGitで管理
- `images/`: 本文画像、表紙・裏表紙PDF、表紙の元PDF
- `sty/`、`layouts/`、`css/`、`data/`: 組版・表示設定と補助データ
- `lib/`、`review-ext.rb`: ビルド処理とRe:VIEW拡張
- `config.yml`、`config-starter.yml`: 書誌情報、用紙・本文幅などの設定
- `tmp/`: 過去のバックアップや作業ファイル（Git管理対象外）

生成した本のPDFや中間ファイルはGit管理対象外です。
`images/` 内のPDFは素材なので管理対象です。


## ユーザーズガイド

以下のサイトで「ユーザーズガイド」のPDFファイルをダウンロードできます。

- https://kauplan.org/reviewstarter/


## コンパイル方法

### 印刷用（表紙・裏表紙を含めない本文PDF）

```sh
./build-print
```

印刷用モード `STARTER_TARGET=pbook` で `UEComic10-book.pdf` を生成します。
表紙・裏表紙を除外し、モノクロで出力します。大扉・目次・奥付は含まれます。
既存の `UEComic10-book.pdf` は上書きされます。
表紙素材は `config-starter.yml` の `frontcover_pdffile`・`backcover_pdffile`
で指定した `images/` 内のPDFです。本文PDFとは別ファイルのまま保持します。

### 通常のビルド

Docker を使う場合：

```terminal
$ docker pull kauplan/review2.5    # 3GB 以上ダウンロードするので注意
$ unzip UEComic10-book.zip
$ cd UEComic10-book/
$ docker run --rm -v $PWD:/work -w /work kauplan/review2.5 rake pdf
$ ## または rake docker:pdf でもOK
$ ls UEComic10-book.pdf
```

Docker を使わない場合 (Ruby と Re:VIEW と LaTeX が必要)：

```terminal
$ gem install review --version 2.5  # 最初に1回だけ行う（2回目からは不要）
$ review version                    # Re:VIEWのバージョンを確認
2.5.0                               # （3系は未サポートなので注意!!）
$ unzip UEComic10-book.zip
$ cd UEComic10-book/
$ rake pdf
$ ls UEComic10-book.pdf
```

これで「UEComic10-book.pdf」が生成されます。


## ソースコードの部分掲載

コードの掲載範囲をコメントで囲みます。目印は単独の行に書きます。

```d
// book:begin levelOne
int levelOne (int[] A, int x) {
    // 実装
}
// book:end levelOne
```

原稿から、プロジェクトルートを基準としたファイル名と範囲名を指定します。

```text
//list[][][file=code/bsearch/a.d,region=levelOne]{
//}
```

`# book:begin 名前`、`-- book:begin 名前`、`/* book:begin 名前 */`、
`<!-- book:begin 名前 -->`、`(* book:begin 名前 *)`、`{- book:begin 名前 -}`、
`; book:begin 名前`、`% book:begin 名前` も使えます。終了側は `begin` を
`end` に変えます。範囲名には半角英数字・`_`・`.`・`-` が使えます。
目印の行は掲載されません。範囲の入れ子も使えますが、同じ名前の再定義や
開始・終了の不一致、指定した範囲がない場合はビルドエラーになります。

`region` を省略するとファイル全体を掲載します。
Dockerでのビルドにも、このプロジェクト内の実装がそのまま使われます。

## 問い合わせ

Twitter でハッシュタグ「#reviewstarter」つきでツイートしてください。
