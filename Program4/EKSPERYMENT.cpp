#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <boost/progress.hpp>
#include <random>



using namespace std;



template <class T>
class Eksperyment{

	vector<T> tab;
	vector<T> t;

    public:
		static float czasSredniKopcowania;
		static float czasSredniBabelkowania;
		static float czasSredniIntrospektywnego;

		void Kopcowanie();
        void Babelkowanie();
        void Introspektywne(int left, int right, int depth_limit);
        void WczytajZPliku(const char* nazwa_pliku);
        void menu();

        Eksperyment(){}
        virtual ~Eksperyment(){}
        };

template <class T>
float Eksperyment<T>::czasSredniKopcowania=0;

template <class T>
float Eksperyment<T>::czasSredniBabelkowania=0;

template <class T>
float Eksperyment<T>::czasSredniIntrospektywnego=0;


template <class T>
void Eksperyment<T>::WczytajZPliku(const char* nazwa_pliku)
{
    ifstream plik;
    T elem;

	tab.clear();
	plik.open(nazwa_pliku);

	while(plik.good())
	{
		plik >> elem;
		tab.push_back(elem);
	}
	plik.close();
}


template <class T>
void Eksperyment<T>::Introspektywne(int left, int right, int depth_limit)
{

    

    int i = left;
    int j = right;

	if(depth_limit==0)
    {
        make_heap(t.begin()+left, t.begin()+right+1);
        sort_heap(t.begin()+left, t.begin()+right+1);
        return;
    }

    int x = t[(left + right) / 2];

    while(i <= j)
    {
        while(t[i] < x)
             i++;
        while(t[j] > x)
             j--;
        if(i <= j)
        {
            swap(t[i], t[j]);
            i++;
            j--;
        }
    }

	if(left < j)
		Introspektywne(left, j, depth_limit-1);

    if(right > i)
		Introspektywne(i, right, depth_limit-1);

}




template <class T>
void Eksperyment<T>::Kopcowanie()
{

		
        make_heap(t.begin(), t.end());
		sort_heap(t.begin(), t.end());
        
}



template <class T>
void Eksperyment<T>::Babelkowanie()
{
	int zamiana;

    for(int j = t.size() - 1; j > 0; j--)
  	{
    	zamiana = 1;
    	for(int i = 0; i < j; i++)
      		if(t[i] > t[i + 1])
      		{
        		swap(t[i], t[i + 1]);
        		zamiana = 0;
      		}
    	if(zamiana==1) break;
  	}
    

}



template <class T>
void Eksperyment<T>::menu()
{
    int ileElem, rodzajSort, procentSort;


        cout<<"Ile elementow ma miec tablica?"<<endl;
        cin>>ileElem;


		cout << "\n1.sortowanie tablicy z rozkladu Poissona"<<endl;
		cout << "2.sortowanie tablicy z rozkladu normalnego"<<endl;
		cout << "3.sortowanie tablicy z rozkladu jednostajnego"<<endl;
        cin>>rodzajSort;

        cout << "\n1.sortowanie tablicy nieuporzadkowanej"<<endl;
		cout << "2.sortowanie tablicy posortowanej w 50%"<<endl;
		cout << "3.sortowanie tablicy posortowanej w 90%"<<endl;
		cout << "4.sortowanie tablicy posortowanej w 99%"<<endl;
        cin>>procentSort;

		switch(rodzajSort)
		{
		    case 1:
                switch(ileElem)
                {
                    case 100:
                        WczytajZPliku("poisson100.txt");
                        break;
                    case 1000:
                        WczytajZPliku("poisson1000.txt");
                        break;
                    case 10000:
                        WczytajZPliku("poisson10000.txt");
                        break;
                    case 100000:
                        WczytajZPliku("poisson100000.txt");
                        break;
                    case 1000000:
                        WczytajZPliku("poisson1000000.txt");
                        break;

                }
             case 2:
                    switch(ileElem)
                    {
                    case 100:
                        WczytajZPliku("norma100.txt");
                        break;
                    case 1000:
                        WczytajZPliku("norma1000.txt");
                        break;
                    case 10000:
                        WczytajZPliku("norma10000.txt");
                        break;
                    case 100000:
                        WczytajZPliku("norma100000.txt");
                        break;
                    case 1000000:
                        WczytajZPliku("norma1000000.txt");
                        break;
                    }

                case 3:
                    switch(ileElem)
                    {

                    case 100:
                        WczytajZPliku("jedno100.txt");
                        break;
                    case 1000:
                        WczytajZPliku("jedno1000.txt");
                        break;
                    case 10000:
                        WczytajZPliku("jedno10000.txt");
                        break;
                    case 100000:
                        WczytajZPliku("jedno100000.txt");
                        break;
                    case 1000000:
                        WczytajZPliku("jedno1000000.txt");
                        break;
                    }

		}


        switch(procentSort)
        {
            case 1:
                break;
            case 2:
                sort(tab.begin(), tab.end()-(tab.size()/2));
                break;
            case 3:
                sort(tab.begin(), tab.end()-(tab.size()/10));
                break;
            case 4:
                sort(tab.begin(), tab.end()-(tab.size()/100));
                break;

        }



		for(int licznik=0; licznik<10; licznik++)
		{
			t=tab;
			
			boost::timer stoper1;
        	Kopcowanie();
			czasSredniKopcowania += stoper1.elapsed();
			t.clear();
			t=tab;

			boost::timer stoper2;
			Babelkowanie();
			czasSredniBabelkowania += stoper2.elapsed();
			t.clear();
			t=tab;
			
			boost::timer stoper3;
			Introspektywne(0, t.size()-1, 2*log(t.size())/M_LN2);
			czasSredniIntrospektywnego += stoper3.elapsed();
			t.clear();

		}

		cout<<"Czas kopcowania: "<<czasSredniKopcowania/10<<endl;
		cout<<"Czas babelkowania: "<<czasSredniBabelkowania/10<<endl;
		cout<<"Czas sortowania introspektywnego: "<<czasSredniIntrospektywnego/10<<endl;

}

int main()
{
    Eksperyment<int> ek;

    ek.menu();

    return 0;
}

