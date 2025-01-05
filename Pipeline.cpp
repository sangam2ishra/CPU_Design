#include <bits/stdc++.h>
using namespace std;

vector<int> GPR(32, 0);     // General Purpose Registers
unordered_map<int, int> DM; // Data Memory

map<string, string> Operation = {
    {"0000", "AND"},
    {"0001", "OR"},
    {"0010", "ADD"},
    {"0110", "SUB"},
    {"1111", "MUL"},
};

vector<bool> busy(32, false);

string get_type(string &opcode)
{
    if (opcode == "0100011")
    {
        return "S";
    }
    else if (opcode == "1100011")
    {
        return "B";
    }
    else if (opcode == "0110011")
    {
        return "R";
    }
    else if (opcode == "0010011")
    {
        return "I";
    }
    else if (opcode == "0000011")
    {
        return "Ld";
    }
    else if (opcode == "1101111")
    {
        return "U";
    }
    return "";
}
string SignedExtend(string s, int len = 32)
{
    int n = s.length();
    char c = s[0];  // Most significant bit for sign extension
    while (n < len) // Extend to 32 bits or specified length
    {
        s = c + s;
        n++;
    }
    return s;
}

// Convert binary string to decimal (supports negative numbers using 2's complement)
int to_decimal(const string &s)
{
    bitset<32> bits(s);
    return bits.to_ulong() - bits.test(31) * 2 * (1L << 31); // Convert considering signed 2's complement
}

// Immediate Generator
// string ImmGen(string immPart1, string immPart2)
// {
//     string immediate = immPart1 + immPart2; // Combine parts for immediate
//     return SignedExtend(immediate);         // Sign extend to 32 bits
// }

// Instruction Memory Class
int PC = 0;
int TPC = 0;
int NPC = 0;
class InstructionMemory
{
public:
    map<int, string> IM; // Instruction Memory Map
    // int pc;              // Program Counter

    InstructionMemory(vector<string> &machineCode)
    {
        // pc = 0;
        for (int i = 0; i < machineCode.size(); i++)
        {
            IM[i * 4] = machineCode[i]; // Store instruction at appropriate memory address
        }
    }

    string instruction(int x, int y, int pc)
    {
        if (IM.find(pc) == IM.end())
        {
            return "NOP"; // No operation if no instruction found
        }
        int mini = min(31 - x, 31 - y);
        int maxi = max(31 - x, 31 - y);
        return IM[pc].substr(mini, maxi - mini + 1);
    }

    string instruction(int x, int pc)
    {
        if (IM.find(pc) == IM.end())
        {
            return "NOP";
        }
        return string(1, IM[pc][31 - x]); // Return the specific bit as a string
    }
};

string ImmGen(InstructionMemory *IM, string &type, int pc)
{
    if (type == "I" || type == "Ld")
    {
        return IM->instruction(31, 20, pc);
    }

    if (type == "J")
    {
        string imm_for_j = IM->instruction(31, pc) + IM->instruction(19, 12, pc) + IM->instruction(20, pc) + IM->instruction(21, 30, pc);

        return imm_for_j;
    }

    if (type == "B")
    {
        string imm_for_b = IM->instruction(31, pc) + IM->instruction(7, pc) + IM->instruction(25, 30, pc) + IM->instruction(11, 8, pc);
        cout << to_decimal(imm_for_b) << endl;

        return imm_for_b;
    }
    if (type == "S")
    {
        string imm_for_S = IM->instruction(31, 25, pc) + IM->instruction(11, 7, pc);
        cout << to_decimal(imm_for_S) << endl;
        return imm_for_S;
    }
    return IM->instruction(31, 20, pc);
}

