/********************************************************************************
** Form generated from reading UI file 'contactwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTWIDGET_H
#define UI_CONTACTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactWidget
{
public:
    QPushButton *btn_chat;
    QPushButton *btn_logout;
    QPushButton *btn_refresh;
    QTextBrowser *usertable;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btn_file;

    void setupUi(QWidget *ContactWidget)
    {
        if (ContactWidget->objectName().isEmpty())
            ContactWidget->setObjectName(QStringLiteral("ContactWidget"));
        ContactWidget->resize(400, 336);
        btn_chat = new QPushButton(ContactWidget);
        btn_chat->setObjectName(QStringLiteral("btn_chat"));
        btn_chat->setGeometry(QRect(210, 300, 80, 25));
        btn_logout = new QPushButton(ContactWidget);
        btn_logout->setObjectName(QStringLiteral("btn_logout"));
        btn_logout->setGeometry(QRect(310, 300, 80, 25));
        btn_refresh = new QPushButton(ContactWidget);
        btn_refresh->setObjectName(QStringLiteral("btn_refresh"));
        btn_refresh->setGeometry(QRect(110, 300, 80, 25));
        usertable = new QTextBrowser(ContactWidget);
        usertable->setObjectName(QStringLiteral("usertable"));
        usertable->setGeometry(QRect(10, 30, 371, 192));
        lineEdit = new QLineEdit(ContactWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 250, 371, 31));
        label = new QLabel(ContactWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 230, 71, 17));
        label_2 = new QLabel(ContactWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 10, 61, 17));
        btn_file = new QPushButton(ContactWidget);
        btn_file->setObjectName(QStringLiteral("btn_file"));
        btn_file->setGeometry(QRect(10, 300, 80, 25));

        retranslateUi(ContactWidget);

        QMetaObject::connectSlotsByName(ContactWidget);
    } // setupUi

    void retranslateUi(QWidget *ContactWidget)
    {
        ContactWidget->setWindowTitle(QApplication::translate("ContactWidget", "Form", nullptr));
        btn_chat->setText(QApplication::translate("ContactWidget", "begin", nullptr));
        btn_logout->setText(QApplication::translate("ContactWidget", "logout", nullptr));
        btn_refresh->setText(QApplication::translate("ContactWidget", "refresh", nullptr));
        label->setText(QApplication::translate("ContactWidget", "Your choice", nullptr));
        label_2->setText(QApplication::translate("ContactWidget", "Usertable", nullptr));
        btn_file->setText(QApplication::translate("ContactWidget", "Sendfile", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ContactWidget: public Ui_ContactWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTWIDGET_H
