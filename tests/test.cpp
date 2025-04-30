#include <iostream>
#include <cassert>
#include "funkcje.h"
using namespace std;

int main() {
    Gameplay gra;

    // Sprawdzenie, że początkowo gra nie jest wygrana
    assert(!gra.Wygrana() && "Na poczatku gra nie powinna byc wygrana");

    // Zasymuluj zwycięstwo
    gra.testSymulujWygrana();

    // Sprawdź, czy gra została wygrana
    assert(gra.Wygrana() && "Gra powinna byc wygrana po zapelnieniu stosow");

    cout << "Test funkcji Wygrana() zakonczony sukcesem!\n";
    return 0;
}
