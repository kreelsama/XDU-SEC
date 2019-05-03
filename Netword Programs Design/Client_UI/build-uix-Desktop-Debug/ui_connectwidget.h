/********************************************************************************
** Form generated from reading UI file 'connectwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTWIDGET_H
#define UI_CONNECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectWidget
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *ConnectWidget)
    {
        if (ConnectWidget->objectName().isEmpty())
            ConnectWidget->setObjectName(QStringLiteral("ConnectWidget"));
        ConnectWidget->resize(400, 127);
        label = new QLabel(ConnectWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 40, 21, 17));
        label_2 = new QLabel(ConnectWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(200, 40, 31, 17));

        retranslateUi(ConnectWidget);

        QMetaObject::connectSlotsByName(ConnectWidget);
    } // setupUi

    void retranslateUi(QWidget *ConnectWidget)
    {
        ConnectWidget->setWindowTitle(QApplication::translate("ConnectWidget", "ConnectWidget", nullptr));
        label->setText(QApplication::translate("ConnectWidget", "IP", nullptr));
        label_2->setText(QApplication::translate("ConnectWidget", "PORT", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectWidget: public Ui_ConnectWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTWIDGET_H
