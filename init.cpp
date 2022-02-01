#pragma once
#include "include.cpp"
#include "tools.cpp"
#include "global_values.hpp"


// 文法の遷移を3重vectorに写す
vector<Pbnf> bnfToList(){
	ifstream input_bnf = fileToIfstream("bnf");

	// この関数で返す値
	vector<Pbnf> local_token_trans_table;
	string now_line = "";
	while(getline(input_bnf, now_line)){
		// コメントはスルー
		if(sz(now_line) >= 2 && now_line[0] == '/' && now_line[1] == '/')continue;
		// 空行もスルー
		if(now_line == "")continue;

		//srcとdstを分ける
		string src = "";
		vector<string> dst;
		{
			int i = 0;
			// srcの単語を取得する
			src = getNextStr(now_line, i);
			// separator( ::= or := )を取得
			string separator = getNextStr(now_line, i);
			// ::=のとき
			if(separator == "::="){
				while(i < sz(now_line)){
					string word = getNextStr(now_line, i);
					dst.push_back(word);
				}
			}
			// :=のとき
			else if(separator == ":="){
				// 右辺は一つのtokenで終端記号
				string word = getNextStr(now_line, i);
				// 終端記号列
				terminal_symbols.insert(word);
			}
			// それ以外ならエラー
			else {
				cerr << "separatorが:=でも::=でもない" << endl;
				assert(0);
			}
			// TODO: bnf_src_to_dstとtoken_trans_tableどっちも共通した過程を踏むのにtoken_trans_tableのための関数になっているので調整する
			bnf_src_to_dst[src].push_back(dst);
			local_token_trans_table.push_back(Pbnf(src, dst));
		}
	}
	return local_token_trans_table;
}

// 変数の初期化
void init(){
	// tokenstr_to_tokenkindの初期化
	tokenstr_to_tokenkind["TERMINAL"] = TERMINAL;
	tokenstr_to_tokenkind["EOP"] = EOP;
	tokenstr_to_tokenkind["ID"] = ID;
	tokenstr_to_tokenkind["NUMBER"] = NUMBER;
	tokenstr_to_tokenkind["INT"] = INT;
	tokenstr_to_tokenkind["SEMICOLON"] = SEMICOLON;
	tokenstr_to_tokenkind["COMMA"] = COMMA;
	tokenstr_to_tokenkind["LPAREN"] = LPAREN;
	tokenstr_to_tokenkind["RPAREN"] = RPAREN;
	tokenstr_to_tokenkind["LBRACE"] = LBRACE;
	tokenstr_to_tokenkind["RBRACE"] = RBRACE;
	tokenstr_to_tokenkind["LBRACKET"] = LBRACKET;
	tokenstr_to_tokenkind["RBRACKET"] = RBRACKET;
	tokenstr_to_tokenkind["ASSIGN"] = ASSIGN;
	tokenstr_to_tokenkind["PROGRAM"] = PROGRAM;
	tokenstr_to_tokenkind["COMPOUND"] = COMPOUND;
	tokenstr_to_tokenkind["STATEMENT"] = STATEMENT;
	tokenstr_to_tokenkind["DECLARATION_STATEMENT"] = DECLARATION_STATEMENT;
	tokenstr_to_tokenkind["ASSIGN_STATEMENT"] = ASSIGN_STATEMENT;
	tokenstr_to_tokenkind["NUSED"] = NUSED;
	token_trans_table = bnfToList();
}