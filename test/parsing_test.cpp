#pragma once
#include "../include.cpp"
#include "../global_values.hpp"
#include "../lexical.cpp"
#include "../parsing.cpp"

int main(){
    Parsing p;
    auto t = p.create_ll_parsing_table();
    for(auto ti : t){
        auto [non_term, term] = ti.first;
        int num = ti.second;
        cout << non_term << " " << term << " " << num << endl;
    }
}