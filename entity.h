
#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
using namespace std;

class Entity {
protected:
    string id;
    string name;

public:
    Entity(const string& id, const string& name)
        : id(id), name(name) {}

    // virtual destructor
    virtual ~Entity() {}

    // pure virtual functions
    virtual void display() const = 0;
    virtual string getID() const = 0;

    string getName() const {
        return name;
    }
};

#endif 
