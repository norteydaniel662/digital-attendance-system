#include "AttendanceSession.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string statusToString(Status s) {
    switch (s) {
        case Status::Present: return "PRESENT";
        case Status::Absent:  return "ABSENT";
        case Status::Late:    return "LATE";
        default: return "UNKNOWN";
    }
}

static Status stringToStatus(const std::string& s) {
    if (s == "PRESENT") return Status::Present;
    if (s == "ABSENT")  return Status::Absent;
    if (s == "LATE")    return Status::Late;
    return Status::Absent;
}

std::string AttendanceSession::sanitize(const std::string& s) {
    std::string out = s;
    for (char& ch : out) {
        if (!((ch >= 'a' && ch <= 'z') ||
              (ch >= 'A' && ch <= 'Z') ||
              (ch >= '0' && ch <= '9') ||
              ch == '_' || ch == '-')) {
            ch = '_';
        }
    }
    return out;
}

AttendanceSession::AttendanceSession(const std::string& c,
                                     const std::string& d,
                                     const std::string& t,
                                     int dur)
    : courseCode(c), date(d), startTime(t), durationMinutes(dur) {}

std::string AttendanceSession::getCourseCode() const { return courseCode; }
std::string AttendanceSession::getDate() const { return date; }
std::string AttendanceSession::getStartTime() const { return startTime; }
int AttendanceSession::getDurationMinutes() const { return durationMinutes; }

const std::vector<AttendanceRecord>& AttendanceSession::getRecords() const {
    return records;
}

void AttendanceSession::addOrUpdateRecord(const std::string& indexNumber, Status status) {
    for (auto& r : records) {
        if (r.indexNumber == indexNumber) {
            r.status = status;
            return;
        }
    }
    records.push_back({ indexNumber, status });
}

bool AttendanceSession::updateRecord(const std::string& indexNumber, Status newStatus) {
    for (auto& r : records) {
        if (r.indexNumber == indexNumber) {
            r.status = newStatus;
            return true;
        }
    }
    return false;
}

void AttendanceSession::summaryCounts(int& present, int& absent, int& late) const {
    present = absent = late = 0;
    for (const auto& r : records) {
        switch (r.status) {
            case Status::Present: ++present; break;
            case Status::Absent:  ++absent;  break;
            case Status::Late:    ++late;    break;
        }
    }
}

std::string AttendanceSession::defaultFileName() const {
    return "session_" + sanitize(courseCode) + "_" + sanitize(date) + ".txt";
}

bool AttendanceSession::saveToFile(const std::string& filePath) const {
    std::string path = filePath.empty() ? defaultFileName() : filePath;
    std::ofstream out(path);
    if (!out) return false;

    out << courseCode << "|" << date << "|" << startTime << "|" << durationMinutes << "\n";

    for (const auto& r : records) {
        out << r.indexNumber << "|" << statusToString(r.status) << "\n";
    }
    return true;
}

bool AttendanceSession::loadFromFile(const std::string& filePath,
                                     AttendanceSession& outSession) {
    std::ifstream in(filePath);
    if (!in) return false;

    std::string line;
    if (!std::getline(in, line)) return false;

    std::istringstream header(line);
    std::string c, d, t, durStr;
    std::getline(header, c, '|');
    std::getline(header, d, '|');
    std::getline(header, t, '|');
    std::getline(header, durStr, '|');

    int dur = std::stoi(durStr);
    outSession = AttendanceSession(c, d, t, dur);

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string idx, stat;
        std::getline(iss, idx, '|');
        std::getline(iss, stat, '|');

        outSession.addOrUpdateRecord(idx, stringToStatus(stat));
    }

    return true;
}
