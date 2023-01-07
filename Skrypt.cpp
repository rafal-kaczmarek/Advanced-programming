#include<iostream>
#include<cmath>
#include<vector>
#include<ctime>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <iomanip>

using namespace std;

//Klasa w ktorej odbywa się symulacja rozkladu bogactwa
class Rozklad_bogactwa{

private:
int Osoba_A;
int Osoba_B;
int Wygrany;
int Przegrany;
int Liczba_osob;
int Liczba_rund;
int Dochod_poczatkowy;
int Osoba_max;
int* Liczba_wygranych;
int* Liczba_przegranych;
double Podatek;
double Suma_podatkow;
double Maksimum;
double Odsetek_majatku;
double** Majatek;


public:
//Konstruktor klasy
Rozklad_bogactwa(int Liczbaosob, int Liczbarund, int Dochodpoczatkowy=10, double Odsetekmajatku=0.01, double Stawkapodatku=0){
	srand(time(0));
	Liczba_osob = Liczbaosob;
	Liczba_rund = Liczbarund;
	Dochod_poczatkowy = Dochodpoczatkowy;
	Podatek = Stawkapodatku;
	Odsetek_majatku = Odsetekmajatku;

	Liczba_wygranych = new int[Liczba_osob];
	Liczba_przegranych = new int[Liczba_osob];

	Suma_podatkow = 0;
	Maksimum = 0;
	Osoba_max = -1;

	Majatek = new double*[Liczba_rund + 1];
	for(int i=0; i<=Liczba_rund; i++){
    	Majatek[i] = new double[Liczba_osob];
	}

	for(int i=0; i<Liczba_osob; i++){
    	Liczba_wygranych[i] = 0;
    	Liczba_przegranych[i] = 0;
    	Majatek[0][i] = Dochod_poczatkowy;
	}
}

//Losowanie 2 osób, które będą między sobą grać
int Losowanie_osob(){
    do {
        Osoba_A =rand()%Liczba_osob;
        Osoba_B =rand()%Liczba_osob;
    } while(Osoba_A==Osoba_B);
    return Osoba_A;
    return Osoba_B;
}

//Losowanie kto wygra rozgrywkę
int Wynik_gry() {
	if(rand()%2==0){
    	Wygrany = Osoba_A;
    	Przegrany = Osoba_B;
    	Liczba_wygranych[Osoba_A] = Liczba_wygranych[Osoba_A] + 1;
    	Liczba_przegranych[Osoba_B] = Liczba_przegranych[Osoba_B] + 1;
	}
	else {
        Wygrany = Osoba_B;
    	Przegrany = Osoba_A;
    	Liczba_przegranych[Osoba_A] = Liczba_przegranych[Osoba_A] + 1;
    	Liczba_wygranych[Osoba_B] = Liczba_wygranych[Osoba_B] + 1;
	}
	return Przegrany;
	return Wygrany;
}

//Zmiana wartości majątku w zależności od tego kto wygrał a kto przegrał. Kwota wygranej to minimum z wartości majątku dwóch graczy
//pomnożonego przez odsetek wskazany jako procent wygranej. Jeśli występuje podatek to wygrana jest pomniejszona o podatek.
void Zmiana_majatku(int Numer_rundy){

    double Kwota_Wygranej = Odsetek_majatku * min(Majatek[Numer_rundy-1][Wygrany], Majatek[Numer_rundy-1][Przegrany]);

    if(Kwota_Wygranej > 0) Kwota_Wygranej = Kwota_Wygranej;
    else Kwota_Wygranej = 0;

    for(int i=0; i<Liczba_osob; i++){
        if(i == Wygrany){
            Majatek[Numer_rundy][Wygrany] = Majatek[Numer_rundy-1][Wygrany] + Kwota_Wygranej*(1-Podatek);
        }
        else if (i == Przegrany) {
            Majatek[Numer_rundy][Przegrany] = Majatek[Numer_rundy-1][Przegrany] - Kwota_Wygranej;
        }
    	else {
            Majatek[Numer_rundy][i] = Majatek[Numer_rundy-1][i];
        }
    }

    Suma_podatkow = Suma_podatkow + Kwota_Wygranej*Podatek;
}

//Wyświetlanie wynikow ostatniej rundy. Przydatne do wyswietlenia dokładnego przebiegu rozgrywki, krok po kroku
void Drukuj_przebieg_ostatniej_rundy(int numer_rundy=-1, int liczba_osob=2){
    if(numer_rundy==-1) numer_rundy=Liczba_rund;
    cout << endl;
    cout<< "Runda " <<numer_rundy << endl <<"    Wygrany to: " << Wygrany << endl << "    Przegrany to: "<< Przegrany <<endl;
        cout<< "Wartosc majatku:"<< endl;
    if(liczba_osob==2) {
        cout <<"    Osoba " << Wygrany << ": " << Majatek[numer_rundy][Wygrany] << endl;
        cout <<"    Osoba " << Przegrany << ": " << Majatek[numer_rundy][Przegrany] << endl;
    }
    else if (liczba_osob>=0){
        for(int j =0;j<liczba_osob;j++){
            cout <<"    Osoba " << j << ": " << Majatek[numer_rundy][j] << endl;
        }
    }
}

//Uruchamia w odpowiedniej kolejności metody konieczne do przeprowadzenia symulacji
void Uruchom_symulacje(bool przebieg_symulacji = false, bool dla_wszystkich=false) {
    for (int i = 1; i<=Liczba_rund; i++)
    {
        Losowanie_osob();
        Wynik_gry();
        Zmiana_majatku(i);
        if(przebieg_symulacji == true && dla_wszystkich == true) Drukuj_przebieg_ostatniej_rundy(i,Liczba_osob);
        if(przebieg_symulacji == true && dla_wszystkich == false) Drukuj_przebieg_ostatniej_rundy(i,2);
    }
}

//Wyświetlanie ustawień symulacji wprowadzonych przez użytkownika
void Drukuj_ustawienia_symulacji(){
cout << endl << "Ustawienia symulacji"<<endl;
cout << "   Liczba osob: "<<Liczba_osob << endl;
cout << "   Liczba rund: "<<Liczba_rund << endl;
cout << "   Dochod poczatkowy: "<<Dochod_poczatkowy << endl;
cout << "   Odsetek majatku uzywany do wyznaczenia wygranej: "<<Odsetek_majatku<< endl;
cout << "   Podatek: "<<Podatek<< endl;
}

//Podsumowanie ostatecznych wyników
void Drukuj_wyniki(int numer_osoby = -1){
	if (numer_osoby >=0) {
    	cout<<"Osoba "<<numer_osoby<<":"<<endl<<"    wygrala "<<Liczba_wygranych[numer_osoby]<<" razy" <<endl;
    	cout<<"    przegrala "<<Liczba_przegranych[numer_osoby]<<" razy" <<endl;
    	cout<<"    uzyskala majatek rowny "<<Majatek[Liczba_rund][numer_osoby] <<endl;
	}
	else {
        for(int i=0; i<Liczba_osob; i++){
            cout<<"Osoba "<<i<<":"<<endl<<"    wygrala "<<Liczba_wygranych[i]<<" razy" <<endl;
            cout<<"    przegrala "<<Liczba_przegranych[i]<<" razy" <<endl;
            cout<<"    uzyskala majatek rowny "<<Majatek[Liczba_rund][i] <<endl;
        }
	}
}

//Wartość minimalna majątku z ostatecznych wyników gry
double Wartosc_minimum() {
	double minimum = Majatek[Liczba_rund][0];
	for (int i=1; i<Liczba_osob; i++) {
    	if(Majatek[Liczba_rund][i] < minimum ) minimum = Majatek[Liczba_rund][i];
	}
	return minimum;
}

//Wartość maksymalna majątku z ostatecznych wyników gry
double Wartosc_maksimum() {
	Maksimum=Majatek[Liczba_rund][0];
	for (int i=1; i<Liczba_osob; i++) {
    	if(Majatek[Liczba_rund][i] > Maksimum) {
            Maksimum = Majatek[Liczba_rund][i];
            Osoba_max = i;
    	}
	}
	return Maksimum;
}

//Wartość średniej majątku z ostatecznych wyników gry
double Wartosc_sredniej() {
	double suma = 0;
	for (int i=0; i<Liczba_osob; i++) {
    	suma = suma + Majatek[Liczba_rund][i];
	}
	return suma/(Liczba_osob);
}

//Wartość wariancji majątku z ostatecznych wyników gry
double Wartosc_wariancji() {
	double licznik = 0;
	for (int i=0; i<Liczba_osob; i++) {
    	licznik = licznik + pow((Majatek[Liczba_rund][i]-Majatek[0][0]),2);
	}
	return licznik/(Liczba_osob);
}

//Zwraca sumę podatków z każdej transakcji
double Wartosc_podatkow()
{
    return Suma_podatkow;
}

//Wyswietlenie wygranego
int Wygrana_osoba(){
    if(Osoba_max== -1) Wartosc_maksimum();
	return Osoba_max;
}

//Wyswietlenie numeru rundy w której uzyskano wartość równą wartości maksymalnej z ostatecznych wyników gry
int Numer_rundy_max(){
    if(Osoba_max== -1) Wartosc_maksimum();
    bool x=true;
	int j=0;
    while(x==true && j <= Liczba_rund)
        if(Majatek[j][Osoba_max]==Maksimum) {
            return j;
            x=false;
            }
        else j++;
}

//Zapisuje majątek każdego gracza w poszczególnych rundach do pliku csv
void Zapisz_wyniki_csv(string nazwa_pliku = "wyniki_symulacji.csv") {
    cout<<endl<<"Trwa zapisywanie wynikow w pliku o nazwie: "<< nazwa_pliku<<endl;
    std::ofstream myfile;
    myfile.open(nazwa_pliku.c_str());

    myfile<<"Numer rundy;";
    for (int j=0;j<Liczba_osob;j++){
        myfile<<"Osoba "<<j<<";";
    }
    myfile<<"\n";
    for(int i = 0;i<=Liczba_rund;i++){
        myfile << i <<";";
        for(int j=0;j<Liczba_osob;j++){
            myfile<<Majatek[i][j] <<";";
        }
        myfile <<"\n";
    }
    myfile.close();
    cout<<endl<<"Wyniki zostaly zapisane"<<endl;
}

};

int main()
{
    cout<<setprecision(5);
    Rozklad_bogactwa RB(10,500000,10,0.1,0.1);
    RB.Uruchom_symulacje();
    cout<< endl << "######## PODSUMOWANIE ########" << endl;
    RB.Drukuj_ustawienia_symulacji();
    cout<<endl;
    RB.Drukuj_wyniki();
    cout<< endl;
    cout<< "Wygrana osoba: " <<RB.Wygrana_osoba()<<endl;
    cout<< "Minimum: " <<RB.Wartosc_minimum() << endl;
    cout<< "Maksimum: " << RB.Wartosc_maksimum()<<endl;
    cout<< "Wariancja: " << RB.Wartosc_wariancji() << endl;
    cout<< "Maksimum zostalo uzyskane w rundzie: " << RB.Numer_rundy_max()<<endl;
    cout<< "Suma podatkow: " <<RB.Wartosc_podatkow() << endl;
    //cout<< "Srednia: " << RB.Wartosc_sredniej() << endl;

    RB.Zapisz_wyniki_csv("Wyniki.csv");
}
