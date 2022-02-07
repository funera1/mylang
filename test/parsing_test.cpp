#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../lexical.cpp"
#include "../parsing.cpp"
#include "../init.cpp"

int main(){
	string str = fileToString("input.txt");
    init();
	// 字句解析
	vector<string> input_stream;
	vector<string> token_stream;
	auto x = lexicalAnalysis(str);

    input_stream = x.first;
    token_stream = x.second;
    for(int i = 0; i < sz(token_stream); i++)cout << token_stream[i] << endl;
    // assert(0);
    Parsing p(bnf_transition_list, nonterm_list);
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
    bool f = p.parsing(token_stream);
}