// Control Unit Class
class ControlUnit
{
public:
    string opcode;
    int ALUSrc, Mem2Reg, RegWrite, MemWrite, Branch, jump, ALUOp1, ALUOp0, MemRead;
    string ALUOp;
    ControlUnit()
    {
    }
    ControlUnit(string op, string func3 = "", string func7 = "")
    {
        opcode = op;
        string type = get_type(op);
        if (type == "S")
        {
            cout << "Store detected>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        }
        ALUSrc = (type == "S" || type == "I" || type == "Ld") ? 1 : 0;
        Mem2Reg = (type == "Ld") ? 1 : 0;
        RegWrite = (type == "R" || type == "I" || type == "Ld") ? 1 : 0;
        MemWrite = (type == "S") ? 1 : 0;
        Branch = (type == "B") ? 1 : 0;
        jump = (type == "U") ? 1 : 0;
        ALUOp1 = (type == "R" || type == "I") ? 1 : 0;
        ALUOp0 = (type == "B") ? 1 : 0;
        MemRead = !MemWrite;
        ALUOp = to_string(ALUOp1) + to_string(ALUOp0);
    }
};

string ALUControl(string ALUOp, string func7, string func3)
{
    // char ins_30 = imm[0];            // Instruction's 30th bit (for R-type operations)
    // string func3 = imm.substr(1, 3); // func3 is 3 bits
    string ALUSelect;

    if (ALUOp == "00")
    {
        return "0010";
    }
    if (ALUOp == "01")
    {
        return "0110";
    }

    if (func7 == "0000000" && func3 == "000")
    {
        return "0010";
    }
    if (func7 == "0100000" && func3 == "000")
    {
        return "0110";
    }
    if (func7 == "0000000" && func3 == "111")
    {
        return "0000";
    }
    if (func7 == "0000000" && func3 == "110")
    {
        return "0001";
    }
    if (func7 == "0000001" && func3 == "000")
    {
        return "1111";
    }

    return ALUSelect;
}

// ALU Class
class ALU
{
public:
    int ALUresult;
    bool ALUZeroFlag;

    ALU(string ALUSelect, int rs1, int rs2)
    {
        string op = Operation[ALUSelect];
        if (op == "ADD")
        {
            ALUresult = rs1 + rs2; // Perform addition
        }
        else if (op == "AND")
        {
            ALUresult = rs1 & rs2;
        }
        else if (op == "OR")
        {
            ALUresult = rs1 | rs2;
        }
        else if (op == "SUB")
        {
            ALUresult = rs1 - rs2;
        }
        else if (op == "MUL")
        {
            cout << "10000000000000000000000000000************************************************************0" << endl;
            ALUresult = rs1 * rs2;
        }

        ALUZeroFlag = (rs1 == rs2); // Set Zero Flag if rs1 == rs2
    }
};

class IFID
{
public:
    string IR;
    int DPC;
};

class IDEX
{
public:
    int DPC, JPC;
    string imm;
    string func3;
    int rs1, rs2;
    string rdl;
    ControlUnit CW;
    int SD_val;
};

class EXMO
{
public:
    int ALUOut;
    ControlUnit CW;
    string rdl;
    int DPC;
    int rs2;
};

class MOWB
{
public:
    int LDOut;
    ControlUnit CW;
    int ALUOut;
    int DPC;
    string rdl;
};

IFID ifid;
IDEX idex;
EXMO exmo;
MOWB mowb;

class CPU
{
public:
    InstructionMemory *IM;
    vector<bool> fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis;
    CPU(InstructionMemory *instrMem)
    {
        IM = instrMem;
    }

    void vis_reset(vector<bool> &fetch_vis, vector<bool> &decode_vis, vector<bool> &execute_vis, vector<bool> &MemOp_vis, vector<bool> &writeBack_vis, int pc)
    {
        fetch_vis[pc] = false;
        decode_vis[pc] = false;
        execute_vis[pc] = false;
        MemOp_vis[pc] = false;
        writeBack_vis[pc] = false;
    }

