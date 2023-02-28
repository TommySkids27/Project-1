#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
#include <iomanip>
#include "Project1.hpp"

std::string ConverttoBinary(std::string line);
std::string addLeadingZeroes(std::string binaryStr);
std::string FIND_IN_MAP(std::string opcodeStr, std::map<std::string, std::string> MAP, int address);
std::string twosCompliment(std::string binaryStr);
std::string decimalToHex(unsigned int decimalValue);
void GENERATE_LABEL(int address, std::string IMM);

bool ERROR = false; //error flag

int main() {
     std::string FileName;
     std::cout << "Enter file name with suffix: ";
     std::cin >> FileName;
    std::ifstream inputFile(FileName); // open file for reading
    std::ofstream outfile("proj1_skidmore.asm", std::ios::trunc);
    //std::ifstream inputFile("test_case4.obj");
    std::string line;
    std::string RTYPE = "000000";
    int address = 0;
  
     if (inputFile.is_open()) {
        address = 2; 
        while (std::getline(inputFile, line)) { // read line by line
            std::string BINARY; //std::string FINISH_HEX;
            BINARY = ConverttoBinary(line);
            std::string OPCODE = BINARY.substr(0, 6);
        if(line.length() != 8) {
            std::cout << "Cannot disassemble " << line << " at line " << address-1 << std::endl;
            exit(0);//if a line is not 8 characters in length, throw error and terminate program
        }
        if (OPCODE == "000100" || OPCODE == "000101"){ //if opcode = bne or beq
             std::string IMM = BINARY.substr(16, 16); //isolate immediate value (I-type)
             GENERATE_LABEL(address, IMM); //generate the label and store in label map
        }
        address++; //increment address for every new line
        }
        inputFile.clear(); //clear end of file flag
        inputFile.seekg(0, std::ios::beg);// set the position of the file pointer back to the beginning of the file
        address = 1;
        while (std::getline(inputFile, line)) { // read line by line
           if (!labels.empty()) {
            std::map<int, std::string>::iterator it;
            for (it = labels.begin(); it != labels.end(); ++it) {
                if (it->first == address) {
                    outfile << it->second << std::endl;
                 }
            }
           }
            std::string BINARY; //std::string FINISH_HEX;
            BINARY = ConverttoBinary(line);
            std::string OPCODE = BINARY.substr(0, 6);
            std::string INSTRUCTION;
             if(OPCODE == RTYPE) {
                 std::string MNEM = FIND_IN_MAP(BINARY.substr(26, 6), R_FUNCT, address);
                 std::string RT = FIND_IN_MAP(BINARY.substr(11, 5), REGISTERS, address);
                 std::string RD = FIND_IN_MAP(BINARY.substr(16, 5), REGISTERS, address);
                 std::string RS = FIND_IN_MAP(BINARY.substr(6, 5), REGISTERS, address);
            
                 if (MNEM == "srl" || MNEM == "sll") {
                    std::bitset<8> bits(BINARY.substr(21, 5));
                    std::string SHAMT = std::to_string(bits.to_ulong()); 
                    INSTRUCTION = "\t" + MNEM + " " + RD + ", "  + RT + ", " + SHAMT;
                }
                else {
                     INSTRUCTION = "\t" + MNEM + " " + RD + ", " + RS + ", " + RT;
                }
            }
            else { //else I-type instruction
                 std::string OP = FIND_IN_MAP(OPCODE, I_OPCODES, address);
                 std::string RS = FIND_IN_MAP(BINARY.substr(6, 5), REGISTERS, address);
                 std::string RT = FIND_IN_MAP(BINARY.substr(11, 5), REGISTERS, address);
                 std::string IMM = BINARY.substr(16, 16); 
                if(OP == "lw" || OP == "sw") {
                    std::bitset<16> bits(IMM);
                    std::string decimal_IMM = std::to_string(bits.to_ulong());
                    INSTRUCTION = "\t" + OP + " " + RT + ", " + decimal_IMM + "(" + RS + ")"; 
                }
                else if(OP == "bne" || OP == "beq") {
                    std::string LABEL; std::string OFFSET;
                    int target_address;
                    if(IMM[0] == '1') {
                            int offset = std::stoi(twosCompliment(IMM)); // convert 2-compliment string to integer
                            target_address = address - offset; //the target address to place the label
                    }
                     else {
                             std::bitset<16> bitset(IMM); //2-compliment conversion is not required
                             int offset = bitset.to_ulong();
                             target_address = address + offset; //add the postive offset to current address
                        }
                    std::string address_label = "Addr_" + decimalToHex((target_address) * 4); //create addr header
                    INSTRUCTION = "\t" + OP + " " + RT + ", " + RS + ", " + address_label; //print instruction
                }
                else {
                    std::bitset<16> bits(IMM);
                    std::string decimal_IMM = std::to_string(bits.to_ulong());
                     INSTRUCTION = "\t" + OP + " " + RT + ", " + RS + ", " + decimal_IMM; //ptherwise "normal" I-type format
                }     
            }
            outfile << INSTRUCTION << std::endl; //add instruction to new line of .asm file
            address++; //increment address each line
        }
        inputFile.close(); 
        outfile.close();  // close files
     }
    else {
         std::cout << "Error opening file." << std::endl;
    }
    std::cout << "SUCCESS" << std::endl;
     return 0;
}


