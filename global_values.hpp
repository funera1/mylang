#pragma once
#include "include.cpp"
// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = P_Src_Dst("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
vector<P_src_dst> bnf_transition_list;
// dstの移動先のリスト
vector< vector<vector<string>> >dfa_graphs;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstr_to_tokenkind;
// map<string, vector<vector<string>>> bnf_src_to_dst;
vector<string> nonterm_list;
vector<string> term_list;
string home_dir = "/home/funera1/workspace/mylang/";
// set<string> key_tokens = {"int", "if", "while"};
map<string, string> key_tokens = {
    {"int", "INT"},
    {"if", "IF"},
    {"while", "WHILE"},
    {"print", "PRINT"}
};
const int INF = 1e9;