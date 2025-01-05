#include <bits/stdc++.h>
#include "Type.h"
#include "FUNC3.h"
#include "FUNC7.h"
#include "IMM7.h"

using namespace std;

pair<string, string> seg_off_reg(const string &s)
{
    string offset, reg;
    bool is_offset = true;
    for (char ch : s)
    {
        if (ch == '(')
        {
            is_offset = false;
        }
        else if (ch == ')')
        {
            break;
        }
        else if (is_offset)
        {
            offset += ch;
        }
        else
        {
            reg += ch;
        }
    }
    return {offset, reg};
}

class Register
{
public:
    string EncodeRegister(const string &s)
    {
        return bitset<5>(stoi(s.substr(1))).to_string();
    }
};

class Immediate
{
public:
    string getImmediateCode(int num)
    {
        return bitset<12>(num < 0 ? (1 << 12) + num : num).to_string();
    }
    string getShamtCode(int shamt)
    {
        if (shamt < 0 || shamt >= (1 << 5))
            throw invalid_argument("shamt out of bounds");
        return bitset<5>(shamt).to_string();
    }
    vector<string> getBTypeImmediateCode(int num)
    {
        string imm_bin = bitset<13>((num < 0 ? (1 << 13) + num : num)).to_string();
        return {imm_bin.substr(0, 1), imm_bin.substr(1, 1), imm_bin.substr(2, 6), imm_bin.substr(8, 4)};
    }
    string getJTypeImmediateCode(int num)
    {
        string imm_bin = bitset<21>((num < 0 ? (1 << 21) + num : num)).to_string();
        return imm_bin.substr(0, 1) + imm_bin.substr(10, 10) + imm_bin.substr(9, 1) + imm_bin.substr(1, 8);
    }
    string getUTypeImmediateCode(int num)
    {
        return bitset<20>(num).to_string();
    }
};

class Instruction
{
public:
    Instruction(const vector<string> &v, ofstream &outFile)
    {
        auto inst_type = Type[v[0]];
        if (inst_type == "R")
            encodeRType(v, outFile);
        else if (inst_type == "ImmArith")
            encodeImmTypeArithmetic(v, outFile);
        else if (inst_type == "ImmLogical")
            encodeImmTypeLogical(v, outFile);
        else if (inst_type == "ImmLoad")
            encodeImmTypeLoad(v, outFile);
        else if (inst_type == "B")
            encodeBType(v, outFile);
        else if (inst_type == "U")
            encodeUType(v, outFile);
        else if (inst_type == "J")
            encodeJType(v, outFile);
        else if (inst_type == "S")
            encodeSType(v, outFile);
        else
            outFile << "Instruction type not supported" << endl;
    }

private:
    Register reg;
    Immediate Imm;

    void encodeRType(const vector<string> &v, ofstream &outFile)
    {
        string func3 = FUNC3[v[0]], func7 = FUNC7[v[0]], opcode = "0110011";
        outFile << func7 + reg.EncodeRegister(v[3]) + reg.EncodeRegister(v[2]) + func3 + reg.EncodeRegister(v[1]) + opcode << endl;
    }

    void encodeImmTypeArithmetic(const vector<string> &v, ofstream &outFile)
    {
        string opcode = "0010011", func3 = FUNC3[v[0]];
        outFile << Imm.getImmediateCode(stoi(v[3])) + reg.EncodeRegister(v[2]) + func3 + reg.EncodeRegister(v[1]) + opcode << endl;
    }

    void encodeImmTypeLogical(const vector<string> &v, ofstream &outFile)
    {
        string opcode = "0010011", func3 = FUNC3[v[0]];
        outFile << IMM7[v[0]] + Imm.getShamtCode(stoi(v[3])) + reg.EncodeRegister(v[2]) + func3 + reg.EncodeRegister(v[1]) + opcode << endl;
    }

    void encodeImmTypeLoad(const vector<string> &v, ofstream &outFile)
    {
        string opcode = (v[0] == "jalr") ? "1100111" : "0000011", func3 = FUNC3[v[0]];
        auto [offset, rs1] = seg_off_reg(v[2]);
        outFile << Imm.getImmediateCode(stoi(offset)) + reg.EncodeRegister(rs1) + func3 + reg.EncodeRegister(v[1]) + opcode << endl;
    }

    void encodeSType(const vector<string> &v, ofstream &outFile)
    {
        string opcode = "0100011", func3 = FUNC3[v[0]];
        auto [offset, rs1] = seg_off_reg(v[2]);
        string imm = Imm.getImmediateCode(stoi(offset));
        outFile << imm.substr(0, 7) + reg.EncodeRegister(v[1]) + reg.EncodeRegister(rs1) + func3 + imm.substr(7) + opcode << endl;
    }

    void encodeBType(const vector<string> &v, ofstream &outFile)
    {
        string opcode = "1100011", func3 = FUNC3[v[0]];
        vector<string> imm_T = Imm.getBTypeImmediateCode(stoi(v[3]));
        string imm12 = imm_T[0], imm11 = imm_T[1], imm10_5 = imm_T[2], imm4_1 = imm_T[3];
        outFile << imm12 + imm10_5 + reg.EncodeRegister(v[2]) + reg.EncodeRegister(v[1]) + func3 + imm4_1 + imm11 + opcode << endl;
    }

    void encodeJType(const vector<string> &v, ofstream &outFile)
    {
        string opcode = "1101111";
        outFile << Imm.getJTypeImmediateCode(stoi(v[2])) + reg.EncodeRegister(v[1]) + opcode << endl;
    }

    void encodeUType(const vector<string> &v, ofstream &outFile)
    {
        string opcode = (v[0] == "lui") ? "0110111" : (v[0] == "auipc") ? "0010111"
                                                                        : throw invalid_argument("Unsupported U-Type instruction");
        outFile << Imm.getUTypeImmediateCode(stoi(v[2])) + reg.EncodeRegister(v[1]) + opcode << endl;
    }
};

void read_from_file(const string &filename, ofstream &outFile)
{
    ifstream inputFile(filename);
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        vector<string> instruction;
        string token;
        while (ss >> token)
            instruction.push_back(token);
        Instruction(instruction, outFile);
    }
}

int main()
{
    ofstream outFile("machine.txt");
    read_from_file("assembly.txt", outFile);
    outFile.close();
    return 0;
}
