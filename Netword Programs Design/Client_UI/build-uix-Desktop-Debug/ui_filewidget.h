/********************************************************************************
** Form generated from reading UI file 'filewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEWIDGET_H
#define UI_FILEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileWidget
{
public:
    QProgressBar *progressBar;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *FileWidget)
    {
        if (FileWidget->objectName().isEmpty())
            FileWidget->setObjectName(QStringLiteral("FileWidget"));
        FileWidget->resize(400, 136);
        progressBar = new QProgressBar(FileWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(40, 70, 301, 31));
        progressBar->setValue(24);
        label = new QLabel(FileWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 80, 21, 17));
        label_2 = new QLabel(FileWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 40, 141, 17));

        retranslateUi(FileWidget);

        QMetaObject::connectSlotsByName(FileWidget);
    } // setupUi

    void retranslateUi(QWidget *FileWidget)
    {
        FileWidget->setWindowTitle(QApplication::translate("FileWidget", "Form", nullptr));
        label->setText(QApplication::translate("FileWidget", "Bar", nullptr));
        label_2->setText(QApplication::translate("FileWidget", "filename", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileWidget: public Ui_FileWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEWIDGET_H
