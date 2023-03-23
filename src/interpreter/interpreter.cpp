#include "interpreter.hpp"
#include "intp_declaration.cpp"
#include "intp_assign.cpp"
#include "intp_if.cpp"
#include "intp_while.cpp"
#include "intp_print.cpp"

void interpreter(statement_node* root){
    // ASTを走査
    auto node = root;
    while(node = get_next_statement_node(node)){
        if(node->token == "DECLARATION_STATEMENT"){
            intp_declaration(node);
        }
        if(node->token == "ASSIGN_STATEMENT"){
            intp_assign(node);
        }
        if(node->token == "IF_STATEMENT"){
            intp_if(node);
        }
        if(node->token == "WHILE_STATEMENT"){
            intp_while(node);
        }
        if(node->token == "PRINT_STATEMENT"){
            intp_print(node);
        }
    }
}