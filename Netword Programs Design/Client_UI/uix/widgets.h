#ifndef WIDGETS_H
#define WIDGETS_H

#include <QWidget>
#include <QApplication>
#include <QString>
#include <QTextEdit>
#include <QTextBrowser>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QProcess>
#include <QString>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <QApplication>
#include <netinet/in.h>
#include <QProgressBar>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <thread>
#include <pthread.h>
#include <ctime>
#include <errno.h>
#include <random>
#include <signal.h>


using std::vector;
using std::thread;
namespace Ui {
class LoginWidget;
class ConnectWidget;
class ChatWidget;
class ContactWidget;
class FileWidget;
}

//int create_socket(uint16_t, struct sockaddr_in, char*);
int timeout(int, int);
void raise_error(int, QWidget*);
void toString(QString, char*, int);
int listen_proc(int);
void SigHandler(int);
void exit_all(int);//Can only be called from parent processes;
extern pid_t listenpid;
extern uint16_t localport;
extern struct sockaddr_in server_addr;
extern int server_sockfd;


typedef struct {
    QString *timestamp;
    QString *type;
    QString *mes;
}message;

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    char *name;
    //int server_sockfd;
    //struct sockaddr_in server_sock;

public slots:
    void reg();
    void login();
    void xclose();

private:
    Ui::LoginWidget *ui;
};



class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    int newconnect();
    struct sockaddr_in sock;
    ~ChatWidget();
    int sockread;
    int lport, rport;
    void init(int);


public slots:
    void sendmsg(const char*s = nullptr);
    void recvmsg();
    void sendfile();
    void terminate();

private:
    void do_update(int, const char*);
    vector<message> hist;
    Ui::ChatWidget *ui;
    ::thread *th;
    pid_t pids[2]{0};

};



class ConnectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectWidget(QWidget *parent = nullptr);
    ~ConnectWidget();
    struct sockaddr_in remote_addr;
    int sockfd;

public slots:
    void connect_server();
    void zclose();
private:
    Ui::ConnectWidget *ui;
    QPushButton *pushButton1;
    QPushButton *pushButton2;
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
};


class ContactWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContactWidget(QWidget *parent = nullptr);
    int sockfd;
    char*name = nullptr;

    ~ContactWidget();
public slots:
    void refresh();
    void chat();
    void sendfile();
    void logout();
private:
    Ui::ContactWidget *ui;
};


class FileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileWidget(QWidget *parent = nullptr);
    ~FileWidget();

private:
    Ui::FileWidget *ui;
};


#ifndef PROJ_FLAG_H
#define PROJ_FLAG_H
#define FLAG_OK "FLAG_OK"
#define FLAG_FAIL "FLAG_FAIL"
#define FLAG_END "FLAG_END"
#define FLAG_SYNC "FLAG_SYNC"
#define FLAG_QUERY "FLAG_QUERY"
#define FLAG_CONTACT "FLAG_CONTACT"
#define FLAG_REGISTER "FLAG_REGISTER"
#define FLAG_QUIT    "FLAG_QUIT"
#define FLAG_LOGIN   "FLAG_LOGIN"
#define FLAG_ACK  "FLAG_ACK"
#define FLAG_FILE "FLAG_FILE"
#endif

#endif
