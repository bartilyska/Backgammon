#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#define POLA 25 //indeksowane pola od 1 dlatego 25 a nie 24
#define PLANSZAX 60 // rozmiar x tablicy z plansza
#define PLANSZAY 20 //
#define ILEMAKS 7 // maksymalna ilosc pionkow na polu, w przeciwnym wypadku bedzie ilosc pionkow
#define LEWYGORNY 13 //indeks pola w lewym gornym rogu
#define LEWYDOLNY 12
#define BRAKBICIABIALE 50 // w celu sprawdzania najblizszego bicia dla bialych
#define BRAKBICIACZARNE -1 // w celu spr najblizszego bicia dla czarnych
#define GORA '-'  // symbole do tworzenia ASCII art
#define BOK ':'
#define BAR '|'
#define NIC '.'
#define BIALY 'b'
#define CZARNY 'c'
#define CZYSCEKRAN system("cls")
using namespace std;
//1.Znalezienie najblizszego bicia do dworu przeciwnika powoduje ze  nie omine bicia ! nie musze rozdziabywac ruchow!
//2.Lepiej znalezc pionka ktorego chce zbic, a nie ktorym pionkiem sie ruszyc i o ile oczek.
struct gra // na podstawie tego mozna narysowac plansze i przesymulowac gre
{
    int biale[POLA];
    int czarne[POLA];
    int rzut1,rzut2,gracz,barbialy,barczarny,poprzedniruch,posuniecia;
};
struct gracz
{
    char imie[30];
    int punkty;
};
int rzutkostka();
int maxi(int  a,int  b);
void autor();
void info();
void ktoZaczyna(gra *plansza);
void ustawPionki(gra *plansza);
void wyswietlPionki(gra &plansza);
void menu(gra *plansza); // basic funkcje
void poczatek(gra *plansza);
void graj(gra *plansza);
void grajZWczytu(gra *plan);
bool czyGrac(gra *plansza);
int ilepionkowjednego(int tab[]);
void wypiszInfoOGrze(gra *plansza);
void warunkiDlaPierwszego(gra *plansza); //glowne drzewo sterowania gra
void warunkiDlaDrugiego(gra *plansza); //glowne drzewo sterowania gra

bool warunekKolejkaBialy(gra *plansza);
bool warunekKolejkaCzarny(gra *plansza);//ruszanie z bara
void CzyLegalBarBialy(gra *plansza);
void CzyLegalBarCzarny(gra *plansza);
bool CzyMozliwyRuchBialy(gra *plansza);
bool CzyMozliwyRuchCzarny(gra *plansza);
void RuchBarBialy(int *oczka,gra *plansza);
void RuchBarCzarny(int *oczka,gra *plansza);
int znajdzBicieBarBialy(int*oczka,gra *plansza);
int znajdzBicieBarCzarny(int*oczka,gra *plansza);

bool CzyMozliwyRuchBialyDublet(gra *plansza);// ruchy po dubletach
bool CzyMozliwyRuchCzarnyDublet(gra *plansza);
bool CzyLegalRuchBialyDublet(int pole,int oczka,gra *plan);
bool CzyLegalRuchCzarnyDublet(int pole,int oczka,gra *plan);
void RuchBialyDublet(gra *plansza);
void RuchCzarnyDublet(gra *plansza);
int BicieCzarnyDublet(gra *plansza);
int BicieBialyDublet(gra *plansza);

void ruchBialy(gra *plansza);
void ruchCzarny(gra *plansza); // ruszanie pionkami i forced bicia
bool LegalBialy(int pole,int oczka,gra *p);
bool LegalCzarny(int pole,int oczka,gra *p);
bool BijBialy(int pole,int oczka,gra *plansza);
bool BijCzarny(int pole,int oczka,gra *plansza);
int BicieBialy(gra *p);
int BicieCzarny(gra *p);


bool CzyFazaBialy(gra *plansza);
bool CzyFazaCzarny(gra *plansza);//sytuacje koncowki gry
void WyprowadzBialy(gra *plansza);
void WyprowadzCzarny(gra *plansza);
bool czyLegalWyprowadzBialy(int pole,int oczka,gra *plansza);
bool czyLegalWyprowadzCzarny(int pole,int oczka,gra *plansza);

bool TakiSamRuch(int oczka,gra *plansza);
void UstawPoprzedniRuch(int *oczka,gra *plansza);


void wyswietlWszystko(gra *plansza);
void tworzPlansze(gra &plansza);//rysowanie planszy
void indeksyPolGora();
void indeksyPolDol();
void tworzLiczbyGora(gra &plan,char obraz[PLANSZAY][PLANSZAX]);
void tworzLiczbyDol(gra &plan,char obraz[PLANSZAY][PLANSZAX]);
void tworzPionkiGoraLewy(gra &plansza,char obraz[PLANSZAY][PLANSZAX]);
void tworzPionkiGoraPrawy(gra &plansza,char obraz[PLANSZAY][PLANSZAX]);
void tworzPionkiDolLewy(gra &plansza,char obraz[PLANSZAY][PLANSZAX]);
void tworzPionkiDolPrawy(gra &plansza,char obraz[PLANSZAY][PLANSZAX]);
void PionkiBar(gra &plansza,char obraz[PLANSZAY][PLANSZAX]);
void wykonaj(gra &plansza,char obraz[PLANSZAY][PLANSZAX]);
void rysujPlansze(char obraz[PLANSZAY][PLANSZAX]);

