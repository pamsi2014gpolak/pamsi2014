#include <iostream>
#include <Windows.h>
#include <cmath>
#include <random>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>

using namespace std;



class porownajKrawedz;
class Krawedz;
typedef priority_queue<Krawedz,vector<Krawedz>,porownajKrawedz> kolejka_priorytetowa;

Krawedz **macierz = NULL;
vector< vector< Krawedz>> lista;
int LWierzcholkow = 0;
int LKrawedzi = 0;

int kruskalLista();
int kruskalMacierz();
int primLista();
int primMacierz();



kolejka_priorytetowa przepisz(kolejka_priorytetowa);
void UsunZkolejki(kolejka_priorytetowa *, int);
int znajdz(int, int *);


void UsrednijCzas(vector <long double>);
void OznaczJakoNieodwiedzony();

void LosujGrafy(double);
void zapisanieLosowychGrafow();

void wczytywanieZPliku(string);
void WyswietlPolaczenia(vector <Krawedz>);

void eksperyment();


class Krawedz {
    public:
        int wierzPocz, wierzKonc, waga;
        bool odwiedzony;
        Krawedz():waga(-1), wierzPocz(-1), wierzKonc(-1), odwiedzony(true){};
        Krawedz(int wwej, int wwyj, int w): wierzPocz(wwej), wierzKonc(wwyj), waga(w), odwiedzony(false) {};
        };

class porownajKrawedz {
    public:
        bool operator() (const Krawedz &k1, const Krawedz &k2) {
        if(k1.waga > k2.waga)
            return true;
        return false;
        }
};



void wczytywanieZPliku(string nazwa) {
fstream plik(nazwa, ios::in);
int w, wierzPocz, wierzKonc;
if(plik.good())
    {
        do
        {
            plik >> wierzPocz >> wierzKonc >> w;

            if(LWierzcholkow < wierzPocz)
                LWierzcholkow=wierzPocz;
            if(LWierzcholkow < wierzKonc)
                LWierzcholkow = wierzKonc;

            (LKrawedzi)++;
        }while(!plik.eof());
    }
plik.close();
LWierzcholkow += 1;

macierz = new Krawedz*[LWierzcholkow];
for(int i = 0; i < LWierzcholkow; i++)
macierz[i] = new Krawedz[LWierzcholkow];
lista = vector< vector< Krawedz>>(LWierzcholkow, vector<Krawedz> (0));

plik.open(nazwa);
if(plik.good())
    {
        do
        {
            plik >> wierzPocz >> wierzKonc >> w;

            Krawedz k1(wierzPocz,wierzKonc,w);
            Krawedz k2(wierzKonc,wierzPocz,w);
            macierz[wierzPocz][wierzKonc] = k1;
            macierz[wierzKonc][wierzPocz] = k2;
            lista[wierzPocz].push_back(k1);
            lista[wierzKonc].push_back(k2);
        }while(!plik.eof());
    }
plik.close();

}


void LosujGrafy(double p_stwo)
{
    macierz = new Krawedz*[LWierzcholkow];
    for(int i = 0; i < LWierzcholkow; i++)
        macierz[i] = new Krawedz[LWierzcholkow];
    lista = vector< vector< Krawedz>>(LWierzcholkow, vector<Krawedz> (0));

    default_random_engine generator;
    poisson_distribution<int> distribution(100.0);

    int* tablica_wag = new int[LWierzcholkow * LWierzcholkow];
    for(int i = 0; i < LWierzcholkow * LWierzcholkow; i++)
        tablica_wag[i] = distribution(generator);

    for(int i = 0; i <LWierzcholkow; i++)
        for(int j = i+1; j < LWierzcholkow; j++)
        {
            if(rand()%100000 < p_stwo*100000)
            {
                Krawedz k_pie(i,j,tablica_wag[i*j]);
                Krawedz k_nast(j,i,tablica_wag[i*j]);
                macierz[i][j] = k_pie;
                macierz[j][i] = k_nast;
                lista[i].push_back(k_pie);
                lista[j].push_back(k_nast);
                LKrawedzi++;
            }
        }

}



