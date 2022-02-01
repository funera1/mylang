#pragma once
#include "include.cpp"
// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = Pbnf("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
vector<Pbnf> token_trans_table;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstr_to_tokenkind;
// 終端記号の集合
set<string> terminal_symbols;
