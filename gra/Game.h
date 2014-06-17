#pragma once


class Game
{
    bool _firstPlayer;
    int _fields[64]; // 0 - no move, 1 - player 1, 2 - player 2
public:
    int * rplansza(){return _fields;}
    Game();
    int getResult(); // check if someone won; 0 - game is running, 1,2 - one's player victory, 3 - draw
    bool makeMove(int fieldId); // true - movement was available; false - movement was forbidden
    void resetTable(); // reset state of table
    void drawTable(); // drawing table with values of fields
    int getPlayer();
    void Wyczysc(int ID){_fields[ID]=0;
    _firstPlayer = !_firstPlayer;}

    int minimax(int poziom, float t);
    bool ruch(int pole, int gracz);
};
