#include "dialog.h"
#include "ui_dialog.h"
#include <QThread>
#include <QString>
#include <thread>
#include <mutex>
#include <chrono>
#include "delegate.h"
#include "grid.h"
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    g = grid();
    isStopped = true;


    qRegisterMetaType<QVector<int>>("aaa");

    std::thread th1 (&Dialog::run, this);
    th1.detach();

    model = new QStandardItemModel(10, 10, this);
    setUpModel(10, 10);
    ui->tableView->setModel(model);
    myDelegate = new Delegate(this);
    ui->tableView->setItemDelegate(myDelegate);
    ui->tableView->setMouseTracking(true);

//    mThread = new myThread(this);
//    connect(mThread, SIGNAL(makeStep()), this, SLOT(onMakeStep()));
}

Dialog::~Dialog()
{
    isStopped = true;
    delete ui;
}

void Dialog::run()
{
    bool localIsStopped = isStopped;
    int i = 0;
    while(1)
    {
        if (!localIsStopped)
        {
            mutex.lock();
            g.makeStep();
            getGridFromLogid();
            mutex.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));



        }
        localIsStopped = isStopped;
    }
}

//set up grid size
void Dialog::on_SizePushButton_clicked()
{
    QString labelText = "Width: ";
    int width = ui->XspinBox->value();
    int height = ui->YspinBox->value();
    labelText.append(QString::number(width));
    labelText.append(", Height: ");
    labelText.append(QString::number(height));
    ui->sizeLabel->setText(labelText);

    model->setColumnCount(width);
    model->setRowCount(height);

    setUpModel(width, height);

    this->setFocus();

}

//sets up a model so that newly added cells are marked as dead, but without killing living cells
void Dialog::setUpModel(int w, int h)
{
    g.setSize(w, h);
    for (int col = 0; col < model->columnCount(); col++)
    {
        for (int row = 0; row < model->rowCount(); row++)
        {
            QModelIndex index = model->index(row, col, QModelIndex());
            if (index.data() != 1)
            {
                model->setData(index, 0);
            }
        }
    }
}

void Dialog::setUpCellSize(int cellSize)
{
    ui->tableView->horizontalHeader()->setDefaultSectionSize(cellSize);
    ui->tableView->verticalHeader()->setDefaultSectionSize(cellSize);
}

void Dialog::on_stepButton_clicked()
{
    if (isStopped)
    {
        g.makeStep();
        getGridFromLogid();
    }
    this->setFocus();
}

void Dialog::on_startButton_clicked()
{
    isStopped = false;
}

void Dialog::on_pauseButton_clicked()
{
    isStopped = true;
    this->setFocus();
}

void Dialog::on_speedSlider_valueChanged(int value)
{
    latency = value;
}

void Dialog::on_cellSizeSlider_valueChanged(int value)
{
    setUpCellSize(value);
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        g.makeStep();
        getGridFromLogid();
        return;
    }
    if (event->key() == Qt::Key_Plus)
    {
        ui->cellSizeSlider->setValue(ui->cellSizeSlider->value() + 1);
        return;
    }
    if (event->key() == Qt::Key_Minus)
    {
        ui->cellSizeSlider->setValue(ui->cellSizeSlider->value() - 1);

        return;
    }

}

void Dialog::on_clearButton_clicked()
{
    g.setSize(model->columnCount(), model->rowCount());
    getGridFromLogid();

    this->setFocus();
}

void Dialog::getGridFromLogid()
{
    std::vector<cell> cells = g.getGrid();
    if(model->columnCount() * model->rowCount() != (int)cells.size())
    {
        throw "Grid dimensions do not match with grid in memory";
    }
    for (int row = 0; row < model->rowCount(); row++)
    {
        for(int col = 0; col < model->columnCount(); col ++)
        {
            QModelIndex cellIndex = model->index(row, col, QModelIndex());
            model->setData(cellIndex, cells[row * model->columnCount() + col].isAlive);
        }
    }
}

void Dialog::on_tableView_entered(const QModelIndex &index)
{
//    QApplication::keyboardModifiers().testFlag(Qt::ControlModifier
    if (QApplication::mouseButtons().testFlag(Qt::LeftButton))
    {
        model->setData(index, 1);
        g.setCellAlive(index.column(), index.row());
    }
    if (QApplication::mouseButtons().testFlag(Qt::RightButton))
    {
        model->setData(index, 0);
        g.setCellDead(index.column(), index.row());
    }
    this->setFocus();
}

void Dialog::on_tableView_pressed(const QModelIndex &index)
{
    if(index.data().toInt() == 1)
    {
        model->setData(index, 0);
        g.setCellDead(index.column(), index.row());
    }
    else
    {
        model->setData(index, 1);
        g.setCellAlive(index.column(), index.row());
    }

    ui->tableView->setModel(model);
    this->setFocus();
}
