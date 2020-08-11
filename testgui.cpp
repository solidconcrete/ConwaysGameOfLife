#include <QtWidgets>
#include <QtTest/QtTest>
#include "dialog.h"
#include "ui_dialog.h"
#include <assert.h>
class TestGui: public QObject
{
    Q_OBJECT
private:
//    Dialog d;
private slots:
//    void testGui_data();
    void testSizeSpinner();

};
void TestGui::testSizeSpinner()
{
    int a = 5;
    assert(a == 6);
}



//uncomment this line and comment main class to run tests
QTEST_MAIN(TestGui)
#include "testgui.moc"
