#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <vector>

using namespace std;

const string prompt = "~$";

enum inputType {
    CIN, GL
};

enum wynik {
    T, W1, W2, R
};

enum tura {
    G1, G2
};

const bool mono = false;
/*
    USTAWIC POWYZSZA STALA NA true W PRZYPADKU BLEDNEGO WYSWIETLANIA GRY
    PRZYKLAD:
        ←[35mWYBIERZ GRE←[35m zamiast WYBIERZ GRE
*/

const int wh = 25; //szerokosc ekranu
const int ww = 80; //wysokosc ekranu
const int pw = 25; //szerokosc planszy
const int ph = 13; //wysokosc planszy

const char pi1 = 0xB3; // │
const char pi2 = 0xBA; // ║
const char po1 = 0xC4; // ─
const char po2 = 0xCD; // ═
const char sr1 = 0xC5; // ┼
const char sr2 = 0xCE; // ╬
const char bol = 0xC3; // ├
const char bop = 0xB4; // ┤
const char rpg = 0xBF; // ┐
const char rlg = 0xDA; // ┌
const char rld = 0xC0; // └
const char rpd = 0xD9; // ┘
const char sd = 0xC1; // ┴
const char sg = 0xC2; // ┬
const char rlg2 = 0xC9; // ╔
const char rpg2 = 0xBB; // ╗
const char rld2 = 0xC8; // ╚
const char rpd2 = 0xBC; // ╝
//wartosci stalych typu char zapisuje w szesnastkowym dla wygody korzystania z tabeli dostepnej na wikipedii
const char z[6] = { 'O', 'X', 0x03, 0x04, 0x05, 0x06 }; // zetony

//kolory i dekoracje tekstu
const string bold = "\033[1m";

const string red = "\033[31m";
const string magenta = "\033[35m";
const string bgreen = "\033[32;1m";
const string white = "\033[37m";
const string byellow = "\033[33;1m";
const string blue = "\033[34m";
const string cyan = "\033[36m";

const string creset = "\033[0m";

struct t_plansza {
    char q[3][3]; //lewa gorna czesc planszy
    char w[3][3]; //prawa gorna czesc planszy
    char a[3][3]; //lewa dolna czesc planszy
    char s[3][3]; //prawa dolna czesc planszy
};

struct gracz {
    string imie;
    char zeton = ' ';
    int czas;
};

void menu();
string getinput(inputType x = CIN);
t_plansza inicjalizuj();
void centrowanalinia(string linia, string kolor, int off = 0);

struct ruch {
    t_plansza plansza = inicjalizuj();
    tura t = G1;
    string akcja = "";
};

void monochrom() {
    if (mono) {
        /*
        tryb monochromatyczny. potrzebny ze wzgledu na to,
        ze windowsowa linia komend domyslnie nie obsluguje
        sekwencji kolorow ANSI, a zeby je wlaczyc potrzebne
        jest uzycie zakazanej biblioteki windows.h

        sam nie wiem czy to jest legalne, z jednej strony
        uzywam stalych jako zmiennych globalnych ale z
        drugiej strony stale globalne sa dozwolone, a w
        projekcie nie jest wspomniane nic o modyfikacji
        stalych :)
        */
        string* temp = const_cast<string*>(&bold);
        *temp = "";
        temp = const_cast<string*>(&red);
        *temp = "";
        temp = const_cast<string*>(&magenta);
        *temp = "";
        temp = const_cast<string*>(&bgreen);
        *temp = "";
        temp = const_cast<string*>(&white);
        *temp = "";
        temp = const_cast<string*>(&byellow);
        *temp = "";
        temp = const_cast<string*>(&blue);
        *temp = "";
        temp = const_cast<string*>(&cyan);
        *temp = "";
        temp = const_cast<string*>(&creset);
        *temp = "";
    }
}

void poziome() {
    for (int j = 0; j < 3; j++)
        cout << po1;
}

void centrowanieplanszy() {
    for (int i = 0; i < (ww - pw) / 2 + 1; i++)
        cout << " ";
}

void gornaKrawedz() {
    centrowanieplanszy();
    cout << rlg;
    for (int i = 0; i < 2; i++) {
        poziome();
        cout << sg;
    }
    poziome();
    cout << pi2;
    for (int i = 0; i < 2; i++) {
        poziome();
        cout << sg;
    }
    poziome();
    cout << rpg << endl;
}

void dolnaKrawedz() {
    cout << rld;
    for (int i = 0; i < 2; i++) {
        poziome();
        cout << sd;
    }
    poziome();
    cout << pi2;
    for (int i = 0; i < 2; i++) {
        poziome();
        cout << sd;
    }
    poziome();
    cout << rpd << endl;
}

