# botanist documentation {#mainpage}

<img style="width:100%;" src="https://raw.githubusercontent.com/Enchan1207/botanist/master/banner.png">

botanistは、組み込みシステム向けに最適化された数式パーサと評価器を提供するライブラリです。

### Features

 - ライブラリが提供する機能を部分的に採用することができます:
    - 数式をトークナイズする (`botanist::Tokenizer`)
    - トークンリストから構文木を生成する (`botanist::Analyser`)
    - 構文木を直列化し、逆ポーランド記法に近い形に整形する (`botanist::Serializer`)
    - 整形された構文リストを評価し、計算結果を得る (`botanist::DoubleEvaluator`)
        - デフォルトでは倍精度浮動小数点数を用いた評価器が実装されていますが、ユースケースに合わせて自由に拡張することができます。
 - 小規模組込みシステム上での動作に特化しています:
    - C++11でコンパイルしています。
    - 動的メモリ割当て (`malloc`, `new` 等) を使用していません。
    - 例外 (`try`, `throw`) を使用していません。
    - STLに依存しません (コレクションライブラリには[collection2](https://github.com/Enchan1207/collection2)を採用しています)。
    - **全てのテストケースがAVRマイコン(atmega328p, atmega2560)でパスすることを(QEMUによるエミュレーションにより)確認しています。**

### Specification

botanistは以下のBNFによる数式を受け入れます。

```
expression := unary ( "+" unary | "-" unary )*
unary := factor ( "*" factor | "/" factor )*
factor := number | "(" expression ")"
number := real* | real "." real*
real := 0|1|2|3|4|5|6|7|8|9
```

変数や関数は現バージョン(v0.1.0)ではサポートされていません。

### Installation

現行バージョンでは以下のインストール方法をサポートしています:

 - ホストマシン上でビルドする
 - 既存のCMakeプロジェクトに依存関係として追加する

\note 構成およびインストールには [CMake](https://cmake.org/) が必要です。

#### 1. マシンに直接インストールする

プロジェクトルートで以下のコマンドを実行します:

```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --install .
```

この手順により、botanistが `/usr/local` 以下にインストールされます。


#### 2. 既存のCMakeプロジェクトに追加する

ホストマシンに直接インストールせず、既存のCMakeプロジェクトに依存関係としてインストールすることも可能です。  
既存プロジェクトの `CMakeLists.txt` に以下の記述を追加します:

```CMake
# Fetch library
include(FetchContent)
FetchContent_Declare(
    botanist
    GIT_REPOSITORY https://github.com/Enchan1207/botanist
    GIT_TAG v0.3.0
)
FetchContent_MakeAvailable(botanist)
```

次に、botanistを使用するターゲットに以下の記述を追加します:

```CMake
# Include directories are automatically set when linking
target_link_libraries(main PRIVATE
    botanist 
)
```

以上で完了です。

### Usage

各クラスの詳細な使用法については [API References](@ref apirefs) を参照してください。  
ソースファイルから自動生成されたドキュメントは <a href="annotated.html">**Classes**</a> より参照できます。

### Author, License

Author: [Enchan](https://github.com/Enchan1207)  
License: [MIT](https://opensource.org/license/mit/)
