#include <iostream>
#include <stack>

using namespace std;

template <class T>
class Stack {

        stack<T> stos;  //stos, wykorzystywany we wszystkich funkcjach
        stack<T> s;     //stos pomocniczy pozwalajacy na przechowanie elementow glownego stosu i powrotne zwrocenie
    public:

        void DodajElement(T Item);
        void UsunElement();
        void WyswietlStos();
        void ZnajdzElement(T Item);
        void menu();

        Stack(){}
        virtual ~Stack(){}

};


template <class T>
void Stack<T>::DodajElement(T Item)
{
    stos.push(Item);
}

template <class T>
void Stack<T>::UsunElement()
{
    if(!stos.empty())
        stos.pop();
    else
        cout<<"Stos jest pusty"<<endl;
}

template <class T>
void Stack<T>::WyswietlStos()
{

    while(!stos.empty())
    {
        cout<<stos.top()<<endl;
        s.push(stos.top());
        stos.pop();
    }
    while(!s.empty())
    {
        stos.push(s.top());
        s.pop();
    }
}

template <class T>
void Stack<T>::ZnajdzElement(T Item)
{
    T a;

    while(!stos.empty())
    {
        a=stos.top();
        if(a==Item)
        {
            cout<<"Znaleziono element\n";
            break;

        }else{
            s.push(stos.top());
            stos.pop();
            }

    }

    if(stos.empty())
        cout<<"Nie znaleziono elementu\n";

    while(!s.empty())
    {
        stos.push(s.top());
        s.pop();
    }
}


template <class T>
void Stack<T>::menu()
{
	int opcja;


	do{
		cout << "\n1.Dodaj element"<<endl;
		cout << "2.Usun element"<<endl;
        cout << "3.Znajdz element"<<endl;
		cout << "4.Wyswietl stos"<<endl;
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
				UsunElement();
				break;
			case 3:
                T d;
                cout << "\nWpisz element, ktory chcesz znalezc:"<<endl;
                cin >> d;
                ZnajdzElement(d);
                break;
			case 4:
				cout<<"\nZawartosc stosu: "<<endl;
				WyswietlStos();
				break;
		}

	}while(opcja !=0);

}

int main()
{
Stack<int> st;

st.menu();

return 0;
}
