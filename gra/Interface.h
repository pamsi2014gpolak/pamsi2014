#pragma once
#include "Game.h"

class Interface
{
    Game _game;
    int _firstCPU; // 0 means Human Player
    int _secondCPU; // 0 means Human Player
    public:
    Interface();
    void startNewGame(int firstCPU, int secondCPU);
    void printGameTable();
    void CPUmove(int lvl);
    int minimax(Game gra,int gracz, int poziom, float d);
};
