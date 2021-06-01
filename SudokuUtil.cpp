#include "mainwindow.h"

void Sudoku::updateClues(){
    int i = 0;
    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            if( field[y][x] != 0 ){
                clues[i]->setText( QString( std::to_string( field[y][x]).c_str() ) );
                pencil[i]->hide();
            }
            clues[i]->setReadOnly(true);
            i++;
        }
    }
}

void Sudoku::updatePencil(){
    for(int i=0; i < 81;i++){
        QString fill;
        for(auto i: fills[i])
            fill += std::to_string(i).c_str();
        pencil[i]->show();
        pencil[i]->setText( QString( fill ) );
        pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
    }
}

void Sudoku::updatePencilxy(){
    uint16_t i=0;
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {

            QString fill;
            for(auto i: fillss[y][x])
                fill += std::to_string(i).c_str();
            pencil[i]->show();
            pencil[i]->setText( QString( fill ) );
            pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
            pencil[i]->setReadOnly(true);
            i++;
        }
    }
}

void Sudoku::untilFind_8(){
    ussv state = fills;
    fills = find_8();

    while( state != fills ){
        fills = state;
        state = find_8();
    }
    find_8();
    rowColElim(Axis::XY, 0 , usv() = {404,404,404});

}

void Sudoku::untilOverFly(){
    ussv state = field;
    field = overFly();

    untilRowColSearch();
    untilFind_8();

    while( state != field ){
        field = state;
        untilRowColSearch();
        untilFind_8();
        state = overFly();


    }
    untilRowColSearch();
    untilFind_8();
}

void Sudoku::untilRowColSearch(){
    ussv state = field;
    field = rowColSearch();

    while( state != field ){
        field = state;
        state = rowColSearch();

    }
}
/*
void Sudoku::untilNakedDouble(){
    usssv state = fillss;
    fillss = nakedDouble();

    while( state != fillss ){
        fillss = state;
        state = nakedDouble();

    }
}

void Sudoku::untilLockedCandidate(){
    usssv state = fillss;
    fillss = lockedCandidate();

    while( state != fillss ){
        fillss = state;
        state = lockedCandidate();

    }
}

void Sudoku::untilInBoxLockedCandidate(){
    usssv state = fillss;
    fillss = inBoxLockedCandidate();

    while( state != fillss ){
        fillss = state;
        state = inBoxLockedCandidate();

    }
}*/

ussv Sudoku::negative(ussv options){
    ussv positive;
    for(uint32_t i=0; i < options.size();i++){
        positive.push_back( {} );
        int f=1;
        for(; f < 10 ; f++)
            if(std::find(options[i].begin(), options[i].end(), f) == options[i].end() && options[i][0] != 0) positive[i].push_back( f );
            //else if(options[i][0] == 0) positive[i].push_back( 0 );
    }

    return positive;
}

void Sudoku::pUssv(ussv vector){
    for(auto &ii: vector){
        for(auto i: ii){
            std::cout<< i;
        }
        std::cout<<std::endl;
    }
}

void Sudoku::pBbv(bbv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

void Sudoku::pUsv(usv vector){
    for(auto i: vector){
        std::cout<<i;
    }
    std::cout<<std::endl;
}

usv Sudoku::getFieldlist(ussv field){
    usv fieldList;

    for(auto &i: field){
        for(auto ii: i){
            fieldList.push_back( ii );
        }
    }
    return fieldList;
}

int16_t Sudoku::find_v(usv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

int16_t Sudoku::find_bv(bv v, uint16_t value){
    auto it = std::find (v.begin(), v.end(), value);
    if( it != v.end() ) return it - v.begin();
    else if( it == v.end() ) return -1;
    else return ~0L;
}

void Sudoku::clueElim(){
    for (int y=0; y < 9;y++) {
        for (int x=0; x < 9;x++) {
            if(fillss[y][x].size() == 1) {
                field[y][x] = fillss[y][x][0];
                fillss[y][x] = {};
            }
        }
    }
}

void Sudoku::erase(usv &v, uint16_t value){
    v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

void Sudoku::eraseP(usv *v, uint16_t value){
    v->erase(std::remove(v->begin(), v->end(), value), v->end());
}

int16_t Sudoku::search_v(usv in, usv array){
    auto it = std::search(in.begin(), in.end(), array.begin(), array.end());
    if( it != in.end() ) return it - in.begin();
    else if( it == in.end() ) return -1;
    else return ~0L;
}