#include "../include.cpp"
struct variable_info {
    string type;
    int value_int;
};

map<string, variable_info> variable_table;

// すでに変数が存在するかどうか
bool is_exist_in_variable_table(string name){
    return ( variable_table.find(name) != variable_table.end() );
}
variable_info init_variable_info(){
    variable_info v_info;
    v_info.type = "";
    v_info.value_int = 0;
    return v_info;
}
void register_variable(string type, string name){
    auto v_info = init_variable_info();
    v_info.type = type;
    variable_table[name] = v_info;
}
void assign_value_int(string name, int value){
    assert(variable_table[name].type == "INT");
    variable_table[name].value_int = value;
}
int get_variable_value_int(string id){
    if(variable_table[id].type != "INT"){
        cout << variable_table[id].type << "型は数値計算できません" << endl;
        assert(0);
    }
    return variable_table[id].value_int;
}