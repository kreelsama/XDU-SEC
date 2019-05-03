/********************************************************************************
** Form generated from reading UI file 'ui.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_H
#define UI_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UIClass
{
public:
    QTextEdit *cli;
    QPushButton *btn_plot;
    QPushButton *btn_cancel;
    QLabel *label;
    QPushButton *btn_clr;

    void setupUi(QWidget *UIClass)
    {
        if (UIClass->objectName().isEmpty())
            UIClass->setObjectName(QStringLiteral("UIClass"));
        UIClass->resize(652, 400);
        cli = new QTextEdit(UIClass);
        cli->setObjectName(QStringLiteral("cli"));
        cli->setGeometry(QRect(30, 40, 201, 281));
        btn_plot = new QPushButton(UIClass);
        btn_plot->setObjectName(QStringLiteral("btn_plot"));
        btn_plot->setGeometry(QRect(420, 340, 75, 23));
        btn_cancel = new QPushButton(UIClass);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setGeometry(QRect(530, 340, 75, 23));
        label = new QLabel(UIClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 54, 12));
        btn_clr = new QPushButton(UIClass);
        btn_clr->setObjectName(QStringLiteral("btn_clr"));
        btn_clr->setGeometry(QRect(300, 340, 75, 23));

        retranslateUi(UIClass);

        QMetaObject::connectSlotsByName(UIClass);
    } // setupUi

    void retranslateUi(QWidget *UIClass)
    {
        UIClass->setWindowTitle(QApplication::translate("UIClass", "UI", Q_NULLPTR));
        btn_plot->setText(QApplication::translate("UIClass", "Plot", Q_NULLPTR));
        btn_cancel->setText(QApplication::translate("UIClass", "Cancel", Q_NULLPTR));
        label->setText(QApplication::translate("UIClass", "Commands", Q_NULLPTR));
        btn_clr->setText(QApplication::translate("UIClass", "Clear", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class UIClass: public Ui_UIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_H
