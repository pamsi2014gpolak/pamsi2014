#include <iostream>
#include <queue>

using namespace std;

template <class T>
class Kolejka {

    queue<T> kol;

    static int rozmiar;

    public:

        void DodajElement(T item);
        T PobierzElement();
        void WyswietlRozmiar();
        void menu();

};

template <class T>
int Kolejka<T>::rozmiar=0;

template <class T>
void Kolejka<T>::DodajElement(T item)
{
    kol.push(item);
    rozmiar++;
}


template <class T>
T Kolejka<T>::PobierzElement()
{
    T zm;
    if(!kol.empty())
    {

        zm=kol.front();
        kol.pop();
        rozmiar--;
        cout<<zm<<endl;
    }else{
        cout<<"Kolejka jest pusta"<<endl;}

    return zm;

}




template <class T>
void Kolejka<T>::WyswietlRozmiar()
{
    if(!kol.empty())
        cout<<"Dlugosc kolejki: "<<rozmiar<<endl;
    else
        cout<<"Kolejka jest pusta\n";
}


template <class T>
void Kolejka<T>::menu()
{
	int opcja;


	do{
		cout << "\n1.Dodaj element"<<endl;
		cout << "2.Pobierz element"<<endl;
        cout << "3.Wyswietl dlugosc kolejki"<<endl;
		cout << "0.Koniec"<<endl;

		cout << "Wybierz opcje\n";
		cin >> opcja;

		switch(opcja)
		{
			case 0: break;
			case 1:
                T n;
                cout << "\nWpisz element, ktory chcesz dodac:"<<endl;
                cin >> n;
                DodajElement(n);
                break;
			case 2:
				PobierzElement();
				break;
			case 3:
                WyswietlRozmiar();
                break;
		}

	}while(opcja !=0);

}

int main()
{
   Kolejka<int> k;

   k.menu();

   return 0;
}
