#pragma once
#include "include.cpp"
// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = Pbnf("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
// TODO: token_trans_tableより遷移のリストみたいな名前のほうが良さそう
vector<Pbnf> token_trans_table;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstr_to_tokenkind;
// 終端記号の集合
// TODO: terminal_symbolsいらないので消す
set<string> terminal_symbols;
map<string, vector<vector<string>>> bnf_src_to_dst;
vector<string> nonterm_list;
vector<string> term_list;
string home_dir = "/home/funera1/workspace/mylang/";