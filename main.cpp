// COMSC-210 | Lab 28 | Sarthak Pani
// based on the instructor's lab 24 solution
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25;

bool load_data(const string &file, string values[], int count);
bool read_number(const string &prompt, int low, int high, int &value);
int select_goat(const list<Goat> &trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, const string names[], const string colors[]);
void display_trip(const list<Goat> &trip);
void find_goat(const list<Goat> &trip, const string &name);
void count_goats(const list<Goat> &trip, const string &color);
void youngest_goat(const list<Goat> &trip);
void oldest_goat(const list<Goat> &trip);
int main_menu();

int main() {
    srand(time(0));
    string names[SZ_NAMES], colors[SZ_COLORS];
    if (!load_data("names.txt", names, SZ_NAMES) ||
        !load_data("colors.txt", colors, SZ_COLORS)) {
        cerr << "Unable to load goat names and colors.\n";
        return 1;
    }

    list<Goat> trip;
    int trip_size = rand() % 8 + 8;
    for (int i = 0; i < trip_size; ++i) {
        int age = rand() % MAX_AGE;
        string name = names[rand() % SZ_NAMES];
        string color = colors[rand() % SZ_COLORS];
        trip.push_back(Goat(name, age, color));
    }

    int choice;
    while ((choice = main_menu()) != 8) {
        switch (choice) {
            case 1:
                add_goat(trip, names, colors);
                break;
            case 2:
                delete_goat(trip);
                break;
            case 3:
                display_trip(trip);
                break;
            case 4: {
                string name;
                cout << "Goat name: ";
                if (!getline(cin, name)) {
                    return 0;
                }
                find_goat(trip, name);
                break;
            }
            case 5: {
                string color;
                cout << "Goat color: ";
                if (!getline(cin, color)) {
                    return 0;
                }
                count_goats(trip, color);
                break;
            }
            case 6:
                youngest_goat(trip);
                break;
            case 7:
                oldest_goat(trip);
                break;
        }
    }
    cout << "Goodbye!\n";
    return 0;
}

// read the required entries without stepping beyond the array
bool load_data(const string &file, string values[], int count) {
    ifstream fin(file);
    for (int i = 0; i < count; ++i) {
        if (!(fin >> values[i])) {
            return false;
        }
    }
    return true;
}

// accept a whole number in range and stop on end of input
bool read_number(const string &prompt, int low, int high, int &value) {
    string line;
    while (cout << prompt, getline(cin, line)) {
        istringstream input(line);
        int number;
        char extra;
        if (input >> number && !(input >> extra) && number >= low && number <= high) {
            value = number;
            return true;
        }
        cout << "Invalid choice. Try again.\n";
    }
    return false;
}

int main_menu() {
    cout << "\n*** GOAT MANAGER ***\n"
         << "[1] Add a goat\n[2] Delete a goat\n[3] List goats\n"
         << "[4] Find a goat by name\n[5] Count goats by color\n[6] Show the youngest goat\n[7] Show the oldest goat\n[8] Quit\n";
    int choice = 8;
    read_number("Choice --> ", 1, 8, choice);
    return choice;
}

void delete_goat(list<Goat> &trip) {
    if (trip.empty()) {
        cout << "No goats to delete.\n";
        return;
    }
    int index = select_goat(trip);
    if (index == 0) {
        return;
    }
    auto it = trip.begin();
    advance(it, index - 1);
    trip.erase(it);
    cout << "Goat deleted. New trip size: " << trip.size() << '\n';
}

void add_goat(list<Goat> &trip, const string names[], const string colors[]) {
    int age = rand() % MAX_AGE;
    string name = names[rand() % SZ_NAMES];
    string color = colors[rand() % SZ_COLORS];
    trip.push_back(Goat(name, age, color));
    cout << "Goat added. New trip size: " << trip.size() << '\n';
}

void display_trip(const list<Goat> &trip) {
    if (trip.empty()) {
        cout << "No goats in the herd.\n";
    }
    int i = 1;
    for (const auto &goat : trip) {
        cout << "    [" << i++ << "] " << goat.get_name() << " ("
             << goat.get_age() << ", " << goat.get_color() << ")\n";
    }
}

int select_goat(const list<Goat> &trip) {
    display_trip(trip);
    int index = 0;
    read_number("Choice --> ", 1, static_cast<int>(trip.size()), index);
    return index;
}

// find the first goat with an exact name match
void find_goat(const list<Goat> &trip, const string &name) {
    auto it = find_if(trip.begin(), trip.end(), [&name](const Goat &goat) {
        return goat.get_name() == name;
    });
    if (it == trip.end()) {
        cout << "Goat not found.\n";
    } else {
        cout << it->get_name() << " (" << it->get_age() << ", "
             << it->get_color() << ")\n";
    }
}

// count every goat with an exact color match
void count_goats(const list<Goat> &trip, const string &color) {
    auto count = count_if(trip.begin(), trip.end(), [&color](const Goat &goat) {
        return goat.get_color() == color;
    });
    cout << "Goats with color " << color << ": " << count << '\n';
}

// show the first goat with the lowest age
void youngest_goat(const list<Goat> &trip) {
    if (trip.empty()) {
        cout << "No goats in the herd.\n";
        return;
    }
    auto it = min_element(trip.begin(), trip.end(), [](const Goat &a, const Goat &b) {
        return a.get_age() < b.get_age();
    });
    cout << "Youngest goat: " << it->get_name() << " (" << it->get_age()
         << ", " << it->get_color() << ")\n";
}

// show the first goat with the highest age
void oldest_goat(const list<Goat> &trip) {
    if (trip.empty()) {
        cout << "No goats in the herd.\n";
        return;
    }
    auto it = max_element(trip.begin(), trip.end(), [](const Goat &a, const Goat &b) {
        return a.get_age() < b.get_age();
    });
    cout << "Oldest goat: " << it->get_name() << " (" << it->get_age()
         << ", " << it->get_color() << ")\n";
}
