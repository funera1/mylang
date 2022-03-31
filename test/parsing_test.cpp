#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../lexical.cpp"
#include "../parsing.cpp"
#include "../init.cpp"
#include "../RST.cpp"
#include "../ast/AST.cpp"
#include "../interpreter/interpreter.cpp"
#include "../packrat_parsing.cpp"
#include "../packrat_rst.cpp"

void dbg_print_token_stream(vector<string> token_stream){
    for(int i = 0; i < sz(token_stream); i++)cout << token_stream[i] << endl;
}

void dbg_check_first_set(Parsing p){
    cout << "###CHECK FIRST SET###" << endl;
    for(auto [non_term, first_set] : p.first_sets){
        cout << non_term << ":" << endl;
        cout << '{';
        for(auto t : first_set){
            cout << t << ", ";
        }
        cout << '}' << endl;
    }
    cout << endl;
}

void dbg_check_follow_set(Parsing p){
    cout << "###CHECK FOLLOW SET###" << endl;
    for(auto [non_term, follow_set] : p.follow_sets){
        cout << non_term << ":" << endl;
        cout << '{';
        for(auto t : follow_set){
            cout << t << ", ";
        }
        cout << '}' << endl;
    }
    cout << endl;
}

void dbg_check_parsing_table(Parsing p){
    auto t = p.create_ll_parsing_table();
    for(auto ti : t){
        auto [non_term, term] = ti.first;
        int num = ti.second;
        if(num == -1)continue;
        cout << non_term << " " << term << " " << num << endl;
    }
    cout << endl << endl;
}

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

    parsing(token_stream);
    auto t = get_rst(token_stream);
    for(auto ti : t)cout << ti << " ";cout << endl;
    // DEBUG
    // dbg_print_token_stream(token_stream);
    // Parsing p;

    // DEBUG: first_set
    // dbg_check_first_set(p);
    // DEBUG: follow_setが成功してるかどうか
    // dbg_check_follow_set(p);
    // DEBUG: parsing tableが成功してるかどうか
    // dbg_check_parsing_table(p);
    // DEBUG: 構文解析のテスト
    // p.parsing(token_stream);
    // DEBUG: RSTのテスト
    // node_t* node = init_node("TOKEN");
    // nonterm_node* root = create_RST(token_stream, input_stream);
    // all_watch_RST(root);
    // statement_node* st_root = RST_to_AST(root);
    // all_watch_AST(st_root);
    // interpreter(st_root);
}