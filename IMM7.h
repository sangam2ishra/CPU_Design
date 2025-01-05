#ifndef IMM7_H
#define IMM7_H

#include <bits/stdc++.h>
using namespace std;

static map<string, string> IMM7 = {
    // Logical Shift Instructions
    {"slli", "0000000"},  // Shift Left Logical Immediate
    {"srli", "0000000"},  // Shift Right Logical Immediate
    {"srai", "0100000"},  // Shift Right Arithmetic Immediate

    // Logical Instructions (functions that use the same IMM7 value)
    {"andi", "0000000"},  // AND Immediate
    {"ori",  "0000000"},  // OR Immediate
    {"xori", "0000000"}   // XOR Immediate
    // Add more instructions here if needed
};

#endif // IMM7_H
