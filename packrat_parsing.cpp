#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"

using ParsingTableInfo = pair<int, vector<string>>;
using ParsingTable = vector<map<string, ParsingTableInfo>>;
// Packrat parsing with left recursion
void parsing(vector<string> token_sream){
	int ts_size = sz(token_sream);
	ParsingTable dp(ts_size);
	for(int i = ts_size-1; i >= 0; i--){
		// dp[i]の初期化
		auto token_i = token_sream[i];
		dp[i][token_i] = ParsingTableInfo(i+1, vector<string>{token_i});

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
				dp[i][A] = ParsingTableInfo(nexti, vs_1+vs_2+...)とする
			*/
			for(auto [src, dst] : bnf_transition_list){
				int nexti = i;
				vector<string> base = {};
				bool breaked = false;
				for(auto di : dst){
					if(nexti >= ts_size){
						break;
					}
					// dp[nexti][di]が存在しないとき
					if(dp[nexti].count(di) == 0){
						breaked = true;
						break;
					}
					auto [tmpi, str_list] = dp[nexti][di];
					nexti = tmpi;
					// str_listを連結させる
					base = add_str_list(base, str_list);
					
				}
				// 最後までいけたら
				if(!breaked && dp[i][src] != ParsingTableInfo(nexti, base)){
					changed = true;
					dp[i][src] = ParsingTableInfo(nexti, base);
				}
			}
		}
	}
	// 構文解析が成功したかを判定
	for(auto dpi : dp[0]){
		dpi.second.second.push_back("$");
		if(token_sream == dpi.second.second){
			cout << "OK: complete parsing" << endl;
			return;
		}
	}
	cout << "NG: miss parsing" << endl;
	assert(0);
}