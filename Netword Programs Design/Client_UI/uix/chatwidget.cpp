#include "widgets.h"
#include "ui_chatwidget.h"



ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    connect(ui->btn_send, SIGNAL(clicked()), this, SLOT(sendmsg()));
    connect(ui->btn_recv, SIGNAL(clicked()), this, SLOT(recvmsg()));
    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(terminate()));
}


void ChatWidget::init(int flag){
    if(flag == 0){
        if(this->newconnect()!=0){
            return;
        }
    }
    this->show();
    //th = new ::thread{&ChatWidget::recvmsg, this};
    //th->detach();
}


int ChatWidget::newconnect(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<=0){
        QMessageBox::information(this, "Error", "Socket error");
        return -1;
    }
    if(::connect(sockfd, (struct sockaddr*)&sock, sizeof(sock)) == -1){
        QMessageBox::information(this, "Error", strerror(errno));
        return -1;
    }
    char buf[1024]{0};

    read(sockfd, buf, sizeof(buf));//Receiving remote new port
    rport = atoi(buf);// set remote port

    fprintf(stdout, "Message: Start new connection to %s:%s\n" ,inet_ntoa(sock.sin_addr), buf);
    ::close(sockfd);

    sock.sin_port = htons(atoi(buf));
    sockread = socket(AF_INET, SOCK_STREAM, 0);//Create TCP socket

    ::connect(sockread, (struct sockaddr*) &sock, sizeof(sock));
    if(sockread<=0){
        QMessageBox::information(this, "Error", "Socket error");
        return 2;
    }
    if(0>=write(sockread, FLAG_OK, sizeof(FLAG_OK))){
        QMessageBox::information(this, "Error", strerror(errno));
    }

    if(0>=read(sockread, buf, sizeof(buf))){
        QMessageBox::information(this, "Error", strerror(errno));
    }
    else{
        fprintf(stdout, "Receive %s from %s:%d\n", buf, inet_ntoa(sock.sin_addr), ntohs(sock.sin_port));
    }
    return 0;
}


void ChatWidget::recvmsg(){
    char msg[1024]{0};
    socklen_t len = sizeof(sock);
    while(true){
        ssize_t lens;
        lens = read(sockread, msg,sizeof(msg));
        if(lens<=0){
            fprintf(stdout, "Error:%s\n", strerror(errno));
            return;
        }
        else{
            std::cout<<"Message Received:"<<msg<<std::endl;
        }
        if(!strcmp(msg, FLAG_QUIT)){
            sendmsg(FLAG_QUIT);
            //do_update(1, "Sender terminates the dialog");
            return ;
        }
        //do_update(1, msg);
    }
}


void ChatWidget::sendmsg(const char* s ){
    char msg[1024]{0};
    if(!s){
        QString txt = ui->Input->text();
        toString(txt, msg, txt.length());
    }
    else{
        strcpy(msg, s);
    }
    do_update(2, s);
    if(write(sockread, msg, strlen(msg))<=0){
        QMessageBox::information(this, "Error", strerror(errno));
    }
}


void ChatWidget::terminate(){
    sendmsg(FLAG_QUIT);
    ::close(sockread);
    this->close();//pids[0] closes automotically
}

void ChatWidget::do_update(int s, const char*str){
    time_t rawtime;
    QString disp;
    struct tm *info;
    char tmp[21]{0};
    message msg;
    time(&rawtime);
    info = localtime( &rawtime );
    strftime(tmp, 20, "%H:%M:%S", info);
    msg.timestamp = new QString(tmp);
    if(s==1){
        msg.type = new QString("Received\n");
    }
    else{
        msg.type = new QString("Sended\n");
    }
    if(!str)
        msg.mes = new QString(ui->Input->text());
    else
        msg.mes = new QString(str);
    hist.push_back(msg);
    for(auto p:hist){
       disp.append(p.timestamp);
       disp.append(' ');
       disp.append(p.type);
       disp.append(p.mes);
       disp.append('\n');
    }
    ui->History->setText(disp);
    ui->History->moveCursor(QTextCursor::End);//scroll to the bottom
}


void ChatWidget::sendfile(){
    QMessageBox::information(this, "Note", "Not implementated yet");
}

ChatWidget::~ChatWidget()
{
    for(auto &p : hist){
        delete p.mes;
        delete p.timestamp;
        delete p.type;
    }
    delete ui;
}
