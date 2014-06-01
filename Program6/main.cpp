#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<list>
#include<functional>
#include<Windows.h>
#include<ctime>
#include<algorithm>

#define INF 100000
using namespace std;


class wierzcholek
{
	int nr;
	int odl;
	friend bool operator > (wierzcholek const x, wierzcholek const y){return x.odl > y.odl;};
	friend bool operator < (wierzcholek const x, wierzcholek const y){return x.odl < y.odl;};
};

class Krawedz
{
public:
	int poczatek;
	int koniec;
	int waga;
	Krawedz(){Krawedz(0, 0, 0);};
	Krawedz(int pocz, int kon, int wg){poczatek=pocz, koniec=kon, waga=wg;};
	friend bool operator > (Krawedz const x, Krawedz const y){return x.waga > y.waga;};
	friend bool operator < (Krawedz const x, Krawedz const y){return x.waga < y.waga;};
};

class Sasiad
{
public:
	int nr_wierzch;
	int waga;
	Sasiad(){Sasiad(0, 0);};
	Sasiad(int nr, int wg){nr_wierzch=nr; waga=wg;};
};

class graf
{
public:
	int V, E, S; //ilosc wierzcholkow, Krawedzi i wierzcholek startowy
	//reprezentacje
	int **macierz_sasiedztwa;
	vector<Sasiad> *lista_sasiedztwa;
	//inne
	void wypelnij_lista_Krawedzi(vector<Krawedz> lista_Krawedzi);
	void wypelnij_lista_Sasiadow();
	void wypelnij_macierz_sasiedztwa();
	//konstruktory, destruktory, inicjalizacja, generowanie
	void wyczysc();
	void inicjalizuj(int ilosc_wierzcholkow, int ilosc_Krawedzi, int wierzcholek_startowy);
	void wygeneruj_Krawedzie();
	graf(){V=0; E=0; inicjalizuj(0, 0, 0);}; //tworzy pusty graf
	graf(int ilosc_wierzcholkow, int ilosc_Krawedzi){V=0; E=0; inicjalizuj(ilosc_wierzcholkow, ilosc_Krawedzi, 0);};
	~graf(){wyczysc();};
	//znajdowanie najkrotszej drogi
	void dijkstra(bool lista);
	void bellman_ford(bool lista);
	int *poprzednik; //poprzedni element na drodze od startu
	int *odleglosc; //tablica dlugosci najkrotszych mozliwych drog do danego wierzcholka
	void wyswietl_droge();
	//wyswietlenie reprezentacji
	void wyswietl_macierz_sasiedztwa();
	void wyswietl_liste_sasiedztwa();
	void wyswietl_liste_Krawedzi();
	//odczyt i zapis pliku
	bool wczytaj_liste_Krawedzi(string nazwa_pliku);
	bool zapisz_liste_Krawedzi(string nazwa_pliku);
	bool zapisz_macierz_sasiedztwa(string nazwa_pliku);
	bool zapisz_liste_sasiedztwa(string nazwa_pliku);
};

//-----------------------------------------------------------------

bool srt(const Krawedz& x, const Krawedz& y)
{
	return x.waga>y.waga;
}

void sortuj_wektor(vector<Krawedz> *wektor)
{
	sort(wektor->begin(), wektor->end(), srt);
}
//-----------------------------------------------------------------

void graf::wypelnij_lista_Krawedzi(vector<Krawedz> lista_Krawedzi)
{
	Krawedz nowa;
	//wyczyszczenie starych reprezentacji
	for(int i=0; i<V; i++)
	{
		if(!lista_sasiedztwa[i].empty())
			lista_sasiedztwa[i].clear();
		for(int j=0; j<V; j++)
			macierz_sasiedztwa[i][j]=INF;
	}

	//stworzenie nowych reprezentacji
	for(unsigned int i=0; i<lista_Krawedzi.size(); i++)
	{

		nowa=lista_Krawedzi[i];

		Sasiad s1(nowa.koniec, nowa.waga);

		lista_sasiedztwa[nowa.poczatek].push_back(s1);
		macierz_sasiedztwa[nowa.poczatek][nowa.koniec]=nowa.waga;
	}
}

