#include <iostream>
#include <string>

int main() {
    std::string hex_str = "8d090004";

    // Convert hexadecimal string to decimal integer
    uint64_t decimal_num = std::stoull(hex_str, nullptr, 16);

    std::cout << "The decimal equivalent of " << hex_str
              << " is " << decimal_num << std::endl;

    return 0;
}