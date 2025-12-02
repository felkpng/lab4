#pragma once
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

#include "GasNetwork.h"
#include "Connection.h"
#include "Pipe.h"
#include "Ks.h"
#include "temp.h"
#include "search.h"
#include "logger.h"

using namespace std;

vector<int> Selections(const std::string& enter) {
    std::vector<int> elements;
    std::stringstream ss(enter);
    int n;

    while (ss >> n) {
        elements.push_back(n);
    }

    if (!ss.eof()) {
        return {};
    }

    return elements;
}

template<typename T>
vector<int> IdList(map<int, T> m) {
    vector<int> keys;

    for (const auto& pair : m) {
        keys.push_back(pair.first);
    }

    return keys;
}

bool is_element(vector<int> vec, int n) {
    for (int m : vec)
        if (m == n) return true;
    return false;
}

GasNetwork::GasNetwork() : name(""), pipes({}), stations({}), pipes_id(0), stations_id(0) {}

bool GasNetwork::can_be_paired(int start_id, int stop_id) {
    if (start_id == stop_id) return false;
    for (Connection con : connections)
        if ((con.start_id == start_id && con.stop_id == stop_id) || (con.start_id == stop_id && con.stop_id == start_id)) return false;
    return true;
}

bool GasNetwork::can_be_pipe(int pipe_id) {
    for (Connection con : connections)
        if (con.pipe_id == pipe_id) return false;
    return true;
}

vector<int> GasNetwork::search_by_diameter(int diameter) {

    vector<int> elements = {};
    for (const auto& pair : pipes) {
        if (pair.second.getDiameter() == diameter && !pair.second.isRepair() && can_be_pipe(pair.first))
            elements.push_back(pair.first);
    }
    return elements;
}

void GasNetwork::EraseConnection() {
    cout << "\nDeleting a connection. To delete, enter the pipe ID: ";
    int enter = Enter<int>();
    int x = 0;

    for (Connection con : connections)
        if (con.pipe_id == enter) {
            connections.erase(connections.begin() + x);
            x++;
        }
}

void GasNetwork::EraseConnections_byPipe(int pipe_id) {
    int x = 0;
    for (Connection con : connections) {
        if (con.pipe_id == pipe_id)
            connections.erase(connections.begin() + x);
        else { x++; }
    }
}

void GasNetwork::EraseConnections_byKs(int station_id) {
    int x = 0;
    for (Connection con : connections) {
        if (con.start_id == station_id || con.stop_id == station_id)
            connections.erase(connections.begin() + x);
        else { x++; }
    }
}

void GasNetwork::NewPipe() {
    Pipe newPipe;
    cin >> newPipe;

    pipes[pipes_id] = newPipe;
    pipes_id++;

    system("cls");
}

void GasNetwork::NewKs() {
    Ks newKs;
    cin >> newKs;

    stations[stations_id] = newKs;
    stations_id++;

    system("cls");
}

void GasNetwork::NewConnection() {
    int start_id = -2;
    bool no_exit = true;
    vector<int> station_ids = IdList(stations);

    cout << "(-1 to exit to menu)\n";
    while (no_exit) {
        cout << "Enter the start station ID: ";
        start_id = Enter<int>();

        if (start_id == -1) { no_exit = false; }
        if (is_element(station_ids, start_id) || !no_exit) break;
        cout << "Error! Invalid ID\n";
    }

    int stop_id = -2;
    while (no_exit) {
        cout << "Enter the end station ID: ";
        stop_id = Enter<int>();

        if (stop_id == -1) { no_exit = false; }
        if ((is_element(station_ids, stop_id) && can_be_paired(start_id, stop_id)) || !no_exit) break;
        cout << "Error! Invalid ID\n";
    }

    int diameter = 0;
    while (no_exit) {
        cout << "Enter the diameter: ";
        diameter = Enter<int>();

        if (diameter == -1) { no_exit = false; }
        if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400 || !no_exit) break;
        cout << "Invalid diameter!\n";
    }

    int pipe_id = -2;
    if (no_exit) {
        vector<int> pipe_ids = search_by_diameter(diameter);

        if (pipe_ids.size() == 0) {
            cout << "No available pipes. Creating a new one\n";

            Pipe newPipe;
            newPipe.with_diameter(diameter);

            pipes[pipes_id] = newPipe;
            pipe_id = pipes_id;
            pipes_id++;
        }
        else {
            cout << "\nAvailable pipes:\n";

            for (const auto& pair : pipes) {
                if (is_element(pipe_ids, pair.first)) {
                    cout << "ID: " << pair.first << endl;
                    cout << pair.second << endl;
                }
            }

            while (true) {
                cout << "Select a pipe: ";
                pipe_id = Enter<int>();

                if (is_element(pipe_ids, pipe_id)) break;
                cout << "Invalid selection!" << endl;
            }
        }

        Connection newConnection(start_id, stop_id, pipe_id);
        connections.push_back(newConnection);
    }

    system("cls");
}