void gornaSekcja(t_plansza pl, int i) {
    centrowanieplanszy();
    for (char x : pl.q[i])
        cout << pi1 << " " << x << " ";
    cout << pi2;
    for (char x : pl.w[i])
        cout << " " << x << " " << pi1;
    cout << endl;
    centrowanieplanszy();
    if (i == 2) {
        for (int j = 0; j < 25; j++) {
            if (j == 12) {
                cout << sr2;
                continue;
            }
            cout << po2;
        }
        cout << endl;
        return;
    }
    cout << bol;
    poziome();
    for (int j = 0; j < 2; j++) {
        cout << sr1;
        poziome();
    }
    cout << pi2;
    for (int j = 0; j < 2; j++) {
        poziome();
        cout << sr1;
    }
    poziome();
    cout << bop << endl;
} //rysowanie gornych czesci planszy

void dolnaSekcja(t_plansza pl, int i) {
    centrowanieplanszy();
    for (char x : pl.a[i])
        cout << pi1 << " " << x << " ";
    cout << pi2;
    for (char x : pl.s[i])
        cout << " " << x << " " << pi1;
    cout << endl;
    centrowanieplanszy();
    if (i == 2)
        return;
    cout << bol;
    poziome();
    for (int j = 0; j < 2; j++) {
        cout << sr1;
        poziome();
    }
    cout << pi2;
    for (int j = 0; j < 2; j++) {
        poziome();
        cout << sr1;
    }
    poziome();
    cout << bop << endl;
} //rysowanie dolnych czesci planszy

void draw(t_plansza p, string kolor) {
    cout << kolor << endl;
    gornaKrawedz();
    for (int i = 0; i < 3; i++)
        gornaSekcja(p, i);
    for (int i = 0; i < 3; i++)
        dolnaSekcja(p, i);
    dolnaKrawedz();
    cout << creset;
}

t_plansza inicjalizuj() {  //wypelnienie tablicy spacjami dla zachowania odpowiednich proporcji i inicjalizacji
    t_plansza p;
    for (int i = 0; i < 3; i++)
        for (char& x : p.q[i])
            x = ' ';
    for (int i = 0; i < 3; i++)
        for (char& x : p.a[i])
            x = ' ';
    for (int i = 0; i < 3; i++)
        for (char& x : p.w[i])
            x = ' ';
    for (int i = 0; i < 3; i++)
        for (char& x : p.s[i])
            x = ' ';
    return p;
}

void cls(int off = 0) { //czyszczenie ekranu, argument off pozwala na wypisanie mniejszej ilosci nowych linii
    for (int x = wh; x > off; x--)
        cout << endl;
}

