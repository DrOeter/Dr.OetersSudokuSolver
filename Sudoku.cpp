#include "Sudoku.h"

/** @brief Namespace SudokuSolv */
namespace sus {
    using namespace sus;

    /** @brief Eigenes BinaryPredicate um Objekte bei denen der die variable algo gleich 404 ist
     *  @param a 1. SudokuField
     *  @param b 2. SudokuField
     *  @return 1 wenn sie gleich sind und algo gleich 404 sonst 0
     */
    inline bool Pred(sf &a, sf &b){                                         //Es werden nich Werte sondern Eigenschaften von Objekten verglichen
        if (a.getAlgo() == b.getAlgo() && a.getAlgo() == 404)  return 1;
        else return 0;
    }

    /** @brief Entfernt alle sf aus sfv die algo == 404 haben
     *  @param list vector der durchsucht wird
     *  @return void
     */
    void resize(sfv &list){
        auto ip = std::unique(list.begin(), list.end(), Pred);              //Entfernt Duplikate
        list.resize(std::distance(list.begin(), ip));                       //Enfernt überschüssigen Speicher
    }
}

/** @brief Start der Anwendung der Algorithmen in einem Entscheidungsbaum,
 *         bei dem dann der richtige Weg zurückgegangen wird
 *  @return void
 */
void Sudoku::start(){
    sfv list(5), b_list(5);                         //Listen der einzelnen Knotenpunkte vor und nach der Anwendung des Algorithmus
    sffv history;                                   //2D vector der dann die Listen aufnimmt
    ussv unsolved = field;                          //Speichert ursprüngliches Sudoku
    uint16_t done = 0, c = 0, tmp_thread = 0;       //done gleich 1 wenn korrekt gelöst, c ist der iterator für die liste list, tmp_thread
    SudokuSolv sudoku(field);
    sudoku.Solve();                                 //Anwendung eines Linearen Lösungsversuchs

    if(sudoku.hasIntegrity(sudoku.getField())){     // wenn erfolgreich überspringe den Rest
        field = sudoku.getField();
        done = 1;
    }

    c = 0;
    tmp_thread = 0;
    std::vector<std::thread> t(5);

    while(done == 0){
        for(uint16_t i=0; i < 5;i++){                                                               //Anwendung der 5 Algorithmen
            t[i] = std::thread([&](SudokuSolv ssudoku, uint16_t ii, uint64_t id, uint16_t *done){   //Thread wird mit einer Lambda-Funktion mit seperatem Speicher gestartet
                if( *done == 1 || ssudoku.getField().empty()) return;                               //geh raus er schon fertig ist oder field leer ist

                if(!ssudoku.hasIntegrity(ssudoku.getField()) && *done == 0){
                    sf before, after;                                                               //SudokuField für vor und nach der Anwendung des Algorithmus
                    before.setSudokuSolv(ssudoku);                                                  //Speichert Daten
                    before.setAlgo(ii);                                                             //Algorithmus der angewendet wurde
                    before.setID( ssudoku.ID );                                                     //Nimmt ID dieses Knotenpunktes auf

                    ssudoku.useAlgo(ii);                                                            //Anwendung des Algorithmus

                    after.setSudokuSolv(ssudoku);                                                   //Speichert Daten
                    after.setAlgo(ii);                                                              //Algorithmus der angewendet wurde
                    after.setID( id + 1 );                                                          //Nimmt ID eines neuen Knotenpunktes auf

                    if(before != after){                                                            //Wenn der Algoithmus etwas gefunden hat
                        if(*done == 0) {
                            b_list[id] = before;
                            list[id] = after;
                        }
                    }
                }
                if(ssudoku.hasIntegrity(ssudoku.getField()) && *done == 0){
                    *done = 1;                                                                      //Wenn korrekt gelöst wurde erkennen alle anderen Threads mit diesem
                    field = ssudoku.getField();                                                     //Pointer, dass sie nichts mehr machen müssen
                }
            }, sudoku, i, (tmp_thread + i), &done );
        }
        for(uint16_t i=0; i < 5;i++)                                                                //Warte bis die Threads fertig sind
            t[i].join();
        tmp_thread+=5;
        list.insert(list.end(), 5, sf());                                                           //Allocate neuen Speicher für die Threads
        b_list.insert(b_list.end(), 5, sf());

        if(list.size() >= 100000) break;                                                            //nach 100000 / 5 Knotenpunkten gibt er auf
        if(!list.empty()){
            sudoku.setField(list[c].getField());                                                    //ein weiterer Punkt in der liste wird übergeben
            sudoku.setFieldOptions(list[c].getFieldOptions());
            sudoku.ID = list[c].getID();
            c++;
        }
    }
    sus::resize(b_list);                                                                            //Entfernt alle SudokuField wo nichts gefunden wurde also wo algo == 404
    sus::resize(list);

    for(auto i=b_list.begin(); i != b_list.end() - 1; i++)
        if(i->getAlgo() == 404) b_list.erase(i);

    for(auto i=list.begin(); i != list.end() - 1; i++)
        if(i->getAlgo() == 404) list.erase(i);

    b_list.erase(b_list.end() - 1);
    list.erase(list.end() - 1);

    for(int16_t i=list.size()-1; i >= 0 ; i--)                                                      //Listen werden in 2D vector übertragen
        history.push_back({b_list[i], list[i]});

    sffv solution;
    bool start = 0;
    int64_t id = 0;

    for(auto i=history.begin(); i != history.end();i++){                                            //Schleife fängt an mit dem letzten Element aus list und b_list
        if(sudoku.hasIntegrity((*i)[1].getField()) && start == 0) {                                 //Sobald er in der history das gelöste Sudoku findet beginnt die Rückverfolgung
            start = 1;
            id = (*i)[0].getID();                                                                   //Die ID vor der Berechnung wird an id übergeben womit gesucht wird
            solution.push_back(*i);                                                                 //Das erste Element der Lösung wird hinzugefügt
        }
        else if(!sudoku.hasIntegrity((*i)[1].getField()) && start == 0)  continue;

        for(uint16_t pos=1; pos < history.size() - (i - history.begin());pos++){                    //id wird im Rest der History gesucht
            if( id == (*(i + pos))[1].getID() ){                                                    //Wenn die id in history[1] gefunden wird bedeutet das, dass er ein weiteres Teil des Zurückweg gefunden hat
                solution.push_back((*(i + pos)));
                id = (*(i + pos))[0].getID();                                                       //ID von History[0] also vor der Anwendung des Algorithmus wird an id übergeben
                break;
            }
        }
        if(solution.size() > 100) break;
        if(solution.back()[0].getID() == 0)break;                                                   //springt raus wenn er Oben angekommen ist
    }
    std::reverse(solution.begin(), solution.end());                                                 //Spiegelung des vectors

    SudokuSolv finalTest(unsolved);                                                                 //Ab hier wird geprüft ob der Lösungsweg Korrekt ist

    finalTest.untilFind_8();
    finalTest.untilOverFly();

    for(auto i=solution.begin(); i != solution.end();i++)                                           //Anwendung des Lösungsweges
        finalTest.useAlgo((*i)[0].getAlgo());

    if(!finalTest.hasIntegrity(finalTest.getField())){                                              //Auffangmechanismus Wahrscheinlich Mittlerweile überflüssig
        for(uint16_t i=0; i < 5;i++){
            SudokuSolv last(finalTest);
            last.useAlgo(i);
            if(last.hasIntegrity(last.getField())){
               finalTest = last;
               break;
            }
        }
    }

    if(finalTest.hasIntegrity(finalTest.getField())){
        std::cout<<"correct"<<std::endl;
        isCorrect = 1;
    }

    if(!list.empty()) this->fieldOptions = list.back().getFieldOptions();
}

