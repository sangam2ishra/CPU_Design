// #ifndef FUNC7_H
// #define FUNC7_H

// #include<bits/stdc++.h>
// using namespace std;

// static map<string, string> FUNC7 = {
//     {"add", "0000000"}

// };

// #endif // TYPE_H


#ifndef FUNC7_H
#define FUNC7_H

#include <bits/stdc++.h>
using namespace std;

static map<string, string> FUNC7 = {
    // R-Type Instructions
    {"add", "0000000"},  // ADD
    {"sub", "0100000"},  // SUB
    {"sll", "0000000"},  // SHIFT LEFT LOGICAL
    {"slt", "0000000"},  // SET LESS THAN
    {"sltu", "0000000"}, // SET LESS THAN UNSIGNED
    {"xor", "0000000"},  // XOR
    {"srl", "0000000"},  // SHIFT RIGHT LOGICAL
    {"sra", "0100000"},  // SHIFT RIGHT ARITHMETIC
    {"or",  "0000000"},  // OR
    {"and", "0000000"},  // AND
    {"mul", "0000001"},

    // For specific `func7` values in other instructions:
    // Include any additional instructions with unique func7 values if needed.
};

#endif // FUNC7_H