    void fetch()
    {
        cout << PC << "Getting fetched" << endl;
        string instruction = IM->instruction(0, 31, PC);
        ifid.IR = instruction;
        ifid.DPC = PC;
        TPC = PC + 4;
        // bool check_hazard = [] check_Control_Hazard() = >
        // {
        // }

        auto check_hazard = [&]() -> bool
        {
            if (IM->instruction(0, 6, PC) == "1100011")
                return true;
            return false;
        };
        if (!check_hazard())
        {
            PC = TPC;
            vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
        }
    }

    bool decode()
    {
        // can also do the same thing from ifid.ir
        string rsl1 = IM->instruction(19, 15, ifid.DPC);

        if (busy[to_decimal(rsl1)])
        {
            return false;
        }
        string rsl2 = IM->instruction(24, 20, ifid.DPC);
        if (busy[to_decimal(rsl2)])
        {
            string opcode = IM->instruction(0, 6, ifid.DPC);
            string Type = get_type(opcode);
            if (Type != "I" && Type != "U")
                return false;
        }
        cout << ifid.DPC << " getting decoded\n";
        string imm_for_j = IM->instruction(31, ifid.DPC) + IM->instruction(19, 12, ifid.DPC) + IM->instruction(20, ifid.DPC) + IM->instruction(21, 30, ifid.DPC);
        string sign_extend_j = SignedExtend(imm_for_j);

        idex.JPC = ifid.DPC + (to_decimal(sign_extend_j) << 1);
        string opcode = IM->instruction(6, 0, ifid.DPC);
        idex.DPC = ifid.DPC;
        string type = get_type(opcode);
        cout << type << endl;
        cout << ifid.DPC << endl;
        idex.imm = ImmGen(IM, type, ifid.DPC);
        cout << "idex.imm " << idex.imm << endl;
        cout << "alksdnfldsfk" << endl;
        idex.func3 = IM->instruction(14, 12, ifid.DPC);
        string func7 = IM->instruction(25, 31, ifid.DPC);
        idex.rdl = IM->instruction(11, 7, ifid.DPC);
        idex.rs1 = GPR[to_decimal(rsl1)];
        ControlUnit ControlWord(opcode, idex.func3);
        idex.CW = ControlWord;

        if (idex.CW.ALUSrc)
        {
            string sign_extend = SignedExtend(idex.imm);
            idex.rs2 = to_decimal(sign_extend);
        }
        else
        {
            idex.rs2 = GPR[to_decimal(rsl2)];
        }
        idex.SD_val = GPR[to_decimal(rsl2)];

        if (idex.CW.RegWrite)
        {
            busy[to_decimal(idex.rdl)] = true;
            cout << "reg " << to_decimal(idex.rdl) << " is busy" << endl;
        }
        return true;
    }

    void execute()
    {
        string ALUSelect = ALUControl(idex.CW.ALUOp, IM->instruction(25, 31, idex.DPC), idex.func3);
        ALU alu(ALUSelect, idex.rs1, idex.rs2);
        exmo.ALUOut = alu.ALUresult;
        int ALUZeroFlag = alu.ALUZeroFlag;
        exmo.rs2 = idex.SD_val;
        exmo.CW = idex.CW;
        exmo.rdl = idex.rdl;
        exmo.DPC = idex.DPC;
        cout << idex.DPC << "Getting executed\n";
        cout << "hello\n";
        // Compute branch target address
        cout << idex.imm << endl;
        cout << idex.imm << endl;
        int bpc = ((to_decimal(SignedExtend(idex.imm))) << 1) + idex.DPC;
        cout << "HI" << endl;
        int ppc=PC;
        // Check for branch
        if (idex.CW.Branch && ALUZeroFlag)
        {
            NPC = bpc;
            PC = NPC;
            vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
        }
        else
        {
            NPC = TPC; // Increment PC by 4 for next instruction
            if (idex.CW.Branch)
            {
                PC = NPC;
                vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
            }
        }

        if (idex.CW.jump)
        {
            NPC = idex.JPC;
            cout<<"Jumping<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
            cout<<ppc<<" "<<NPC<<endl;
            PC = NPC;
            vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
        }
    }

