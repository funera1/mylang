#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"


// bnf_transition_listとnonterm_listが必要
class Parsing {
	// private:
	public:
		map<string, set<string>> first_sets;
		map<string, set<string>> follow_sets;
		map<P_nonterm_term, int> ll_parsing_table;

		void create_first_set(){
			// とりあえず一つの非終端記号についてだけやる->全ての非終端記号についてやる
			// 全ての非終端記号を見る
			for(auto [src, dst] : bnf_transition_list){
				// 既に見てたら飛ばす
				if(!first_sets[src].empty())continue;
				set<string> first_set;
				first_set.insert(src);
				// 最初は自分自身の非終端記号を持ってスタート
				// MEMO: 無限ループしてそう
				while(!all_is_term_in_set(first_set)){
					set<string> first_set_tmp;
					for(auto first_set_i : first_set){
						// first_set_iが終端記号ならそのまま継続 
						if(is_term(first_set_i))first_set_tmp.insert(first_set_i);
						// そうでないなら、遷移させたものを入れる
						else {
							for(auto dst_i : bnf_src_to_dst[first_set_i]){
								first_set_tmp.insert(dst_i[0]);
							}
						}
					}
					first_set = first_set_tmp;
					// DEBUG: first_setの移り変わりを見る
					// cout << '{';
					// for(auto fs : first_set)cout << fs << ", ";
					// cout << '}';
					// cout << endl;
				}
				first_sets[src] = first_set;
			}
		}

		void create_follow_set(){
			// nonterm_listをnonterm_queに変換する
			queue<string> nonterm_que;
			for(auto non_term : nonterm_list) nonterm_que.push(non_term);
			map<string, bool> seen;

			while(sz(nonterm_que)){
				auto non_term = nonterm_que.front();
				nonterm_que.pop();
				bool complete = true;
				// すでに見たかどうかを覚えておく
				seen[non_term] = true;

				// follow_setにsrcをキーとするfollow_setを追加する
				auto add_follow_set_to_follow_set = [&](set<string> follow_set, string src){
					// follow_set(src)がある時
					if(seen[src]){
						follow_set.insert(ALL(follow_sets[src]));
						return true;
					}
					// follow_set(src)がない時
					nonterm_que.push(non_term);
					complete = false;
					return false;
				};
				set<string> follow_set;
				{
					for(auto [src, dst] : bnf_transition_list){
						for(int i = 0; i < sz(dst); i++){
							// 関係ないときはスルー
							if(non_term != dst[i])continue;
							// 次の要素がないとき
							if(i + 1 >= sz(dst)){
								add_follow_set_to_follow_set(follow_set, src);
								break;
							}
							// 次の要素があるとき
							string follow_str = dst[i + 1];
							// follow_strが終端記号のとき
							if(is_term(follow_str)){
								follow_set.insert(follow_str);
							}
							// follow_strが非終端記号のとき
							else {
								// first_set(follow_str)を追加
								// follow_set.insert(ALL(first_sets[follow_str]));
								bool exist_eps = false;
								for(auto first_set_i : first_sets[follow_str]){
									if(first_set_i == "#eps"){
										exist_eps = true;
										continue;
									}
									follow_set.insert(first_set_i);
								}
								// 空文字が含まれてるとき
								if(exist_eps){
									add_follow_set_to_follow_set(follow_set, src);
									break;
								}
							}
						}
					}
				}
				seen[non_term] = complete;
				if(complete){
					follow_sets[non_term] = follow_set;
					// DEBUG: follow_setの中身
					// cout << non_term << ": ";
					// all_watch_in_set(follow_set);
				}
			}
		}
		Parsing(){
			create_first_set();
			create_follow_set();
			ll_parsing_table = create_ll_parsing_table();
		}
		set<string> get_first_set(string s){
			if(is_term(s)){
				return set<string> {s};
			}
			else return first_sets[s];
		}