int gettoken(int gn) { //funkcja zwracajaca indeks wybranego zetonu
    int zet;
    do {
        if (gn == 1)
            cout << endl << "Zeton gracza 1: ";
        else
            cout << endl << "Zeton gracza 2: ";
        cin >> zet;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (zet < 1 || zet > 6 || cin.fail());
    return zet - 1;
}

void initplayers(gracz& g1, gracz& g2) { //pobieranie imion i zetonow graczy przed rozpoczeciem gry
    cout << magenta << "WCZYTYWANIE IMION GRACZY (MAX 25 ZNAKOW)" << creset << endl << endl;
    cout << "Imie gracza 1: " << endl;
    g1.imie = getinput(GL);
    cout << "Imie gracza 2: " << endl;
    g2.imie = getinput(GL);
    if (g1.imie.length() > 25 || g2.imie.length() > 25) {
        cls();
        cout << red << "ZA DLUGIE! WPISZ PONOWNIE" << endl;
        initplayers(g1, g2);
    }
    cout << "Zetony:" << endl;
    for (int i = 0; i < 6; i++)
        cout << i + 1 << " - " << z[i] << " | ";
    g1.zeton = z[gettoken(1)];
    do {
        g2.zeton = z[gettoken(2)];
        if (g1.zeton == g2.zeton)
            cout << red << "ZETONY OBU GRACZY NIE MOGA BYC TAKIE SAME" << creset << endl;
    } while (g1.zeton == g2.zeton);
    int limitczasu;
    do {
        cout << "Czas gry dla kazdego gracza: ";
        cin >> limitczasu;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (limitczasu <= 0 || cin.fail());
    g1.czas = limitczasu;
    g2.czas = limitczasu;
    cls();
}

string getinput(inputType x) { //funkcja pozwalajaca pobrac dane od gracza na 2 rozne sposoby
    string result;
    if (x == GL) {
        cout << bgreen << prompt << creset;
        getline(cin, result);
        return result;
    }
    do {
        cout << bgreen << prompt << creset;
        cin >> result;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (cin.fail());
    return result;
}

void zmianazetonu(gracz& g, gracz g2, t_plansza& p) { //funkcja odpowiadajaca za zmiane zetonu i zamiane zetonow na planszy
    cls();
    cout << endl << "Zetony:" << endl;
    for (int i = 0; i < 6; i++)
        cout << i + 1 << " - " << z[i] << " | ";
    int zn;
    do {
        cout << endl << "Wybierz zeton: ";
        cin >> zn;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (z[zn - 1] == g2.zeton)
            cout << red << "ZETONY OBU GRACZY NIE MOGA BYC TAKIE SAME" << creset << endl;
    } while (zn < 1 || zn > 6 || cin.fail() || z[zn - 1] == g2.zeton);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (p.q[i][j] == g.zeton)
                p.q[i][j] = z[zn - 1];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (p.w[i][j] == g.zeton)
                p.w[i][j] = z[zn - 1];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (p.a[i][j] == g.zeton)
                p.a[i][j] = z[zn - 1];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (p.s[i][j] == g.zeton)
                p.s[i][j] = z[zn - 1];

    g.zeton = z[zn - 1];
}

wynik przekatna1(char temp[6][6], int i, int j, tura t) { //sprawdzanie wygranej po przekatnej w kolko i krzyzyk
    if (temp[i][j] == temp[i + 1][j + 1] && temp[i + 1][j + 1] == temp[i + 2][j + 2] && temp[i][j] != ' ' && t == G1)
        return W2;
    else if (temp[i][j] == temp[i + 1][j + 1] && temp[i + 1][j + 1] == temp[i + 2][j + 2] && temp[i][j] != ' ' && t == G2)
        return W1;
    else return T;
}

wynik przekatna2(char temp[6][6], int i, int j, tura t) { //sprawdzanie wygranej po przekatnej w kolko i krzyzyk
    if (temp[i][j] == temp[i - 1][j + 1] && temp[i - 1][j + 1] == temp[i - 2][j + 2] && temp[i][j] != ' ' && t == G1)
        return W2;
    else if (temp[i][j] == temp[i - 1][j + 1] && temp[i - 1][j + 1] == temp[i - 2][j + 2] && temp[i][j] != ' ' && t == G2)
        return W1;
    else return T;
}

char przekatnaptg1(char temp[6][6], int i, int j) { //sprawdzanie wygranej po przekatnej w pentago
    if (temp[i][j] == temp[i + 1][j + 1] && temp[i + 1][j + 1] == temp[i + 2][j + 2] && temp[i + 2][j + 2] == temp[i + 3][j + 3] && temp[i + 3][j + 3] == temp[i + 4][j + 4] && temp[i][j] != ' ')
        return temp[i][j];
    else return ' ';
}

char przekatnaptg2(char temp[6][6], int i, int j) { //sprawdzanie wygranej po przekatnej w pentago
    if (temp[i][j] == temp[i - 1][j + 1] && temp[i - 1][j + 1] == temp[i - 2][j + 2] && temp[i - 2][j + 2] == temp[i - 3][j + 3] && temp[i - 3][j + 3] == temp[i - 4][j + 4] && temp[i][j] != ' ')
        return temp[i][j];
    else return ' ';
}

void przegladajruchy(vector<ruch> r) { //przegladanie rozgrywki po zakonczeniu gry
    r.erase(r.begin()); //usuniecie pierwszego ruchu, jest to pusta plansza bez ktorej undo nie dzialalo
    int idx = 0;
    string tekst[3];
    while (true) {

        tekst[0] = "RUCH " + to_string(idx + 1) + " z " + to_string(r.size());
        tekst[1] = "TURA GRACZA " + to_string(r[idx].t + 1);
        tekst[2] = "WYKONANY RUCH: " + r[idx].akcja;
        cls();
        for (string s : tekst)
            centrowanalinia(s, byellow);
        cls(wh - 3);
        draw(r[idx].plansza, white);
        cout << endl;
        cout << endl << endl << "(z) poprzedni ruch | (x) kolejny ruch | (q) wyjdz do menu" << endl;
        string x = getinput();
        if (x == "z" && idx > 0)
            idx--;
        else if (x == "x" && idx < r.size() - 1)
            idx++;
        else if (x == "q")
            return;
    }
}

void predef(t_plansza& p, tura& t, char z1, char z2) { //wczytanie planszy predefiniowanej
    p.q[0][1] = z2;
    p.q[1][0] = z1;
    p.q[1][1] = z1;
    p.q[1][2] = z2;
    p.q[2][1] = z1;
    p.w[0][1] = z1;
    p.w[1][0] = z1;
    p.w[1][1] = z2;
    p.a[0][0] = z1;
    p.a[0][1] = z1;
    p.a[1][0] = z2;
    p.a[1][2] = z1;
    p.a[2][1] = z2;
    p.s[1][1] = z2;
    p.s[2][1] = z2;
    p.s[2][2] = z2;
    t = G1;
}

void wygrana(gracz g, t_plansza p, vector<ruch> r) { //ekran informujacy o wygranej
    cls();
    draw(p, bgreen);
    string tekst1 = "GRACZ " + g.imie + "(" + g.zeton + ") WYGRYWA!!!";
    string tekst2 = "Wcisnij enter aby wyswietlic ruchy...";
    centrowanalinia(tekst1, bgreen);
    for (int i = 0; i < 5; i++)
        cout << endl;
    cout << magenta << tekst2 << creset << endl;
    string temp;
    getline(cin, temp);
    przegladajruchy(r);
}

void remis(t_plansza p, vector<ruch> ruchy) { //ekran informujacy o remisie
    cls();
    draw(p, byellow);
    string tekst1 = "REMIS";
    string tekst2 = "Wcisnij enter aby wyswietlic ruchy...";
    centrowanalinia(tekst1, byellow);
    for (int i = 0; i < 5; i++)
        cout << endl;
    cout << magenta << tekst2 << creset << endl;
    string temp;
    getline(cin, temp);
}

void dosprawdzenia(t_plansza p, char(&temp)[6][6]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp[i][j] = p.q[i][j];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp[i + 3][j] = p.a[i][j];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp[i][j + 3] = p.w[i][j];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            temp[i + 3][j + 3] = p.s[i][j];
} // funkcja wpisujaca dane z planszy do tablicy 6x6 w celu ulatwienia sprawdzania

wynik sprawdzptg(t_plansza p, tura t, char g1z, char g2z) { //sprawdzanie wygranej w pentago
    char temp[6][6];
    dosprawdzenia(p, temp);
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            if (temp[i][j] == temp[i][j + 1] && temp[i][j + 1] == temp[i][j + 2] && temp[i][j + 2] == temp[i][j + 3] && temp[i][j + 3] == temp[i][j + 4] && temp[i][j] != ' ') {
                if (t == G1)
                    return W2;
                else
                    return W1;
            } //sprawdzenie w pionie
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 6; j++)
            if (temp[i][j] == temp[i + 1][j] && temp[i + 1][j] == temp[i + 2][j] && temp[i + 2][j] == temp[i + 3][j] && temp[i + 3][j] == temp[i + 4][j] && temp[i][j] != ' ') {
                if (temp[i][j] == g2z)
                    return W2;
                else if (temp[i][j] == g1z)
                    return W1;
            } //sprawdzanie w poziomie

    char zetonwygranego;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            zetonwygranego = przekatnaptg1(temp, i, j);
            if (zetonwygranego == g1z)
                return W1;
            else if (zetonwygranego == g2z)
                return W2;
        } //sprawdzanie po przekatnej 1
    for (int i = 4; i < 6; i++)
        for (int j = 0; j < 2; j++) {
            zetonwygranego = przekatnaptg2(temp, i, j);
            if (zetonwygranego == g1z)
                return W1;
            else if (zetonwygranego == g2z)
                return W2;
        } //sprawdzanie po przekatnej 2
    bool jestpuste = false;
    for (int i = 0; i < 6; i++) //sprawdzanie czy na planszy zostaly jakies puste miejsca
        for (char x : temp[i])
            if (x == ' ')
                jestpuste = true;
    
    if (!jestpuste)
        return R; //jesli nie ma pustych miejsc - remis

    return T;
}

wynik sprawdzkik(t_plansza p, tura t) { //sprawdzanie wygranej w kolko i krzyzyk
    char temp[6][6];
    dosprawdzenia(p, temp);
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 5; j++)
            if (temp[i][j] == temp[i][j + 1] && temp[i][j + 1] == temp[i][j + 2] && temp[i][j] != ' ') {
                if (t == G1)
                    return W2;
                else
                    return W1;
            } //sprawdzanie w pionie
    for (int j = 0; j < 6; j++)
        for (int i = 0; i < 5; i++)
            if (temp[i][j] == temp[i + 1][j] && temp[i + 1][j] == temp[i + 2][j] && temp[i][j] != ' ') {
                if (t == G1)
                    return W2;
                else
                    return W1;
            } //sprawdzanie w poziomie

    wynik wp;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            wp = przekatna1(temp, i, j, t);
            if (wp == W1)
                return wp;
            else if (wp == W2)
                return wp;
        } //sprawdzanie po przekatnej 1
    for (int i = 2; i < 6; i++)
        for (int j = 0; j < 4; j++) {
            wp = przekatna2(temp, i, j, t);
            if (wp == W1)
                return wp;
            else if (wp == W2)
                return wp;
        } //sprawdzanie po przekatnej 2

    bool jestpuste = false;
    for (int i = 0; i < 6; i++)
        for (char x : temp[i])
            if (x == ' ')
                jestpuste = true;;

    if (!jestpuste)
        return R; //remis

    return T;
}

