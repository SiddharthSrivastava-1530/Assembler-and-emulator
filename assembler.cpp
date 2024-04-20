#include <bits/stdc++.h>
using namespace std;
#define fastio                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);
map<string, int> label_pc;
vector<pair<int, string>> errors;
vector<pair<string,string>>opop;
map<int,vector<string>>commands;
map<int,int>indices;
map<string, string> mnemonic_opcode;
map<string, int> mnemonic_operand;
long long power(long long x, long long y) {
    if (y == 0)
        return 1;
    long long res = power(x, y / 2);
    if (y % 2)
        return res * res * x;
    else
        return res * res;
}
void remove_comments(string &x)
{
    if (x.find(';') != string::npos)
    {
        x = x.substr(0, x.find(';'));
    }
}
void remove_spaces(string &s)
{
    int st = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != ' ')
        {
            st = i;
            break;
        }
    }
    int end = s.length()-1;
    for (int i = s.length() - 1; i >= 0; i--)
    {
        if (s[i] != ' ')
        {
            end = i;
            break;
        }
    }
    string x = "";
    for (int i = st; i <= end; i++)
    {
        x += s[i];
    }
    s = x;
}
bool isvalid(string &label)
{
    if (isalpha(label[0]) == 0)
        return false;
    for (int i = 1; i < label.length(); i++)
    {
        if (isalpha(label[i] == 0) && (label[i] < '0' || label[i] > '9'))
            return false;
    }
    return true;
}
int tobin(string x)
{
    
    int sum=0;
    for(int i=1;i<x.length();i++)
    {
        if(x[i]=='1')
        {
            sum+=power(2,x.length()-1-i);
        }
    }
    if(x[0]=='1')
    sum*=-1;
    return sum;
}
int insertlabel(string x, int pc)
{
    // the parameter is trimmed before passing it to the function
    if (!isvalid(x))
        return 0;
    if (label_pc.find(x) != label_pc.end())
    {
        return 1;
    }
    label_pc[x] = pc;
    return 2;
}