void RuchNiepoprawny();
void StrataKolejki(); // komunikaty
void WymuszoneBicie();
void NieDziala();
void zapiszPlansze(gra *plansza); // zapisuje 1 plansze (podst)
void zapiszGre(gra *plansza); // bedzie zapisywac na koncu pliku nowe ruchy (roz)
void czyscGre(); //wyczysci plik po zaczeciu nowej gry
void wczytajPlansze(gra *plansza);
void wczytajGre();
void wyborPlanszy(gra *calosc,int ile);
void wyswietlRanking();
void sortujRanking(gracz *osoba,int ile); // ranking i koncowka gry
int LiczPunkty(int bar,int tab[]);
void DodajNaRanking(int pkt);
void Zwyciestwo(gra *plansza);
void ZapisRanking(gracz *osoba,int ile);
int main()
{
    srand(time(NULL));
    gra plansza; //tworz strukture
    ustawPionki(&plansza);
    menu(&plansza);
    return 0;
}
int rzutkostka()
{
    return rand()%6+1;
}
int maxi(int a,int b)
{
    if(a>b)
        return a;
    else return b;
}
void autor()
{
    cout<<"\t\t       WELCOME TO THE GAME OF BACKGAMMON"<<endl;
    cout<<"\t\t\t Bartosz Lyskanowski s198051"<<endl<<endl;
}
void info()
{
    cout<<" \t   P-Play     E-Exit      R-Ranking     L-Load    O-obejrz"<<" ";
}
void menu(gra *plansza)
{
    tworzPlansze(*plansza);
    cout<<endl;
    info();
    char input;
    cin>>input;
    switch(input)
    {
        case 'P': case 'p':
        {
            CZYSCEKRAN;
            poczatek(plansza);
            break;
        }
        case 'R': case 'r':
        {
            CZYSCEKRAN;
            cout<<"Ranking\n\n";
            wyswietlRanking();
            cout<<endl<<"Wpisz znak by wrocic do menu ";
            char a;
            cin>>a;
            CZYSCEKRAN;
            menu(plansza);
            break;
        }
        case 'L': case 'l':
        {
            CZYSCEKRAN;
            wczytajPlansze(plansza);
            grajZWczytu(plansza);
            break;
        }
        case 'O': case 'o':
        {
            CZYSCEKRAN;
            wczytajGre();
            break;
        }
        default:
        {
            exit(0);
        }
    }
}
void poczatek(gra* plansza)
{
    tworzPlansze(*plansza);
    ktoZaczyna(plansza);
    char c;
    cout<<"\tWcisnij guzik, by grac dalej: ";
    cin>>c;
    CZYSCEKRAN;
    czyscGre();
    while(czyGrac(plansza))
    {
        tworzPlansze(*plansza);
        graj(plansza);
        CZYSCEKRAN;
    }
    Zwyciestwo(plansza);
}
void graj(gra* plansza)
{
    plansza->rzut1=rzutkostka(),plansza->rzut2=rzutkostka();
    plansza->poprzedniruch=0; // brak poprzedniego ruchu
    if(plansza->rzut1==plansza->rzut2)
        plansza->posuniecia=4;
    else plansza->posuniecia=2;
    wypiszInfoOGrze(plansza);
    while(plansza->posuniecia>0)
    {
        zapiszGre(plansza);
        zapiszPlansze(plansza);
        if(plansza->gracz==1)
        {
            warunkiDlaPierwszego(plansza);
        } // glowna czesc gry
        else{
            warunkiDlaDrugiego(plansza);
        }
        if(!czyGrac(plansza))
            break;
    }
    if(plansza->gracz==1)
        plansza->gracz=2;
    else plansza->gracz=1;
}
void grajZWczytu(gra *plan)
{
    while(czyGrac(plan))
    {
        tworzPlansze(*plan);
        wypiszInfoOGrze(plan);
        while(plan->posuniecia>0)
        {
            zapiszGre(plan);
            zapiszPlansze(plan);
            if(plan->gracz==1)
            {
                warunkiDlaPierwszego(plan);
            }
            else
            {
                warunkiDlaDrugiego(plan);
            }
            if(!czyGrac(plan))
                break;
        }
        if(plan->gracz==1)
            plan->gracz=2;
        else plan->gracz=1;
        plan->rzut1=rzutkostka(),plan->rzut2=rzutkostka();
         plan->poprzedniruch=0;
        if(plan->rzut1==plan->rzut2)
            plan->posuniecia=4;
        else plan->posuniecia=2;
        CZYSCEKRAN;
    }
    Zwyciestwo(plan);
}
void wypiszInfoOGrze(gra *plansza)
{
    cout<<"\tWyrzucone oczka to: "<<plansza->rzut1<<" "<<plansza->rzut2<<endl;
    cout<<"\tKolej gracza nr: "<<plansza->gracz<<" "<<"Posuniecia: "<<plansza->posuniecia<<endl;
    cout<<"\tPoprzednie posuniecie: ";
    if(plansza->poprzedniruch!=0)
        cout<<plansza->poprzedniruch<<endl;
    else cout<<endl;
}
void wyswietlWszystko(gra *plansza)
{
    CZYSCEKRAN;
    tworzPlansze(*plansza);
    wypiszInfoOGrze(plansza);
}
bool TakiSamRuch(int oczka,gra *plansza)
{
    if(plansza->rzut1==plansza->rzut2)
        return false;
    else
    {
        if(oczka==plansza->poprzedniruch)
            return true;
        else return false;
    }
    return false;
}
bool czyGrac(gra* plansza)
{
    if(ilepionkowjednego(plansza->biale)==0)
        return false;
    if(ilepionkowjednego(plansza->czarne)==0)
        return false;
     return true;
}
int ilepionkowjednego(int tab[])
{
    int sumapionkow=0;
    for(int i=1;i<POLA;i++)
    {
        sumapionkow+=tab[i];
    }
    return sumapionkow;
}
void ktoZaczyna(gra* plansza)
{
    int kostka1=0,kostka2=0,gracz;
    while(kostka1==kostka2) //rzucaj poki nie beda rozne
    {
    kostka1=rzutkostka();
    kostka2=rzutkostka();
        if(kostka1>kostka2)
            gracz=1;
        else gracz=2;
    }
    cout<<"\tKostki to: "<<kostka1<<" "<<kostka2<<endl;
    cout<<"\tZaczyna gracz: "<<gracz<<endl;
    plansza->gracz=gracz;
}
void UstawPoprzedniRuch(int *oczka,gra *plansza)
{
    if(plansza->rzut1!=plansza->rzut2)
    {
        plansza->poprzedniruch=(*oczka);
    }
}
void warunkiDlaPierwszego(gra *plansza) //drzewo sterowania
{
    if(plansza->barbialy>0)
        CzyLegalBarBialy(plansza);
    else{
        if(CzyFazaBialy(plansza))
        {
          WyprowadzBialy(plansza);
        }
        else
        {
            if(plansza->rzut1==plansza->rzut2)
            {
                if(CzyMozliwyRuchBialyDublet(plansza))
                {
                    RuchBialyDublet(plansza);
                }
            }
            else
            {
                if(CzyMozliwyRuchBialy(plansza))
                {
                      ruchBialy(plansza);
                }
            }
        }
    }
}
void warunkiDlaDrugiego(gra *plansza) //drzewo sterowania
{
    if(plansza->barczarny>0)
        CzyLegalBarCzarny(plansza);
    else{
        if(CzyFazaCzarny(plansza))
        {
            WyprowadzCzarny(plansza);
        }
        else
        {
            if(plansza->rzut1==plansza->rzut2)
            {
                if(CzyMozliwyRuchCzarnyDublet(plansza))
                {
                    RuchCzarnyDublet(plansza);
                }
            }
            else
            {
                if(CzyMozliwyRuchCzarny(plansza))
                {
                    ruchCzarny(plansza);
                }
            }
        }
    }
}
void CzyLegalBarBialy(gra *plansza)
{
    if(warunekKolejkaBialy(plansza))
    {
        plansza->posuniecia=0;
        StrataKolejki();
        wyswietlWszystko(plansza);
    }
    else{
        int oczka;
        cout<<"\tMozliwy ruch tylko jedna kostka. Przemiesc pionka z bara :";
        cin>>oczka;
        if((oczka!=plansza->rzut1 && oczka!=plansza->rzut2) || plansza->czarne[oczka]>1 || TakiSamRuch(oczka,plansza))
        {
            RuchNiepoprawny();
            wyswietlWszystko(plansza);
        }
        else
        {
            RuchBarBialy(&oczka,plansza);
        }
    }
}
void CzyLegalBarCzarny(gra *plansza)
{
    if(warunekKolejkaCzarny(plansza))
    {
        plansza->posuniecia=0;
        StrataKolejki();
        wyswietlWszystko(plansza);
    }
    else{
        int oczka;
        cout<<"\tMozliwy ruch tylko jedna kostka. Przemiesc pionka z bara :";
        cin>>oczka;
        if((oczka!=plansza->rzut1 && oczka!=plansza->rzut2)||plansza->biale[POLA-oczka]>1||TakiSamRuch(oczka,plansza))
        {
            RuchNiepoprawny();
            wyswietlWszystko(plansza);
        }
        else
        {
            RuchBarCzarny(&oczka,plansza);
        }
    }
}
void RuchBarCzarny(int*oczka,gra *plansza)
{
    int bicie=znajdzBicieBarCzarny(oczka,plansza); //indeks pionka do zbicia
    if(bicie==BRAKBICIACZARNE)
    {
        plansza->barczarny--;
        plansza->czarne[POLA-*oczka]++;
        plansza->posuniecia--;
        UstawPoprzedniRuch(oczka,plansza);
    }
    else{
        if(POLA-*oczka!=bicie)
        {
            WymuszoneBicie();
        }
        else
        {
            plansza->biale[POLA-*oczka]--;
            plansza->barbialy++;
            plansza->barczarny--;
            plansza->czarne[POLA-*oczka]++;
            plansza->posuniecia--;
            UstawPoprzedniRuch(oczka,plansza);
        }
    }
    wyswietlWszystko(plansza);
}
void RuchBarBialy(int*oczka,gra *plansza)
{
    int bicie=znajdzBicieBarBialy(oczka,plansza);
    if(bicie==BRAKBICIABIALE)
    {
        plansza->barbialy--;
        plansza->biale[*oczka]++;
        plansza->posuniecia--;
        UstawPoprzedniRuch(oczka,plansza);
    }
    else{
        if(*oczka!=bicie)
        {
            WymuszoneBicie();
        }
        else
        {
            plansza->czarne[*oczka]--;
            plansza->barczarny++;
            plansza->barbialy--;
            plansza->biale[*oczka]++;
            plansza->posuniecia--;
            UstawPoprzedniRuch(oczka,plansza);
        }
    }
    wyswietlWszystko(plansza);
}
int znajdzBicieBarBialy(int*oczka,gra *plansza)
{
    int najbicie=BRAKBICIABIALE;
    if(plansza->czarne[plansza->rzut1]==1 && najbicie>plansza->rzut1)
        najbicie=plansza->rzut1;
    if(plansza->czarne[plansza->rzut2]==1 && najbicie>plansza->rzut2)
        najbicie=plansza->rzut2;
    return najbicie;
}
int znajdzBicieBarCzarny(int*oczka,gra *plansza)
{
    int najbicie=BRAKBICIACZARNE;
    if(plansza->biale[POLA-plansza->rzut1]==1 && najbicie<POLA-plansza->rzut1)
        najbicie=POLA-plansza->rzut1;
    if(plansza->biale[POLA-plansza->rzut2]==1 && najbicie<POLA-plansza->rzut2)
        najbicie=POLA-plansza->rzut2;
    return najbicie;
}
bool warunekKolejkaBialy(gra *plansza)
{
    if((plansza->czarne[plansza->rzut1]>1 && plansza->czarne[plansza->rzut2]>1) ||
       (plansza->czarne[plansza->rzut1]>1 && plansza->poprzedniruch==plansza->rzut2)
       || (plansza->czarne[plansza->rzut2]>1 && plansza->poprzedniruch==plansza->rzut1))
        return true;
    return false;
}
bool warunekKolejkaCzarny(gra *plansza)
{
    if((plansza->biale[POLA-plansza->rzut1]>1 && plansza->biale[POLA-plansza->rzut2]>1) ||
       (plansza->biale[POLA-plansza->rzut1]>1 && plansza->poprzedniruch==plansza->rzut2)
       || (plansza->biale[POLA-plansza->rzut2]>1 && plansza->poprzedniruch==plansza->rzut1))
       return true;
    return false;
}

