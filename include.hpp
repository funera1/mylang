#include <bits/stdc++.h>
#define sz(x) (x).size()
using namespace std;
using Pbnf = pair<string, vector<string>>;

// 定数
static int TOKEN_TRANS_TABLE_MAX_SIZE = 100;
// tokenの種類
typedef enum tokens {
	TERMINAL,EOP,ID,NUMBER,INT,SEMICOLON,COMMA,LPAREN,RPAREN,LBRACE,
	RBRACE,LBRACKET,RBRACKET,ASSIGN,PROGRAM,COMPOUND,STATEMENT,
    DECLARATION_STATEMENT,ASSIGN_STATEMENT,NUSED
} tokenkind;