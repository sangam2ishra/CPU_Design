#include<bits/stdc++.h>
#include "Type.h"
#include "FUNC3.h"
#include "FUNC7.h"
#include "IMM7.h"

using namespace std;

vector<string> MACHINE_CODE;

pair<string, string> seg_off_reg(const string &s)
{
    string offset, reg;
    bool is_offset=true;
    for(char ch: s)
    {
        if(ch=='(')
        {
            is_offset=false;
        }
        else if(ch==')')
        {
            break;
        }
        else if(is_offset)
        {
            offset+=ch;
        }
        else{
            reg+=ch;
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
        return bitset<12>(num<0?(1<<12)+num:num).to_string();
    }
    
    string getShamtCode(int shamt)
    {
        if(shamt<0 || shamt>=(1<<5))
        {
            throw invalid_argument("shamt out of bounds");
            
        }
        return bitset<5>(shamt).to_string();
    }
    
    vector<string> getBTypeImmediateCode(int num)
    {
        string imm_bin=bitset<13>((num<0?(1<<13)+num:num)).to_string();
        return {imm_bin.substr(0, 1), imm_bin.substr(1,1), imm_bin.substr(2, 6), imm_bin.substr(8, 4)};
    }
    
    
    string getJTypeImmediateCode(int num)
    {
        string imm_bin=bitset<21>((num<0?(1<<21)+num:num)).to_string();
        return imm_bin.substr(0, 1)+imm_bin.substr(10, 10)+imm_bin.substr(9, 1)+imm_bin.substr(1, 8);
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
        auto inst_type=Type[v[0]];
        
        if(inst_type=="R")
        {
            encodeRType(v, outFile);
        }
        else if(inst_type=="ImmArith")
        {
            encodeImmTypeArithemetic(v, outFile);
        }
        else if(inst_type=="ImmLogical")
        {
            encodeImmTypeLogical(v, outFile);
        }
        else if(inst_type=="ImmLoad")
        {
            encodeImmTypeLoad(v, outFile);
        }
        else if(inst_type=="B")
        {
            encodeBType(v, outFile);
        }
        else if(inst_type=="U")
        {
            encodeUType(v, outFile);
        }
        else if(inst_type=="J")
        {
            encodeJType(v, outFile);
        }
        else if(inst_type=="S")
        {
            encodeSType(v, outFile);
        }
        else
        {
            cout<<"Instruction Type not supported"<<endl;
        }
    }
    
    private:
    Register reg;
    Immediate Imm;
    
    void encodeRType(const vector<string> &v, ofstream &outFile)
    {
        string func3=FUNC3[v[0]],func7=FUNC7[v[0]], opcode="0110011";
        cout<<func7+reg.EncodeRegister(v[3])+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(func7+reg.EncodeRegister(v[3])+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    void encodeImmTypeArithemetic(const vector<string> &v, ofstream &outFile)
    {
        string func3=FUNC3[v[0]],opcode="0010011";
        cout<<Imm.getImmediateCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getImmediateCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    void encodeImmTypeLogical(const vector<string> &v, ofstream &outFile)
    {
        string opcode="0010011", func3=FUNC3[v[0]];
        cout<<IMM7[v[0]]+Imm.getShamtCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(IMM7[v[0]]+Imm.getShamtCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    void encodeImmTypeLoad(const vector<string> &v, ofstream &outFile)
    {
        string opcode=(v[0]=="jalr")?"1100111":"0000011", func3=FUNC3[v[0]];
        // auto [offset, rs1]=seg_off_reg(v[2]);
        auto result=seg_off_reg(v[2]);
        string offset=result.first;
        string rs1=result.second;
        cout<<Imm.getImmediateCode(stoi(offset))+reg.EncodeRegister(rs1)+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getImmediateCode(stoi(offset))+reg.EncodeRegister(rs1)+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    
    void encodeSType(const vector<string> &v, ofstream &outFile)
    {
        string opcode="0100011", func3=FUNC3[v[0]];
        // auto [offset, rs1]=seg_off_reg(v[2]);
        auto result=seg_off_reg(v[2]);
        string offset=result.first;
        string rs1=result.second;
        string imm=Imm.getImmediateCode(stoi(offset));
        cout<<imm.substr(0, 7)+reg.EncodeRegister(v[1])+reg.EncodeRegister(rs1)+func3+imm.substr(7)+opcode<<endl;
        MACHINE_CODE.push_back(imm.substr(0, 7)+reg.EncodeRegister(v[1])+reg.EncodeRegister(rs1)+func3+imm.substr(7)+opcode);
    }
    
    void encodeBType(const vector<string> &v, ofstream &outFile)
    {
        string opcode="1100011", func3=FUNC3[v[0]];
        vector<string> imm_T=Imm.getBTypeImmediateCode(stoi(v[3]));
        string imm12=imm_T[0], imm11=imm_T[1], imm10_5=imm_T[2], imm4_1=imm_T[3];
        cout<<imm12+imm10_5+reg.EncodeRegister(v[2])+reg.EncodeRegister(v[1])+func3+imm4_1+imm11+opcode<<endl;
        MACHINE_CODE.push_back(imm12+imm10_5+reg.EncodeRegister(v[2])+reg.EncodeRegister(v[1])+func3+imm4_1+imm11+opcode);
    }
    
    void encodeJType(const vector<string>&v, ofstream &outFile)
    {
        string opcode="1101111";
        cout<<Imm.getJTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getJTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode);
    }
    

    void encodeUType(const vector<string> &v, ofstream &outFile)
    {
        string opcode=(v[0]=="lui")?"0110111":(v[0]=="auipc")?"0010111":throw invalid_argument("Unsupported U-Type instruction");
        cout<<Imm.getUTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getUTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode);
        
    }
};


void read_from_file(const string &filename, ofstream &outFile)
{
    ifstream inputFile(filename);
    string line;
    while(getline(inputFile, line))
    {
        stringstream ss(line);
        vector<string> instruction;
        string token;
        while(ss>>token)
        {
            instruction.push_back(token);
            
        }
        Instruction(instruction, outFile);
    }
}





//CPU DESIGN--------------------------------------------------------------------------------------------------------

vector<int> GPR(32, 0);
unordered_map<int, int> DM;

map<string, string> Operation={
    {"0000", "AND"},
    {"0001", "OR"},
    {"0010", "ADD"},
    {"0110", "SUB"},
    {"1111", "MUL"}
};

vector<bool> busy(32, false);

string get_type(string &opcode)
{
    if(opcode=="0100011")
    {
        return "S";
    }
    else if(opcode=="1100011")
    {
        return "B";
    }
    else if(opcode=="0110011")
    {
        return "R";
    }
    else if(opcode=="0010011")
    {
        return "I";
    }
    else if(opcode=="0000011")
    {
        return "Ld";
    }
    else if(opcode=="1101111")
    {
        return "U";
    }
    return "";
}


string SignedExtend(string s, int len=32)
{
    int n=s.length();
    char c=s[0];
    while(n<len)
    {
        s=c+s;
        n++;
    }
    return s;
}

int to_decimal(const string &s)
{
    bitset<32> bits(s);
    return bits.to_ulong()-bits.test(31)*2*(1LL<<31);
}


int PC=0;
int TPC=0;
int NPC=0;


class InstructionMemory
{
    public:
        map<int, string> IM;
        // int pc;
        
        InstructionMemory(vector<string> &machineCode)
        {
            // pc=0;
            for(int i=0; i<machineCode.size(); i++)
            {
                IM[i*4]=machineCode[i];
            }
        }
        
        string instruction(int x, int y, int pc)
        {
            if(IM.find(pc)==IM.end())
            {
                return "NOP";
            }
            
            int mini=min(31-x, 31-y);
            int maxi=max(31-x, 31-y);
            return IM[pc].substr(mini, maxi-mini+1);
            
        }
        
        string instruction(int x, int pc)
        {
            if(IM.find(pc)==IM.end())
            {
                return "NOP";
            }
            return string(1, IM[pc][31-x]);
        }
        
};



string ImmGen(InstructionMemory *IM, string &type, int pc)
{
    if(type=="I" || type=="Ld")
    {
        return IM->instruction(31, 20, pc);
    }
    if(type=="J")
    {
        string imm_for_j=IM->instruction(31, pc)+IM->instruction(19,12, pc)+IM->instruction(20, pc)+IM->instruction(21, 30, pc);
        return imm_for_j;
    }
    if(type=="B")
    {
        string imm_for_b=IM->instruction(31, pc)+IM->instruction(7, pc)+IM->instruction(25, 30, pc)+IM->instruction(11, 8, pc);
        return imm_for_b;
    }
    if(type=="S")
    {
        string imm_for_S=IM->instruction(31, 25, pc)+IM->instruction(11, 7, pc);
        return imm_for_S;
    }
    return IM->instruction(31,20, pc);
}

class ControlUnit
{
  public:
  string opcode;
  int ALUSrc, Mem2Reg, RegWrite, MemWrite, Branch, jump, ALUOp1, ALUOp0, MemRead;
  string ALUOp;
  ControlUnit()
  {
      
  }
  
  ControlUnit(string op, string func3="", string func7="")
  {
      opcode=op;
      string type=get_type(op);
      ALUSrc=(type=="S"||type=="I"||type=="Ld")?1:0;
      Mem2Reg=(type=="Ld")?1:0;
      RegWrite=(type=="R" || type=="I" || type=="Ld")?1:0;
      MemWrite=(type=="S")?1:0;
      Branch=(type=="B")?1:0;
      jump=(type=="U")?1:0;
      ALUOp1=(type=="R" )?1:0;
      ALUOp0=(type=="B")?1:0;
      MemRead=!MemWrite;
      ALUOp=to_string(ALUOp1)+to_string(ALUOp0);
      
  }
};

string ALUControl(string ALUOp, string func7, string func3)
{
    string ALUSelect;
    
    if(ALUOp=="00")
    {
        return "0010";
    }
    if(ALUOp=="01")
    {
        return "0110";
    }
    
    if(func7=="0000000" && func3=="000")
    {
        return "0010";
    }
    if(func7=="0100000" && func3=="000")
    {
        return "0110";
    }
    if(func7=="0000000" && func3=="111")
    {
        return "0000";
    }
    if(func7=="0000000" && func3=="110")
    {
        return "0001";
    }
    if(func7=="0000001" && func3=="000")
    {
        return "1111";
    }
    return ALUSelect;
}


class ALU
{
    public:
    int ALUresult;
    bool ALUZeroFlag;
    
    ALU(string ALUSelect, int rs1, int rs2)
    {
        string op=Operation[ALUSelect];
        if(op=="ADD")
        {
            ALUresult=rs1+rs2;
        }
        else if(op=="AND")
        {
            ALUresult=rs1&rs2;
            
        }
        else if(op=="OR")
        {
            ALUresult=rs1|rs2;
        }
        else if(op=="SUB")
        {
            ALUresult=rs1-rs2;
        }
        else if(op=="MUL")
        {
            ALUresult=rs1*rs2;
        }
        
        ALUZeroFlag=(rs1==rs2);
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
        IM=instrMem;
    }
    
    
    
    void vis_reset(vector<bool> &fetch_vis, vector<bool> &decode_vis, vector<bool> &execute_vis, vector<bool> &MemOp_vis, vector<bool> &writeBack_vis, int pc)
    {
        fetch_vis[pc]=false;
        decode_vis[pc]=false;
        execute_vis[pc]=false;
        MemOp_vis[pc]=false;
        writeBack_vis[pc]=false;
    }
    void fetch()
    {
        string instruction=IM->instruction(0, 31, PC);
        
        ifid.IR=instruction;
        ifid.DPC=PC;
        TPC=PC+4;
        
        auto check_hazard=[&]()->bool
        {
            if(IM->instruction(0, 6, PC)=="1100011")
            {
                return true;
            }
            return false;
        };
        if(!check_hazard())
        {
            PC=TPC;
            vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
        }
    }
    
    bool decode()
    {
        string rsl1=IM->instruction(19, 15, ifid.DPC);
        
        if(busy[to_decimal(rsl1)])
        {
            return false;
        }
        
        string rsl2=IM->instruction(24, 20, ifid.DPC);
        if(busy[to_decimal(rsl2)])
        {
            string opcode=IM->instruction(0, 6, ifid.DPC);
            string Type=get_type(opcode);
            if(Type!="I" && Type!="U")
            {
                return false;
            }
        }
        string imm_for_j=IM->instruction(31, ifid.DPC)+IM->instruction(19, 12, ifid.DPC)+IM->instruction(20, ifid.DPC)+IM->instruction(21, 30, ifid.DPC);
        string sign_extend_j=SignedExtend(imm_for_j);
        
        idex.JPC=ifid.DPC+(to_decimal(sign_extend_j)<<1);
        
        // idex.JPC=IM->pc+(to_decimal(sign_extend_j)<<1);
        string opcode=IM->instruction(6, 0, ifid.DPC);
        idex.DPC=ifid.DPC;
        string type=get_type(opcode);
        idex.imm=ImmGen(IM, type, ifid.DPC);
        idex.func3=IM->instruction(14, 12, ifid.DPC);
        string func7=IM->instruction(25, 31, ifid.DPC);
        idex.rdl=IM->instruction(11, 7, ifid.DPC);

        idex.rs1=GPR[to_decimal(rsl1)];
        ControlUnit ControlWord(opcode, idex.func3);
        idex.CW=ControlWord;
        
        if(idex.CW.ALUSrc)
        {
            string sign_extend=SignedExtend(idex.imm);
            idex.rs2=to_decimal(sign_extend);
            
        }
        else
        {
            idex.rs2=GPR[to_decimal(rsl2)];
        }
        
        idex.SD_val=GPR[to_decimal(rsl2)];
        
        if(idex.CW.RegWrite)
        {
            busy[to_decimal(idex.rdl)]=true;
            
        }
        return true;
    }
    
    
    void execute()
    {
        string ALUSelect=ALUControl(idex.CW.ALUOp, IM->instruction(25, 31, idex.DPC), idex.func3);
        ALU alu(ALUSelect, idex.rs1, idex.rs2);
        exmo.ALUOut=alu.ALUresult;
        int ALUZeroFlag=alu.ALUZeroFlag;
        exmo.rs2=idex.SD_val;
        exmo.CW=idex.CW;
        exmo.rdl=idex.rdl;
        exmo.DPC=idex.DPC;
        
        int bpc=((to_decimal(SignedExtend(idex.imm)))<<1)+idex.DPC;
        
        int ppc=PC;
        
        if(idex.CW.Branch && ALUZeroFlag)
        {
            NPC=bpc;
            PC=NPC;
            vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
            
        }
        else
        {
            NPC=TPC;
            if(idex.CW.Branch)
            {
                PC=NPC;
                vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
            }
        }
        
        if(idex.CW.jump)
        {
            NPC=idex.JPC;
            PC=NPC;
            vis_reset(fetch_vis, decode_vis, execute_vis, MemOp_vis, writeBack_vis, PC);
            
        }
    }
    
    void MemOp()
    {
        if(exmo.CW.MemWrite)
        {
            DM[exmo.ALUOut]=exmo.rs2;
        }
        if(exmo.CW.MemRead)
        {
            if(DM.find(exmo.ALUOut)!=DM.end())
                mowb.LDOut=DM[exmo.ALUOut];
        }
        mowb.DPC=exmo.DPC;
        
        mowb.ALUOut=exmo.ALUOut;
        mowb.CW=exmo.CW;
        mowb.rdl=exmo.rdl;
    }
    
    void writeBack()
    {
        if(mowb.CW.RegWrite)
        {
            if(mowb.CW.Mem2Reg)
            {
                GPR[to_decimal(mowb.rdl)]=mowb.LDOut;
            }
            else
            {
                GPR[to_decimal(mowb.rdl)]=mowb.ALUOut;
            }
            busy[to_decimal(mowb.rdl)]=false;
        }
    }
    
    void run()
    {
        int n=IM->IM.size();
        fetch_vis=vector<bool>(4*n, false);
        decode_vis=vector<bool> (4*n, false);
        execute_vis=vector<bool> (4*n, false);
        MemOp_vis=vector<bool> (4*n, false);
        writeBack_vis=vector<bool> (4*n, false);
        int cycles=0;
        while(true)
        {
            cycles++;
            if(!writeBack_vis[mowb.DPC] && MemOp_vis[mowb.DPC])
            {
                writeBack_vis[mowb.DPC]=true;
                writeBack();
            }
            if(!MemOp_vis[exmo.DPC] && execute_vis[exmo.DPC])
            {
                MemOp_vis[exmo.DPC]=true;
                MemOp();
            }
            
            if(!execute_vis[idex.DPC] && decode_vis[idex.DPC])
            {
                execute_vis[idex.DPC]=true;
                execute();
            }
            
            if(!decode_vis[ifid.DPC] && fetch_vis[ifid.DPC])
            {
                if(decode())
                {
                    decode_vis[ifid.DPC]=true;
                }
                else{
                    continue;
                }
            }
            
            if(!fetch_vis[PC] && PC<4*n)
            {
                fetch_vis[PC]=true;
                fetch();
            }
            
            if(writeBack_vis[4*n-4])
            {
                break;
            }
        }
        

        cout<<"GPR values:\n";
        for(int i=0; i<32; i++)
        {
            cout<<"x"<<i<<": "<<GPR[i]<<endl;
        }

        cout<<"No. of cycles: "<<cycles<<endl;
        
        cout<<"\n\nData Memory\n";
        for(auto it=DM.begin(); it!=DM.end(); ++it)
        {
            cout<<"Address: "<<it->first<<", Value: "<<it->second<<endl;
        }
    }
};


int main()
{
    cout<<"Machine Code\n";
    ofstream outFile("machine.txt");
    read_from_file("sum_n_nums.txt", outFile);
    outFile.close();
    freopen("machine.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    cout<<"\n\n\n\n\n\n\n\n\nCPU Design"<<endl;
    // MACHINE_CODE.push_back("100010011")
    
    
    MACHINE_CODE.push_back("00000000000000000000000000010011");
    InstructionMemory IM(MACHINE_CODE);
    CPU cpu(&IM);
    cpu.run();
    return 0;
}




































