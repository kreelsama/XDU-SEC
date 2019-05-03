/********************************************************************************
** Form generated from reading UI file 'loginwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QPushButton *btnsub;
    QPushButton *btnclose;
    QPushButton *btnreg;
    QLineEdit *name;
    QLineEdit *passwd;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName(QStringLiteral("LoginWidget"));
        LoginWidget->resize(400, 175);
        btnsub = new QPushButton(LoginWidget);
        btnsub->setObjectName(QStringLiteral("btnsub"));
        btnsub->setGeometry(QRect(200, 140, 80, 25));
        btnclose = new QPushButton(LoginWidget);
        btnclose->setObjectName(QStringLiteral("btnclose"));
        btnclose->setGeometry(QRect(300, 140, 80, 25));
        btnreg = new QPushButton(LoginWidget);
        btnreg->setObjectName(QStringLiteral("btnreg"));
        btnreg->setGeometry(QRect(90, 140, 80, 25));
        name = new QLineEdit(LoginWidget);
        name->setObjectName(QStringLiteral("name"));
        name->setGeometry(QRect(60, 30, 261, 31));
        passwd = new QLineEdit(LoginWidget);
        passwd->setObjectName(QStringLiteral("passwd"));
        passwd->setGeometry(QRect(60, 90, 261, 31));
        label = new QLabel(LoginWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 10, 41, 17));
        label_2 = new QLabel(LoginWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 70, 54, 17));

        retranslateUi(LoginWidget);

        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QApplication::translate("LoginWidget", "Form", nullptr));
        btnsub->setText(QApplication::translate("LoginWidget", "login", nullptr));
        btnclose->setText(QApplication::translate("LoginWidget", "cancel", nullptr));
        btnreg->setText(QApplication::translate("LoginWidget", "register", nullptr));
        label->setText(QApplication::translate("LoginWidget", "Name", nullptr));
        label_2->setText(QApplication::translate("LoginWidget", "Password", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
