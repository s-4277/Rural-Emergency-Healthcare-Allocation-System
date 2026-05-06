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
    virtual void inputDetails()
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter Name: ";
        getline(cin, name);

        cout << "Enter Age: ";
        cin >> age;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Gender: ";
        getline(cin, gender);
    }

    virtual void displayDetails()
    {
        cout << "\nName: " << name;
        cout << "\nAge: " << age;
        cout << "\nGender: " << gender;
    }

    string getName()
    {
        return name;
    }

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

    void assignHospital()
    {
        if(village == "Taradevi" || village == "Jubbarhatti")
            hospital = "IGMC Shimla";

        else if(village == "Barog" || village == "Nagaun" || village == "Tanaji")
            hospital = "Civil Hospital Kandaghat";

        else if(village == "Dera" || village == "Kaithlighat" || village == "Kiarighat")
            hospital = "Civil Hospital Kandaghat";

        else if(village == "Rachhana")
            hospital = "Regional Hospital Solan";

        else
            hospital = "Regional Hospital Solan";
    }

    /* --------- POLYMORPHISM (Function Overriding) --------- */

    void inputDetails() override
    {
        Person::inputDetails();

        cout << "Enter Village: ";
        getline(cin, village);

        cout << "Enter Severity (1-5): ";
        cin >> severity;

        assignHospital();
    }

    void displayDetails() override
    {
        cout << "\n========== Patient Record ==========";
        cout << "\nPatient ID: " << id;

        Person::displayDetails();

        cout << "\nVillage: " << village;
        cout << "\nSeverity Level: " << severity;
        cout << "\nAssigned Hospital: " << hospital;
        cout << "\n====================================\n";
    }
};

/* ---------------- GLOBAL ARRAY ---------------- */

Patient patients[MAX_PATIENTS];
int patientCount = 0;

/* ---------------- FUNCTIONS ---------------- */

void addPatient()
{
    if(patientCount >= MAX_PATIENTS)
    {
        cout << "\nPatient storage full!\n";
        return;
    }

    patients[patientCount].setID(patientCount + 1);

    patients[patientCount].inputDetails();

    patientCount++;

    cout << "\nPatient Registered Successfully!\n";
}

void showPatients()
{
    if(patientCount == 0)
    {
        cout << "\nNo patients registered.\n";
        return;
    }

    for(int i = 0; i < patientCount; i++)
    {
        patients[i].displayDetails();
    }
}

void searchPatient()
{
    string searchName;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter patient name: ";
    getline(cin, searchName);

    for(int i = 0; i < patientCount; i++)
    {
        if(patients[i].getName() == searchName)
        {
            patients[i].displayDetails();
            return;
        }
    }

    cout << "\nPatient not found.\n";
}

/* ---------------- MAIN FUNCTION ---------------- */

int main()
{
    int choice;

    do
    {
        cout << "\n========================================";
        cout << "\n Rural Healthcare Allocation System";
        cout << "\n========================================";
        cout << "\n1. Register Patient";
        cout << "\n2. Show All Patients";
        cout << "\n3. Search Patient";
        cout << "\n4. Exit";
        cout << "\nEnter choice: ";

        cin >> choice;

        switch(choice)
        {
            case 1:
                addPatient();
                break;

            case 2:
                showPatients();
                break;

            case 3:
                searchPatient();
                break;

            case 4:
                cout << "\nSystem Closed Successfully.\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while(choice != 4);

    return 0;
}
