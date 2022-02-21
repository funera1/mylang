#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../lexical.cpp"
#include "../parsing.cpp"
#include "../init.cpp"
#include "../RST.cpp"
#include "../AST.cpp"

int main(){
	string str = fileToString("input.txt");
    init();
    // cout << sz(bnf_transition_list) << endl;
	// 字句解析
	vector<string> input_stream;
	vector<string> token_stream;
	auto x = lexicalAnalysis(str);

    input_stream = x.first;
    token_stream = x.second;
    // for(int i = 0; i < sz(token_stream); i++)cout << token_stream[i] << endl;
    // for(auto [src, dst] : bnf_transition_list){
    //     cout << src << ": ";
    //     for(auto di : dst)cout << di << ", ";
    //     cout << endl;
    // }
    Parsing p;

    // DEBUG: first_set
    // cout << "###CHECK FIRST SET###" << endl;
    // for(auto [non_term, first_set] : p.first_sets){
    //     cout << non_term << ":" << endl;
    //     cout << '{';
    //     for(auto t : first_set){
    //         cout << t << ", ";
    //     }
    //     cout << '}' << endl;
    // }
    // cout << endl;
    // DEBUG: follow_setが成功してるかどうか
    // cout << "###CHECK FOLLOW SET###" << endl;
    // for(auto [non_term, follow_set] : p.follow_sets){
    //     cout << non_term << ":" << endl;
    //     cout << '{';
    //     for(auto t : follow_set){
    //         cout << t << ", ";
    //     }
    //     cout << '}' << endl;
    // }
    // cout << endl;
    // DEBUG: parsing tableが成功してるかどうか
    // auto t = p.create_ll_parsing_table();
    // for(auto ti : t){
    //     auto [non_term, term] = ti.first;
    //     int num = ti.second;
    //     if(num == -1)continue;
    //     cout << non_term << " " << term << " " << num << endl;
    // }
    // cout << endl << endl;

    // DEBUG: 構文解析のテスト
    p.parsing(token_stream);
    // DEBUG: RSTのテスト
    // node_t* node = init_node("TOKEN");
    node_t* root = create_RST(token_stream, input_stream);
}