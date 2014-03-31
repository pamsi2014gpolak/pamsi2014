#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;


template <class T>
class Tablica
{
	private:
        T *tablica;
        int n;

		void WczytanieTablicyZKonsoli();
		void WyswietlenieTablicyWKonsoli();
		void WczytanieTablicyZPlikuTekstowego();
		void ZapisywanieDoPlikuTekstowego();
		void WypelnianieTablicyLosowymi();
		void ZapisywanieDoPlikuBinarnego();
		void WczytanieZPlikuBinarnego();
		T DynamicznaKopia();


	public:
		Tablica():tablica(NULL),n(0){}
		virtual ~Tablica(){delete [] tablica;}
		void menu();
			Tablica & operator =(const Tablica<T> &t)
			{
            return *this;
            }

};



template <class T>
void Tablica <T>::WczytanieTablicyZKonsoli() 
{
	cout << "Podaj rozmiar tablicy\n";
    cin >> n;
    tablica = new T[n];

	for(int i = 0; i < n; i++) 
	{

	cin >> tablica[i];

	}
}


template <class T>
void Tablica <T>::WyswietlenieTablicyWKonsoli() 
{
	for(int i = 0; i < n; i++) 
	{
	cout << tablica[i] << endl;
	}
}

template <class T>
void Tablica <T>::WczytanieTablicyZPlikuTekstowego()
{
	
	int i=0;
	int j=0;
    
	cout << "Podaj rozmiar tablicy\n";
	cin >> n;

	ifstream liczby;

	liczby.open("liczby.txt");
	tablica = new T[n];
	while(liczby.good())
	{
		liczby >> tablica[i];
		i++;
	}
	liczby.close();


	while(j<n)
	{
		cout<<tablica[j]<<endl;
		j++;
	}


}


template <class T>
void Tablica <T>::ZapisywanieDoPlikuTekstowego()
{
	int i=0;
	ofstream liczby1;

	liczby1.open("liczby1.txt");
	while(i<n)
	{
		liczby1 << tablica[i]<<endl;
		i++;
	}
	liczby1.close();
}


template <class T>
void Tablica <T>::WypelnianieTablicyLosowymi()
{
	int i=0;
	int j=0;
	int ileLosowych;
	int n;
	cout<<"Podaj rozmiar tablicy ";
	cin>>n;
	tablica = new T[n];

	cout<<"Ile liczb sposrod "<<n<<" ma byc losowych?"<<endl;
	cin >> ileLosowych;

	while(i<ileLosowych)
	{
	tablica[i]= (T)(rand() % 10)+1;
		i++;
	}

	while(i<n)
	{
		tablica[i]=0;
		i++;
	}

	cout<<"\n\n";
	while(j<n)
	{
		cout<<tablica[j]<<endl;
		j++;
	}
}





template <class T>
void Tablica <T>::ZapisywanieDoPlikuBinarnego()
{
	int i=0;

	ofstream liczbybinarne;

    liczbybinarne.open("liczbybinarne.bin", ios::binary);
	while(i<n)
	{
		liczbybinarne.write((const char*)&tablica[i],sizeof(int));
		i++;
	}


	liczbybinarne.close();
}


template <class T>
void Tablica <T>::WczytanieZPlikuBinarnego()
{
	int i=0;
	int j=0;

	cout << "Podaj rozmiar tablicy\n";
	cin >> n;    

	ifstream liczbybinarne;

	liczbybinarne.open("liczbybinarne.bin", ios::binary);
	tablica = new T[n];
	while(liczbybinarne.good())
	{
		liczbybinarne.read((char*)&tablica[i],sizeof(int));
		i++;
	}

	while(j<n)
	{
		cout<<tablica[j]<<endl;
		j++;
	}

}


template <class T>
T Tablica<T>::DynamicznaKopia()
{
	T *tabKopia;
	tabKopia = new T[n];
	for(int i=0;i<n;i++)
		tabKopia[i] = tablica[i];
	for(int j=0;j<n;j++)
		cout<<tabKopia[j]<<endl;
	return *tabKopia;
}




template <class T>
void Tablica <T>::menu()
{

	int opcja;

	do{
		cout << "1.Wczytanie tablicy z konsoli"<<endl;
		cout << "2.Wyświetlenie tablicy"<<endl;
		cout << "3.Wczytywanie z pliku tekstowego"<<endl;
		cout << "4.Zapisywanie do pliku tekstowego"<<endl;
		cout << "5.Wczytywanie z pliku binarnego"<<endl;
		cout << "6.Zapisywanie do pliku binarnego"<<endl;
		cout << "7.Wypełnianie tablicy liczbami losowymi"<<endl;
		cout << "8.Dynamiczna kopia tablicy + wyswietlenie"<<endl;
		cout << "0.Koniec"<<endl;

		cout << "Wybierz opcje";
		cin >> opcja;

		switch(opcja)
		{
			case 0: break;
			case 1:
                WczytanieTablicyZKonsoli();
                break;
			case 2:
				WyswietlenieTablicyWKonsoli(); break;
			case 3:
				WczytanieTablicyZPlikuTekstowego(); break;
			case 4:
				ZapisywanieDoPlikuTekstowego(); break;
			case 5:
				WczytanieZPlikuBinarnego(); break;
			case 6:
				ZapisywanieDoPlikuBinarnego(); break;
			case 7:
				WypelnianieTablicyLosowymi(); break;
			case 8:
				DynamicznaKopia(); break;
				

		}

	}while(opcja !=0);

}



int main()
{
	srand(time(0));
   	Tablica<int> t;
    t.menu();

return 0;
}