//koniec sekcji z barem

bool CzyMozliwyRuchBialy(gra *plansza)
{
    for(int i=1;i<POLA;i++)
    {
        if(plansza->biale[i]>0)
        {  // jezeli ktores z pol jest dostepne to i tak mozna sie ruszyc dwiema kostkami
            if(i+plansza->rzut1<POLA && plansza->rzut1!=plansza->poprzedniruch &&  plansza->czarne[i+plansza->rzut1]<2)
                return true;
            if(i+plansza->rzut2<POLA  && plansza->rzut2!=plansza->poprzedniruch && plansza->czarne[i+plansza->rzut2]<2)
                return true;
        }
    }
    StrataKolejki();
    plansza->posuniecia=0;
    return false;
}
bool CzyMozliwyRuchCzarny(gra *plansza)
{
    for(int i=1;i<POLA;i++)
    {
        if(plansza->czarne[i]>0)
        {
            if(i-plansza->rzut1>0 && plansza->rzut1!=plansza->poprzedniruch &&  plansza->biale[i-plansza->rzut1]<2)
                return true;
            if(i-plansza->rzut2>0  && plansza->rzut2!=plansza->poprzedniruch && plansza->biale[i-plansza->rzut2]<2)
                return true;
        }
    }
    StrataKolejki();
    plansza->posuniecia=0;
    return false;
}
bool CzyMozliwyRuchBialyDublet(gra *plansza)
{
    for(int i=1;i<POLA;i++)
    {
        if(plansza->biale[i]>0)
        {  //jezeli pierwsze pole jest zablokowane to na inne nie ma i tak dostepu oraz rzut1=rzut2
            if(i+(plansza->rzut1)<POLA &&  plansza->czarne[i+plansza->rzut1]<2)
                return true;
        }
    }
    StrataKolejki();
    plansza->posuniecia=0;
    return false;
}
bool CzyMozliwyRuchCzarnyDublet(gra *plansza)
{
    for(int i=1;i<POLA;i++)
    {
        if(plansza->czarne[i]>0)
        {
            if(i-(plansza->rzut1)>0 &&  plansza->biale[i-plansza->rzut1]<2)
                return true;
        }
    }
    StrataKolejki();
    plansza->posuniecia=0;
    return false;
}

