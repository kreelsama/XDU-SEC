/********************************************************************************
** Form generated from reading UI file 'chatwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWIDGET_H
#define UI_CHATWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWidget
{
public:
    QTextBrowser *History;
    QLineEdit *Input;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btn_send;
    QPushButton *btn_exit;
    QPushButton *btn_file;

    void setupUi(QWidget *ChatWidget)
    {
        if (ChatWidget->objectName().isEmpty())
            ChatWidget->setObjectName(QStringLiteral("ChatWidget"));
        ChatWidget->resize(389, 444);
        History = new QTextBrowser(ChatWidget);
        History->setObjectName(QStringLiteral("History"));
        History->setGeometry(QRect(20, 50, 351, 241));
        Input = new QLineEdit(ChatWidget);
        Input->setObjectName(QStringLiteral("Input"));
        Input->setGeometry(QRect(20, 330, 351, 41));
        label = new QLabel(ChatWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 54, 17));
        label_2 = new QLabel(ChatWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 300, 54, 17));
        btn_send = new QPushButton(ChatWidget);
        btn_send->setObjectName(QStringLiteral("btn_send"));
        btn_send->setGeometry(QRect(180, 400, 80, 25));
        btn_exit = new QPushButton(ChatWidget);
        btn_exit->setObjectName(QStringLiteral("btn_exit"));
        btn_exit->setGeometry(QRect(290, 400, 80, 25));
        btn_file = new QPushButton(ChatWidget);
        btn_file->setObjectName(QStringLiteral("btn_file"));
        btn_file->setGeometry(QRect(70, 400, 80, 25));

        retranslateUi(ChatWidget);

        QMetaObject::connectSlotsByName(ChatWidget);
    } // setupUi

    void retranslateUi(QWidget *ChatWidget)
    {
        ChatWidget->setWindowTitle(QApplication::translate("ChatWidget", "Form", nullptr));
        label->setText(QApplication::translate("ChatWidget", "History", nullptr));
        label_2->setText(QApplication::translate("ChatWidget", "Input", nullptr));
        btn_send->setText(QApplication::translate("ChatWidget", "Send", nullptr));
        btn_exit->setText(QApplication::translate("ChatWidget", "Exit", nullptr));
        btn_file->setText(QApplication::translate("ChatWidget", "Send File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWidget: public Ui_ChatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIDGET_H
