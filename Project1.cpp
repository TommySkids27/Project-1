#include <iostream>
#include <fstream>
#include <bitset>
#include "Project1.hpp"

std::string ConverttoBinary(std::string line);
std::string addLeadingZeroes(std::string binaryStr);
std::string FIND_IN_MAP(std::string opcodeStr, std::map<std::string, std::string> MAP);
std::string twosCompliment(std::string binaryStr);
int main() {
    //std::string FileName;
    //std::cout << "Enter file name with suffix: " << std::endl;
    //std::cin >> FileName;
    //std::ifstream inputFile(FileName); // open file for reading
    std::ifstream inputFile("test_case3.obj");
    std::ofstream outfile("proj1_skidmore.asm", std::ios::trunc);
    std::string line;
    std::string RTYPE = "000000";
    int address = 0;


    std::map<int, std::string> labels;
     if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) { // read line by line
        std::string BINARY; //std::string FINISH_HEX;
        BINARY = ConverttoBinary(line);
        std::string OPCODE = BINARY.substr(0, 6);
        if (OPCODE == "000100" || OPCODE == "000101"){
            std::string IMM = BINARY.substr(16, 16);
            std::string B_OFFSET;
            if(IMM[0] == '1') {
                B_OFFSET = "-" + twosCompliment(IMM);
            }
            else {
                std::bitset<16> bitset(IMM);
                int decimalValue = bitset.to_ulong();
                B_OFFSET = std::to_string(decimalValue);
            }
            std::cout << " h hi di: " << B_OFFSET << std::endl;
        }
        
        } inputFile.close();
    }



    inputFile.open("test_case3.obj");
    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) { // read line by line
           //std::cout << "FILELINE " << line << std::endl; // print line
            std::string BINARY; //std::string FINISH_HEX;
            BINARY = ConverttoBinary(line);
           // std::cout << "Converted binary value: " << BINARY << std::endl;
            std::string OPCODE = BINARY.substr(0, 6);
            std::string INSTRUCTION;
            std::cout << "opcode: " << OPCODE << std::endl;
            if(OPCODE == RTYPE) {
                 std::cout << "R TYPE" << std::endl;
                 std::string MNEM = FIND_IN_MAP(BINARY.substr(26, 6), R_OPCODES);
                 std::string RT = FIND_IN_MAP(BINARY.substr(11, 5), REGISTERS);
                 std::string RD = FIND_IN_MAP(BINARY.substr(16, 5), REGISTERS);
                 std::string RS = FIND_IN_MAP(BINARY.substr(6, 5), REGISTERS);
                if (MNEM == "srl" || MNEM == "sll") {
                    std::bitset<8> bits(BINARY.substr(21, 5));
                    std::string SHAMT = std::to_string(bits.to_ulong()); 
                    INSTRUCTION = MNEM + " " + RD + ", "  + RT + ", " + SHAMT;
                }
                else {
                     INSTRUCTION = MNEM + " " + RD + ", " + RS + ", " + RT;
                }
                std::cout << INSTRUCTION << std::endl; 
            }
            else { //else I-type instruction
                 std::cout << "I TYPE" << std::endl;
                 std::string OP = FIND_IN_MAP(OPCODE, I_OPCODES);
                 std::string RS = FIND_IN_MAP(BINARY.substr(6, 5), REGISTERS);
                 std::string RT = FIND_IN_MAP(BINARY.substr(11, 5), REGISTERS);
                 std::string IMM = BINARY.substr(16, 16);
                
                if(OP == "beq" || OP == "bne") {
                    std::string IMMM;
                    std::cout << "IMM: " << IMM << std::endl;
                    if(IMM[0] == '1') {
                        IMMM = "-" + twosCompliment(IMM);
                    }
                    else {
                        std::bitset<16> bitset(IMM);
                        int decimalValue = bitset.to_ulong();
                        IMMM = std::to_string(decimalValue);
                    }
                    std::cout << "IMMM: " << IMMM << std::endl;
                    int offset = std::stoi(IMM, nullptr, 16);
                    std::cout << "OFFSET: " << offset << std::endl;
                    int targetAddress = address + (offset * 4);
                    std::cout << "TARGET ADDRESS: " << targetAddress << std::endl;



                }
                else if(OP == "lw" || OP == "sw") {
                    std::bitset<16> bits(IMM);
                    std::string decimal_IMM = std::to_string(bits.to_ulong());
                    INSTRUCTION = OP + " " + RT + ", " + decimal_IMM + "(" + RS + ")"; 
                }
                else {
                    std::bitset<16> bits(IMM);
                    std::string decimal_IMM = std::to_string(bits.to_ulong());
                     INSTRUCTION = OP + " " + RT + ", " + RS + ", " + decimal_IMM; 
                }
                 std::cout << INSTRUCTION << std::endl;     
            }
            outfile << INSTRUCTION << std::endl; //add instruction to new line of .asm file
            address += 4;
        }
        inputFile.close(); // close file
        outfile.close();
    }
    else { 
        std::cout << "Error opening file." << std::endl;
    }
    return 0;
}


std::string ConverttoBinary(std::string line) {
    std::string binaryString = ""; //declare empty string 
    uint64_t DecValue = std::stoull(line, nullptr, 16); // convert the string (the hex value) into a unsigned 64 bit integer (decimal)
    //note uint64_t was used to handle large hex values that would overflow an int 
      //std::cout << "Dec value " << DecValue << std::endl;
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

std::string FIND_IN_MAP(std::string opcodeStr, std::map<std::string, std::string> MAP) {
    
    //std::cout << "opcodeStr" << opcodeStr << std::endl;
    // Iterate over the map and look for the opcode
    std::map<std::string, std::string>::iterator it;
    for (it = MAP.begin(); it != MAP.end(); ++it) {
    //std::cout << it->first << ": " << it->second << std::endl;
    if (it->second == opcodeStr) {
        // Opcode found, return the instruction name
        //std::cout << "f u n c t !: " << it->first << std::endl;
        return it->first;
    }
}
    // Opcode not found, print the opcode and return an empty string
    std::cout << "String not found: " << opcodeStr << std::endl;
    return "";

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