void GasNetwork::EditPipes(vector<int> selectedPipes) {
    cout << "\nActions\n1. Change name\n2. Change length\n3. Change diameter\n4. Change repair status\n5. Delete\n0. Back to menu\nChoice: ";

    int choice = Enter<int>();
    string n;
    float l;
    int d;
    bool r;

    switch (choice) {
    case 1:
        cout << "New name: ";
        n = Enter<string>();
        for (const auto& x : selectedPipes)
            pipes[x].setName(n);
        break;
    case 2:
        cout << "New length: ";
        l = Enter<float>();
        while (l <= 0) {
            std::cout << "Length must be greater than 0\nTry again: ";
            l = Enter<float>();
        }

        for (const auto& x : selectedPipes)
            pipes[x].setLength(l);
        break;
    case 3:
        cout << "New diameter: ";
        d = Enter<int>();
        while (d <= 0) {
            std::cout << "Diameter must be greater than 0\nTry again: ";
            d = Enter<int>();
        }

        for (const auto& x : selectedPipes)
            pipes[x].setDiameter(d);
        break;
    case 4:
        cout << "New repair status (0/1): ";
        r = Enter<bool>();

        for (const auto& x : selectedPipes)
            pipes[x].setRepair(r);
        break;
    case 5:
        for (const auto& x : selectedPipes) {
            EraseConnections_byPipe(x);
            pipes.erase(x);
        }
        break;
    case 6:
        break;
    }
}

void GasNetwork::EditStations(vector<int> selectedStations) {
    cout << "\nActions\n1. Change name\n2. Change total workshops\n3. Change working workshops\n4. Change type\n5. Delete\n0. Back to menu\nChoice: ";

    int choice = Enter<int>();
    int c;
    string n;
    int w;

    switch (choice) {
    case 1:
        cout << "New name: ";
        n = Enter<string>();
        for (const auto& x : selectedStations)
            stations[x].setName(n);
        break;
    case 2:
        c = 0;
        for (const auto& x : selectedStations)
            if (stations[x].getWorkshopsWorking() > c)
                c = stations[x].getWorkshopsWorking();

        cout << "New total workshops: ";
        w = Enter<int>();
        while (w < 0 || w < c) {
            std::cout << "Number must be non-negative and not less than " << c << "\nTry again: ";
            w = Enter<int>();
        }

        for (const auto& x : selectedStations)
            stations[x].setWorkshopsCount(w);
        break;
    case 3:
        c = 100000000;
        for (const auto& x : selectedStations)
            if (stations[x].getWorkshopsCount() < c)
                c = stations[x].getWorkshopsCount();

        cout << "New working workshops: ";
        w = Enter<int>();

        while (w > c || w < 0) {
            std::cout << "Number must be between 0 and " << c << " (inclusive)\nTry again: ";
            w = Enter<int>();
        }

        for (const auto& x : selectedStations)
            stations[x].setWorkshopsWorking(w);
        break;
    case 4:
        cout << "New type: ";
        n = Enter<string>();

        for (const auto& x : selectedStations)
            stations[x].setType(n);
        break;
    case 5:
        for (const auto& x : selectedStations) {
            EraseConnections_byKs(x);
            stations.erase(x);
        }
        break;
    case 6:
        break;
    }
}