void graf::wypelnij_lista_Sasiadow()
{
	Krawedz nowa;
	//wyczyszczenie starej macierzy sasiedztwa
	for(int i=0; i<V; i++)
		for(int j=0; j<V; j++)
			macierz_sasiedztwa[i][j]=INF;


	for(int i=0; i<V; i++)
	{
		nowa.poczatek=i;
		for(unsigned int j=0; j<lista_sasiedztwa[i].size(); j++)
				macierz_sasiedztwa[i][lista_sasiedztwa[i][j].nr_wierzch]=lista_sasiedztwa[i][j].waga;
	}
}

void graf::wypelnij_macierz_sasiedztwa()
{
	Krawedz nowa;
	//wyczyszczenie starej listy sasiedztwa
	for(int i=0; i<V; i++)
		if(!lista_sasiedztwa[i].empty())
			lista_sasiedztwa[i].clear();

	//stworzenie nowych reprezentacji
	for(int i=0; i<V; i++)
	{
		for(int j=0; j<V; j++)
		{
			if(macierz_sasiedztwa[i][j]!=INF && i!=j)
			{
				Sasiad s(j, macierz_sasiedztwa[i][j]);
				lista_sasiedztwa[i].push_back(s);
			}
		}
	}
}

void graf::wyczysc()
{
	if(V>0)
	{
		for(int i=0; i<V; i++)
		{
			delete [] macierz_sasiedztwa[i]; //usuniecie macierzy sasiedztwa
			if (!lista_sasiedztwa[i].empty()) lista_sasiedztwa[i].clear(); //usuniecie listy sasiedztwa
		}
		delete [] lista_sasiedztwa;
		delete [] macierz_sasiedztwa;
		delete [] odleglosc;
		delete [] poprzednik;
	}

	V=E=S=0;
}

void graf::inicjalizuj(int ilosc_wierzcholkow, int ilosc_Krawedzi, int wierzcholek_startowy)
{
	if(V>0) wyczysc();

	V=ilosc_wierzcholkow;
	E=ilosc_Krawedzi;
	S=wierzcholek_startowy;

	//stworzenie pustej macierzy sasiedztwa oraz tablicy odleglosci i poprzednikow
	macierz_sasiedztwa=new int * [V]; //pierwszy wymiar
	odleglosc=new int [V];
	poprzednik=new int[V];
	for(int i=0; i<V; i++ )
	{
		//wszystkie wierzcholki ze startem laczy Krawedz o nieskonczonej dlugosci
		odleglosc[i]=INF;
		poprzednik[i]=S;
		macierz_sasiedztwa[i] = new int [V]; //dodanie drugiego wymiaru
		for(int j=0; j<V; j++)
			macierz_sasiedztwa[i][j]=INF; //ustawienie nieskonczonosci, czyli braku Krawedzi
	}

	//stworzenie pustej listy sasiedztwa
	lista_sasiedztwa=new vector<Sasiad>[V]; //dynamiczna tablica wektorow (pustych)
}

void graf::wygeneruj_Krawedzie()
{
	if(V==0 || E==0) return;
	//stworzenie listy mozliwych Krawedzi
	Krawedz nowa;
	vector<Krawedz> mozliwe;
	nowa.waga=INF; //na razie wszystkie maja nieskonczonosc
	for(int i=0; i<V; i++)
	{
		nowa.poczatek=i;
		for(int j=0; j<V; j++)
		{
			if(macierz_sasiedztwa[i][j]==INF && i!=j)
			{
				nowa.koniec=j;
				mozliwe.push_back(nowa);
			}
		}
	}
	//branie losowej Krawedzi z listy mozliwych, przypisanie jej wagi i dodanie do grafu
	vector<Krawedz> lista_Krawedzi;
	for(int wygenerowane_Krawedzie=0; wygenerowane_Krawedzie<E; wygenerowane_Krawedzie++)
	{
		if(mozliwe.size()>4)
            swap(mozliwe.back(), mozliwe[rand() % (mozliwe.size()-2)]);
		mozliwe.back().waga=1+(rand() % 1000);
		lista_Krawedzi.push_back(mozliwe.back());
		mozliwe.pop_back();
	}
	wypelnij_lista_Krawedzi(lista_Krawedzi);
}







