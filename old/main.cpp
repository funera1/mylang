#include <bits/stdc++.h>
using namespace std;

#define rep(i,n)for(int i=0;(i)<(int)(n);i++)
#define sz(s) s.size()

//仕様
//val = a or a * b;(*は四則演算)
//print(a);
//のいずれかになる
//変数名はa-z
//型はint型のみ

bool isval(char c){
    return('a' <= c && c <= 'z');
}

void print_(string s, map<char, int> &vals){
    int res = 0;
    for(int i = 0; i < sz(s); i++){
        if(s[i] == ';')break;
        if(s[i] == '('){
            //次が変数かどうか
            if(isval(s[i + 1])){
                res = vals[s[i + 1]];
                assert(s[i + 2] == ')');
            }
            else {
                i++;
                while(i < sz(s) && s[i] == ')'){
                    res = 10 * res + vals[s[i]];
                    i++;
                }
            }
        }
    }
    cout << res << endl;
}

bool isop(char c){
    return (c == '+' || c == '*' || c == '-' || c == '/');
}

bool isnum(char c){
    return ('0' <= c && c <= '9');
}

void substitution(string s, map<char, int> &vals){
    char c = s[0];
    // cout << s << " " << sz(s) << endl;
    bool f = false;
    for(int i = 0; i < sz(s); i++){
        if(s[i] == '='){
            f = true;
            //来るのが変数か数値かわからん
            int n[2];
            n[0] = 0;
            n[1] = 0;
            int j = 0;
            char op = '\0';
            while(1){
                if(s[i] == ';')break;
                if(s[i] == '\0')continue;
                if(isop(s[i])){
                    op = s[i];
                    j++;
                }
                else if(isval(s[i]))n[j] = vals[s[i]];
                else if(isnum(s[i])){
                    n[j] = 10 * n[j] + vals[s[i]];
                }
                i++;
            }
            int res = 0;
            if(op == '+')res = n[0] + n[1];
            if(op == '-')res = n[0] - n[1];
            if(op == '*')res = n[0] * n[1];
            if(op == '/'){
                if(n[1] == 0)cout << "ERROR! dont 0 divide.";
                else res = n[0] / n[1];
            }
            if(op == '\0')res = n[0];
            vals[c] = res;
        }
        if(f)break;
    }
}

int main(){
    string s;
    // getline(cin, s);        //c++は1行目読み込みうまくいかん
    map<char, int> vals;  //変数
    for(int i = 0; i < 10; i++)vals[(char)('0' + i)] = i; //数字の変数に数字を入れておく
    while(getline(cin, s)){ //1行ずつソースコードを読み込む
        if(s == "exit;")break;   //読み込み終わったら終了
        else if(s.substr(0, 5) == "print"){
            print_(s, vals);
        }
        //それ以外のときは
        else{
            substitution(s, vals);
        }
    }
}