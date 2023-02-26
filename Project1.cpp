#include <iostream>
#include <fstream>
#include <string>
//using namespace std;
std::string ConverttoBinary(std::string line);
std::string addLeadingZeroes(std::string binaryStr);

int main() {
    //std::string FileName;
    //std::cout << "Enter file name with suffix: " << std::endl;
    //std::cin >> FileName;
    //std::ifstream inputFile(FileName); // open file for reading
    std::ifstream inputFile("test_case2.obj");
    std::string line;

    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) { // read line by line
           std::cout << "FILELINE " << line << std::endl; // print line
            std::string BINARY; //std::string FINISH_HEX;
            BINARY = ConverttoBinary(line);
            std::cout << "Converted binary value: " << BINARY << std::endl;
        }
        inputFile.close(); // close file
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
    std::cout << "Dec value " << DecValue << std::endl;
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