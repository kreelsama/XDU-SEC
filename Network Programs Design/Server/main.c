#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<memory.h>
#include<math.h>
#include<sqlite3.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/select.h>
#include<signal.h>
#include<time.h>
#include"flag.h"

typedef struct ipport{
    char ip_addr[20];
    uint16_t port;
}ipport;

void raise_error(int);
static int query(void*, int , char**, char**);
static int ipquery(void*, int, char**, char**);
static int count(void*, int, char**, char**);
int run(const uint16_t, const char *);
int new_connect(int, struct sockaddr_in, sqlite3*);
int reset(const char*);
int create_socket(int, struct sockaddr_in*,char*);
int timeout(int, int);
void handler(int);
void quit();

void quit(){
    fprintf(stdout, "Process %d exited\n", getpid());
}

void raise_error(int type){
    switch(type){
        case 0:
            fprintf(stderr,"Usage Error!\n");
            fprintf(stdout,"Usage [run <port> <database file>] | [reset <database file>]\n");
            break;
        case 1:
            fprintf(stderr,"Cannot open database.\n");
            break;
        case 3:
            fprintf(stderr, "Cannot creat a scoket.\n");
            break;
        case 4:
            fprintf(stderr, "Bind error.\n");
            break;
        case 5:
            fprintf(stderr, "Listen error.\n");
            break;
        case 6:
            fprintf(stderr, "Accept error.\n");
            break;
        case 7:
            fprintf(stderr, "Send error.\n");
            break;
        default:
            break;
    }
    fprintf(stderr, strerror(errno));
    fprintf(stdout, "\n");
}

int timeout(int fd, int out){
    if(out == 0)
        return 1;
    fd_set read_fdset;
    struct timeval timeo;
    int ret;
    FD_ZERO(&read_fdset);
    FD_SET(fd,&read_fdset);
    timeo.tv_sec = out;
    timeo.tv_usec = 0;
    ret = select(fd+1, &read_fdset, NULL, NULL, &timeo);
    return ret;
}

void handler(int sig){
    fprintf(stdout, "\nReceived signal %d, exiting\n", sig);
    kill(0, SIGKILL);
    errno = EINTR;
}

static int query(void *result, int argc, char **argv, char **azColName){
    char *res = (char*) result;
    int i = 0;
    while(argv[i]!=NULL){
        strcat(res, argv[i]);
        strcat(res, " ");
        i++;
    }
    return 0;
}


static int ipquery(void *result, int argc, char **argv, char **azColName){
    ipport *res = (ipport*) result;
    if(!*argv){
        res->ip_addr[0] = 0;
        res->port = 0;
    }
    else {
        strcpy(res->ip_addr, argv[0]);
        res->port = atoi(argv[1]);
    }
    return 0;
}


static int count(void* cnt, int argc, char**argv, char**azColName){
    int *x = (int*)cnt;
    *x = atoi(argv[0]);
    return 0;
}


int create_socket(int port, struct sockaddr_in *srvaddr, char*inaddr){
    int sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd==-1){
        raise_error(3);
        exit(1);
    }

    bzero(srvaddr,sizeof(struct sockaddr_in));
    srvaddr->sin_family=AF_INET;
    srvaddr->sin_port=htons(port);
    if(!inaddr)
        srvaddr->sin_addr.s_addr=htonl(INADDR_ANY);
    else
        inet_aton(inaddr, &srvaddr->sin_addr);
    fprintf(stdout,"Message: Created socket at port: %d\n", port);
    if(bind(sockfd, (struct sockaddr *)srvaddr, sizeof(struct sockaddr_in))==-1){
        raise_error(4);
        fprintf(stdout, "%d\n", errno);
        close(sockfd);
        exit(1);
    }
    return sockfd;
}

