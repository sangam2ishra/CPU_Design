// #ifndef TYPE_H
// #define TYPE_H

// #include<bits/stdc++.h>
// using namespace std;

// static map<string, string> Type = {
//     {"add", "R"},
//     {"addi", "ImmArith"},
//     {"slli", "ImmLogical"},
//     {"lw", "ImmLoad"},
//     {"beq","B"}

//     // You can add more key-value pairs here
// };

// #endif // TYPE_H


#ifndef TYPE_H
#define TYPE_H

#include <bits/stdc++.h>
using namespace std;

static map<string, string> Type = {
    // R-Type Instructions
    {"add", "R"},
    {"sub", "R"},
    {"sll", "R"},
    {"slt", "R"},
    {"sltu", "R"},
    {"xor", "R"},
    {"srl", "R"},
    {"sra", "R"},
    {"or", "R"},
    {"and", "R"},
    {"mul", "R"},

    // I-Type Instructions (Arithmetic)
    {"addi", "ImmArith"},
    {"slti", "ImmArith"},
    {"sltiu", "ImmArith"},
    {"xori", "ImmArith"},
    {"ori", "ImmArith"},
    {"andi", "ImmArith"},

    // I-Type Instructions (Logical and Shift)
    {"slli", "ImmLogical"},
    {"srli", "ImmLogical"},
    {"srai", "ImmLogical"},

    // I-Type Instructions (Load)
    {"lb", "ImmLoad"},
    {"lh", "ImmLoad"},
    {"lw", "ImmLoad"},
    {"lbu", "ImmLoad"},
    {"lhu", "ImmLoad"},

    // S-Type Instructions (Store)
    {"sb", "S"},
    {"sh", "S"},
    {"sw", "S"},

    // B-Type Instructions (Branch)
    {"beq", "B"},
    {"bne", "B"},
    {"blt", "B"},
    {"bge", "B"},
    {"bltu", "B"},
    {"bgeu", "B"},

    // U-Type Instructions
    {"lui", "U"},
    {"auipc", "U"},

    // J-Type Instructions
    {"jal", "J"},

    // I-Type Instructions (Jumps and Links)
    {"jalr", "ImmLoad"},
    //SType
    {"sd", "S"},

    // Fence Instructions (not categorized, but usually treated as I-Type)
    {"fence", "ImmArith"},
    {"fence.i", "ImmArith"}
    
    // You can add more instructions if needed
};

#endif // TYPE_H