void centrowanalinia(string linia, string kolor, int off) {
    for (int i = 0; i < ((ww - off) - linia.length()) / 2; i++)
        cout << " ";
    if (linia.length() % 2 == 1)
        cout << " ";
    cout << kolor << linia << magenta;
    for (int i = 0; i < ((ww - off) - linia.length()) / 2; i++)
        cout << " ";
} //funkcja wypisujaca tekst na srodku ekranu (offset na potrzeby tekstu znajdujacego sie w ramce)

string ekran(t_plansza plansza, gracz g1, gracz g2, int tura, string kolor, bool obr = false) { //glowny ekran wyswietlany podczas rozgrywki
    string linie[6][2] =
    {
            {"", white},
            {"<GRACZ 1: " + g1.imie + " (" + g1.zeton + ")>   <GRACZ 2: " + g2.imie + " (" + g2.zeton + ")>", white},
            {"TURA GRACZA " + to_string(tura), white },
            {"", white},
            {"", white},
            {"(p) pauza | (u) undo | (o) wczytanie predef. planszy | (m) opcje | (h) pomoc ", bgreen}
    };
    if (tura == 1)
        linie[3][0] = "POZOSTALY CZAS GRACZA <" + g1.imie + ">: " + to_string(g1.czas) + " SEKUND";
    else
        linie[3][0] = "POZOSTALY CZAS GRACZA <" + g2.imie + ">: " + to_string(g2.czas) + " SEKUND";
    draw(plansza, kolor);
    cout << endl;
    cout << magenta;
    cout << rlg2;
    for (int i = 0; i < ww - 2; i++)
        cout << po2;
    cout << rpg2;
    cout << endl;
    for (int j = 0; j < 6; j++) {
        string linia = linie[j][0];
        string kolor = linie[j][1];
        cout << pi2;
        centrowanalinia(linia, kolor, 2);
        cout << pi2 << endl;
    }
    cout << magenta;
    cout << rld2;
    for (int i = 0; i < ww - 2; i++)
        cout << po2;
    cout << rpd2;
    cout << creset << endl;
    if (!obr)
        cout << bgreen << "Wprowadz akcje:" << creset << endl;
    else
        cout << bgreen << "Wprowadz obrot:" << creset << endl;
    return getinput();
}

