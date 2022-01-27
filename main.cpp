#include "include.hpp"
#include "init.cpp"
#include "tools.cpp"

// グローバル変数

// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = Pbnf("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
vector<Pbnf> tokenTransTable;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstrToTokenkind;
// 終端記号の集合
set<string> terminalSymbols;




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

// LL(1)法の準備
void createFirstSet(){

}
bool parsing(vector<string>& input_stream, vector<int>& token_stream){
    // token列を文法の通りに遷移させて、間違っていないかをチェックする。
    int i = 0; 
	createFirstSet();
}

int main() {
	// 入力記号列を読み込む
	string str = fileToString("input.txt");
	// for(int i = 0; i < sz(str); i++)cout << i << " " << str[i] << endl;

	// 初期化
	init();

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