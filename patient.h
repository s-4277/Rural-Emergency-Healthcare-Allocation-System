#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

/* =========================================================
   TEMPLATE CLASS
========================================================= */

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

    T getID() const
    {
        return id;
    }
};

/* =========================================================
   BASE ENTITY CLASS (Polymorphism)
========================================================= */

class Entity
{
public:
    virtual void display() const = 0;
    virtual ~Entity() {}
};

/* =========================================================
   CUSTOM EXCEPTIONS
========================================================= */

class InvalidSeverityException : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Invalid severity! Severity must be between 1 and 5.";
    }
};

class InvalidPatientDataException : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Invalid patient data entered.";
    }
};

class FileNotFoundException : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Required file not found.";
    }
};

/* =========================================================
   PERSON CLASS
========================================================= */

class Person
{
protected:
    string name;
    int age;
    string gender;

public:
    virtual void inputDetails()
    {
        cin.ignore();

        cout << "\nEnter Name: ";
        getline(cin, name);

        cout << "Enter Age: ";
        cin >> age;

        cin.ignore();

        cout << "Enter Gender: ";
        getline(cin, gender);
    }

    string getName() const
    {
        return name;
    }

    virtual ~Person() {}
};

/* =========================================================
   PATIENT CLASS
========================================================= */

class Patient : public Person,
                public Record<int>,
                public Entity
{
private:
    string village;
    string nearestHub;
    int severity;

    string assignedHospitalID;
    string assignedHospitalName;

public:

    /* ---------------- Constructor ---------------- */

    Patient()
    {
        assignedHospitalID = "NONE";
        assignedHospitalName = "Not Assigned";
    }

    /* ---------------- Registration ---------------- */

    static Patient registerNewPatient(
        const vector<string>& validVillages,
        const map<string, string>& villageHubMap)
    {
        Patient p;

        p.inputDetails();

        cout << "Enter Village: ";
        getline(cin, p.village);

        if (villageHubMap.find(p.village) == villageHubMap.end())
        {
            throw InvalidPatientDataException();
        }

        p.nearestHub = villageHubMap.at(p.village);

        cout << "Enter Severity (1-5): ";
        cin >> p.severity;

        if (p.severity < 1 || p.severity > 5)
        {
            throw InvalidSeverityException();
        }

        return p;
    }

    /* ---------------- Display ---------------- */

    void display() const override
    {
        cout << "\n====================================";
        cout << "\nPatient ID      : " << id;
        cout << "\nName            : " << name;
        cout << "\nAge             : " << age;
        cout << "\nGender          : " << gender;
        cout << "\nVillage         : " << village;
        cout << "\nNearest Hub     : " << nearestHub;
        cout << "\nSeverity        : " << severity;
        cout << "\nAssigned Hosp   : " << assignedHospitalName;
        cout << "\n====================================\n";
    }

    /* ---------------- Getters ---------------- */

    int getSeverity() const
    {
        return severity;
    }

    string getVillage() const
    {
        return village;
    }

    string getNearestHub() const
    {
        return nearestHub;
    }

    string getAssignedHospitalID() const
    {
        return assignedHospitalID;
    }

    /* ---------------- Setters ---------------- */

    void setAssignedHospital(string hospID,
                             string hospName)
    {
        assignedHospitalID = hospID;
        assignedHospitalName = hospName;
    }
};

#endif
