# sa-is
implementation of sa-is algorithm

## Usage 
``` shell
$ make
```

``` shell
$ ./make_input input.txt 1000
```

``` shell
$ ./sa-is input.txt output.txt
```

## Description
主にメモリ使用量の削減をするようにプログラムをリファクタリングした。

メモリ使用量は入力サイズの2倍であり、これはyuta mori氏の実装と一致する。

しかし、実行時間はyuta mori氏の実装のほうが数倍高速である。

