#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QDebug>
#include <iostream>

void MainWindow::initGui(){
    pencil.push_back( ui->A1 );
    pencil.push_back( ui->A2 );
    pencil.push_back( ui->A3 );
    pencil.push_back( ui->A4 );
    pencil.push_back( ui->A5 );
    pencil.push_back( ui->A6 );
    pencil.push_back( ui->A7 );
    pencil.push_back( ui->A8 );
    pencil.push_back( ui->A9 );
    pencil.push_back( ui->A1_2 );
    pencil.push_back( ui->A2_2 );
    pencil.push_back( ui->A3_2 );
    pencil.push_back( ui->A4_2 );
    pencil.push_back( ui->A5_2 );
    pencil.push_back( ui->A6_2 );
    pencil.push_back( ui->A7_2 );
    pencil.push_back( ui->A8_2 );
    pencil.push_back( ui->A9_2 );
    pencil.push_back( ui->A1_3 );
    pencil.push_back( ui->A2_3 );
    pencil.push_back( ui->A3_3 );
    pencil.push_back( ui->A4_3 );
    pencil.push_back( ui->A5_3 );
    pencil.push_back( ui->A6_3 );
    pencil.push_back( ui->A7_3 );
    pencil.push_back( ui->A8_3 );
    pencil.push_back( ui->A9_3 );
    pencil.push_back( ui->A1_4 );
    pencil.push_back( ui->A2_4 );
    pencil.push_back( ui->A3_4 );
    pencil.push_back( ui->A4_4 );
    pencil.push_back( ui->A5_4 );
    pencil.push_back( ui->A6_4 );
    pencil.push_back( ui->A7_4 );
    pencil.push_back( ui->A8_4 );
    pencil.push_back( ui->A9_4 );
    pencil.push_back( ui->A1_5 );
    pencil.push_back( ui->A2_5 );
    pencil.push_back( ui->A3_5 );
    pencil.push_back( ui->A4_5 );
    pencil.push_back( ui->A5_5 );
    pencil.push_back( ui->A6_5 );
    pencil.push_back( ui->A7_5 );
    pencil.push_back( ui->A8_5 );
    pencil.push_back( ui->A9_5 );
    pencil.push_back( ui->A1_6 );
    pencil.push_back( ui->A2_6 );
    pencil.push_back( ui->A3_6 );
    pencil.push_back( ui->A4_6 );
    pencil.push_back( ui->A5_6 );
    pencil.push_back( ui->A6_6 );
    pencil.push_back( ui->A7_6 );
    pencil.push_back( ui->A8_6 );
    pencil.push_back( ui->A9_6 );
    pencil.push_back( ui->A1_7 );
    pencil.push_back( ui->A2_7 );
    pencil.push_back( ui->A3_7 );
    pencil.push_back( ui->A4_7 );
    pencil.push_back( ui->A5_7 );
    pencil.push_back( ui->A6_7 );
    pencil.push_back( ui->A7_7 );
    pencil.push_back( ui->A8_7 );
    pencil.push_back( ui->A9_7 );
    pencil.push_back( ui->A1_8 );
    pencil.push_back( ui->A2_8 );
    pencil.push_back( ui->A3_8 );
    pencil.push_back( ui->A4_8 );
    pencil.push_back( ui->A5_8 );
    pencil.push_back( ui->A6_8 );
    pencil.push_back( ui->A7_8 );
    pencil.push_back( ui->A8_8 );
    pencil.push_back( ui->A9_8 );
    pencil.push_back( ui->A1_9 );
    pencil.push_back( ui->A2_9 );
    pencil.push_back( ui->A3_9 );
    pencil.push_back( ui->A4_9 );
    pencil.push_back( ui->A5_9 );
    pencil.push_back( ui->A6_9 );
    pencil.push_back( ui->A7_9 );
    pencil.push_back( ui->A8_9 );
    pencil.push_back( ui->A9_9 );


    clues.push_back( ui->a1 );
    clues.push_back( ui->a2 );
    clues.push_back( ui->a3 );
    clues.push_back( ui->a4 );
    clues.push_back( ui->a5 );
    clues.push_back( ui->a6 );
    clues.push_back( ui->a7 );
    clues.push_back( ui->a8 );
    clues.push_back( ui->a9 );
    clues.push_back( ui->b1 );
    clues.push_back( ui->b2 );
    clues.push_back( ui->b3 );
    clues.push_back( ui->b4 );
    clues.push_back( ui->b5 );
    clues.push_back( ui->b6 );
    clues.push_back( ui->b7 );
    clues.push_back( ui->b8 );
    clues.push_back( ui->b9 );
    clues.push_back( ui->c1 );
    clues.push_back( ui->c2 );
    clues.push_back( ui->c3 );
    clues.push_back( ui->c4 );
    clues.push_back( ui->c5 );
    clues.push_back( ui->c6 );
    clues.push_back( ui->c7 );
    clues.push_back( ui->c8 );
    clues.push_back( ui->c9 );
    clues.push_back( ui->d1 );
    clues.push_back( ui->d2 );
    clues.push_back( ui->d3 );
    clues.push_back( ui->d4 );
    clues.push_back( ui->d5 );
    clues.push_back( ui->d6 );
    clues.push_back( ui->d7 );
    clues.push_back( ui->d8 );
    clues.push_back( ui->d9 );
    clues.push_back( ui->e1 );
    clues.push_back( ui->e2 );
    clues.push_back( ui->e3 );
    clues.push_back( ui->e4 );
    clues.push_back( ui->e5 );
    clues.push_back( ui->e6 );
    clues.push_back( ui->e7 );
    clues.push_back( ui->e8 );
    clues.push_back( ui->e9 );
    clues.push_back( ui->f1 );
    clues.push_back( ui->f2 );
    clues.push_back( ui->f3 );
    clues.push_back( ui->f4 );
    clues.push_back( ui->f5 );
    clues.push_back( ui->f6 );
    clues.push_back( ui->f7 );
    clues.push_back( ui->f8 );
    clues.push_back( ui->f9 );
    clues.push_back( ui->g1 );
    clues.push_back( ui->g2 );
    clues.push_back( ui->g3 );
    clues.push_back( ui->g4 );
    clues.push_back( ui->g5 );
    clues.push_back( ui->g6 );
    clues.push_back( ui->g7 );
    clues.push_back( ui->g8 );
    clues.push_back( ui->g9 );
    clues.push_back( ui->h1 );
    clues.push_back( ui->h2 );
    clues.push_back( ui->h3 );
    clues.push_back( ui->h4 );
    clues.push_back( ui->h5 );
    clues.push_back( ui->h6 );
    clues.push_back( ui->h7 );
    clues.push_back( ui->h8 );
    clues.push_back( ui->h9 );
    clues.push_back( ui->i1 );
    clues.push_back( ui->i2 );
    clues.push_back( ui->i3 );
    clues.push_back( ui->i4 );
    clues.push_back( ui->i5 );
    clues.push_back( ui->i6 );
    clues.push_back( ui->i7 );
    clues.push_back( ui->i8 );
    clues.push_back( ui->i9 );

}

