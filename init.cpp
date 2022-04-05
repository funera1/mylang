#pragma once
#include "include.cpp"
#include "tools.cpp"
#include "global_values.hpp"

// {}: ループ
// state_graph[i][j] = (i-jに辺がある ? dst[j] : "")
// 状態0としてダミーノードを設けるので1個ずれる
vector<vector<string>> get_state_graph_by_ebnf(vector<string> dst){
	vector<string> new_dst;
	int state_quantity = sz(dst) + 1;
	// state_graph[i][j]: (i, j)に辺があれば1, else 0
	vector<vector<string>> state_graph(state_quantity, vector<string>(state_quantity, ""));
	stack<int> loop_stack;
	for(int i = 0; i < sz(dst); i++){
		new_dst.push_back(dst[i]);
		// ループ始まり
		if(dst[i] == "{"){
			int cnt = 0;
			while(dst[i] == "{"){
				cnt++;
				i++;
			}
			new_dst.push_back(dst[i]);
			while(cnt--)loop_stack.push(sz(new_dst));
		}
		if(dst[i] == "}"){
			int top = loop_stack.top();
			// ループになっているのは{}の内側なので、i+1, i-1する
			// TODO: ↑だめ, }}になってるときバグる
			// TODO: {{もバグる
			state_graph[sz(new_dst)-1][top] = dst[top];
			loop_stack.pop();
		}
	}
	for(int i = 0; i < sz(new_dst); i++){
		state_graph[i][i+1] = new_dst[i];
	}
	return state_graph;
}

// 文法の遷移を3重vectorに写す
void create_bnf_transtion_list(){
	ifstream input_bnf = fileToIfstream(home_dir+"bnf");

	// この関数で返す値
	string now_line = "";

	set<string> term_set;
	while(getline(input_bnf, now_line)){
		// コメントはスルー
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
			if(separator == "="){
				while(i < sz(now_line)){
					string word = getNextStr(now_line, i);
					// 区切り
					if(word == "\\"){
						bnf_transition_list.push_back(P_src_dst(src, dst));
						dst = vector<string>();
					}
					else {
						// 終端記号(""か''で囲まれているもの)
						if((word[0] == '"' && word[sz(word)-1] == '"') || (word[0] == '\'' && word[sz(word)-1] == '\'')){
							// 引用符を消す
							word = word.substr(1, sz(word)-2);
							term_set.insert(word);
						}
						dst.push_back(word);
					}
				}
			}
			// それ以外ならエラー
			else {
				cout << "separatorが=でない" << endl;
				assert(0);
			}
			// TODO: bnf_src_to_dstとbnf_transition_listどっちも共通した過程を踏むのにbnf_transition_listのための関数になっているので調整する
			bnf_transition_list.push_back(P_src_dst(src, dst));

			// bnf_transition_list[term] = {}で初期化
			for(auto t : term_set){
				bnf_transition_list.push_back(P_src_dst(t, vector<string>{}));
			}
		}
	}
}


// 変数の初期化
void init(){
	create_bnf_transtion_list();
	create_nonterm_and_term_list();
}