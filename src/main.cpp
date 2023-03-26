#include "include.hpp"

// void dbg_print_token_stream(vector<string> token_stream){
//     for(int i = 0; i < sz(token_stream); i++)cout << token_stream[i] << endl;
// }

// void dbg_check_first_set(Parsing p){
//     cout << "###CHECK FIRST SET###" << endl;
//     for(auto [non_term, first_set] : p.first_sets){
//         cout << non_term << ":" << endl;
//         cout << '{';
//         for(auto t : first_set){
//             cout << t << ", ";
//         }
//         cout << '}' << endl;
//     }
//     cout << endl;
// }

// void dbg_check_follow_set(Parsing p){
//     cout << "###CHECK FOLLOW SET###" << endl;
//     for(auto [non_term, follow_set] : p.follow_sets){
//         cout << non_term << ":" << endl;
//         cout << '{';
//         for(auto t : follow_set){
//             cout << t << ", ";
//         }
//         cout << '}' << endl;
//     }
//     cout << endl;
// }

// void dbg_check_parsing_table(Parsing p){
//     auto t = p.create_ll_parsing_table();
//     for(auto ti : t){
//         auto [non_term, term] = ti.first;
//         int num = ti.second;
//         if(num == -1)continue;
//         cout << non_term << " " << term << " " << num << endl;
//     }
//     cout << endl << endl;
// }

int main(int argc, char* argv[]){
    if (argc != 2) {
        cout << "argument number is invalid" << endl;
    }
    string input_file = argv[1];
    string str = file_to_string(input_file);
    init();
    // cout << sz(bnf_transition_list) << endl;
    // 字句解析
    string x = parse_input(str);
    cout << x << endl;


    // cout << "[bnf_transition_list]" << endl;
    // for(auto [src, dst] : bnf_transition_list){
    //     cout << src << ": ";
    //     for(auto di : dst)cout << di << " ";
    //     cout << endl;
    // }
    // cout << "\\[bnf_transition_list]" << endl;
    // return 0;
    // cout << "[graph]" << endl;
    // auto dig_2_graph = dfa_graphs[4];
    // for(int i = 0; i < sz(dig_2_graph); i++){
    //     for(int j = 0; j < sz(dig_2_graph[i]); j++){
    //         if(dig_2_graph[i][j] != ""){
    //             cout << i << "->" << j << ": " << dig_2_graph[i][j] << endl;
    //         }
    //     }
    // }
    // cout << "\\[graph]" << endl;
    // for(auto [src, dst] : bnf_transition_list){
    //     cout << src << ": ";
    //     for(auto di : dst)cout << di << " ";
    //     cout << endl;
    // }
    // parse(x);
    // auto t = get_rst(token_stream);
    // for(auto ti : t)cout << ti << " ";cout << endl;
    // DEBUG
    // dbg_print_token_stream(token_stream);
}
