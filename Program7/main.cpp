#include <fstream>
#include <iostream>
#include <list>
#include <time.h>
#include <string>

using namespace std;

#define MAX 2147483647

//uwzgledniono prezentacje grafu za pomoca macierzy sasiedztwa


class Graf {
    public:
        int** graf;
        int** przeplywy;
        int v;
        int e;
        int s;
        int t;
        int fmax;

        int EdmondsKarp();
        void wypisz();


Graf(int wierzcholki, int krawedzie, int start, int koniec)
{
    v=wierzcholki;
    e=krawedzie;
    s=start;
    t=koniec;
    graf=new int*[v];
    przeplywy=new int*[v];
    fmax=0;

for(int i=0; i<v; i++)
    {
        graf[i]=new int[v];
        przeplywy[i]=new int[v];
    }

}



Graf(string nazwa)
{

int a,b,c;
fstream plik;
fmax=0;

plik.open(nazwa.c_str(), ios::in | ios::out);
if(plik.good()==true)
    {

        plik >> v;
        plik >> e;
        plik >> s;
        plik >> t;

        graf=new int*[v];
        przeplywy=new int*[v];

        for(int i=0; i<v; i++)
            {
                graf[i]=new int[v];
                przeplywy[i]=new int[v];

                for(int j=0; j<v; j++)
                    {
                        graf[i][j]=0;
                        przeplywy[i][j]=0;
                    }
            }

            while(!plik.eof())
                {
                    plik >> a;
                    plik >> b;
                    plik >> c;
                    graf[a][b]=c;
                }
                plik.close();
    }else
        cout << "Nie otworzono pliku" << endl;


}

};



int Graf::EdmondsKarp()
{

    int* poprz=new int[v];
    int* przepustowosc=new int[v];
    list<int> q;
    bool flaga;
    int x, y, cp;


    do{
        for(int i=0; i<v; i++)
            poprz[i]=-1;

        poprz[s]=-2;

        przepustowosc[s]=MAX;

        q.clear();
        q.push_back(s);

        flaga=false;

        while(!q.empty())
        {

            x=q.front();
            q.pop_front();

            for(y=0; y<v; y++)
            {
                cp=graf[x][y]-przeplywy[x][y];

                if(cp && poprz[y]==-1)
                {
                    poprz[y]=x;
                    if(przepustowosc[x] > cp)
                        przepustowosc[y]=cp;
                    else
                        przepustowosc[y]=przepustowosc[x];


                    if(y==t)
                    {
                        fmax+=przepustowosc[t];
                        while(y!=s)
                        {
                            x=poprz[y];
                            przeplywy[x][y]+=przepustowosc[t];
                            y=x;
                        }

                        flaga=true;
                        break;
                    }
                    q.push_back(y);
                }

            }
            if(flaga)break;
        }

        if(!flaga)
            break;
    }while(true);

    return fmax;
}





void Graf::wypisz()
{
    for(int i=0; i<v; i++)
        for(int j=0; j<v; j++)
            if(graf[i][j])
                cout << i << " " << "->" << j << " " << przeplywy[i][j] << "/" << graf[i][j] << endl;
    cout << endl;
}



int main()
{
    int czy_wyswietlic_sieci,ile_testow;
    double czas, czas_w_sumie=0;
    char wybierz_opcje;
    string nazwa;

    cout << "Wybierz opcje:" << endl;
    cout << "w - Wczytanie Grafu z pliku" << endl;
    cout << "t - Rozpoczecie testow."<< endl;
    cout << "q - Koniec"<< endl;
    cin >> wybierz_opcje;

    while(wybierz_opcje != 'q')
        {
            switch(wybierz_opcje)
            {

            case 'w':
                cout << "Podaj nazwe pliku: ";
                cin >> nazwa;
                cout << "Plik zostal wczytany, mozliwe opcje: " << endl;
                cout << "w - Wczytaj graf z pliku." << endl;
                cout << "e - Eksperyment."<< endl;
                cout << "q - Koniec."<< endl;
                cin >> wybierz_opcje;
            break;

            case 'e':
            cout << "Podaj ilosc testow." << endl;
            cin >> ile_testow;
            cout << "Wyswietlac uzyskane sieci przeplywu?" << endl << "1 - TAK" << endl << "0 - NIE" << endl;
            cin >> czy_wyswietlic_sieci;
            for(int i=0; i<ile_testow; i++)
                {
                    Graf g(nazwa);
                    czas=0;
                    czas=clock();
                    g.EdmondsKarp();
                    czas=clock()-czas;
                    czas_w_sumie+=czas;
                    cout << "Maksymalny przeplyw: " << g.fmax << endl;
                    if(czy_wyswietlic_sieci)
                        g.wypisz();
                }

            cout << "Czas dla algorytmu Edmondsa-Karpa wynosi: " << 1000*czas_w_sumie/CLOCKS_PER_SEC/ile_testow << " ms." << endl;

            cout << "Testy zostaly zakonczone, co dalej? " << endl;
            cout << "w - Wczytaj graf z pliku." << endl;
            cout << "e - Eksperyment."<< endl;
            cout << "q - Koniec."<< endl;
            cin >> wybierz_opcje;
            break;

            default:
                cout << "Niedozwolony znak" << endl;
                cout << "w - Wczytaj graf z pliku." << endl;
                cout << "e - Eksperyment."<< endl;
                cout << "q - Koniec."<< endl;
                cin >> wybierz_opcje;
            }
        }

    return 0;
}