//koniec sprawdzania utraty kolejki

void RuchBialyDublet(gra *plansza)
{
    cout<<"\tPodaj pole i o ile chcesz sie ruszyc: ";
    int pole,oczka;
    cin>>pole>>oczka;
    if(CzyLegalRuchBialyDublet(pole,oczka,plansza))
    {
        if(plansza->czarne[pole+oczka]==1)
        {
            plansza->barczarny++;
            plansza->czarne[pole+oczka]--;
            plansza->biale[pole]--;
            plansza->biale[pole+oczka]++;
        }
        else
        {
            plansza->biale[pole]--;
            plansza->biale[pole+oczka]++;
        }
        plansza->posuniecia-=(oczka/plansza->rzut1);
    }
    else
    {
        RuchNiepoprawny();
    }
    wyswietlWszystko(plansza);
}
void RuchCzarnyDublet(gra *plansza)
{
    cout<<"\tPodaj pole i o ile chcesz sie ruszyc: ";
    int pole,oczka;
    cin>>pole>>oczka;
    if(CzyLegalRuchCzarnyDublet(pole,oczka,plansza))
    {
        if(plansza->biale[pole-oczka]==1)
        {
            plansza->barbialy++;
            plansza->biale[pole-oczka]--;
            plansza->czarne[pole]--;
            plansza->czarne[pole-oczka]++;
        }
        else{
            plansza->czarne[pole]--;
            plansza->czarne[pole-oczka]++;
        }
        plansza->posuniecia-=(oczka/plansza->rzut1);
    }
    else
    {
        RuchNiepoprawny();
    }
    wyswietlWszystko(plansza);
}
bool CzyLegalRuchBialyDublet(int pole,int oczka,gra *plan)
{
    if(pole>0 && pole<POLA && plan->biale[pole]>0 && oczka%(plan->rzut1)==0
       && oczka<=(plan->rzut1)*(plan->posuniecia) && pole+oczka<POLA)
    { //sprawdz czy podana liczba oczek sie zgadza z koscmi
        for(int j=pole+(plan->rzut1);j<=pole+oczka;j+=plan->rzut1)
        {
            if(plan->czarne[j]>1)
                return false;
        }
        if(BicieBialyDublet(plan)==BRAKBICIABIALE)
            return true;
        else
        {
            if(pole+oczka==BicieBialyDublet(plan))
                return true;
            else
            {
                cout<<"\tBicie : "<<BicieBialyDublet(plan)<<endl;
                return false;
            }
        }
    }
    return false;
}
bool CzyLegalRuchCzarnyDublet(int pole,int oczka,gra *plan)
{
    if(pole>0 && pole<POLA && plan->czarne[pole]>0 && oczka%(plan->rzut1)==0
       && oczka<=(plan->rzut1)*(plan->posuniecia) && pole-oczka>0)
    {
        for(int j=pole-(plan->rzut1);j>=pole-oczka;j-=plan->rzut1)
        { // spr czy nie blokuja piony
            if(plan->biale[j]>1)
                return false;
        }
        if(BicieCzarnyDublet(plan)==BRAKBICIACZARNE)
          return true;
        else
        { // forced bicie
            if(pole-oczka==BicieCzarnyDublet(plan))
                return true;
            else
            {
                cout<<"\tBicie : "<<BicieCzarnyDublet(plan)<<endl;
                return false;
            }
        }
    }
    return false;
}
int BicieCzarnyDublet(gra *plansza)
{
    int najbicie=BRAKBICIACZARNE;
    for(int i=POLA-1;i>0;i--)
    {
        if(plansza->biale[i]==1)
        {
            int j=1;
            while(j<=plansza->posuniecia)
            { //znajdz pole bialego pionka i spr czy odleglosc jego i ktoregos czarnego jest rowna wielokrotnosci kostki
                if(plansza->biale[i+(plansza->rzut1)*j]>1)
                    break; // jezeli napotkasz na >1 pionka przerwij szukanie
                 if(i+(plansza->rzut1)*j<POLA && plansza->czarne[i+(plansza->rzut1)*j]>0 )
                    najbicie=i; // jezeli znaleziono pionka ktory moze zbic to zapisz indeks "i"
                j++;
            }
        }
        if(najbicie==i)
            break;
    }
    return najbicie;
}
int BicieBialyDublet(gra *plansza)
{
    int najbicie=BRAKBICIABIALE;
    for(int i=1;i<POLA;i++)
    {
        if(plansza->czarne[i]==1)
        {
            int j=1;
            while(j<=plansza->posuniecia)
            {
                if(plansza->czarne[i-(plansza->rzut1)*j]>1)
                    break;
                 if(i-(plansza->rzut1)*j>0 && plansza->biale[i-(plansza->rzut1)*j]>0)
                    najbicie=i;
                j++;
            }
        }
        if(najbicie==i)
            break;
    }
    return najbicie;
}

//koniec dubletow

