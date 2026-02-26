#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include "Student.h"
#include "AttendanceSession.h"

using namespace std;

// ===== In-memory data =====
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

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static bool indexExists(const string& index) {
    for (const auto& s : g_students) {
        if (s.getIndexNumber() == index) return true;
    }
    return false;
}

static int readInt(const string& prompt, int minVal, int maxVal) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= minVal && v <= maxVal) {
            clearInput();
            return v;
        }
        cout << "Invalid input. Enter between " << minVal << " and " << maxVal << ".\n";
        clearInput();
    }
}

// ---------- Week 4: Students persistence ----------
static bool saveStudentsToFile(const string& filename = "students.txt") {
    ofstream out(filename);
    if (!out) return false;

    for (const auto& s : g_students)
        out << s.getIndexNumber() << "|" << s.getFullName() << "\n";

    return true;
}

static bool loadStudentsFromFile(const string& filename = "students.txt") {
    ifstream in(filename);
    if (!in) return false;

    g_students.clear();
    string line;

    while (getline(in, line)) {
        istringstream iss(line);
        string idx, name;

        if (getline(iss, idx, '|') && getline(iss, name, '|'))
            g_students.emplace_back(idx, name);
    }
    return true;
}

// ---------- Student features ----------
void addStudent() {
    string index, name;
    cout << "\n--- Add Student ---\n";
    cout << "Enter Index Number: ";
    cin >> index; clearInput();
    index = trim(index);

    if (indexExists(index)) {
        cout << "Index already exists.\n";
        return;
    }

    cout << "Enter Full Name: ";
    getline(cin, name);
    name = trim(name);

    if (index.empty() || name.empty()) {
        cout << "Inputs cannot be empty.\n";
        return;
    }

    g_students.emplace_back(index, name);
    cout << "Student added.\n";
}

void viewStudents() {
    cout << "\n--- Registered Students ---\n";
    if (g_students.empty()) {
        cout << "No students.\n";
        return;
    }
    for (const auto& s : g_students) s.display();
}

// ---------- Session features (Week 2) ----------
void createSession() {
    cout << "\n--- Create Session ---\n";
    string course, date, startTime;

    cout << "Course Code: ";
    cin >> course; clearInput();

    cout << "Date (YYYY-MM-DD): ";
    cin >> date; clearInput();

    cout << "Start Time (HH:MM): ";
    cin >> startTime; clearInput();

    int duration = readInt("Duration (minutes): ", 0, 600);

    g_currentSession = AttendanceSession(course, date, startTime, duration);
    g_hasCurrentSession = true;

    cout << "Session created. File: "
         << g_currentSession.defaultFileName() << "\n";
}

void showCurrentSession() {
    cout << "\n--- Current Session ---\n";
    if (!g_hasCurrentSession) {
        cout << "No session.\n";
        return;
    }
    cout << "Course: " << g_currentSession.getCourseCode() << "\n"
         << "Date: " << g_currentSession.getDate() << "\n"
         << "Start Time: " << g_currentSession.getStartTime() << "\n"
         << "Duration: " << g_currentSession.getDurationMinutes() << " mins\n";
}

// ---------- Week 3 features ----------
static Status chooseStatus() {
    cout << "Status:\n1) Present\n2) Absent\n3) Late\n";
    int opt = readInt("Choose (1-3): ", 1, 3);
    return (opt == 1) ? Status::Present :
           (opt == 2) ? Status::Absent :
                        Status::Late;
}

void markAttendance() {
    if (!g_hasCurrentSession) {
        cout << "No session.\n";
        return;
    }

    cout << "\n--- Mark Attendance ---\n";
    cout << "Enter index or 'DONE'.\n";

    while (true) {
        cout << "Index: ";
        string idx;
        cin >> idx; clearInput();

        if (idx == "DONE" || idx == "done") break;

        Status st = chooseStatus();
        g_currentSession.addOrUpdateRecord(idx, st);
        cout << "Recorded.\n";
    }
}

void updateAttendance() {
    if (!g_hasCurrentSession) {
        cout << "No session.\n";
        return;
    }

    cout << "Index to update: ";
    string idx;
    cin >> idx; clearInput();

    Status st = chooseStatus();

    if (g_currentSession.updateRecord(idx, st))
        cout << "Updated.\n";
    else
        cout << "No record found.\n";
}

