#include "allocator.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

// ============================================================
//  calculateScore
//  Scoring Formula:
//    severityWeight = severity * 10        (higher = more urgent)
//    distanceScore  = 100 - distance       (closer = better)
//    bedScore       = min(availableBeds,50) (more beds = safer)
//
//  Total Score = severityWeight + distanceScore + bedScore
//
//  Eligibility (non-negotiable, checked before scoring):
//    - availableBeds > 0
//    - if severity >= 4 → hospital must have emergency = true
// ============================================================

Allocator::HospitalScore Allocator::calculateScore(Hospital& hospital,
                                                    const Patient& patient) {
    int severity      = patient.getSeverity();
    string hub        = patient.getNearestHub();
    int distance      = hospital.getDistanceFromHub(hub);
    int availBeds     = hospital.getAvailableBeds();

    int severityWeight = severity * 10;
    int distanceScore  = 100 - distance;
    int bedScore       = min(availBeds, 50);

    double total = severityWeight + distanceScore + bedScore;

    return { &hospital, total, severityWeight, distanceScore, bedScore };
}

// ============================================================
//  rankHospitals
//  Returns all eligible hospitals ranked by score (desc)
//  Used internally by allocate() and externally for report
// ============================================================

vector<Allocator::HospitalScore> Allocator::rankHospitals(const Patient& patient,
                                                           vector<Hospital>& hospitals) {
    vector<HospitalScore> eligible;

    for (auto& h : hospitals) {
        if (!h.isEligible(patient.getSeverity())) continue;
        eligible.push_back(calculateScore(h, patient));
    }

    // Sort by score descending using our template
    sortByField(eligible,
                [](const HospitalScore& hs) { return hs.score; },
                true);

    return eligible;
}

// ============================================================
//  allocate
//  Finds best hospital, sets it on patient, decrements bed
//  Throws NoHospitalAvailableException if none qualify
// ============================================================

void Allocator::allocate(Patient& patient, vector<Hospital>& hospitals) {
    vector<HospitalScore> ranked = rankHospitals(patient, hospitals);

    if (ranked.empty()) {
        throw NoHospitalAvailableException(patient.getSeverity(),
                                           patient.getNearestHub());
    }

    HospitalScore& best = ranked[0];

    // Set hospital on patient
    patient.setAssignedHospital(best.hospital->getID(),
                                best.hospital->getName());

    // Decrement bed — throws BedUnavailableException if somehow 0
    // (double safety, isEligible already checks > 0)
    try {
        best.hospital->decrementBed();
    } catch (const exception& e) {
        cerr << "  [!] Bed decrement warning: " << e.what() << "\n";
    }

    // Show allocation result
    cout << "\n  ╔══════════════════════════════════════════════════╗\n";
    cout << "  ║           ALLOCATION SUCCESSFUL                  ║\n";
    cout << "  ╠══════════════════════════════════════════════════╣\n";
    cout << "  ║ Patient  : " << left << setw(38) << patient.getName() << "║\n";
    cout << "  ║ Severity : " << left << setw(38) << patient.getSeverity()  << "║\n";
    cout << "  ║ Hub      : " << left << setw(38) << patient.getNearestHub() << "║\n";
    cout << "  ╠══════════════════════════════════════════════════╣\n";
    cout << "  ║ Assigned : " << left << setw(38) << best.hospital->getName() << "║\n";
    cout << "  ║ Hosp ID  : " << left << setw(38) << best.hospital->getID() << "║\n";
    cout << "  ║ Location : " << left << setw(38) << best.hospital->getLocation() << "║\n";
    cout << "  ║ Phone    : " << left << setw(38) << best.hospital->getPhone() << "║\n";
    cout << "  ║ Score    : " << left << setw(38) << best.score << "║\n";
    cout << "  ╚══════════════════════════════════════════════════╝\n";
}

// ============================================================
//  sortPatientsBySeverity
//  Sorts patients vector: highest severity first
//  Uses the generic template sortByField ✅
// ============================================================

void Allocator::sortPatientsBySeverity(vector<Patient>& patients) {
    sortByField(patients,
                [](const Patient& p) { return p.getSeverity(); },
                true); // descending — critical first
}

// ============================================================
//  displayAllocationReport
//  Shows full scoring table for top 5 hospitals for a patient
//  Useful for transparency / teacher demo
// ============================================================

void Allocator::displayAllocationReport(const Patient& patient,
                                        vector<Hospital>& hospitals) {
    vector<HospitalScore> ranked = rankHospitals(patient, hospitals);

    cout << "\n  ┌─────────────────────────────────────────────────────────────────────┐\n";
    cout << "  │           HOSPITAL SCORING REPORT                                   │\n";
    cout << "  │  Patient : " << left << setw(57) << patient.getName() << "│\n";
    cout << "  │  Village : " << left << setw(57) << patient.getVillage() << "│\n";
    cout << "  │  Hub     : " << left << setw(57) << patient.getNearestHub() << "│\n";
    cout << "  │  Severity: " << left << setw(57) << patient.getSeverity() << "│\n";
    cout << "  ├──────┬───────────────────────────────┬───────┬────────┬──────┬───────┤\n";
    cout << "  │ Rank │ Hospital                      │SevWt  │DistSc  │BedSc │ Total │\n";
    cout << "  ├──────┼───────────────────────────────┼───────┼────────┼──────┼───────┤\n";

    if (ranked.empty()) {
        cout << "  │      No eligible hospitals found for this patient.                  │\n";
    } else {
        int limit = min((int)ranked.size(), 5);
        for (int i = 0; i < limit; i++) {
            auto& hs = ranked[i];
            string hospName = hs.hospital->getName();
            if (hospName.size() > 29) hospName = hospName.substr(0, 26) + "...";

            cout << "  │ " << setw(4) << (i + 1)
                 << " │ " << left << setw(29) << hospName
                 << " │ " << right << setw(5) << hs.severityWeight
                 << " │ " << setw(6) << hs.distanceScore
                 << " │ " << setw(4) << hs.bedScore
                 << " │ " << setw(5) << hs.score << " │\n";
        }
    }

    cout << "  └──────┴───────────────────────────────┴───────┴────────┴──────┴───────┘\n";
}