int run(const uint16_t port, const char*fp){
    sqlite3 *db;
    char *err;
    int rc;
    rc = sqlite3_open(fp, &db);
    if(rc!=SQLITE_OK){
        raise_error(1);
        return -1;
    }
    else{
        fprintf(stdout, "Message: Connection to database established.\n");
    }
    char *sql5 = "DELETE FROM ONLINE;";
    if(SQLITE_OK!=sqlite3_exec(db, sql5, NULL, NULL, &err)){
        fprintf(stdout, "Cannot clear ONLINE");
    }
    char *sql="SELECT COUNT(*) FROM USERS;";
    int cnt = 0;
    rc = sqlite3_exec(db, sql, count, &cnt,&err);
    if(rc == SQLITE_OK){
        fprintf(stdout, "Message: There are %d registered users.\n", cnt);
    }
    else{
        fprintf(stderr, "Error %s\n", err);
        sqlite3_free(err);
    }
    /****Begin listening*****/
    int sockfd, client_fd;
    struct sockaddr_in my_addr, remote_addr;
    signal(SIGINT, handler);//QUIT Operation

    sockfd = create_socket(port, &my_addr, NULL);
    pid_t pid;
    while(1){
        bzero(&remote_addr, sizeof(remote_addr));
        fprintf(stdout, "Message: Begin listening\n");
        if(errno == EINTR){
            break;
        }
        if(listen(sockfd, 5)==-1) {
            raise_error(5);
            continue;
        }
        int sin_size = sizeof(struct sockaddr_in);

        client_fd = accept(sockfd, (struct sockaddr*)&remote_addr, &sin_size);

        if(client_fd == -1){
            raise_error(6);
            continue;
        }
        fprintf(stdout, "Message: Received connection from %s\n", inet_ntoa(remote_addr.sin_addr));
        if(!(pid=fork())) {
            new_connect(client_fd, remote_addr, db);
            return 0;
        }
        else{
            fprintf(stdout, "Message: Created pid %d for ip %s\n", pid, inet_ntoa(remote_addr.sin_addr));
        }
    }
    sql = "DELETE FROM ONLINE";
    if(SQLITE_OK!=sqlite3_exec(db, sql, NULL, NULL, &err)){
        fputs(err, stderr);
        sqlite3_free(err);
    }
    return 0;
}