/** @brief Startet den einfachen Lösungsversuch
 *         Die Algorithmen werden solange nacheinander ausgeführt
 *         bis sich field nicht mehr ändert
 *  @return void
 */
void SudokuSolv::Solve(){
    untilFind_8();
    untilOverFly();
}

/** @brief Setzt fieldOptions
 *  @param m_options 3D vector
 *  @return void
 */
void Sudoku::setFieldOptions(usssv o){
    fieldOptions = o;
}

/** @brief Gibt fieldOptions aus
 *  @return fieldOptions 3D vector
 */
usssv Sudoku::getFieldOptions(){
    return fieldOptions;
}

/** @brief Funktion gibt field aus
 *  @return field 2D vector
 */
ussv Sudoku::getField(){
    return field;
}

/** @brief Gibt field aus
 *  @return field 2D vector
 */
ussv SudokuSolv::getField(){
    return field;
}

/** @brief Gibt fieldOptions aus
 *  @return fieldOptions 3D vector
 */
usssv SudokuSolv::getFieldOptions(){
    return fieldOptions;
}

/** @brief Setzt fieldOptions
 *  @param m_options 3D vector
 *  @return void
 */
void SudokuSolv::setFieldOptions(usssv o){
    fieldOptions = o;
}

/** @brief Setzt field
 *  @param m_field 2D vector
 *  @return void
 */
void SudokuSolv::setField(ussv f){
    field = f;
}

/** @brief Benutzt ausgewählten Algorithmus
 *  @param algo sagt welcher Algorithmus benutzt wird
 *  @return void
 */
void SudokuSolv::useAlgo(uint16_t algo){
    if(algo == 0) hiddenSingle();
    if(algo == 1) nakedDouble();
    if(algo == 2) nakedTriplet();
    if(algo == 3) lockedCandidate();
    if(algo == 4) inBoxLockedCandidate();
}

/** @brief Prüft ob das Sudoku korrekt gelöst wurde
 *  @param field
 *  @return true bei korrekter Lösung
 */
bool Sudoku::hasIntegrity(ussv field){
    bool integrity = 1;
    if(!field.empty()) this->field = field;

    for(uint16_t y=0; y < 9;y++){
        usv row = SudokuRowCol(this->field).getRow(y);
        for(uint16_t value=1; value < 10;value++){
            uint16_t count = std::count (row.begin(), row.end(), value);
            if(count > 1 || count < 1) integrity = 0;
        }
    }

    for(uint16_t x=0; x < 9;x++){
        usv col = SudokuRowCol(this->field).getCol(x);
        for(uint16_t value=1; value < 10;value++){
            uint16_t count = std::count (col.begin(), col.end(), value);
            if(count > 1 || count < 1) integrity = 0;
        }
    }

    for(uint16_t y=0; y < 3;y++){
        for(uint16_t x=0; x < 3;x++){
            usv box = SudokuBox(this->field).getBox(x, y);
            for(uint16_t value=1; value < 10;value++){
                uint16_t count = std::count (box.begin(), box.end(), value);
                if(count > 1 || count < 1) integrity = 0;
            }
        }
    }

    return integrity;
}

