#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>



using namespace std;

template <class T>
class Kopiec{

    vector<T> tab;

    public:

        void StworzTablice();
        void PrzywrocKopiec();
        void UsunSzczyt();
        void WyswietlKopiec();
        void menu();


        Kopiec(){tab={};}
        virtual ~Kopiec(){}
    };


template <class T>
void Kopiec<T>::StworzTablice()
{
    int rozmiar;
    int zakres;
    cout<<"Podaj liczbe elementow: \n";
    cin>>rozmiar;
    cout<<"Podaj maksymalna mozliwa wylosowana wartosc: \n";
    cin>>zakres;

    for(int i=1; i<rozmiar; i++)
    {
        tab.insert(tab.end(), ( rand() % zakres ) + 1);
    }
}

template <class T>
void Kopiec<T>::UsunSzczyt()
{
    pop_heap(tab.begin(), tab.end());
}

template <class T>
void Kopiec<T>::WyswietlKopiec()
{
    cout<<"\n";
    for(unsigned int i=0; i<tab.size(); i++)
        cout<<tab[i]<<endl;


}

template <class T>
void Kopiec<T>::PrzywrocKopiec()
{
    if(is_heap(tab.begin(), tab.end()) == true)
        cout<<"Kopiec jest dobry\n";
    else
        make_heap(tab.begin(), tab.end());
}

template <class T>
void Kopiec<T>::menu()
{
	int opcja;

    do{
		cout << "\n1.Utworz tablice"<<endl;
		cout << "2.Przywroc wlasnosci kopca"<<endl;
        cout << "3.Usun elementu ze szczytu"<<endl;
		cout << "4.Wyswietl kopiec"<<endl;
		cout << "0.Koniec"<<endl;

		cout << "Wybierz opcje\n";
		cin >> opcja;

		switch(opcja)
		{
			case 0: break;
			case 1:
                StworzTablice();
                break;
			case 2:
				PrzywrocKopiec();
				break;
			case 3:
                UsunSzczyt();
                break;
			case 4:
				cout<<"\nZawartosc kopca: "<<endl;
				WyswietlKopiec();
				break;
		}

	}while(opcja !=0);

}

int main()
{
    srand(time(NULL));

    Kopiec<int> k;

    k.menu();

    return 0;
}

