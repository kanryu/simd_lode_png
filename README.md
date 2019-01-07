# simd_lode_png
A fast png loader implementation

## 注意

このプロジェクトはまだ開発中のものです。

## 目的

libpngより短時間でpng画像をビットマップにデコードするライブラリを作成します。
これは1スレッドで考える場合と並列処理化した場合の両方を含みます。
つまり1スレッド実行でもある程度高速処理できるようにした上で、並列実装を盛り込み更に高速化します。
最終的にlibpngの5倍以上の能力を目指します。

## 現在の内容

- libdeflate
    - submoduleを取ってきて、中でmsys2/mingw64版のmake/gccを使ってビルドしてください。msvcにやらせるとSIMD実装が含まれないビルドになります。
- lodepng
    - 数カ所にhackが入っています。
    - zlib処理をlibdeflateで差し替えれるようにしています。
    - intel/filter_sse2_intrinsics.c はlibpngから取ってきたものです。
    - intel/filter_sse2_intrinsics.c のビルドにはnasmが必要です。msys2のpacmanで入ります。
    - これからlodepng.cppに更にhackを入れていきます。
    - まずはオーバーヘッドの大きい decodeGeneric() および postProcessScanlines()あたりから。
- simd_lode_png
    - hackされたlodepngのクライアントです。
    - ここで実際の画像の読み込みテストをやります。

## 現時点での修正点

- zlibのデコード処理をlibdeflateに差し替えた
- unfilterをlibpngのものに差し替えた

## 今後の修正予定

- unfilterをparngのものに差し替える
- lodepngのオーバーヘッドを削る
- lodepngがやっている動的メモリ確保の回数を減らし、一度確保したメモリを使い回せるようにする
- 同時に、出力先のビットマップのバッファは外部から指定できるようにする
- lodepngが実施している各処理を並列化し、デコード所要時間を更に削る

## ライセンス

zlib/MIT/Apache2のいずれかの予定。

