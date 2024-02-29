#pragma once

#define mytype signed char
#define boardtype signed char
typedef boardtype TField[8][8];

extern "C" mytype GetMode(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode);
extern "C" bool CheckCoord(mytype x1, mytype y1);
extern "C" void BInit(TField&);
extern "C" void BCopy(TField&, TField&);
extern "C" void Move(TField&, mytype, mytype, mytype, mytype);
extern "C" void Beat(TField&, mytype, mytype, mytype, mytype);
extern "C" bool SMCheck(TField&, mytype, mytype, mytype, mytype);
extern "C" bool SBCheck(TField&, mytype, mytype, mytype, mytype, bool);
extern "C" int getAssess(TField&);

bool NTBDamka(TField& field, mytype x, mytype y, bool turn, mytype mode);
bool NTBDamkaDiag(TField& field, mytype* x, mytype* y, bool turn, mytype mode);
bool NTBDamkaOneMore(TField& field, mytype x, mytype y, bool turn, mytype mode);
void DamkaBeat(TField& field, mytype x1, mytype y1, mytype x2, mytype y2, mytype mode);
mytype amountOfDamka(TField& field);

