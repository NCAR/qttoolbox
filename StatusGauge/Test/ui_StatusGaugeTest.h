/********************************************************************************
** Form generated from reading ui file 'StatusGaugeTest.ui'
**
** Created: Thu Apr 24 12:01:51 2008
**      by: Qt User Interface Compiler version 4.2.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STATUSGAUGETEST_H
#define UI_STATUSGAUGETEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <Qt3Support/Q3MimeSourceFactory>

class Ui_StatusGaugeTest
{
public:
    QHBoxLayout *hboxLayout;

    void setupUi(QDialog *StatusGaugeTest)
    {
    StatusGaugeTest->setObjectName(QString::fromUtf8("StatusGaugeTest"));
    hboxLayout = new QHBoxLayout(StatusGaugeTest);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(9);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

    retranslateUi(StatusGaugeTest);

    QSize size(208, 636);
    size = size.expandedTo(StatusGaugeTest->minimumSizeHint());
    StatusGaugeTest->resize(size);


    QMetaObject::connectSlotsByName(StatusGaugeTest);
    } // setupUi

    void retranslateUi(QDialog *StatusGaugeTest)
    {
    StatusGaugeTest->setWindowTitle(QApplication::translate("StatusGaugeTest", "StatusGaugeTest", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(StatusGaugeTest);
    } // retranslateUi

};

namespace Ui {
    class StatusGaugeTest: public Ui_StatusGaugeTest {};
} // namespace Ui

#endif // UI_STATUSGAUGETEST_H