bool wstawzeton(char& p, char t) { //wstawianie zetonu + spradzanie czy pole jest puste
    if (p == ' ') {
        p = t;
        return 1;
    }
    else
        return 0;
}

void pomoc(time_t& czas) { //ekran pomocy
    cls();
    string linie[8][2] =
    {
        {"WCZYTANIE PREDEFINIOWANEJ PLANSZY: (o)", cyan},
        {"USTAWIANIE ZETONU: q/w/a/s + 1-9", bgreen},
        {"(TYLKO PENTAGO) obrot: q/w/a/s + z/x", bgreen},
        {"Poszczegolne sekcje planszy ulozone sa tak jak przyciski na numpadzie", bgreen},
        {"(z) obrot zgodnie ze wskazowkami zegara", bgreen},
        {"(x) obrot przeciwny do wskazowek zegara", bgreen},
        {"WYGRYWA GRACZ KTORY ULOZY 3(kolko i krzyzyk)/5(pentago) ZETONOW W JEDNEJ LINII", bgreen},
        {"Przykladowe ruchy: ", bgreen}
    
    }; //taki zapis pozwala na ustalenie koloru dla konkretnej linii bez zaburzania wartosci zwracanej przez funkcje length()
    char ruchy[6] = { 'q', 'w', 'a', 's', 'z', 'x' };
    linie[7][0] += ruchy[rand() % 4];
    linie[7][0] += to_string((rand() % 9) + 1);
    linie[7][0] += " ";
    linie[7][0] += ruchy[rand() % 4];
    linie[7][0] += to_string((rand() % 9) + 1);
    linie[7][0] += " ";
    linie[7][0] += ruchy[rand() % 4];
    linie[7][0] += to_string((rand() % 9) + 1);
    linie[7][0] += " ";
    linie[7][0] += ruchy[rand() % 4];
    linie[7][0] += ruchy[(rand() % 2) + 4];
    string tekst2 = "Wcisnij enter aby powrocic do gry...";
    for (auto s : linie) {
        centrowanalinia(s[0], s[1]);
    }
    cls(wh / 2 + 5);
    cout << magenta << tekst2 << creset << endl;
    string temp;
    getline(cin, temp);
    czas = time(NULL);
    return;
}

