#include "Pipe.h"
#include <string>
#include <iostream>

#include "temp.h"
#include "logger.h"

Pipe::Pipe() : name(""), length(0.0f), diameter(0), repair(false) {}
Pipe::Pipe(std::string name, float length, int diameter, bool repair) : name(name), length(length), diameter(diameter), repair(repair) {}

void Pipe::with_diameter(int d) {
    std::cout << "\nCreating pipe\n";

    std::cout << "Name: ";
    std::string n;
    std::getline(std::cin >> std::ws, n);

    std::cout << "Length: ";
    float l = Enter<float>();
    while (l <= 0) {
        std::cout << "Length must be greater than 0\nTry again: ";
        l = Enter<float>();
    }

    std::cout << "Diameter: " << d << std::endl;

    std::cout << "Under repair (0/1): 0";

    setName(n);
    setLength(l);
    setDiameter(d);
    setRepair(false);
}

std::ostream& operator<<(std::ostream& os, const Pipe& truba) {
    bool toConsole = (&os == &std::cout);

    if (toConsole) {
        os << "Name: " << truba.getName() << '\n'
            << "Length: " << truba.getLength() << " | "
            << "Diameter: " << truba.getDiameter() << " | "
            << (truba.isRepair() ? "Under" : "Not under") << " repair\n";
    }
    else {
        os << truba.getName() << '\n'
            << truba.getLength() << '\n'
            << truba.getDiameter() << '\n'
            << truba.isRepair() << '\n';
    }

    return os;
}

std::istream& operator>>(std::istream& is, Pipe& truba) {
    bool fromConsole = (&is == &std::cin);

    if (fromConsole) {
        std::cout << "\nCreating pipe\n";

        std::cout << "Name: ";
        std::string n;
        std::getline(std::cin >> std::ws, n);

        std::cout << "Length: ";
        float l = Enter<float>();
        while (l <= 0) {
            std::cout << "Length must be greater than 0\nTry again: ";
            l = Enter<float>();
        }

        std::cout << "Diameter (500, 700, 1000, 1400 mm): ";
        int d = Enter<int>();
        while (d != 500 && d != 700 && d != 1000 && d != 1400) {
            std::cout << "Valid values: 500, 700, 1000, 1400\nTry again: ";
            d = Enter<int>();
        }

        std::cout << "Under repair (0/1): ";
        bool r = Enter<bool>();

        truba.setName(n);
        truba.setLength(l);
        truba.setDiameter(d);
        truba.setRepair(r);
    }
    else {
        std::string n;
        float l;
        int d;
        bool r;

        std::getline(is >> std::ws, n);
        is >> l >> d >> r;

        truba.setName(n);
        truba.setLength(l);
        truba.setDiameter(d);
        truba.setRepair(r);
    }

    return is;
}