void WyswietlPolaczenia(vector <Krawedz> wyniki)
{
    for(int i = 0; i < (int) wyniki.size(); i++)
        cout << wyniki[i].wierzPocz << "    " << wyniki[i].wierzKonc << "   " << wyniki[i].waga << endl;
}


void OznaczJakoNieodwiedzony()
{
    for(int i = 0; i < LWierzcholkow; i++)
        {
            for(int j = 0; j < LWierzcholkow; j++)
            macierz[i][j].odwiedzony = false;
        }
}



kolejka_priorytetowa przepisz(kolejka_priorytetowa kol)
{
    if(!kol.empty())
        {
            kolejka_priorytetowa kolejka;
            int rozmiar = kol.size();
            for(int i = 0; i < rozmiar; i++)
            {
                Krawedz k(kol.top().wierzPocz,kol.top().wierzKonc,kol.top().waga);
                kolejka.push(k);
                kol.pop();
            }
            return kolejka;
				}
    
}



int znajdz(int x, int *poprz)
{
    if(x != poprz[x])
    		poprz[x] = znajdz(poprz[x], poprz);
    return poprz[x];
}


void UsunZkolejki(kolejka_priorytetowa *kolejka, int wierzKonc)
{
    kolejka_priorytetowa pomocnicza;
    pomocnicza = przepisz(*kolejka);
    int rozmiar = (*kolejka).size();
    while(!(*kolejka).empty())
        (*kolejka).pop();
    for(int i = 0; i < rozmiar; i++)
        {
            if(pomocnicza.top().wierzKonc == wierzKonc)
            pomocnicza.pop();
            else
            {
                (*kolejka).push(pomocnicza.top());
                pomocnicza.pop();
            }
    }
}



int primLista()
{
    int aktualny = 0, suma = 0;
    vector< vector< Krawedz>> lista_druga = lista;

    kolejka_priorytetowa kolejka;
    vector <Krawedz> wynik;
    int WymaganaLKrawedzi = LWierzcholkow - 1;
    do
        {
            for(int i = 0; i < LWierzcholkow; i++)
            {
                for(unsigned int j = 0; j < lista[i].size(); j++)
                {
                    if(lista_druga[i][j].wierzPocz == aktualny && lista_druga[i][j].odwiedzony == false)
                        kolejka.push(lista_druga[i][j]);
                    if(lista_druga[i][j].wierzKonc == aktualny)
                        lista_druga[i][j].odwiedzony = true;
                }
            }
            UsunZkolejki(&kolejka,aktualny);

            if(!kolejka.empty())
                {
                    while(kolejka.top().odwiedzony == true)
                        kolejka.pop();
                    if(kolejka.top().odwiedzony == false)
                    {
                        aktualny = kolejka.top().wierzKonc;
                        wynik.push_back(kolejka.top());
                        suma +=kolejka.top().waga;
                        kolejka.pop();
                    }
            }
            WymaganaLKrawedzi--;
        }while(WymaganaLKrawedzi > 0);


    WyswietlPolaczenia(wynik);
    cout << "suma: " << suma << endl;

    return suma;
}



int primMacierz()
{
    int aktualny = 0, suma = 0;
    Krawedz **macierzTMP = macierz;

    kolejka_priorytetowa kolejka;
    vector <Krawedz> wynik;
    int WymaganaLKrawedzi = LWierzcholkow - 1;
    do
        {
            for(int i = 0; i < LWierzcholkow; i++)
                {
                    for(int j = 0; j < LWierzcholkow; j++)
                    {
                        if(macierzTMP[i][j].wierzPocz == aktualny && macierzTMP[i][j].odwiedzony == false)
                            kolejka.push(macierzTMP[i][j]);
                        if(macierzTMP[i][j].wierzKonc == aktualny)
                            macierzTMP[i][j].odwiedzony = true;
                    }
            }
            UsunZkolejki(&kolejka,aktualny);

            if(!kolejka.empty())
            {
                while(kolejka.top().odwiedzony == true)
                    kolejka.pop();
                if(kolejka.top().odwiedzony == false)
                {
                    aktualny = kolejka.top().wierzKonc;
                    suma += kolejka.top().waga;
                    wynik.push_back(kolejka.top());
                    kolejka.pop();
                }
            }
            WymaganaLKrawedzi--;
    }while(WymaganaLKrawedzi > 0);


    WyswietlPolaczenia(wynik);
    cout << "suma: " << suma << endl;


    return suma;
}



