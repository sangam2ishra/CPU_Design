#include <bits/stdc++.h>
#include "Type.h"
#include "FUNC3.h"
#include "FUNC7.h"
#include "IMM7.h"

using namespace std;

pair<string, string> seg_off_reg(string s)
{
    string offset;
    string reg;
    bool check = true;
    int n = s.length();
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '(')
        {
            check = false;
            continue;
        }
        else if (s[i] == ')')
        {
            break;
        }
        if (check)
        {
            offset += s[i];
        }
        else
        {
            reg += s[i];
        }
    }
    return {offset, reg};
}

class Register
{
public:
    string EncodeRegister(const string &s)
    {
        string temp = s.substr(1);
        int reg_num = stoi(temp);
        return bitset<5>(reg_num).to_string();
    }
};

class RTypeInstruction
{
public:
    Register reg;
    string rd, rs1, rs2;
    string func3, func7, opcode;
    vector<string> instruction;

    RTypeInstruction(vector<string> v)
    {
        instruction = v;
        opcode = "0110011";
        parse(v);
    }

    void parse(const vector<string> &v)
    {
        instruction = v;
        if (instruction.size() < 4)
        {
            throw invalid_argument("Instruction does not have enough parts.");
        }
        func3 = FUNC3[instruction[0]];
        func7 = FUNC7[instruction[0]];
        rd = instruction[1];
        rs1 = instruction[2];
        rs2 = instruction[3];
    }

    string encode()
    {
        return func7 + reg.EncodeRegister(rs2) + reg.EncodeRegister(rs1) + func3 + reg.EncodeRegister(rd) + opcode;
    }
};

class Immediate
{
public:
    string getImmediateCode(int num)
    {
        if (num < 0)
        {
            num = (1 << 12) + num;
        }
        return bitset<12>(num).to_string();
    }

    string getShamtCode(int shamt)
    {
        if (shamt < 0 || shamt >= (1 << 5))
        {
            throw invalid_argument("shamt out of bounds, must be between 0 and 31.");
        }
        return bitset<5>(shamt).to_string();
    }

    vector<string> getBTypeImmediateCode(int num)
    {
        if (num < 0)
        {
            num = (1 << 12) + num;
        }
        string imm_bin = bitset<13>(num).to_string();

       
        string imm12,imm11;
        imm12+=imm_bin[0];
        
        imm11+=imm_bin[1];
        string imm10_5=imm_bin.substr(2, 6);
        string imm4_1=imm_bin.substr(8, 4);
        return {imm12, imm11, imm10_5, imm4_1};
    }

    string getJTypeImmediateCode(int num)
    {
        if (num < 0)
        {
            num = (1 << 20) + num;
        }
        string imm_bin = bitset<21>(num).to_string();

        string imm20 = imm_bin.substr(0, 1);     // 1 bit
        string imm19_12 = imm_bin.substr(1, 8);  // 8 bits
        string imm11 = imm_bin.substr(9, 1);     // 1 bit
        string imm10_1 = imm_bin.substr(10, 10); // 10 bits

        return imm20 + imm10_1+ imm11+imm19_12;
    }

    string getUTypeImmediateCode(int num)
    {
        return bitset<20>(num).to_string();
    }
};

class ImmType : public Immediate
{
public:
    Register reg;
    string rd, rs1, imm, func3, opcode;
    vector<string> instruction;

    string encode()
    {
        return imm + reg.EncodeRegister(rs1) + func3 + reg.EncodeRegister(rd) + opcode;
    }
};

class ImmTypeArithmetic : public ImmType
{
public:
    ImmTypeArithmetic(vector<string> v)
    {
        instruction = v;
        parse(v);
    }
    void parse(vector<string> &v)
    {
        opcode = "0010011";
        Immediate Imm;
        imm = Imm.getImmediateCode(stoi(v[3]));
        rs1 = v[2];
        rd = v[1];
        func3 = FUNC3[v[0]];
    }
};

class ImmTypeLogical : public ImmType
{
public:
    ImmTypeLogical(vector<string> v)
    {
        instruction = v;
        parse(v);
    }
    void parse(vector<string> &v)
    {
        opcode = "0010011";
        Immediate Imm;
        imm = IMM7[v[0]] + Imm.getShamtCode(stoi(v[3]));
        rs1 = v[2];
        rd = v[1];
        func3 = FUNC3[v[0]];
        // cout<<func3<<endl;
    }
};

class ImmTypeLoad : public ImmType
{
public:
    ImmTypeLoad(vector<string> v)
    {
        instruction = v;
        parse(v);
    }
    void parse(vector<string> &v)
    {
        opcode = "0000011";
        if(v[0]=="jalr") opcode="1100111";
        rd = v[1];
        auto temp = seg_off_reg(v[2]);
        rs1 = temp.second;
        string offset = temp.first;
        Immediate Imm;
        imm = Imm.getImmediateCode(stoi(offset));
        func3 = FUNC3[v[0]];

    
    }
};

