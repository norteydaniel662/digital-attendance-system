#include <iostream>
#include <vector>
#include "Student.h"
using namespace std;

vector<Student> studentList;

void addStudent() {
    string index, name;

    cout << "\n--- Add Student ---\n";
    cout << "Enter Index Number: ";
    cin >> index;

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, name);

    Student s(index, name);
    studentList.push_back(s);

    cout << "Student added successfully!\n";
}

void viewStudents() {
    cout << "\n--- Registered Students ---\n";

    if (studentList.empty()) {
        cout << "No students registered yet.\n";
        return;
    }

    for (const Student &s : studentList) {
        s.display();
    }
}

void menu() {
    int choice;

    do {
        cout << "\n===== DIGITAL ATTENDANCE SYSTEM (WEEK 1) =====\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 3);
}

int main() {
    menu();
    return 0;
}