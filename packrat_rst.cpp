#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"


// parを親とするtreeを構築する
// ret par [[ tree ]]
vector<string> make_tree(string par, vector<string> tree){
	vector<string> ret;
	ret.push_back(par);
	ret.push_back("[[");
	ret = add_str_list(ret, tree);
	ret.push_back("]]");
	return ret;
}

// RstTableInfo(packrat_parserアルゴリズムで次見始める位置, 木を逆ポーランド記法で表したもの)
// ここでのvector<string>は基本的にstackとして扱う
using RstTableInfo = pair<int, vector<string>>;
using RstTable = vector<map<string, RstTableInfo>>;

// Packrat parsing with left recursion
vector<string> get_rst(vector<string> token_sream){
	int ts_size = sz(token_sream);
	RstTable dp(ts_size);
	for(int i = ts_size-1; i >= 0; i--){
		// dp[i]の初期化
		auto token_i = token_sream[i];
		dp[i][token_i] = RstTableInfo(i+1, vector<string>{token_i});

		bool changed = true;
		while(changed){
			changed = false;
			// bnf_listの走査. 
			/* 
				変換A->abcについて
				auto [nexti, vs_1] = dp[i][a]
				auto [nexti, vs_2] = dp[nexti][b]
				auto [nexti, vs_3] = dp[nexti][c]
				がそれぞれ存在するなら
				dp[i][A] = RstTableInfo(nexti, vs_1+vs_2+...)とする
			*/
			for(auto [src, dst] : bnf_transition_list){
				bool seen_last_dst = false;
				int nexti = i;
				vector<string> base_tree = {};
				for(int dst_cur = 0; dst_cur < sz(dst); dst_cur++){
					string di = dst[dst_cur];
					// nextiがtoken_steramのサイズを超えたらbreak
					if(nexti >= ts_size){
						break;
					}
					// dp[nexti][di]が存在しないとき
					if(dp[nexti].count(di) == 0){
						break;
					}
					auto [tmpi, now_tree] = dp[nexti][di];
					nexti = tmpi;
					// nonterm_node_listを連結させる
					base_tree = add_str_list(base_tree, now_tree);
					if(dst_cur == sz(dst) - 1){
						// base = src [[ base ]]の形にする
						base_tree = make_tree(src, base_tree);
						seen_last_dst = true;
					}
				}
				// 最後までいけたら
				if(seen_last_dst && dp[i][src] != RstTableInfo(nexti, base_tree)){
					for(auto bi : base_tree)cout << bi << " ";cout << endl;
					changed = true;
					dp[i][src] = RstTableInfo(nexti, base_tree);
				}
			}
		}
	}
	return dp[0]["PROGRAM"].second;
}