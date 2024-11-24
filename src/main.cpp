#include "../include/DES.h"
#include<iostream>

using namespace std;

int main() {
    DES des;
    string text = "ana are mere si";
    string key = "anaareme";
    string encryptedText = des.encryption(text, key);
    cout << "encryption (hex): " << encryptedText << endl;
    string decryptedText = des.decryption(encryptedText, key);
    cout << "decryption: " << decryptedText << endl;
    return 0;
}
