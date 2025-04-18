#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// gives me the row of the csv
vector<string> CSVLine(const string& line) {
    vector<string> result;
    string field;
    bool insideQuotes = false;
    for (const char c : line) {
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    result.push_back(field);
    return result;
}

// Removes brackets and single quotes from the genre, cast, director columns
string OnlyWords(const string& input) {
    string output;
    for (const char c : input) {
        if (c != '[' && c != ']' && c != '\'') {
            output += c;
        }
    }
    return output;
}

int main() {
    int userYear;
    int minRuntime;
    string genreInput;

    cout << "Year:" << endl;
    cin >> userYear;

    cout << "Genre:" << endl;
    cin.ignore();
    getline(cin, genreInput);

    cout << "Runtime (in minutes):" << endl;
    cin >> minRuntime;

    ifstream file("100k_Movies_dataset.csv");
    string line;
    getline(file, line);

    while (getline(file, line)) {
        if (auto columns = CSVLine(line); columns.size() >= 10) {
            const string& title = columns[0];
            const int runtime       = stoi(columns[2]);
            const int releasedYear  = stoi(columns[8]);
            string genre = OnlyWords(columns[3]);
            if (releasedYear == userYear && runtime >= minRuntime) {
                if (genre.find(genreInput) != string::npos) {
                    cout << "Title:   " << title        << "\n"
                         << "Runtime: " << runtime      << "\n"
                         << "Genre:   " << genre        << "\n"
                         << "Year:    " << releasedYear << "\n\n";
                }
            }
        }
    }

    file.close();
    return 0;
}
