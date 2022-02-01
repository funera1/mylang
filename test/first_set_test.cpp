#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../lexical.cpp"
#include "../parsing.cpp"

int main() {
	// 入力記号列を読み込む
	string str = fileToString(home_dir+"test/input.txt");
	// for(int i = 0; i < sz(str); i++)cout << i << " " << str[i] << endl;

	// 初期化
	init();
	// 字句解析
	vector<string> input_stream;
	vector<string> token_stream;
	auto tmp = lexicalAnalysis(str);
	input_stream = tmp.first;
	token_stream = tmp.second;
    // for(int i = 0; i < sz(input_stream); i++){
    //     cout << input_stream[i] << " " << token_stream[i] << endl;
    // }
	map<string, set<string>> first_set = create_first_set();	
	for(auto a : first_set){
		cout << a.first << ": ";
		for(auto se : a.second){
			cout << se << ", ";
		}
		cout << endl;
	}
}