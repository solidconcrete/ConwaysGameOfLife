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

    g = grid(5, 5);
    isStopped = true;
    latency = 500;

    qRegisterMetaType<QVector<int>>("QVectorMeta");

    std::thread thread (&Dialog::run, this);
    thread.detach();

    model = new QStandardItemModel(5, 5, this);
    setUpModel();
    ui->tableView->setModel(model);
    myDelegate = new Delegate(this);
    ui->tableView->setItemDelegate(myDelegate);
    ui->tableView->setMouseTracking(true);

}

Dialog::~Dialog()
{
    isStopped = true;
    delete ui;
}

void Dialog::run()
{
    bool localIsStopped = isStopped;

    while(1)
    {
        if (!localIsStopped)
        {
            auto start = std::chrono::system_clock::now();
            mutex.lock();
            g.makeStep();
            paintGrid();
            mutex.unlock();
            auto end = std::chrono::system_clock::now();
            auto elapsed = end - start;
            if (elapsed  < std::chrono::milliseconds(latency))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(latency) - elapsed);
            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(latency));
        }
        localIsStopped = isStopped;
    }
}

//set up grid size
void Dialog::on_SizePushButton_clicked()
{
    if (!isStopped)
    {
        return;
    }
    QString labelText = "Width: ";
    int width = ui->XspinBox->value();
    int height = ui->YspinBox->value();
    labelText.append(QString::number(width));
    labelText.append(", Height: ");
    labelText.append(QString::number(height));
    ui->sizeLabel->setText(labelText);

    model->setColumnCount(width);
    model->setRowCount(height);

    g.changeSize(width, height);
    setUpModel();

    this->setFocus();

}

//sets up a model so that newly added cells are marked as dead, but without killing living cells
void Dialog::setUpModel()
{
    paintGrid();
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
        paintGrid();
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
    latency = 1100 - value;
}

void Dialog::on_cellSizeSlider_valueChanged(int value)
{
    setUpCellSize(value);
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && isStopped)
    {
        g.makeStep();
        paintGrid();
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
    g.setEmpty();
    paintGrid();

    this->setFocus();
}

void Dialog::paintGrid()
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
    if (QApplication::mouseButtons().testFlag(Qt::LeftButton) && isStopped)
    {
        model->setData(index, 1);
        g.setCellAlive(index.column(), index.row());
    }
    if (QApplication::mouseButtons().testFlag(Qt::RightButton) && isStopped)
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