//znajdowanie najkrotszej drogi
void graf::dijkstra(bool lista)
{
	//wyzerowanie tablicy odleglosci i poprzednikow
	for(int i=0; i<V; i++)
	{
		odleglosc[i]=INF;
		poprzednik[i]=S;
	}

	//tablica zrelaksowanych wierzcholkow
	bool *tab_relaks = new bool [V];
	for(int i=0; i<V; i++)
        tab_relaks[i]=false;

	int relaksowany=0;
	odleglosc[S]=0;

	for(int j=0; j<V; j++)
	{
		//znalezienie niezrelaksowanego wierzcholka x o najmniejszym odleglosc[x]
		int min_odl=INF;
		for(int i=0; i<V; i++)
		{
			if(odleglosc[i]<min_odl && tab_relaks[i]==false)
			{
				relaksowany=i;
				min_odl=odleglosc[i];
			}
		}
		//zrelaksowanie go
		tab_relaks[relaksowany]=true;
		//sprawdzenie drog wychodzacych ze znalezionego wierzcholka
		if(lista) //z listy
		{
			int Sasiad, waga;
			for(unsigned int i=0; i<lista_sasiedztwa[relaksowany].size(); i++)
			{
				//sprawdzenie, czy nie znalezlismy krotszej drogi
				Sasiad=lista_sasiedztwa[relaksowany][i].nr_wierzch;
				waga=lista_sasiedztwa[relaksowany][i].waga;
				if(waga<0)
					cout << "Ujemna waga Krawedzi!" << endl;
				if((odleglosc[relaksowany]+waga) < odleglosc[Sasiad])
				{
					//jesli tak, to podmienienie jej
					odleglosc[Sasiad]=(odleglosc[relaksowany]+waga);
					poprzednik[Sasiad]=relaksowany;
				}
			}
		}
		else
		{
			for(int i=0; i<V; i++)
			{

				if(relaksowany!=i && macierz_sasiedztwa[relaksowany][i] < INF)

					if(macierz_sasiedztwa[relaksowany][i]<0)
						cout << "Ujemna waga Krawedzi!" << endl;
					if(odleglosc[relaksowany]+macierz_sasiedztwa[relaksowany][i] < odleglosc[i])
					{

						odleglosc[i]=(odleglosc[relaksowany]+macierz_sasiedztwa[relaksowany][i]);
						poprzednik[i]=relaksowany;
					}
			}
		}
	}
}

void graf::bellman_ford(bool lista)
{

	for(int i=0; i<V; i++)
	{
		odleglosc[i]=INF;
		poprzednik[i]=S;
	}

	enum obecnosc_w_kolejce {NIE_BYLO, JEST, BYL};
	list<int> kolejka_wierzcholkow;
	int badany;

	int *tab_obecnosci=new int[V];
	for(int i=0; i<V; i++)
		tab_obecnosci[i]=NIE_BYLO;

	kolejka_wierzcholkow.push_front(S);
	tab_obecnosci[S]=JEST;
	odleglosc[S]=0;
	while(!kolejka_wierzcholkow.empty())
	{

		badany=kolejka_wierzcholkow.front();
		kolejka_wierzcholkow.pop_front();

		if(lista)
		{
			int Sasiad, waga;
			for(unsigned int i=0; i<lista_sasiedztwa[badany].size(); i++)
			{

				Sasiad=lista_sasiedztwa[badany][i].nr_wierzch;
				waga=lista_sasiedztwa[badany][i].waga;
				if((odleglosc[badany]+waga) < odleglosc[Sasiad])
				{

					odleglosc[Sasiad]=(odleglosc[badany]+waga);
					poprzednik[Sasiad]=badany;

					if(tab_obecnosci[Sasiad]==NIE_BYLO)
                        kolejka_wierzcholkow.push_back(Sasiad);

					else if(tab_obecnosci[Sasiad]==BYL)
                        kolejka_wierzcholkow.push_front(Sasiad);

				}
			}
		}
		else
		{
			for(int i=0; i<V; i++)
			{

				if(badany!=i && macierz_sasiedztwa[badany][i] < INF)

					if(odleglosc[badany]+macierz_sasiedztwa[badany][i] < odleglosc[i])
					{
						//jesli tak, to podmienienie jej
						odleglosc[i]=(odleglosc[badany]+macierz_sasiedztwa[badany][i]);
						poprzednik[i]=badany;
						//sprawdzenie obecnosci w kolejce wierzcholkow
						//jesli nie bylo go jeszcze, to wrzucenie go na koniec
						if(tab_obecnosci[i]==NIE_BYLO) kolejka_wierzcholkow.push_back(i);
						//jesli juz byl, ale go nie ma, to wrzucenie go na poczatek
						else if(tab_obecnosci[i]==BYL) kolejka_wierzcholkow.push_front(i);
						//jesli juz jest w kolejce, to nie robimy nic
					}
			}
		}
	}
}

