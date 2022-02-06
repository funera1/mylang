#pragma once
#include "include.cpp"
// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = P_Src_Dst("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
// TODO: bnf_transition_listより遷移のリストみたいな名前のほうが良さそう
vector<P_src_dst> bnf_transition_list;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstr_to_tokenkind;
// 終端記号の集合
// TODO: terminal_symbolsいらないので消す
set<string> terminal_symbols;
map<string, vector<vector<string>>> bnf_src_to_dst;
vector<string> nonterm_list;
vector<string> term_list;
string home_dir = "/home/funera1/workspace/mylang/";