    void MemOp()
    {
        if (exmo.CW.MemWrite)
        {
            DM[exmo.ALUOut] = exmo.rs2; // Write to data memory
        }

        if (exmo.CW.MemRead)
        {
            if (DM.find(exmo.ALUOut) != DM.end())
                mowb.LDOut = DM[exmo.ALUOut]; // Load from data memory
        }
        mowb.DPC = exmo.DPC;
        cout << exmo.DPC << "Getting memory\n";
        mowb.ALUOut = exmo.ALUOut;
        mowb.CW = exmo.CW;
        mowb.rdl = exmo.rdl;
    }

    void writeBack()
    {
        if (mowb.CW.RegWrite)
        {
            if (mowb.CW.Mem2Reg)
            {
                GPR[to_decimal(mowb.rdl)] = mowb.LDOut; // Write from memory to register
            }
            else
            {
                GPR[to_decimal(mowb.rdl)] = mowb.ALUOut; // Write ALU result to register
                // DM[mowb.ALUOut] = 1e9 + 7;
            }
            cout << "Writing in Reg " << to_decimal(mowb.rdl) << endl;
            busy[to_decimal(mowb.rdl)] = false;
        }
        cout << mowb.DPC << "Getting writtenBack\n";
        cout << mowb.DPC << " completed**********************************************************************************************" << endl;
    }

    void run()
    {
        int n = IM->IM.size();
        fetch_vis = vector<bool>(4 * n, false);
        decode_vis = vector<bool>(4 * n, false);
        execute_vis = vector<bool>(4 * n, false);
        MemOp_vis = vector<bool>(4 * n, false);
        writeBack_vis = vector<bool>(4 * n, false);

        while (true)
        {
            if (!writeBack_vis[mowb.DPC] && MemOp_vis[mowb.DPC])
            {
                writeBack_vis[mowb.DPC] = true;
                cout << 5 << endl;
                writeBack();
            }
            if (!MemOp_vis[exmo.DPC] && execute_vis[exmo.DPC])
            {
                MemOp_vis[exmo.DPC] = true;
                cout << 4 << endl;
                MemOp();
            }
            if (!execute_vis[idex.DPC] && decode_vis[idex.DPC])
            {
                execute_vis[idex.DPC] = true;
                cout << 3 << endl;
                execute();
            }
            if (!decode_vis[ifid.DPC] && fetch_vis[ifid.DPC])
            {
                if (decode())
                {
                    decode_vis[ifid.DPC] = true;
                    cout << 2 << endl;
                }
                else
                {
                    cout << "Data Hazard\n";
                    continue;
                }
            }
            if (!fetch_vis[PC] && PC < 4 * n)
            {
                fetch_vis[PC] = true;
                cout << 1 << endl;
                fetch();
            }
            if (writeBack_vis[4 * n - 4])
            {
                cout << "COMPLETED\n";
                break;
            }
        }

        // Output register values
        cout << "GPR values:\n";
        for (int i = 0; i < 32; i++)
        {
            cout << "x" << i << ": " << GPR[i] << endl;
        }

        // Output data memory contents
        cout << "\n\nData Memory:\n";
        for (auto it = DM.begin(); it != DM.end(); ++it)
        {
            cout << "Address: " << it->first << ", Value: " << it->second << endl;
        }
        cout << "size of data memory=" << DM.size() << endl;
    }
};

signed main()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    // DM[4]=12;
    // GPR[2]=5;

    vector<string> machineCodes;
    string line;
    while (getline(cin, line))
    {
        machineCodes.push_back(line);
    }

    machineCodes.push_back("00000000000000010000000100010011");

    InstructionMemory IM(machineCodes);
    CPU cpu(&IM);
    cpu.run();

    return 0;
}