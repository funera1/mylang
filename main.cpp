#include <bits/stdc++.h>
#define sz(x) (x).size()
using namespace std;

// 定数
static int TOKEN_TRANS_TABLE_MAX_SIZE = 100;
// tokenの種類
typedef enum tokens {
	TERMINAL,EOP,ID,NUMBER,INT,SEMICOLON,COMMA,LPAREN,RPAREN,LBRACE,
	RBRACE,LBRACKET,RBRACKET,ASSIGN,PROGRAM,COMPOUND,STATEMENT,
    DECLARATION_STATEMENT,ASSIGN_STATEMENT,NUSED
} tokenkind;

// グローバル変数

// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[PROGRAM] = {{"LBRACKET", "COMPOUND", "RBRACKET"}}
vector<vector<vector<string>>> tokenTransTable(NUSED+1, vector<vector<string>>(TOKEN_TRANS_TABLE_MAX_SIZE));
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstrToTokenkind;
// 終端記号の集合
set<string> terminalSymbols;




// fileの文字列をそのまま返す
string fileToString(string inputFilePath){
	ifstream ifs(inputFilePath);
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

// 文法の遷移を3重vectorに写す
vector<vector<vector<string>>> bnfToList(){
	string filename("bnf");
	ifstream input_bnf(filename);
	// fileが開かなければエラー
	if(!input_bnf.is_open()){
		cerr << "Could not open the bnf file." << endl;
		return {};
	}

	// この関数で返す値
	vector<vector<vector<string>>> localTokenTransTable(NUSED+1, vector<vector<string>>(TOKEN_TRANS_TABLE_MAX_SIZE));
	string now_line = "";
	// 1行ずつ読み込んでいく
	while(getline(input_bnf, now_line)){
		// コメントはスルー
		if(sz(now_line) >= 2 && now_line[0] == '/' && now_line[1] == '/')continue;
		// 空行もスルー
		if(now_line == "")continue;

		//srcとdstを分ける
		string src = "";
		vector<vector<string>> dst;
		{
			int i = 0;
			// srcの単語を取得する
			src = getNextStr(now_line, i);
			// separator( ::= or := )を取得
			string separator = getNextStr(now_line, i);
			// ::=のとき
			if(separator == "::="){
				int cnt = 0;
				while(i < sz(now_line)){
					string word = getNextStr(now_line, i);
					if(word == "|")cnt++;
					else localTokenTransTable[tokenstrToTokenkind[src]][cnt].push_back(word);
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
		}
		// srcとdstをloalTokenstrToTokenkind
		// localTokenTransTable.resize(sz(dst));
		// localTokenTransTable[tokenstrToTokenkind[src]] = dst;
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

// 単語の一文字目でトークンを決め打ちする
int firstCheckToken(char c){
	if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))return ID;
	if('0' <= c && c <= '9')return NUMBER;
	if(c == ';')return SEMICOLON;
	if(c == ',')return COMMA;
	if(c == '(')return LPAREN;
	if(c == ')')return RPAREN;
	if(c == '[')return LBRACE;
	if(c == ']')return RBRACE;
	if(c == '{')return LBRACKET;
	if(c == '}')return RBRACKET;
	if(c == '=')return ASSIGN;
	if(c == ' ')return EOP;
	if(c == '\n')return EOP;
	if(c == '\t')return EOP;
	return NUSED;
}

// IDがtokenでないか確かめる
int checkIdIsToken(string s){
	if(s == "int")return INT;
	else return ID;
}

// 字句解析
// 入力記号列strをtoken毎に分割し、
void lexicalAnalysis(string str, vector<string>& input_stream, vector<int>& token_stream){
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

bool parsing(vector<string>& input_stream, vector<int>& token_stream){
    // token列を文法の通りに遷移させて、間違っていないかをチェックする。
    int i = 0; 
    while(i < sz(token_stream)){

    }
}

int main() {
	// 入力記号列を読み込む
	string str = fileToString("input.txt");
	// for(int i = 0; i < sz(str); i++)cout << i << " " << str[i] << endl;

	// 初期化
	init();
	// test
	auto PROGRAM_T = tokenTransTable[LBRACKET];
	for(int i = 0; i < sz(PROGRAM_T); i++){
		auto Pi = PROGRAM_T[i];
		for(int j = 0; j < sz(Pi); j++)cout << Pi[j] << " ";
		cout << endl;
	}

	// 字句解析
	vector<string> input_stream;
	vector<int> token_stream;
	lexicalAnalysis(str, input_stream, token_stream);

    // 構文解析
    // 正しい構文ならtrue, そうでないならfalseが返ってくる
    // if(!parsing(input_stream, token_stream)){
    //     cout << "ERROR: 構文が間違っています" << endl;
    //     assert(-1);
    // }
	//test
	// int n = sz(input_stream);
	// assert(n == sz(token_stream));
	// for(string i : input_stream)cout << i << " ";cout << endl;
	// for(int t : token_stream)cout << t << " ";cout << endl;
}