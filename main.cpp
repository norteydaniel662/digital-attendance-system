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
        cout << "Invalid input. Please enter a number between "
             << minVal << " and " << maxVal << ".\n";
        clearInput();
    }
}

// ---------- Week 4: Students persistence ----------
static bool saveStudentsToFile(const string& filename = "students.txt") {
    ofstream out(filename);
    if (!out) {
        cout << "ERROR: Could not open " << filename << " for writing.\n";
        return false;
    }
    for (const auto& s : g_students) {
        out << s.getIndexNumber() << "|" << s.getFullName() << "\n";
    }
    return true;
}

static bool loadStudentsFromFile(const string& filename = "students.txt") {
    ifstream in(filename);
    if (!in) {
        return false;
    }
    g_students.clear();

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string idx, name;
        if (!getline(iss, idx, '|')) continue;
        if (!getline(iss, name, '|')) continue;
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
        cout << "A student with this index already exists.\n";
        return;
    }

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
    cin >> course; clearInput();

    cout << "Date (YYYY-MM-DD): ";
    cin >> date; clearInput();

    cout << "Start Time (HH:MM): ";
    cin >> startTime; clearInput();

    duration = readInt("Duration (minutes): ", 0, 600);

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
         << "Start Time: " << g_currentCurrentSession.getStartTime() << "\n"
         << "Duration: " << g_currentSession.getDurationMinutes() << " minutes\n";
}

// ---------- Week 3 features ----------
static Status chooseStatus() {
    cout << "Choose status: \n"
         << "  1) Present\n"
         << "  2) Absent\n"
         << "  3) Late\n";
    int opt = readInt("Select 1-3: ", 1, 3);
    switch (opt) {
        case 1: return Status::Present;
        case 2: return Status::Absent;
        case 3: return Status::Late;
        default: return Status::Absent;
    }
}

void markAttendance() {
    if (!g_hasCurrentSession) {
        cout << "No active session. Create a session first.\n";
        return;
    }

    cout << "\n--- Mark Attendance ---\n";
    cout << "Enter index numbers one by one. Type 'DONE' to finish.\n";

    while (true) {
        cout << "Index Number (or DONE): ";
        string idx;
        cin >> idx; clearInput();

        if (idx == "DONE" || idx == "done") break;

        if (!indexExists(idx)) {
            cout << "WARNING: Index not found in student list.\n";
        }

        Status st = chooseStatus();
        g_currentSession.addOrUpdateRecord(idx, st);
        cout << "Recorded " << idx << ".\n";
    }
}

void updateAttendance() {
    if (!g_hasCurrentSession) {
        cout << "No active session. Create a session first.\n";
        return;
    }

    cout << "\n--- Update Attendance ---\n";
    cout << "Enter Index Number to update: ";
    string idx;
    cin >> idx; clearInput();

    Status st = chooseStatus();
    if (g_currentSession.updateRecord(idx, st)) {
        cout << "Updated record for " << idx << ".\n";
    } else {
        cout << "No record found for " << idx << ".\n";
    }
}

void displayAttendanceList() {
    if (!g_hasCurrentSession) {
        cout << "No active session.\n";
        return;
    }

    cout << "\n--- Attendance List for "
         << g_currentSession.getCourseCode() << " on "
         << g_currentSession.getDate() << " ---\n";

    const auto& recs = g_currentSession.getRecords();
    if (recs.empty()) {
        cout << "No attendance records yet.\n";
    } else {
        for (const auto& r : recs) {
            cout << "Index: " << r.indexNumber << "  Status: ";
            switch (r.status) {
                case Status::Present: cout << "PRESENT"; break;
                case Status::Absent:  cout << "ABSENT";  break;
                case Status::Late:    cout << "LATE";    break;
            }
            cout << "\n";
        }
    }

    int p, a, l;
    g_currentSession.summaryCounts(p, a, l);
    cout << "Summary -> Present: " << p
         << ", Absent: "  << a
         << ", Late: "    << l << "\n";
}

// ---------- Week 4: Export to Excel-friendly CSV ----------
void exportSessionToCSV() {
    if (!g_hasCurrentSession) {
        cout << "No active session to export.\n";
        return;
    }

    string filename = "attendance_export.csv";
    ofstream out(filename);

    if (!out) {
        cout << "Failed to create CSV file.\n";
        return;
    }

    out << "Index Number,Status\n";

    for (const auto& r : g_currentSession.getRecords()) {
        string statusText;
        switch (r.status) {
            case Status::Present: statusText = "PRESENT"; break;
            case Status::Absent:  statusText = "ABSENT";  break;
            case Status::Late:    statusText = "LATE";    break;
        }
        out << r.indexNumber << "," << statusText << "\n";
    }

    int p, a, l;
    g_currentSession.summaryCounts(p, a, l);

    out << "\nSummary,,\n";
    out << "Present," << p << "\n";
    out << "Absent,"  << a << "\n";
    out << "Late,"    << l << "\n";

    out.close();

    cout << "Exported to " << filename << "\n";
}

void saveCurrentSession() {
    if (!g_hasCurrentSession) {
        cout << "No active session to save.\n";
        return;
    }
    if (g_currentSession.saveToFile()) {
        cout << "Session saved to " << g_currentSession.defaultFileName() << "\n";
    } else {
        cout << "Failed to save session.\n";
    }
}

void loadSessionFromFile() {
    cout << "\nEnter session filename: ";
    string fname;
    cin >> fname; clearInput();

    AttendanceSession s;
    if (AttendanceSession::loadFromFile(fname, s)) {
        g_currentSession = s;
        g_hasCurrentSession = true;
        cout << "Session loaded.\n";
    } else {
        cout << "Failed to load session.\n";
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

        if (!(cin >> choice)) { clearInput(); continue; }

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
        cout << "\n=== LECTURE SESSIONS ===\n"
             << "1. Create Session\n"
             << "2. Show Current Session\n"
             << "3. Mark Attendance\n"
             << "4. Update Attendance\n"
             << "5. Display Attendance + Summary\n"
             << "6. Save Current Session\n"
             << "7. Load Session From File\n"
             << "8. Export Attendance to Excel (CSV)\n"
             << "9. Back\n";

        if (!(cin >> choice)) { clearInput(); continue; }

        switch (choice) {
            case 1: createSession(); break;
            case 2: showCurrentSession(); break;
            case 3: markAttendance(); break;
            case 4: updateAttendance(); break;
            case 5: displayAttendanceList(); break;
            case 6: saveCurrentSession(); break;
            case 7: loadSessionFromFile(); break;
            case 8: exportSessionToCSV(); break;
            case 9: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 9);
}

void mainMenu() {
    loadStudentsFromFile(); // Auto-load

    int choice = -1;
    do {
        cout << "\n===== DIGITAL ATTENDANCE SYSTEM =====\n"
             << "1. Student Management\n"
             << "2. Attendance Sessions\n"
             << "3. Save Students to File\n"
             << "4. Exit\n";

        if (!(cin >> choice)) { clearInput(); continue; }

        switch (choice) {
            case 1: studentsMenu(); break;
            case 2: sessionsMenu(); break;
            case 3:
                if (saveStudentsToFile()) cout << "Students saved.\n";
                else cout << "Failed.\n";
                break;
            case 4: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 4);
}

int main() {
    mainMenu();
    return 0;
}




