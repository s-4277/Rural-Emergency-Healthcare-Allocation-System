#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "patient.h"
#include "hospital.h"
#include <vector>
#include <string>
using namespace std;

// exceptions
class FileNotFoundException : public exception {
private:
    string message;
public:
    FileNotFoundException(const string& filename)
        : message("Error: Could not open file — " + filename) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class FileParseException : public exception {
private:
    string message;
public:
    FileParseException(const string& detail)
        : message("Error: Failed to parse data — " + detail) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

//  FileManager to handle all file I/O, loads hospitals, places, patients from .txt files & saves patient records back to file

class FileManager {
public:
    // Load hospitals from hospitals.txt
    static vector<Hospital> loadHospitals(const string& filename);

    // Load villages + hub mapping from places.txt
    // Returns vector of pairs: <villageName, nearestHub>
    static vector<pair<string, string>> loadPlaces(const string& filename);

    // Save a single patient record (append mode)
    static void savePatient(const Patient& patient, const string& filename);

    // Load all patients from patients.txt
    static vector<Patient> loadPatients(const string& filename,
                                        const vector<pair<string,string>>& villageHubMap);

    // Save all patients at once (overwrite mode) — used on exit
    static void saveAllPatients(const vector<Patient>& patients, const string& filename);
};

#endif // FILEMANAGER_H
