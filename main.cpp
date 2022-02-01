#include "include.cpp"
#include "init.cpp"
#include "tools.cpp"


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
	auto [input_stream, token_stream] = lexicalAnalysis(str);

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