class SType
{
public:
    vector<string> instruction;
    Register reg;
    string rs1, rs2;
    string imm, imm7, imm5;
    string func3, opcode;

    SType(vector<string> v)
    {
        instruction = v;
        parse(v);
    }

    void parse(vector<string> &v)
    {
        opcode = "0100011";
        Immediate Imm;
        auto temp = seg_off_reg(v[2]);
        rs1 = temp.second;
        rs2 = v[1];
        imm = Imm.getImmediateCode(stoi(temp.first));
        imm7 = imm.substr(0, 7);
        imm5 = imm.substr(7);
        func3 = FUNC3[v[0]];
    }

    string encode()
    {
        return imm7 + reg.EncodeRegister(rs2) + reg.EncodeRegister(rs1) + func3 + imm5 + opcode;
    }
};
class BType
{
public:
    vector<string> instruction;
    Register reg;
    string rs1, rs2;
    string imm12, imm11, imm10_5, imm4_1, func3, opcode;

    BType(vector<string> v)
    {
        parse(v);
    }

    void parse(vector<string> &v)
    {
        opcode = "1100011";
        Immediate Imm;
        auto imm_T =Imm.getBTypeImmediateCode(stoi(v[3]));
        // [imm12, imm11, imm10_5, imm4_1] = imm_T;
        imm12=imm_T[0];
        imm11=imm_T[1];
        imm10_5=imm_T[2];
        imm4_1=imm_T[3];
        // cout<<imm4_1<<endl;
        rs1 = v[1];
        rs2 = v[2];
        func3 = FUNC3[v[0]];
    }

    string encode()
    {
        // return imm12 + imm11 + reg.EncodeRegister(rs1) + reg.EncodeRegister(rs2) + func3 + imm10_5 + imm4_1 + opcode;
        return imm12+imm10_5+reg.EncodeRegister(rs2)+reg.EncodeRegister(rs1)+func3+imm4_1+imm11+opcode;
    }
};


class JType
{
public:
    vector<string> instruction;
    Register reg;
    string rd;
    string imm;
    string opcode;

    JType(vector<string> v)
    {
        instruction = v;
        parse(v);
    }

    void parse(vector<string> &v)
    {
        opcode = "1101111"; // Opcode for JAL
        rd = reg.EncodeRegister(v[1]);
        int imm_val = stoi(v[2]);
        Immediate Imm;
        imm = Imm.getJTypeImmediateCode(imm_val);
    }

    string encode()
    {
        return imm + rd + opcode;
    }
};

class UType
{
public:
    Register reg;
    string rd, imm, opcode;
    vector<string> instruction;

    UType(vector<string> v)
    {
        instruction = v;
        parse(v);
    }

    void parse(vector<string> &v)
    {
        if (v[0] == "lui")
            opcode = "0110111"; // Opcode for LUI

        else if (v[0] == "auipc")
            opcode = "0010111"; // Opcode for AUIPC
        else
            throw invalid_argument("Unsupported U-Type instruction.");
        Immediate Imm;
        imm = bitset<20>(stoi(v[2])).to_string(); // Assuming you want a 20-bit immediate value
        rd = reg.EncodeRegister(v[1]);
    }

    string encode()
    {
        return imm + rd + opcode;
    }
};

// In the Instruction class:
class Instruction
{
public:
    Instruction(const vector<string> &instruction, ofstream &outFile)
    {
        string s = instruction[0];
        if (Type[s] == "R")
        {
            RTypeInstruction ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if (Type[s] == "ImmArith")
        {
            ImmTypeArithmetic ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if (Type[s] == "ImmLogical")
        {
            ImmTypeLogical ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if (Type[s] == "ImmLoad")
        {
            ImmTypeLoad ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if (Type[s] == "B")
        {
            BType ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if (Type[s] == "U")
        {
            UType ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if (Type[s] == "J")
        {
            JType ins(instruction);
            string encodedInstruction = ins.encode();
            outFile << encodedInstruction << endl;
        }
        else if(Type[s]=="S")
        {
            SType ins(instruction);
            string encodedInstruction=ins.encode();
            outFile<<encodedInstruction<<endl;
        }
        else
        {
            outFile << "Instruction type not supported" << endl;
        }
    }
};

void read_from_file(const string &filename, ofstream &outFile)
{
    ifstream inputFile(filename);
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        string token;
        vector<string> instruction;
        while (ss >> token)
        {
            instruction.push_back(token);
        }
        Instruction inst(instruction, outFile);
    }
}

int main()
{
    ofstream outFile("machine.txt");
    read_from_file("assembly.txt", outFile);
    outFile.close();
    return 0;
}
