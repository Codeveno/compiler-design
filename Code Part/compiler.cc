#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "execute.h"

InstructionNode* create_instruction(InstructionType type) {
    InstructionNode* new_node = (InstructionNode*)malloc(sizeof(InstructionNode));
    if (new_node == NULL) {
        printf("Error: Memory allocation failed for instruction node.\n");
        exit(1);
    }
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

InstructionNode* parse_Generate_Intermediate_Representation() {
    std::vector<std::string> input_lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        input_lines.push_back(line);
    }

    InstructionNode* head = NULL;
    InstructionNode* prev = NULL;

    for (const std::string& line : input_lines) {
        if (line.empty() || line[0] == '/') {
            continue;
        }

        if (line.find("IN") == 0) {
            int var_index = std::stoi(line.substr(3));
            InstructionNode* node = create_instruction(IN);
            node->input_inst.var_index = var_index;  
            if (prev == NULL) head = node;
            else prev->next = node;
            prev = node;
        } else if (line.find("OUT") == 0) {
            int var_index = std::stoi(line.substr(4));
            InstructionNode* node = create_instruction(OUT);
            node->output_inst.var_index = var_index;  
            if (prev == NULL) head = node;
            else prev->next = node;
            prev = node;
        } else if (line.find("ASSIGN") == 0) {
            size_t equals_pos = line.find("=");
            if (equals_pos != std::string::npos) {
                std::string lhs = line.substr(7, equals_pos - 7);
                std::string rhs = line.substr(equals_pos + 1);

                size_t operator_pos = rhs.find_first_of("+-*/");
                if (operator_pos != std::string::npos) {
                    std::string operator_str = rhs.substr(operator_pos, 1);
                    std::string rhs_left = rhs.substr(0, operator_pos);
                    std::string rhs_right = rhs.substr(operator_pos + 1);

                    InstructionNode* node = create_instruction(ASSIGN);
                    node->assign_inst.left_hand_side_index = std::stoi(lhs); 
                    node->assign_inst.opernd1_index = std::stoi(rhs_left);  
                    node->assign_inst.opernd2_index = std::stoi(rhs_right); 
                    node->assign_inst.op = (operator_str == "+") ? OPERATOR_PLUS :
                                          (operator_str == "-") ? OPERATOR_MINUS :
                                          (operator_str == "*") ? OPERATOR_MULT : OPERATOR_DIV;

                    if (prev == NULL) head = node;
                    else prev->next = node;
                    prev = node;
                }
            }
        } else if (line.find("CJMP") == 0) {
            std::string condition = line.substr(5);
            size_t operator_pos = condition.find_first_of("<>=!");
            if (operator_pos != std::string::npos) {
                std::string lhs = condition.substr(0, operator_pos);
                std::string operator_str = condition.substr(operator_pos, 2);
                std::string rhs = condition.substr(operator_pos + 2);

                InstructionNode* node = create_instruction(CJMP);
                node->cjmp_inst.opernd1_index = std::stoi(lhs);  
                node->cjmp_inst.opernd2_index = std::stoi(rhs);  
                node->cjmp_inst.condition_op = (operator_str == "==") ? CONDITION_NOTEQUAL :
                                               (operator_str == ">")  ? CONDITION_GREATER : CONDITION_LESS;

                if (prev == NULL) head = node;
                else prev->next = node;
                prev = node;
            }
        } else if (line.find("JMP") == 0) {
            std::string label = line.substr(4);
            InstructionNode* node = create_instruction(JMP);
            node->jmp_inst.target = NULL;  // In case no target is provided, can be updated later

            if (prev == NULL) head = node;
            else prev->next = node;
            prev = node;
        }
    }

    return head;
}