int new_connect(int client_fd, struct sockaddr_in remote_addr, sqlite3 *db){
    int  sockfd;
    ssize_t rc;
    uint16_t port;
    int sin_size = sizeof(struct sockaddr_in);
    char buf[1024] = {'\0'};
    struct sockaddr_in my_addr;
    srand((unsigned int)time(NULL));
    port = 1024 + rand() % 40000;
    sprintf(buf,"%d", port);
    rc = send(client_fd, buf, sizeof(char)*strlen(buf),0 );
    if(rc == -1){
        raise_error(7);
        close(client_fd);
        return -1;
    }
    fprintf(stdout, "Message: Sended new port %d to %s\n", port, inet_ntoa(remote_addr.sin_addr));
    close(client_fd);

    sockfd = create_socket(port, &my_addr, inet_ntoa(remote_addr.sin_addr));
    if(listen(sockfd,1) == -1){
        raise_error(5);
        return -1;
    }
    if(!timeout(sockfd, 10)){
        fprintf(stdout, "Note: Host %s:%d doesn't repond, quitting...", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
    }
    bzero(&remote_addr, sizeof(struct sockaddr_in));
    if((client_fd = accept(sockfd, (struct sockaddr*)&remote_addr, &sin_size))==-1){
        fprintf(stdout, strerror(errno));
    }
    fprintf(stdout, "Note: Accept new connection from %s:%d\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
    int ret;
    ssize_t n;
    int login = 0, cnt = 0;
    char *errmsg;
    char name[100] = {'\0'};
    char sql[1024];
    while(1){
        fprintf(stdout, "Turn to initial state\n");
        bzero(sql, sizeof(sql));
        bzero(buf, sizeof(buf));
        n = recv(client_fd,buf, sizeof(buf), 0);
        if (n == 0){
            fprintf(stdout, "Received ZERO!\n");
            quit();
            break;
        }
        fprintf(stdout ,"Message: Received %s from %s:%d\n", buf, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
        //Main section
        if(!strcmp(buf, FLAG_CONTACT)){
            ipport ipinfo = {};
            char ip[25]={'\0'};
            bzero(buf, sizeof(buf));
            ret = timeout(client_fd, 0);
            if(!ret){
                continue;
            }
            read(client_fd, buf, sizeof(buf));
            strcpy(sql, "SELECT IP_ADDR, PORT FROM ONLINE WHERE NAME=\"");
            strcat(sql, buf);
            strcat(sql, "\";");
            if(SQLITE_OK!=sqlite3_exec(db, sql, ipquery, (void*) &ipinfo,&errmsg)){
                fputs(errmsg, stderr);
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                continue;
            }
            if(ipinfo.ip_addr[0] == 0 || ipinfo.port == 0){
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                continue;
            }
            sprintf(ip, "%s %d", ipinfo.ip_addr, ipinfo.port);
            fprintf(stdout,"Writing: %s\n", ip);
            if(0>=write(client_fd, ip, sizeof(ip))){
                continue;
            }
            continue;
        }
        else if(!strcmp(buf, FLAG_QUERY)){
            char contact[1024] = {'\0'};
            int c;
            strcpy(sql, "SELECT COUNT(*) FROM ONLINE;");
            if(SQLITE_OK!=sqlite3_exec(db,sql,count,&c,&errmsg)){
                fprintf(stdout, "%s\n", errmsg);
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                sqlite3_free(errmsg);
                continue;
            }
            sprintf(contact,"%d",c);
            fprintf(stdout, "Sending numbers of online users %d\n", c);
            n = send(client_fd,contact,sizeof(contact),0);
            if(n<=0){
                continue;
            }
            bzero(sql, sizeof(sql));
            bzero(contact, sizeof(contact));
            strcpy(sql, "SELECT NAME FROM ONLINE;");
            if(sqlite3_exec(db, sql, query, contact, &errmsg)!=SQLITE_OK){
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                fprintf(stderr, errmsg);
                continue;
            }
            write(client_fd, contact, sizeof(contact));
            continue;
        }
        else if(!strcmp(buf, FLAG_REGISTER)){
            if(read(client_fd, buf, sizeof(buf))<=0){
                continue;
            }
            char Name[100]={'\0'},  passwd[100]={'\0'};
            sscanf(buf,"%s %s", Name, passwd);
            
            sprintf(sql,"INSERT INTO USERS VALUES(\"%s\", \"%s\")", Name, passwd);
            fprintf(stdout, "Executing: %s\n", sql);
            if(sqlite3_exec(db, sql, NULL, NULL, &errmsg)!=SQLITE_OK){
                fputs(errmsg ,stderr);
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                continue;
            }
            write(client_fd, FLAG_OK, sizeof(FLAG_OK));
        }
        else if(!strcmp(buf, FLAG_QUIT)){
            break;
        }
        else if(!strcmp(buf, FLAG_LOGIN)){
            if(login){
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                continue;
            }
            ret = timeout(client_fd, 0);
            if(!ret){
                continue;
            }
            n = recv(client_fd,buf, sizeof(buf), 0);
            fprintf(stdout, "%s\n", buf);
            if(n<=0){
                if(errno == EINTR)
                    break;
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                continue;
            }
            int i = 0;
            for(i = 0; buf[i]!=' '; ++i)
                name[i] = buf[i];
            sprintf(sql, "SELECT COUNT(*) FROM USERS WHERE NAME=\"%s\" AND PASSWD= \"%s\";", name, buf+i+1);
            fprintf(stdout, "%s\n", sql);
            if(sqlite3_exec(db, sql, count, &cnt, &errmsg)!=SQLITE_OK){
                fputs(errmsg, stderr);
                sqlite3_free(errmsg);
                write(client_fd,FLAG_FAIL,sizeof(FLAG_FAIL));
                continue;
            }
            if(cnt == 1) {
                fprintf(stdout, "Message: Write FLAG_OK\n");
                write(client_fd, FLAG_OK, sizeof(FLAG_OK));
            }else{
                fprintf(stdout, "Message: Write FLAG_FAIL\n");
                write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
                continue;
            }
            bzero(buf, sizeof(buf));
            bzero(sql, sizeof(sql));
            n = read(client_fd, buf, sizeof(buf));//Reading port
            if(n<=0){
                fprintf(stdout, "ZERO RECEIVED IN LOGIN\n");
                continue;
            }
            sprintf(sql, "INSERT INTO ONLINE VALUES(\"%s\", \"%s\", %s);", name, inet_ntoa(remote_addr.sin_addr),buf);
            fprintf(stdout, "Executing %s\n",sql);
            if(sqlite3_exec(db, sql, count, &cnt, &errmsg)!=SQLITE_OK){
                fputs(errmsg, stderr);
                sqlite3_free(errmsg);
                continue;
            }
            login = 1;
        }

        else {
            puts(buf);
            write(client_fd, FLAG_FAIL, sizeof(FLAG_FAIL));
        }
    }
    bzero(sql, sizeof(sql));
    strcpy(sql, "DELETE FROM ONLINE WHERE NAME = \"");
    strcat(sql, name);
    strcat(sql, "\";");
    if(sqlite3_exec(db, sql, count, &cnt, &errmsg)!=SQLITE_OK){
        fputs(errmsg, stderr);
        sqlite3_free(errmsg);
    }
    close(client_fd);
    return 0;
}

int reset(const char* fp){
    sqlite3 *db;
    int rc;
    char* err = NULL;
    puts(fp);
    rc = sqlite3_open(fp,&db);
    if(rc!=SQLITE_OK){
        raise_error(1);
        return -1;
    }
    fprintf(stdout, "Reseting database...\n");
    char *sql1 = "CREATE TABLE USERS("\
          "NAME VARCHAR(1,100) UNIQUE  NOT NULL,"\
          "PASSWD VARHCAR(8,100) NOT NULL);";
    char *sql2 = "DELETE FROM USERS;";
    char *sql3 = "DROP TABLE USERS;";
    char *sql4 = "CREATE TABLE ONLINE("\
                 "NAME UNIQUE NOT NULL REFERENCES USERS(NAME),"\
                 "IP_ADDR BIG INT NOT NULL,"\
                 "PORT    INT     NOT NULL);";
    char *sql5 = "DELETE FROM ONLINE;";
    rc = sqlite3_exec(db, sql3, NULL, NULL, &err);
    if(rc == SQLITE_OK){
       fprintf(stdout, "Droping table successful\n");
    }
    else{
        fprintf(stderr, "%s\n", err);
    }
    rc = sqlite3_exec(db, sql1, NULL,  NULL, &err);
    if(rc==SQLITE_OK){
        fprintf(stdout, "Creating table USERS successful\n");
    }else{
        fprintf(stderr, "Error: %s", err);
        sqlite3_free(&err);
    }
    rc = sqlite3_exec(db, sql2, NULL, NULL, &err);
    if(rc==SQLITE_OK){
        fprintf(stdout, "Deleting users successful\n");
    }
    else{
        fprintf(stdout, "%s\n", err);
    }
    rc = sqlite3_exec(db, sql4, NULL, NULL, &err);
    if(rc==SQLITE_OK){
        fprintf(stdout, "Creating table ONLINE successful.\n");
    }else{
        fprintf(stdout,"%s\n", err);
    }
    rc = sqlite3_exec(db ,sql5, NULL, NULL, &err);
    if(rc==SQLITE_OK){
        fprintf(stdout, "Sweeping online users successful.\n");
    }
    fprintf(stdout, "Done\n");
    sqlite3_close(db);
    return 0;
}

int main(int argc, char*argv[]){
    if(argc<=2 || argc>=5){
        raise_error(0);
        return -1;
    }
    if(!strcmp(argv[1] ,"run")){
        int port = atoi(argv[2]);
        if(argc!=4){
            raise_error(0);
            return -1;
        }
        if(port<=0 || port>=65535){
            raise_error(0);
            fprintf(stderr, "Illegal port number\n");
            return -1;
        }
        int res = run(port, argv[3]);
    }
    else if(!strcmp(argv[1], "reset")){
        if(argc!=3){
            raise_error(0);
            return 1;
        }
        if(reset(argv[2])){
            return 2;
        }
    }
    else{
        raise_error(0);
        return -1;
    }
    return 0;
}