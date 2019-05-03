#include <stdio.h>
#include "miracl.h"
#include <memory.h>
#include <stdlib.h>
#include <time.h>

int CRT(big a[], big b[], big sum){
    //a->a, b->m, res->result which must be mirvar(0) BEFORE passing into this function
    big mul, x, y, z,one;
    int len = 0;

    x = mirvar(0);
    y = mirvar(0);
    z = mirvar(0);
    mul = mirvar(1);
    one = mirvar(1);

    for(len = 0; a[len]!=NULL && b[len]!=NULL ; len++);

    //validating the gcd of input modulus
    for(int j = 0; j < len; j++){
        for(int k = j + 1; k < len; k++){
            egcd(b[j], b[k], x);
            if(mr_compare(x, mirvar(1))){
                fprintf(stderr, "Input unsupported\n");
                mirkill(x);
                mirkill(y);
                mirkill(z);
                mirkill(mul);
                return 4;
            }
        }
    }

    for(int k = 0; k < len; k++) {
        powmod(a[k], one, b[k], a[k]);
        multiply(mul, b[k], mul);
    }

    for(int k = 0; k < len; k++){
        copy(mul,z);
        divide(z, b[k], x);
        xgcd(x, b[k], y, z, z);
        multiply(y, x, z);
        multiply(z, a[k], z);
        add(z, sum ,sum);
    }

    powmod(sum, one, mul, sum);

    mirkill(x);
    mirkill(y);
    mirkill(z);
    mirkill(mul);
    mirkill(one);
    return 1;
}

int exm(big d[]){
    //GCD test. if coprime: return 0; else return 1
    big res = mirvar(0);
    big one = mirvar(1);
    for(int i = 1; i <= 5; i++){
        for(int j = i + 1; j <= 5; j++){
            if(!(d[i]&&d[j]))
                continue;
            egcd(d[i], d[j], res);
            if(mr_compare(res, one))
                return 1;
        }
    }
    mirkill(res);
    mirkill(one);
    return 0;
}

int main(int argc, char*argv[]) {
    miracl *mip = mirsys(5000, 10);
    mip->IOBASE = 10;

    big d[7] = {NULL};
    big messages[7]={NULL}, modulus[7] = {NULL};
    big sec;
    big tmp;

    d[0] = mirvar(0);//constant
    d[6] = mirvar(0);//constant
    sec = mirvar(0);
    tmp = mirvar(0);
    irand((unsigned int)time(NULL));
    bigdig(300,10, sec);
    printf("Original secret message:\n");
    cotnum(sec, stdout);

    nroot(sec,3, d[0]);//d[0] = ceil(sec^(1/3))
    nroot(sec,2, d[6]);//d[6] = ceil(sec^(1/2))

    int i = 0;
    int r = 0, iter = 0;

    do {
        printf("round %d\n", r ++ );
        for(i = 1; i <=5; i++){
            if(d[i])
                mirkill(d[i]);
            d[i] = NULL;
        }

        for (i = 1; i <= 5; i++) {
            d[i] = mirvar(0);
            do {
                subtract(d[6], d[i - 1], tmp);
                bigrand(tmp, tmp);
                if (tmp == 0)
                    break;
                add(tmp, d[i - 1], d[i]);
                iter ++;
            }while(exm(d));
        }
        if(i == 6 && !exm(d))
            break;
    }while(1);

    mirkill(tmp);
    tmp = mirvar(1);
    fprintf(stdout,"Algorithm succedeed after %d round(s) and %d iteration\n; Generated series is:\n", r, iter);
    for(i = 1; i <= 5; i++){
        modulus[i-1] = mirvar(0);
        messages[i-1] = mirvar(0);
        copy(d[i],modulus[i-1]);
        powmod(sec, tmp, modulus[i-1], messages[i-1]);
        fprintf(stdout, "Pairs for the number %d person:\n", i);
        cotnum(messages[i - 1], stdout);
        cotnum(modulus[i - 1], stdout);
    }

    fprintf(stdout, "TEST PART STARTED HERE\n");
    big testmes[4] = {NULL}, testmod[4] = {NULL};
    i = 0;
    int randlist[3] = {-1, -1, -1};
    srand((unsigned int)time(NULL));
    fprintf(stdout, "Select ");
    do{
        int x;
        x = rand()%5;
        if(x == randlist[0] || x == randlist[1] || x == randlist[2])
            continue;
        randlist[i] = x;
        fprintf(stdout, "%d ", x);
        i ++;
    }while(i<3);
    fprintf(stdout, "for test\n");
    for(i = 0; i < 3; i++){
        testmes[i] = messages[randlist[i]];
        testmod[i] = modulus[randlist[i]];
    }

    big res = mirvar(0);
    CRT(testmes, testmod, res);
    fprintf(stdout, "Decoded secret:\n");
    cotnum(res, stdout);
    if(!mr_compare(res, sec))
        fprintf(stdout, "EQUAL!\n");
    return 0;
}