#include "SHA512.h"
#include <iostream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");
    string message = "The quick brown fox jumps over the lazy dog";
    cout << SHA_512(message);
    return 0;
}