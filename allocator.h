
#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "patient.h"
#include "hospital.h"
#include <vector>
#include <string>
using namespace std;

// ============================================================
//  Custom Exception for Allocation
// ============================================================

class NoHospitalAvailableException : public exception {
private:
    string message;
public:
    NoHospitalAvailableException(int severity, const string& hub)
        : message("Error: No eligible hospital found for severity " +
                  to_string(severity) + " patient from hub " + hub + ".") {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// ============================================================
//  Template — Generic Sorter
//  Sorts any vector by a key extracted via a lambda
//  Used for: sorting patients by severity (desc)
//            sorting hospitals by score (desc)
// ============================================================

template <typename T, typename KeyFunc>
void sortByField(vector<T>& vec, KeyFunc getKey, bool descending = true) {
    int n = vec.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool shouldSwap = descending
                ? (getKey(vec[j]) < getKey(vec[j + 1]))
                : (getKey(vec[j]) > getKey(vec[j + 1]));
            if (shouldSwap) swap(vec[j], vec[j + 1]);
        }
    }
}

// ============================================================
//  Allocator — Core Decision Engine
//  Scores hospitals and allocates best one to patient
// ============================================================

class Allocator {
private:
    // Scoring breakdown struct for display
    struct HospitalScore {
        Hospital* hospital;
        double score;
        int severityWeight;
        int distanceScore;
        int bedScore;
    };

    // Calculate score for one hospital against one patient
    static HospitalScore calculateScore(Hospital& hospital,
                                        const Patient& patient);

public:
    // Main allocation function
    // Finds best hospital, updates patient, decrements bed
    static void allocate(Patient& patient,
                         vector<Hospital>& hospitals);

    // Sort patients by severity (critical first)
    static void sortPatientsBySeverity(vector<Patient>& patients);

    // Sort hospitals by score for display
    static vector<HospitalScore> rankHospitals(const Patient& patient,
                                               vector<Hospital>& hospitals);

    // Display scoring breakdown for a patient
    static void displayAllocationReport(const Patient& patient,
                                        vector<Hospital>& hospitals);
};

#endif // ALLOCATOR_H
