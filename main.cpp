#include "ArduinoJson-v6.21.2.h"
#include <string>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <vector>


using namespace std;

const string plikUstawienia = "opcje.json";
const string najlepszeWyniki = "wyniki.json";

struct wynikiEntry
{
    string gracz; // nazwa gracza i wynik
    int wynik;
};

bool porWynik(const wynikiEntry& x, const wynikiEntry& y)
{
    return x.wynik > y.wynik; // wpisy wyników
}



void showWyniki()
{
    ifstream file(najlepszeWyniki);
    if (file.good())

    {
        vector<wynikiEntry> Wyniki; //  przechoanie wpisy wyników

        string line;
        while (getline(file, line))
        {
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, line); //
            if (!error)
            {
                string gracz = doc["PlayerName"].as<string>(); // Odczytanie nazwy gracza i wyniku
                int wynik = doc["Score"].as<int>();

                Wyniki.push_back({ gracz, wynik });
                // Dodanie wpisu do wektora
            }
            else
            {
                cout << "Blad podczas odczytu danych z pliku wyniki.json\n";
                break;
            }
        }

        file.close();


        // sortowanie wynikow
        sort(Wyniki.begin(), Wyniki.end(), porWynik);

        // Wyświetlanie posortowanych wyników
        for (const auto& para : Wyniki)
        {

            cout << "Gracz: " << para.gracz << ", Wynik: " << para.wynik << '\n'; // Wyświetlenie posortowanych wpisów
        }
    }
    else
    {
        cout << "Blad podczas otwierania pliku wyniki.json\n";

    }
}

void saveWynik(const string& gracz, int wynik)
{
    ofstream file(najlepszeWyniki, ios::app); // Otwarcie pliku do zapisu
    if (file.good())
    {
        StaticJsonDocument<1024> doc;
        doc["PlayerName"] = gracz; // Tworzenie dokumentu JSON z nazwą gracza i wynikiem
        doc["Score"] = wynik;

        serializeJson(doc, file);
        file << '\n';

        file.close();
    }
    else
    {
        cout << "Blad podczas zapisu do pliku wyniki.json\n";

    }
}

void showOptions(StaticJsonDocument<1024>& doc)
{

    //Pokazanie zawartosci
    for (JsonPair p : doc.as<JsonObject>())
    {
        cout << p.key() << " " << p.value() << '\n'; // Wyświetlenie klucza i wartości w dokumencie
    }
}

void createDefaultConfig()
{
    ofstream file(plikUstawienia); // Otwarcie pliku do zapisu
    if (file.good())
    {
        StaticJsonDocument<1024> doc;

        doc["Resolution"] = "1920x1080";
        doc["Fullscreen"] = true;
        doc["V-Sync"] = false;

        serializeJson(doc, file);

        file.close();

    }
    else
    {
        cout << "Blad podczas tworzenia pliku settings.json\n";

    }
}

void updateConfig(const string& key, const string& value)
{
    ifstream readFile(plikUstawienia);
    if (readFile.good())

    {
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, readFile);
        readFile.close();

        if (!error)
        {
            doc[key] = value;


            ofstream writeFile(plikUstawienia); // Otwarcie pliku do zapisu
            if (writeFile.good())
            {
                serializeJson(doc, writeFile); // Serializacja dokumentu do pliku
                writeFile.close();
                cout << "Pomyslnie zaktualizowano ustawienie " << key << '\n';
            }
            else
            {
                cout << "Blad podczas zapisu do pliku settings.json\n";

            }

        }
        else
        {
            cout << "Blad podczas odczytu danych z pliku settings.json\n";
        }
    }
    else
    {

        cout << "Blad podczas otwierania pliku settings.json\n";
    }
}




