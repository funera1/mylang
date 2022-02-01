#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"

map<string, set<string>> create_first_set(){
	// とりあえず一つの非終端記号についてだけやる->全ての非終端記号についてやる
	map<string, set<string>> first_sets;
	// 全ての非終端記号を見る
	for(auto [src, dst] : token_trans_table){
		// 既に見てたら飛ばす
		if(!first_sets[src].empty())continue;
		set<string> first_set;
		// 最初は自分自身の非終端記号を持ってスタート
		first_set.insert(src);
		while(!all_is_term_in_set(first_set)){
			set<string> first_setTmp;
			for(auto si : first_set){
				for(auto dst_i : bnf_src_to_dst[si]){
					first_setTmp.insert(dst_i[0]);
				}
			}
			first_set = first_setTmp;
		}
		first_sets[src] = first_set;
	}
	return first_sets;
}

// 途中
// 空文字を定義してないから作れなさそうだし、まだ必要ないかも
// void create_follow_set(map<string, set<string>> first_set){
// 	map<string, set<string>> follow_sets;
// 	for(auto [src, dst] : token_trans_table){
// 		if(!follow_sets[src].empty())continue;
// 		set<string> follow_set;
// 	}
// }

void create_ll_parsing_table(map<string, set<string>> first_set, map<string, set<string>> follow_set){
	map<pair<string, string>, string> ll_parsing_table;
	// すべての(non-term, term)の組み合わせについて表を作る
	// algorithm:
	// T[A, a]にA->wが入るのは以下の場合のみ
	// - aがfirst_set(w)に含まれる
	// - epsがfirst_set(w)に含まれ、aがfollow_set(A)に含まれる
	
	// TODO: non-termとtermをそれぞれ持つデータ集合がほしい
}
