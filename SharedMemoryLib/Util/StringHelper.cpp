#include "stdafx.h"
#include "StringHelper.h"

// https://stackoverflow.com/questions/47977829/generate-a-random-string-in-c11
std::string mx404::Util::random_string() {
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);

    return str.substr(0, 32);    // assumes 32 < number of characters in str
}