void ruchBialy(gra *plansza)
{
        cout<<"\tPodaj pole i o ile chcesz sie ruszyc: ";
        int pole,oczka;
        cin>>pole>>oczka;
        if(LegalBialy(pole,oczka,plansza))
        {
            if(plansza->czarne[pole+oczka]==1)//czybicie
            {
                plansza->barczarny++;
                plansza->czarne[pole+oczka]--;
                plansza->biale[pole]--;
                plansza->biale[pole+oczka]++;
            }
            else
            {
                plansza->biale[pole]--;
                plansza->biale[pole+oczka]++;
            }
            plansza->poprzedniruch=oczka;
            if(oczka==plansza->rzut1+plansza->rzut2)
            plansza->posuniecia-=2;
            else plansza->posuniecia--;
        }
        else
        {
            RuchNiepoprawny();
        }
        wyswietlWszystko(plansza);
}
void ruchCzarny(gra *plansza)
{
    cout<<"\tPodaj pole i o ile chcesz sie ruszyc: ";
    int pole,oczka;
    cin>>pole>>oczka;
    if(LegalCzarny(pole,oczka,plansza))
    {
        if(plansza->biale[pole-oczka]==1)//czybicie
        {
            plansza->barbialy++;
            plansza->biale[pole-oczka]--;
            plansza->czarne[pole]--;
            plansza->czarne[pole-oczka]++;
        }
        else
        {
            plansza->czarne[pole]--;
            plansza->czarne[pole-oczka]++;
        }
        plansza->poprzedniruch=oczka;
        if(oczka==plansza->rzut1+plansza->rzut2)
            plansza->posuniecia-=2;
        else plansza->posuniecia--;
    }
    else
    {
        RuchNiepoprawny();
    }
    wyswietlWszystko(plansza);
}
bool LegalBialy(int pole,int oczka, gra *p)
{
    if(pole>0 && pole<POLA && p->biale[pole]>0 && pole+oczka<POLA)
    {
        // warunki ruchu
        if((oczka==p->rzut1 || oczka==p->rzut2) && oczka!=p->poprzedniruch && p->czarne[pole+oczka]<2)
        { //ruch jedna kostka
            if(BijBialy(pole,oczka,p))
                return true;
            return false;
        }
        if(p->poprzedniruch==0 && (oczka==p->rzut1+p->rzut2 &&
        (p->czarne[pole+p->rzut1]<2 || p->czarne[pole+p->rzut2]<2))
           && p->czarne[pole+p->rzut1+p->rzut2]<2)
        {   //ruch dwiema kostkami naraz
            if(BijBialy(pole,oczka,p))
                return true;
            return false;
        }
    }
    return false;
}
bool LegalCzarny(int pole,int oczka,gra *p)
{
    if(pole>0 && pole<POLA && p->czarne[pole]>0 && pole-oczka>0)
    {
        if((oczka==p->rzut1 || oczka==p->rzut2) && oczka!=p->poprzedniruch
                && p->biale[pole-oczka]<2)
        {
            if(BijCzarny(pole,oczka,p))
                return true;
            return false;
        }
        if(p->poprzedniruch==0 && (oczka==p->rzut1+p->rzut2 &&
            (p->biale[pole-p->rzut1]<2 || p->biale[pole-p->rzut2]<2))
           && p->biale[pole-p->rzut1-p->rzut2]<2)
        {
            if(BijCzarny(pole,oczka,p))
                return true;
            return false;
        }
    }
    return false;
}
bool BijBialy(int pole,int oczka,gra *plansza)
{
    if(BicieBialy(plansza)==BRAKBICIABIALE)
        return true;
    else
    {
        if(pole+oczka==BicieBialy(plansza))
            return true;
        else
        {
            cout<<"\tBicie : "<<BicieBialy(plansza)<<endl;
            return false;
        }
    }
    return false;
}
bool BijCzarny(int pole,int oczka,gra *plansza)
{
    if(BicieCzarny(plansza)==BRAKBICIACZARNE)
        return true;
    else
    {
        // forced bicie
        if(pole-oczka==BicieCzarny(plansza))
            return true;
        else
        {
            cout<<"\tBicie : "<<BicieCzarny(plansza)<<endl;
            return false;
        }
    }
    return false;
}
int BicieBialy(gra *p)
{
    int najbicie=BRAKBICIABIALE;
    for(int i=1;i<POLA;i++) //szukaj jaknajblizej domu czarnych
    {
        if(p->czarne[i]==1) //znaleziono potencjalnego do zbicia
        { //czy mozna zbic jedna kostka
            if(i-(p->rzut1)>0 && p->biale[i-(p->rzut1)]>0 && p->rzut1!=p->poprzedniruch)
                najbicie=i;
            if(i-(p->rzut2)>0 && p->biale[i-(p->rzut2)]>0 && p->rzut2!=p->poprzedniruch)
                najbicie=i;
            if(i-(p->rzut1+p->rzut2)>0 && p->biale[i-p->rzut1-p->rzut2]>0 && p->poprzedniruch==0
               &&(p->czarne[i-p->rzut1]<2 || p->czarne[i-p->rzut2]<2)) //czy mozna zbic dwiema kostkami
                najbicie=i;
        }
        if(najbicie==i) //jak znaleziono to to jest najoptymalniejsza pozycja
            break;
    }
    return najbicie;
}
int BicieCzarny(gra *p)
{
    int najbicie=BRAKBICIACZARNE;
    for(int i=POLA-1;i>0;i--)
    {
        if(p->biale[i]==1)
        {
            if(i+(p->rzut1)<POLA && p->czarne[i+(p->rzut1)]>0 && p->rzut1!=p->poprzedniruch)
                najbicie=i;
            if(i+(p->rzut2)<POLA && p->czarne[i+(p->rzut2)]>0 && p->rzut2!=p->poprzedniruch)
                najbicie=i;
            if(i+(p->rzut1+p->rzut2)<POLA && p->czarne[i+p->rzut1+p->rzut2]>0 && p->poprzedniruch==0
               && (p->biale[i+p->rzut1]<2 || p->biale[i+p->rzut2]<2))
                najbicie=i;
        }
        if(najbicie==i)
            break;
    }
    return najbicie;
}

//koniec normalnych ruchow

