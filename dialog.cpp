#include "dialog.h"
#include "ui_dialog.h"

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

    //table set-up
    model = new QStandardItemModel(5, 5, this);
    ui->tableView->setModel(model);
    myDelegate = new Delegate(this);
    ui->tableView->setItemDelegate(myDelegate);
    ui->tableView->setMouseTracking(true);

    paintGrid();

}

Dialog::~Dialog()
{
    isStopped = true;
    delete ui;
}

//thread
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

            auto end = std::chrono::system_clock::now();
            auto elapsed = end - start;

            if (elapsed  < std::chrono::milliseconds(latency))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(latency) - elapsed);
            }

            paintGrid();
            mutex.unlock();
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

    int width = ui->XspinBox->value();
    int height = ui->YspinBox->value();

    model->setColumnCount(width);
    model->setRowCount(height);

    g.changeSize(width, height);
    paintGrid();

    this->setFocus();
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
    if (!isStopped)
    {
        return;
    }

    g.setGridEmpty();
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
        for(int col = 0; col < model->columnCount(); col++)
        {
            QModelIndex cellIndex = model->index(row, col, QModelIndex());
            model->setData(cellIndex, cells[row * model->columnCount() + col].isAlive);
        }
    }
}

void Dialog::on_tableView_entered(const QModelIndex &index)
{
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
