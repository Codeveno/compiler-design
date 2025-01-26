#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>  
#include <vector>
#include "execute.h"

// Global memory and input tracking variables
int mem[1000] = {0};   // Memory storage (1000 variables)
int next_available = 0; // Keeps track of next available memory index
std::vector<int> inputs; // Stores input values for IN operations
int next_input = 0;      // Tracks the current input value

void debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void execute_program(struct InstructionNode * program) {
    struct InstructionNode * pc = program;  // Program counter (current instruction)
    int op1, op2, result;

    while (pc != NULL) {  // Loop through each instruction until end of program
        switch (pc->type) {
            case NOOP:  // No operation, just move to the next instruction
                pc = pc->next;
                break;

            case IN:  // Input instruction: read value into memory
                if (next_input < inputs.size()) {
                    mem[pc->input_inst.var_index] = inputs[next_input];
                    next_input++;
                } else {
                    debug("Error: No more inputs available.\n");
                    exit(1);  // Exit if no more inputs
                }
                pc = pc->next;
                break;

            case OUT:  // Output instruction: print value from memory
                printf("%d", mem[pc->output_inst.var_index]);

                // Check if the next instruction is an OUT and print a space, else print a newline
                if (pc->next != NULL && pc->next->type == OUT) {
                    printf(" ");  // Print space if more values are coming
                } else {
                    printf("\n");  // Print newline if it's the last output
                }

                pc = pc->next;  // Move to the next instruction
                break;

            case ASSIGN:  // Assignment instruction: perform operation and assign result
                switch (pc->assign_inst.op) {
                    case OPERATOR_PLUS:
                        op1 = mem[pc->assign_inst.opernd1_index];
                        op2 = mem[pc->assign_inst.opernd2_index];
                        result = op1 + op2;
                        break;
                    case OPERATOR_MINUS:
                        op1 = mem[pc->assign_inst.opernd1_index];
                        op2 = mem[pc->assign_inst.opernd2_index];
                        result = op1 - op2;
                        break;
                    case OPERATOR_MULT:
                        op1 = mem[pc->assign_inst.opernd1_index];
                        op2 = mem[pc->assign_inst.opernd2_index];
                        result = op1 * op2;
                        break;
                    case OPERATOR_DIV:
                        op1 = mem[pc->assign_inst.opernd1_index];
                        op2 = mem[pc->assign_inst.opernd2_index];
                        if (op2 == 0) {
                            debug("Error: Division by zero.\n");
                            exit(1);  // Exit on division by zero
                        }
                        result = op1 / op2;
                        break;
                    case OPERATOR_NONE:
                        op1 = mem[pc->assign_inst.opernd1_index];
                        result = op1;  // Just copy the value
                        break;
                    default:
                        debug("Error: Unsupported arithmetic operator.\n");
                        exit(1);  // Exit on unsupported operator
                }
                mem[pc->assign_inst.left_hand_side_index] = result; // Store result in memory
                pc = pc->next;
                break;

            case CJMP:  // Conditional jump instruction: check condition and jump if true
                if (pc->cjmp_inst.target == NULL) {
                    debug("Error: cjmp_inst->target is null.\n");
                    exit(1);
                }
                op1 = mem[pc->cjmp_inst.opernd1_index];
                op2 = mem[pc->cjmp_inst.opernd2_index];
                switch (pc->cjmp_inst.condition_op) {
                    case CONDITION_GREATER:
                        pc = (op1 > op2) ? pc->cjmp_inst.target : pc->next;
                        break;
                    case CONDITION_LESS:
                        pc = (op1 < op2) ? pc->cjmp_inst.target : pc->next;
                        break;
                    case CONDITION_NOTEQUAL:
                        pc = (op1 != op2) ? pc->cjmp_inst.target : pc->next;
                        break;
                    default:
                        debug("Error: Unsupported conditional operator.\n");
                        exit(1);
                }
                break;

            case JMP:  // Unconditional jump instruction: jump to a specific instruction
                if (pc->jmp_inst.target == NULL) {
                    debug("Error: jmp_inst->target is null.\n");
                    exit(1);
                }
                pc = pc->jmp_inst.target;  // Jump to the target instruction
                break;

            default:  // Error handling for unsupported instruction types
                debug("Error: Invalid instruction type (%d).\n", pc->type);
                exit(1);
        }
    }
}

// Entry point of the program, generates IR and executes it
int main() {
    struct InstructionNode* program;
    program = parse_Generate_Intermediate_Representation(); // Generate the IR
    if (program == NULL) {
        debug("Error: Failed to generate Intermediate Representation (IR).\n");
        return 1;  
    }

    execute_program(program); // Execute the program using the IR
    return 0;
}
