#include "../include/DES.h"
#include<iostream>

using namespace std;

int main() {
    DES des;
    string text = "ana are mere si";
    string key = "anaareme";
    int constant = 80;
    string encryptedText = des.encryption(text, key, static_cast<uint8_t>(constant));
    cout << "encryption (hex): " << encryptedText << endl;
    string decryptedText = des.decryption(encryptedText, key, constant);
    cout << "decryption: " << decryptedText << endl;
    return 0;
}