#include "huffman.hpp"
#include <iostream>
#include <string>
#include <algorithm> // Für std::transform
#include <cctype>    // Für std::tolower

int main() {
    std::string mode;
    std::cout << "Möchten Sie komprimieren (c) oder dekomprimieren (d)? ";
    std::getline(std::cin, mode);
    // Umwandlung in Kleinbuchstaben
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
    
    if (mode != "c" && mode != "d") {
        std::cout << "Ungültiger Modus. Bitte 'c' (komprimieren) oder 'd' (dekomprimieren) eingeben.\n";
        return 1;
    }
    
    std::string inputPath;
    std::cout << "Bitte geben Sie den vollständigen Pfad zur Datei ein: ";
    std::getline(std::cin, inputPath);
    
    if (mode == "c") {
         Huffman::compressFile(inputPath);
    } else if (mode == "d") {
         Huffman::decompressFile(inputPath);
    }
    
    return 0;
}
