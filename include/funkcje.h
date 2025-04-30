#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;


// Sta³e
const int MAX_C = 52;
const int KIER = 0;
const int KARO = 1;
const int TREFL = 2;
const int PIK = 3;
const int CZARNY = 0;
const int CZERWONY = 1;

// Karta
struct Card {
    int value = 0;
    int shape = 0;
    int color = 0;
    bool is_face_up = false;
    string card_description;

    Card();
    Card(int val, int sha, int col);
};

// Gra
class Gameplay {
    Card deck[MAX_C];
    queue<Card> choice;
    vector<Card> odkryte;

    vector<Card> pile1, pile2, pile3, pile4, pile5, pile6, pile7;
    vector<vector<Card>> h1_7;
    stack<Card> s8, s9, s10, s11;
    vector<stack<Card>> s8_11;

public:
    Gameplay();
    void generujTalie();
    void rozdaj();
    void printNaglowek();
    void printLinia();
    void printGora();
    void printDol();
    void printNumeracja();
    void wyswietlPlansze();
    void Menu();
    void dobierzKarty();
    void ruchKolumnaNaKolumne(int z, string kartaStr, int d);
    void ruchKolumnaNaStos(int z, string kartaStr, int d);
    void ruchChoiceNaKolumne(string kartaStr, int d);
    void ruchChoiceNaStos(string kartaStr, int d);
    void ruchStosNaKolumne(int z, string kartaStr, int d);
    bool czyJestWKartach(const Card& karta);
    bool Wygrana();
    void testSymulujWygrana();
};
