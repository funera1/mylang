#include "include.hpp"

// (次見る位置, 今まで)
using ParseTableInfo = pair<int, vector<string>>;
using ParseTable = vector<map<string, ParseTableInfo>>;

vector<int> get_accept_states(vector<string> dst){
	int ret = sz(dst); 
	return vector<int>{ret};
}

bool update_dptable(string input_str, int trans_i, int input_start_i, ParseTable& dp,
					vector<int>& memo_input_continue_i, vector<vector<int>>& memo_seen_state){
	auto [src, dst] = bnf_transition_list[trans_i];
	// cout << src << endl;
	// srcが終端記号のとき
	// dp[input_start_i][input] = ParsingTableInfo(input_start_i+1, vector<string>{src});
	int state_quantity = sz(dst) + 1;
	// 受理状態のリスト
	// とりあえずは(ループしかないうちは)dstの最後のみが受理状態
	vector<int> accept_states = get_accept_states(dst);
	// 続きから始める
	int input_continue_i = memo_input_continue_i[trans_i];
	vector<int> seen_state = memo_seen_state[trans_i];

	// ここぽいな
	vector<string> base = (dp[input_start_i].count(src) ? dp[input_start_i][src].second : vector<string>{});
	// dstから状態遷移図の構築をする
	// 構造はmap<pair<int, string>, vector<int>> 
	vector<vector<string>> state_graph = dfa_graphs[trans_i];
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
				// 範囲外参照check
				assert(state_i < sz(state_graph) && state_j < sz(state_graph[state_i]));
				if(state_graph[state_i][state_j] != ""){
					string now_token = state_graph[state_i][state_j];
					assert(nexti < sz(dp));
					
					// nextiにtokenがあるなら
					if(dp[nexti].count(now_token) == 1){
						// ---
						cout << "FUGAAAAAAA" << endl;
						cout << src << " " << nexti << " " << now_token << endl;
						auto [hoge, fuga] = dp[nexti][now_token];
						// ---
						new_seen_state[state_j] = 1;
						base = add_str_list(base, dp[nexti][now_token].second);
						for(auto bi : base)cout << bi << " ";cout << endl;
						nexti = dp[nexti][now_token].first;
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
				if(1){
					cout << "[update dptable]" << endl;
					cout << "accept_state: ";
					for(auto ai : accept_states)cout << ai << " ";cout << endl;
					cout << input_start_i << endl;
					cout << "src: " << src << endl;
					// cout << "dst: ";
					cout << "state: " << endl;
					for(auto si : new_seen_state)cout << si << " ";cout << endl;
					for(auto bi : base)cout << bi << " ";cout << endl;
					cout << "\\[update dptable]" << endl << endl;
				}
				// ---
				dp[input_start_i][src] = ParseTableInfo(nexti, base);
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
void parse(string input_str){
	int input_str_size = sz(input_str);
	ParseTable dp(input_str_size);
	for(int i = input_str_size-1; i >= 0; i--){
		// dp[i]の初期化
		// {}で囲んでるのはcharをstringにconvertするため
		string input_i = {input_str[i]};

		// dp[i]初期状態
		cout << "START: " << i << ", " << input_i << endl;
		dp[i][input_i] = ParseTableInfo(i+1, vector<string>{input_i});

		// なんでvectorで持ってる?
		vector<int> memo_input_continue_i(sz(bnf_transition_list), i);
		// dstの最大サイズを100とする
		const int DST_MAX_SIZE = 100;
		// 初期化: memo_seen_state[][0]=1
		vector<vector<int>> memo_seen_state(sz(bnf_transition_list), vector<int>(DST_MAX_SIZE, 0));
		for(int bi = 0; bi < sz(bnf_transition_list); bi++)memo_seen_state[bi][0] = 1;

		bool changed = true;
		map<string, int> last_update_transiton_priority;
		int cnt = 0; 
        // 変更がある限りループする
		while(changed){
			cnt++;
			cout << cnt << endl;
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
            // 変換表のi番目の変換について決め打ちする
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
				// ---
				// debug: 2週目以降の(i, trans_i)についてupdate_dptableに渡す引数が知りたい
				// ---
				// if(src == "2_dig"){
				// 	cout << "########################" << endl;
				// 	cout << "input_continue_i: " << memo_input_continue_i[trans_i] << endl;
				// 	cout << "seen_state: ";
				// 	for(auto s : memo_seen_state[trans_i])cout << s << " ";
				// 	cout << endl;
				// 	cout << "########################" << endl << endl;
				// }
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
			cout << "OK: complete to parse" << endl;
			return;
		}
	}
	auto tmp = dp[0]["PROGRAM"].second;
	for(auto ti : tmp)cout << ti << " ";cout << endl;
	cout << "NG: failed to parse" << endl;
	assert(0);
}