void GasNetwork::SelectionToGroups(vector<int>& elements, vector<int>& selectedPipes, vector<int>& selectedStations) {
    for (int i = 0; i < elements.size(); i++) {
        if (pipes.count(elements[i]) > 0)
            selectedPipes.push_back(elements[i]);
        if (stations.count(elements[i]) > 0)
            selectedStations.push_back(elements[i]);
    }

    system("cls");
    cout << "Selection: ";
    for (const auto& x : selectedPipes) {
        cout << "ID: " << x << endl;
        cout << pipes[x];
        cout << endl;
    }

    for (const auto& x : selectedStations) {
        cout << "ID: " << x << endl;
        cout << stations[x];
        cout << endl;
    }
}

void GasNetwork::EditSelection(vector<int> elements, bool isPipes) {
    if (isPipes) {
        for (int elem : elements) {
            cout << "ID: " << elem << endl;
            cout << pipes[elem];
            cout << endl;
        }

        EditPipes(elements);
    }
    else {
        for (int elem : elements) {
            cout << "ID: " << elem << endl;
            cout << stations[elem];
            cout << endl;
        }

        EditStations(elements);
    }

    system("cls");
    cout << "Changes applied successfully!" << endl;
}

void GasNetwork::ShowAll() {
    system("cls");
    for (const auto& pair : pipes) {
        cout << "ID: " << pair.first << endl;
        cout << pipes[pair.first];
        cout << endl;
    }

    for (const auto& pair : stations) {
        cout << "ID: " << pair.first << endl;
        cout << stations[pair.first];
        cout << endl;
    }

    cout << "Connections:" << endl;
    for (Connection con : connections) {
        cout << con.start_id << " -> " << con.stop_id << " (pipe " << con.pipe_id << ")" << endl;

    }

    cout << "\nActions:\n1. Edit pipes\n2. Edit stations\n3. Delete connection\n0. Back\nChoice: ";
    int choice = Enter<int>();

    if (choice == 1 || choice == 2) {
        cout << "Enter IDs separated by spaces: ";
        string enter;
        enter = LoggedInput();

        vector<int> elements = Selections(enter);
        if (elements.size() == 0) { return; }
        if (choice == 1) EditSelection(elements, true);
        else EditSelection(elements, false);
    }
    else if (choice == 3) EraseConnection();

    system("cls");
}

void GasNetwork::Search() {
    system("cls");
    cout << "Search by: \n1. By name\n2. Pipes by repair status\n3. Stations by percentage of unused workshops\nChoice: ";
    int enter = Enter<int>();
    vector<int> elements = {};

    system("cls");
    switch (enter) {
    case 1:
        elements = SearchByName(pipes, stations);
        break;
    case 2:
        elements = SearchByRepair(pipes);
        break;
    case 3:
        elements = SearchByWorkshops(stations);
        break;
    };

    if (elements.size() == 0)
        cout << "No objects found matching the criteria" << endl;
    else if (enter != 1) {
        EditSelection(elements, (enter == 2));
    }
}

void GasNetwork::SaveData() {
    cout << "Enter file name: ";
    string fileName = Enter<string>();

    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (const auto& pair : pipes) {
            outFile << "#PIPE" << endl;
            outFile << pair.first << endl;
            outFile << pipes[pair.first] << endl;
        }

        for (const auto& pair : stations) {
            outFile << "#KS" << endl;
            outFile << pair.first << endl;
            outFile << stations[pair.first] << endl;
        }

        for (Connection con : connections) {
            outFile << "#CONNECTION" << endl;
            outFile << con << endl;
        }

        outFile.close();

        system("cls");
        cout << "Saved to " << fileName << endl;
    }

    else
        cout << "Error opening file";
}

