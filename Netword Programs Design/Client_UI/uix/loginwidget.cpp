#include "widgets.h"
#include "ui_loginwidget.h"


LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    connect(ui->btnreg, SIGNAL(clicked()), this, SLOT(reg()));
    connect(ui->btnsub, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->btnclose, SIGNAL(clicked()), this, SLOT(xclose()));
    name = nullptr;
}


void LoginWidget::reg(){
    char buf[100]{0}, txt[100]{0};
    QString n = ui->name->text();
    QString m = ui->passwd->text();
    if(n.isEmpty() || m.isEmpty()){
        QMessageBox::information(this, "Error", "You have to input all the uernames and password to proceed");
        return;
    }
    if(n.contains(' ')||m.contains(' ')){
        QMessageBox::information(this, "Error", "Username and password cannot contain spaces");
        return;
    }
    if(m.length()<8){
        QMessageBox::information(this, "Error", "Password must be at least 8 characters");
        return;
    }
    if(0>=write(server_sockfd, FLAG_REGISTER, sizeof(FLAG_REGISTER))){
        QMessageBox::information(this, "Error", "Write Error!");
        return;
    }
    if(name){
        delete name;
    }
    name = new char[n.length() + 1]{0};
    toString(n, name, n.length());
    toString(m, buf, m.length());
    sprintf(txt, "%s %s", name, buf);

    if(write(server_sockfd, txt, strlen(txt))<=0){
        QMessageBox::information(this, "Error", "Write Error");
        return;
    }
    bzero(buf, sizeof(buf));
    if(0>=read(server_sockfd, buf, sizeof(buf))){
        QMessageBox::information(this, "Error", "Read error");
        return;
    }
    if(!strcmp(buf, FLAG_FAIL)){
        QMessageBox::information(this, "Error", "Fail to Register");
        return;
    }
    else if(!strcmp(buf, FLAG_OK)){
        QMessageBox::information(this, "Success", "Register successfully");
        return;
    }
    else{
        QMessageBox::information(this, "Error", buf);
    }
}



void LoginWidget::login(){
    signal(SIGPIPE, exit_all);
    QString n = ui->name->text();
    QString m = ui->passwd->text();
    char buf[100]{0}, txt[100]{0};
    if(n.isEmpty() || m.isEmpty()){
        QMessageBox::information(this, "Error", "You have to input all the uernames and password to proceed");
        return;
    }
    if(n.contains(' ')||m.contains(' ')){
        QMessageBox::information(this, "Error", "Username and password cannot contain spaces");
        return;
    }
    if(m.length()<8){
        QMessageBox::information(this, "Error", "Password must be at least 8 characters");
        return;
    }
    if(0>= write(server_sockfd, FLAG_LOGIN, sizeof(FLAG_LOGIN))){
        QMessageBox::information(this, "Error", "Write Error!");
        return;
    }
    if(name)
        delete name;
    name = new char[n.length() + 1]{0};
    toString(n, name, n.length());
    toString(m, buf, m.length());
    name[n.length()] = 0;
    buf[m.length()] = 0;
    sprintf(txt, "%s %s", name, buf);
    fprintf(stdout, "Write:%s %s\n", name, buf);
    if(write(server_sockfd, txt, strlen(txt) )<=0){
        QMessageBox::information(this, "Error", "Write Error");
        return;
    }
    bzero(buf, sizeof(buf));
    if(timeout(server_sockfd, 5)<0){
        QMessageBox::information(this, "Note", "Server not responding");
        return;
    }
    if(0>=read(server_sockfd, buf, sizeof(buf))){
        QMessageBox::information(this, "Error", "Read error");
        return;
    }
    fprintf(stdout, "Received %s\n", buf);
    if(!strcmp(buf, FLAG_FAIL)){
        QMessageBox::information(this, "Error", "Login Error");
    }
    else if(!strcmp(buf, FLAG_OK)){
        QMessageBox::information(this, "Success", "You have loggin successfully");
        bzero(buf, sizeof(buf));
        sprintf(buf, "%d", localport);
        if(0>=write(server_sockfd, buf, strlen(buf) )){
            QMessageBox::information(this, "Error", "Cannot connect to server");
            return;
        }
        ContactWidget *c = new ContactWidget;
        c->show();
        this->close();
    }
    else{
        QMessageBox::information(this, "Error", buf);
    }
}

void LoginWidget::xclose(){
    ::close(server_sockfd);
    exit_all(0);
    this->close();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}
