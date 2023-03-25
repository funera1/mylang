#pragma once
#include "include.cpp"
#include "tools.cpp"
#include "global_values.hpp"

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
							// string tmp_word = word.substr(1, sz(word)-2);
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
		}
	}
	// bnf_transition_list[term] = {}で初期化
	// for(string t : term_set){
	// 	bnf_transition_list.push_back(P_src_dst(t, vector<string>{t}));
	// }
}

string remove_quotation(string s){
	if(s[0] == '\"' && s[sz(s)-1] == '\"')s = s.substr(1, sz(s)-2);
	if(s[0] == '\'' && s[sz(s)-1] == '\'')s = s.substr(1, sz(s)-2);
	return s;
}

// {}: ループ
// dfa_graph[i][j] = (i-jに辺がある ? dst[j] : "")
// 状態0としてダミーノードを設けるので1個ずれる
vector<vector<string>> get_dfa(vector<string>& dst){
	// for(auto di : dst)cout << di << " ";cout << endl;
	vector<string> new_dst;
	int state_quantity = sz(dst) + 1;
	// dfa_graph[i][j]: (i, j)に辺があれば1, else 0
	vector<vector<string>> dfa_graph(state_quantity, vector<string>(state_quantity, ""));
	stack<int> loop_stack;
	for(int i = 0; i < sz(dst); i++){
		// ループ始まり
		if(dst[i] == "{"){
			int cnt = 0;
			while(dst[i] == "{"){
				cnt++;
				i++;
				if(i >= sz(dst)){
					cout << dst[0] << endl;
					assert(i < sz(dst));
				}
			}
			dst[i] = remove_quotation(dst[i]);
			new_dst.push_back(dst[i]);
			while(cnt--)loop_stack.push(sz(new_dst));
		}
		// ループ終わり
		else if(dst[i] == "}"){
			int top = loop_stack.top();
			cout << "[ " << new_dst[top-1] << " ]" << endl;
			cout << sz(new_dst) << "->" << top << endl;
			cout << "\\[ " << new_dst[top-1] << " ]" << endl;
 			dfa_graph[sz(new_dst)][top] = new_dst[top-1];
			loop_stack.pop();
		}
		else {
			dst[i] = remove_quotation(dst[i]);
			new_dst.push_back(dst[i]);
		}
	}
	for(int i = 0; i < sz(new_dst); i++){
		dfa_graph[i][i+1] = new_dst[i];
	}
	// dstからオプションなどを取り除いたものに更新する
	dst = new_dst;
	return dfa_graph;
}

void create_dfa_graphs(){
	for(int i = 0; i < sz(bnf_transition_list); i++){
		auto [src, dst] = bnf_transition_list[i];
		vector<vector<string>> dfa_graph = get_dfa(dst);
		dfa_graphs.push_back(dfa_graph);
		bnf_transition_list[i] = P_src_dst(src, dst);
	}
}

// 変数の初期化
void init(){
	// これらの関数は全てbnf_transition_listについて作用する
	create_bnf_transtion_list();
	create_dfa_graphs();
	for(int i = 0; i < sz(bnf_transition_list); i++){
		auto [src, dst] = bnf_transition_list[i];
		src = remove_quotation(src);
		bnf_transition_list[i] = P_src_dst(src, dst);
	}
}
