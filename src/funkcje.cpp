#include "funkcje.h"

Card::Card() {}

Card::Card(int val, int sha, int col) {
    value = val;
    shape = sha;
    is_face_up = false;

    string card_shape[] = { "H", "D", "C", "S" };
    string card_value[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

    card_description = card_shape[shape] + card_value[value];
    color = (shape == KIER || shape == KARO) ? CZERWONY : CZARNY;
}

Gameplay::Gameplay()
    : h1_7{ pile1, pile2, pile3, pile4, pile5, pile6, pile7 },
    s8_11{ s8, s9, s10, s11 } {}

void Gameplay::generujTalie() {
    int index = 0;
    for (int shape = 0; shape < 4; ++shape) {
        for (int val = 1; val <= 13; ++val) {
            deck[index++] = Card(val, shape, (shape == KIER || shape == KARO) ? CZERWONY : CZARNY);
        }
    }

    srand(static_cast<unsigned int>(time(0)));

    for (int i = MAX_C - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
}

void Gameplay::rozdaj() {
    int position = 0;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j <= i; ++j) {
            h1_7[i].push_back(deck[position]);
            if (j == i) h1_7[i][j].is_face_up = true;
            position++;
        }
    }

    for (int i = position; i < MAX_C; ++i) {

        choice.push(deck[i]);
    }

    for (int i = 0; i < 3 && !choice.empty(); ++i) {
        Card temp = choice.front();
        choice.pop();
        temp.is_face_up = true;
        odkryte.push_back(temp);
    }
};

void Gameplay::printNaglowek() {
    cout << "      0                    8      9      10     11\n";  // PrzesuniÍcie o 6
}

void Gameplay::printLinia() {
    cout << "   --------------------------------------------------\n";
}

void Gameplay::printGora() {
    cout << "      "; // odstÍp dla numeru 0

    int n = odkryte.size();
    for (int i = 0; i < 3; ++i) {
        if (i < odkryte.size()) {
            if (i < (3 - n)) {
                cout << setw(6) << " ";
            }
            else {
                cout << setw(6) << left << odkryte[i - (3 - n)].card_description;
            }
        }
    }


    cout << "     "; // odstÍp do stosÛw

    // Stosy 8ñ11
    for (int i = 0; i < 4; ++i) {
        if (!s8_11[i].empty())
            cout << setw(7) << left << s8_11[i].top().card_description;
        else
            cout << setw(7) << left << "0";
    }

    cout << endl;
}


void Gameplay::printDol() {
    const int STALA_WYSOKOSC = 15;

    for (int i = 0; i < STALA_WYSOKOSC; i++) {
        cout << "   ";  // Pozostawiamy to tak, jak by≥o, bez przesuniÍcia
        for (int j = 0; j < 7; j++) {
            cout << "| ";
            if (i < h1_7[j].size()) {
                if (h1_7[j][i].is_face_up)
                    cout << setw(4) << left << h1_7[j][i].card_description << " ";
                else
                    cout << "===  ";
            }
            else {
                cout << "     ";
            }
        }
        cout << "|\n";
    }
}

void Gameplay::printNumeracja() {
    cout << "       ";  // Pozostawiamy numeracjÍ tak, jak by≥a
    for (int i = 1; i <= 7; i++) {
        cout << setw(7) << i;
    }
    cout << endl;
};

void Gameplay::wyswietlPlansze() {
    printNaglowek();
    printLinia();
    printGora();
    printLinia();
    printDol();
    printNumeracja();
};

//RUCHY

void Gameplay::Menu() {
    int z, d;
    string kartaStr;

    generujTalie();
    rozdaj();

    while (!Wygrana())
    {
        wyswietlPlansze();

        cout << "\n>> ";
        cin >> z;

        Wygrana();

        if (z == 0 && cin.peek() == '\n') {
            dobierzKarty();
            continue;
        }

        cin >> kartaStr >> d;

        if (z >= 1 && z <= 7 && d >= 1 && d <= 7) {
            ruchKolumnaNaKolumne(z, kartaStr, d);
        }
        else if (z >= 1 && z <= 7 && d >= 8 && d <= 11) {
            ruchKolumnaNaStos(z, kartaStr, d);
        }
        else if (z == 0 && d >= 1 && d <= 7) {
            ruchChoiceNaKolumne(kartaStr, d);
        }
        else if (z == 0 && d >= 8 && d <= 11) {
            ruchChoiceNaStos(kartaStr, d);
        }
        else if (z >= 8 && z <= 11 && d >= 1 && d <= 7) {
            ruchStosNaKolumne(z, kartaStr, d);
        }
        else {
            cout << "Nieprawidlowa komenda.\n";
        }
    }
}