int kruskalLista()
{
    vector< vector< Krawedz>> listaTMP = lista;
    kolejka_priorytetowa kolejka;

    for(int i = 0; i < LWierzcholkow; i++)
        {
            for(int j = 0; j < (int)listaTMP[i].size(); j++)
            if(listaTMP[i][j].waga != -1)
            kolejka.push(listaTMP[i][j]);
        }

    vector <Krawedz> wynik;
    int pin, pout, suma = 0;
    int *poprz = new int[LWierzcholkow];

    for(int i = 0; i < LWierzcholkow; i++)
        poprz[i] = i;
    while(!kolejka.empty())
    {
        Krawedz k = kolejka.top();
        pin = znajdz(k.wierzPocz, poprz);
        pout = znajdz(k.wierzKonc, poprz);
        if(pin != pout)
        {
            wynik.push_back(k);
            suma += k.waga;
            poprz[pin] = poprz[pout];
        }
        kolejka.pop();
    }

        WyswietlPolaczenia(wynik);

        cout << "suma: " << suma << endl;


    return suma;
}



int kruskalMacierz()
{
    Krawedz **macierzTMP = macierz;
    kolejka_priorytetowa kolejka;

    for(int i = 0; i < LWierzcholkow; i++)
        for(int j = i; j < LWierzcholkow; j++)
            if(macierzTMP[i][j].waga != -1)
                kolejka.push(macierzTMP[i][j]);

    vector <Krawedz> wynik;
    int pin, pout, suma = 0;
    int *poprz = new int[LWierzcholkow];

    for(int i = 0; i < LWierzcholkow; i++)
        poprz[i] = i;

    while(!kolejka.empty())
        {
            Krawedz k = kolejka.top();
            pin = znajdz(k.wierzPocz, poprz);
            pout = znajdz(k.wierzKonc, poprz);
            if(pin != pout)
            {
                wynik.push_back(k);
                suma += k.waga;
                poprz[pin] = poprz[pout];
            }
            kolejka.pop();
        }


    WyswietlPolaczenia(wynik);
    cout << "suma: " << suma << endl;


    return suma;
}




