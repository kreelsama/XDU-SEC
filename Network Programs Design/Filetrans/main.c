#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>
#include <stdlib.h>
#include <signal.h>
#define FLAG_END "FLAG_END"
#define PORT 6699


void sighandler(int sig){
    fprintf(stdout, "Signal %d received, aborting transmission.\n", sig);
    errno = EINTR;
}

int server_proc(){
    int sockfd, newfd;
    struct sockaddr_in server_addr, client_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        fprintf(stderr, "Create socket error: %s\n", strerror(errno));
        return 0;
    }
    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        fprintf(stderr, "Bind error: %s", strerror(errno));
        return 0;
    }
    socklen_t len = sizeof(server_addr);
    while(1) {
        if (listen(sockfd, 1) < 0) {
            fprintf(stderr, "Listen error: %s", strerror(errno));
            return 0;
        }
        newfd = accept(sockfd, (struct sockaddr *) &client_addr, &len);
        if ( newfd< 0) {
            fprintf(stderr, "Connect error: %s\n", strerror(errno));
            return 0;
        }
        char buf[1025] = {'\0'};
        char filename[30] = {""}, tmpname[35] = {0};
        if (read(newfd, filename, sizeof(filename)) <= 0) {//receive filename
            fprintf(stderr, "Read error: %s\n", strerror(errno));
            return 0;
        }
        FILE *fp;
        unsigned int pt = 0;
        fp = fopen(filename, "a");
        sprintf(tmpname, "%s.tmp", filename);
        FILE *fptmp = fopen(tmpname, "r+");
        if(!fptmp) {
            fptmp = fopen(tmpname, "w+");
            write(newfd, "0", sizeof("0"));
        }
        else{
            bzero(buf, sizeof(buf));
            fscanf(fptmp, "%s", buf);
            fprintf(stdout, "%s\n", buf);
            pt = atoi(buf);
            write(newfd, buf, sizeof(buf));
        }

        int end = 0;
        while (1) {
            bzero(buf, sizeof(buf));
            if (read(newfd, buf, 1024) <= 0) {
                fprintf(stdout, "Server terminated the file tansmitting\n");
                fprintf(stdout, "Written file byts: %d\n", pt);
                break;
            }
            //fprintf(stdout, buf);
            if (!strcmp(buf, FLAG_END)) {
                fprintf(stdout, "Server has finished transmitting\n");
                end = 1;
                break;
            }
            fwrite(buf, strlen(buf), 1, fp);
            pt += strlen(buf);
        }
        close(newfd);
        fclose(fp);

        bzero(buf, sizeof(buf));
        if (end == 1) {
            sprintf(buf, "rm -f %s", tmpname);
            if (!system(buf)) {
                fprintf(stdout, "Clearing %s\n", tmpname);
                fprintf(stdout, "Read %d chars\n", pt);
            }
            else{
                fprintf(stdout, "Delete file %s error.\n", tmpname);
            }
        }
        else{
            fseek(fptmp, 0, SEEK_SET);
            sprintf(buf, "%d", pt);
            fwrite(buf, strlen(buf), 1, fptmp);
        }
        fclose(fptmp);
    }
    return 0;
}


int client_proc(char *filename){
    struct sockaddr_in remote_addr;
    bzero(&remote_addr, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &remote_addr.sin_addr);
    remote_addr.sin_port = htons(PORT);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        fprintf(stderr, "Create socket error: %s\n", strerror(errno));
        return 0;
    }
    if(connect(sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr))<0){
        fprintf(stderr, "Cannot connect to the server: %s\n", strerror(errno));
        return 0;
    }
    FILE *fp = fopen(filename, "r");
    write(sockfd, filename, strlen(filename));
    char buf[1025] = {0};
    read(sockfd, buf, 1024);
    int size = atoi(buf);
    fseek(fp, size, SEEK_SET);
    signal(SIGINT, sighandler);
    fprintf(stdout, "Begin sending file %s\n", filename);
    int i = 0;
    while(!feof(fp)){
        i ++;
        if(i >= 20)
            return 0;
        bzero(buf, sizeof(buf));
        fread(buf, 1024, 1, fp);
        if(errno == EINTR){
            return 0;
        }
        if(write(sockfd, buf, 1024) <0 ){
            fprintf(stdout, "Server died... exiting");
            break;
        }
    }
    write(sockfd, FLAG_END, sizeof(FLAG_END));
    fclose(fp);
    close(sockfd);
    return 0;
}




int main(int argc, char*argv[]) {
    //Two modes: Receive mode and send mode
    if(!strcmp(argv[1], "receive")){
        return server_proc();
    }
    else if(!strcmp(argv[1], "send")){
        return client_proc(argv[2]);
    }
    else{
        fprintf(stderr, "Usage error");
    }
    return 0;
}