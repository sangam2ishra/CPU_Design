#ifndef FUNC3_H
#define FUNC3_H

#include <bits/stdc++.h>
using namespace std;

static map<string, string> FUNC3 = {
    // R-Type Instructions
    {"add", "000"},   // ADD
    {"sub", "000"},   // SUB
    {"sll", "001"},   // SHIFT LEFT LOGICAL
    {"slt", "010"},   // SET LESS THAN
    {"sltu", "011"},  // SET LESS THAN UNSIGNED
    {"xor", "100"},   // XOR
    {"srl", "101"},   // SHIFT RIGHT LOGICAL
    {"sra", "101"},   // SHIFT RIGHT ARITHMETIC
    {"or",  "110"},   // OR
    {"and", "111"},   // AND
    {"mul", "000"},

    // I-Type Instructions
    {"addi", "000"},  // ADD IMMEDIATE
    {"slti", "010"},  // SET LESS THAN IMMEDIATE
    {"sltiu","011"},  // SET LESS THAN UNSIGNED IMMEDIATE
    {"xori", "100"},  // XOR IMMEDIATE
    {"ori",  "110"},  // OR IMMEDIATE
    {"andi", "111"},  // AND IMMEDIATE
    {"slli", "001"},  // SHIFT LEFT LOGICAL IMMEDIATE
    {"srli", "101"},  // SHIFT RIGHT LOGICAL IMMEDIATE
    {"srai", "101"},  // SHIFT RIGHT ARITHMETIC IMMEDIATE

    // S-Type Instructions
    {"sb",   "000"},  // STORE BYTE
    {"sh",   "001"},  // STORE HALFWORD
    {"sw",   "010"},  // STORE WORD

    // B-Type Instructions
    {"beq",  "000"},  // BRANCH EQUAL
    {"bne",  "001"},  // BRANCH NOT EQUAL
    {"blt",  "100"},  // BRANCH LESS THAN
    {"bge",  "101"},  // BRANCH GREATER THAN OR EQUAL
    {"bltu", "110"},  // BRANCH LESS THAN UNSIGNED
    {"bgeu", "111"},  // BRANCH GREATER THAN OR EQUAL UNSIGNED

    // U-Type Instructions
    {"lui",  "0110111"}, // LOAD UPPER IMMEDIATE
    {"auipc","0010111"}, // ADD IMMEDIATE TO PC

    // J-Type Instructions
    {"jal",  "1101111"}, // JUMP AND LINK
    {"jalr", "000"}, // JUMP AND LINK REGISTER

    // I-Type Load Instructions
    {"lb",   "000"},  // LOAD BYTE
    {"lh",   "001"},  // LOAD HALFWORD
    {"lw",   "010"},  // LOAD WORD
    {"lbu",  "100"},  // LOAD BYTE UNSIGNED
    {"lhu",  "101"},  // LOAD HALFWORD UNSIGNED
    //S-Type
    {"sd", "011"},
    // CSR Instructions
    {"csrrw", "001"},  // CSR READ AND WRITE
    {"csrrs", "010"},  // CSR READ AND SET
    {"csrrc", "011"},  // CSR READ AND CLEAR
    {"csrrwi","101"},  // CSR READ AND WRITE WITH IMMEDIATE
    {"csrrsi","110"},  // CSR READ AND SET WITH IMMEDIATE
    {"csrrci","111"},  // CSR READ AND CLEAR WITH IMMEDIATE

    // Additional Instructions
    {"nop", "000"},   // No Operation (usually represented as addi x0, x0, 0)
    {"ecall", "000"}, // Environment Call (pseudo instruction)
    {"ebreak", "000"} // Environment Break (pseudo instruction)
    // Add more instructions if needed
};

#endif // FUNC3_H
