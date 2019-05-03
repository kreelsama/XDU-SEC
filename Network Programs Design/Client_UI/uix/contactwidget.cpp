#include "widgets.h"
#include "ui_contactwidget.h"


ContactWidget::ContactWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactWidget)
{
    ui->setupUi(this);
    connect(ui->btn_refresh, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(ui->btn_logout, SIGNAL(clicked()), this, SLOT(logout()));
    connect(ui->btn_chat, SIGNAL(clicked()), this, SLOT(chat()));
    refresh();
}

void ContactWidget::sendfile(){
    return;
    QString st = ui->lineEdit->text();
    if(st.isEmpty()){
        QMessageBox::information(this, "Error", "Input contact cannot be empty");
        return;
    }
    char buf [1024]{0};
    write(server_sockfd, FLAG_CONTACT, sizeof(FLAG_CONTACT));

    toString(st,buf, st.length());
    write(server_sockfd, buf, sizeof(buf));
    if(timeout(server_sockfd, 5)<0){
        QMessageBox::information(this, "Note", "Server not responding");
        return;
    }
    read(server_sockfd, buf, sizeof(buf));//Reading destination socket and port
    if(!strcmp(buf, FLAG_FAIL)){
        QMessageBox::information(this, "Error", "No such user");
        refresh();
        return;
    }
}


void ContactWidget::refresh(){
    char buf[1024]{0};
    write(server_sockfd, FLAG_QUERY, sizeof(FLAG_QUERY));
    read(server_sockfd, buf, sizeof(buf));
    if(!strcmp(buf, FLAG_FAIL)){
        return;
    }
    fprintf(stdout, "There're %s users online", buf);
    bzero(buf, sizeof(buf));
    read(server_sockfd, buf, sizeof(buf));
    QString lit(buf);
    lit.replace(' ', '\n');
    ui->usertable->setText(lit);
}


void ContactWidget::logout(){
    ssize_t size = write(server_sockfd, FLAG_QUIT, sizeof(FLAG_QUIT));
    fprintf(stdout, "Writing FLAG_QUIT to server, get respond code %ld\n",size);
    ::close(server_sockfd);
    exit_all(0);
    this->close();
}


void ContactWidget::chat(){
    QString st = ui->lineEdit->text();
    if(st.isEmpty()){
        QMessageBox::information(this, "Error", "Input contact cannot be empty");
        return;
    }
    char buf [1024]{0};
    write(server_sockfd, FLAG_CONTACT, sizeof(FLAG_CONTACT));

    toString(st,buf, st.length());
    write(server_sockfd, buf, sizeof(buf));
    if(timeout(server_sockfd, 5)<0){
        QMessageBox::information(this, "Note", "Server not responding");
        return;
    }
    read(server_sockfd, buf, sizeof(buf));//Reading destination socket and port
    if(!strcmp(buf, FLAG_FAIL)){
        QMessageBox::information(this, "Error", "No such user");
        refresh();
        return;
    }
        ChatWidget *w = new ChatWidget;
        char ip[20]{0};
        int port;
        sscanf(buf, "%s %d", ip, &port);
        fprintf(stdout, "Note: Received %s\n", buf);
        inet_aton(ip,&w->sock.sin_addr);
        w->sock.sin_port = htons(port);
        w->sock.sin_family = AF_INET;
        w->init(0);
}

ContactWidget::~ContactWidget()
{
    delete ui;
}
