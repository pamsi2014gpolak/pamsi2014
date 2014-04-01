#include <iostream>

using namespace std;


template <class T>
class Lista {

        T dana;		//zmienna okreslonego typu przechowujaca dane
        Lista *next;
        Lista *prev;	//zmienne wskaznikowe pokazujace na poprzedni lub nastepny eleemnt

        Lista *obecny;
        Lista *head;	//zmienne wskaznikowe pokazujace na pierwszy, ostatni, badz aktualny element
        Lista *tail;

	public:

		void DodajElementNaPoczatek(T item);
        void DodajElementNaKoniec(T item);
        void UsunPierwszyElement();
        void UsunOstatniElement();			//metody pozwalajace na podstawowe operacje na stosie
        void WyswietlListe();
        void WyswietlListeOdKonca();
		void menu();


		Lista(){prev=0; next=0; obecny=0;}	//konstruktor bezparametryczny zerujacy zmienne
        Lista(Lista* p, Lista* n, T item){prev=p; next=n; dana=item;}	//konstruktor parametryczny inicjalizujacy zmienne
		virtual ~Lista(){} //destruktor
    };



template <class T>
void Lista<T>::DodajElementNaPoczatek(T item)
{

    if(obecny==0)
    {
        obecny=new Lista(0,0,item); //tworzymy pierwszy element nie wskazujacy ani w przod ani w tyl, zawierajacy wartosc item
        head=obecny;	
        tail=obecny;	//jako jedyny elemnt jest zarowno pierwszy jak i ostatni

    }
    else{
        head->prev=new Lista(0,head,item);	//nowy element umieszczamy poprzedzajac dotychczasowy head
        head=head->prev;		//przesuwamy wskaznik
        obecny=head;			//jako obecny ustawiamy head
        }
}


template <class T>
void Lista<T>::DodajElementNaKoniec(T item)
{

    if(obecny==0)
    {
        obecny=new Lista(0,0,item);	//tworzymy pierwszy element nie wskazujacy ani w przod ani w tyl, zawierajacy wartosc item
        head=obecny;
        tail=obecny;	//jako jedyny elemnt jest zarowno pierwszy jak i ostatni
    }
    else{
        obecny=new Lista(tail,0,item); //dodajemy na koniec, wiec poprzedzajacy elemnt to pporzedni tail, nastepnego nie ma
        tail->next=obecny;		
        tail=obecny;		//pod tailem mamy teraz  nowy element
        }
}


template <class T>
void Lista<T>::UsunPierwszyElement()
{
	if(obecny==0)
		cout<<"Lista jest pusta"<<endl;
	else	

	if(head!=tail)
	{
		Lista* a;
		a=head->next; 	//tworzymy zmienna wskazujaca na nastepny elemnt 
		a->prev=0;		//zeruje wskaznik na dotychczasowy head
		delete head;	//usuwamy dotychczasowy head
		head=a;			//pod head podstawiamy nastepna wartosc
	}else{
		head=0; tail=0; obecny=0;
		}
	
}


template <class T>
void Lista<T>::UsunOstatniElement()
{
	if(obecny==0)
		cout<<"Lista jest pusta"<<endl;
	else	
	
	if(head!=tail)
	{

		Lista* a;		//tworzymy zmienna wskazujaca na poprzedni elemnt 
		a=tail->prev;
		a->next=0;		//zeruje wskaznik na dotychczasowy tail
		delete tail;	//usuwam wskaznik na dotychczasowy tail
		tail=a;			//pod tail podstawiamy poprzedzajaca wartosc
	}else{
		head=0; tail=0; obecny=0;
		}
}



template <class T>
void Lista<T>::WyswietlListe()
{
    for(Lista *l=head; l!=0; l=l->next)		//zaczynamy od pierwwzego elementu(head) i po przejsciu petli przechodzimy do nastepnego(next)
        cout<<l->dana<<endl;
}


template <class T>
void Lista<T>::WyswietlListeOdKonca()
{
    for(Lista *l=tail; l!=0; l=l->prev)		//zaczynamy od ostatniego elementu(tail) i po przejsciu petli przechodzimy do poprzedniego(prev)
        cout<<l->dana<<endl;
}



template <class T>
void Lista<T>::menu()
{
	int opcja;

	do{
		cout << "\n1.Dodaj element na poczatek"<<endl;
		cout << "2.Dodaj element na koniec"<<endl;
		cout << "3.Usun element z poczatku"<<endl;
		cout << "4.Usun element z konca"<<endl;
		cout << "5.Wyswietl liste"<<endl;
		cout << "6.Wyswietl liste od konca"<<endl;
		cout << "0.Koniec"<<endl;

		cout << "Wybierz opcje ";
		cin >> opcja;

		switch(opcja)
		{
			case 0: break;
			case 1:
				T x;
                cout << "\nWpisz element, ktory chcesz dodac na poczatek:"<<endl;
                cin >> x;
                DodajElementNaPoczatek(x);
                break;
			case 2:
				T y;
                cout << "\nWpisz element, ktory chcesz dodac na koniec:"<<endl;
                cin >> y;
                DodajElementNaKoniec(y);
				break;
			case 3:
				UsunPierwszyElement();
				break;
			case 4:
				UsunOstatniElement();
				break;
			case 5:
				cout<<"\nElementy listy: "<<endl;
				WyswietlListe();
				break;
			case 6:
				cout<<"\nElementy listy od konca: "<<endl;
				WyswietlListeOdKonca();
				break;
		}

	}while(opcja !=0);

}


int main()
{
	Lista<int> li;	//tworzymy instancje klasy, przez ktora wywolujemy funkcje
	
	li.menu();		//wywolanie menu

	return 0;
}


