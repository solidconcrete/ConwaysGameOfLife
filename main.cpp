//#include "dialog.h"
//#include "grid.h"
//#include "cellStruct.h"
//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Dialog w;
//    w.show();
//    return a.exec();
////    int w = 5, h = 5;
////        grid d(w,h);

////    QDebug deb = qDebug();
//////    d.setCellAlive(1,0);
////    d.setCellAlive(8,1);
////    d.setCellAlive(9,2);
////    d.setCellAlive(9,3);
////    d.setCellAlive(8,3);
////    d.setCellAlive(2,3);
//////    d.setCellAlive(3,2);

////    std::vector<cell> g = d.getGrid();
////    int index = 0;
////    for (int i = 0; i < w; i++)
////    {
////        for (int j = 0; j < h; j++)
////        {
////            if (g.at(index).isAlive)
////            {
////                deb << 1 ;
//////                << "(" << g.at(index).aliveNeighbors<< ")";
////            }
////            else
////            {
////                deb << 0;
//////                << "(" << g.at(index).aliveNeighbors<< ")";
////            }
////            index ++;
////        }
////        deb << "\n";
////    }
////    deb << "\n";
////    w = 3; h = 3;
////    d.setSize(w, h);
////    g = d.getGrid();
////    index = 0;
////    for (int i = 0; i < w; i++)
////    {
////        for (int j = 0; j < h; j++)
////        {
////            if (g.at(index).isAlive)
////            {
////                deb << 1 ;
//////                << "(" << g.at(index).aliveNeighbors<< ")";
////            }
////            else
////            {
////                deb << 0;
//////                << "(" << g.at(index).aliveNeighbors<< ")";
////            }
////            index ++;
////        }
////        deb << "\n";
////    }
////    for ( int k = 0; k < 20; k++)
////    {
////        index = 0;
////        deb << "\n";
////        deb << "\n";
////        d.makeStep();
////            g = d.getGrid();
////            index = 0;
////            for (int i = 0; i < 10; i++)
////            {
////                for (int j = 0; j < 10; j++)
////                {
////                    if (g.at(index).isAlive)
////                    {
////                        deb << 1 ;
////        //                << "(" ;<< g.at(index).aliveNeighbors<< ")";
////                    }
////                    else
////                    {
////                        deb << 0 ;
////        //                    << "(" << g.at(index).aliveNeighbors<< ")";
////                    }
////                    index ++;
////                }
////                deb << "\n";
////            }
////    }
//}
