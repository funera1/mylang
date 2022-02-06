#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"


class Parsing {
	// private:
	public:
		map<string, set<string>> first_sets;
		map<string, set<string>> follow_sets;
		vector<P_src_dst> bnf_transition_list;
		vector<string> nonterm_list;

		void create_first_set(){
			// とりあえず一つの非終端記号についてだけやる->全ての非終端記号についてやる
			// 全ての非終端記号を見る
			for(auto [src, dst] : bnf_transition_list){
				// 既に見てたら飛ばす
				if(!first_sets[src].empty())continue;
				set<string> first_set;
				// 最初は自分自身の非終端記号を持ってスタート
				first_set.insert(src);
				while(!all_is_term_in_set(first_set)){
					set<string> first_set_tmp;
					for(auto si : first_set){
						for(auto dst_i : bnf_src_to_dst[si]){
							first_set_tmp.insert(dst_i[0]);
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
				// すでに見たかどうかを覚えておく
				seen[non_term] = true;

				set<string> follow_set;
				// 初期化
				follow_set.insert(non_term);
				while(!all_is_term_in_set(follow_set)){
					// TODO: follow_set_tmpいる？
					set<string> follow_set_tmp;
					for(auto [src, dst] : bnf_transition_list){
						for(int i = 0; i < sz(dst); i++){
							// 関係ないときはスルー
							if(non_term != dst[i])continue;

							// 次の要素がないとき
							if(i + 1 >= sz(dst)){
								// follow_set(src)がある時
								if(seen[src]){
									follow_set_tmp.insert(ALL(follow_sets[src]));
									break;
								}
								// follow_set(src)がない時
								nonterm_que.push(non_term);
								break;
							}
							// 次の要素があるとき
							string follow_str = dst[i + 1];
							// follow_strが終端記号のとき
							if(is_term(follow_str)){
								follow_set_tmp.insert(follow_str);
								break;
							}
							// follow_strが非終端記号のとき
							else {
								// 空文字が含まれてるとき
								if(is_in_set(first_sets[follow_str], "#eps")){
									follow_set_tmp.insert(src);
								}
								else {
									// first_set(follow_str)を追加
									follow_set_tmp.insert(ALL(first_sets[follow_str]));
									// for(auto fs : first_sets[follow_str]){
									// 	follow_set_tmp.insert(fs);
									// }
								}
							}
						}
					}
					follow_set = follow_set_tmp;
				}
				follow_sets[non_term] = follow_set;
			}
		}
		Parsing(vector<P_src_dst> arg_bnf_transition_list, vector<string> arg_nonterm_list){
			bnf_transition_list = arg_bnf_transition_list;
			nonterm_list = arg_nonterm_list;

			create_first_set();
			create_follow_set();
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
		map<P_nonterm_term, int> create_ll_parsing_table(){
			// P_nonterm_term(non_term, term)
			map<P_nonterm_term, int> ll_parsing_table;
			// 初期化
			for(auto nonterm : nonterm_list){
				for(auto term : term_list){
					ll_parsing_table[P_nonterm_term(nonterm, term)] = -1;
				}
			}


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

				// 空文字がfirst_set(w)に含まれる場合
				if(is_in_set(first_set, "#eps")){
					auto follow_set = get_follow_set(dst[0]);
					for(auto follow_i : follow_set){
						ll_parsing_table[P_nonterm_term(src, follow_i)] = i;
					}
				}
				// 空文字がfirst_set(w)に含まれない場合
				else {
					for(auto first_i : first_set){
						ll_parsing_table[P_nonterm_term(src, first_i)] = i;
					}
				}
			}
			return ll_parsing_table;
		}
		bool parsing(vector<string> token_stream){
			auto parsing_table = create_ll_parsing_table();
			stack<string> parsing_stack;
			// 初期値
			parsing_stack.push("PROGRAM");

			int token_stream_cursor = 0;
			while(token_stream_cursor < sz(token_stream)){
				auto token_i = token_stream[token_stream_cursor];
				string parsing_stack_top = parsing_stack.top();
				// DEBUG:
				cout << token_i << " " << parsing_stack_top << endl;
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
					int transition_num = parsing_table[P_nonterm_term(parsing_stack_top, token_i)];
					if(transition_num == -1){
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

