#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include "Student.h"
#include "AttendanceSession.h"

using namespace std;

// ===== In-memory data (Week 1 + Week 2) =====
vector<Student> g_students;

AttendanceSession g_currentSession;
bool g_hasCurrentSession = false;

// ---------- Helpers ----------
static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static bool indexExists(const string& index) {
    for (const auto& s : g_students) {
        if (s.getIndexNumber() == index) return true;
    }
    return false;
}

// ---------- Student features (Week 1) ----------
void addStudent() {
    string index, name;
    cout << "\n--- Add Student ---\n";
    cout << "Enter Index Number: ";
    cin >> index;
    index = trim(index);

    if (indexExists(index)) {
        cout << "A student with this index already exists.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Full Name: ";
    getline(cin, name);
    name = trim(name);

    if (index.empty() || name.empty()) {
        cout << "Index and Name cannot be empty.\n";
        return;
    }

    g_students.emplace_back(index, name);
    cout << "Student added successfully.\n";
}

void viewStudents() {
    cout << "\n--- Registered Students ---\n";
    if (g_students.empty()) {
        cout << "No students registered yet.\n";
        return;
    }
    for (const auto& s : g_students) s.display();
}

// ---------- Session features (Week 2) ----------
void createSession() {
    cout << "\n--- Create Lecture Session ---\n";
    string course, date, startTime;
    int duration = 0;

    cout << "Course Code (e.g., EE201): ";
    cin >> course;

    cout << "Date (YYYY-MM-DD): ";
    cin >> date;

    cout << "Start Time (HH:MM): ";
    cin >> startTime;

    cout << "Duration (minutes): ";
    cin >> duration;
    if (duration < 0) duration = 0;

    g_currentSession = AttendanceSession(course, date, startTime, duration);
    g_hasCurrentSession = true;

    cout << "Session created successfully.\n";
    cout << "Course: " << g_currentSession.getCourseCode()
         << ", Date: " << g_currentSession.getDate()
         << ", Start: " << g_currentSession.getStartTime()
         << ", Duration: " << g_currentSession.getDurationMinutes() << " mins\n";
    cout << "Default session file name will be: "
         << g_currentSession.defaultFileName() << "\n";
}

void showCurrentSession() {
    cout << "\n--- Current Session ---\n";
    if (!g_hasCurrentSession) {
        cout << "No session created yet.\n";
        return;
    }
    cout << "Course: " << g_currentSession.getCourseCode() << "\n"
         << "Date: " << g_currentSession.getDate() << "\n"
         << "Start Time: " << g_currentSession.getStartTime() << "\n"
         << "Duration: " << g_currentSession.getDurationMinutes() << " minutes\n";
}

// ---------- Menus ----------
void studentsMenu() {
    int choice = -1;
    do {
        cout << "\n=== STUDENT MANAGEMENT ===\n"
             << "1. Add Student\n"
             << "2. View Students\n"
             << "3. Back\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

void sessionsMenu() {
    int choice = -1;
    do {
        cout << "\n=== LECTURE SESSIONS (WEEK 2) ===\n"
             << "1. Create Session\n"
             << "2. Show Current Session\n"
             << "3. Back\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createSession(); break;
            case 2: showCurrentSession(); break;
            case 3: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

void mainMenu() {
    int choice = -1;
    do {
        cout << "\n===== DIGITAL ATTENDANCE SYSTEM =====\n"
             << "1. Student Management\n"
             << "2. Attendance Sessions (Week 2)\n"
             << "3. Exit\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: studentsMenu(); break;
            case 2: sessionsMenu(); break;
            case 3: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

int main() {
    mainMenu();
    return 0;
}
