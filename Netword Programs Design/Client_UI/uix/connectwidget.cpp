#include "widgets.h"
#include "ui_connectwidget.h"

ConnectWidget::ConnectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectWidget)
{
    ui->setupUi(this);
    pushButton1 = new QPushButton(this);
    pushButton1->setObjectName(QStringLiteral("pushButton"));
    pushButton1->setGeometry(QRect(200, 80, 80, 21));
    pushButton2 = new QPushButton(this);
    pushButton2->setObjectName(QStringLiteral("pushButton_2"));
    pushButton2->setGeometry(QRect(300, 80, 80, 21));
    pushButton1->setText(QApplication::translate("ConnectWidget", "Connect", nullptr));
    pushButton2->setText(QApplication::translate("ConnectWidget", "Exit", nullptr));
    lineEdit1 = new QLineEdit(this);
    lineEdit1->setObjectName(QStringLiteral("lineEdit1"));
    lineEdit1->setGeometry(QRect(50, 40, 113, 25));
    lineEdit2 = new QLineEdit(this);
    lineEdit2->setObjectName(QStringLiteral("lineEdit2"));
    lineEdit2->setGeometry(QRect(230, 40, 113, 25));
    connect(pushButton1, SIGNAL(clicked()), this, SLOT(connect_server()));
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(zclose()));

}

ConnectWidget::~ConnectWidget()
{
    delete ui;
    delete pushButton1;
    delete pushButton2;
    delete lineEdit1;
    delete lineEdit2;
}

void ConnectWidget::zclose(){
    fprintf(stdout, "Closing at connection phase\n");
    exit_all(0);
    this->close();
}


void ConnectWidget::connect_server(){
    QString ip = lineEdit1->text();
    QString port = lineEdit2->text();
    if(ip.isEmpty() || port.isEmpty()){
        QMessageBox::information(this, "Error", "Server ip address and port can't be empty");
        return;
    }
    char *txt = new char[ip.length()]{0};
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    toString(ip, txt, ip.length());
    txt[ip.length()] = 0;
    fprintf(stdout, "Message: input ip address: %s\n", txt);
    inet_aton(txt, &server_addr.sin_addr);
    toString(port, txt, port.length());
    txt[port.length()] = 0;
    fprintf(stdout, "Message: input port: %s\n", txt);
    server_addr.sin_port = htons(atoi(txt));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        raise_error(3, this);
        return;
    }
    if(::connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))==-1){
        raise_error(1, this);
        ::close(sockfd);
        return ;
    }
    char buf[30]{0};
    if(timeout(sockfd, 5)==0){
        QMessageBox::information(this, "Note", "Server not responding");
        return;
    }
    if(recv(sockfd, buf, sizeof(buf), 0)<=0){
        QMessageBox::information(this, "Error", "Recv error");
        return;
    }
    fprintf(stdout, "Message: Receive new server port: %s\n", buf);
    if(!atoi(buf)){
        fprintf(stderr,"Error: Server port invalid, exiting...\n" );
        return;
    }
    //The second connection
    toString(ip, txt, ip.length());
    txt[ip.length()] = 0;
    inet_aton(txt, &server_addr.sin_addr);
    server_addr.sin_port = htons(atoi(buf));
    ::close(sockfd);
    if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0))<=0 ){
        raise_error(3, this);
        return;
    }
    fprintf(stdout, "Message: Attempting to connect %s:%s\n", inet_ntoa(server_addr.sin_addr), buf);
    sleep(1);
    if(::connect(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr) )){
        raise_error(1, this);
        return;
    }
    delete[]txt;
    QMessageBox::information(this,"Note", "Connection Established");
    //Make a new widget
    LoginWidget *x = new LoginWidget;

    x->show();
    this->close();
}