bool Gameplay::czyJestWKartach(const Card& karta) {
    // Sprawdü karty na stole
    for (auto& kolumna : h1_7) {
        for (auto& k : kolumna) {
            if (k.card_description == karta.card_description) return true;
        }
    }
    // Sprawdü stosy koÒcowe 8-11
    for (auto& stos : s8_11) {
        stack<Card> temp = stos;
        while (!temp.empty()) {
            if (temp.top().card_description == karta.card_description) return true;
            temp.pop();
        }
    }
    // Sprawdü odkryte
    for (auto& k : odkryte) {
        if (k.card_description == karta.card_description) return true;
    }

    return false;
}

void Gameplay::dobierzKarty() {
    if (choice.empty() && odkryte.empty()) {
        cout << "\nResetowanie stosu dobierania...\n";

        // Jeúli choice i odkryte puste - resetujemy
        for (auto& karta : deck) {
            if (!czyJestWKartach(karta)) {
                karta.is_face_up = false;
                choice.push(karta);
            }
        }
    }

    odkryte.clear();

    int ile = min(3, (int)choice.size());
    vector<Card> temp_cards;
    for (int i = 0; i < ile; ++i) {
        Card temp = choice.front();
        choice.pop();
        temp.is_face_up = true;
        temp_cards.push_back(temp);
    }

    // Dodajemy je tak, øeby najnowsza by≥a na koncu (czyli po prawej)
    for (int i = 0; i < temp_cards.size(); ++i) {
        odkryte.push_back(temp_cards[i]);
    }
}

void Gameplay::ruchKolumnaNaKolumne(int z, string kartaStr, int d) {
    vector<Card>& kolumna = h1_7[z - 1];

    int idx = -1;
    for (int i = 0; i < kolumna.size(); ++i) {
        if (kolumna[i].card_description == kartaStr && kolumna[i].is_face_up) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "Nie znaleziono odkrytej karty " << kartaStr << " w kolumnie " << z << ".\n";
        return;
    }

    vector<Card> tymczasowe(kolumna.begin() + idx, kolumna.end());
    vector<Card>& cel = h1_7[d - 1];

    if (!cel.empty()) {
        Card& top = cel.back();
        if (top.color == tymczasowe[0].color || top.value != tymczasowe[0].value + 1) {
            cout << "Nie moøna polozyc ñ kolor lub wartosc sie nie zgadza.\n";
            return;
        }
    }
    else {
        if (tymczasowe[0].value != 13) {
            cout << "Na pusta kolumne mozna polozyc tylko krola.\n";
            return;
        }
    }

    kolumna.erase(kolumna.begin() + idx, kolumna.end());
    cel.insert(cel.end(), tymczasowe.begin(), tymczasowe.end());
    if (!kolumna.empty()) kolumna.back().is_face_up = true;
}

void Gameplay::ruchKolumnaNaStos(int z, string kartaStr, int d) {
    vector<Card>& kolumna = h1_7[z - 1];
    if (kolumna.empty() || !kolumna.back().is_face_up) {
        cout << "Brak odkrytej karty na koncu kolumny.\n";
        return;
    }

    Card karta = kolumna.back();
    if (karta.card_description != kartaStr) {
        cout << "Na koncu kolumny nie ma " << kartaStr << ".\n";
        return;
    }

    stack<Card>& stos = s8_11[d - 8];
    if (stos.empty()) {
        if (karta.value != 1) {
            cout << "Na pusty stos mozna polozyc tylko Asa.\n";
            return;
        }
    }
    else {
        Card top = stos.top();
        if (top.shape != karta.shape || karta.value != top.value + 1) {
            cout << "Nieprawidlowy ruch do stosu koncowego.\n";
            return;
        }
    }

    stos.push(karta);
    kolumna.pop_back();
    if (!kolumna.empty()) kolumna.back().is_face_up = true;
}

