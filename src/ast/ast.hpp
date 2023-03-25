#pragma once
#include "../tools.cpp"
#include "../rst.cpp"


typedef struct statement_node;
statement_node* rst_to_ast(nonterm_node* root_nonterm_node);
int get_variable_value_int(string id);
