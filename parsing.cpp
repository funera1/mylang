#include <bits/stdc++.h>
#define sz(x) (x).size()
using namespace std;

void createFirstSet(){
	// とりあえず一つの非終端記号についてだけやる
	set<string> firstSet;
	while(1){
		set<string> firstSetTmp;
		for(auto si : firstSet){
			// TODO: m(名前未定):map<string, dst>, srcからdstに行けるやつを作る
			for(auto mi : m[si]){
				firstSetTmp.insert(mi);
			}
		}
		firstSet = firstSetTmp;
		// すべてのfirstSetの要素について終端記号かチェックする
		bool f = true;
		for(auto fs : firstSet){
			// TODO: is_term(s): sが終端記号かどうかを判定する関数を作る
			// (終端記号->1, 非終端記号->0)
			if(!is_term(fs))f = false;
		}
		if(f)break;
	}
}