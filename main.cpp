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

const string plikUstawienia = "settings";
const string najlepszeWyniki = "highscores";

struct HighScoreEntry
{
    string playerName; // nazwa gracza i wynik
    int score;
};

bool compareByScore(const HighScoreEntry& a, const HighScoreEntry& b)
{
    return a.score > b.score; // wpisy wyników
}



void showHighScores()
{
    ifstream file(najlepszeWyniki);
    if (file.good())

    {
        vector<HighScoreEntry> highScores; //  przechoanie wpisy wyników

        string line;
        while (getline(file, line))
        {
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, line); // 
            if (!error)
            {
                string playerName = doc["PlayerName"].as<string>(); // Odczytanie nazwy gracza i wyniku
                int score = doc["Score"].as<int>();

                highScores.push_back({ playerName, score });
                // Dodanie wpisu do wektora
            }
            else
            {
                cout << "Blad podczas odczytu danych z pliku highscores.json\n";
                break;
            }
        }

        file.close();


        // sortowanie wynikow
        sort(highScores.begin(), highScores.end(), compareByScore);

        // Wyœwietlanie posortowanych wyników
        for (const auto& entry : highScores)
        {

            cout << "Gracz: " << entry.playerName << ", Wynik: " << entry.score << '\n'; // Wyœwietlenie posortowanych wpisów
        }
    }
    else
    {
        cout << "Blad podczas otwierania pliku highscores.json\n";

    }
}

void saveHighScore(const string& playerName, int score)
{
    ofstream file(najlepszeWyniki, ios::app); // Otwarcie pliku do zapisu
    if (file.good())
    {
        StaticJsonDocument<1024> doc;
        doc["PlayerName"] = playerName; // Tworzenie dokumentu JSON z nazw¹ gracza i wynikiem
        doc["Score"] = score;

        serializeJson(doc, file); 
        file << '\n';

        file.close();
    }
    else
    {
        cout << "Blad podczas zapisu do pliku highscores.json\n";

    }
}

void showOptions(StaticJsonDocument<1024>& doc)
{

    //Pokazanie zawartosci
    for (JsonPair p : doc.as<JsonObject>())
    {
        cout << p.key() << " " << p.value() << '\n'; // Wyœwietlenie klucza i wartoœci w dokumencie
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


void graj();
void instrukcja();
void ustawienia();
void highScores();

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

    cout << "\nWitaj w programie\n";
    cout << "--------------------\n";
    cout << "1. Nowa gra\n";
    cout << "2. Ustawienia\n";
    cout << "3. Instrukcja gry\n";
    cout << "4. Wyniki\n";
    cout << "5. Wyjscie\n\n";
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
        instrukcja();
        break;
    case 4:
        system("cls");
        highScores();
        break;
    case 5:
        return 0;
    default:
        cout << "Zly wybor. Sprobuj jeszcze raz.\n";
        break;
    }

    return 0;
}



void instrukcja()
{
    cout << "\nInstrukcja gry\n";
    cout << "--------------------\n";
    cout << "Twoim zadaniem jest zdobyc zloto\n";
    cout << "Isc na targ.\n";
    cout << "Kupic bron i zarobic najwiecej siana.\n";
    cout << "Powodzenia czarnuchu!\n";

    int wybor;
    do
    {
        cout << "1. Wroc do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        if (wybor != 1)
        {
            cout << "Zly wybor. Sprobuj jeszcze raz.\n";
        }
    } while (wybor != 1);
    system("cls");
    main();
}

void ustawienia()
{
    cout << "\nUstawienia\n";
    cout << "--------------------\n";

    int wybor;
    do
    {
        cout << "1. Zmien rozdzielczosc\n";
        cout << "2. Zmien tryb pelnoekranowy\n";
        cout << "3. Zmien opcje V-Sync\n";
        cout << "4. Powrot do menu glownego\n";
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
                main();
                return;
            }

            updateConfig("Resolution", rozdzielczosc);
            break;

        }
        case 2:
        {
            string trybPelnoekranowy;

            cout << "Pelny ekran:\n";
            cout << "--------------------\n";
            cout << "1. Tak\n";
            cout << "2. Nie\n";
            cout << "Wybierz tryb (wpisz numer): ";
            cin >> wybor;


            switch (wybor)
            {
            case 1:
                trybPelnoekranowy = "true";
                break;
            case 2:
                trybPelnoekranowy = "false";
                break;
            default:
                cout << "Niepoprawny wybor trybu pelnoekranowego. Powrot do menu glownego.\n";

                main();

                return;
            }

            updateConfig("Fullscreen", trybPelnoekranowy);
            break;
        }
        case 3:
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
                main();
                return;
            }

            updateConfig("V-Sync", VSync);
            break;
        }
        case 4:
            system("cls");
            main(); 
            return;
        default:
            cout << "Zly wybor. Sprobuj jeszcze raz.\n";
            break;
        }
    } while (true);
}

void highScores()
{
    cout << "\nWyniki\n";
    cout << "--------------------\n";

    showHighScores();

    int wybor;
    do
    {
        cout << "\n1. Wroc do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        if (wybor != 1)
        {
            cout << "Zly wybor. Sprobuj jeszcze raz.\n";
        }
    }

    while (wybor != 1);

    system("cls");
    main(); 
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

    string playerName;
    cout << "Wpisz swoje imie: ";
    cin >> playerName;

    saveHighScore(playerName, wynik);

    int wybor;
    do
    {
        cout << "\n1. Wroc do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        if (wybor != 1)
        {
            cout << "Zly wybor. Sprobuj jeszcze raz.\n";
        }
    } while (wybor != 1);

    system("cls");
    main();
}