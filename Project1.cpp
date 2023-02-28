#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
#include <iomanip>
#include "Project1.hpp"

std::string ConverttoBinary(std::string line);
std::string addLeadingZeroes(std::string binaryStr);
std::string FIND_IN_MAP(std::string opcodeStr, std::map<std::string, std::string> MAP);
std::string twosCompliment(std::string binaryStr);
std::string decimalToHex(unsigned int decimalValue);
void GENERATE_LABEL(int address, std::string IMM);

bool ERROR = false; //error flag

int main() {
    // std::string FileName;
    // std::cout << "Enter file name with suffix: ";
    // std::cin >> FileName;
    // std::ifstream inputFile(FileName); // open file for reading
    std::ofstream outfile("proj1_skidmore.asm", std::ios::trunc);
    std::ifstream inputFile("test_case4.obj");
    std::string line;
    std::string RTYPE = "000000";
    int address = 0;
  
     if (inputFile.is_open()) {
        address = 2; 
        while (std::getline(inputFile, line)) { // read line by line
            std::cout << "line" << line << std::endl;
            std::string BINARY; //std::string FINISH_HEX;
            BINARY = ConverttoBinary(line);
            std::string OPCODE = BINARY.substr(0, 6);
        if (OPCODE == "000100" || OPCODE == "000101"){
             std::string IMM = BINARY.substr(16, 16);
             GENERATE_LABEL(address, IMM);
        }
        address++;
        std::cout << "address" << address << std::endl;
        }
         std::cout << "PAST CLEASR" << std::endl;
        inputFile.clear();// set the position of the file pointer back to the beginning of the file
        inputFile.seekg(0, std::ios::beg);
        address = 1;
        while (std::getline(inputFile, line)) { // read line by line
           //std::cout << "FILELINE " << line << std::endl; // print line
           if (!labels.empty()) {
            std::map<int, std::string>::iterator it;
            for (it = labels.begin(); it != labels.end(); ++it) {
            //std::cout << it->first << ": " << it->second << std::endl;
                if (it->first == address) {
                    outfile << it->second << std::endl;
                 }
            }
           }
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
                    INSTRUCTION = "\t" + MNEM + " " + RD + ", "  + RT + ", " + SHAMT;
                }
                else {
                     INSTRUCTION = "\t" + MNEM + " " + RD + ", " + RS + ", " + RT;
                }
                std::cout << INSTRUCTION << std::endl; 
            }
            else { //else I-type instruction
                 std::cout << "I TYPE" << std::endl;
                 std::string OP = FIND_IN_MAP(OPCODE, I_OPCODES);
                 std::string RS = FIND_IN_MAP(BINARY.substr(6, 5), REGISTERS);
                 std::string RT = FIND_IN_MAP(BINARY.substr(11, 5), REGISTERS);
                 std::string IMM = BINARY.substr(16, 16);
                
                 if(OP == "lw" || OP == "sw") {
                    std::bitset<16> bits(IMM);
                    std::string decimal_IMM = std::to_string(bits.to_ulong());
                    INSTRUCTION = "\t" + OP + " " + RT + ", " + decimal_IMM + "(" + RS + ")"; 
                }
                // else if(OP == "bne" || OP == "beq") {
                //     std::map<int, std::string>::iterator it;
                //     for (it = labels.begin(); it != labels.end(); ++it) {
                //     if (it->first == address) {
                //     outfile << it->second << std::endl;
                //  }
            //}
                     //INSTRUCTION = "\t" + OP + " " + RT + ", " + RS + ", " + ; 
                //}
                else {
                    std::bitset<16> bits(IMM);
                    std::string decimal_IMM = std::to_string(bits.to_ulong());
                     INSTRUCTION = "\t" + OP + " " + RT + ", " + RS + ", " + decimal_IMM; 
                }
                 std::cout << INSTRUCTION << std::endl;     
            }
            outfile << INSTRUCTION << std::endl; //add instruction to new line of .asm file
            address++;
        }
        inputFile.close(); // close file
        outfile.close();
    //}
   // else { 
    //    std::cout << "Error opening file." << std::endl;
    //}
    
     }
    else {
         std::cout << "Error opening file." << std::endl;
    }


    //inputFile.open("test_case2.obj");
    //if (inputFile.is_open()) {
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
    std::string ERROR = "ERROR: " + opcodeStr + " is not a valid opcode or funct or label.";
    // Opcode not found, print the opcode and return an empty string
    return ERROR;
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
    return ss.str();
}
void GENERATE_LABEL(int address, std::string IMM) {
            std::string OFFSET;
            int target_address;
            if(IMM[0] == '1') {
                //OFFSET = twosCompliment(IMM);
                int offset = std::stoi(twosCompliment(IMM)); // convert string to integer
                target_address = address - offset; //print label_line
                std::cout << "LABEL L I N E: - " <<  target_address << std::endl;
            }
            else {
                std::bitset<16> bitset(IMM);
                int offset = bitset.to_ulong();
                target_address = address + offset;
                std::cout << "LABEL L I N E: add " <<  target_address << std::endl;
                //OFFSET = std::to_string(decimalValue); //converting string representsing binary value into a signed integer
            }
            std::string address_label = "Addr_" + decimalToHex((target_address-1) * 4) + ":";
            std::string TARGET_ADDRESS = std::to_string(target_address);
            std::cout << "add label " << address_label << std::endl; //name of label
            std::cout << "target addreess " << TARGET_ADDRESS << std::endl; //name of l
            labels[target_address] = address_label;
}
        