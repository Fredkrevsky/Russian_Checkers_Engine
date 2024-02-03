#pragma once

#define mytype signed char
#define boardtype signed char
typedef boardtype TField[8][8];

mytype GetMode(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode);
bool CheckCoord(mytype x, mytype y);

extern const float AssesDamka[8][8];
extern const float AssesSimple[8][8];

class Board {
private:
    void PossibleDamka();  
    bool NTBDamka(mytype x, mytype y, bool turn, mytype mode);
    bool NTBDamkaDiag(mytype* x, mytype* y, bool turn, mytype mode);
public:
    TField Field;
    Board();
    Board(const TField Field); 
    Board(const Board& other);  
    bool NTBDamkaOneMore(mytype x, mytype y, bool turn, mytype mode);
    void Move(mytype x1, mytype y1, mytype x2, mytype y2);
    void Beat(mytype x1, mytype y1, mytype x2, mytype y2);
    bool SimpleMoveCheck(mytype x1, mytype y1, mytype x2, mytype y2);
    bool SimpleBeatCheck(mytype x1, mytype y1, mytype x2, mytype y2, bool turn);
    void DamkaBeat(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode);
    float getAsses();
    mytype amountOfDamka();
};

