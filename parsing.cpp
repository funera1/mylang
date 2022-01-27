#include "include.hpp"
#include "global_values.hpp"
#include "tools.cpp"

void createFirstSet(){
	// とりあえず一つの非終端記号についてだけやる
	set<string> firstSet;
	while(1){
		set<string> firstSetTmp;
		for(auto si : firstSet){
			for(auto dst_i : bnf_src_to_dst[si]){
				firstSetTmp.insert(dst_i[0]);
			}
		}
		firstSet = firstSetTmp;
		// すべてのfirstSetの要素について終端記号かチェックする
		bool f = true;
		for(auto fs : firstSet){
			if(!is_term(fs))f = false;
		}
		if(f)break;
	}
}