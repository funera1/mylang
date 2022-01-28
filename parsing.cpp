#include "include.hpp"
#include "global_values.hpp"
#include "tools.cpp"

void createFirstSet(){
	// とりあえず一つの非終端記号についてだけやる->全ての非終端記号についてやる
	map<string, set<string>> firstSets;
	// 全ての非終端記号を見る
	for(auto [src, dst] : token_trans_table){
		// 既に見てたら飛ばす
		if(!firstSets[src].empty())continue;
		set<string> firstSet;
		// 最初は自分自身の非終端記号を持ってスタート
		firstSet.insert(src);
		while(!all_is_term_in_set(firstSet)){
			set<string> firstSetTmp;
			for(auto si : firstSet){
				for(auto dst_i : bnf_src_to_dst[si]){
					firstSetTmp.insert(dst_i[0]);
				}
			}
			firstSet = firstSetTmp;
		}
		firstSets[src] = firstSet;
	}
}