/** @brief Sucht in jeder Box nach einem
 *         Kästchen wo eine Options einmalig ist
 *  @return field 2D vector
 */
void SudokuSolv::hiddenSingle(){
    if(hasIntegrity(field)) return;

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = sus::xbox[xBoxPos][0];
            uint16_t y = sus::ybox[yBoxPos][0];
            SudokuBoxOptions boxObj(fieldOptions);
            usssv options = boxObj.get3dBox(x, y);

            for(uint16_t value=1, count=0; value < 10;value++){
                usv coords;
                count = 0;
                for(uint16_t yy=0; yy < 3;yy++){
                    for(uint16_t xx=0; xx < 3;xx++){
                        if(find_v(options[yy][xx], value) != -1){
                            coords.push_back(boxObj.getPos(0, xx));
                            coords.push_back(boxObj.getPos(1, yy));
                            coords.push_back( xx );
                            coords.push_back( yy );
                            count++;
                        }
                    }
                }

                if(count == 1){
                    field[coords[1]][coords[0]] = value;
                    fieldOptions[coords[1]][coords[0]].clear();

                    for(uint16_t x=0; x < 9;x++){
                        if(SudokuBox::findBox(xBoxPos, yBoxPos) != SudokuBox::findBox(x, coords[3])){
                            erase(fieldOptions[coords[1]][x], value);
                        }
                    }
                    for(uint16_t y=0; y < 9;y++){
                        if(SudokuBox::findBox(xBoxPos, yBoxPos) != SudokuBox::findBox(coords[2], y)){
                            erase(fieldOptions[y][coords[0]], value);
                        }
                    }
                    clueElim();
                    untilRefresh();
                }
            }
        }
    }
}

/** @brief Sucht in jeder Reihe und Spalte nach einem
 *         Kästchen wo eine Options einmalig ist
 *  @return field 2D vector
 */
ussv SudokuSolv::rowColElim(){
    if(hasIntegrity(field)) return ussv();
    usv coords = {404,404,404};

        for(uint16_t y=0; y < 9;y++){
            bv line = {0,0,0,0,0,0,0,0,0};
            for(uint16_t value=1; value < 10;value++){

                uint16_t x_loc = 404;
                line = {0,0,0,0,0,0,0,0,0};
                for(uint16_t x=0; x < 9;x++){
                    if(field[y][x] == 0){
                        if(find_v(fieldOptions[y][x], value) == -1) line[x] = 1;
                        if(find_v(fieldOptions[y][x], value) != -1 && coords[0] != x && coords[1] != x && coords[2] != x) x_loc = x;
                    }
                    else if(field[y][x] != value) line[x] = 1;
                }
                uint16_t complete = 0;
                for(auto i: line)
                    if(i == 0) complete++;

                usv box;
                if(x_loc != 404) box = SudokuBox(field).getBox(x_loc, y);

                if(complete == 1 && line[x_loc] == 0 && x_loc != 404 && find_v(box, value) == -1 ) field[y][x_loc] = value;
            }
        }

        for(int x=0; x < 9;x++){
            bv line = {0,0,0,0,0,0,0,0,0};
            for(uint16_t value=1; value < 10;value++){

                uint16_t y_loc = 404;
                line = {0,0,0,0,0,0,0,0,0};
                for(uint16_t y=0; y < 9;y++){
                    if(field[y][x] == 0){
                        if(find_v(fieldOptions[y][x], value) == -1) line[y] = 1;
                        if(find_v(fieldOptions[y][x], value) != -1 && coords[0] != y && coords[1] != y && coords[2] != y) y_loc = y;
                    }
                    else if(field[y][x] != value) line[y] = 1;
                }
                uint16_t complete = 0;
                for(auto i: line)
                    if(i == 0) complete++;

                usv box;
                if(y_loc != 404) box = SudokuBox(field).getBox(x, y_loc);

                if(complete == 1 && line[y_loc] == 0 && y_loc != 404 && find_v(box, value) == -1 ) field[y_loc][x] = value;
            }
        }
    untilRefresh();

    return field;
}

/** @brief Sucht nach 8 unterschiedlichen
 *         werten in einer Reihe, Spalte und Box, um einen
 *         eindutigen Wert für ein Kästchen zu finden.
 *         Zudem Updatet sie die fieldOptions variable
 *  @return fieldOptionsList 2D vector
 */