void graf::wyswietl_droge()
{
	if(V>0)
	{
		cout << "Wierzcholek: ";
		for(int i=0; i<V; i++)
			cout << i << " ";
		cout << endl;
		cout << "Odleglosc:   ";
		for(int i=0; i<V; i++)
		{
			if (odleglosc[i]>=INF)
                cout << "- ";
			else
                cout << odleglosc[i] << " ";
		}
		cout << endl;
		cout << "Poprzedni:  ";//poprzedni wierzcholek w drodze do biezacego
		for(int i=0; i<V; i++)
		{
			if (odleglosc[i]>=INF || i==S)
                cout << "- ";
			else
                cout << poprzednik[i] << " ";
		}
		cout << endl << endl;
	}
}

//wyswietlenie reprezentacji
void graf::wyswietl_macierz_sasiedztwa()
{
	cout << "Macierz sasiedztwa:" << endl;
	cout << " |";
	for(int i=0; i<V; i++)
		cout << i << "|";
	cout << endl;
	for(int i=0; i<V; i++)
	{
		cout << i << "|";
		for(int j=0; j<V; j++)
		{
			if(i==j)
                cout << "- ";
			else if(macierz_sasiedztwa[i][j]>=INF)
                cout << "* ";
			else
                cout << macierz_sasiedztwa[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void graf::wyswietl_liste_sasiedztwa()
{
	cout << "Lista sasiedztwa (numer Sasiada, waga Krawedzi):" << endl;
	for(int i=0; i<V; i++) //kolejne wierzcholki
	{
		cout << "Wierzcholek nr " << i << ": ";
		for(unsigned int j=0; j<lista_sasiedztwa[i].size(); j++)
		{
			cout << "(" << lista_sasiedztwa[i][j].nr_wierzch << ", " << lista_sasiedztwa[i][j].waga << ") ";
		}
		cout << endl;
	}
	cout << endl;
}

void graf::wyswietl_liste_Krawedzi()
{
	vector<Krawedz> lista_Krawedzi;
	Krawedz nowa;
	//z listy Sasiadow
	for(int i=0; i<V; i++) //kolejne wierzcholki
	{
		nowa.poczatek=i;
		for(unsigned int j=0; j<lista_sasiedztwa[i].size(); j++) //kolejni sasiedzi danego wierzcholka
		{
				nowa.koniec=lista_sasiedztwa[i][j].nr_wierzch;
				nowa.waga=lista_sasiedztwa[i][j].waga;
				lista_Krawedzi.push_back(nowa);
		}
	}

	cout << "Lista Krawedzi:" << endl;
	for(int i=0; i<E; i++)
	{
			cout << lista_Krawedzi[i].poczatek << " " << lista_Krawedzi[i].koniec << " " << lista_Krawedzi[i].waga << endl;
	}
	cout << endl;
}


bool graf::wczytaj_liste_Krawedzi(string nazwa_pliku)
{
	fstream plik;
	vector<Krawedz> lista_Krawedzi;
	std::cout << "Nazwa pliku: " << nazwa_pliku.c_str() << "\n";
    plik.open(nazwa_pliku.c_str(), fstream::in);
	if(!plik.good())
	{
		cout << "Blad pliku!" << endl;
		return false;
	}
	else
	{
		cout << "Udane otwarcie pliku!" << endl;
		wyczysc();
		std::cout << "Wyczyscil\n";
		Krawedz nowa;
		int ilosc_wierzcholkow;
		int ilosc_Krawedzi;
		int wierzcholek_startowy;
		plik >> ilosc_Krawedzi;
		plik >> ilosc_wierzcholkow;
		plik >> wierzcholek_startowy;
		std::cout << "Wczytal dane: " << ilosc_Krawedzi << "|" << ilosc_wierzcholkow << "|" << wierzcholek_startowy << "\n";
		while(plik >> nowa.poczatek && plik >> nowa.koniec && plik >> nowa.waga)
		{
			if(nowa.waga!=0 )
				lista_Krawedzi.push_back(nowa); //umieszczenie Krawedzi na liscie
		}
		plik.close();
		inicjalizuj(ilosc_wierzcholkow, ilosc_Krawedzi, wierzcholek_startowy);
		wypelnij_lista_Krawedzi(lista_Krawedzi);
		cout << "Stworzono graf!" << endl;
		return true;
	}
}

bool graf::zapisz_liste_Krawedzi(string nazwa_pliku)
{
	fstream plik;
    plik.open(nazwa_pliku.c_str(), ios::out);
	vector<Krawedz> lista_Krawedzi;
	Krawedz nowa;
	//z listy sasiedztwa
	for(int i=0; i<V; i++) //kolejne wierzcholki
	{
		nowa.poczatek=i;
		for(unsigned int j=0; j<lista_sasiedztwa[i].size(); j++) //kolejni sasiedzi danego wierzcholka
		{
				nowa.koniec=lista_sasiedztwa[i][j].nr_wierzch;
				nowa.waga=lista_sasiedztwa[i][j].waga;
				lista_Krawedzi.push_back(nowa);
		}
	}
	if(plik.good() && lista_Krawedzi.size()!=0)
	{
		plik << E << " " << V << " " << S << endl;
		for(int i=0; i<E; i++)
		{
			plik << lista_Krawedzi[i].poczatek << " " << lista_Krawedzi[i].koniec << " " << lista_Krawedzi[i].waga << endl;
		}
		plik.close();
		return true;
	}
	else return false;
}

bool graf::zapisz_macierz_sasiedztwa(string nazwa_pliku)
{
	fstream plik;
    plik.open(nazwa_pliku.c_str(), ios::out);
	if(plik.good() && V>0)
	{
		for(int i=0; i<V; i++)
		{
			for(int j=0; j<V; j++)
				plik << macierz_sasiedztwa[i][j] << " ";
			plik << endl;
		}
		plik.close();
		cout << "Zapisano macierz sasiedztwa w pliku " << nazwa_pliku << endl;
		return true;
	}
	else return false;
}

bool graf::zapisz_liste_sasiedztwa(string nazwa_pliku)
{
	fstream plik;
    plik.open(nazwa_pliku.c_str(), ios::out);
	if(plik.good() && E>0)
	{
		for(int i=0; i<V; i++) //kolejne wierzcholki
		{
			plik << "Wierzcholek nr " << i << ": ";
			for(unsigned int j=0; j<lista_sasiedztwa[i].size(); j++)
			{
				plik << "(" << lista_sasiedztwa[i][j].nr_wierzch << ", " << lista_sasiedztwa[i][j].waga << ") ";
			}
			plik << endl;
		}
		plik.close();
		cout << "Zapisano liste sasiedztwa w pliku " << nazwa_pliku << endl;
		return true;
	}
	else return false;
}

void eksperyment()
{
	srand((int)time(NULL));
	graf g;
	fstream plik;
	plik.open("pomiary.txt", ios::out);
	int rozmiary[5] = {50,100,200,400,800};
	int gestosci[4] = {25, 50, 75, 100};//poki co inty, potem dziele przez 100(zeby uzyskac p-stwo
	int v, d;
	int s=0;
	int ilosc_obiegow=100;

	LARGE_INTEGER freq, start, stop, czas_generowania;
	LARGE_INTEGER czas_dijkstra_lista, czas_dijkstra_macierz, czas_bellman_ford_lista, czas_bellman_ford_macierz;
	QueryPerformanceFrequency(&freq);

	czas_dijkstra_lista.QuadPart=0; czas_dijkstra_macierz.QuadPart=0;
	czas_bellman_ford_lista.QuadPart=0;	czas_bellman_ford_macierz.QuadPart=0;
	czas_generowania.QuadPart=0;


	for(int i=0; i<5; i++)
	{
		v=rozmiary[i];
		cout << "Rozmiar: " << v << endl;
		for(int j=0; j<4; j++)
		{
			d=gestosci[j];
			cout << "	Gestosc: " << d << endl;
			plik << v << " " << d << endl;
			for(int obieg=0; obieg<ilosc_obiegow; obieg++)
			{
				QueryPerformanceCounter(&start);
				g.inicjalizuj(v, (int)((double)d/100)*v*(v-1), s);
				g.wygeneruj_Krawedzie();
				QueryPerformanceCounter(&stop);
				czas_generowania.QuadPart=czas_generowania.QuadPart+(stop.QuadPart-start.QuadPart);

				QueryPerformanceCounter(&start);
				g.dijkstra(true);
				QueryPerformanceCounter(&stop);
				czas_dijkstra_lista.QuadPart=czas_dijkstra_lista.QuadPart+(stop.QuadPart-start.QuadPart);

				QueryPerformanceCounter(&start);
				g.dijkstra(false);
				QueryPerformanceCounter(&stop);
				czas_dijkstra_macierz.QuadPart=czas_dijkstra_macierz.QuadPart+(stop.QuadPart-start.QuadPart);

				QueryPerformanceCounter(&start);
				g.bellman_ford(true);
				QueryPerformanceCounter(&stop);
				czas_bellman_ford_lista.QuadPart=czas_bellman_ford_lista.QuadPart+(stop.QuadPart-start.QuadPart);

				QueryPerformanceCounter(&start);
				g.bellman_ford(false);
				QueryPerformanceCounter(&stop);
				czas_bellman_ford_macierz.QuadPart=czas_bellman_ford_macierz.QuadPart+(stop.QuadPart-start.QuadPart);
			}
			plik << "	" << ((double)1000/ilosc_obiegow)*(double)czas_dijkstra_lista.QuadPart/(double)freq.QuadPart << endl;
			plik << "	" << ((double)1000/ilosc_obiegow)*(double)czas_dijkstra_macierz.QuadPart/(double)freq.QuadPart << endl;
			plik << "	" << ((double)1000/ilosc_obiegow)*(double)czas_bellman_ford_lista.QuadPart/(double)freq.QuadPart << endl;
			plik << "	" << ((double)1000/ilosc_obiegow)*(double)czas_bellman_ford_macierz.QuadPart/(double)freq.QuadPart << endl;
		}
	}

	plik.close();
}

int main()
{
	graf g;
	char opcja='x';
	string nazwa_pliku;
	srand((int)time(NULL));

	while(opcja!='q')
	{
		cout << "Menu:" << endl;
		cout << "1 - eksperyment" << endl;
		cout << "2 - wczytaj liste Krawedzi z pliku i znajdz najkrotsze sciezki" << endl;
		cout << "q - wyjscie" << endl;
		cout << "Wybrana opcja: ";
		cin >> opcja;

		switch(opcja)
		{
		case '1':
			eksperyment();
			break;
		case '2':
			cout << "Podaj nazwe pliku: ";
			cin >> nazwa_pliku;
			g.wczytaj_liste_Krawedzi(nazwa_pliku);
			g.wyswietl_liste_Krawedzi();
			g.wyswietl_liste_sasiedztwa();
			g.wyswietl_macierz_sasiedztwa();

			g.dijkstra(true);
			cout << "Dijkstra - lista" << endl;
			g.wyswietl_droge();

			g.dijkstra(false);
			cout << "Dijkstra - macierz" << endl;
			g.wyswietl_droge();

			g.bellman_ford(true);
			cout << "Bellman-Ford - lista" << endl;
			g.wyswietl_droge();

			g.bellman_ford(false);
			cout << "Bellman-Ford - macierz" << endl;
			g.wyswietl_droge();
			break;
		case 'q':
			break;
		default:
			break;
		}
	}

	system("pause");
	return 0;
}
