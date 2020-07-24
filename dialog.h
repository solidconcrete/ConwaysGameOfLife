#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "delegate.h"
#include "myThread.h"
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

    QList<int> const checkNeighbors(QModelIndex &index);

    QList<QModelIndex> const findLiveCells();

//    QList<QModelIndex> const findCellsToKill();

//    QList<QModelIndex> const findCellsToResurrect();

    void killCells(QList<QModelIndex>);

    void resurrectCells(QList<QModelIndex>);

    int countAliveNeighbors(QModelIndex);

    void setUpVisibility(int margin);

    void makeStep();

    void setUpCellSize(int);

    void setUpModel();

    void setUpGridSize();

    void keyPressEvent(QKeyEvent *event);

    myThread *mThread;

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

public slots:
    void onMakeStep();

private:
    Ui::Dialog *ui;
    QStandardItemModel *model;
    Delegate *myDelegate;
    QList<QModelIndex> aliveCells;
};
#endif // DIALOG_H
