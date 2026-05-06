//  stores hospital data loaded from hospitals.txt
//  overrides display() and getID() from Entity
#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "entity.h"
#include <string>
using namespace std;

class Hospital : public Entity {
private:
    string location;
    int availableBeds;
    int totalBeds;
    string phone;
    bool emergency;

    int distWaknaghat;
    int distKandaghat;
    int distSolan;
    int distShimla;
    int distChail;

public:

    Hospital(const string& id, const string& name, const string& location, int availableBeds, int totalBeds, const string& phone, bool emergency, int distWaknaghat, int distKandaghat, int distSolan, int distShimla, int distChail);

    //fucntion overriding
    void display() const override;
    string getID() const override;

    string getLocation() const;
    int getAvailableBeds() const;
    int getTotalBeds() const;
    string getPhone() const;
    bool isEmergency() const;
    int getDistanceFromHub(const string& hub) const;

    void decrementBed();
    bool isEligible(int severity) const;
};

#endif
