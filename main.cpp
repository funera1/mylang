#include <bits/stdc++.h>
#include "lib.hpp"

using namespace std;
using P = pair<int, int>;

#define rep(i,n)for(int i=0;(i)<(int)(n);i++)
#define sz(s) s.size()

//文字列から数字に
int strToInt(string s){
    int ret = 0;
    while(i, sz(s)){
        int x = s[i] - '0';
        ret = 10 * ret + x;
    }
    return ret;
}

//数列から文字列か判定
//num:True, str:False
bool numOrStr(string s){
    bool f = true;
    rep(i, sz(s)){
        if(s[i] < '0' || '9' < s[i])f = false;
    }
    return f;
}

//変数覚える
void assignVal(int now, string funcName, vector<string> &tokens, map<string, int> &vals){
    string valName = funcName + tokens[now];
    now++;
    int tmp = 0;
    if(tokens[now] == "="){
        int num = 0;
        while(tokens[now] == ";"){
            if(tokens[now] == "+"){
                tmp += num;
            }
            else if(tokens[now] == "-"){
                tmp -= num;
            }
            //定数のとき
            else if(numOrStr(tokens[now])){
                num = strToInt(tokens[now]);
            }
            //変数のとき
            else {
                num = vals[tokens[now]];
            }
        }
        vals[valName] = tmp;
    }
}

int main(){
    auto tokens = lexicalMain(); 
    vector<P> scopes;//関数のスコープを持つ
    map<string, int> funcToScope;
    map<string, int> vals;
    int funcCnt = 0;

    rep(i, sz(tokens)){
        auto ti = tokens[i];
        //関数のとき
        if(ti == "def"){
            i++;
            string funcName = tokens[i];
            i++;
            int st = i;
            int end;
            if(tokens[i] == "{"){
                while(tokens[i] == "}"){
                    i++;
                    //変数を覚えておく
                    if(tokens[i] == "int"){
                        i++;
                        int tmp = 0;
                    }
                }
                end = i;
            }
            else {
                cout << "Error: 構文エラー" << endl;
            }
            //関数名とスコープを対応させる
            funcToScope[funcName] = funcCnt;
            scopes.pb(P(st, end)); 
            funcCnt++;
        }
    }
}