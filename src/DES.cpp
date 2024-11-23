#include "../include/DES.h"
#include<iostream>
#include <algorithm>
#include<bitset>
#include <sstream>
#include <iomanip>
#include <stdint.h>

using namespace std;

DES::DES()
{

}

vector<uint8_t> DES::initialPermutation(const vector<uint8_t> &input)
{
    vector<uint8_t> result(64);
    for (int i = 0; i < 64; i++) {
        result[i] = input[IP[i] - 1];
    }
    return result;
}

vector<uint8_t> DES::finalPermutation(const vector<uint8_t> &input)
{
    vector<uint8_t> result(64);
    for (int i = 0; i < 64; i++) {
        result[i] = input[FP[i] - 1];
    }
    return result;
}

vector<uint8_t> DES::pPermutation(const vector<uint8_t> &input)
{
    vector<uint8_t> result(32);
    for (int i = 0; i < 32; i++) {
        result[i] = input[P[i]];
    }
    return result;
}

vector<uint8_t> DES::expansion(const vector<uint8_t> &input)
{
    vector<uint8_t> result(48);
    for (int i = 0; i < 48; i++) {
        result[i] = input[E[i]];
    }
    return result;
}

vector<uint8_t> DES::PC1permutation(const vector<uint8_t> &input)
{
    vector<uint8_t> result(56);
    for (int i = 0; i < 56; i++) {
        result[i] = input[PC1[i]];
    }
    return result;
}

vector<uint8_t> DES::PC2Permutation(const vector<uint8_t> &input)
{
    vector<uint8_t> result(48);
    for (int i = 0; i < 48; i++) {
        result[i] = input[PC2[i]];
    }
    return result;
}

vector<uint8_t> DES::rotateLeftBy1(vector<uint8_t> &input)
{
    rotate(input.begin(), input.begin() + 1, input.end());
    return input;
}

vector<uint8_t> DES::rotateLeftBy2(vector<uint8_t> &input)
{
    rotate(input.begin(), input.begin() + 2, input.end());
    return input;
}

vector<uint8_t> DES::rotateRightBy1(vector<uint8_t> &input)
{
    rotate(input.rbegin(), input.rbegin() + 1, input.rend());
    return input;
}
vector<uint8_t> DES::rotateRightBy2(vector<uint8_t> &input)
{
    rotate(input.rbegin(), input.rbegin() + 2, input.rend());
    return input;
}

vector<uint8_t> DES::toBinary(const string &input)
{
    vector<uint8_t> binaryBits;

    for (size_t i = 0; i < 8; ++i) {
        if (i < input.size()) {
            bitset<8> charBits(input[i]);
            for (int j = 7; j >= 0; --j) {
                binaryBits.push_back(charBits[j]);
            }
        } else {
            for (int j = 0; j < 8; ++j) {
                binaryBits.push_back(0);
            }
        }
    }
    return binaryBits;
}

vector<vector<uint8_t>> DES::splitMessageToBlocks(const string& message)
{
    vector<vector<uint8_t>> binaryBlocks;
    for (size_t i = 0; i < message.size(); i += 8) {
        string block = message.substr(i, 8);
        binaryBlocks.push_back(toBinary(block));
    }
    return binaryBlocks;
}

 string DES::toText(const vector<uint8_t> &input)
{
    string decryptedText;
    size_t totalBytes = input.size() / 8;
    for (size_t i = 0; i < totalBytes; ++i) {
        uint8_t character = 0;
        for (size_t j = 0; j < 8; ++j) {
            character = (character << 1) | input[i * 8 + j];
        }
       if (character != 0) {
           decryptedText.push_back(static_cast<char>(character));
        }
    }
   return decryptedText;
}

vector<vector<uint8_t>> DES::generateKeysForEncryption(const string& key)
{
    vector<vector<uint8_t>> keys;
    vector<uint8_t> binaryKey = toBinary(key);
    binaryKey = PC1permutation(binaryKey);
    vector<uint8_t> left(binaryKey.begin(), binaryKey.begin() + 28);
    vector<uint8_t> right(binaryKey.begin() + 28, binaryKey.end());
    for(int i=1; i<=16; i++) {
        if(i == 1 || i == 2 || i == 9 || i == 16) {
            left = rotateLeftBy1(left);
            right = rotateLeftBy1(right);
        }
        else {
            left = rotateLeftBy2(left);
            right = rotateLeftBy2(right);
        }
        vector<uint8_t> key = left;
        key.insert(key.end(), right.begin(), right.end());
        key = PC2Permutation(key);
        keys.push_back(key);
    }
    return keys;
}