		set<string> get_follow_set(string s){
			if(is_term(s)){
				return set<string> {};
			}
			else return follow_sets[s];
		}
		// TODO: A -> abcについてfirst_set(a)にepsが含まれるときにバグるはずなので直す
		map<P_nonterm_term, int> create_ll_parsing_table(){
			// P_nonterm_term(non_term, term)
			map<P_nonterm_term, int> ll_parsing_table;

			// すべての(non-term, term)の組み合わせについて表を作る
			// algorithm:
			// T[A, a]にA->wが入るのは以下の場合のみ
			// - aがfirst_set(w)に含まれる
			// - epsがfirst_set(w)に含まれ、aがfollow_set(A)に含まれる
			for(int i = 0; i < sz(bnf_transition_list); i++){
				auto [src, dst] = bnf_transition_list[i];
				auto first_set = get_first_set(dst[0]);
				// DEBUG: first_setの中身
				// cout << src + "'s first_set:" << endl;
				// for(auto si : first_set){
				// 	cout << si << " ";
				// }
				// cout << endl;
				bool eps_exist = false;
				for(auto first_i : first_set){
					if(first_i == "#eps"){
						eps_exist = true;
					}
					ll_parsing_table[P_nonterm_term(src, first_i)] = i;
				}
				// 空文字がfirst_set(w)に含まれる場合
				if(eps_exist){
					// A -> wにおいてepsがFi(w)に含まれ、aがFo(A)に含まれる
					auto follow_set = get_follow_set(src);
					for(auto follow_i : follow_set){
						ll_parsing_table[P_nonterm_term(src, follow_i)] = i;
					}
					// assert(0);
				}
			}
			return ll_parsing_table;
		}
		
		int get_ll_parsing_table(string non_term, string term){
			// (non_term, term)について構文解析表が定義されていない場合エラーを出す
			if(ll_parsing_table.find(P_nonterm_term(non_term, term)) == ll_parsing_table.end()){
				// (non_term, "#eps")がある場合はそれを返す
				if(ll_parsing_table.find(P_nonterm_term(non_term, "#eps")) != ll_parsing_table.end()){
					return ll_parsing_table[P_nonterm_term(non_term, "#eps")];
				}
				cout << "<" << non_term << ">と<" << term << ">は";
				cout << "構文解析表にありえないトークンの組み合わせです" << endl;
				assert(0);
				return -1;
			}
			return ll_parsing_table[P_nonterm_term(non_term, term)];
		}

		bool parsing(vector<string> token_stream){
			stack<string> parsing_stack;
			// 初期値
			parsing_stack.push("$");
			parsing_stack.push("PROGRAM");

			int token_stream_cursor = 0;
			while(token_stream_cursor < sz(token_stream)){
				auto token_i = token_stream[token_stream_cursor];
				string parsing_stack_top = parsing_stack.top();
				// DEBUG:
				// cout << token_i << " " << parsing_stack_top << endl;
				// DEBUG
				// all_watch_in_stack(parsing_stack);
				// stackのtopが$の場合
				if(parsing_stack_top == "$"){
					// 入力バッファとスタックどちらも$のとき
					if(token_i == parsing_stack_top){
						parsing_stack.pop();
						token_stream_cursor++;
						cout << "OK:構文解析終了" << endl;
						return true;
					}
					// それ以外のとき
					else {
						cout << "構文エラーです" << endl;
						return false;
					}
				}
				// stackのtopが#epsの場合
				else if(parsing_stack_top == "#eps"){
					// 入力を進めずにstackだけ抜く
					parsing_stack.pop();
				}
				// stackのtopが終端記号の場合
				else if(is_term(parsing_stack_top)){
					if(token_i == parsing_stack_top){
						token_stream_cursor++;
						parsing_stack.pop();
					}
					else {
						cout << "構文エラーです" << endl;
						return false;
					}
				}
				// stackのtopが非終端記号の場合
				else {
					int transition_num = get_ll_parsing_table(parsing_stack_top, token_i);
					if(transition_num < 0){
						cout << "構文エラーです" << endl;
						return false;
					}
					auto [src, dst] = bnf_transition_list[transition_num];
					parsing_stack.pop();
					// stackには後ろから入れる
					for(int dst_cur = sz(dst) - 1; dst_cur >= 0; dst_cur--){
						parsing_stack.push(dst[dst_cur]);
					}
				}
			}
		}
};

