#include <stdio.h>
#include "miracl.h"
#define false 0
#define true 1

int fermat(big x){
    int i = 0;
    big y, z, m;
    y = mirvar(0);
    z = mirvar(0);
    m = mirvar(1);
    for(i = 0; i < 5; i++){
        bigrand(x,y);
        egcd(x,y,z);
        if(mr_compare(z,m)){
            return  false;
        }
        subtract(x,mirvar(1),z);
        powmod(y,z,x,z);
        if(mr_compare(z,m)){
            return false;
        }
    }
    mirkill(y);
    mirkill(z);
    mirkill(m);
    return true;
}

int main(int argc, char* argv[]) {
    FILE * fp;mirkill();
    int res = 0;
    big x;
    miracl  *mip = mirsys(1000,10);
    mip->IOBASE = 10;
    x = mirvar(0);
    if(argc == 2)
        fp = fopen(argv[1],"r");
    else if(argc==1)
        fp = stdin;
    else{
        fprintf(stderr, "Usage error");
        return -2;
    }

    if(!fp){
        fprintf(stderr, "cannot open file");
        return -1;
    }

    while(!feof(fp)){
        cinnum(x, fp);
        res = fermat(x);
        if(res){
            cotnum(x, stdout);
            fprintf(stdout, " is a prime number under 99.999%% probabililty\n");
        }
        else{
            cotnum(x, stdout);
            fprintf(stdout, " is not a prime\n");
        }
    }
    fclose(fp);
    mirkill(x);
    mirexit();
    return 0;
}