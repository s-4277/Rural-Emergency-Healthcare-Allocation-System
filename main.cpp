#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include "entity.h"
#include "patient.h"
#include "hospital.h"
#include "allocator.h"
#include "filemanager.h"
using namespace std;

// ============================================================
//  Global Data
// ============================================================

vector<Hospital>              hospitals;
vector<Patient>               patients;
vector<pair<string, string>>  villageHubMap;  // <village, hub>
vector<string>                validVillages;

// ============================================================
//  UI Helpers
// ============================================================

void printHeader() {
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════════════╗\n";
    cout << "  ║     RURAL HEALTHCARE ALLOCATION SYSTEM               ║\n";
    cout << "  ║     Waknaghat Region, Himachal Pradesh               ║\n";
    cout << "  ╚══════════════════════════════════════════════════════╝\n";
}

void printMenu() {
    cout << "\n";
    cout << "  ┌──────────────────────────────────────┐\n";
    cout << "  │              MAIN MENU               │\n";
    cout << "  ├──────────────────────────────────────┤\n";
    cout << "  │  1.  Register New Patient            │\n";
    cout << "  │  2.  Allocate Hospital to Patient    │\n";
    cout << "  │  3.  View All Patients               │\n";
    cout << "  │  4.  Search Patient by ID            │\n";
    cout << "  │  5.  Sort Patients by Severity       │\n";
    cout << "  │  6.  View All Hospitals              │\n";
    cout << "  │  7.  View Allocation Score Report    │\n";
    cout << "  │  8.  View Village Directory          │\n";
    cout << "  │  9.  Exit                            │\n";
    cout << "  └──────────────────────────────────────┘\n";
    cout << "  Enter choice: ";
}

void printDivider() {
    cout << "  ──────────────────────────────────────────────────────\n";
}

int getIntInput(const string& prompt, int min, int max) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= min && val <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid input. Enter a number between "
             << min << " and " << max << ".\n";
    }
}

// ============================================================
//  1. Register New Patient
// ============================================================

void registerPatient() {
    printDivider();
    cout << "  REGISTER NEW PATIENT\n";
    printDivider();

    try {
        Patient p = Patient::registerNewPatient(validVillages, villageHubMap);
        patients.push_back(p);

        // Save immediately to file
        FileManager::savePatient(p, "patients.txt");

        cout << "\n  [✓] Patient registered successfully with ID: "
             << p.getID() << "\n";
        cout << "  [i] Use option 2 to allocate a hospital.\n";

    } catch (const InvalidSeverityException& e) {
        cerr << "\n  [!] " << e.what() << "\n";
    } catch (const InvalidPatientDataException& e) {
        cerr << "\n  [!] " << e.what() << "\n";
    } catch (const FileNotFoundException& e) {
        cerr << "\n  [!] " << e.what() << "\n";
    } catch (const exception& e) {
        cerr << "\n  [!] Unexpected error: " << e.what() << "\n";
    }
}

// ============================================================
//  2. Allocate Hospital to Patient
// ============================================================

void allocateHospital() {
    printDivider();
    cout << "  ALLOCATE HOSPITAL\n";
    printDivider();

    if (patients.empty()) {
        cout << "  [!] No patients registered yet.\n";
        return;
    }

    // Show unallocated patients
    cout << "\n  Unallocated Patients:\n";
    vector<int> unallocatedIdx;
    for (int i = 0; i < (int)patients.size(); i++) {
        if (patients[i].getAssignedHospitalID() == "NONE") {
            cout << "  [" << unallocatedIdx.size() + 1 << "] "
                 << patients[i].getID() << " — "
                 << patients[i].getName()
                 << " (Severity: " << patients[i].getSeverity() << ")\n";
            unallocatedIdx.push_back(i);
        }
    }

    if (unallocatedIdx.empty()) {
        cout << "  [i] All patients have been allocated.\n";
        return;
    }

    int sel = getIntInput("\n  Select patient number: ", 1, unallocatedIdx.size());
    Patient& selected = patients[unallocatedIdx[sel - 1]];

    try {
        Allocator::allocate(selected, hospitals);

        // Persist updated patients list
        FileManager::saveAllPatients(patients, "patients.txt");

    } catch (const NoHospitalAvailableException& e) {
        cerr << "\n  [!] " << e.what() << "\n";
        cerr << "  [i] Consider contacting hospitals directly or lowering severity filter.\n";
    } catch (const exception& e) {
        cerr << "\n  [!] " << e.what() << "\n";
    }
}

// ============================================================
//  3. View All Patients
// ============================================================

void viewAllPatients() {
    printDivider();
    cout << "  ALL PATIENTS  (" << patients.size() << " total)\n";
    printDivider();

    if (patients.empty()) {
        cout << "  [i] No patients registered yet.\n";
        return;
    }

    // Polymorphism in action — Entity* pointing to Patient objects
    for (auto& p : patients) {
        Entity* e = &p;
        e->display();
    }
}

// ============================================================
//  4. Search Patient by ID
// ============================================================

