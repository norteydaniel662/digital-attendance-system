#include "AttendanceSession.h"
#include <fstream>
#include <sstream>
#include <algorithm>

AttendanceSession::AttendanceSession()
    : durationMinutes(0) {}

AttendanceSession::AttendanceSession(const std::string& c,
                                     const std::string& d,
                                     const std::string& t,
                                     int dur)
    : courseCode(c), date(d), startTime(t), durationMinutes(dur) {}

std::string AttendanceSession::sanitize(const std::string& s) {
    std::string out = s;
    for (char& ch : out) {
        if (!(isalnum(ch) || ch == '_' || ch == '-'))
            ch = '_';
    }
    return out;
}

std::string AttendanceSession::getCourseCode() const { return courseCode; }
std::string AttendanceSession::getDate() const { return date; }
std::string AttendanceSession::getStartTime() const { return startTime; }
int AttendanceSession::getDurationMinutes() const { return durationMinutes; }

void AttendanceSession::addOrUpdateRecord(const std::string& index, Status status) {
    for (auto& rec : records) {
        if (rec.indexNumber == index) {
            rec.status = status;
            return;
        }
    }
    records.push_back({ index, status });
}

bool AttendanceSession::updateRecord(const std::string& index, Status newStatus) {
    for (auto& rec : records) {
        if (rec.indexNumber == index) {
            rec.status = newStatus;
            return true;
        }
    }
    return false;
}

const std::vector<AttendanceRecord>& AttendanceSession::getRecords() const {
    return records;
}

void AttendanceSession::summaryCounts(int& p, int& a, int& l) const {
    p = a = l = 0;
    for (const auto& rec : records) {
        if (rec.status == Status::Present) p++;
        else if (rec.status == Status::Absent) a++;
        else l++;
    }
}

std::string AttendanceSession::defaultFileName() const {
    return "session_" + sanitize(courseCode) + "_" + sanitize(date) + ".txt";
}

static std::string statusToString(Status s) {
    if (s == Status::Present) return "PRESENT";
    if (s == Status::Absent) return "ABSENT";
    return "LATE";
}

static Status stringToStatus(const std::string& s) {
    if (s == "PRESENT") return Status::Present;
    if (s == "ABSENT") return Status::Absent;
    return Status::Late;
}

bool AttendanceSession::saveToFile(const std::string& filePath) const {
    std::string path = filePath.empty() ? defaultFileName() : filePath;
    std::ofstream out(path);
    if (!out) return false;

    out << courseCode << "|" << date << "|" << startTime << "|" << durationMinutes << "\n";

    for (const auto& rec : records) {
        out << rec.indexNumber << "|" << statusToString(rec.status) << "\n";
    }
    return true;
}

bool AttendanceSession::loadFromFile(const std::string& filePath,
                                     AttendanceSession& outSession) {
    std::ifstream in(filePath);
    if (!in) return false;

    std::string line;
    if (!std::getline(in, line)) return false;

    std::istringstream iss(line);
    std::string c, d, t, dur;
    getline(iss, c, '|');
    getline(iss, d, '|');
    getline(iss, t, '|');
    getline(iss, dur, '|');

    outSession = AttendanceSession(c, d, t, stoi(dur));

    while (getline(in, line)) {
        std::istringstream rs(line);
        std::string idx, st;
        getline(rs, idx, '|');
        getline(rs, st, '|');
        outSession.addOrUpdateRecord(idx, stringToStatus(st));
    }

    return true;
}
