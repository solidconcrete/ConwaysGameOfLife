#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <chrono>
#include "delegate.h"
#include "grid.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

    friend class TestGui;

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:

    void on_SizePushButton_clicked();

    void on_stepButton_clicked();

    void on_startButton_clicked();

    void on_pauseButton_clicked();

    void on_speedSlider_valueChanged(int value);

    void on_cellSizeSlider_valueChanged(int value);

    void on_clearButton_clicked();

    void on_tableView_entered(const QModelIndex &index);

    void on_tableView_pressed(const QModelIndex &index);

private:

    Ui::Dialog *ui;
    QStandardItemModel *model;
    Delegate *myDelegate;
    std::mutex mutex;
    bool isStopped;
    unsigned int latency;
    grid g;

    void setUpCellSize(int);

    void setUpModel();

    void setUpGridSize();

    void keyPressEvent(QKeyEvent *event);

    void passGridToLogic();

    void paintGrid();

    void run();

};
#endif // DIALOG_H