void displayAttendanceList() {
    if (!g_hasCurrentSession) {
        cout << "No session.\n";
        return;
    }

    cout << "\n--- Attendance List ---\n";
    const auto& recs = g_currentSession.getRecords();

    if (recs.empty()) {
        cout << "No records.\n";
        return;
    }

    for (const auto& r : recs) {
        string status =
            (r.status == Status::Present) ? "PRESENT" :
            (r.status == Status::Absent)  ? "ABSENT"  :
                                            "LATE";
        cout << "Index: " << r.indexNumber << " | " << status << "\n";
    }

    int p, a, l;
    g_currentSession.summaryCounts(p, a, l);

    cout << "Summary: Present=" << p
         << ", Absent=" << a
         << ", Late=" << l << "\n";
}

// ---------- Week 4: CSV Export ----------
void exportSessionToCSV() {
    if (!g_hasCurrentSession) {
        cout << "No session.\n";
        return;
    }

    ofstream out("attendance_export.csv");
    if (!out) {
        cout << "Could not write CSV.\n";
        return;
    }

    out << "Index Number,Status\n";

    for (const auto& r : g_currentSession.getRecords()) {
        string s = (r.status == Status::Present) ? "PRESENT" :
                   (r.status == Status::Absent)  ? "ABSENT"  :
                                                   "LATE";
        out << r.indexNumber << "," << s << "\n";
    }

    int p, a, l;
    g_currentSession.summaryCounts(p, a, l);

    out << "\nSummary,,\n";
    out << "Present," << p << "\n";
    out << "Absent,"  << a << "\n";
    out << "Late,"    << l << "\n";

    cout << "Exported to attendance_export.csv\n";
}

// ---------- Week 4: Save/Load Session ----------
void saveCurrentSession() {
    if (!g_hasCurrentSession) {
        cout << "No session.\n";
        return;
    }
    if (g_currentSession.saveToFile())
        cout << "Session saved.\n";
    else
        cout << "Save failed.\n";
}

void loadSessionFromFile() {
    cout << "Enter session filename: ";
    string fname;
    cin >> fname; clearInput();

    AttendanceSession s;
    if (AttendanceSession::loadFromFile(fname, s)) {
        g_currentSession = s;
        g_hasCurrentSession = true;
        cout << "Session loaded.\n";
    } else {
        cout << "Load failed.\n";
    }
}

// ---------- Menus ----------
void studentsMenu() {
    int choice = -1;
    do {
        cout << "\n=== STUDENT MANAGEMENT ===\n"
             << "1. Add Student\n"
             << "2. View Students\n"
             << "3. Back\n";

        cin >> choice; clearInput();

        if (choice == 1) addStudent();
        else if (choice == 2) viewStudents();

    } while (choice != 3);
}

void sessionsMenu() {
    int choice = -1;
    do {
        cout << "\n=== ATTENDANCE SESSIONS ===\n"
             << "1. Create Session\n"
             << "2. Show Current Session\n"
             << "3. Mark Attendance\n"
             << "4. Update Attendance\n"
             << "5. Display Attendance\n"
             << "6. Save Current Session\n"
             << "7. Load Session From File\n"
             << "8. Export to Excel (CSV)\n"
             << "9. Back\n";

        cin >> choice; clearInput();

        if (choice == 1) createSession();
        else if (choice == 2) showCurrentSession();
        else if (choice == 3) markAttendance();
        else if (choice == 4) updateAttendance();
        else if (choice == 5) displayAttendanceList();
        else if (choice == 6) saveCurrentSession();
        else if (choice == 7) loadSessionFromFile();
        else if (choice == 8) exportSessionToCSV();

    } while (choice != 9);
}

// ---------- Main Program ----------
void mainMenu() {
    loadStudentsFromFile();

    int choice = -1;
    do {
        cout << "\n===== DIGITAL ATTENDANCE SYSTEM =====\n"
             << "1. Student Management\n"
             << "2. Attendance Sessions\n"
             << "3. Save Students\n"
             << "4. Exit\n";

        cin >> choice; clearInput();

        if (choice == 1) studentsMenu();
        else if (choice == 2) sessionsMenu();
        else if (choice == 3) 
            saveStudentsToFile() ? 
            cout << "Saved.\n" : cout << "Save failed.\n";

    } while (choice != 4);
}

int main() {
    mainMenu();
    return 0;
}





