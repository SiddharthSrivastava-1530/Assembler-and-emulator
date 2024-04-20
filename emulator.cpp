#include <bits/stdc++.h>
using namespace std;
typedef int32_t bigint;
#define fastio                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);
map<string, string> opcode_instruction;
//Function definitions
void initialize();
long long power(long long x, long long y);
int tobin(string x);
void instruction_set_archi();
string hexaconvert(int x);
void memory_dump(ofstream &tf, int program_counter);
void trace(ofstream &tf, int program_counter);
void display(ofstream &tf, string &opc, bigint &operand);
void ldc(bigint &operand);
void adc(bigint &operand);
void ldl(bigint &operand);
void stl(bigint &operand);
void ldnl(bigint &operand);
void stnl(bigint &operand);
void add();
void sub();
void shl();
void shr();
void adj(bigint &operand);
void a2sp();
void sp2a();
void call(bigint &operand);
void ret(bool &halting);
void brz(bigint &operand);
void brlz(bigint &operand);
void br(bigint &operand);
void halt(bool &halting);

void initialize()
{
    opcode_instruction["00000000"] = "ldc";
    opcode_instruction["00000001"] = "adc";
    opcode_instruction["00000010"] = "ldl";
    opcode_instruction["00000011"] = "stl";
    opcode_instruction["00000100"] = "ldnl";
    opcode_instruction["00000101"] = "stnl";
    opcode_instruction["00000110"] = "add";
    opcode_instruction["00000111"] = "sub";
    opcode_instruction["00001000"] = "shl";
    opcode_instruction["00001001"] = "shr";
    opcode_instruction["00001010"] = "adj";
    opcode_instruction["00001011"] = "a2sp";
    opcode_instruction["00001100"] = "sp2a";
    opcode_instruction["00001101"] = "call";
    opcode_instruction["00001110"] = "return";
    opcode_instruction["00001111"] = "brz";
    opcode_instruction["00010000"] = "brlz";
    opcode_instruction["00010001"] = "br";
    opcode_instruction["00010010"] = "HALT";
    opcode_instruction["10000001"] = "data";
    opcode_instruction["10000010"] = "SET";
}
long long power(long long x, long long y)
{
    // function to calculate x raised to power y
    if (y == 0)
        return 1;
    long long res = power(x, y / 2);
    if (y % 2)
        return res * res * x;
    else
        return res * res;
}
int tobin(string x)
{
    // function to convert number from its binary form to decimal form
    int sum = 0;
    for (int i = 1; i < x.length(); i++)
    {
        if (x[i] == '1')
        {
            sum += power(2, x.length() - 1 - i);
        }
    }
    if (x[0] == '1')
        sum *= -1;
    return sum;
}
string toint(int x)
{
    // function to convert number from its decimal format to binary format
    string ret = "";
    int copy = x;
    x = abs(x);
    while (x != 0)
    {
        if (x % 2)
        {
            ret = "1" + ret;
        }
        else
        {
            ret = "0" + ret;
        }
        x /= 2;
    }
    while (ret.length() < 7)
    {
        ret = "0" + ret;
    }
    if (copy < 0)
    {
        ret = "1" + ret;
    }
    else
    {
        ret = "0" + ret;
    }
    return ret;
}
vector<bigint> mem_tab(10000); // represents memory
// represents register a , register b , program counter and stack pointer
bigint reg1 = 0, reg2 = 0, program_counter = 0, stack_pointer = 9999;
void instruction_set_archi()
{
    // display the instruction set architecture
    cout << "Opcode Mnemonic Operand\n";
    cout << "Instruction=data     operand=value" << endl;
    cout << "Instruction=ldc      operand=value      opcode=0" << endl;
    cout << "Instruction=adc      operand=value      opcode=1" << endl;
    cout << "Instruction=ldl      operand=value      opcode=2" << endl;
    cout << "Instruction=stl      operand=value      opcode=3" << endl;
    cout << "Instruction=ldnl     operand=value      opcode=4" << endl;
    cout << "Instruction=stnl     operand=value      opcode=5" << endl;
    cout << "Instruction=add                         opcode=6" << endl;
    cout << "Instruction=sub                         opcode=7" << endl;
    cout << "Instruction=shl\n                       opcode=8" << endl;
    cout << "Instruction=shr\n                       opcode=9" << endl;
    cout << "Instruction=adj      operand=value      opcode=10" << endl;
    cout << "Instruction=a2sp                        opcode=11" << endl;
    cout << "Instruction=sp2a                        opcode=12" << endl;
    cout << "Instruction=call     operand=offset     opcode=13" << endl;
    cout << "Instruction=return                      opcode=14" << endl;
    cout << "Instruction=brz      operand=offset     opcode=15" << endl;
    cout << "Instruction=brlz     operand=offset     opcode=16" << endl;
    cout << "Instruction=br       operand=offset     opcode=17" << endl;
    cout << "Instruction=HALT                        opcode=18" << endl;
    cout << "Instruction=SET      operand=value" << endl;
}

