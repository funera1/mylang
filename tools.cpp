#pragma once
#include "include.cpp"

// fileの文字列をそのまま返す
string fileToString(string input_file_path){
	ifstream ifs(input_file_path);
	// 読み込めなかったらエラー
	if (ifs.fail()) {
		cerr << "Failed to open file." << endl;
		return "";
	}
	string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	return str;
}

// 文字がアルファベットなら1, そうでなければ0を返す
bool charIsAlphabet(char c){
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

// 現在の位置から次の連続した文字列を取得する
string getNextStr(string baseStr, int& now_cursol){
	// 次の文字が来るまで空白とタブ文字と改行を捨てる
	for(; now_cursol < sz(baseStr); now_cursol++){
		if(baseStr[now_cursol] == ' ' || baseStr[now_cursol] == '\t' || baseStr[now_cursol] == '\n')continue;
		break;
	}
	// 連続した文字列を取得する
	string next_str = "";
	for(; now_cursol < sz(baseStr); now_cursol++){
		if(baseStr[now_cursol] == ' ' || baseStr[now_cursol] == '\t' || baseStr[now_cursol] == '\n')break;
		next_str.push_back(baseStr[now_cursol]);
	}
	return next_str;
}


// 単語の一文字目でトークンを決め打ちする
string firstCheckToken(char c){
	if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))return "ID";
	if('0' <= c && c <= '9')return "NUMBER";
	if(c == ';')return "SEMICOLON";
	if(c == ',')return "COMMA";
	if(c == '(')return "LPAREN";
	if(c == ')')return "RPAREN";
	if(c == '[')return "LBRACE";
	if(c == ']')return "RBRACE";
	if(c == '{')return "LBRACKET";
	if(c == '}')return "RBRACKET";
	if(c == '=')return "ASSIGN";
	if(c == ' ')return "EOP";
	if(c == '\n')return "EOP";
	if(c == '\t')return "EOP";
	return "NUSED";
}

// IDがtokenでないか確かめる
string checkIdIsToken(string s){
	if(s == "int")return "INT";
	else return "ID";
}

// 終端記号かチェック
bool is_term(string s){
	if(sz(s) == 0)return false;
	// 非終端記号は全て大文字、終端記号は全て小文字である
	if('A' <= s[0] && s[0] <= 'Z')return false;
	else return true;
}

bool all_is_term_in_set(set<string> st){
	bool all_is_term = true;
	for(auto si : st){
		if(!is_term(si))all_is_term = false;
	}
	return all_is_term;
}

ifstream fileToIfstream(string file_path){
	string filename(file_path);
	ifstream input_file(filename);
	// fileが開かなければエラー
	if(!input_file.is_open()){
		cout << "Could not open this file." << endl;
		assert(0);
	}
	return input_file;
}

void get_nonterm_and_term_list(){
	map<string, bool> seen;
	auto add_list = [&](string s){
		if(is_term(s)) term_list.push_back(s);
		else nonterm_list.push_back(s);
		seen[s] = true;
	};
	for(auto [src, dst] : token_trans_table){
		if(seen[src])continue;
		add_list(src);
		for(auto dst_i : dst){
			if(seen[dst_i])continue;
			add_list(dst_i);
		}
	}
}

bool is_in_set(set<string> st, string item){
	return (bool)(st.find(item) != st.end());
}