#pragma once
#include "include.cpp"
#include "init.cpp"

// 字句解析
// 入力記号列strをtoken毎に分割し、
pair<vector<string>, vector<int>> lexicalAnalysis(string str){
    vector<string> input_stream;
    vector<int> token_stream;
	int i = 0;
	while(i < sz(str)){
		char c = str[i];
		cout << i << " " << c << endl;
		// スペース、タブ、改行は捨てる
		while(c == '\n' || c == '\t' || c == ' '){
			i++;
			c = str[i];
		}
		string now_word = "";
		now_word.push_back(c);
		int now_token = firstCheckToken(c);
		// 間違った記号を使っている時はエラー
		if(now_token == NUSED) {
			cerr << "ERROR: You may use wrong signs." << endl;
			assert(0);
		}
		// 一文字目で決め打ちしたtokenを先を見ることで確定させる
		switch (now_token){
			// 識別子の時. 先頭がアルファベットでそれ以降は数字かアルファベットの繰り返し
			case ID:
				while(i + 1 < sz(str) && (firstCheckToken(str[i + 1]) == ID || firstCheckToken(str[i + 1]) == NUMBER)){
					i++;
					now_word.push_back(str[i]);
				}
				break;
			// 数字の時. 数字が続く限り繰り返す
			case NUMBER:
				while(i + 1 < sz(str) && firstCheckToken(str[i + 1]) == NUMBER){
					i++;
					now_word.push_back(str[i]);
				}
				break;
			default:
				break;
		}
		// idがtokenでないか確かめる
		if(now_token == ID)now_token = checkIdIsToken(now_word);
		input_stream.push_back(now_word);
		token_stream.push_back(now_token);
		i++;
	}
}