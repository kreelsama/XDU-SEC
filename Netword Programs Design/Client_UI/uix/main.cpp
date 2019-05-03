#include "widgets.h"
#include <QApplication>

uint16_t localport;
struct sockaddr_in server_addr;
int server_sockfd;
pid_t listenpid;

void raise_error(int type, QWidget*parent){
    switch(type){
        case 1:
            QMessageBox::information(parent, "Error", "Cannot connect the server");
            break;
        case 3:
            QMessageBox::information(parent, "Error", "Cannot create the socket");
            break;
        case 4:
            QMessageBox::information(parent, "Error", "Bind error.");
            break;
        case 5:
            QMessageBox::information(parent, "Error", "Listen error.");
            break;
        case 6:
            QMessageBox::information(parent, "Error", "Accept error.");
            break;
        case 7:
            QMessageBox::information(parent, "Error", "Send error.");
            break;
        default:
            break;
    }
}


int timeout(int fd, int timeout){
    fd_set read_fdset;
    struct timeval timeo;
    int ret;
    FD_ZERO(&read_fdset);
    FD_SET(fd,&read_fdset);
    timeo.tv_sec = timeout;
    timeo.tv_usec = 0;
    ret = select(fd+1, &read_fdset, nullptr, nullptr, &timeo);
    return ret;
}

void toString(QString q, char*txt, int len){
    using std::string;
    string x(q.toStdString());
    int i = 0;
    for(auto c:x){
        txt[i] = c;
        i++;
    }
    txt[i] = '\0';
}

void SigHandler(int){
    fprintf(stdout, "pid = %d recerived SIGKILL\n", getpid());
    exit(0);
}


int listen_proc(int port){
    signal(SIGKILL, SigHandler);
    using std::default_random_engine;
    default_random_engine e;
    std::uniform_int_distribution<uint16_t> u(1024, 65535);
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<=0)
        return -1;
    bzero(&addr, sizeof(addr));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr))){
        qDebug("Bind error\n");
        return -4;
    }
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buf[1024]{0};
    ssize_t size;
    while(1){
        if(listen(sockfd, 5) == -1){
            qDebug("Listen Error\n");
            return -2;
        }
        e.seed(time(nullptr));

        int newfd = ::accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);
        fprintf(stdout, "Message: Listen Process Received connection from %s\n", inet_ntoa(client_addr.sin_addr));
        if(!fork()){
            int x = 0;
            int newsockfd;
            uint16_t nport;
            struct sockaddr_in sock;
            socklen_t len = sizeof(sock);
            QApplication a(x, nullptr);
            ChatWidget newchat;
            newsockfd = socket(AF_INET, SOCK_STREAM, 0);
            bzero(buf, sizeof(buf));

            do{
                nport = u(e);
                addr.sin_addr.s_addr = htonl(INADDR_ANY);
                addr.sin_family = AF_INET;
                addr.sin_port = htons(nport);
                if(0>bind(newsockfd, (struct sockaddr*) &addr, sizeof(sock))){
                    x = 0;
                    continue;
                }
                x = 1;
            }while(x == 0);
            sprintf(buf, "%d", nport);
            if(listen(newsockfd, 5) == -1){
                qDebug("Listen Error\n");
                return -2;
            }
            size = write(newfd,buf, strlen(buf));//Send new port
            newsockfd = ::accept(newsockfd, (struct sockaddr*) &newchat.sock, &addr_size);
            newchat.sockread = newsockfd;

            recvfrom(newsockfd, buf, sizeof(buf), 0, (struct sockaddr*)&newchat.sock, &len );
            ::close(newfd);
            fprintf(stdout, "Listen Proc Received %s from %s:%d\n",buf, inet_ntoa(newchat.sock.sin_addr), ntohs(newchat.sock.sin_port));
            sendto(newsockfd, FLAG_OK, sizeof(FLAG_OK), 0, (struct sockaddr*)&newchat.sock, len);

            newchat.init(1);
            newchat.show();
            return a.exec();
        }
    }
}

void exit_all(int x){
    fprintf(stdout, "Sweeping all child processes from pid=%d\n", getpid());
    kill(listenpid, SIGKILL);//kill all processes
    exit(3);
}

int main(int argc, char *argv[])
{
    using std::default_random_engine;
    default_random_engine e;
    std::uniform_int_distribution<uint16_t> u(1024, 65535);
    e.seed(time(nullptr));
    localport = u(e);
    if(!(listenpid=fork())){
        fprintf(stdout, "Listen process started at port %d\n", localport);
        listen_proc(localport);
        return 0;
    }
    QApplication a(argc, argv);
    ConnectWidget b;
    b.show();
    return a.exec();
}
