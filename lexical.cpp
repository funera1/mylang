#pragma once
#include "include.cpp"
#include "init.cpp"

// 字句解析
// 入力記号列strをtoken毎に分割し、
pair<vector<string>, vector<string>> lexicalAnalysis(string str){
    vector<string> input_stream;
    vector<string> token_stream;
	int i = 0;
	while(i < sz(str)){
		char c = str[i];
		// スペース、タブ、改行は捨てる
		while(c == '\n' || c == '\t' || c == ' '){
			i++;
			c = str[i];
		}
		string now_word = "";
		now_word.push_back(c);
		string now_token = firstCheckToken(c);
		// 間違った記号を使っている時はエラー
		if(now_token == "NUSED") {
			cout << "ERROR: You may use wrong signs." << endl;
			assert(0);
		}
		// 一文字目で決め打ちしたtokenを先を見ることで確定させる
		// 識別子の時. 先頭がアルファベットでそれ以降は数字かアルファベットの繰り返し
		if(now_token == "ID"){
			while(i + 1 < sz(str) && (firstCheckToken(str[i + 1]) == "ID" || firstCheckToken(str[i + 1]) == "NUMBER")){
				i++;
				now_word.push_back(str[i]);
			}
		}
		// 	// 数字の時. 数字が続く限り繰り返す
		else if(now_token == "NUMBER"){
			while(i + 1 < sz(str) && firstCheckToken(str[i + 1]) == "NUMBER"){
				i++;
				now_word.push_back(str[i]);
			}
		}
		if(now_token == "ID")now_token = checkIdIsToken(now_word);
		input_stream.push_back(now_word);
		if(now_token == "ID")token_stream.push_back("#id");
		else if(now_token == "NUM")token_stream.push_back("#num");
		else token_stream.push_back(now_word);
		i++;
	}
	return make_pair(input_stream, token_stream);
}