std::string ConverttoBinary(std::string line) {
    std::string binaryString = ""; //declare empty string 
    uint64_t DecValue = std::stoull(line, nullptr, 16); // convert the string (the hex value) into a unsigned 64 bit integer (decimal)
    //note uint64_t was used to handle large hex values that would overflow an int 
    for (std::size_t i = 0; DecValue > 0; i++) {
        binaryString = std::to_string(DecValue % 2) + binaryString; //converts DecValue into string and appends the binary string
        DecValue /= 2; //convert decimal value in string into a binary value
    }
    if (binaryString.length() < 32) { //if string is less than 32 characters in length, additional 0s should be added to front
        binaryString = addLeadingZeroes(binaryString); //add zeroes to binaryString from function call
    }
    return binaryString;
}

std::string addLeadingZeroes(std::string binaryStr) {
    int numZeroes = 32 - binaryStr.length();  // numZeroes is the number of zeroes needed to add
    std::string leadingZeroes(numZeroes, '0');  // Create a string of zeroes, with the number of zeroes needed
    return leadingZeroes + binaryStr;  // Concatenate the two strings   
}

std::string FIND_IN_MAP(std::string opcodeStr, std::map<std::string, std::string> MAP, int address) {
    
    // Iterate over the map and look for the opcode
    std::map<std::string, std::string>::iterator it;
    for (it = MAP.begin(); it != MAP.end(); ++it) {
        if (it->second == opcodeStr) { // Opcode found, return the instruction name
        return it->first;
        }
    }
    std::string ADDRESS = std::to_string(address);
    std::string ERROR = "Cannot disassemble " + opcodeStr + " at line " + ADDRESS; // Opcode not found, print the opcode and return error message for outfile
    std::cout << ERROR << std::endl;
    exit(0);
    //return "";
}
std::string twosCompliment(std::string binaryStr) {
    for(std::size_t i = 0; i < binaryStr.length(); i++){
         if (binaryStr[i] == '0') {
            binaryStr[i] = '1';
         } else {
            binaryStr[i] = '0';
         }
    } //flip all of the bits 
    bool carry = true;
    for (int i = binaryStr.length() - 1; i >= 0; i--) {
        if (binaryStr[i] == '0' && carry) {
            binaryStr[i] = '1';
            carry = false;
        } else if (binaryStr[i] == '1' && carry) {
            binaryStr[i] = '0';
        }
    }
    
    // convert binary number to string and return it
    std::bitset<16> bitset(binaryStr);
    int decimalValue = bitset.to_ulong();
    return std::to_string(decimalValue);
}

std::string decimalToHex(unsigned int decimalValue) {
    std::stringstream ss;
    ss << std::hex << std::setw(4) << std::setfill('0') << decimalValue;
    return ss.str(); //converts an unsigned int into a hex representation of that value as a string (for addr_label)
}
void GENERATE_LABEL(int address, std::string IMM) {
            std::string OFFSET;
            int target_address;
            if(IMM[0] == '1') {
                int offset = std::stoi(twosCompliment(IMM)); // convert string to integer
                target_address = address - offset; //print label_line
            }
            else {
                std::bitset<16> bitset(IMM);
                int offset = bitset.to_ulong();
                target_address = address + offset;
            }
            std::string address_label = "Addr_" + decimalToHex((target_address-1) * 4) + ":";
            std::string TARGET_ADDRESS = std::to_string(target_address);
            labels[target_address] = address_label; //generates the label and stores the address_label with the line it needs to be printed on
}
        