void pauza(time_t& czas) {
    time_t czasodpoczatku = time(NULL) - czas;
    cls();
    string tekst1 = "PAUZA";
    string tekst2 = "Wcisnij enter aby kontynuowac";
    centrowanalinia(tekst1, magenta);
    for (int i = 0; i < wh / 2 - 3; i++)
        cout << endl;
    cout << magenta << tekst2 << creset << endl;
    getinput(GL);
    czas = time(NULL) - czasodpoczatku; //"zatrzymuje" zegar zachowujac czas od poczatku rundy do wlaczenia pauzy
}

void zmianakoloru(string& kolor) {
    bool poszlo = false;
    string header[2] = { "WYBIERZ KOLOR", white };
    string linie[7][2] =
    {
            {"(1) bialy", white},
            {"(2) czerwony", red},
            {"(3) zielony", bgreen},
            {"(4) niebieski", blue},
            {"(5) cyjan", cyan},
            {"(6) magenta", magenta},
            {"(7) zolty", byellow}
    };
    do {
        cls();
        centrowanalinia(header[0], header[1]);
        cls(wh - 8);
        for (auto linia : linie)
            centrowanalinia(linia[0], linia[1]);
        cls(wh - 8);
        string in = getinput();
        if (in.length() == 1) {
            if (in == "1") {
                kolor = white;
                poszlo = true;
            }
            else if (in == "2") {
                kolor = red;
                poszlo = true;
            }
            else if (in == "3") {
                kolor = bgreen;
                poszlo = true;
            }
            else if (in == "4") {
                kolor = blue;
                poszlo = true;
            }
            else if (in == "5") {
                kolor = cyan;
                poszlo = true;
            }
            else if (in == "6") {
                kolor = magenta;
                poszlo = true;
            }
            else if (in == "7") {
                kolor = byellow;
                poszlo = true;
            }

        }

    } while (!poszlo);
}

void opcje(gracz& g, gracz g2, t_plansza& p, time_t& czas, string& kolor) {
    time_t czasodpoczatku = time(NULL) - czas;

    cls();
    bool poszlo = false;
    string in;
    string linie[4][2] =
    {
            {"OPCJE", blue},
            {"(1) zmiana zetonu", cyan},
            {"(2) zmiana koloru planszy", cyan},
            {"(e) powrot do gry", cyan}
    };

    if (mono)
        linie[2][0] = ""; //usuniecie opcji zmiany koloru planszy w przypadku ustawienia trybu monochromatycznego

    do {
        centrowanalinia(linie[0][0], linie[0][1]);
        cls(wh - 10);
        for (int i = 1; i < 4; i++)
            centrowanalinia(linie[i][0], linie[i][1]);
        cls(wh - 12);
        in = getinput();
        if (in.length() == 1) {
            if (in == "1") {
                zmianazetonu(g, g2, p);
                poszlo = true;
            }
            else if (in == "2" && !mono) {
                zmianakoloru(kolor);
                poszlo = true;
            }
            else if (in == "e")
                poszlo = true;
        }
    } while (!poszlo);

    czas = time(NULL) - czasodpoczatku;
}

void undo(t_plansza& p, vector<ruch>& ruchy, tura& t) {
    int tempsize = ruchy.size();
    if (tempsize > 1)
        ruchy.pop_back();
        ruch temp = ruchy.back();
        p = temp.plansza;
        t = tura(tempsize % 2);
}