ussv SudokuSolv::find_8(){
    if(hasIntegrity(field)) return ussv();
    fieldOptionList.clear();


    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            usv fill;

            for(int yy=0; yy < 9 ; yy++)
                if ( find_v(fill, field[yy][x]) == -1 && field[yy][x] != 0 ) fill.push_back( field[yy][x] );

            for(int xx=0; xx < 9 ; xx++)
                if ( find_v(fill, field[y][xx]) == -1 && field[y][xx] != 0 ) fill.push_back( field[y][xx] );

            ussv box = SudokuBox(field).get2dBox(x, y);

            for(int y=0; y < 3;y++){
                for(int x=0; x < 3;x++){
                    if ( find_v(fill, box[y][x]) == -1 && box[y][x] != 0 )
                        fill.push_back( box[y][x] );
                }
            }

            if(field[y][x] == 0 && fill.size() == 8){
                int f=1;
                for(; f < 10 ; f++)
                    if(std::find(fill.begin(), fill.end(), f) == fill.end()) break;

                if(field[y][x] == 0) field[y][x] = f;
            }
            if(field[y][x] == 0) fieldOptionList.push_back( fill );
            else if(field[y][x] != 0) fieldOptionList.push_back( usv() = {0} );
        }
    }
    fieldOptionList = negative( fieldOptionList );
    ussv new_fills;

    if(!fieldOptions.empty()) {
        uint16_t i = 0;
        for(int y=0; y < 9;y++){
            for(int x=0; x < 9;x++){
                usv values;
                for(int value=1; value < 10;value++){
                    if(find_v(fieldOptions[y][x], value) != -1 && find_v(fieldOptionList[i], value) != -1 ){
                        values.push_back( value );
                    }
                }
                new_fills.push_back( values );
                i++;
            }
        }
    }
    else if(fieldOptions.empty()) new_fills = fieldOptionList;

    fieldOptions.clear();

    uint16_t ii = 0;
    for (uint16_t c=0; c < 9;c++ ) {
        ussv line(9);
        for (auto &i: line ) {
            i = new_fills[ii];
            ii++;
        }
        fieldOptions.push_back(line);
    }
    //rowColElim(Axis::XY, 0 , usv() = {404,404,404});

    clueElim();

    return fieldOptionList;
}


usv SudokuSolv::collectRow(ussv field, int rc, Axis axis){
    usv collect;
    if(axis == Axis::X){
        for(int i=0; i < 9;i++)
            if( field[i][rc] != 0 ) collect.push_back( field[i][rc] );
    }
    else if(axis == Axis::Y){
        for(int i=0; i < 9;i++)
            if( field[rc][i] != 0 ) collect.push_back( field[rc][i] );
    }

    return collect;
}

void SudokuSolv::boxElim(bbv &box, sv rows, uint16_t i,uint16_t x, uint16_t y){
    if(i == 0 || i == 1){
        for(int el=0; el < 3;el++)
            box[ el ][ x + rows[i] ] = 1;
    }
    else if(i == 2 || i == 3){
        for(int el=0; el < 3;el++)
            box[ y + rows[i] ][ el ] = 1;
    }

}

void SudokuSolv::rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xb, uint16_t yb){

    ussv clues;
    clues.push_back( collectRow( field, x + pos_row[0], Axis::X ) );
    clues.push_back( collectRow( field, x + pos_row[1], Axis::X ) );
    clues.push_back( collectRow( field, y + pos_row[2], Axis::Y ) );
    clues.push_back( collectRow( field, y + pos_row[3], Axis::Y ) );

    usv gridxy = SudokuBox::findBox(x, y);

    for (int value=1; value < 10;value++ ){
        bbv box = {{0,0,0},
                   {0,0,0},
                   {0,0,0}};

        ussv selectBox = SudokuBox(field).get2dBox(x, y);

        bool value_isin = 0;

        for (int yy=0; yy < 3;yy++) {
            for (int xx=0; xx < 3;xx++) {
                if( selectBox[yy][xx] != 0) box[yy][xx] = 1;
                if( selectBox[yy][xx] == value) value_isin = 1;
            }
        }
        //std::cout<<"peniskakaka"<<value_isin<<y<<"  "<<x<<std::endl;

        if( value_isin ) continue;
        for(int i=0; i < 4;i++ ){
            if ( find_v(clues[i], value) != -1){
                boxElim(box, pos_row, i, xb, yb);
            }
        }
        uint16_t complete = 0;
        for(auto &ii: box){
            for(auto i: ii){
                if(i == 0) complete++;
            }
        }
        if( box[yb][xb] == 0 && complete == 1) {
            field[y][x] = value;
        }
        if(complete >= 2 && complete <= 9){
            usssv options = SudokuBoxOptions(fieldOptions).get3dBox(x, y);

            uint16_t pos[2], one = 0;
            for (int yy=0; yy < 3;yy++) {
                for (int xx=0; xx < 3;xx++) {
                    if( selectBox[yy][xx] == 0 && box[yy][xx] == 0) {
                        if ( find_v(options[yy][xx], value) != -1){
                            pos[0] = (yy - yb) + y;
                            pos[1] = (xx - xb) + x;
                            one++;
                        }
                    }
                }
            }
            if(one == 1 && field[ pos[0] ][ pos[1] ] == 0 ) field[ pos[0] ][ pos[1] ] = value;
        }
    }
}

/** @brief Ähnlich wie hiddenSingle nur das in
 *         Reihen oder Spalten innerhalb der Box
 *         nach einer einmailgen Options gesucht
 *         wird, die sonst nicht in der Box vorkommt
 *  @return field 2D vector
 */
ussv SudokuSolv::overFly(){
    if(hasIntegrity(field)) return ussv();

    ussv newfield = field;

    for(uint16_t y=0; y < 9;y++){
        for(uint16_t x=0; x < 9;x++){
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y + 2
                rowColSolve(newfield, sus::position[0], x, y, 0, 0);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y + 2
                rowColSolve(newfield, sus::position[1], x, y, 1, 0);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x - 1 & x - 2 & y + 1 & y + 2
                rowColSolve(newfield, sus::position[2], x, y, 2, 0);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y - 1
                rowColSolve(newfield, sus::position[3], x, y, 0, 1);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y - 1
                rowColSolve(newfield, sus::position[4], x, y, 1, 1);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 2 & y + 1 & y - 1
                rowColSolve(newfield, sus::position[5], x, y, 2, 1);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x + 2 & y - 1 & y - 2
                rowColSolve(newfield, sus::position[6], x, y, 0, 2);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x - 1 & y - 1 & y - 2
                rowColSolve(newfield, sus::position[7], x, y, 1, 2);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x - 1 & x - 2 & y - 1 & y - 2
                rowColSolve(newfield, sus::position[8], x, y, 2, 2);
            }
        }
    }
    field = newfield;
    return newfield;
}

