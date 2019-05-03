#include "sm2.h"

int doublepoint(big px, big py, big rx, big ry);
int multipoint(big px, big py, big n, big rx, big ry);
int addpoints(big ax, big ay, big bx, big by, big rx, big ry);
void init();
void mod(big x, big n, big res);
void inv(big x, big n, big res);
void mirfree(big mr, ...);
int verify(big x, big y);
void num2hex(big num, char *hex);
void hex2point(char*hex, big x, big y);
void point2hex(big x, big y, char* hex);
void KDF(char *hex, int klen, big t);
void encrypt(file inPlainText, file inKey, file outCipher);
void decrypt(file inCipher, file inKey, file outPlainText);
void genkey(file privout, file pubout);

//(0,0) stands for O
big a, b, p, n, h;//fixed parameters.
big Gx, Gy;
int l;

void init(){
    a = mirvar(1);
    b = mirvar(1);
    p = mirvar(23);
    cinstr(a, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC");
    cinstr(b, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93");
    cinstr(p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF");

    Gx = mirvar(3);
    Gy = mirvar(10);
    n = mirvar(28);
    h = mirvar(1);
    cinstr(Gx, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7");
    cinstr(Gy, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0");
    cinstr(n,  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123");
    l = 32;
}

void mod(big x, big n, big res){
    big one = mirvar(1);
    powmod(x, one, n, res);
    mirkill(one);
}

void inv(big x, big n, big res){
    big tmp = mirvar(0);
    xgcd(x, n, res, tmp, tmp);
    mirkill(tmp);
}

void mirfree(big mr, ...){
    //Variable parameters
    va_list arg_ptr;
    big arg_value = mr;
    va_start(arg_ptr, mr);

    do{
        mirkill(arg_value);
        arg_value = va_arg(arg_ptr, big);
    }while(arg_value != NULL);

    va_end(arg_ptr);
}

int verify(big x, big y){
    big tx, ty, ax, two, three;
    tx = mirvar(0);
    ty = mirvar(0);
    ax = mirvar(0);
    two = mirvar(2);
    three = mirvar(3);
    powmod(y, two, p, ty);
    powmod(x, three, p, tx);
    multiply(x, a, ax);
    add(tx, ax, tx);
    add(tx, b, tx);
    mod(tx, p, tx);
    if(mr_compare(tx, ty))
        return 0;
    else
        return 1;
}

int addpoints(big ax, big ay, big bx, big by, big rx, big ry){
    //Add two dots: (ax, ay) + (bx, by) = (rx, ry)
    //Note that ay+by != 0
    if(!mr_compare(ax, bx) && !mr_compare(ay, by)){
        return  doublepoint(ax, ay, rx, ry);
    }
    if(!verify(ax, ay)){
        copy(bx, rx);
        copy(by, ry);
        return 0;
    }
    if(!verify(bx, by)){
        copy(ax, rx);
        copy(ay, ry);
        return 0;
    }
    if(!mr_compare(ax, bx) ){
        fprintf(stdout, "Infty point encountered\n");
        zero(rx);
        zero(ry);
        return -1;
    }
    big x, y, lambda, lambda_square, rxx, ryy;
    big  two;
    x = mirvar(0);
    y = mirvar(0);
    rxx = mirvar(0);
    ryy = mirvar(0);
    two = mirvar(2);
    add(ay, by, x);
    lambda = mirvar(0);
    lambda_square = mirvar(0);
    mod(x, p, x);
    if(!mr_compare(x, y)){
        mirfree(x, y, lambda, lambda_square, two, NULL);
        fprintf(stderr, "Invalid addition operation.\n");
        return -1;
    }
    //Calculate lambda
    subtract(by, ay, y);
    mod(y, p, y);
    subtract(bx, ax, x);
    mod(x, p, x);
    inv(x, p, x);
    multiply(x, y, x);
    mod(x, p, lambda);

    powmod(lambda, two, p, lambda_square);//lambda^2
    mod(lambda_square, p, lambda_square);

    //calculate rx
    subtract(lambda_square, ax, rxx);
    subtract(rxx, bx, rxx);
    mod(rxx, p, rxx);
    //calculate ry
    subtract(ax, rxx, x);
    multiply(x, lambda, x);
    subtract(x, ay, ryy);
    mod(ryy, p, ryy);

    zero(rx);
    zero(ry);
    copy(rxx, rx);
    copy(ryy, ry);
    mirfree(x, y, lambda, lambda_square, two, rxx, ryy, NULL);
    return 0;
}

int multipoint(big px, big py, big n, big rx, big ry){
    //Imitating of the fast modular algorithm
    big two = mirvar(2), one = mirvar(1), zero = mirvar(0);
    big times, tx, ty, nx, ny, tmp;
    times = mirvar(0);
    tx = mirvar(0);
    ty = mirvar(0);
    tmp = mirvar(0);
    nx = mirvar(0);
    ny = mirvar(0);
    copy(n, times);
    copy(px, nx);
    copy(py, ny);
    mod(times, two, tmp);
    if(!mr_compare(tmp, one)){
        copy(px, tx);
        copy(py, ty);
    }
    sftbit(times, -1, times);
    while(mr_compare(times, zero)>0){
        doublepoint(nx, ny, nx, ny);
        mod(times, two, tmp);
        if(!mr_compare(tmp, one))
            addpoints(nx, ny, tx, ty, tx, ty);
        sftbit(times, -1, times);//times /= 2
    }

    copy(tx, rx);
    copy(ty, ry);
    mirfree(one, two, zero, times, tx, ty, nx, ny, tmp, NULL);
    return 0;
}

int doublepoint(big px, big py, big rx, big ry){
    big one = mirvar(1), two = mirvar(2), three = mirvar(3);
    big lambda, lambda_square, x, y, rxx, ryy;
    lambda = mirvar(0);
    lambda_square = mirvar(0);
    x = mirvar(0);
    y = mirvar(0);
    rxx = mirvar(0);
    ryy = mirvar(0);
    powmod(px, two, p, x);
    multiply(x, three, x);
    add(x, a, x);
    mod(x, p, x);

    multiply(py, two, y);
    inv(y, p, y);
    multiply(x, y, lambda);
    mod(lambda, p, lambda);

    powmod(lambda, two, p, lambda_square);

    subtract(lambda_square, px, rxx);
    subtract(rxx, px, rxx);
    mod(rxx, p, rxx);

    subtract(px, rxx, x);
    multiply(x, lambda, ryy);
    subtract(ryy, py, ryy);
    mod(ryy, p, ryy);

    copy(rxx, rx);
    copy(ryy, ry);
    mirfree(one, two, three, lambda, lambda_square, x, y, rxx, ryy, NULL);
    return 0;
}

void num2hex(big num, char *hex) {
    char hexstring[500] = {0};
    cotstr(num, hexstring);
    size_t padlen = 2*l - strlen(hexstring);//Length must be 64 hexs
    for(int i = 0; i < padlen; i++)
        hex[i] = '0';
    strcat(hex, hexstring);
}

void hex2point(char*hex, big x, big y){
    if(strlen(hex) != 4*l + 2){
        fprintf(stderr, "Invalid hex length\n");
        return;
    }
    if(!(hex[0]=='0'&&hex[1]=='4')){
        fprintf(stderr, "Invalid hex string\n");
        return;
    }
    cinstr(y, hex + 66);
    char xs[65] = {0};
    strncpy(xs, hex + 2, 64);
    cinstr(x, xs);
}


void point2hex(big x, big y, char *hex) {
    hex[0] = '0';
    hex[1] = '4';
    char tmp[500]={0};
    num2hex(x, hex+2);
    num2hex(y, hex+2*l+2);
}

void KDF(char *hex, int klen, big t) {//hex must consist of 128 hexs
    const int v = 256/8;//256 bit sm3 output
    unsigned int ct = 1;
    char res[5000] = {0};
    char hash[65] = {0};
    for(int i = 0; i < klen/v; i++){
        memset(hash, 0, sizeof(hash));
        char wait[500] = {0};
        snprintf(wait, 500, "%s%08x", hex, ct);
        SM3(wait, hash);
        strcat(res, hash);
        ct ++;
    }
    res[klen] = 0;
    cinstr(t, res);
}

void encrypt(file inPlainText, file inKey, file outCipher) {
    big one = mirvar(1), pbx, pby, k, plain, tx, ty;
    char string[1000] = {0}, plaintext[5000] = {0};
    char C1[1000] = {0}, C2[5000] = {0}, C3[5000] = {0};
    int ch;
    for(int i = 0; !feof(inPlainText); i++){
        ch = fgetc(inPlainText);
        if(ch == -1)
            break;
        snprintf(plaintext + 2*i, 3, "%02x", ch);
    }
    int klen = (int) strlen(plaintext);
    fprintf(stdout, "Get hex of the plaintext with the length of %d : %s\n", klen, plaintext);
    pbx = mirvar(0);
    pby = mirvar(0);
    k = mirvar(0);
    plain = mirvar(0);
    tx = mirvar(0);
    ty = mirvar(0);
    cinstr(plain, plaintext);
    irand((unsigned int)time(NULL));

    do {
        bigrand(n, k);
    }while(!mr_compare(k, one));

    multipoint(Gx, Gy, k, tx, ty);
    point2hex(tx, ty, C1);

    fprintf(outCipher, "%s", C1);
    fprintf(stdout, "Writing coordinates:%s\n", C1);
    memset(string, 0,sizeof(string));
    for(int i = 0; !feof(inKey); i++){
        ch = fgetc(inKey);
        if(ch == -1 )
            break;
        string[i] = (char) ch;
    }
    hex2point(string, pbx, pby);
    multipoint(pbx, pby, one, tx, ty);
    if(!verify(tx, ty)){
        fprintf(stderr, "Pubkey is invalid...\n");
        mirfree(one, pbx, pby, tx, ty, k, plain, NULL);
        return;
    }
    multipoint(pbx, pby, k, tx, ty);
    big t = mirvar(0);
    memset(string, 0,sizeof(string));
    point2hex(tx, ty, string);
    KDF(string + 2, klen, t);
    mr_xor(plain, t, t);
    cotstr(t, C2);
    fprintf(outCipher, "%s", C2);
    fprintf(stdout, "Writing ciphertext: %s\n", C2);

    memset(string, 0,sizeof(string));
    num2hex(tx, C3);
    strcat(C3, plaintext);
    num2hex(ty, string);
    strcat(C3, string);
    memset(string, 0, sizeof(string));
    SM3(C3,string);
    fprintf(stdout, "Writing hash: %s\n", string);
    fprintf(outCipher, "%s", string);
    mirfree(one, pbx, pby, k, plain, tx, ty, t, NULL);
}

void genkey(file privout, file pubout){
    char hex[5000] = {0};
    irand((unsigned int)time(NULL));
    big privkey;
    privkey = mirvar(0);
    bigrand(n, privkey);
    fprintf(stdout, "Generated private key:");
    cotnum(privkey, stdout);
    num2hex(privkey, hex);
    fwrite(hex, sizeof(char), strlen(hex), privout);
    memset(hex, 0, sizeof(hex));
    big px, py;
    px = mirvar(0);
    py = mirvar(0);
    multipoint(Gx, Gy, privkey, px, py);
    point2hex(px, py, hex);
    fprintf(stdout , "Generated public key: %s\n", hex+2);
    fwrite(hex, sizeof(char), strlen(hex), pubout);
    mirfree(privkey, px, py, NULL);
}

void decrypt(file inCipher, file inKey, file outPlainText) {
    char hex_raw[5000] = {0};
    int cur = 0;
    while(!feof(inCipher)){
        int ch = fgetc(inCipher);
        if(ch == -1)//EOF
            break;
        if(!isxdigit(ch))
            continue;
        hex_raw[cur] = (char)ch;
        ++cur;
    }
    size_t length = strlen(hex_raw);
    //130 bytes prefix is C1
    //the middle is C2
    //64 bytes in the rear is C3
    char C1[200] = {0}, C2[5000] = {0}, C3[5000] = {0};
    strncpy(C1, hex_raw, 130);
    strncpy(C2, hex_raw + 130, length - 130 - 64);
    strncpy(C3, hex_raw + length - 64, 64);
    fprintf(stdout, "Coordinates derived in hex: %s\n", C1);
    fprintf(stdout,"Cipher derived in hex %s\n", C2);
    big x, y, cipher, tx, ty, dB, t, msg;
    int klen = length - 130 - 64;
    x = mirvar(0);
    y = mirvar(0);
    cipher = mirvar(0);
    tx = mirvar(0);
    ty = mirvar(0);
    dB = mirvar(0);
    t = mirvar(0);
    msg = mirvar(0);
    hex2point(C1, x, y);
    if(!verify(x, y)){
        fprintf(stderr, "Not in the curve...\n");
        return;
    }
    cinnum(dB, inKey);
    cinstr(cipher, C2);
    multipoint(x, y, dB, tx, ty);
    char hex[5000] = {0};
    point2hex(tx, ty, hex);
    KDF(hex + 2, klen, t);
    mr_xor(cipher, t, msg);
    memset(hex, 0, sizeof(hex));
    num2hex(tx, hex);
    cotstr(msg, hex+64);
    num2hex(ty, hex + 64 + klen);
    char hash[65]= {0};
    SM3(hex, hash);
    fprintf(stdout, "Expected hash:  %s\n", C3);
    fprintf(stdout, "Calculated hash:%s\n", hash);
    if(strcmp(hash, C3)){
        fprintf(stdout, "Hash check dosen't pass\n");
        return;
    }
    else{
        fprintf(stdout, "Hash check passed!\n");
    }
    memset(hex, 0, sizeof(hex));
    cotstr(msg, hex);
    char result[5000]={0};
    size_t len = strlen(hex) / 2;
    for(int i = 0; i < len; i++){
        sscanf(hex + 2*i, "%02x", result + i);
    }
    fprintf(stdout, "Message is: %s\n", result);
    fwrite(result, sizeof(char), strlen(result), outPlainText);
    mirfree(x, y, cipher, tx, ty, dB, t, msg, NULL);
}


int main(int argc, char* argv[]) {
    //Usage: ./sm3 <enc|dec> <plaintext file|ciphertext file> <key file>

    miracl *mip;
    mip = mirsys(5000, 16);
    mip->IOBASE = 16;
    if(argc!=5 && argc!= 4){
        fprintf(stderr, "Usage:./sm3 <enc|dec> <plaintext file|ciphertext file> <key file> <output file>\n"
                        "       ./sm3 gen <private key file> <public key file>\n");
        return -1;
    }
    init();
    if(argc == 4 ){
        if(strcmp(argv[1], "gen")){
            fprintf(stderr, "Usage error.\n");
            return 1;
        }
        file prikey = fopen(argv[2], "w");
        file pubkey = fopen(argv[3], "w");
        if(!prikey || !pubkey){
            fprintf(stderr, "Cannot open file to write: %s", strerror(errno));
            return 3;
        }
        genkey(prikey, pubkey);
        fclose(prikey);
        fclose(pubkey);
    }

    if(argc == 5){
        file tf = fopen(argv[2], "r");
        file kf = fopen(argv[3], "r");
        file of = fopen(argv[4], "w");

        if(!tf || !kf || !of){
            fprintf(stderr, "File input/output error BEFORE the procedure: %s;\n", strerror(errno));
            return -1;
        }

        if(!strcmp(argv[1], "enc")){
            encrypt(tf, kf, of);
        }
        else if(!strcmp(argv[1], "dec")){
            decrypt(tf, kf, of);
        }
        else{
            fprintf(stderr, "Usage error.\n");
            return 0;
        }
        fclose(tf);
        fclose(of);
        fclose(kf);
    }

    mirfree(a, b, p, n, h, Gx, Gy, NULL);

    return 0;
}