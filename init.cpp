#pragma once
#include "include.cpp"
#include "tools.cpp"
#include "global_values.hpp"


// 文法の遷移を3重vectorに写す
vector<P_src_dst> create_bnf_transtion_list(){
	ifstream input_bnf = fileToIfstream(home_dir+"bnf");

	// この関数で返す値
	vector<P_src_dst> local_bnf_transition_list;
	string now_line = "";

	set<string> term_set;
	while(getline(input_bnf, now_line)){
		// コメントはスルー
		// if(sz(now_line) >= 2 && now_line[0] == '/' && now_line[1] == '/')continue;
		if(sz(now_line) >= 1 && now_line[0] == ';')continue;
		// 空行もスルー
		if(now_line == "")continue;

		//srcとdstを分ける
		string src = "";
		vector<string> dst;
		{
			int i = 0;
			// srcの単語を取得する
			src = getNextStr(now_line, i);
			// separator( ::= or := )を取得
			string separator = getNextStr(now_line, i);
			// ::=のとき
			if(separator == "::="){
				while(i < sz(now_line)){
					string word = getNextStr(now_line, i);
					dst.push_back(word);
				}
			}
			// :=のとき
			else if(separator == ":="){
				// 右辺は一つのtokenで終端記号
				string word = getNextStr(now_line, i);
				dst.push_back(word);
				term_set.insert(word);
			}
			// それ以外ならエラー
			else {
				cout << "separatorが:=でも::=でもない" << endl;
				assert(0);
			}
			// TODO: bnf_src_to_dstとbnf_transition_listどっちも共通した過程を踏むのにbnf_transition_listのための関数になっているので調整する
			bnf_src_to_dst[src].push_back(dst);
			local_bnf_transition_list.push_back(P_src_dst(src, dst));

			// bnf_transition_list[term] = {}で初期化
			for(auto t : term_set){
				bnf_transition_list.push_back(P_src_dst(t, vector<string>{}));
			}
		}
	}
	return local_bnf_transition_list;
}


// 変数の初期化
void init(){
	bnf_transition_list = create_bnf_transtion_list();
	create_nonterm_and_term_list();
}