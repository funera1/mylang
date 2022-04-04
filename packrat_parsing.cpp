#pragma once
#include "include.cpp"
#include "global_values.hpp"
#include "tools.cpp"

using ParsingTableInfo = pair<int, vector<string>>;
using ParsingTable = vector<map<string, ParsingTableInfo>>;

vector<vector<int>> get_dfa(vector<string> dst){
	// {}は繰り返し
	// の決定性オートマトンを作成
	// dst[i]とdst[j]に辺があればdfa[i][j]=1, else 0となる
	int dst_size = sz(dst);
	vector<vector<int>> dfa(dst_size, vector<int>(dst_size, 0));
}

// {}: ループ
// state_graph[i][j] = (i-jに辺がある ? dst[j] : "")
// 状態0としてダミーノードを設けるので1個ずれる
vector<vector<string>> get_state_graph_by_ebnf(vector<string> dst){
	int state_quantity = sz(dst) + 1;
	// state_graph[i][j]: (i, j)に辺があれば1, else 0
	vector<vector<string>> state_graph(state_quantity, vector<string>(state_quantity, ""));
	stack<int> loop_stack;
	for(int i = 0; i < sz(dst); i++){
		state_graph[i][i+1] = dst[i];
		// ループ始まり
		if(dst[i] == "{"){
			loop_stack.push(i);
		}
		if(dst[i] == "}"){
			int top = loop_stack.top();
			// ループになっているのは{}の内側なので、i+1, i-1する
			// TODO: ↑だめ, }}になってるときバグる
			// TODO: {{もバグる
			state_graph[i-1][top+1] = dst[top+1];
			loop_stack.pop();
		}
	}
	return state_graph;
}


bool update_dptable(string input_str, int trans_i, int input_start_i, ParsingTable& dp,
					vector<int>& memo_input_continue_i, vector<vector<int>>& memo_seen_state){
	auto [src, dst] = bnf_transition_list[trans_i];
	int state_quantity = sz(dst) + 1;
	// 受理状態のリスト
	// とりあえずは(ループしかないうちは)dstの最後のみが受理状態
	if(sz(dst) == 0)return false;
	vector<int> accept_states = {state_quantity-1};
	// 続きから始める
	int input_continue_i = memo_input_continue_i[trans_i];
	vector<int> seen_state = memo_seen_state[trans_i];

	vector<string> base;
	// dstから状態遷移図の構築をする
	// 構造はmap<pair<int, string>, vector<int>> 
	vector<vector<string>> state_graph = get_state_graph_by_ebnf(dst);
	vector<int> new_seen_state(state_quantity, 0);
	// input_continue_iより後ろのtokenについて結合できなるなら最小の結合をしたい
	// ここでの結合とはA->a,b,cという変換があったときinput_continue_iの後ろでa,b,cが得られた時dp[i][A]のテーブルを更新すること
	for(int nexti = input_continue_i; nexti < sz(input_str);){
		int oldi = nexti;
		// 状態iを決め打ち
		for(int state_i = 0; state_i < state_quantity; state_i++){
			if(seen_state[state_i] == 0)continue;
			// state_i, state_jについて辺があるかを調べたい
			for(int state_j = 0; state_j < state_quantity; state_j++){
				// 辺があるとき
				assert(state_i < sz(state_graph) && state_j < sz(state_graph[state_i]));
				if(state_graph[state_i][state_j] != ""){
					string token = state_graph[state_i][state_j];
					assert(nexti < sz(dp));
					// nextiにtokenがあるなら
					if(dp[nexti].count(token) == 1){
						new_seen_state[state_j] = 1;
						base = add_str_list(base, dp[nexti][token].second);
						// forで++されるから1引く
						nexti = dp[nexti][token].first;
					}
				}
			}
			// 更新されなかったらbreak;
		}
		// 受理状態に到達したか確認
		for(int accept_state : accept_states){
			// 受理状態に到達したのでdpを更新
			assert(0 <= accept_state && accept_state < sz(new_seen_state));
			if(new_seen_state[accept_state]){
				// ---
				cout << "src: " << src << endl;
				cout << "dst: ";
				for(auto bi : base)cout << bi << " ";cout << endl;
				// ---
				dp[input_start_i][src] = ParsingTableInfo(nexti, base);
				memo_input_continue_i[trans_i] = nexti;
				memo_seen_state[trans_i] = new_seen_state;
				return true;
			}
		}
		seen_state = new_seen_state;
		if(oldi == nexti)break;
	}
	return false;
}
// Packrat parsing with left recursion
void parsing(string input_str){
	int input_str_size = sz(input_str);
	ParsingTable dp(input_str_size);
	for(int i = input_str_size-1; i >= 0; i--){
		// dp[i]の初期化
		// {}で囲んでるのはcharをstringにconvertするため
		string input_i = {input_str[i]};
		// debug
		// cout << input_i << endl;
		dp[i][input_i] = ParsingTableInfo(i+1, vector<string>{input_i});

		vector<int> memo_input_continue_i(sz(bnf_transition_list), i);
		// dstの最大サイズを100とする
		const int DST_MAX_SIZE = 100;
		// 初期化: memo_seen_state[][0]=1
		vector<vector<int>> memo_seen_state(sz(bnf_transition_list), vector<int>(DST_MAX_SIZE, 0));
		for(int bi = 0; bi < sz(bnf_transition_list); bi++)memo_seen_state[bi][0] = 1;

		bool changed = true;
		map<string, int> last_update_transiton_priority;
		int cnt = 0; 
		while(changed){
			cnt++;
			// cout << cnt << endl;
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
			for(int trans_i = 0; trans_i < sz(bnf_transition_list); trans_i++){
				auto [src, dst] = bnf_transition_list[trans_i];
				// last_update_transition_priorityは1始まりとする
				// あり得ない数字で初期化
				if(last_update_transiton_priority.count(src) == 0)last_update_transiton_priority[src] = INF; 
				transition_priority[src]++;
				if(last_update_transiton_priority[src] < transition_priority[src])continue;
				
				// dstを頭から見ていって一致しているかを確かめる
				// これを関数化する
				// (変換の番号, &dptable)
				bool ret_update_dptable = update_dptable(input_str, trans_i, i, dp, memo_input_continue_i, memo_seen_state);
				// dptableが更新されたならlast_update_transition_priorityも更新する
				if(ret_update_dptable){
					last_update_transiton_priority[src] = transition_priority[src];
				}
				changed |= ret_update_dptable;
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

