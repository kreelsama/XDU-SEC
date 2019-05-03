#include <stdio.h>
#include <string.h>
#include "miracl.h"

unsigned int T[64] = {0};

unsigned int lshift(unsigned int x, int n){
    return (x << n) + (x >> (32-n) & 0xFFFFFFFF);
}

unsigned int FF(int rd, unsigned int X, unsigned int Y, unsigned int Z){
    if(rd <= 15)
        return X ^ Y ^ Z;
    else{
        return (X & Y) | (X & Z) | (Y & Z);
    }
}

unsigned int GG(int rd, unsigned int X,unsigned int Y, unsigned int Z){
    if(rd <= 15)
        return X ^ Y ^ Z;
    else
        return (X & Y) | ( (~X) & Z );
}

unsigned int Perm0(unsigned int X){
    return X ^ lshift(X, 9) ^ lshift(X, 17);
}

unsigned int Perm1(unsigned int X){
    return X ^ lshift(X, 15) ^ lshift(X, 23);
}


void CF(unsigned int V[8], const unsigned int W[16]){
    unsigned int Wt[68] ={0}, Wp[64] = {0};
    //Message Extension
    for(int j = 0; j < 68; j++){
        if(j <= 15){
            Wt[j] = W[j];
            if(j >= 4){
                Wp[j-4] = Wt[j-4] ^ Wt[j];
            }
            continue;
        }
        Wt[j] = Perm1(Wt[j-16] ^ Wt[j-9] ^ lshift(Wt[j-3], 15)) ^ lshift(Wt[j-13], 7) ^ Wt[j-6];
        Wp[j-4] = Wt[j-4] ^ Wt[j];
    }
    //Compress section
    unsigned int A[8], SS1, SS2, TT1, TT2;//A[8] stands for ABCDEFGH
    //A B C D E F G H
    //0 1 2 3 4 5 6 7
    for(int j = 0; j < 8; j++)
        A[j] = V[j];
    for(int j = 0; j < 64; j++){
        SS1 = lshift(lshift(A[0], 12) + A[4] + lshift(T[j], j), 7);
        SS2 = SS1 ^ lshift(A[0], 12);
        TT1 = FF(j, A[0], A[1], A[2]) + A[3] + SS2 + Wp[j];
        TT2 = GG(j, A[4], A[5], A[6]) + A[7] + SS1 + Wt[j];
        A[3] = A[2];
        A[2] = lshift(A[1], 9);
        A[1] = A[0];
        A[0] = TT1;
        A[7] = A[6];
        A[6] = lshift(A[5], 19);
        A[5] = A[4];
        A[4] = Perm0(TT2);
    }
    for(int j = 0; j < 8; j++)
        V[j] = A[j] ^ V[j];
}

void SM3(unsigned char* string, char* hash){
    long int size = strlen(string);
    unsigned int V[8]={0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,
                       0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e};
    long int resize = 0;
    long int blk = 1 + (int)((size*8)/512);
    if(size*8 % 512 > 448)
        resize = (blk + 1) * 512;
    else
        resize = blk * 512;
    resize /= 8;
    int cur = 0, i = 0;
    while(cur < resize){
        i = 0;
        unsigned char ch;
        unsigned int W[16] = {0};
        while(i<16){
            int j = 0;
            while(j < 4){
                if(cur >= size )
                    ch = 0;
                else{
                    ch = string[cur];
                }
                W[i] = ( W[i] << 8 )+ ch;
                j += 1;
                cur += 1;
            }
            i += 1;
        }
        if(cur > size) {
            if(string[size - 1] == 0x80)
                W[15] = (unsigned int) (size * 8 - 8);
            else
                W[15] = (unsigned int) (size * 8 - 4);
        }
        CF(V, W);
    }
    sprintf(hash, "%08x%08x%08x%08x%08x%08x%08x%08x", V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    fprintf(stdout, "SM3 sum is: %s", hash);
    puts("\n");
}

int main(int argc, char*argv[]) {
    FILE *fp = fopen(argv[1], "r");
    miracl *mip = mirsys(1000, 16);
    mip->IOBASE = 16;
    unsigned char msg[500] = {0};
    char hexstring[500]={0};

    if(!fp){
        fprintf(stderr, "No such file or directory\n");
        return 0;
    }
    for(int i = 0; i <64; i++){
        if(i<=15)
            T[i] = 0x79cc4519;
        else
            T[i] = 0x7a879d8a;
    }
    big number;
    number = mirvar(0);
    cinnum(number, fp);
    cotstr(number, hexstring);
    mirkill(number);
    size_t len = strlen(hexstring);
    if(len % 2 == 0){//Even length: padding 0x80
        hexstring[len] = '8';
        hexstring[len+1] = '0';
        len += 2;
    }
    else{//Odd length: padding 0x8
        hexstring[len] = '8';
        len += 1;
    }
    for(int i = 0; 2*i < len; i++) {
        sscanf(hexstring + 2 * i, "%02x", msg + i);
    }
    char hash[65]={0};
    SM3(msg, hash);
    return 0;
}