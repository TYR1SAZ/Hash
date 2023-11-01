#include "SHA512.h"

void BitPrint(char& byte)
{
    for (int i = 7; i >= 0; --i) // 8 бит (00000000) 
        std::cout << ((byte >> i) & 1);
    std::cout << " ";
}

void BitPrint(uint64_t& byte)
{
    for (int i = 63; i >= 0; --i) // 64 бит (00000000) 
        std::cout << ((byte >> i) & 1);
    std::cout << endl;
}

// Дополняем исходное сообщение нулями до кратности 896 битам и добавляем в конец 128 бит длинны исходного сообщения; итого сообщение кратно 1024 битам
int AddZeroAndLengthBit(string& message)
{
    int oldLenMess = message.size();
    std::cout << "Старая длинна блока в битах: " << oldLenMess * 8 << endl;

    int newLenMess = oldLenMess + BYTE - oldLenMess % BYTE;
    message[oldLenMess - 1] ++;
    message += 128;
    for (int i = message.size(); i < newLenMess - 8; i++)
    {
        message += '\0';
    }
    uint64_t len = oldLenMess;
    len <<= 3;
    for (int i = 0; i < 8; i++)
    {
        message += (len >> (7 - i) * 8);
    }
    oldLenMess >>= 3;
    std::cout << "Новая длинна блока в битах: " << newLenMess * 8 << endl;
    return newLenMess;
}

string SHA_512(string& message)
{
    // Константы для SHA512
    uint64_t h[8]
    {
        0x6A09E667F3BCC908,
        0xBB67AE8584CAA73B,
        0x3C6EF372FE94F82B,
        0xA54FF53A5F1D36F1,
        0x510E527FADE682D1,
        0x9B05688C2B3E6C1F,
        0x1F83D9ABFB41BD6B,
        0x5BE0CD19137E2179,
    };

    //Таблица констант
    //(первые 64 бита дробных частей кубических корней первых 80 простых чисел [от 2 до 409]):
    uint64_t k[]
    {
         0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
         0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
         0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
         0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
         0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
         0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
         0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
         0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
         0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
         0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
         0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
         0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
         0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
         0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
         0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
         0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
         0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
         0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
         0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
         0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
    };
    int len = AddZeroAndLengthBit(message);
    uint64_t* W;
    uint64_t s[8];
    for (int i = 0; i < len / BYTE; i++)
    {
        W = new uint64_t[80]{ 0 };
        for (int j = 0; j < 16 * 8; j += 8)
        {
            int k = j / 8;
            W[k] = message[i * 16 + j + 0];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 1];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 2];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 3];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 4];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 5];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 6];
            W[k] <<= 8;
            W[k] += message[i * 16 + j + 7];
            BitPrint(W[k]);
        }
        for (size_t j = 16; j < 80; j++)
        {

            W[j] = W[j - 16] + sig0(W[j - 15]) + W[j - 7] + sig1(W[j - 2]);
        }
        for (int j = 0; j < 8; j++)
        {
            s[j] = h[j];
        }
        for (int j = 0; j < 80; j++)
        {
            uint64_t temp2 = Sig0(s[0]) + Maj(s[0], s[1], s[2]);
            uint64_t temp1 = s[7] + Sig1(s[4]) + Ch(s[4], s[5], s[6]) + k[j] + W[j];


            s[7] = s[6];
            s[6] = s[5];
            s[5] = s[4];
            s[4] = s[3] + temp1;
            s[3] = s[2];
            s[2] = s[1];
            s[1] = s[0];
            s[0] = temp1 + temp2;
        }
        for (int j = 0; j < 8; j++)
        {
            h[j] += s[j];
        }
    }
    stringstream stream;
    stream << hex << h[0] << " " << h[1] << " " << h[2] << " " << h[3] << " " << h[4] << " " << h[5] << " " << h[6] << " " << h[7];
    //stream << hex << h[0] << h[1] << h[2] << h[3] << h[4] << h[5] << h[6] << h[7];
    string digest = stream.str();
    return digest;
}