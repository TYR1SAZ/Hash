#pragma once
#include <iostream>
#include <sstream>

using namespace std;

const unsigned BIT = 1024;
const unsigned BYTE = 128;

// Циклический сдвиг вправо
#define RotR(x, n) ((x>>n)|(x<<((sizeof(x)<<3)-n)))
#define Ch(x,y,z) ((x&y)^(~x&z))
#define Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define Sig0(x) ((RotR(x, 28))^(RotR(x,34))^(RotR(x, 39)))
#define Sig1(x) ((RotR(x, 14))^(RotR(x,18))^(RotR(x, 41)))
#define sig0(x) (RotR(x, 1)^RotR(x,8)^(x>>7))
#define sig1(x) (RotR(x, 19)^RotR(x,61)^(x>>6))

void BitPrint(char& byte);

void BitPrint(uint64_t& byte);

// Дополняем исходное сообщение нулями до кратности 896 битам и добавляем в конец 128 бит длинны исходного сообщения; итого сообщение кратно 1024 битам
int AddZeroAndLengthBit(string& message);

string SHA_512(string& message);