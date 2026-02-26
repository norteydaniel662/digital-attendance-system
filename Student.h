#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>
using namespace std;

class Student {
private:
    string indexNumber;
    string fullName;

public:
    Student() {}
    Student(const string& index, const string& name)
        : indexNumber(index), fullName(name) {}

    string getIndexNumber() const { return indexNumber; }
    string getFullName() const { return fullName; }

    void display() const {
        cout << "Index Number: " << indexNumber
             << ", Full Name: " << fullName << endl;
    }
};

#endif
