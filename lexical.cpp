#include <bits/stdc++.h>
using namespace std;

#define rep(i,n)for(int i=0;(i)<(int)(n);i++)
#define sz(s) s.size()
#define pb push_back


//与えられた文字列をtoken列にして返す
vector<string> lexical(string s, set<string> keywords, set<char> separators){
	string token = "";
	vector<string> tokens;
	rep(i, sz(s)){
		//s[i]が区切り子の場合
		if(separators.count(s[i])){
			//tokenが空文字列でなければtokensに追加
			if(token != "")tokens.pb(token);
			token = "";
			if(s[i] == ' ' || s[i] == '\0')continue;
			string tmp = "";
			tmp.pb(s[i]);
			tokens.pb(tmp);
		}
		else {
			token.pb(s[i]);
		}
	}
	return tokens;
}

//tokenに対応する属性を返す


vector<string> lexicalMain(){
	//キーワードのsetを持つ
	set<string> keywords;
	{
		keywords.insert("print");
		keywords.insert("for");
		keywords.insert("if");
		keywords.insert("else");
		keywords.insert("assign");
		keywords.insert("call");
	}
	//区切り子のsetを持つ
	set<char> separators;
	{
		separators.insert(')');
		separators.insert('(');
		separators.insert('{');
		separators.insert('}');
		separators.insert('[');
		separators.insert(']');
		separators.insert(',');
		separators.insert(';');
		separators.insert('+');
		separators.insert('-');
		separators.insert('=');
		separators.insert('\0');
		separators.insert(' ');
	}
	string s;
	vector<string> tokens;
	//ソースコードをtoken列にする
	while(getline(cin, s)){
		auto lis = lexical(s, keywords, separators);
		for(auto li : lis)tokens.pb(li);
	}
	return tokens;
}