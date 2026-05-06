#include "filemanager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

vector<Hospital> FileManager::loadHospitals(const string& filename) {
    vector<Hospital> hospitals;
    ifstream file(filename);

    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    string line;
    getline(file, line); // skip header row

    int lineNum = 1;
    while (getline(file, line)) {
        lineNum++;
        if (trim(line).empty()) continue;

        stringstream ss(line);
        string id, name, location, availStr, totalStr,
               phone, emergStr,
               dWakStr, dKanStr, dSolStr, dShiStr, dChaStr;

        try {
            getline(ss, id,        ',');
            getline(ss, name,      ',');
            getline(ss, location,  ',');
            getline(ss, availStr,  ',');
            getline(ss, totalStr,  ',');
            getline(ss, phone,     ',');
            getline(ss, emergStr,  ',');
            getline(ss, dWakStr,   ',');
            getline(ss, dKanStr,   ',');
            getline(ss, dSolStr,   ',');
            getline(ss, dShiStr,   ',');
            getline(ss, dChaStr);

            // Trim all fields
            id       = trim(id);
            name     = trim(name);
            location = trim(location);
            phone    = trim(phone);
            emergStr = trim(emergStr);

            if (id.empty() || name.empty()) {
                cerr << "  [!] Skipping malformed line " << lineNum << "\n";
                continue;
            }

            int availBeds  = stoi(trim(availStr));
            int totalBeds  = stoi(trim(totalStr));
            bool emergency = (trim(emergStr) == "Yes");
            int dWak       = stoi(trim(dWakStr));
            int dKan       = stoi(trim(dKanStr));
            int dSol       = stoi(trim(dSolStr));
            int dShi       = stoi(trim(dShiStr));
            int dCha       = stoi(trim(dChaStr));

            hospitals.emplace_back(id, name, location,
                                   availBeds, totalBeds,
                                   phone, emergency,
                                   dWak, dKan, dSol, dShi, dCha);

        } catch (const InvalidHospitalDataException& e) {
            cerr << "  [!] Line " << lineNum << ": " << e.what() << "\n";
        } catch (const invalid_argument&) {
            cerr << "  [!] Line " << lineNum << ": Invalid numeric value, skipping.\n";
        } catch (const out_of_range&) {
            cerr << "  [!] Line " << lineNum << ": Numeric value out of range, skipping.\n";
        }
    }

    file.close();

    if (hospitals.empty()) {
        throw FileParseException("No valid hospitals loaded from " + filename);
    }

    cout << "  [✓] Loaded " << hospitals.size() << " hospitals from " << filename << "\n";
    return hospitals;
}

//  Format:
//  Village,NearestHub
//  Returns vector of <village, hub> pairs

vector<pair<string, string>> FileManager::loadPlaces(const string& filename) {
    vector<pair<string, string>> places;
    ifstream file(filename);

    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    string line;
    getline(file, line); // skip header row

    int lineNum = 1;
    while (getline(file, line)) {
        lineNum++;
        if (trim(line).empty()) continue;

        stringstream ss(line);
        string village, hub;

        try {
            getline(ss, village, ',');
            getline(ss, hub);

            village = trim(village);
            hub     = trim(hub);

            if (village.empty() || hub.empty()) {
                cerr << "  [!] Skipping malformed line " << lineNum << " in places file.\n";
                continue;
            }

            places.emplace_back(village, hub);

        } catch (...) {
            cerr << "  [!] Error parsing places file at line " << lineNum << "\n";
        }
    }

    file.close();

    if (places.empty()) {
        throw FileParseException("No valid places loaded from " + filename);
    }

    cout << "  [✓] Loaded " << places.size() << " villages from " << filename << "\n";
    return places;
}

//  Save Single Patient (append mode)
//  Format:
//  ID,Name,Age,Gender,Village,Hub,Severity,HospID,HospName


void FileManager::savePatient(const Patient& patient, const string& filename) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    file << patient.toFileString() << "\n";
    file.close();
}


//  Load All Patients from patients.txt
//  Format:
//  ID,Name,Age,Gender,Village,Hub,Severity,HospID,HospName

vector<Patient> FileManager::loadPatients(const string& filename,
                                          const vector<pair<string,string>>& villageHubMap) {
    vector<Patient> patients;
    ifstream file(filename);

    if (!file.is_open()) {
        // patients.txt may not exist yet on first run — not a fatal error
        cout << "  [i] No existing patient records found. Starting fresh.\n";
        return patients;
    }

    string line;
    int lineNum = 0;
    int patientCounter = 0;

    while (getline(file, line)) {
        lineNum++;
        if (trim(line).empty()) continue;

        stringstream ss(line);
        string id, name, ageStr, gender, village,
               hub, sevStr, hospID, hospName;

        try {
            getline(ss, id,       ',');
            getline(ss, name,     ',');
            getline(ss, ageStr,   ',');
            getline(ss, gender,   ',');
            getline(ss, village,  ',');
            getline(ss, hub,      ',');
            getline(ss, sevStr,   ',');
            getline(ss, hospID,   ',');
            getline(ss, hospName);

            id       = trim(id);
            name     = trim(name);
            gender   = trim(gender);
            village  = trim(village);
            hub      = trim(hub);
            hospID   = trim(hospID);
            hospName = trim(hospName);

            int age      = stoi(trim(ageStr));
            int severity = stoi(trim(sevStr));

            // Track highest patient number to sync static counter
            if (id.size() > 1 && id[0] == 'P') {
                try {
                    int num = stoi(id.substr(1));
                    if (num > patientCounter) patientCounter = num;
                } catch (...) {}
            }

            patients.emplace_back(id, name, age, gender,
                                  village, hub, severity,
                                  hospID, hospName);

        } catch (const InvalidSeverityException& e) {
            cerr << "  [!] Line " << lineNum << ": " << e.what() << "\n";
        } catch (const InvalidPatientDataException& e) {
            cerr << "  [!] Line " << lineNum << ": " << e.what() << "\n";
        } catch (const invalid_argument&) {
            cerr << "  [!] Line " << lineNum << ": Invalid numeric value, skipping.\n";
        } catch (const out_of_range&) {
            cerr << "  [!] Line " << lineNum << ": Numeric value out of range, skipping.\n";
        }
    }

    file.close();

    cout << "  [✓] Loaded " << patients.size() << " patient records from " << filename << "\n";
    return patients;
}


void FileManager::saveAllPatients(const vector<Patient>& patients, const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    for (const auto& p : patients) {
        file << p.toFileString() << "\n";
    }

    file.close();
    cout << "  [✓] Patient records saved to " << filename << "\n";
}