void ustawienia()
{
    cout << "\nUstawienia\n";
    cout << "--------------------\n";

    bool czy = true;
    int wybor;
    do
    {
        cout << "1. Zmien rozdzielczosc\n";
        cout << "2. Zmien opcje V-Sync\n";
        cout << "3. Powrot do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        switch (wybor)
        {
        case 1:
        {
            string rozdzielczosc;

            cout << "Dostepne rozdzielczosci:\n";
            cout << "--------------------\n";
            cout << "1. 640x480\n";
            cout << "2. 1680x1050\n";
            cout << "3. 1920x1080\n";
            cout << "Wybierz rozdzielczosc (wpisz numer): ";
            cin >> wybor;

            switch (wybor)
            {
            case 1:
                rozdzielczosc = "640x480";
                break;
            case 2:
                rozdzielczosc = "1680x1050";
                break;
            case 3:
                rozdzielczosc = "1920x1080";
                break;
            default:
                cout << "Niepoprawny wybor rozdzielczosci. Powrot do menu glownego.\n";
                return;
            }

            updateConfig("Resolution", rozdzielczosc);
            break;

        }

        case 2:
        {
            string VSync;

            cout << "V-Sync:\n";
            cout << "--------------------\n";
            cout << "1. Wlaczony\n";
            cout << "2. Wylaczony\n";
            cout << "Wybierz opcje (wpisz numer): ";
            cin >> wybor;


            switch (wybor)
            {
            case 1:
                VSync = "true";
                break;
            case 2:
                VSync = "false";
                break;
            default:
                cout << "Niepoprawny wybor opcji V-Sync. Powrot do menu glownego.\n";
                system("cls");
                return;
            }

            updateConfig("V-Sync", VSync);
            break;
        }

        case 3:
            czy = false;
            break;

        default:
            cout << "Zly wybor. Sprobuj jeszcze raz.\n";
            break;



        }
    } while (czy == true);
}

void Wyniki()
{
    cout << "\nWyniki\n";
    cout << "--------------------\n";

    showWyniki();

    cout << "wcisnij cos aby wrocic do menu" << endl;

    getchar();
    getchar();


}

void graj()
{


    cout << "Witaj w grze o Kongijskim handlarzu broniom!" << endl;
    cout << "Twoim zadaniem jest pomoc mu zdobyc jak najwiecej zlota." << endl;

    int wynik = 0;

    while (true)
    {
        cout << "Co chcesz zrobic? (1) Szukaj zlota (2) Idz na targ (3) Zakoncz gre ";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            int found_gold = rand() % 100 + 1;
            cout << "Znalazles " << found_gold << " sztuk zlota!" << endl;
            wynik += found_gold;
        }
        else if (choice == 2)
        {
            if (wynik < 50)
            {
                cout << "Nie masz wystarczajaco zlota, aby isc na targ." << endl;
            }
            else
            {
                cout << "Kupiles ulepszona bron za 50 sztuk zlota!" << endl;
                wynik -= 50;
            }
        }
        else if (choice == 3)
        {
            cout << "Koniec gry. Zdobyles " << wynik << " sztuk zlota." << endl;
            break;
        }
        else
        {
            cout << "Zly wybor." << endl;
        }
    }





    cout << "\nJestes koksem! Twoj wynik to " << wynik << "!\n";

    string gracz;
    cout << "Wpisz swoje imie: ";
    cin >> gracz;

    saveWynik(gracz, wynik);


}




int main()
{
    // utworzenie pliku konfiguracyjnego
    ifstream configFile(plikUstawienia);
    if (!configFile.good())
    {
        createDefaultConfig();
    }
    configFile.close();



    int opcja;

    for (;;)
    {
        system("cls");
        cout << "\nWitaj w programie\n";
        cout << "--------------------\n";
        cout << "1. Nowa gra\n";
        cout << "2. Ustawienia\n";
        cout << "3. Wyniki\n";
        cout << "4. Wyjscie\n\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> opcja;

        switch (opcja)
        {
        case 1:
            system("cls");
            graj();
            break;
        case 2:
            system("cls");
            ustawienia();
            break;
        case 3:
            system("cls");
            Wyniki();
            break;
        case 4:
            return 0;
            break;


        default:
            cout << "Zly wybor. Sprobuj jeszcze raz.\n";
            break;
        }

    }
    return 0;
}
