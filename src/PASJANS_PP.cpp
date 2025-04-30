// Amelia Lipińska 203721 
// Alicja Szajgin 203383
// GRA PASJANS KLODLIKE
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include "funkcje.h"
using namespace std;


int main()
{
    //stworzenie obiektu gry
    cout << "(Przykladowy ruch, >> [numerkolumnyzktorej bierzemy] [literainumerkarty] [kolumnanaktora kladziemy]) np. >> 3 D2 5\n";
    cout << "(Aby odswiezyc kolejke, >> 0) np. >> 0\n";
    Gameplay gra;
    //wywołanie menu
    gra.Menu();
    return 0;
}
