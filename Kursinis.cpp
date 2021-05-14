#include <iostream>
#include <deque> // deque -  dinaminių dydžių sekos talpyklos, kurias galima išplėsti arba sutraukti abiejuose galuose (priekyje arba gale).
#include <vector>
#include <conio.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <iomanip>
#include <map>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

void Izanga();
void Pradzia();
void MusioPradziosEkranas();
void MusioPergalesEkranas();
void MusioPralaimejimoEkranas();
void PraeitosGatvesEkranas();
void Laimejimas();
int Pabaiga();
void setConsoleColor(WORD c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Zaidejas
{
public:
    int x, y;
    Zaidejas(int plotis) { x = plotis / 2; y = 0; }
};
class Juosta
{
private:
    deque<bool> masinos;  // deque -  jie leidžia tiesiogiai pasiekti atskirus elementus per atsitiktinės prieigos iteratorius, o saugykla automatiškai tvarkoma, praplečiant ir sudarant konteinerį, jei reikia.
    bool kryptisDesine;
public:
    Juosta(int plotis)
    {
        for (int i = 0; i < plotis; i++)
            masinos.push_front(true);
        kryptisDesine = rand() % 2;
    }
    void Judeti()
    {
        if (kryptisDesine)
        {
            if (rand() % 10 == 1)
                masinos.push_front(true);
            else
                masinos.push_front(false);
            masinos.pop_back();
        }
        else
        {
            if (rand() % 10 == 1)
                masinos.push_back(true);
            else
                masinos.push_back(false);
            masinos.pop_front();
        }

    }
    bool Pozicija(int pos) { return masinos[pos]; }
};
class Zaidimas
{
private:
    bool baigti, arLaimejo;
    int juostuSkaicius;
    int plotis;
    Zaidejas* zaidejas;
    vector<Juosta*> arena;
public:
    Zaidimas(int w, int h)
    {
        juostuSkaicius = h;
        plotis = w;
        baigti = false;
        for (int i = 0; i < juostuSkaicius; i++)
            arena.push_back(new Juosta(plotis));
        zaidejas = new Zaidejas(plotis);
    }
    ~Zaidimas()
    {
        delete zaidejas;
        for (int i = 0; i < arena.size(); i++)
        {
            Juosta* current = arena.back();
            arena.pop_back();
            delete current;
        }
    }
    void Piesimas()
    {
        system("cls");
        for (int i = 0; i < plotis; i++)
            cout << '_';
        cout << endl;
        for (int i = 0; i < juostuSkaicius-1; i++)
        {
            for (int j = 0; j < plotis; j++)
            {
                if (arena[i]->Pozicija(j) && i != 0 && i != juostuSkaicius - 1)
                {
                    setConsoleColor(FOREGROUND_RED);
                    cout << "O";
                    setConsoleColor(7);
                }
                    
                else if (zaidejas->x == j && zaidejas->y == i)
                {
                    setConsoleColor(FOREGROUND_GREEN);
                    cout << "+";
                    setConsoleColor(7);
                }
                else
                    cout << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < plotis; i++)
            cout << '_';
        cout << endl;
        
    }
    void Ivestis()
    {
        if (_kbhit())
        {
            char current = _getch();
            if ((current == 'a' || current == 75) && zaidejas->x!=0)
                zaidejas->x--;
            if ((current == 'd' || current == 77) && zaidejas->x != plotis-2)
                zaidejas->x++;
            if ((current == 'w' || current == 72) && zaidejas->y != 0)
                zaidejas->y--;
            if ((current == 's' || current == 80))
                zaidejas->y++;
            if (current == 'q')
                baigti = true;
        }
    }
    void Veikimas()
    {
        for (int i = 1; i < juostuSkaicius - 1; i++)
        {
            if (rand() % 5 == 1)
                arena[i]->Judeti();
            if (arena[i]->Pozicija(zaidejas->x) && zaidejas->y == i)
            {
                baigti = true;
                arLaimejo = false;
            }

        }
        if (zaidejas->y == juostuSkaicius - 1)
        {
            baigti = true;
            arLaimejo = true;
        }
    }
    int Paleidimas()
    {
        while (!baigti)
        {
            Ivestis();
            Piesimas();
            Veikimas();
        }
        if (arLaimejo)
            return 1;
        else
            return 0;
    }
};

class Mustynes
{
protected:
    int manoGyvybes;
    int priesoGyvybes;
    int specialEnergija;
    int marozEnergija;
    int veiksmas;
    bool musames = true, pergale;
    bool priesasPasigyde, nesigydziau;
    map<int, string> tekstas;
    string priesoReakcijos[10];

public:
    Mustynes() :manoGyvybes(0), priesoGyvybes(0), specialEnergija(0), marozEnergija(0) {}
    Mustynes(int x, int z, int y, int v) :manoGyvybes(x), priesoGyvybes(z), specialEnergija(y), marozEnergija(v) {}
    ~Mustynes() {}

    void operator !()
    {
        manoGyvybes = 100;
        priesoGyvybes = 100;
        specialEnergija = 0;
        marozEnergija = 0;
        priesasPasigyde = false;
        nesigydziau = true;
    }
    void operator ++()
    {
        int i = 0;
        string eilute;
        ifstream fd("file.txt");
        while (getline(fd, eilute)) 
        {
            tekstas[i] = eilute;
            i++;
        }
    }

    bool getMusames()
    {return musames;}

    bool getPergale()
    {return pergale;}

    int getPasigydziau()
    {
        if (nesigydziau)
            return 1;
        else 
            return 0;
    }

    int getManoGyvybes()
    {return manoGyvybes;}

    int getKompoGyvybes()
    {return priesoGyvybes;}

    int getManoEnergija()
    {return specialEnergija;}

    int getKompoEnergija()
    {return marozEnergija;}

    void arLaimejau()
    {
        if (priesoGyvybes <= 0)
        {
            pergale = true;
            musames = false;
            Isvedimas(6);
            setConsoleColor(FOREGROUND_GREEN);
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      " << setw(70) << "|   PRIESUI BAIGESI GYVYBES. PRIESAS BUVO NUGALETAS : )" << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n\n\n";
            system("pause");
        }
        else if (manoGyvybes <= 0)
        {
            pergale = false;
            musames = false;
            Isvedimas(7);
            setConsoleColor(FOREGROUND_RED);
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      " << setw(70) << "|   JUMS BAIGESI GYVYBES. PRIESAS JUS NUGALEJO : (" << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n\n\n";
            system("pause");
        }

    }
    void PriesoReakcijos()
    {
        string s;
        ifstream fE("priesas.txt");
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                getline(fE, s);
                priesoReakcijos[i] += s;
                priesoReakcijos[i] += '\n';
            }
        }
        fE.close();
    }
    void Isvedimas(int x)
    {
        system("cls");
        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout<<"-----------------------------------------------------------------------" << endl;
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << left << setw(20) << "|Prieso gyvybes : " << setw(3) << priesoGyvybes << "hp                                             |\n";
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << left << setw(20) << "|Prieso energija : " << setw(3) << marozEnergija << "pp                                             |\n";
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout<< "-----------------------------------------------------------------------" << endl << endl;
        
        setConsoleColor(7);
        cout << priesoReakcijos[x] << endl;

        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << "-----------------------------------------------------------------------" << endl;
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << left << setw(20) << "|Jusu gyvybes : " << setw(3) << manoGyvybes << "hp                                             |\n";
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_GREEN | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << left << setw(20) << "|Jusu energija : " << setw(3) << specialEnergija << "pp                                             |\n";
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << "|1 - paprasta ataka      2 - speciali ataka(20pp)      3 - pasigyti(" << nesigydziau << ")|\n";
        setConsoleColor(7);
        cout << "      ";
        setConsoleColor(FOREGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        cout << "-----------------------------------------------------------------------" << endl << endl;
        setConsoleColor(7);
        
    }
};

class Museikos :public Mustynes
{
public:
    void ManoVeiksmas()
    {
        int randomas;
        int puolimas;
        int pasigydymas;
        int x = 1;
        bool vesti = true;
        cout << setw(6) << ' ' << "Iveskite norimo ejimo skaiciu: ";
        cin >> veiksmas;
        while (x == 1)
        {

            while (veiksmas != 1 && veiksmas != 2 && veiksmas != 3)
            {
                cout << "\n      -----------------------------------------------------------------------" << endl;
                cout << "      "<<setw(70)<<"|   Tokio pasirinkimo nera, bandykite vel." << "|" <<endl ;
                cout << "      -----------------------------------------------------------------------\n\n\n";
                system("pause");
                Isvedimas(8);
                cout << setw(6) << ' ' << "Iveskite norimo ejimo skaiciu: ";
                cin >> veiksmas;
            }

            if (veiksmas == 3)
            {
                if (!nesigydziau)
                {
                    Isvedimas(8);
                    cout << "\n      -----------------------------------------------------------------------" << endl;
                    cout << "      " << setw(70) << "|   Jus jau pasigydete. Bandykite kita veiksma." << "|" << endl;
                    cout << "      -----------------------------------------------------------------------\n\n\n";
                    system("pause");
                    while (veiksmas != 1 && veiksmas != 2)
                    {
                        Isvedimas(8);
                        cout << setw(6) << ' ' << "Iveskite norimo ejimo skaiciu: ";
                        cin >> veiksmas;
                        if (veiksmas != 2 && veiksmas != 1)
                        {
                            Isvedimas(8);
                            cout << "\n      -----------------------------------------------------------------------" << endl;
                            cout << "      " << setw(70) << "|   Tokio pasirinkimo nera, bandykite vel." << "|" << endl;
                            cout << "      -----------------------------------------------------------------------\n\n\n";
                            cout << "        \n";
                        }

                    }
                }
                else
                {
                    Isvedimas(2);
                    randomas = rand() % 3 + 12;
                    setConsoleColor(11);
                    cout << "\n      -----------------------------------------------------------------------" << endl;
                    cout << "      | " << setw(68) << tekstas[randomas] << "|" << endl;
                    cout << "      -----------------------------------------------------------------------\n\n\n";
                    setConsoleColor(7);
                    system("pause");
                    if (manoGyvybes <= 80)
                        manoGyvybes += 20;
                    else manoGyvybes = 100;
                    nesigydziau = false;
                    x = 0;
                }
            }

            if (veiksmas == 1)
            {
                puolimas = rand() % 15 + 5;
                priesoGyvybes -= puolimas;
                specialEnergija += puolimas;
                Isvedimas(0);
                randomas = rand() % 3;
                setConsoleColor(11);
                cout << "\n      -----------------------------------------------------------------------" << endl;
                cout << "      | " << setw(65) << tekstas[randomas] << setw(3) << puolimas << "|" << endl;
                cout << "      -----------------------------------------------------------------------\n\n\n";
                setConsoleColor(7);
                system("pause");
                x = 0;
            }
            else if (veiksmas == 2)
            {
                if (specialEnergija >= 20)
                {
                    puolimas = rand() % 30 + 10;
                    priesoGyvybes -= puolimas;
                    specialEnergija -= 20;
                    Isvedimas(1);
                    randomas = rand() % 3 + 6;
                    setConsoleColor(11);
                    cout << "\n      -----------------------------------------------------------------------" << endl;
                    cout << "      | " << setw(65) << tekstas[randomas] << setw(3) << puolimas << "|" << endl;
                    cout << "      -----------------------------------------------------------------------\n\n\n";
                    setConsoleColor(7);
                    system("pause");
                    x = 0;
                }
                else
                {
                    Isvedimas(8);
                    cout << "\n      -----------------------------------------------------------------------" << endl;
                    cout << "      " << setw(70) << "|   Neturite tam energijos. Bandykite kita veiksma." << "|" << endl;
                    cout << "      -----------------------------------------------------------------------\n\n\n";
                    system("pause");
                    while (veiksmas != 1 && veiksmas != 3)
                    {
                        Isvedimas(8);
                        cout << setw(6) << ' ' << "Iveskite norimo ejimo skaiciu: ";
                        cin >> veiksmas;
                        if (veiksmas != 3 && veiksmas != 1)
                        {
                            Isvedimas(8);
                            cout << "\n      -----------------------------------------------------------------------" << endl;
                            cout << "      " << setw(70) << "|   Pasirinkimas netinkamas. Bandykite is naujo." << "|" << endl;
                            cout << "      -----------------------------------------------------------------------\n\n\n";
                            system("pause");
                        }
                    }

                }

            }
        }    
        if (priesoGyvybes <= 0)
            arLaimejau();
    }
    void KompoVeiksmas()
    {
        int randomas;
        int puolimas;
        if (priesoGyvybes <= 40 && !priesasPasigyde)
        {
            randomas = rand() % 3 + 15;
            Isvedimas(5);
            setConsoleColor(12);
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      | " << setw(68) << tekstas[randomas] << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n\n\n";
            setConsoleColor(7);
            priesoGyvybes += 20;
            priesasPasigyde = true;
            system("pause");
        }
        else if (marozEnergija >= 20)
        {
            puolimas = rand() % 30 + 10;
            manoGyvybes -= puolimas;
            marozEnergija -= 20;
            randomas = rand() % 3 + 9;
            Isvedimas(4);
            setConsoleColor(12);
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      | " << setw(65) << tekstas[randomas] << setw(3) << puolimas << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n\n\n";
            setConsoleColor(7);
            system("pause");
        }
        else 
        {
            puolimas = rand() % 15 + 1;
            manoGyvybes -= puolimas;
            marozEnergija += puolimas;
            randomas = rand() % 3 + 3;
            Isvedimas(3);
            setConsoleColor(12);
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      | " << setw(65) << tekstas[randomas] << setw(3) << puolimas<< "|" << endl;
            cout << "      -----------------------------------------------------------------------\n\n\n";
            setConsoleColor(7);
            system("pause");
        }
       if (manoGyvybes <= 0)
           arLaimejau();
    }
};


int main()
{      
    Izanga();
    bool zaisti = 1;
    while (zaisti)
    {
        srand(time(NULL));
        int n = 0;
        bool pergale;
        Pradzia();
        while (n != 1 && n != 2 && n != 3 && n != 4 && n != 5)
        {
            cin >> n;
            if (n != 1 && n != 2 && n != 3 && n != 4 && n != 5)
            {
                system("cls");
                cout << "\n      -----------------------------------------------------------------------" << endl;
                cout << "      " << setw(70) << "|   Pasirinkimas netinkamas. Bandykite is naujo." << "|" << endl;
                cout << "      -----------------------------------------------------------------------\n";
                Pradzia();
            }
            else if (n == 1 || n == 2 || n == 3 || n == 4 || n == 5)
                break;
        }

        if (n == 1)
        {
            Zaidimas zaidimas(25, 6);
            pergale = zaidimas.Paleidimas();
        }
        else if (n == 2)
        {
            Zaidimas zaidimas(25, 12);
            pergale = zaidimas.Paleidimas();
        }
        else if (n == 3)
        {
            Zaidimas zaidimas(20, 20);
            pergale = zaidimas.Paleidimas();
        }
        else if (n == 4)
        {
            Zaidimas zaidimas(15, 27);
            pergale = zaidimas.Paleidimas();
        }
        else if (n == 5)
        {
            int a, b;
            system("cls");
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      " << setw(70) << "|   PASIRINKITE ARENOS PLOTI:" << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n";
            cout << "      " << "    Plotis - ";
            cin >> a;
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      " << setw(70) << "|   PASIRINKITE ARENOS ILGI:" << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n";
            cout << "      " << "    Ilgis - ";
            cin >> b;
            a += 2; b += 2;
            Zaidimas zaidimas(a, b);
            pergale = zaidimas.Paleidimas();
        }

        if (!pergale)
        {
            MusioPradziosEkranas();
            Museikos m;
            !m;
            ++m;
            m.PriesoReakcijos();
            while (m.getMusames())
            {
                m.Isvedimas(8);
                m.ManoVeiksmas();
                if (!m.getMusames())
                    break;
                m.KompoVeiksmas();
                if (!m.getMusames())
                    break;
                m.arLaimejau();
            }
            if (m.getPergale())
            {
                MusioPergalesEkranas();
                Laimejimas();
                if (Pabaiga())
                    return 0;
            }
            else 
            {
                MusioPralaimejimoEkranas();
                if (Pabaiga())
                    return 0;
            }
        }
        else
        {
            PraeitosGatvesEkranas();
            if (Pabaiga())
                return 0;
        }
    }
    system("pause");
    return 0;
}

void Izanga()
{
    cout << "\n      -----------------------------------------------------------------------" << endl;
    cout << "      " << left <<setw(70) << "|   IVESKITE SAVO VARDA:" << "|" << endl;
    cout << "      -----------------------------------------------------------------------\n      ";
    string vardas;
    cin >> vardas;
    system("cls");
    cout << "\n" << setw(6) << ' ' << setw(20) << "-----------------------------------------------------------------------\n";
    cout << setw(6) << " " << "| LABA, " << left << setw(62)<<vardas<<  "|\n";
    cout << setw(6) << " " << left << setw(70) << "| SIAME ZAIDIME - JUS PAPRASTAS ZMOGELIS, " << "|\n";
    cout << setw(6) << " " << left << setw(70) << "| KURIS PO SUNKIOS DIENOS DARBE PAJUDA NAMO." << "|\n";
    cout << setw(6) << " " << left << setw(70) << "| NUSPRENDUS EITI TRUMPESNIU KELIU, SUSIDURETE SU MAROZU PILNA GATVE." << "|\n";
    cout << setw(6) << " " << left << setw(70) << "| JUSU UZDUOTIS - SEKMINGAI PRAEITI GATVE IR PASIEKTI NAMUS." << "|\n";
    cout << setw(6) << " " << left << setw(70) << "| JEIGU IR SUSIDURSITE SU MAROZU - TEKS KOVOTI!" << "|\n";
    cout << setw(6) << " " << left << setw(70) << "| SEKMES! : )" << "|\n";
    cout << setw(6) << ' ' << setw(20) << "-----------------------------------------------------------------------\n\n\n\n\n";
    system("pause");
    system("cls");
}

void Pradzia()
{
    cout << "\n\n      ";
    setConsoleColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
    cout<< "------------------------------------------\n";
    setConsoleColor(7);
    cout << "      ";
    setConsoleColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
    cout << left << setw(41) << "| PASIRINKITE ARENOS SUDETINGUMA:" << "|\n";
    setConsoleColor(7);
    cout << "      ";
    setConsoleColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
    cout<< "------------------------------------------\n";
    setConsoleColor(10);
    cout << "      ------------------------------------------\n";
    cout << "      " << left << setw(38) << "| LENGVAS "<<setw(3)<<"1";
    cout << "|\n      ------------------------------------------\n";
    setConsoleColor(11);
    cout << "      ------------------------------------------\n";
    cout << "      " << left << setw(38) << "| VIDUTINIS " << setw(3) << "2";
    cout << "|\n      ------------------------------------------\n";
    setConsoleColor(6);
    cout << "      ------------------------------------------\n";
    cout << "      " << left << setw(38) << "| SUNKUS " << setw(3) << "3";
    cout << "|\n      ------------------------------------------\n";
    setConsoleColor(4);
    cout << "      ------------------------------------------\n";
    cout << "      " << left << setw(38) << "| TRAGISKAS " << setw(3) << "4";
    cout << "|\n      ------------------------------------------\n";
    setConsoleColor(13);
    cout << "      ------------------------------------------\n";
    cout << "      " << left << setw(38) << "| PASIRINKITE PATS " << setw(3) << "5";
    cout << "|\n      ------------------------------------------\n\n";
    setConsoleColor(7);

    cout << setw(6) << ' ' << "Iveskite norimo sunkumo skaiciu: ";
}

void MusioPradziosEkranas()
{
    system("cls");
    setConsoleColor(4);
    cout << "\n\n\n" << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n";
    cout << setw(15) << " " << left << setw(53) << "| !!ATSITRENKET I AGRESYVU MAROZA!!" << "|\n";
    cout << setw(15) << " " << left << setw(53) << "| MAROZAS:" << "|\n";
    cout << setw(15) << " " << left << setw(53) << "| - E, TU, DUOK CIGARETE >: )" << "|\n";
    cout << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n\n\n";
    setConsoleColor(7);
    string s;
    ifstream f("emojis.txt");
    while (getline(f, s))
        cout << "        " << s << endl;
    f.close();
    system("pause");
    system("cls");
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
    cout << "\n\n\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
    cout << "\n\n                                          !!!!!        APGINKITE SAVO CIGARETES IR GARBE!        !!!!!                                                                  \n\n";
    cout << "\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n\n";
    setConsoleColor(7);
    system("pause");
    system("cls");
}

void MusioPralaimejimoEkranas()
{
    system("cls");
    setConsoleColor(FOREGROUND_RED);
    cout << "\n\n\n" << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n";
    cout << setw(15) << " " << left << setw(53) << "| ): PRALAIMEJOTE :(" << "|\n";
    cout << setw(15) << " " << left << setw(53) << "| UZUOJAUTA, JUSU CIGARETES BUVO ATIMTOS.." << "|\n";
    cout << setw(15) << " " << left << setw(53) << "| SEKMES KITA KARTA!" << "|\n";
    cout << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n\n\n";
    setConsoleColor(7);
    string s;
    ifstream f2("pralaimejimas.txt");
    while (getline(f2, s))
        cout << "        " << s << endl;
    system("pause");
    f2.close();
}

void MusioPergalesEkranas()
{
    system("cls");
    cout << "\x07"; // garsas, kai pasieki galą
    setConsoleColor(FOREGROUND_GREEN);
    cout << "\n\n\n" << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n";
    cout << setw(15) << " " << left << setw(53) << "| SEKMINGAI IVEIKETE PRIESA!" << "|\n";
    cout << setw(15) << " " << left << setw(53) << "| (: SVEIKINIMAI :)" << "|\n";
    cout << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n\n\n";
    setConsoleColor(7);
    string s;
    ifstream f1("pergale.txt");
    while (getline(f1, s))
        cout << "        " << s << endl;
    cout << "\n\n\n\n";
    system("pause");
    f1.close();
}

void PraeitosGatvesEkranas()
{
    system("cls");
    cout << "\x07"; // garsas, kai pasieki galą
    setConsoleColor(FOREGROUND_GREEN);
    cout << "\n\n\n" << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n";
    cout << setw(15) << " " << left << setw(53) << "| SEKMINGAI PRAEJOTE VISUS ZMONES!" << "|\n";
    cout << setw(15) << " " << left << setw(53) << "| (: SVEIKINIMAI :)" << "|\n";
    cout << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n\n\n";
    setConsoleColor(7);
    ifstream f1("pergale.txt");
    string s;
    while (getline(f1, s))
        cout << "        " << s << endl;
    cout << "\n\n\n\n";
    system("pause");
    f1.close();
    
}

void Laimejimas()
{
    system("cls");
    ifstream fG("gatve.txt");
    string s;
    for (int i = 0; i < 13; i++)
    {
        system("cls");
        setConsoleColor(FOREGROUND_GREEN);
        cout << "\n\n\n" << setw(15) << ' ' << setw(20) << "---------------------------------------------------------\n";
        cout << setw(15) << " " << left << setw(57) << "| SEKMINGAI IVEIKUS GATVES MAROZA," << "|\n";
        cout << setw(15) << " " << left << setw(57) << "| VISI KITI MAROZAI, NENOREDAMI PATYS PASIDARYTI GEDOS," << "|\n";
        cout << setw(15) << " " << left << setw(57) << "| PADARE TARPA JUMS PRAEITI." << "|\n";
        cout << setw(15) << ' ' << setw(20) << "---------------------------------------------------------\n\n\n";
        setConsoleColor(7);
        for (int j = 0; j < 14; j++)
        {
            getline(fG, s);
            cout << "                   " << s << endl;
        }
        if(i != 12)
            this_thread::sleep_for(std::chrono::nanoseconds(999999999));
    }
    cout << "\n\n\n\n";
    system("pause");
    fG.close();
}

int Pabaiga()
{
    system("cls");
    cout << "\n\n\n" << setw(15) << ' ';
    setConsoleColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
    cout << setw(20) << "-----------------------------------------------------\n";
    setConsoleColor(7);
    cout << setw(15) << " ";
    setConsoleColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); 
    cout << left << setw(52) << "| ZAISTI IS NAUJO?"<<"|\n";
    setConsoleColor(7);
    cout << setw(15) << ' ';
    setConsoleColor(BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE); 
    cout<< setw(20) << "-----------------------------------------------------";
    setConsoleColor(FOREGROUND_GREEN);
    cout << "\n" << setw(15) << ' ' << setw(20) << "-----------------------------------------------------\n";
    cout << setw(15) << " " << left << setw(50) << "| TAIP" <<setw(3)<<"1 |\n";
    cout << setw(15) << ' ' << setw(20) << "-----------------------------------------------------";
    setConsoleColor(FOREGROUND_RED);
    cout << "\n" << setw(15) << ' ' << setw(20) << "-----------------------------------------------------\n";
    cout << setw(15) << " " << left << setw(50) << "| NE" << setw(3) << "0 |\n";
    cout << setw(15) << ' ' << setw(20) << "-----------------------------------------------------\n\n\n\n";
    setConsoleColor(7);
    int y = 2;
    while (y != 0 && y != 1)
    {
        cin >> y;
        if (y == 0)
        {
            system("cls");
            cout << "\n\n\n\n" << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n";
            cout << setw(15) << " " << left << setw(53) << "| ACIU, KAD ZAIDETE C :" << "|\n";
            cout << setw(15) << " " << left << setw(53) << "| GRAZIOS DIENOS!" << "|\n";
            cout << setw(15) << ' ' << setw(20) << "------------------------------------------------------\n\n\n\n";
            string s;
            ifstream fP("pabaiga.txt");
            setConsoleColor(FOREGROUND_BLUE);
            while (getline(fP, s))
                cout << "        " << s << endl;
            fP.close();
            setConsoleColor(7);
            cout << "\n\n\n\n\n\n";
            return 1;
        }

        if (y != 0 && y != 1)
        {
            system("cls");
            cout << "\n      -----------------------------------------------------------------------" << endl;
            cout << "      " << setw(70) << "|   Pasirinkimas netinkamas. Bandykite is naujo." << "|" << endl;
            cout << "      -----------------------------------------------------------------------\n";
            cout << "\n" << setw(15) << ' ' << setw(20) << "-----------------------------------------------------\n";
            cout << setw(15) << " " << left << setw(50) << "| TAIP" << setw(3) << "1 |\n";
            cout << setw(15) << ' ' << setw(20) << "-----------------------------------------------------";
            cout << "\n" << setw(15) << ' ' << setw(20) << "-----------------------------------------------------\n";
            cout << setw(15) << " " << left << setw(50) << "| NE" << setw(3) << "0 |\n";
            cout << setw(15) << ' ' << setw(20) << "-----------------------------------------------------\n\n\n\n";
            system("pause");
        }
        if (y == 1)
        {
            system("cls");
            return 0;
        }
            
    }
}

// push_front(), push_back() yra integruota funkcija C ++ STL, naudojama elementui įterpti sąrašo konteinerio priekyje prieš pat dabartinį viršutinį elementą.
// deque::pop_front Pašalinamas pirmasis elementas deque talpykloje, veiksmingai sumažinant jo dydį vienu
// „->“ yra vadinamas rodyklių operatoriumi ir naudojamas nukrypti nuo žymiklio, todėl sintaksė yra tokia pati kaip (* ptr). Nukreipti žymeklį reiškia gauti to adreso vertę

