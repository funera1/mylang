#pragma once
#include <bits/stdc++.h>
#define sz(x) (x).size()
#define ALL(x) (x).begin(),(x).end()
using namespace std;
using P_src_dst = pair<string, vector<string>>;
using P_nonterm_term = pair<string, string>;

// 定数
// static int TOKEN_TRANS_TABLE_MAX_SIZE = 100;
// tokenの種類
// extern enum tokenkind {
// 	TERMINAL,EOP,ID,NUMBER,INT,SEMICOLON,COMMA,LPAREN,RPAREN,LBRACE,
// 	RBRACE,LBRACKET,RBRACKET,ASSIGN,PROGRAM,COMPOUND,STATEMENT,
//     DECLARATION_STATEMENT,ASSIGN_STATEMENT,NUSED
// } ;