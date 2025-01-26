#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>
#include <vector>

// Forward declaration of the InstructionNode structure
struct InstructionNode;

// Memory and input management (declarations should be in a corresponding .cpp file)
extern int mem[1000];
extern int next_available;
extern std::vector<int> inputs;
extern int next_input;

// Arithmetic Operators (Enum definition for arithmetic operations)
enum ArithmeticOperatorType {
    OPERATOR_NONE = 123,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULT,
    OPERATOR_DIV
};

// Conditional Operators (Enum definition for conditional comparisons)
enum ConditionalOperatorType {
    CONDITION_GREATER = 345,
    CONDITION_LESS,
    CONDITION_NOTEQUAL
};

// Instruction Types (Enum for different types of instructions)
enum InstructionType {
    NOOP = 1000,  // No operation
    IN,           // Input instruction
    OUT,          // Output instruction
    ASSIGN,       // Assignment instruction
    CJMP,         // Conditional jump
    JMP           // Jump
};

// InstructionNode structure for different instruction types
// This structure can hold different types of instructions using a union
struct InstructionNode {
    InstructionType type;  // Type of instruction (e.g., IN, OUT, ASSIGN)

    // Union to hold different instruction data
    union {
        struct {
            int left_hand_side_index;
            int opernd1_index;
            int opernd2_index;
            ArithmeticOperatorType op;
        } assign_inst;

        struct {
            int var_index;
        } input_inst;

        struct {
            int var_index;
        } output_inst;

        struct {
            ConditionalOperatorType condition_op;
            int opernd1_index;
            int opernd2_index;
            struct InstructionNode* target;  // Target instruction for CJMP
        } cjmp_inst;

        struct {
            struct InstructionNode* target;  // Target instruction for JMP
        } jmp_inst;
    };

    struct InstructionNode* next;  // Pointer to the next instruction in the list or NULL
};

// Debug function prototype
void debug(const char* format, ...);

// Function to parse and generate the intermediate representation of the program
struct InstructionNode* parse_Generate_Intermediate_Representation();

#endif /* _COMPILER_H_ */
