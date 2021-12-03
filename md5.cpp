#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

using word = uint32_t;
using byte = uint8_t;

const word initA = 0x01234567;
const word initB = 0x89abcdef;
const word initC = 0xfedcba98;
const word initD = 0x76543210;
const word T[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

void md5(string input);
vector<word> md5Loop(vector<word> data);

word round1(word x[], word a, word b, word c, word d, word k, word s, word i);
word round2(word x[], word a, word b, word c, word d, word k, word s, word i);
word round3(word x[], word a, word b, word c, word d, word k, word s, word t);
word round4(word x[], word a, word b, word c, word d, word k, word s, word t);

word F(word x, word y, word z);
word G(word x, word y, word z);
word H(word x, word y, word z);
word I(word x, word y, word z);


int main()
{
    md5("test1sdfdf sdfsdfsdfsdf fsdsdgdh ndfk ndfjfknv dfkbjfn bd bkjbndfkjbn dfbjn");
}

//Add length in 64 bit form NOTICE: CANNOT HANDLE LENGTH OF MORE THAN 64 bits
void md5(string input)
{
    vector<word> data;

    //Load data
    for(int i = 0; i < input.length(); i++)
        data.push_back(input.at(i));
    uint64_t length = data.size();

    //Need to pad so that data is 512k + 448 bits long, or 64k + 56 bytes long
    int padbytes = 56 - (data.size() % 64); //Determines how much is "left over" after 512, and makes that the pad bytes length
    if(padbytes == 0)
        padbytes = 56; //Must be at least one bit of padding

    data.push_back(0x80);
    for(int i = 0; i < padbytes -1; i++)
        data.push_back(0);

    //Add 0s (for length)
    for(int i = 0; i < 4; i++)
        data.push_back(0);

    //Add length, converted to bytes
    for(int i = 0; i < 4; i++)
    {
        int shift = i*8;
        uint64_t mask = 0xff000000 >> shift;
        uint64_t masked = length & mask;
        data.push_back( (masked << shift) >> 24 );

        cout << ((masked << shift) >> 24) << endl;
    }

    //Perform hash
    vector<word> out = md5Loop(data);

    /* Print hash */
    for(int i = 0; i < 4; i++)
        cout << hex << out.at(i);
}

vector<word> md5Loop(vector<word> data)
{
    //Init abcd
    word a = initA;
    word b = initB;
    word c = initC;
    word d = initD;

    /* Process each 16 word block */
    for(int i = 0; i < data.size()/16; i++) // 16 or 16-1?
    {
        //cout << "test";
        /* Copy block i into X */
        word x[16];
        for(int j = 0; j < 16; j++)
            x[j] = data.at(i*16+j);

        /* Save A as AA and so on */
        word aa = a;
        word bb = b;
        word cc = c;
        word dd = d;

        /* Round 1. */
        /* Perform round1(a, b, c, d, k, s, i), which returns the new value for a */
        a = round1(x, a, b, c, d, 0, 7, 1);
        d = round1(x, d, a, b, c, 1, 12, 2);
        c = round1(x, c, d, a, b, 2, 17, 3);
        b = round1(x, b, c, d, a, 3, 22, 1);

        a = round1(x, a, b, c, d, 4, 7, 5);
        d = round1(x, d, a, b, c, 5, 12, 6);
        c = round1(x, c, d, a, b, 6, 17, 7);
        b = round1(x, b, c, d, a, 7, 22, 8);

        a = round1(x, a, b, c, d, 8, 7, 9);
        d = round1(x, d, a, b, c, 9, 12, 10);
        c = round1(x, c, d, a, b, 10, 17, 11);
        b = round1(x, b, c, d, a, 11, 22, 12);

        a = round1(x, a, b, c, d, 12, 7, 13);
        d = round1(x, d, a, b, c, 13, 12, 14);
        c = round1(x, c, d, a, b, 14, 17, 15);
        b = round1(x, b, c, d, a, 15, 22, 16);

        /* Round 2. */
        /* Perform round2(x, abcd ksi) */
        a = round2(x, a, b, c, d, 1, 5, 17);
        d = round2(x, d, a, b, c, 6, 9, 18);
        c = round2(x, c, d, a, b, 11, 14, 19);
        b = round2(x, b, c, d, a, 0, 20, 20);

        a = round2(x, a, b, c, d, 5, 5, 21);
        d = round2(x, d, a, b, c, 10, 9, 22);
        c = round2(x, c, d, a, b, 15, 14, 23);
        b = round2(x, b, c, d, a, 4, 20, 24);

        a = round2(x, a, b, c, d, 9, 5, 25);
        d = round2(x, d, a, b, c, 14, 9, 26);
        c = round2(x, c, d, a, b, 3, 14, 27);
        b = round2(x, b, c, d, a, 8, 20, 28);

        a = round2(x, a, b, c, d, 13, 5, 29);
        d = round2(x, d, a, b, c, 2, 9, 30);
        c = round2(x, c, d, a, b, 7, 14, 31);
        b = round2(x, b, c, d, a, 12, 20, 32);

        /* Round 3. */
        /* Perform round3(x, abcd kst) */
        a = round3(x, a, b, c, d, 5, 4, 33);
        d = round3(x, d, a, b, c, 8, 11, 34);
        c = round3(x, c, d, a, b, 11, 16, 35);
        b = round3(x, b, c, d, a, 14, 23, 36);

        a = round3(x, a, b, c, d, 1, 4, 37);
        d = round3(x, d, a, b, c, 4, 11, 38);
        c = round3(x, c, d, a, b, 7, 16, 39);
        b = round3(x, b, c, d, a, 10, 23, 40);

        a = round3(x, a, b, c, d, 13, 4, 41);
        d = round3(x, d, a, b, c, 0, 11, 42);
        c = round3(x, c, d, a, b, 3, 16, 43);
        b = round3(x, b, c, d, a, 6, 23, 44);

        a = round3(x, a, b, c, d, 4, 6, 61);
        d = round3(x, d, a, b, c, 1, 10, 62);
        c = round3(x, c, d, a, b, 2, 15, 63);
        b = round3(x, b, c, d, a, 9, 21, 64);

        /* Round 4. */
        /* Perform round4(x, abcd kst) */
        a = round4(x, a, b, c, d, 0, 6, 49);
        d = round4(x, d, a, b, c, 7, 10, 50);
        c = round4(x, c, d, a, b, 14, 15, 51);
        b = round4(x, b, c, d, a, 5, 21, 52);

        a = round4(x, a, b, c, d, 12, 6, 53);
        d = round4(x, d, a, b, c, 3, 10, 54);
        c = round4(x, c, d, a, b, 10, 15, 55);
        b = round4(x, b, c, d, a, 1, 21, 56);

        a = round4(x, a, b, c, d, 8, 6, 57);
        d = round4(x, d, a, b, c, 15, 10, 58);
        c = round4(x, c, d, a, b, 6, 15, 59);
        b = round4(x, b, c, d, a, 13, 21, 60);

        a = round4(x, a, b, c, d, 4, 6, 61);
        d = round4(x, d, a, b, c, 11, 10, 62);
        c = round4(x, c, d, a, b, 2, 15, 63);
        b = round4(x, b, c, d, a, 9, 21, 64);

        /* A = A + AA and so on */
        a += aa;
        b += bb;
        c += cc;
        d += dd;
    }

    return vector<word>({a, b, c, d});
}

word round1(word x[], word a, word b, word c, word d, word k, word s, word i)
{
    word mid = a + F(b,c,d) + x[k] + T[i];
    mid <<= s;
    return b + mid;
}

word round2(word x[], word a, word b, word c, word d, word k, word s, word i)
{
    word mid = a + G(b,c,d) + x[k] + T[i];
    mid <<= s;
    return b + mid;
}

word round3(word x[], word a, word b, word c, word d, word k, word s, word t)
{
    word mid = a + H(b,c,d) + x[k] + T[t];
    mid <<= s;
    return b+mid;
}

word round4(word x[], word a, word b, word c, word d, word k, word s, word t)
{
    word mid = a + I(b,c,d) + x[k] + T[t];
    mid <<= s;
    return b+mid;
}

word F(word x, word y, word z)
{
    return (x & y) | (!x & z);
}

word G(word x, word y, word z)
{
    return (x & y) | (y & !z);
}

word H(word x, word y, word z)
{
    return x ^ y ^ z;
}

word I(word x, word y, word z)
{
    return y ^ (x | !z);
}
