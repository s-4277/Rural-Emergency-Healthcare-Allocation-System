#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <limits>
using namespace std;

const int MAX_PATIENTS = 100;

/* ---------------- TEMPLATE CLASS ---------------- */

template <typename T>
class Record
{
protected:
    T id;

public:
    void setID(T value)
    {
        id = value;
    }

    T getID()
    {
        return id;
    }
};

/* ---------------- BASE CLASS ---------------- */

class Person
{
protected:
    string name;
    int age;
    string gender;

public:
    virtual void inputDetails();

    virtual void displayDetails();

    string getName();

    virtual ~Person() {}
};

/* ---------------- DERIVED CLASS ---------------- */

class Patient : public Person, public Record<int>
{
private:
    string village;
    int severity;
    string hospital;

public:
    void assignHospital();

    void inputDetails() override;

    void displayDetails() override;
};

/* ---------------- FUNCTION DECLARATIONS ---------------- */

void addPatient();

void showPatients();

void searchPatient();

#endif
