#include "include.hpp"
#include "tools.cpp"


// グローバル変数

// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = Pbnf("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
vector<Pbnf> tokenTransTable;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstrToTokenkind;
// 終端記号の集合
set<string> terminalSymbols;


// 文法の遷移を3重vectorに写す
vector<Pbnf> bnfToList(){
	string filename("bnf");
	ifstream input_bnf(filename);
	// fileが開かなければエラー
	if(!input_bnf.is_open()){
		cerr << "Could not open the bnf file." << endl;
		return {};
	}

	// この関数で返す値
	vector<Pbnf> localTokenTransTable;
	string now_line = "";
	// 1行ずつ読み込んでいく
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
				terminalSymbols.insert(word);
			}
			// それ以外ならエラー
			else {
				cerr << "separatorが:=でも::=でもない" << endl;
				assert(0);
			}
			localTokenTransTable.push_back(Pbnf(src, dst));
		}
	}
	return localTokenTransTable;
}

// 変数の初期化
void init(){
	// tokenstrToTokenkindの初期化
	tokenstrToTokenkind["TERMINAL"] = TERMINAL;
	tokenstrToTokenkind["EOP"] = EOP;
	tokenstrToTokenkind["ID"] = ID;
	tokenstrToTokenkind["NUMBER"] = NUMBER;
	tokenstrToTokenkind["INT"] = INT;
	tokenstrToTokenkind["SEMICOLON"] = SEMICOLON;
	tokenstrToTokenkind["COMMA"] = COMMA;
	tokenstrToTokenkind["LPAREN"] = LPAREN;
	tokenstrToTokenkind["RPAREN"] = RPAREN;
	tokenstrToTokenkind["LBRACE"] = LBRACE;
	tokenstrToTokenkind["RBRACE"] = RBRACE;
	tokenstrToTokenkind["LBRACKET"] = LBRACKET;
	tokenstrToTokenkind["RBRACKET"] = RBRACKET;
	tokenstrToTokenkind["ASSIGN"] = ASSIGN;
	tokenstrToTokenkind["PROGRAM"] = PROGRAM;
	tokenstrToTokenkind["COMPOUND"] = COMPOUND;
	tokenstrToTokenkind["STATEMENT"] = STATEMENT;
	tokenstrToTokenkind["DECLARATION_STATEMENT"] = DECLARATION_STATEMENT;
	tokenstrToTokenkind["ASSIGN_STATEMENT"] = ASSIGN_STATEMENT;
	tokenstrToTokenkind["NUSED"] = NUSED;
	tokenTransTable = bnfToList();
}