void eksperyment()
{
    vector <long double> czasPrim_lista, czasPrim_macierz, czasKruskal_lista, czasKruskal_macierz;
    std::fstream plik("wyniki.txt", std::fstream::out | std::fstream::app);

    int wierzcholki[] = {100,200,400,800};
    __int64 czestotliwosc;
    QueryPerformanceFrequency((LARGE_INTEGER*) &czestotliwosc);
    for(int i = 4; i < 5; i++)
        {
            LWierzcholkow = wierzcholki[i];

            double p[] = {3 * log((double)LWierzcholkow)/LWierzcholkow, pow((double)(LWierzcholkow),(-1/3.0)), pow((double)(LWierzcholkow),(-1/2.0)), 1};

            for(int j = 0; j < 4; j++)
            {
                if(plik.good())
                plik << "\n liczba Wierzcholkow: " << LWierzcholkow
                << " prawdopodobienstwo: " << p[j] << endl;
                cout << "\n liczba Wierzcholkow: " << LWierzcholkow
                << " prawdopodobienstwo: " << p[j] << endl;
                for(int k = 0; k < 100; k++)
                {

                    LosujGrafy(p[j]);

                    __int64 poczPrim_lista = 0, konPrim_lista = 0, poczPrim_macierz = 0, konPrim_macierz = 0;
                    OznaczJakoNieodwiedzony();

                    QueryPerformanceCounter((LARGE_INTEGER*) &poczPrim_lista);
                    primLista();
                    QueryPerformanceCounter((LARGE_INTEGER*) &konPrim_lista);

                    OznaczJakoNieodwiedzony();

                    QueryPerformanceCounter((LARGE_INTEGER*) &poczPrim_macierz);
                    primMacierz();
                    QueryPerformanceCounter((LARGE_INTEGER*) &konPrim_macierz);

                    OznaczJakoNieodwiedzony();

                    czasPrim_lista.push_back((long double)((konPrim_lista - poczPrim_lista)*1000/czestotliwosc));
                    czasPrim_macierz.push_back((long double)((konPrim_macierz - poczPrim_macierz)*1000/czestotliwosc));

                    __int64 poczKruskal_lista = 0, konKruskal_lista = 0, poczKruskal_macierz = 0, konKruskal_macierz = 0;

                    QueryPerformanceCounter((LARGE_INTEGER*) &poczKruskal_lista);
                    kruskalLista();
                    QueryPerformanceCounter((LARGE_INTEGER*) &konKruskal_lista);

                    QueryPerformanceCounter((LARGE_INTEGER*) &poczKruskal_macierz);
                    kruskalMacierz();
                    QueryPerformanceCounter((LARGE_INTEGER*) &konKruskal_macierz);

                    czasKruskal_lista.push_back((long double)((konKruskal_lista - poczKruskal_lista)*1000/czestotliwosc));
                    czasKruskal_macierz.push_back((long double)((konKruskal_macierz - poczKruskal_macierz)*1000/czestotliwosc));
                }
                cout << endl;

                plik << "Kruskal lista" << endl;
                cout << "Kruskal lista" << endl;
                UsrednijCzas(czasKruskal_lista);

                plik << "Kruskal macierz" << endl;
                cout << "Kruskal macierz" << endl;
                UsrednijCzas(czasKruskal_macierz);

                plik << "Prim lista" << endl;
                cout << "Prim lista" << endl;
                UsrednijCzas(czasPrim_lista);

                plik << "Prim macierz" << endl;
                cout << "Prim macierz" << endl;
                UsrednijCzas(czasPrim_macierz);

                czasKruskal_lista.clear();
                czasKruskal_macierz.clear();
                czasPrim_lista.clear();
                czasPrim_macierz.clear();
            }
}
plik.close();
}


void UsrednijCzas(vector <long double> wektor_czasow)
{
    long double suma_czasow = 0;

    for(unsigned int i = 0; i < wektor_czasow.size(); i++)
        {
            suma_czasow += wektor_czasow[i];
        }
    long double wynik = suma_czasow/wektor_czasow.size();
    fstream plik("wyniki.txt", fstream::out | fstream::app);
    if(plik.good())
        plik << "\nczas(ms): " << wynik << endl;
    cout << "\nczas(ms): " << wynik << endl;

    plik.close();
}

void zapisanieLosowychGrafow()
{
    string nazwa;
    float prawdopodobienstwo;
    cout << "podaj liczbe wierzcholkow oraz prawdopodobienstwo: \n";
    cin >> LWierzcholkow >> prawdopodobienstwo;
    nazwa = "zapisany_graf.txt";

    LosujGrafy(prawdopodobienstwo);

    fstream plik(nazwa, ios::out);
    if(plik.good())
        {
            for(int i = 0; i < LWierzcholkow; i++)
            {
                for(int j = i; j < LWierzcholkow; j++)
                    if(macierz[i][j].waga != -1)
                        plik << macierz[i][j].wierzPocz << "\t" << macierz[i][j].wierzKonc << "\t" << macierz[i][j].waga << endl;
            }
        }
}






int main()
{

int wybierz = 0;
string nazwa;


do {
cout <<
"1  eksperyment\n"
"2  odczyt z pliku\n"
"3  zapis do pliku\n"
"0  wyjscie\n";
cin >> wybierz;
if(wybierz == 1)
eksperyment();

if(wybierz == 2)
{
    cout << "Z jakiego pliku czytac (*.txt)\n";
    cin >> nazwa;
    wczytywanieZPliku(nazwa);
    cout << "\nPRIM LISTA\n";
    primLista();
    cout << "\nPRIM MACIERZ\n";
    primMacierz();
    cout << "\nKRUSKAL LISTA\n";
    kruskalLista();
    cout << "\nKRUSKAL MACIERZ\n";
    kruskalMacierz();

}

if(wybierz == 3)
    zapisanieLosowychGrafow();
}while( wybierz != 0);
system("PAUSE");
return 0;
}


