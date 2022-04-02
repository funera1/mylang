#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"

using ParsingTableInfo = pair<int, vector<string>>;
using ParsingTable = vector<map<string, ParsingTableInfo>>;
// Packrat parsing with left recursion
void parsing(string input_str){
	int input_str_size = sz(input_str);
	ParsingTable dp(input_str_size);
	for(int i = input_str_size-1; i >= 0; i--){
		// dp[i]の初期化
		// {}で囲んでるのはcharをstringにconvertするため
		string input_i = {input_str[i]};
		dp[i][input_i] = ParsingTableInfo(i+1, vector<string>{input_i});

		bool changed = true;
		map<string, int> last_update_transiton_priority;
		while(changed){
			changed = false;
			map<string, int> transition_priority;
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
				// last_update_transition_priorityは1始まりとする
				// あり得ない数字で初期化
				if(last_update_transiton_priority.count(src) == 0)last_update_transiton_priority[src] = 10000; 
				transition_priority[src]++;
				if(last_update_transiton_priority[src] < transition_priority[src])continue;
				
				bool seen_last_dst = false;
				int nexti = i;
				vector<string> base = {};
				for(int dst_cur = 0; dst_cur < sz(dst); dst_cur++){
					string di = dst[dst_cur];
					if(nexti >= input_str_size){
						break;
					}
					// dp[nexti][di]が存在しないとき
					if(dp[nexti].count(di) == 0){
						break;
					}
					auto [tmpi, str_list] = dp[nexti][di];
					nexti = tmpi;
					// str_listを連結させる
					base = add_str_list(base, str_list);
					if(dst_cur == sz(dst) - 1)seen_last_dst = true;
				}
				// 最後までいけたら
				if(seen_last_dst && dp[i][src] != ParsingTableInfo(nexti, base)){
					changed = true;
					// ---
					cout << src << endl;
					for(auto bi : base)cout << bi << " ";cout << endl;
					// ---
					dp[i][src] = ParsingTableInfo(nexti, base);
					last_update_transiton_priority[src] = transition_priority[src];
				}
			}
		}
	}
	// 構文解析が成功したかを判定
	vector<string> input_stream;
	for(char c : input_str)input_stream.push_back({c});
	for(auto dpi : dp[0]){
		// dpi.second.second.push_back("$");
		if(input_stream == dpi.second.second){
			cout << "OK: complete parsing" << endl;
			return;
		}
	}
	auto tmp = dp[0]["PROGRAM"].second;
	for(auto ti : tmp)cout << ti << " ";cout << endl;
	cout << "NG: miss parsing" << endl;
	assert(0);
}