void GasNetwork::LoadData() {
    cout << "Enter file name: ";
    string fileName = Enter<string>();

    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cout << "Failed to open file " << fileName << endl;
        return;
    }

    pipes.clear();
    stations.clear();
    connections = {};
    pipes_id = 0;
    stations_id = 0;

    string line;
    while (getline(inFile, line)) {
        if (line == "#PIPE") {
            int id;
            if (!(inFile >> id)) { cout << "Error reading pipe ID\n"; return; }
            inFile.ignore();

            Pipe truba;
            inFile >> truba;
            pipes[id] = truba;
            if (id >= pipes_id) pipes_id = id + 1;
        }

        else if (line == "#KS") {
            int id;
            string name;
            int workshops_count;
            int workshops_working;
            string type;

            if (!(inFile >> id)) { cout << "Error reading station ID\n"; return; }
            inFile.ignore();

            getline(inFile, name);
            if (!(inFile >> workshops_count)) { cout << "Error reading workshops count\n"; return; }
            inFile.ignore();

            if (!(inFile >> workshops_working)) { cout << "Error reading working workshops\n"; return; }
            inFile.ignore();

            getline(inFile, type);

            stations[id] = Ks(name, workshops_count, workshops_working, type);
            if (id >= stations_id) stations_id = id + 1;
        }

        else if (line == "#CONNECTION") {
            int start_id;
            int stop_id;
            int pipe_id;

            if (!(inFile >> start_id)) { cout << "Error reading start_id\n"; return; }
            inFile.ignore();

            if (!(inFile >> stop_id)) { cout << "Error reading stop_id\n"; return; }
            inFile.ignore();

            if (!(inFile >> pipe_id)) { cout << "Error reading pipe_id\n"; return; }
            inFile.ignore();

            Connection newConnection(start_id, stop_id, pipe_id);
            connections.push_back(newConnection);
        }
    }

    system("cls");
    cout << "Data loaded successfully!\n";
}

vector<vector<int>> GasNetwork::TopoSort() {
    vector<vector<int>> topoList = {};

    map<int, int> inCounts;
    for (const auto& pair : stations)
        inCounts[pair.first] = 0;

    for (Connection con : connections)
        inCounts[con.stop_id]++;

    vector<int> current_level;
    vector<int> to_delete;
    vector<int> to_decrease;
    while (!inCounts.empty()) {
        current_level = {};
        to_delete = {};
        to_decrease = {};
        for (const auto& pair : inCounts) {
            if (pair.second == 0) {
                for (Connection con : connections)
                    if (con.start_id == pair.first)
                        to_decrease.push_back(con.stop_id);

                current_level.push_back(pair.first);
                to_delete.push_back(pair.first);
            }
        }

        for (int x : to_decrease)
            inCounts[x]--;

        for (int x : to_delete)
            inCounts.erase(x);

        if (current_level.empty()) return { {-1} };
        topoList.push_back(current_level);
    }

    return topoList;
}

void GasNetwork::ShowTopo() {
    vector<vector<int>> topoList = TopoSort();

    if (topoList.empty()) cout << "No stations!";
    else if (topoList[0][0] == -1) cout << "The network contains cycles!";
    else {
        cout << "Topological sorting result\n";
        for (vector<int> s : topoList) {
            cout << "[ ";

            for (int elem : s)
                cout << elem << " ";

            cout << "]\n";
        }
    }

}

vector<vector<int>> FindWays(map<int, vector<int>>& adj, vector<int> way, const int& start, const int& stop) {
    way.push_back(start);
    if (start == stop) {
        return { way };
    }

    int  from_point = -1;
    if (way.size() > 1) from_point = way[way.size() - 2];

    vector<vector<int>> ways = {};

    for (const int& pointB : adj[start]) {
        if (count(way.begin(), way.end(), pointB) == 0) {
            const auto& founded_ways = FindWays(adj, way, pointB, stop);
            for (const auto& w : founded_ways)
                if (!way.empty()) ways.push_back(w);
        }
    }

    return ways;
}