void searchPatient() {
    printDivider();
    cout << "  SEARCH PATIENT BY ID\n";
    printDivider();

    if (patients.empty()) {
        cout << "  [i] No patients registered yet.\n";
        return;
    }

    string searchID;
    cout << "  Enter Patient ID (e.g. P001): ";
    cin >> searchID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Convert to uppercase for comparison
    transform(searchID.begin(), searchID.end(), searchID.begin(), ::toupper);

    bool found = false;
    for (auto& p : patients) {
        if (p.getID() == searchID) {
            p.display();
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "  [!] No patient found with ID: " << searchID << "\n";
    }
}

// ============================================================
//  5. Sort Patients by Severity
// ============================================================

void sortPatients() {
    printDivider();
    cout << "  PATIENTS SORTED BY SEVERITY (Critical First)\n";
    printDivider();

    if (patients.empty()) {
        cout << "  [i] No patients registered yet.\n";
        return;
    }

    // Work on a copy so original order is preserved for ID continuity
    vector<Patient> sorted = patients;
    Allocator::sortPatientsBySeverity(sorted);

    for (auto& p : sorted) {
        p.display();
    }
}

// ============================================================
//  6. View All Hospitals
// ============================================================

void viewAllHospitals() {
    printDivider();
    cout << "  ALL HOSPITALS  (" << hospitals.size() << " total)\n";
    printDivider();

    // Polymorphism — Entity* pointing to Hospital objects
    for (auto& h : hospitals) {
        Entity* e = &h;
        e->display();
    }
}

// ============================================================
//  7. View Allocation Score Report
// ============================================================

void viewScoreReport() {
    printDivider();
    cout << "  ALLOCATION SCORE REPORT\n";
    printDivider();

    if (patients.empty()) {
        cout << "  [i] No patients registered yet.\n";
        return;
    }

    cout << "\n  Select Patient:\n";
    for (int i = 0; i < (int)patients.size(); i++) {
        cout << "  [" << (i + 1) << "] "
             << patients[i].getID() << " — "
             << patients[i].getName()
             << " (Severity: " << patients[i].getSeverity() << ")\n";
    }

    int sel = getIntInput("\n  Patient number: ", 1, patients.size());
    Allocator::displayAllocationReport(patients[sel - 1], hospitals);
}

// ============================================================
//  8. View Village Directory
// ============================================================

void viewVillageDirectory() {
    printDivider();
    cout << "  VILLAGE DIRECTORY  (" << villageHubMap.size() << " villages)\n";
    printDivider();

    cout << "\n  " << left << setw(25) << "Village"
         << setw(15) << "Nearest Hub" << "\n";
    cout << "  " << string(38, '-') << "\n";

    for (const auto& vh : villageHubMap) {
        cout << "  " << left << setw(25) << vh.first
             << setw(15) << vh.second << "\n";
    }
}

// ============================================================
//  Startup — Load all data files
// ============================================================

bool loadAllData() {
    cout << "\n  Loading system data...\n";
    bool success = true;

    // Load hospitals
    try {
        hospitals = FileManager::loadHospitals("hospitals.txt");
    } catch (const FileNotFoundException& e) {
        cerr << "  [✗] " << e.what() << "\n";
        success = false;
    } catch (const FileParseException& e) {
        cerr << "  [✗] " << e.what() << "\n";
        success = false;
    }

    // Load places
    try {
        villageHubMap = FileManager::loadPlaces("places.txt");
        for (const auto& vh : villageHubMap)
            validVillages.push_back(vh.first);
    } catch (const FileNotFoundException& e) {
        cerr << "  [✗] " << e.what() << "\n";
        success = false;
    } catch (const FileParseException& e) {
        cerr << "  [✗] " << e.what() << "\n";
        success = false;
    }

    // Load patients — not fatal if missing
    try {
        patients = FileManager::loadPatients("patients.txt", villageHubMap);
    } catch (const FileParseException& e) {
        cerr << "  [!] " << e.what() << "\n";
    }

    return success;
}

// ============================================================
//  Main
// ============================================================

int main() {
    printHeader();

    if (!loadAllData()) {
        cerr << "\n  [✗] Critical data files missing. Cannot start system.\n";
        cerr << "  [i] Ensure hospitals.txt and places.txt are in the same directory.\n";
        return 1;
    }

    cout << "\n  [✓] System ready.\n";

    int choice;
    do {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: registerPatient();     break;
            case 2: allocateHospital();    break;
            case 3: viewAllPatients();     break;
            case 4: searchPatient();       break;
            case 5: sortPatients();        break;
            case 6: viewAllHospitals();    break;
            case 7: viewScoreReport();     break;
            case 8: viewVillageDirectory(); break;
            case 9:
                cout << "\n  Saving all records...\n";
                try {
                    FileManager::saveAllPatients(patients, "patients.txt");
                } catch (const FileNotFoundException& e) {
                    cerr << "  [!] " << e.what() << "\n";
                }
                cout << "\n  ╔══════════════════════════════════════════════════════╗\n";
                cout << "  ║   Thank you for using Rural Healthcare System.       ║\n";
                cout << "  ║   Stay safe, Waknaghat region.                       ║\n";
                cout << "  ╚══════════════════════════════════════════════════════╝\n\n";
                break;
            default:
                cout << "  [!] Invalid choice. Please select 1–9.\n";
        }

    } while (choice != 9);

    return 0;
}
