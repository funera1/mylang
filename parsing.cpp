#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"


class Parsing {
	private:
		map<string, set<string>> first_sets;
		map<string, set<string>> follow_sets;

		void create_first_set(){
			// とりあえず一つの非終端記号についてだけやる->全ての非終端記号についてやる
			// 全ての非終端記号を見る
			for(auto [src, dst] : token_trans_table){
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
				}
				first_sets[src] = first_set;
			}
		}

		void create_follow_set(){
			for(auto non_term : nonterm_list){
				set<string> follow_set;
				// 初期化
				follow_set.insert(non_term);
				while(!all_is_term_in_set(follow_set)){
					set<string> follow_set_tmp;
					for(auto [src, dst] : token_trans_table){
						for(int i = 0; i < sz(token_trans_table); i++){
							if(non_term != dst[i])continue;
							// 次の要素がないとき
							if(i + 1 >= sz(token_trans_table)){
								follow_set_tmp.insert(src);
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
									for(auto fs : first_sets[follow_str]){
										follow_set_tmp.insert(fs);
									}
								}
							}
						}
					}
					follow_set = follow_set_tmp;
				}
				follow_sets[non_term] = follow_set;
			}
		}
	public:
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
		map<pair<string, string>, int> create_ll_parsing_table(){
			// Pstr(non_term, term)
			using Pstr = pair<string, string>;
			map<Pstr, int> ll_parsing_table;
			// すべての(non-term, term)の組み合わせについて表を作る
			// algorithm:
			// T[A, a]にA->wが入るのは以下の場合のみ
			// - aがfirst_set(w)に含まれる
			// - epsがfirst_set(w)に含まれ、aがfollow_set(A)に含まれる
			for(int i = 0; i < sz(token_trans_table); i++){
				auto [src, dst] = token_trans_table[i];
				auto first_set = get_first_set(dst[0]);
				// 空文字がfirst_set(w)に含まれる場合
				if(is_in_set(first_set, "#eps")){
					auto follow_set = get_follow_set(dst[0]);
					for(auto follow_i : follow_set){
						ll_parsing_table[Pstr(src, follow_i)] = i;
					}
				}
				// 空文字がfirst_set(w)に含まれない場合
				else {
					for(auto first_i : first_set){
						ll_parsing_table[Pstr(src, first_i)] = i;
					}
				}
			}
			return ll_parsing_table;
		}
};