string hexaconvert(int x)
{
    // convert a number from into its hexadecimal format
    int requiredlength = 8;
    ostringstream outputStringStream;

    outputStringStream << hex << setw(requiredlength) << setfill('0') << x;
    string result = outputStringStream.str();

    // ensures that result is exactly of length=requiredlength
    result = result.substr(0, requiredlength);
    return result;
}
void display(ofstream &tf, string &opc, bigint &operand)
{
    tf << "reg1: " << hexaconvert(reg1) << "    reg2: " << hexaconvert(reg2) << "    stack_pointer: " << hexaconvert(stack_pointer)
       << "    program_counter: " << hexaconvert(program_counter) << "    " << opcode_instruction[opc] << " " << operand << endl
       << endl;

    cout << "reg1: " << hexaconvert(reg1) << "    reg2: " << hexaconvert(reg2) << "    stack_pointer: " << hexaconvert(stack_pointer)
         << "    program_counter: " << hexaconvert(program_counter) << "    " << opcode_instruction[opc] << " " << operand << endl
         << endl;
}
void memory_dump(ofstream &tf, int program_counter)
{
    cout << "\n\t Memory dump " << endl;
    tf << "\n\t---Memory dump  " << endl;
    for (int i = 0; i < program_counter; i++)
    {
        // display 4 bytes in one row
        if (i % 4)
        {
            cout << hexaconvert(mem_tab[i]) << " ";
            tf << hexaconvert(mem_tab[i]) << " ";
        }
        else
        {
            // write to console
            cout << "\n"
                 << hexaconvert(i) << "\t" << hexaconvert(mem_tab[i]) << " ";
            // write to trace file
            tf << "\n"
               << hexaconvert(i) << "\t" << hexaconvert(mem_tab[i]) << " ";
        }
    }
    cout << endl;
}
void ldc(bigint &operand)
{
    reg2 = reg1;
    reg1 = operand;
}
void adc(bigint &operand)
{
    reg1 += operand;
}
void ldl(bigint &operand)
{
    reg2 = reg1;
    reg1 = mem_tab[stack_pointer + operand];
}
void stl(bigint &operand)
{
    mem_tab[stack_pointer + operand] = reg1;
    reg1 = reg2;
}
void ldnl(bigint &operand)
{
    reg1 = mem_tab[reg1 + operand];
}
void stnl(bigint &operand)
{
    mem_tab[reg1 + operand] = reg2;
}
void add()
{
    reg1 += reg2;
}
void sub()
{
    reg1 = reg2 - reg1;
}
void shl()
{
    reg1 = reg2 << reg1;
}
void shr()
{
    reg1 = reg2 >> reg1;
}
void adj(bigint &operand)
{
    stack_pointer += operand;
}
void a2sp()
{
    stack_pointer = reg1;
    reg1 = reg2;
}
void sp2a()
{
    reg2 = reg1;
    reg1 = stack_pointer;
}
void call(bigint &operand)
{
    reg2 = reg1;
    reg1 = program_counter;
    program_counter += operand;
}
void ret(bool &halting)
{
    if (program_counter == reg1 && reg1 == reg2)
    {
        halting = true;
    }
    program_counter = reg1;
    reg1 = reg2;
}
void brz(bigint &operand)
{
    if (reg1 == 0)
    {
        program_counter += operand;
    }
}
void brlz(bigint &operand)
{
    if (reg1 < 0)
    {
        program_counter += operand;
    }
}
void br(bigint &operand)
{
    program_counter += operand;
}
void halt(bool &halting)
{
    halting = true;
}
void trace(ofstream &tf, int program_counter)
{
    cout << "\n\t\t---Tracing instructions---\t\t\n\n";
    tf << "\n\t\t---Tracing instruction---\t\t\n\n";

    set<int> offset_program_counter;
    offset_program_counter.insert(13);
    offset_program_counter.insert(15);
    offset_program_counter.insert(16);
    offset_program_counter.insert(17);

    int Line = 0; // represents line number

    bool halting = false;
    // loop runs as long as variable halting remains false
    for (; !halting;)
    {
        bigint instruction = mem_tab[program_counter]; // extract instruction from memory

        // for calculation of negative numbers in hexadecimal
        std::string hex_string = "ffffffff";
        unsigned long hexval;
        std::stringstream ss;
        ss << std::hex << hex_string; // inject the maximum hexadecimal value into the string stream that interprets it as a hex value
        ss >> hexval;                 // extract the maximum hex value from stringstream

        bigint maximum_hexval = (bigint)(hexval); // typecast
        // maximum opcode value
        int maximum_opcode = stoi("ff", nullptr, 16);

        // extraction of opcode
        bigint temp_cd = (instruction & 0x000000ff);
        string temporary_subs = hexaconvert(temp_cd);
        temporary_subs = temporary_subs.substr(6, 2);

        bigint op_code = stoi(temporary_subs, nullptr, 16);

        // conversion to two's complement
        if (temporary_subs[0] >= '8')
        {
            op_code = -(maximum_opcode - op_code + 1);
        }
        // extraction of operand
        bigint operand = instruction & 0xffffff00;

        // conversion to two's complement
        if ((operand >> 31) & (1))
        {
            operand = -(maximum_hexval - operand + 1);
        }
        operand >>= 8;
        string opc = toint(op_code); // conversion of opcode into its binary format
        if (opcode_instruction.find(opc) != opcode_instruction.end())
        {
            display(tf, opc, operand); // displays the contents of reg1 and reg2 and program_counter and stack_pointer
        }
        else
        {
            halting = true;
        }
        // execute the instruction stored in instruction memory, as instructed in the mini project file. Self explanatory.
        if (op_code == 0)
        {
            ldc(operand);
        }
        else if (op_code == 1)
        {
            adc(operand);
        }
        else if (op_code == 2)
        {
            ldl(operand);
        }
        else if (op_code == 3)
        {
           stl(operand);
        }
        else if (op_code == 4)
        {
            ldnl(operand);
        }
        else if (op_code == 5)
        {
            stnl(operand);
        }
        else if (op_code == 6)
        {
            add();
        }
        else if (op_code == 7)
        {
            sub();
        }
        else if (op_code == 8)
        {
            shl();
        }
        else if (op_code == 9)
        {
            shr();
        }
        else if (op_code == 10)
        {
            adj(operand);
        }
        else if (op_code == 11)
        {
            a2sp();
        }
        else if (op_code == 12)
        {
            sp2a();
        }
        else if (op_code == 13)
        {
            call(operand);
        }
        else if (op_code == 14)
        {
           ret(halting);
        }
        else if (op_code == 15)
        {
           brz(operand);
        }
        else if (op_code == 16)
        {
           brlz(operand);
        }
        else if (op_code == 17)
        {
           br(operand);
        }
        else if (op_code == 18)
        {
           halt(halting);
        }

        if (stack_pointer >= 10000)
        {
            cout << " Memory limit exceeded by stack pointer at :" << program_counter << endl;// memory limit exceeds
            halting = true;
        }
        if (offset_program_counter.count(op_code) && operand == -1)
        {
            cout << "Infinite Loop detected! " << endl;
            halting = true;
        }
        // if program is instructed to halt, break the loop
        if (halting)
            break;
        if (!halting)
            program_counter++;
        Line++;
    }
    cout << Line << " number of instructions have been executed!" << endl;
}
int main(int argc, char *argv[])
{

    initialize();

    if (argc < 3)
    {
        cout << R"(Enter your input in the followng format- ./emulator (option) file.o. Here file.o is the object file and option is one of the following- \n
    -before : Show mem_tab dump before program execution.
    -after : Show mem_tab dump after program execution.
    -trace : Show trace of the executed instruction.
    -ISA    : Display the instruction set architecture)"
             << endl;
        return 0;
    }

    string inputfile = string(argv[2]);
    string trace_f = inputfile.substr(0, inputfile.find('.')) + ".trace";
    ifstream input_file(inputfile, ios::out | ios::binary);
    ofstream output_file(trace_f);
    string command;
    int Line = 0;

    while (getline(input_file, command))
    {
        if (command.empty())
            continue;

        // for calculation of negative numbers in hexadecimal
        int maximum_operand = stoi("ffffff", nullptr, 16);
        int maximum_opcode = stoi("ff", nullptr, 16);

        for (int start = 0; start < command.length(); start += 8)
        {
            //extraction of opcode and operand and converting them into their two's complement if necessary
            string commandCode = command.substr(start, 8);
            cout << commandCode << endl;
            unsigned long hexval = stoul(commandCode, nullptr, 16);
            bigint instruction = (bigint)(hexval);
            string temp_cd = commandCode.substr(6, 2);
            bigint op_code = stoi(temp_cd, nullptr, 16);
            if (temp_cd[0] >= '8')
            {
                op_code = -(maximum_opcode - op_code + 1);
            }

            temp_cd = commandCode.substr(0, 6);
            bigint operand = stoi(temp_cd, nullptr, 16);
            if (temp_cd[0] >= '8')
            {
                operand = -(maximum_operand - operand + 1);
            }

            if (op_code < 0)// edge case handling for data and SET instructions
            {
                mem_tab[Line] = operand;
            }
            else
            {
                mem_tab[Line] = instruction;
            }
            Line++;
        }
    }

    // Dumping before execution
    if (string(argv[1]) == "-before")
    {
        memory_dump(output_file, Line);
    }

    // Tracing each instruction
    if (string(argv[1]) == "-trace")
    {
        trace(output_file, program_counter);
    }

    // Dumping after execution
    if (string(argv[1]) == "-after")
    {
        trace(output_file, program_counter);
        memory_dump(output_file, Line);
    }
    if (string(argv[1]) == "-isa")
        instruction_set_archi();

    return 0;
}
