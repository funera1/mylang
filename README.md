# mylang
### developブランチ
bnfファイルで定義された文法を満たすインタプリタとして使うことができます。パーサはLL(1)法です。
お手元の環境で実行するには、global_values.hppのhome_dirをこのディレクトリがあるpathに指定していただく必要があります。
input.txtにソースコードを入力し、outputに実行結果またはエラーの内容が出力されます

### feature/ebnfブランチ
packrat parser with left reculusionとpegのループオプションの導入をしている途中です。
