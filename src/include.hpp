#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <map>
#include <set>
#include <stack>
#include <queue>

#include <string.h>
#define sz(x) (x).size()
#define ALL(x) (x).begin(),(x).end()
using namespace std;
using P_src_dst = pair<string, vector<string>>;
using P_nonterm_term = pair<string, string>;

// あるtokenから遷移するtoken列のリスト(NUSED+1はtokenkindの個数)
// 例) tokenTransTable[0] = P_Src_Dst("PROGRAM", {"LBRACKET", "COMPOUND", "RBRACKET"})
vector<P_src_dst> bnf_transition_list;
// dstの移動先のリスト
vector< vector<vector<string>> >dfa_graphs;
// トークンの文字列から対応するtokenkindを割り当てる
map<string, int> tokenstr_to_tokenkind;
map<string, vector<vector<string>>> bnf_src_to_dst;
vector<string> nonterm_list;
vector<string> term_list;
string home_dir = "/home/funera1/workspace/mylang/";
// set<string> key_tokens = {"int", "if", "while"};
map<string, string> key_tokens = {
    {"int", "INT"},
    {"if", "IF"},
    {"while", "WHILE"},
    {"print", "PRINT"}
};
const int INF = 1e9;

// init.cpp
void init();

// lexical.cpp
string parse_input(string s);
pair<vector<string>, vector<string>> lexical_analysis(string str);

// parse.cpp
class Parser;

// packrat_parse.cpp
void parse(string input_str);

// packrat_cst.cpp
vector<string> get_cst(vector<string> token_sream);

// tools.cpp
string file_to_string(string input_file_path);
bool is_alphabet(char c);
string get_next_str(string base_str, int &not_cursor);
string check_first_token(char c);
string check_id_is_keyword(string s);
bool is_term(string s);
bool is_all_term_in_set(set<string> st);
ifstream file_to_ifstream(string file_path);
void create_nonterm_and_term_list();
bool is_in_set(set<string> st, string item);
int to_num(string s);
bool is_statement(string s);
vector<string> convert_reverse_polish(vector<string> formula);
vector<string> add_str_list(vector<string> dst, vector<string> src);