#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <locale>
#include <codecvt> // pentru conversia între UTF-8 și wstring
#include <cmath>   // pentru funcția log2 și ceil
#include <cwctype> // pentru verificarea caracterelor alfanumerice Unicode

using namespace std;

// Structură pentru a păstra informațiile despre caractere și frecvențe
struct Character {
    wchar_t ch;   // folosește wchar_t pentru caractere Unicode
    int freq;
    wstring code;
};

// Funcție pentru a compara frecvențele (pentru sortare descrescătoare)
bool compare(Character a, Character b) {
    return a.freq > b.freq;
}

// Funcție pentru generarea codurilor binare
wstring toBinary(int num, int bits) {
    wstring binary = L"";
    for (int i = bits - 1; i >= 0; i--) {
        binary += (num & (1 << i)) ? L'1' : L'0';
    }
    return binary;
}

// Funcție pentru generarea codurilor binare pe baza numărului de biți necesari
void generateBinaryCodes(vector<Character>& characters, int bits) {
    for (int i = 0; i < characters.size(); i++) {
        characters[i].code = toBinary(i, bits); // Generăm codul binar pe "bits" biți
    }
}

int main() {
    double count = 0;
    // Setăm locale pentru a suporta caractere UTF-8
    locale::global(locale("en_US.UTF-8"));
    wifstream file("input.txt");
    file.imbue(locale("en_US.UTF-8"));

    if (!file.is_open()) {
        wcerr << L"Eroare la deschiderea fisierului de intrare!" << endl;
        return 1;
    }

    // Deschidem fișierul de ieșire
    wofstream outputFile("output.txt");
    outputFile.imbue(locale("en_US.UTF-8"));
    if (!outputFile.is_open()) {
        wcerr << L"Eroare la deschiderea fisierului de iesire!" << endl;
        return 1;
    }

    // Citirea și procesarea textului din fișierul de intrare
    unordered_map<wchar_t, int> freq;
    wchar_t ch;
    while (file.get(ch)) {
        if (iswalpha(ch) || iswdigit(ch)) { // Procesăm doar caracterele alfanumerice
            freq[ch]++;
            count++;
        }
    }
    file.close();

    // Stocarea caracterelor și a frecvențelor într-un vector
    vector<Character> characters;
    for (auto& pair : freq) {
        characters.push_back({ pair.first, pair.second, L"" });
    }

    // Sortarea caracterelor după frecvență (descrescător)
    sort(characters.begin(), characters.end(), compare);

    // Calcularea numărului minim de biți necesari pentru coduri
    int numUniqueChars = characters.size();
    int bits = ceil(log2(numUniqueChars)); // Numărul de biți necesari

    // Generarea codurilor binare pe "bits" biți
    generateBinaryCodes(characters, bits);

    // Crearea unui map pentru a asocia caracterele cu codurile lor binare
    unordered_map<wchar_t, wstring> charToCode;
    for (const auto& character : characters) {
        charToCode[character.ch] = character.code;
    }

    // Scrierea codurilor generate în fișierul de ieșire

    outputFile << L"Codurile binare sunt:\n";
    for (const auto& character : characters) {
        outputFile << character.ch << L": " << character.code << endl;
    }
    //afisare probabilitate
    outputFile << endl;
    outputFile << endl;
    outputFile << endl;
    outputFile << " caracterul " << "          " << "probabilitatea";
    for (int i = 0; i < characters.size(); i++)
    {
        outputFile << endl << " " << characters[i].ch << L" " << "                   " << characters[i].freq / (double)(count) * 100 << "%";
    }
    // Redeschidem fișierul de intrare pentru a citi din nou textul și a-l codifica
    wifstream inputFile("input.txt");
    inputFile.imbue(locale("en_US.UTF-8"));
    if (!inputFile.is_open()) {
        wcerr << L"Eroare la redeschiderea fisierului de intrare!" << endl;
        return 1;
    }

    // Scriem textul codificat direct în fișierul de ieșire
    outputFile << L"\nTextul codificat este:\n";
    while (inputFile.get(ch)) {
        if (iswalpha(ch) || iswdigit(ch)) { // Ignorăm caracterele non-alfanumerice
            outputFile << charToCode[ch];  // Folosim map-ul pentru a scrie codul
        }
    }
    inputFile.close();
    outputFile.close();

    return 0;
}