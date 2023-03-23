#pragma once
#include "../tools.cpp"
#include "../RST.cpp"


typedef struct statement_node;
statement_node* RST_to_AST(nonterm_node* root_nonterm_node);
int get_variable_value_int(string id);