vector<uint8_t> DES::xorVectors(const vector<uint8_t>& v1, const vector<uint8_t>& v2)
{
    vector<uint8_t> result(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] ^ v2[i];
    }
    return result;
}

vector<uint8_t> DES::sbox(const vector<uint8_t>& input)
{
    vector<uint8_t> result(32);
    for (int i = 0; i < 8; ++i) {
        uint8_t input6bits = 0;
        for (int j = 0; j < 6; ++j) {
            input6bits = (input6bits << 1) | input[i * 6 + j];
        }
        uint8_t row = ((input6bits & 0b100000) >> 5) | (input6bits & 0b000001);
        uint8_t col = (input6bits & 0b011110) >> 1;
        uint8_t output4bits = sboxes[i][row][col];
        for (int j = 3; j >= 0; --j) {
            result[i * 4 + j] = (output4bits & 1);
            output4bits >>= 1;
        }
    }
    return result;
}

vector<vector<uint8_t>> DES::generateKeysForDecryption(const string& key)
{
    vector<vector<uint8_t>> keys = generateKeysForEncryption(key);
    reverse(keys.begin(), keys.end());
    return keys;
}

string toHex(const vector<uint8_t>& data) {
    stringstream ss;
    for (uint8_t byte : data) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

vector<uint8_t> fromHex(const string& hex) {
    vector<uint8_t> data;
    for (size_t i = 0; i < hex.size(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
        data.push_back(byte);
    }
    return data;
}

string addPadding(const string &input)
{
   size_t padLength = 8 - (input.size() % 8);
    return input + string(padLength, static_cast<char>(padLength));
}

string removePadding(const string &input)
{
    char padChar = input[input.size() - 1];
    size_t padLength = static_cast<size_t>(padChar);
    return input.substr(0, input.size() - padLength);
}

string DES::encryption(const string &input, const string& key)
{
    string paddedInput = addPadding(input);
    vector<uint8_t> result;
    vector<uint8_t> intermediate;
    vector<vector<uint8_t>> binaryMessages = splitMessageToBlocks(paddedInput);
    vector<vector<uint8_t>> keys = generateKeysForEncryption(key);
    for(vector<uint8_t> message: binaryMessages) {
        message = initialPermutation(message);
        vector<uint8_t> left(message.begin(), message.begin() + 32);
        vector<uint8_t> right(message.begin() + 32, message.end());
        for(int i=1; i<=16; i++) {
            vector<uint8_t> aux = right;
            aux = expansion(aux);
            aux = xorVectors(aux, keys[i-1]);
            aux = sbox(aux);
            aux = pPermutation(aux);
            aux = xorVectors(aux, left);
            left = right;
            right = aux;
        }
        swap(left, right);

        intermediate = left;
        intermediate.insert(intermediate.end(), right.begin(), right.end());
        intermediate = finalPermutation(intermediate);
        result.insert(result.end(), intermediate.begin(), intermediate.end());
    }
    return toHex(result);
}

string DES::decryption(const string &input, const string& key)
{
    vector<uint8_t> result;
    vector<uint8_t> intermediate;
    vector<vector<uint8_t>> binaryMessages = splitMessageToBlocks(toText(fromHex(input))); 
    vector<vector<uint8_t>> keys = generateKeysForDecryption(key);

    for(vector<uint8_t> message: binaryMessages) {
        message = initialPermutation(message);
        vector<uint8_t> left(message.begin(), message.begin() + 32);
        vector<uint8_t> right(message.begin() + 32, message.end());

        for(int i=1; i<=16; i++) {
            vector<uint8_t> aux = right;
            aux = expansion(aux);
            aux = xorVectors(aux, keys[i-1]);
            aux = sbox(aux);
            aux = pPermutation(aux);
            aux = xorVectors(aux, left);
            left = right;
            right = aux;
        }
        swap(left, right);

        intermediate = left;
        intermediate.insert(intermediate.end(), right.begin(), right.end());
        intermediate = finalPermutation(intermediate);
        result.insert(result.end(), intermediate.begin(), intermediate.end());
    }

    string decryptedText = toText(result);
    return removePadding(decryptedText);
}