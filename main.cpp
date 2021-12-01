#include <bits/stdc++.h>
#include "lib.hpp"

using namespace std;
using P = pair<int, int>;

#define rep(i,n)for(int i=0;(i)<(int)(n);i++)
#define sz(s) s.size()

//values 
vector<string> tokens;
map<string, P> funcToScope;//関数のスコープ
map<string, vector<string>> args;//関数の引数
map<string, int> vals;//変数

//文字列から数字に
int strToInt(string s){
    int ret = 0;
    rep(i, sz(s)){
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
void assignVal(int now, string funcName){
    now++;//int
    string valName = funcName + "#" + tokens[now];
    now++;
    bool f = true;
    int tmp = 0;
    if(tokens[now] == "="){
        int num = 0;
        int op = 1;
        while(tokens[now] != ";"){
            if(tokens[now] == "+"){
                op = 1;
            }
            else if(tokens[now] == "-"){
                op = -1;
            }
            //定数のとき
            else if(numOrStr(tokens[now])){
                num = strToInt(tokens[now]);
                tmp += num * op;
            }
            //変数のとき
            else {
                string valName_ = funcName + "#" + tokens[now];
                num = vals[valName_];
                tmp += num * op;
            }
            now++;
        }
        vals[valName] = tmp;
    }
}


//printする
void printFunc(int &i, string funcName){
    cout << "Print" << ":";
    i++;//print
    i++;//'('
    //式をprintできない
    if(numOrStr(tokens[i])){
        cout << tokens[i] << endl;
    }
    else {
        cout << vals[funcName + "#" + tokens[i]] << endl;
    }
    i++;//')'
    i++;//';'
}
void callFunc(int &i, string fromFuncName);
void query(int &i, string funcName){
    //代入
    if(tokens[i] == "int"){
        assignVal(i, funcName);
    }
    //print
    else if(tokens[i] == "print"){
        printFunc(i, funcName);
    }
    //関数呼び出し
    else if(tokens[i] == "call"){
        callFunc(i, funcName);
    }
    else {
        cout << "Error: 構文エラー" << endl;
        i++;
        return;
    }
}
//関数呼びだす
//call(func(a, b, c));
//def func(int x, int y, int z){
//  int r = x + y + z;
//}
void callFunc(int &i, string fromFuncName){
    i++;//call
    i++;//'('
    string toFuncName = tokens[i];//name
    i++;//name
    i++;//'('
    auto funcArgs = args[toFuncName];
    int ptr = 0;
    while(tokens[i] != ")"){
        if(tokens[i] != ",") {
            int fromVal;
            if(numOrStr(tokens[i])){
                fromVal = strToInt(tokens[i]);
            }
            else {
                fromVal = vals[fromFuncName + "#" + tokens[i]];
            }
            string toValName = funcArgs[ptr];
            vals[toValName] = fromVal;
            ptr++;
        }
        i++;
    }
    i++;//')'
    i++;//')'
    i++;//';'
    
    //関数の中身を実行する
    string funcName = toFuncName;
    auto [st, end] = funcToScope[funcName];
    ptr = st + 1;
    while(ptr < end){
        query(ptr, funcName);
        ptr++;
    }
}



int main(){
    tokens = lexicalMain();
    for(int i = 0; i < sz(tokens);){
        auto ti = tokens[i];
        //関数のとき
        if(ti == "def"){
            i++;//'def'
            string funcName = tokens[i];
            i++;//'('
            //引数
            while(tokens[i] != ")"){
                if(tokens[i] == "int") {
                    i++;
                    string valName = funcName + "#" + tokens[i];
                    args[funcName].pb(valName);
                    vals[valName] = 0;
                }
                i++;
            }
            i++;//')'
            int st = i;
            int end;
            //関数の中身
            if(tokens[i] == "{"){
                while(tokens[i] != "}"){
                    i++;
                }
                end = i;
            }
            else {
                cout << "Error: 構文エラー, " << i << endl;
            }
            //関数名とスコープを対応させる
            funcToScope[funcName] = P(st, end);
            i++;//}
        }
        //地の文
        else {
            string funcName = "";
            query(i, funcName);
        }
    }
}