void Gameplay::ruchChoiceNaKolumne(string kartaStr, int d) {
    if (odkryte.empty()) {
        cout << "Brak kart do zagrania.\n";
        return;
    }

    Card prawa = odkryte.back();  // prawa karta

    if (prawa.card_description != kartaStr) {
        cout << "Moøesz zagrac tylko karte " << prawa.card_description << " (prawa karta)!\n";
        return;
    }

    vector<Card>& cel = h1_7[d - 1];

    if (!cel.empty()) {
        Card& top = cel.back();
        if (top.color == prawa.color || top.value != prawa.value + 1) {
            cout << "Nie mozna polozyc tej karty na kolumne.\n";
            return;
        }
    }
    else {
        if (prawa.value != 13) {
            cout << "Na pusta kolumne mozesz polozyc tylko krola.\n";
            return;
        }
    }

    // zagranie kartÍ
    odkryte.pop_back();
    cel.push_back(prawa);

    //automatycznie dobiera nowπ jeúli coú jeszcze jest w kolejce
    if (!choice.empty()) {
        Card nowa = choice.front();
        choice.pop();
        nowa.is_face_up = true;
        odkryte.insert(odkryte.begin(), nowa);
    }

}



void Gameplay::ruchChoiceNaStos(string kartaStr, int d) {
    if (odkryte.empty()) {
        cout << "Brak kart do zagrania.\n";
        return;
    }

    Card prawa = odkryte.back();  // prawa karta

    if (prawa.card_description != kartaStr) {
        cout << "Moøesz zagraÊ tylko kartÍ " << prawa.card_description << " (prawa karta)!\n";
        return;
    }

    stack<Card>& stos = s8_11[d - 8];

    if (stos.empty()) {
        if (prawa.value != 1) {
            cout << "Na pusty stos moøna po≥oøyÊ tylko Asa.\n";
            return;
        }
    }
    else {
        Card top = stos.top();
        if (top.shape != prawa.shape || prawa.value != top.value + 1) {
            cout << "Nieprawid≥owy ruch na stos.\n";
            return;
        }
    }

    odkryte.pop_back();
    stos.push(prawa);

    if (!choice.empty()) {
        Card nowa = choice.front();
        choice.pop();
        nowa.is_face_up = true;
        odkryte.insert(odkryte.begin(), nowa);
    }
}

void Gameplay::ruchStosNaKolumne(int z, string kartaStr, int d) {
    stack<Card>& stos = s8_11[z - 8];

    if (stos.empty()) {
        cout << "Stos " << z << " jest pusty.\n";
        return;
    }

    Card karta = stos.top();

    if (karta.card_description != kartaStr) {
        cout << "Na wierzchu stosu " << z << " nie ma karty " << kartaStr << ".\n";
        return;
    }

    vector<Card>& kolumna = h1_7[d - 1];

    if (!kolumna.empty()) {
        Card& top = kolumna.back();
        if (top.color == karta.color || top.value != karta.value + 1) {
            cout << "Nie moøna po≥oøyÊ tej karty na kolumnÍ.\n";
            return;
        }
    }
    else {
        if (karta.value != 13) {
            cout << "Na pustπ kolumnÍ moøesz po≥oøyÊ tylko krÛla.\n";
            return;
        }
    }

    stos.pop();
    karta.is_face_up = true; // odkryta karta na kolumnie
    kolumna.push_back(karta);
}


//funkcja wygranej- kiedy 4 stosy sa zape≥nione 

bool Gameplay::Wygrana()
{
    for (int i = 0; i < 4; i++)
    {
        //sprawdza czy jest 13 kart na kazdym stosie, jak sa to ttrue a jak nie to false
        if (s8_11[i].size() != 13)
        {
            return false;
        }
    }
    return true;
}

void Gameplay::testSymulujWygrana() {
    for (int kolor = 0; kolor < 4; ++kolor) {
        for (int wartosc = 1; wartosc <= 13; ++wartosc) {
            Card karta(wartosc, kolor, kolor < 2 ? CZERWONY : CZARNY);
            karta.is_face_up = true;
            s8_11[kolor].push(karta);
        }
    }
}
