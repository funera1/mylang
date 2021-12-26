#include <bits/stdc++.h>
#define sz(x) (x).size()
using namespace std;

// グローバル変数

// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
vector<vector<int>> tokenToMovementList(NUSED+1);
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstringToTokenkind;



// tokenの種類
typedef enum tokens {
	EOP,ID,NUMBER,INT,SEMICOLON,COMMA,LPAREN,RPAREN,LBRACE,
	RBRACE,LBRACKET,RBRACKET,ASSIGN,PROGRAM,COMPOUND,STATEMENT,
    DECLARATION_STATEMENT,ASSIGN_STATEMENT,NUSED
} tokenkind;

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

// 文法の遷移を2重vectorに写す
vector<vector<int>> bnfToList(){
	string filename("bnf");
	ifstream input_bnf(filename);
	// fileが開かなければエラー
	if(!input_bnf.is_open()){
		cerr << "Could not open the bnf file." << endl;
		return {{}};
	}

	string now_line = "";
	// 1行ずつ読み込んでいく
	while(getline(input_bnf, now_line)){
		// コメントはスルー
		if(now_line[0] == '/' && now_line[1] == '/')continue;
		string src = "", dst = "";
	}
}

// 変数の初期化
void init(){
	// tokenstringToTokenkindの初期化
	tokenstringToTokenkind["EOP"] = EOP;
	tokenstringToTokenkind["ID"] = ID;
	tokenstringToTokenkind["NUMBER"] = NUMBER;
	tokenstringToTokenkind["INT"] = INT;
	tokenstringToTokenkind["SEMICOLON"] = SEMICOLON;
	tokenstringToTokenkind["COMMA"] = COMMA;
	tokenstringToTokenkind["LPAREN"] = LPAREN;
	tokenstringToTokenkind["RPAREN"] = RPAREN;
	tokenstringToTokenkind["LBRACE"] = LBRACE;
	tokenstringToTokenkind["RBRACE"] = RBRACE;
	tokenstringToTokenkind["LBRACKET"] = LBRACKET;
	tokenstringToTokenkind["RBRACKET"] = RBRACKET;
	tokenstringToTokenkind["ASSIGN"] = ASSIGN;
	tokenstringToTokenkind["PROGRAM"] = PROGRAM;
	tokenstringToTokenkind["COMPOUND"] = COMPOUND;
	tokenstringToTokenkind["STATEMENT"] = STATEMENT;
	tokenstringToTokenkind["DECLARATION_STATEMENT"] = DECLARATION_STATEMENT;
	tokenstringToTokenkind["ASSIGN_STATEMENT"] = ASSIGN_STATEMENT;
	tokenstringToTokenkind["NUSED"] = NUSED;
	bnfToList();
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

	// 字句解析
	vector<string> input_stream;
	vector<int> token_stream;
	lexicalAnalysis(str, input_stream, token_stream);

    // 構文解析
    // 正しい構文ならtrue, そうでないならfalseが返ってくる
    if(!parsing(input_stream, token_stream)){
        cout << "ERROR: 構文が間違っています" << endl;
        assert(-1);
    }
	//test
	// int n = sz(input_stream);
	// assert(n == sz(token_stream));
	// for(string i : input_stream)cout << i << " ";cout << endl;
	// for(int t : token_stream)cout << t << " ";cout << endl;
}