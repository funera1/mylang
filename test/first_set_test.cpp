#pragma once
#include "../include.cpp"
#include "../lexical.cpp"

int main() {
	// 入力記号列を読み込む
	string str = fileToString("input.txt");
	// for(int i = 0; i < sz(str); i++)cout << i << " " << str[i] << endl;

	// 初期化
	init();

	// 字句解析
	vector<string> input_stream;
	vector<int> token_stream;
	auto [input_stream, token_stream] = lexicalAnalysis(str);
    for(int i = 0; i < sz(input_stream); i++){
        cout << input_stream[i] << " " << token_stream[i] << endl;
    }
}