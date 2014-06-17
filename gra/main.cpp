#include "Game.h"
#include "Interface.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
srand(time(NULL));
Interface i;
i.startNewGame(2, 0); // rozpoczyna grê z 1 graczem komputerowym (gracz typu random) oraz z 1 ludzkim graczem
return 0;

}