string hexaconvert(int x, int requiredlength) {
    std::stringstream ss;
    ss << std::hex << x;
    std::string result = ss.str();

   
    if (x < 0) {
        
        string positive_hex = result.substr(1);
        
        while (positive_hex.length() < (requiredlength - 1)) {
            positive_hex = "f" + positive_hex;
        }
        
        result = "f" + positive_hex;
    } else {
        while (result.length() < requiredlength) {
            result = "0" + result;
        }
    }
    if(result.length()>requiredlength)
    {
        reverse(result.begin(),result.end());
        result=result.substr(0,requiredlength);
        reverse(result.begin(),result.end());
    }
    return result;
}
string hexopcode(int x)
{
    int copy=x;
    x=abs(x);
    string ret="";
    while(x!=0)
    {
        if(x%16==10)
        {
            ret="a"+ret;
        }
        else if(x%16==11)
        {
            ret="b"+ret;
        }
        else if(x%16==12)
        {
            ret="c"+ret;
        }
        else if(x%16==13)
        {
            ret="c"+ret;
        }
        else if(x%16==14)
        {
            ret="d"+ret;
        }
        else if(x%16==15)
        {
            ret="f"+ret;
        }
        else
        {
            string ch="";
            ch+=('0'+x%16);
            ret=ch+ret;
        }
        x/=16;
    }
    while(ret.length()<2)
    ret="0"+ret;
    return ret;
}
int decode_number(string x, int line)
{
    if (label_pc.find(x) != label_pc.end())
        return label_pc[x];
    const char *sptr = x.c_str();
    char *eptr;
    int num;
    if (x.length() > 1 && x[0] == '0')
    {
        if (x[1] == 'b')
        {
            num = strtol(sptr + 2, &eptr, 2);
        }
        else if (x[1] == 'x')
        {
            num = strtol(sptr + 2, &eptr, 16);
        }
        else
            num = strtol(sptr, &eptr, 8);
    }
    else
    {
        num = strtol(sptr, &eptr, 10);
    }
    if (eptr - sptr != x.length())
    {
        if (isvalid(x))
        {
            errors.push_back({line, "Such label doesn't exist: " + x});
        }
        else if (x[0] >= '0' && x[0] <= '9')
        {
            errors.push_back({line, "Invalid operand: " + x});
        }
        else
        {
            errors.push_back({line, "Invalid instruction: " + x});
        }
    }
    return num;
}
void routine_pass1(string inputfile)
{
    string command;
    ifstream inputf(inputfile);
    int counter = 0;
    int line = 0;
    while (getline(inputf, command))
    {
        line++;
        
        remove_spaces(command);
        remove_comments(command);
        remove_spaces(command);
        if(command.empty())
        continue;
        commands[counter].push_back(command);
        string label = "";
        string instruction = "";
        string operand = "";
        int colon = -1;
       if(command.find(':')!=string::npos)
       {
        int colon = command.find(':');
			label = command.substr(0, colon);
			 remove_spaces(label);
			command = command.substr(colon+1, command.length()-colon-1);
			remove_spaces(command);
       }
        if (command.find(' ') != string::npos){
			int space = command.find(' ');
			instruction = command.substr(0, space);
			remove_spaces(instruction);
			operand = command.substr(space+1, command.length()-space-1); 
		  remove_spaces(operand);
		} 
        else{
			 remove_spaces(command);
             instruction=command;
		}
        remove_spaces(instruction);
        remove_spaces(label);
        remove_spaces(operand);
        
        if (instruction == "SET" && !label.empty())
        {
            int current_counter = decode_number(operand, line);
            int x = insertlabel(label, current_counter);
            if (x == 0&&!label.empty())
            {
                errors.push_back({line, "Invalid label: "+label});
            }
            else if (x == 1&&!label.empty())
            {
                errors.push_back({line, "Duplicate label: "+label});
            }
        }
        else if (!label.empty())
        {
            int x = insertlabel(label, counter);
            if (x == 0&&!label.empty())
            {
                errors.push_back({line, "Invalid label: "+label});
            }
            else if (x == 1&&!label.empty())
            {
                errors.push_back({line, "Duplicate label: "+label});
            }
        }
        if (instruction.empty() && operand.empty())
            continue;
       
        mnemonic_opcode["ldc"] = "00000000";
        mnemonic_opcode["adc"] = "00000001";
        mnemonic_opcode["ldl"] = "00000010";
        mnemonic_opcode["stl"] = "00000011";
        mnemonic_opcode["ldnl"] = "00000100";
        mnemonic_opcode["stnl"] = "00000101";
        mnemonic_opcode["add"] = "00000110";
        mnemonic_opcode["sub"] = "00000111";
        mnemonic_opcode["shl"] = "00001000";
        mnemonic_opcode["shr"] = "00001001";
        mnemonic_opcode["adj"] = "00001010";
        mnemonic_opcode["a2sp"] = "00001011";
        mnemonic_opcode["sp2a"] = "00001100";
        mnemonic_opcode["call"] = "00001101";
        mnemonic_opcode["return"] ="00001110";
        mnemonic_opcode["brz"] = "00001111";
        mnemonic_opcode["brlz"] = "00010000";
        mnemonic_opcode["br"] = "00010001";
        mnemonic_opcode["HALT"] = "00010010";
        mnemonic_opcode["data"] = "10000000";
        mnemonic_opcode["SET"] = "10000001";
        mnemonic_operand["ldc"] = 1;
        mnemonic_operand["adc"] = 1;
        mnemonic_operand["ldl"] = 1;
        mnemonic_operand["stl"] = 1;
        mnemonic_operand["ldnl"] = 1;
        mnemonic_operand["stnl"] = 1;
        mnemonic_operand["add"] = 0;
        mnemonic_operand["sub"] = 0;
        mnemonic_operand["shl"] = 0;
        mnemonic_operand["shr"] = 0;
        mnemonic_operand["adj"] = 1;
        mnemonic_operand["a2sp"] = 0;
        mnemonic_operand["sp2a"] = 0;
        mnemonic_operand["call"] = 1;
        mnemonic_operand["return"] = 0;
        mnemonic_operand["brz"] = 1;
        mnemonic_operand["brlz"] = 1;
        mnemonic_operand["br"] = 1;
        mnemonic_operand["HALT"] = 0;
        mnemonic_operand["data"] = 1;
        mnemonic_operand["SET"] = 1;
        if(mnemonic_opcode.find(instruction)==mnemonic_opcode.end())
        {
            if(!instruction.empty())
            errors.push_back({line,"Invalid mnemonic: "+instruction});
        }
        else if(!operand.length()&&mnemonic_operand[instruction])
        {
            if(!instruction.empty())
            errors.push_back({line,"Operand is missing: "+instruction});
        }
        else if(operand.length()&&mnemonic_operand[instruction]==0)
        {
            if(!instruction.empty())
            errors.push_back({line," Unexpected operand encountered: "+instruction+" "+operand});
        }
        if(instruction!="SET")
        {
            indices[counter]=line;
        }
        counter++;
        //cout<<instruction<<" "<<operand<<endl;
        opop.push_back({instruction, operand});
    }
    
}
void routine_pass2(ofstream &logf, ofstream &outf, ofstream &objf)
{
    vector<pair<int,int>>opcode_operand;
    for(int i=0;i<opop.size();i++)
    {
        int line=indices[i];
        string instruction=opop[i].first;
        string operand=opop[i].second;
        string opc=mnemonic_opcode[instruction];
       
        int opcode=tobin(opc);
         //cout<<instruction<<" "<<opc<<" "<<opcode<<endl;
        if(!operand.length())
        {
            operand="0";
        }
        int operand2;
        if(mnemonic_operand[instruction])
        {
            operand2=decode_number(operand,line);
        }
        else
        {
            operand2=0;
        }
        if(instruction=="data"){
        opcode=operand2&0x000000ff;
        operand2>>=8;
        }
        opcode_operand.push_back({opcode,operand2});
    }
    if(!errors.empty())
    {
        cout<<"Errors detected: check the log file"<<endl;
        sort(errors.begin(),errors.end());
        for(auto it:errors)
        {
            logf<<"Error Line :"<<it.first<<" "<<it.second<<endl;
        }
        return;
    }
    else
    {
        cout<<"No Errors Found: Compiled successfully "<<endl;
    }
    int counter=0;
    set<int>offset={13,15,16,17};
    for(counter=0;counter<opcode_operand.size();counter++)
    {
        outf<<hexaconvert(counter,8)<<" ";
        for(int i=0;i<commands[counter].size()-1;i++)
        {
            string x=commands[counter][i];
            outf << "\t\t " << " " << x <<" ";
    		outf << hexaconvert(counter, 8) << endl;
        }
        int opc=opcode_operand[counter].first;
        int operand=opcode_operand[counter].second;
        if(offset.find(opc)!=offset.end())
        {
            operand-=counter+1;
        }
        
        outf<<hexaconvert(operand, 6) << hexopcode(opc) << " " << commands[counter][commands[counter].size()-1] << "\n";

        objf<<hexaconvert(operand, 6) << hexopcode(opc)<<endl;
    }

}
int main(int argc, char* argv[])
{
    if(argc < 2){
        cout<<"Give your input in the following format= ./assembler file.asm .";
        cout<<"Here file.asm is the file that is to be assembled."<<endl;
        return 0;
    }
    string inf = string(argv[1]);
    routine_pass1(inf);
    ofstream outf("outf.L");
    ofstream logf("logf.log");
    ofstream objf("objf.o",ios::out|ios::binary);
    routine_pass2(logf,outf,objf);
}