void MainWindow::getGui(){
    std::vector<unsigned short> line;
    line.push_back( ui->a1->text().toInt() );
    line.push_back( ui->a2->text().toInt() );
    line.push_back( ui->a3->text().toInt() );
    line.push_back( ui->a4->text().toInt() );
    line.push_back( ui->a5->text().toInt() );
    line.push_back( ui->a6->text().toInt() );
    line.push_back( ui->a7->text().toInt() );
    line.push_back( ui->a8->text().toInt() );
    line.push_back( ui->a9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->b1->text().toInt() );
    line.push_back( ui->b2->text().toInt() );
    line.push_back( ui->b3->text().toInt() );
    line.push_back( ui->b4->text().toInt() );
    line.push_back( ui->b5->text().toInt() );
    line.push_back( ui->b6->text().toInt() );
    line.push_back( ui->b7->text().toInt() );
    line.push_back( ui->b8->text().toInt() );
    line.push_back( ui->b9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->c1->text().toInt() );
    line.push_back( ui->c2->text().toInt() );
    line.push_back( ui->c3->text().toInt() );
    line.push_back( ui->c4->text().toInt() );
    line.push_back( ui->c5->text().toInt() );
    line.push_back( ui->c6->text().toInt() );
    line.push_back( ui->c7->text().toInt() );
    line.push_back( ui->c8->text().toInt() );
    line.push_back( ui->c9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->d1->text().toInt() );
    line.push_back( ui->d2->text().toInt() );
    line.push_back( ui->d3->text().toInt() );
    line.push_back( ui->d4->text().toInt() );
    line.push_back( ui->d5->text().toInt() );
    line.push_back( ui->d6->text().toInt() );
    line.push_back( ui->d7->text().toInt() );
    line.push_back( ui->d8->text().toInt() );
    line.push_back( ui->d9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->e1->text().toInt() );
    line.push_back( ui->e2->text().toInt() );
    line.push_back( ui->e3->text().toInt() );
    line.push_back( ui->e4->text().toInt() );
    line.push_back( ui->e5->text().toInt() );
    line.push_back( ui->e6->text().toInt() );
    line.push_back( ui->e7->text().toInt() );
    line.push_back( ui->e8->text().toInt() );
    line.push_back( ui->e9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->f1->text().toInt() );
    line.push_back( ui->f2->text().toInt() );
    line.push_back( ui->f3->text().toInt() );
    line.push_back( ui->f4->text().toInt() );
    line.push_back( ui->f5->text().toInt() );
    line.push_back( ui->f6->text().toInt() );
    line.push_back( ui->f7->text().toInt() );
    line.push_back( ui->f8->text().toInt() );
    line.push_back( ui->f9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->g1->text().toInt() );
    line.push_back( ui->g2->text().toInt() );
    line.push_back( ui->g3->text().toInt() );
    line.push_back( ui->g4->text().toInt() );
    line.push_back( ui->g5->text().toInt() );
    line.push_back( ui->g6->text().toInt() );
    line.push_back( ui->g7->text().toInt() );
    line.push_back( ui->g8->text().toInt() );
    line.push_back( ui->g9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->h1->text().toInt() );
    line.push_back( ui->h2->text().toInt() );
    line.push_back( ui->h3->text().toInt() );
    line.push_back( ui->h4->text().toInt() );
    line.push_back( ui->h5->text().toInt() );
    line.push_back( ui->h6->text().toInt() );
    line.push_back( ui->h7->text().toInt() );
    line.push_back( ui->h8->text().toInt() );
    line.push_back( ui->h9->text().toInt() );

    field.push_back(line);
    line.clear();

    line.push_back( ui->i1->text().toInt() );
    line.push_back( ui->i2->text().toInt() );
    line.push_back( ui->i3->text().toInt() );
    line.push_back( ui->i4->text().toInt() );
    line.push_back( ui->i5->text().toInt() );
    line.push_back( ui->i6->text().toInt() );
    line.push_back( ui->i7->text().toInt() );
    line.push_back( ui->i8->text().toInt() );
    line.push_back( ui->i9->text().toInt() );

    field.push_back(line);
    line.clear();

}
