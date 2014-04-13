#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
#define n 10000
class Introsort{
    vector<int> tab;

    public:
        void Sortowanie(int left, int right);
        void Kopcowanie();
        void WypelnijWektor();
        void Wyswietl();
    };

void Introsort::Sortowanie(int left, int right)
{
  int i = left;
    int j = right;
    int x = tab[( left + right ) / 2 ];
    do
    {
        while( tab[ i ] < x )
             i++;
       
        while( tab[ j ] > x )
             j--;
       
        if( i <= j )
        {
            swap( tab[ i ], tab[ j ] );
           
            i++;
            j--;
        }
    } while( i <= j );
   
    if( left < j ) Sortowanie( left, j );
   
    if( right > i ) Sortowanie( i, right );
}


void Introsort::Kopcowanie()
{
    make_heap(v.end()-5, v.end());
    sort_heap(v.end()-5, v.end());

}


void Introsort::WypelnijWektor()
{

    int elem;
    cout<<endl;
    for(int i=0; i<n; i++)
    {
        elem=( rand() % 50 ) + 1;
        tab.push_back(elem);
    }
}

void Introsort::Wyswietl()
{
    cout<<endl;
    for(vector<int>::iterator it=tab.begin(); it<tab.end(); it++)
    {
        cout<<*it<<endl;
    }
}


int main()
{
    srand(time(NULL));

    Introsort l;

    l.WypelnijWektor();
	cout<<"zaczynamy sortowanie"<<endl;
    l.Sortowanie(0,n-1);
	cout<<"dupa"<<endl;
    //l.Kopcowanie();
    //l.Wyswietl();

    return 0;
}