void GasNetwork::MaxThread() {
    system("cls");
    cout << "start station ID: ";
    int start = Enter<int>();
    cout << "stop station ID: ";
    int stop = Enter<int>();

    map<int, int> throughput_table;
    throughput_table[500] = 6;
    throughput_table[700] = 13;
    throughput_table[1000] = 35;
    throughput_table[1400] = 100;

    map<int, vector<int>> adj;
    vector<int> keys;
    for (const auto& con : connections) {
        if (count(keys.begin(), keys.end(), con.start_id) == 0) {
            keys.push_back(con.start_id);
            adj[con.start_id] = {};
        }

        if (count(keys.begin(), keys.end(), con.stop_id) == 0) {
            keys.push_back(con.stop_id);
            adj[con.stop_id] = {};
        }

        adj[con.start_id].push_back(con.stop_id);
    }

    if (count(keys.begin(), keys.end(), start) == 1 && count(keys.begin(), keys.end(), stop) == 1) {
        vector<int> way;
        auto found_ways = FindWays(adj, way, start, stop);

        int max_throughput = 0;

        for (const auto& way : found_ways) {
            for (int i = 0; i < way.size(); i++)
                cout << way[i] << (i == way.size() - 1 ? "" : " -> ");
            cout << "  |  ";

            int throughput = 2000;
            for (int i = 0; i < way.size() - 1; i++)
                for (const auto& con : connections)
                    if (con.start_id == way[i] && con.stop_id == way[i + 1])
                        throughput = min(throughput, pipes[con.pipe_id].getDiameter());

            max_throughput = max(max_throughput, throughput);

            cout << throughput_table[throughput] << " mil m3/24h" << endl;
        }

        cout << "MaxThread = " << throughput_table[max_throughput] << " mil m3/24h" << endl;
    }
    else cout << "error!\n";
}

void GasNetwork::ShortCut() {
    system("cls");
    cout << "start station ID: ";
    int start = Enter<int>();
    cout << "stop station ID: ";
    int stop = Enter<int>();

    map<int, int> throughput_table;
    throughput_table[500] = 6;
    throughput_table[700] = 13;
    throughput_table[1000] = 35;
    throughput_table[1400] = 100;

    map<int, vector<int>> adj;
    vector<int> keys;
    for (const auto& con : connections) {
        if (count(keys.begin(), keys.end(), con.start_id) == 0) {
            keys.push_back(con.start_id);
            adj[con.start_id] = {};
        }

        if (count(keys.begin(), keys.end(), con.stop_id) == 0) {
            keys.push_back(con.stop_id);
            adj[con.stop_id] = {};
        }

        adj[con.start_id].push_back(con.stop_id);
    }

    if (count(keys.begin(), keys.end(), start) == 1 && count(keys.begin(), keys.end(), stop) == 1) {
        vector<int> way;
        auto found_ways = FindWays(adj, way, start, stop);

        auto shortcut = -1;

        for (const auto& way : found_ways) {
            int path = 0;
            for (int i = 0; i < way.size() - 1; i++)
                for (const auto& con : connections)
                    if (con.start_id == way[i] && con.stop_id == way[i + 1]) {
                        auto pipe = pipes[con.pipe_id];
                        path += round(pipe.getLength() / throughput_table[pipe.getDiameter()]);
                    }


            for (int i = 0; i < way.size(); i++)
                cout << way[i] << (i == way.size() - 1 ? "" : " -> ");
            cout << "  |  ";

            cout << path << "sec" << endl;
            if (shortcut == -1) shortcut = path;
            else shortcut = min(shortcut, path);
        }

        cout << "Shortcut = " << shortcut << "sec" << endl;
    }
    else cout << "error!" << endl;
}

void GasNetwork::NetMenu() {
    while (true) {
        cout << "Menu:\n1. Add pipe\n2. Add station\n3. View all\n4. Search\n5. Add connection\n6. Save\n7. Load\n8. Topological sort\n9. MaxThread\n10. Shortcut\n0. Exit\nChoice: ";

        int choice = Enter<int>();
        switch (choice) {
        case 1:
            NewPipe();
            break;
        case 2:
            NewKs();
            break;
        case 3:
            ShowAll();
            break;
        case 4:
            Search();
            break;
        case 5:
            NewConnection();
            break;
        case 6:
            SaveData();
            break;
        case 7:
            LoadData();
            break;
        case 8:
            ShowTopo();
            break;
        case 9:
            MaxThread();
            break;
        case 10:
            ShortCut();
            break;
        case 0:
            exit(0);
        }
    }
}