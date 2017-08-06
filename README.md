# Dummy Unix File System
***

## 内容

UnixV6のファイルシステムをC++で擬似的に再現したもの。  
テストデータ(TestBlockDevice)は、UnixV6のファイル構造になっている。  
当ファイルシステムは、(bashで使用されるような)各コマンドを実装している。

***
## 実行方法

`$ make`

`$ ./dummyFS`

or

`$ make run`

----
## コマンドの種類
- ls
- ls -l
- cd

----
## 開発環境

macOS Sierra (10.12.3)  
Apple LLVM version 8.0.0 (clang-800.0.42.1)  
Target: x86_64-apple-darwin16.4.0  
Thread model: posix  