bool akcja(string a, gracz& g, gracz g2, t_plansza& p, time_t& czas, tura& t, string& k, vector<ruch>& ruchy) { //funkcja przetwazajaca wszystkie akcje w grze poza obrotem
    if (a.length() == 1) {
        if (a == "h") {
            pomoc(czas);
            return 0;
        }
        else if (a == "m") {
            opcje(g, g2, p, czas, k);
            return 0;
        }
        else if (a == "p") {
            pauza(czas);
            return 0;
        }
        else if (a == "o") {
            predef(p, t, g.zeton, g2.zeton);
            ruch r;
            r.plansza = p;
            r.t = G1;
            r.akcja = "PREDEF";
            ruchy.push_back(r);
            return 0;
        }
        else if (a == "u") {
            undo(p, ruchy, t);
            return 0;
        }
        else
            return 0;
    }
    if (a.length() == 2) {
        char kwadrat = a[0];
        unsigned int pole = (a[1] - '0') - 1;
        if (pole >= 0 && pole <= 9) {
            if (kwadrat == 'q') {
                if (pole < 3 && pole >= 0)
                    return wstawzeton(p.q[2][pole], g.zeton);
                else if (pole < 6)
                    return wstawzeton(p.q[1][pole - 3], g.zeton);
                else if (pole < 9)
                    return wstawzeton(p.q[0][pole - 6], g.zeton);
                else
                    return 0;
            }
            else if (kwadrat == 'w') {
                if (pole < 3)
                    return wstawzeton(p.w[2][pole], g.zeton);
                else if (pole < 6)
                    return wstawzeton(p.w[1][pole - 3], g.zeton);
                else if (pole < 9)
                    return wstawzeton(p.w[0][pole - 6], g.zeton);
                else
                    return 0;
            }
            else if (kwadrat == 'a') {
                if (pole < 3)
                    return wstawzeton(p.a[2][pole], g.zeton);
                else if (pole < 6)
                    return wstawzeton(p.a[1][pole - 3], g.zeton);
                else if (pole < 9)
                    return wstawzeton(p.a[0][pole - 6], g.zeton);
                else
                    return 0;
            }
            else if (kwadrat == 's') {
                if (pole < 3)
                    return wstawzeton(p.s[2][pole], g.zeton);
                else if (pole < 6)
                    return wstawzeton(p.s[1][pole - 3], g.zeton);
                else if (pole < 9)
                    return wstawzeton(p.s[0][pole - 6], g.zeton);
                else
                    return 0;
            }
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

bool obrotz(char(&a)[3][3]) { // obrot zgodnie ze wskazowkami zegara
    for (int j = 0; j < 2; j++) {
        int temp = a[0][j];
        a[0][j] = a[2 - j][0];
        a[2 - j][0] = a[2][2 - j];
        a[2][2 - j] = a[j][2];
        a[j][2] = temp;
    }
    return 1;
}

bool obrotx(char(&a)[3][3]) { //obrot przeciwny do wskazowek zegara
    for (int j = 0; j < 2; j++) {
        int temp = a[0][j];
        a[0][j] = a[j][2];
        a[j][2] = a[2][2 - j];
        a[2][2 - j] = a[2 - j][0];
        a[2 - j][0] = temp;
    }
    return 1;
}

bool obrot(string a, t_plansza& p) { //funkcja przetwazajaca obroty
    if (a.length() == 2) {
        char kwadrat = a[0];
        char kierunek = a[1];
        if (kwadrat == 'q') {
            if (kierunek == 'z')
                return obrotz(p.q);
            else if (kierunek == 'x')
                return obrotx(p.q);
            else
                return 0;
        }
        else if (kwadrat == 'w') {
            if (kierunek == 'z')
                return obrotz(p.w);
            else if (kierunek == 'x')
                return obrotx(p.w);
            else
                return 0;
        }
        else if (kwadrat == 'a') {
            if (kierunek == 'z')
                return obrotz(p.a);
            else if (kierunek == 'x')
                return obrotx(p.a);
            else
                return 0;
        }
        else if (kwadrat == 's') {
            if (kierunek == 'z')
                return obrotz(p.s);
            else if (kierunek == 'x')
                return obrotx(p.s);
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

void timeout(t_plansza p, gracz g, vector<ruch> r) { //ekran informujacy o koncu czasu
    cls();
    draw(p, magenta);
    string tekst1 = "KONIEC CZASU! GRACZ <" + g.imie + "> ZAMULIL";
    string tekst2 = "Wcisnij enter aby wyswietlic ruchy...";
    centrowanalinia(tekst1, byellow);
    for (int i = 0; i < 5; i++)
        cout << endl;
    cout << magenta << tekst2 << creset << endl;
    getinput(GL);
    przegladajruchy(r);
}

void kik() { //funkcja gry w kolko i krzyzyk
    string kolor = white;
    tura t = G1;
    gracz g1, g2;
    cls();
    initplayers(g1, g2);
    t_plansza plansza = inicjalizuj();
    time_t czas;
    vector<ruch> ruchy;
    ruch pierwszy;
    pierwszy.akcja = "";
    pierwszy.plansza = plansza;
    pierwszy.t = t;
    ruchy.push_back(pierwszy);
    while (true) {
        cout << ruchy.size();
        czas = time(NULL);
        if (g1.czas <= 0) {
            timeout(plansza, g1, ruchy);
            return;
        }
        if (g2.czas <= 0) {
            timeout(plansza, g2, ruchy);
            return;
        }
        string a = ekran(plansza, g1, g2, t + 1, kolor);
        if (t == G1) {
            if (akcja(a, g1, g2, plansza, czas, t, kolor, ruchy)) {
                g1.czas -= time(NULL) - czas;
                t = G2;
            }
            else {
                g1.czas -= time(NULL) - czas;
                continue;
            }
        }
        else {
            if (akcja(a, g2, g1, plansza, czas, t, kolor, ruchy)) {
                g2.czas -= time(NULL) - czas;
                t = G1;
            }
            else {
                g2.czas -= time(NULL) - czas;
                continue;
            }
        }
        ruch r;
        r.plansza = plansza;
        if (t == G1)
            r.t = G2;
        else
            r.t = G1;
        r.akcja = a;
        ruchy.push_back(r);
        wynik w = sprawdzkik(plansza, t);
        if (w == W1) {
            wygrana(g1, plansza, ruchy);
            return;
        }
        else if (w == W2) {
            wygrana(g2, plansza, ruchy);
            return;
        }
        else if (w == R) {
            remis(plansza, ruchy);
            return;
        }
    }
}

void pentago() { //funkcja gry w pentago
    string kolor = white;
    tura t = G1;
    gracz g1, g2;
    cls();
    initplayers(g1, g2);
    t_plansza plansza = inicjalizuj();
    time_t czas;
    vector<ruch> ruchy;
    ruch pierwszy;
    pierwszy.akcja = "";
    pierwszy.plansza = plansza;
    pierwszy.t = t;
    ruchy.push_back(pierwszy);
    while (true) {
        string tempakcja = "";
        czas = time(NULL);
        if (g1.czas <= 0) {
            timeout(plansza, g1, ruchy);
            return;
        }
        if (g2.czas <= 0) {
            timeout(plansza, g2, ruchy);
            return;
        }
        string a = ekran(plansza, g1, g2, t + 1, kolor);
        tempakcja += a;
        if (t == G1) {
            if (akcja(a, g1, g2, plansza, czas, t, kolor, ruchy)) {
                a = ekran(plansza, g1, g2, t + 1, kolor, true);
                while (!obrot(a, plansza))
                    a = ekran(plansza, g1, g2, t + 1, kolor, true);
                tempakcja += (" " + a);
                t = G2;
                g1.czas -= time(NULL) - czas;
            }
            else {
                g1.czas -= time(NULL) - czas;
                continue;
            }
        }
        else {
            if (akcja(a, g2, g1, plansza, czas, t, kolor, ruchy)) {
                a = ekran(plansza, g1, g2, t + 1, kolor, true);
                while (!obrot(a, plansza)) {
                    a = ekran(plansza, g1, g2, t + 1, kolor, true);
                }
                tempakcja += (" " + a);
                t = G1;
                g2.czas -= time(NULL) - czas;
            }
            else {
                g2.czas -= time(NULL) - czas;
                continue;
            }
        }
        ruch r;
        r.plansza = plansza;
        if (t == G1)
            r.t = G2;
        else
            r.t = G1;
        r.akcja = tempakcja;
        ruchy.push_back(r);
        wynik w = sprawdzptg(plansza, t, g1.zeton, g2.zeton);

        if (w == W1) {
            wygrana(g1, plansza, ruchy);
            return;
        }
        else if (w == W2) {
            wygrana(g2, plansza, ruchy);
            return;
        }
        else if (w == R) {
            remis(plansza, ruchy);
            return;
        }
    }
}

void menu() { //skromne menu glowne
    string tekst[4];
    tekst[0] = "PROJEKT 1 \"PENTAGO\" - PIOTR MADRAK 188572";
    tekst[1] = "WYBIERZ GRE";
    tekst[2] = "(1) PENTAGO";
    tekst[3] = "(2) KOLKO I KRZYZYK";
    centrowanalinia(tekst[0], magenta);
    cls(wh - 10);
    for (int i = 1; i < 4; i++)
        centrowanalinia(tekst[i], magenta);
    cls(wh - 12);
    string in = getinput(CIN);
    cout << endl;
    if (in == "1")
        pentago();
    else if (in == "2")
        kik();
    else
        return;
}

int main()
{
    monochrom();
    srand(time(NULL));
    while (true)
        menu();
}