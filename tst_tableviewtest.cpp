#include <QtTest>
#include <QString>
#include <iostream>

#include <C:\Users\aleng\OneDrive\Desktop\Starview\mainwindow.h>
#include <C:\Users\aleng\OneDrive\Desktop\Starview\mainwindow.cpp>
#include <C:\Users\aleng\OneDrive\Desktop\Starview\main.cpp>
// add necessary includes here

class tableviewtest : public QObject
{
    Q_OBJECT

public:
    tableviewtest();
    ~tableviewtest();

private slots:
    void test_case1();
    void on_lineEdit_textChanged();
    void findDistance();
};

tableviewtest::tableviewtest()
{

}

tableviewtest::~tableviewtest(){

}

void tableviewtest::bool firstMatch()
{
    QVERIFY(true);

}
void tableviewtest::on_lineEdit_textChanged()
{
    QVERIFY(true);

}

void tableviewtest::findDistance()
{
    QVERIFY(true, "failure");

}

QTEST_APPLESS_MAIN(tableviewtest)

#include "tst_tableviewtest.moc"
