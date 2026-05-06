#include "hospital.h"
#include <iostream>
#include <stdexcept>
using namespace std;

class BedUnavailableException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: No available beds at this hospital.";
    }
};

class InvalidHospitalDataException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Invalid hospital data encountered.";
    }
};

Hospital::Hospital(const string& id,
                   const string& name,
                   const string& location,
                   int availableBeds,
                   int totalBeds,
                   const string& phone,
                   bool emergency,
                   int distWaknaghat,
                   int distKandaghat,
                   int distSolan,
                   int distShimla,
                   int distChail)
    : Entity(id, name),
      location(location),
      availableBeds(availableBeds),
      totalBeds(totalBeds),
      phone(phone),
      emergency(emergency),
      distWaknaghat(distWaknaghat),
      distKandaghat(distKandaghat),
      distSolan(distSolan),
      distShimla(distShimla),
      distChail(distChail)
{
    if (id.empty() || name.empty() || location.empty()) {
        throw InvalidHospitalDataException();
    }
}

string Hospital::getID() const {
    return id;
}

void Hospital::display() const {
    cout << "\n  +-------------------------------------------------+\n";
    cout << "  | Hospital ID   : " << id << "\n";
    cout << "  | Name          : " << name << "\n";
    cout << "  | Location      : " << location << "\n";
    cout << "  | Available Beds: ";
    if (availableBeds == -1)
        cout << "Unknown\n";
    else
        cout << availableBeds << "\n";
    cout << "  | Total Beds    : ";
    if (totalBeds == -1)
        cout << "Unknown\n";
    else
        cout << totalBeds << "\n";
    cout << "  | Phone         : " << phone << "\n";
    cout << "  | Emergency 24x7: " << (emergency ? "Yes" : "No") << "\n";
    cout << "  +-------------------------------------------------+\n";
}


string Hospital::getLocation() const {
    return location;
}

int Hospital::getAvailableBeds() const {
    return availableBeds;
}

int Hospital::getTotalBeds() const {
    return totalBeds;
}

string Hospital::getPhone() const {
    return phone;
}

bool Hospital::isEmergency() const {
    return emergency;
}

int Hospital::getDistanceFromHub(const string& hub) const {
    if (hub == "Waknaghat") return distWaknaghat;
    if (hub == "Kandaghat") return distKandaghat;
    if (hub == "Solan")     return distSolan;
    if (hub == "Shimla")    return distShimla;
    if (hub == "Chail")     return distChail;

    // Unknown hub — return a very large distance so it scores lowest
    return 999;
}

void Hospital::decrementBed() {
    if (availableBeds <= 0) {
        throw BedUnavailableException();
    }
    availableBeds--;
}

//  Eligibility Check
//  Rules:
//   - availableBeds must be > 0 (not -1, not 0)
//   - if severity >= 4, hospital must have emergency = true

bool Hospital::isEligible(int severity) const {
    if (availableBeds <= 0) return false;
    if (severity >= 4 && !emergency) return false;
    return true;
}
