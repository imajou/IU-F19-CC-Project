#include "SymbolTable.hpp"


Node::Node() {
    next = NULL;    
}


Node::Node(std::string value, ClassName class_name) {
    this->value = value;
    this->class_name = class_name;
    this->next = NULL;
}


SymbolTable::SymbolTable() {
   for (int i = 0; i < MAX_HASH; i++) 
        nodes[i] = NULL;
}


int SymbolTable::hashSum(std::string node_value) {
    int hash_sum = 0;
   
    for (char sym : node_value) {
        hash_sum += 29 * (sym + 281);
        hash_sum %= MAX_HASH;
    }

    return hash_sum;
}


bool SymbolTable::insert(std::string value, ClassName class_name) {
    int hash_id = hashSum(value);
    Node* node = new Node(value, class_name);
    if (nodes[hash_id] == NULL) {
        nodes[hash_id] = node;
        
        return true;
    }
    else {
        Node* start = nodes[hash_id];
        if (start->value == value) 
            return false;
        while (start->next != NULL) 
            start = start->next;
        start->next = node;
        return true;
    }
    return false;
}


ClassName SymbolTable::find(std::string value) {
    int hash_id = hashSum(value);
    Node* start = nodes[hash_id];

    if (start == NULL) {
        return ClassName::None;
    }

    while (start != NULL) {
        if (start->value == value) {
            return start->class_name;
        }
        start = start->next;
    }
    return ClassName::None;
}