/** @brief Wenn in einer Reihe oder Spalte einer Box eine Option vorkommt,
 *         die nicht im Rest der Reihe oder Spalte vorkommt muss überall
 *         in der Box außer der ausgewälten Reihe oder Spalte diese Option entfernt werden
 *  @return fieldOptions 3D vector
 */
usssv SudokuSolv::lockedCandidate(){
    if(hasIntegrity(field)) return usssv();

    for(uint16_t y=0; y < 9; y++){
        for(uint16_t value=1; value < 10; value++){
            uint16_t full_row = 0;
            bv box = {0,0,0};
             usv old_box = {404,404};
            for(uint16_t x=0; x < 9; x++){
                usv dec_box = SudokuBox::findBox(x, y);
                if(!fieldOptions[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fieldOptions[y][x], value) != -1 && find_v(sus::xbox[i], x) != -1 ) box[i] = 1;
                }
                else if(field[y][sus::xbox[dec_box[0]][0]] != 0 && field[y][sus::xbox[dec_box[0]][1]] != 0 && field[y][sus::xbox[dec_box[0]][2]] != 0 && old_box != dec_box)
                    full_row++;

                old_box = dec_box;
            }
            if( full_row == 2 )break;

            uint16_t complete = 0;
            for(auto i: box)
                if(i == 1)complete++;

            usv gridxy;
            usssv options;
            SudokuBoxOptions boxObj(fieldOptions);

            for(uint16_t i=0; i < 3; i++)
                if(box[i] == 1 && complete == 1)  {
                    options = boxObj.get3dBox(sus::xbox[i][0], y);
                    gridxy = SudokuBox::findBox(sus::xbox[i][0], y);
                }

            if(!options.empty()){
                for (int yy=0; yy < 3;yy++) {
                    for (int xx=0; xx < 3;xx++) {

                        int16_t pos = find_v(options[yy][xx], value);
                        if( boxObj.getPos(1, yy) != y && pos != -1 && !options[yy][xx].empty()) {
                            boxObj.erase( xx, yy, value);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        for(uint16_t value=1; value < 10; value++){
            uint16_t full_col = 0;
            bv box = {0,0,0};
            usv old_box = {404,404};
            for(uint16_t y=0; y < 9; y++){
                usv dec_box = SudokuBox::findBox(x, y);
                if(!fieldOptions[y][x].empty()){
                    for(uint16_t i=0; i < 3; i++)
                        if( find_v(fieldOptions[y][x], value) != -1 && find_v(sus::xbox[i], y) != -1 ) box[i] = 1;
                }

                else if(field[sus::ybox[dec_box[1]][0]][x] != 0 && field[sus::ybox[dec_box[1]][1]][x] != 0 && field[sus::ybox[dec_box[1]][2]][x] != 0 && old_box != dec_box){
                    full_col++;
                }
                old_box = dec_box;
            }
            if( full_col == 2) break;
            uint16_t complete = 0;
            for(auto i: box)
                if(i == 1)complete++;

            usv gridxy;
            usssv options;
            SudokuBoxOptions boxObj(fieldOptions);

            for(uint16_t i=0; i < 3; i++)
                if(box[i] == 1 && complete == 1) {
                    options = boxObj.get3dBox(x, sus::ybox[i][0]);
                    gridxy = SudokuBox::findBox(x, sus::ybox[i][0]);
                }

            if(!options.empty()){
                for (int xx=0; xx < 3;xx++) {
                    for (int yy=0; yy < 3;yy++) {

                        int16_t pos = find_v(options[yy][xx], value);
                        if( boxObj.getPos(0, xx) != x && pos != -1 && !options[yy][xx].empty()) {
                            boxObj.erase( xx, yy, value);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }
    untilFind_8();
    return fieldOptions;
}

/** @brief Wenn in einer Reihe oder Spalte einer Box eine Option vorkommt,
 *         die nicht im Rest der Box vorkommt muss
 *         in dem Teil der Reihe oder Spalte, der nicht in der
 *         ausgewählten Box ist diese Option entfernt werden
 *  @return fieldOptions 3D vector
 */
usssv SudokuSolv::inBoxLockedCandidate(){
    if(hasIntegrity(field)) return usssv();

    usv gridxy = {0,0};
    for(gridxy[1] = 0;gridxy[1] < 3;gridxy[1]++){
        for(gridxy[0] = 0;gridxy[0] < 3;gridxy[0]++){
            for(uint16_t value=1; value < 10; value++){

                bv row = {0,0,0};
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if(!fieldOptions ARRAY_POS.empty()){
                            if(find_v(fieldOptions ARRAY_POS, value) != -1){
                                row[yy] = 1;
                            }
                        }
                    }
                }
                uint16_t complete = 0;
                for(auto i: row)
                    if(i == 1)complete++;

                for(uint16_t y=0; y < 3; y++){
                    if(row[y] == 1 && complete == 1) {
                        for(uint16_t x=0; x < 9; x++){
                            if( find_v(sus::xbox[gridxy[0]], x) == -1 && find_v(fieldOptions[sus::ybox[gridxy[1]][y]][x], value) != -1 ){
                                erase(fieldOptions[sus::ybox[gridxy[1]][y]][x], value);
                                clueElim();
                                untilRefresh();
                            }
                        }
                    }
                }
            }
        }
    }
    gridxy.clear();
    gridxy = {0,0};

    for(gridxy[1] = 0;gridxy[1] < 3;gridxy[1]++){
        for(gridxy[0] = 0;gridxy[0] < 3;gridxy[0]++){
            for(uint16_t value=1; value < 10; value++){

                bv col = {0,0,0};
                for(uint16_t xx=0; xx < 3; xx++){
                    for(uint16_t yy=0; yy < 3; yy++){
                        if(!fieldOptions ARRAY_POS.empty()){
                            if(find_v(fieldOptions ARRAY_POS, value) != -1){
                                col[xx] = 1;
                            }
                        }
                    }
                }
                uint16_t complete = 0;
                for(auto i: col)
                    if(i == 1)complete++;

                for(uint16_t x=0; x < 3; x++){
                    if(col[x] == 1 && complete == 1) {
                        for(uint16_t y=0; y < 9; y++){
                            if( find_v(sus::ybox[gridxy[1]], y) == -1 && find_v(fieldOptions[y][sus::xbox[gridxy[0]][x]], value) != -1 ){
                                erase(fieldOptions[y][sus::xbox[gridxy[0]][x]], value);
                                clueElim();
                                untilRefresh();
                            }
                        }
                    }
                }
            }
        }
    }
    untilFind_8();

    return fieldOptions;
}

/** @brief Wenn in einer Reihe, Spalte oder Box zwei Kästchen sind,
 *         die zwei gleiche Optionen haben, werden diese aus allen anderen Kästchen entfernt
 *  @return fieldOptions 3D vector
 */
usssv SudokuSolv::nakedDouble(){
    if(hasIntegrity(field)) return usssv();

    for(uint16_t y=0; y < 9; y++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {0, y}, find, 1);

        if(find == 1 && coords.size() == 2){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
            }

            if(SudokuBox::findBox(coords[0], y) == SudokuBox::findBox(coords[1], y)){
                SudokuBoxOptions boxObj(fieldOptions);
                usssv options = boxObj.get3dBox(coords[0], y);
                usv gridxy = SudokuBox::findBox(coords[0], y);

                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && boxObj.getPos(1, yy) != y){
                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);

                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        ussv pair;
        usv coords;
        usv recoverd = {404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {1, x}, find, 1);

        if(find == 1 && coords.size() == 2){
            for(uint16_t y=0; y < 9; y++){
                if(coords[0] != y && coords[1] != y && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != y && coords[1] != y && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
            }

            if(SudokuBox::findBox(x, coords[0]) == SudokuBox::findBox(x, coords[1])){
                SudokuBoxOptions boxObj(fieldOptions);
                usssv options = boxObj.get3dBox(x, coords[0]);
                usv gridxy = SudokuBox::findBox(x, coords[0]);

                for(uint16_t xx=0; xx < 3; xx++){
                    for(uint16_t yy=0; yy < 3; yy++){
                        if( !options[yy][xx].empty() && boxObj.getPos(0, xx) != x){
                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);

                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = sus::xbox[xBoxPos][0];
            uint16_t y = sus::ybox[yBoxPos][0];
            SudokuBoxOptions boxObj(fieldOptions);
            usssv options = boxObj.get3dBox(x, y);
            bool find = 0;
            usv recoverd = {404,404,404};
            usv coords;

            advancedHelper(coords, recoverd, {2, x, y}, find, 1);

            if(find == 1 && coords.size() == 2){
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && options[yy][xx] != recoverd
                            && sus::listToPos[coords[0]] != usv{xx, yy}
                            && sus::listToPos[coords[1]] != usv{xx, yy} ){

                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }

    untilFind_8();

    return fieldOptions;
}

/** @brief Wenn in einer Reihe, Spalte oder Box zwei oder drei Kästchen sind,
 *         die entweder gleiche Optionen oder eine bestimmte kombination an gleichen
 *         und ungleichen Optionen haben, werden diese aus allen anderen Kästchen entfernt
 *  @return fieldOptions 3D vector
 */
usssv SudokuSolv::nakedTriplet(){
    if(hasIntegrity(field)) return usssv();

    for(uint16_t y=0; y < 9; y++){
        usv coords;
        usv recoverd = {404,404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {0, y}, find, 0);

        if(find == 1 && coords.size() == 3){
            for(uint16_t x=0; x < 9; x++){
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
                if(coords[0] != x && coords[1] != x && coords[2] != x && find_v(fieldOptions[y][x], recoverd[2]) != -1 ) erase(fieldOptions[y][x], recoverd[2]);
            }

            if( SudokuBox::findBox(coords[0], y) == SudokuBox::findBox(coords[1], y) ){
                if( SudokuBox::findBox(coords[1], y) == SudokuBox::findBox(coords[2], y)){
                    SudokuBoxOptions boxObj(fieldOptions);
                    usssv options = boxObj.get3dBox(coords[0], y);
                    usv gridxy = SudokuBox::findBox(coords[0], y);

                    for(uint16_t yy=0; yy < 3; yy++){
                        for(uint16_t xx=0; xx < 3; xx++){
                            if( !options[yy][xx].empty() && boxObj.getPos(1, yy) != y){
                                if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                                if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                                if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                                clueElim();
                                untilRefresh();
                            }
                        }
                    }
                }
            }
        }
    }

    for(uint16_t x=0; x < 9; x++){
        usv coords;
        usv recoverd = {404,404,404};
        bool find = 0;

        advancedHelper(coords, recoverd, {1, x}, find, 0);

        if(find == 1 && coords.size() == 3){
            for(uint16_t y=0; y < 9; y++){
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fieldOptions[y][x], recoverd[0]) != -1 ) erase(fieldOptions[y][x], recoverd[0]);
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fieldOptions[y][x], recoverd[1]) != -1 ) erase(fieldOptions[y][x], recoverd[1]);
                if(coords[0] != y && coords[1] != y && coords[2] != y && find_v(fieldOptions[y][x], recoverd[2]) != -1 ) erase(fieldOptions[y][x], recoverd[2]);
            }

            if(SudokuBox::findBox(x, coords[0]) == SudokuBox::findBox(x, coords[1])){
                if(SudokuBox::findBox(x, coords[1]) == SudokuBox::findBox(x, coords[2])){
                    SudokuBoxOptions boxObj(fieldOptions);
                    usssv options = boxObj.get3dBox(x, coords[0]);
                    usv gridxy = SudokuBox::findBox(x, coords[0]);

                    for(uint16_t xx=0; xx < 3; xx++){
                        for(uint16_t yy=0; yy < 3; yy++){
                            if( !options[yy][xx].empty() && boxObj.getPos(0, xx) != x){
                                if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                                if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                                if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                                clueElim();
                                untilRefresh();
                            }
                        }
                    }
                }
            }
        }
    }

    for(uint16_t yBoxPos=0; yBoxPos < 3; yBoxPos++){
        for(uint16_t xBoxPos=0; xBoxPos < 3; xBoxPos++){
            uint16_t x = sus::xbox[xBoxPos][0];
            uint16_t y = sus::ybox[yBoxPos][0];
            SudokuBoxOptions boxObj(fieldOptions);
            usssv options = boxObj.get3dBox(x, y);
            bool find = 0;
            usv recoverd = {404,404,404};
            usv coords;

            advancedHelper(coords, recoverd, {2, x, y}, find, 0);

            if(find == 1 && coords.size() == 3){
                for(uint16_t yy=0; yy < 3; yy++){
                    for(uint16_t xx=0; xx < 3; xx++){
                        if( !options[yy][xx].empty() && options[yy][xx] != recoverd
                            && sus::listToPos[coords[0]] != usv{xx, yy}
                            && sus::listToPos[coords[1]] != usv{xx, yy}
                            && sus::listToPos[coords[2]] != usv{xx, yy}  ){

                            if( find_v(options[yy][xx], recoverd[0]) != -1 ) boxObj.erase( xx, yy, recoverd[0]);
                            if( find_v(options[yy][xx], recoverd[1]) != -1 ) boxObj.erase( xx, yy, recoverd[1]);
                            if( find_v(options[yy][xx], recoverd[2]) != -1 ) boxObj.erase( xx, yy, recoverd[2]);
                            clueElim();
                            untilRefresh();
                        }
                    }
                }
            }
        }
    }
    untilFind_8();

    return fieldOptions;
}

/** @brief Helferfunktion für nakedDouble und nakedTriplet
 *         In verschachtelten for-Schleifen geht man über die Reihe,
 *         Spalte oder Box und sucht nach zwei oder drei Kästchen die
 *         in der Richtigen Beziehung zueinander stehen
 *  @return void
 */
void SudokuSolv::advancedHelper(usv &coords, usv &recoverd, usv position, bool &find, bool isDouble ){
    coords.clear();
    recoverd = {404,404,404};
    ussv rowColBox;
    if(position[0] == 0) rowColBox = SudokuRowColOptions(fieldOptions).getRow(position[1]);
    if(position[0] == 1) rowColBox = SudokuRowColOptions(fieldOptions).getCol(position[1]);
    if(position[0] == 2) rowColBox = SudokuBoxOptions(fieldOptions).get2dBox(position[1], position[2]);

    for(auto first = rowColBox.begin(); first != rowColBox.end(); first++) {

        if((*first).size() == 2){
            for(auto second = first + 1; second != rowColBox.end(); second++){

                if((*second).size() == 2 && (*first) == (*second) && isDouble ){
                    find = 1;
                    coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                    coords.push_back( (uint16_t)(second - rowColBox.begin()) );

                    recoverd = (*first);
                    goto A;
                }
                if(isDouble) continue;

                if((*second).size() == 2
                    && ( ( ( (*first)[0] == (*second)[0]
                    && (*first)[1] != (*second)[1] )
                    || ( (*first)[0] != (*second)[0]
                    && (*first)[1] == (*second)[1] ) )

                    || ( ( (*first)[0] == (*second)[1]
                    && (*first)[1] != (*second)[0] )
                    || ( (*first)[0] != (*second)[1]
                    && (*first)[1] == (*second)[0] ) ) ) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                            && ( ( (*first)[0] == (*second)[1]
                            && (*first)[1] != (*second)[0]
                            && (*first)[1] == (*third)[1]
                            && (*first)[0] != (*third)[0]
                            && (*second)[0] == (*third)[0]
                            && (*second)[1] != (*third)[1] )

                            || ( (*first)[1] == (*second)[0]
                            && (*first)[0] != (*second)[1]
                            && (*first)[0] == (*third)[0]
                            && (*first)[1] != (*third)[1]
                            && (*second)[1] == (*third)[1]
                            && (*second)[0] != (*third)[0] )

                            || ( (*first)[1] == (*second)[1]
                            && (*first)[0] != (*second)[0]
                            && (*first)[0] == (*third)[1]
                            && (*first)[1] != (*third)[0]
                            && (*second)[0] == (*third)[0]
                            && (*second)[1] != (*third)[1] )

                            || ( (*first)[0] == (*second)[0]
                            && (*first)[1] != (*second)[1]
                            && (*first)[1] == (*third)[0]
                            && (*first)[0] != (*third)[1]
                            && (*second)[1] == (*third)[1]
                            && (*second)[0] != (*third)[0] )

                            || ( (*first)[1] == (*second)[1]
                            && (*first)[0] != (*second)[0]
                            && (*first)[0] == (*third)[0]
                            && (*first)[1] != (*third)[1]
                            && (*second)[0] == (*third)[1]
                            && (*second)[1] != (*third)[0] )

                            || ( (*first)[0] == (*second)[0]
                            && (*first)[1] != (*second)[1]
                            && (*first)[1] == (*third)[1]
                            && (*first)[0] != (*third)[0]
                            && (*second)[1] == (*third)[0]
                            && (*second)[0] != (*third)[1] ) ) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd.clear();
                                recoverd.push_back((*first)[0]);
                                recoverd.push_back((*first)[1]);
                                recoverd.push_back((*second)[0]);
                                recoverd.push_back((*second)[1]);
                                recoverd.push_back((*third)[0]);
                                recoverd.push_back((*third)[1]);

                                std::sort(recoverd.begin(), recoverd.end());
                                auto ip = std::unique(recoverd.begin(), recoverd.end());
                                recoverd.resize(std::distance(recoverd.begin(), ip));

                                //pUsv(recoverd);

                                goto A;
                        }
                    }
                }

                if((*second).size() == 2
                    && ( ( ( (*first)[0] == (*second)[0]
                    && (*first)[1] != (*second)[1] )
                    || ( (*first)[0] != (*second)[0]
                    && (*first)[1] == (*second)[1] ) )

                    || ( ( (*first)[0] == (*second)[1]
                    && (*first)[1] != (*second)[0] )
                    || ( (*first)[0] != (*second)[1]
                    && (*first)[1] == (*second)[0] ) ) ) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3
                            && find_v(*third, (*second)[0]) != -1
                            && find_v(*third, (*second)[1]) != -1
                            && find_v(*third, (*first)[0]) != -1
                            && find_v(*third, (*first)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 3
                    && find_v(*second, (*first)[0]) != -1
                    && find_v(*second, (*first)[1]) != -1 ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                           && find_v(*second, (*third)[0]) != -1
                           && find_v(*second, (*third)[1]) != -1

                           && ( ( ( (*first)[0] == (*third)[0]
                           && (*first)[1] != (*third)[1] )
                           || ( (*first)[0] != (*third)[0]
                           && (*first)[1] == (*third)[1] ) )

                           || ( ( (*first)[0] == (*third)[1]
                           && (*first)[1] != (*third)[0] )
                           || ( (*first)[0] != (*third)[1]
                           && (*first)[1] == (*third)[0] ) ) ) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*second);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 3
                    && find_v(*second, (*first)[0]) != -1
                    && find_v(*second, (*first)[1]) != -1  ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3
                            && (*second) == (*third)
                            && find_v(*third, (*first)[0]) != -1
                            && find_v(*third, (*first)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }
            }
        }
        else if(isDouble) continue;
        else if((*first).size() == 3){
            for(auto second = first + 1; second != rowColBox.end(); second++){

                if((*second).size() == 3 && (*first) == (*second) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3 && (*third) == (*first) && (*second) == (*third) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 2
                    && find_v(*first, (*second)[0]) != -1
                    && find_v(*first, (*second)[1]) != -1 ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 3
                            && (*third) == (*first)
                            && find_v(*third, (*second)[0]) != -1
                            && find_v(*third, (*second)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*third);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 3
                    && (*second) == (*first) ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                           && find_v(*first, (*third)[0]) != -1
                           && find_v(*first, (*third)[1]) != -1 ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*second);
                                goto A;
                        }
                    }
                }

                if((*second).size() == 2
                    && find_v(*first, (*second)[0]) != -1
                    && find_v(*first, (*second)[1]) != -1  ){

                    for(auto third = second + 1; third != rowColBox.end(); third++){

                        if((*third).size() == 2
                            && find_v(*first, (*third)[0]) != -1
                            && find_v(*first, (*third)[1]) != -1

                            && ( ( ( (*second)[0] == (*third)[0]
                            && (*second)[1] != (*third)[1] )
                            || ( (*second)[0] != (*third)[0]
                            && (*second)[1] == (*third)[1] ) )

                            || ( ( (*second)[0] == (*third)[1]
                            && (*second)[1] != (*third)[0] )
                            || ( (*second)[0] != (*third)[1]
                            && (*second)[1] == (*third)[0] ) ) ) ){
                                find = 1;
                                coords.push_back( (uint16_t)(first - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(second - rowColBox.begin()) );
                                coords.push_back( (uint16_t)(third - rowColBox.begin()) );

                                recoverd = (*first);
                                goto A;
                        }
                    }
                }
            }
        }
    }
    A:
    return;
}