bool CzyFazaBialy(gra *plansza)
{
    for(int i=1;i<POLA-6;i++) // czy nie ma pionka na planszy poza domem
    {
        if(plansza->biale[i]>0)
            return false;
    }
    if(plansza->barbialy>0) // czy nie ma na barze wtedy splukiwanie pionkow
        return false;
    for(int i=POLA-6;i<POLA;i++)
    { // sprawdz czy da sie wyprowadzic jakiegos pionka
        if(plansza->rzut1!=plansza->poprzedniruch && plansza->rzut1+i>=POLA && plansza->biale[i]>0)
            return true;
        if(plansza->rzut2!=plansza->poprzedniruch && plansza->rzut2+i>=POLA && plansza->biale[i]>0)
            return true;
    }
    return false;
}
bool CzyFazaCzarny(gra *plansza)
{
    for(int i=7;i<POLA;i++)
    {
        if(plansza->czarne[i]>0)
            return false;
    }
    if(plansza->barczarny>0)
        return false;
    for(int i=1;i<7;i++)
    {
        if(plansza->rzut1!=plansza->poprzedniruch && i-plansza->rzut1<=0 && plansza->czarne[i]>0)
            return true;
        if(plansza->rzut2!=plansza->poprzedniruch && i-plansza->rzut2<=0 && plansza->czarne[i]>0)
            return true;
    }
    return false;
}
void WyprowadzBialy(gra *plansza)
{
    cout<<"\tMozliwy ruch jedna kostka, wyprowadz najdalszego piona ";
    int pole,oczka;
    cin>>pole>>oczka;
    if(czyLegalWyprowadzBialy(pole,oczka,plansza))
    {
        plansza->biale[pole]--;
        UstawPoprzedniRuch(&oczka,plansza);
        plansza->posuniecia--;
    }
    else
    {
        RuchNiepoprawny();
    }
        wyswietlWszystko(plansza);
}
void WyprowadzCzarny(gra *plansza)
{
    cout<<"\tMozliwy ruch jedna kostka, wyprowadz najdalszego piona ";
    int pole,oczka;
    cin>>pole>>oczka;
    if(czyLegalWyprowadzCzarny(pole,oczka,plansza))
    {
        plansza->czarne[pole]--;
        UstawPoprzedniRuch(&oczka,plansza);
        plansza->posuniecia--;
    }
    else
    {
        RuchNiepoprawny();
    }
        wyswietlWszystko(plansza);
}
bool czyLegalWyprowadzBialy(int pole,int oczka,gra *plansza)
{
    if((oczka==plansza->rzut1 && plansza->rzut1!=plansza->poprzedniruch && plansza->biale[pole]>0 && plansza->rzut1+pole>=POLA) ||
       (oczka==plansza->rzut2 && plansza->rzut2!=plansza->poprzedniruch && plansza->biale[pole]>0 && plansza->rzut2+pole>=POLA))
    { //sprawdza czy znajdzie sie pionek za plansza ^^
        for(int i=pole-1;i>=POLA-6;i--)
        { //sprawdz czy nie ma optymalniejszego pionka
            if(plansza->biale[i]>0 && i+oczka>=POLA)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
bool czyLegalWyprowadzCzarny(int pole,int oczka,gra *plansza)
{
     if((oczka==plansza->rzut1 && plansza->rzut1!=plansza->poprzedniruch && plansza->czarne[pole]>0 && pole-plansza->rzut1<1) ||
       (oczka==plansza->rzut2 && plansza->rzut2!=plansza->poprzedniruch && plansza->czarne[pole]>0 && pole-plansza->rzut2<1))
    {
        for(int i=pole+1;i<7;i++)
        {
            if(plansza->czarne[i]>0 && i-oczka<1)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
//koniec koncowki gry

void ustawPionki(gra* plansza)
{
    for(int i=0;i<POLA;i++)
    {
        plansza->biale[i]=0;
        plansza->czarne[i]=0;
    }
    plansza->barbialy=0;
    plansza->barczarny=0;
    plansza->biale[1]=2; //pozycja startowa
    plansza->biale[12]=5;
    plansza->biale[17]=3;
    plansza->biale[19]=5;
    plansza->czarne[24]=2;
    plansza->czarne[13]=5;
    plansza->czarne[8]=3;
    plansza->czarne[6]=5;
}
void wyswietlPionki(gra &plansza)
{
    for(int i=1;i<POLA;i++)
    {
        cout<<plansza.biale[i]<<" ";
    }
    cout<<endl;
    for(int i=1;i<POLA;i++)
    {
        cout<<plansza.czarne[i]<<" ";
    }
    cout<<endl;
    cout<<"bialy "<<plansza.barbialy<<" "<<"czarny "<<plansza.barczarny<<endl;
    cout<<"Poprzedni ruch"<<plansza.poprzedniruch<<endl;
}
void tworzPlansze(gra &plansza)
{
    char obraz[PLANSZAY][PLANSZAX];
    for(int i=0;i<PLANSZAY;i++)
    {
        for(int j=0;j<PLANSZAX;j++)
        {
            if(i==0 || i==PLANSZAY-1) //obrysy planszy
                obraz[i][j]=GORA;
            else if(j==0 || j==PLANSZAX-1)
                obraz[i][j]=BOK;
            else obraz[i][j]=' ';
        }
    }
    for(int i=1;i<=ILEMAKS;i++) //pusta gora planszy
    {
        for(int j=1;j<PLANSZAX-1;j++)
        {
            if((j+1)%4==0)
            {
                obraz[i][j]=NIC;
                obraz[i][j+1]=NIC;
            }
        }
    }
    for(int i=PLANSZAY-2;i>=PLANSZAY-ILEMAKS-1;i--) // pusty dol planszy
    {
        for(int j=1;j<PLANSZAX-1;j++)
        {
            if((j+1)%4==0)
            {
                obraz[i][j]=NIC;
                obraz[i][j+1]=NIC;
            }
        }
    }
   wykonaj(plansza,obraz);
}
void wykonaj(gra &plansza,char obraz[PLANSZAY][PLANSZAX])
{
    autor();
    tworzLiczbyGora(plansza,obraz);
    tworzLiczbyDol(plansza,obraz);
    tworzPionkiGoraLewy(plansza,obraz);
    tworzPionkiGoraPrawy(plansza,obraz);
    tworzPionkiDolLewy(plansza,obraz);
    tworzPionkiDolPrawy(plansza,obraz);
    PionkiBar(plansza,obraz);
    indeksyPolGora();
    rysujPlansze(obraz);
    indeksyPolDol();
}
void tworzLiczbyGora(gra &plan,char obraz[PLANSZAY][PLANSZAX])
{
    int pole = LEWYGORNY;
    for(int j=1;j<PLANSZAX/2-3;j++)
    {
        if((j+1)%4==0)
        {
            obraz[ILEMAKS+1][j]=maxi(plan.biale[pole],plan.czarne[pole])/10+'0';
            obraz[ILEMAKS+1][j+1]=maxi(plan.biale[pole],plan.czarne[pole])%10+'0';
            pole++;
        }
    }
    for(int j=PLANSZAX/2+3;j<PLANSZAX-1;j++)
    {
        if((j+1)%4==0)
        {
            obraz[ILEMAKS+1][j]=maxi(plan.biale[pole],plan.czarne[pole])/10+'0';
            obraz[ILEMAKS+1][j+1]=maxi(plan.biale[pole],plan.czarne[pole])%10+'0';
            pole++;
        }
    }
}
void tworzLiczbyDol(gra &plan,char obraz[PLANSZAY][PLANSZAX])
{
    int pole = LEWYDOLNY;
    for(int j=1;j<PLANSZAX/2-3;j++)
    {
        if((j+1)%4==0)
        {
            obraz[PLANSZAY-ILEMAKS-2][j]=maxi(plan.biale[pole],plan.czarne[pole])/10+'0';
            obraz[PLANSZAY-ILEMAKS-2][j+1]=maxi(plan.biale[pole],plan.czarne[pole])%10+'0';
            pole--;
        }
    }
    for(int j=PLANSZAX/2+3;j<PLANSZAX-1;j++)
    {
        if((j+1)%4==0)
        {
            obraz[PLANSZAY-ILEMAKS-2][j]=maxi(plan.biale[pole],plan.czarne[pole])/10+'0';
            obraz[PLANSZAY-ILEMAKS-2][j+1]=maxi(plan.biale[pole],plan.czarne[pole])%10+'0';
            pole--;
        }
    }
}
void tworzPionkiGoraLewy(gra &plansza,char obraz[PLANSZAY][PLANSZAX])
{
    int indeks=LEWYGORNY;
    for(int j=3;j<PLANSZAX/2-3;j+=4)
    {
        char znak;
        int liczba=0,ogranicz=ILEMAKS;
        if(plansza.biale[indeks]!=0)
        {
            znak=BIALY;
            liczba=plansza.biale[indeks];
        }
        else if(plansza.czarne[indeks]!=0)
        {
            znak=CZARNY;
            liczba=plansza.czarne[indeks];
        }
        for(int i=1;i<=liczba && ogranicz--;i++)
        {
            obraz[i][j]=znak;
            obraz[i][j+1]=znak;
        }
        indeks++;
    }

}
void tworzPionkiGoraPrawy(gra &plansza,char obraz[PLANSZAY][PLANSZAX])
{
    int indeks=LEWYGORNY+6;
     for(int j=PLANSZAX/2+5;j<PLANSZAX-1;j+=4)
    {
        char znak;
        int liczba=0,ogranicz=ILEMAKS;
        if(plansza.biale[indeks]!=0)
        {
            znak=BIALY;
            liczba=plansza.biale[indeks];
        }
        else if(plansza.czarne[indeks]!=0)
        {
            znak=CZARNY;
            liczba=plansza.czarne[indeks];
        }
        for(int i=1;i<=liczba && ogranicz--;i++)
        {
            obraz[i][j]=znak;
            obraz[i][j+1]=znak;
        }
        indeks++;
    }
}
void tworzPionkiDolLewy(gra &plansza,char obraz[PLANSZAY][PLANSZAX])
{
    int indeks=LEWYDOLNY;
    for(int j=3;j<PLANSZAX/2-3;j+=4)
    {
        char znak;
        int liczba=0,ogranicz=ILEMAKS;
        if(plansza.biale[indeks]!=0)
        {
            znak=BIALY;
            liczba=plansza.biale[indeks];
        }
        else if(plansza.czarne[indeks]!=0)
        {
            znak=CZARNY;
            liczba=plansza.czarne[indeks];
        }
        for(int i=PLANSZAY-2;i>=PLANSZAY-liczba-1 && ogranicz--;i--)
        {
            obraz[i][j]=znak;
            obraz[i][j+1]=znak;
        }
        indeks--;
    }
}
void tworzPionkiDolPrawy(gra &plansza,char obraz[PLANSZAY][PLANSZAX])
{
    int indeks=LEWYDOLNY-6;
    for(int j=PLANSZAX/2+5;j<PLANSZAX-1;j+=4)
    {
        char znak;
        int liczba=0,ogranicz=ILEMAKS;
        if(plansza.biale[indeks]!=0)
        {
            znak=BIALY;
            liczba=plansza.biale[indeks];
        }
        else if(plansza.czarne[indeks]!=0)
        {
            znak=CZARNY;
            liczba=plansza.czarne[indeks];
        }
        for(int i=PLANSZAY-2;i>=PLANSZAY-liczba-1 && ogranicz--;i--)
        {
            obraz[i][j]=znak;
            obraz[i][j+1]=znak;
        }
        indeks--;
    }
}
void PionkiBar(gra &plansza,char obraz[PLANSZAY][PLANSZAX])
{
    for(int i=1;i<PLANSZAY-1;i++)
    {
        for(int j=PLANSZAX/2-3;j<PLANSZAX/2+3;j++)
        {
            obraz[i][j]=BAR; //tworzenie bara
        }
    }
    for(int i=4;i<4+plansza.barbialy;i++)
    {
        for(int j=PLANSZAX/2-1;j<PLANSZAX/2+1;j++) //dowolna pozycja byle by na barze
        {
            obraz[i][j]=BIALY;
        }
    }
    for(int i=PLANSZAY-7;i<PLANSZAY-7+plansza.barczarny;i++)
    {
        for(int j=PLANSZAX/2-1;j<PLANSZAX/2+1;j++)
        {
            obraz[i][j]=CZARNY;
        }
    }
}
void indeksyPolGora()
{
    cout<<"\t   ";
    for(int i=LEWYGORNY;i<=LEWYGORNY+5;i++)
    {
        cout<<i<<"  ";
    }
    cout<<"\t   ";
    for(int i=LEWYGORNY+6;i<=LEWYGORNY+11;i++)
    {
        cout<<i<<"  ";
    }
    cout<<endl;
}
void indeksyPolDol()
{
    cout<<"\t   ";
    for(int i=LEWYDOLNY;i>=LEWYDOLNY-5;i--)
    {
        if(i>=10)
        cout<<i<<"  ";
        else  cout<<"0"<<i<<"  ";
    }
    cout<<"\t   ";
    for(int i=LEWYDOLNY-6;i>=LEWYDOLNY-11;i--)
    {
        cout<<"0"<<i<<"  ";
    }
    cout<<endl;
}
void rysujPlansze(char obraz[PLANSZAY][PLANSZAX])
{
    for(int i=0;i<PLANSZAY;i++)
    {
        cout<<"\t";
        for(int j=0;j<PLANSZAX;j++)
        {
            cout<<obraz[i][j];
        }
        cout<<endl;
    }
}
void RuchNiepoprawny()
{
    cout<<"\tRuch niepoprawny. Wpisz znak by kontynuowac. ";
    char a;
    cin>>a;
}
void StrataKolejki()
{
    cout<<"\tTracisz kolejke- brak ruchu. Wpisz znak by kontynuowac. ";
    char a;
    cin>>a;
}
void WymuszoneBicie()
{
    cout<<"\tWymuszone bicie. Wpisz znak by kontynuowac. ";
    char a;
    cin>>a;
}
void NieDziala()
{
    cout<<"Nie otwiera sie plik"<<endl;
    exit(0);
}
void czyscGre()
{
    FILE *gra;
    gra=fopen("gra.txt","w"); // wywolywane do wyczyszczenia poprzedniej gry
    if(gra==NULL)
    {
        NieDziala();
    }
    fclose(gra);
}
void wczytajPlansze(gra *plansza)
{
    FILE *wczyt;
    wczyt=fopen("plansza.txt","r");
    if(wczyt==NULL)
    {
        NieDziala();
    }
    for(int i=1;i<POLA;i++)
    {
        fscanf(wczyt,"%d ",&(plansza->biale[i]));
    }
    for(int i=1;i<POLA;i++)
    {
        fscanf(wczyt,"%d ",&(plansza->czarne[i]));
    }
    fscanf(wczyt,"%d %d %d %d %d %d %d",&(plansza->barbialy),&(plansza->barczarny),&(plansza->rzut1),
            &(plansza->rzut2),&(plansza->gracz),&(plansza->poprzedniruch),&(plansza->posuniecia));
    fclose(wczyt);
}
void zapiszPlansze(gra *plansza)
{
    FILE *poz;
    poz=fopen("plansza.txt","w");
    if(poz==NULL)
    {
        NieDziala();
    }
    for(int i=1;i<POLA;i++)
    {
        fprintf(poz,"%d ",plansza->biale[i]);
    }
    for(int i=1;i<POLA;i++)
    {
        fprintf(poz,"%d ",plansza->czarne[i]);
    }
    fprintf(poz,"%d %d %d %d %d %d %d",plansza->barbialy,plansza->barczarny,plansza->rzut1,
            plansza->rzut2,plansza->gracz,plansza->poprzedniruch,plansza->posuniecia);
    fclose(poz);
}
void wczytajGre()
{
    gra play[400];
    FILE *gra;
    gra=fopen("gra.txt","r");
    if(gra==NULL)
    {
        NieDziala();
    }
    int j=1; //numer planszy
    while(fscanf(gra,"%d",&(play[j].biale[1]))==1)
    {
        for(int i=2; i<POLA; i++)
        {
            fscanf(gra,"%d",&(play[j].biale[i]));
        }
        for(int i=1; i<POLA; i++)
        {
            fscanf(gra,"%d",&(play[j].czarne[i]));
        }
        fscanf(gra,"%d %d %d %d %d %d %d",&play[j].barbialy,&play[j].barczarny,&play[j].rzut1,
            &play[j].rzut2,&play[j].gracz,&play[j].poprzedniruch,&play[j].posuniecia);
        j++;
    }
      fclose(gra);
        wyswietlWszystko(&play[1]);
         wyborPlanszy(play,j);
}
void zapiszGre(gra *plansza)
{
    FILE *gra;
    gra=fopen("gra.txt","a");
    if(gra==NULL)
    {
       NieDziala();
    }
    for(int i=1;i<POLA;i++)
    {
        fprintf(gra,"%d ",plansza->biale[i]);
    }
    for(int i=1;i<POLA;i++)
    {
        fprintf(gra,"%d ",plansza->czarne[i]);
    }
    fprintf(gra,"%d %d %d %d %d %d %d\n",plansza->barbialy,plansza->barczarny,plansza->rzut1,
            plansza->rzut2,plansza->gracz,plansza->poprzedniruch,plansza->posuniecia);
    fclose(gra);
}
void wyborPlanszy(gra *play,int ile)
{
    cout<<"\tPodaj numer planszy. Ostatnia partia ma numer "<<ile-1<<". Uzyj 0 zeby zagrac. ";
    int numer=1,poprzednia=1;
    while(1)
    {
        poprzednia=numer;
        cin>>numer;
        if(numer==0)
            break;
        wyswietlWszystko(&play[numer]);
        cout<<"\tPodaj numer planszy. Ostatnia partia ma numer "<<ile-1<<". Uzyj 0 zeby zagrac. ";
    }
    CZYSCEKRAN;
    grajZWczytu(&play[poprzednia]);
}
void wyswietlRanking()
{
    FILE *plik;
    plik=fopen("ranking.txt","r");
    if(plik==NULL)
    {
        NieDziala();
    }
    gracz osoba[30];
    int ile=0;
    while(fscanf(plik,"%s %d",osoba[ile].imie,&(osoba[ile].punkty))==2)
    {
        ile++;
    }
    sortujRanking(osoba,ile);
    for(int i=0;i<ile;i++)
    {
        cout<<osoba[i].imie<<" "<<osoba[i].punkty<<endl;
    }
    fclose(plik);
}
void sortujRanking(gracz *osoba,int ile)
{
    for(int i=0;i<ile;i++)
    {
        for(int j=i+1;j<ile;j++)
        {
            if(osoba[i].punkty<osoba[j].punkty)
            {
                int temp;
                char pom[30];
                temp=osoba[i].punkty;
                osoba[i].punkty=osoba[j].punkty;
                osoba[j].punkty=temp;
                strcpy(pom,osoba[i].imie);
                strcpy(osoba[i].imie,osoba[j].imie);
                strcpy(osoba[j].imie,pom);
            }
        }
    }
}
int LiczPunkty(int bar,int tab[])
{
    if(bar>0) return 3;
    else if(ilepionkowjednego(tab)==15) return 2;
    return 1;
}
void Zwyciestwo(gra* plansza)
{
    if(ilepionkowjednego(plansza->biale)==0)
    {
        cout<<"\tWygraly biale"<<endl;
        int pkt=LiczPunkty(plansza->barczarny,plansza->czarne);
        cout<<"\tZdobyte punkty: "<<pkt<<endl;
        DodajNaRanking(pkt);
    }
    if(ilepionkowjednego(plansza->czarne)==0)
    {
        cout<<"\tWygraly czarne"<<endl;
        int pkt=LiczPunkty(plansza->barbialy,plansza->biale);
        cout<<"\tZdobyte punkty: "<<pkt<<endl;
        DodajNaRanking(pkt);
    }
}
void DodajNaRanking(int pkt)
{
    cout<<"\tPodaj imie: ";
    char nazwa[30];
    cin>>nazwa;
    FILE *plik;
    plik=fopen("ranking.txt","r");
    if(plik==NULL)
    {
        NieDziala();
    }
    gracz osoba[30];
    int ile=0;
    bool ok=0;
    while(fscanf(plik,"%s %d",osoba[ile].imie,&(osoba[ile].punkty))==2)
    {
        if(strcmp(osoba[ile].imie,nazwa)==0)
        {
            osoba[ile].punkty+=pkt;
            ok=1;
        }
        ile++;
    }
    if(ok==0)
    {
        strcpy(osoba[ile].imie,nazwa);
        osoba[ile].punkty=pkt;
        ile++;
    }
    fclose(plik);
    ZapisRanking(osoba,ile);
}
void ZapisRanking(gracz *osoba,int ile)
{
    FILE *zapis;
    zapis=fopen("ranking.txt","w");
    if(zapis==NULL)
    {
        NieDziala();
    }
    for(int i=0;i<ile;i++)
    {
        fprintf(zapis,"%s %d\n",osoba[i].imie,osoba[i].punkty);
    }
    fclose(zapis);
}

