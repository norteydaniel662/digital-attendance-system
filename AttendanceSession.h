#ifndef ATTENDANCE_SESSION_H
#define ATTENDANCE_SESSION_H

#include <string>
#include <vector>

enum class Status {
    Present = 1,
    Absent = 2,
    Late = 3
};

struct AttendanceRecord {
    std::string indexNumber;
    Status status;
};

class AttendanceSession {
private:
    std::string courseCode;
    std::string date;
    std::string startTime;
    int durationMinutes;
    std::vector<AttendanceRecord> records;

    static std::string sanitize(const std::string& s);

public:
    AttendanceSession();
    AttendanceSession(const std::string& c,
                      const std::string& d,
                      const std::string& t,
                      int dur);

    std::string getCourseCode() const;
    std::string getDate() const;
    std::string getStartTime() const;
    int getDurationMinutes() const;

    void addOrUpdateRecord(const std::string& indexNumber, Status status);
    bool updateRecord(const std::string& indexNumber, Status newStatus);

    const std::vector<AttendanceRecord>& getRecords() const;

    void summaryCounts(int& present, int& absent, int& late) const;

    std::string defaultFileName() const;

    bool saveToFile(const std::string& filePath = "") const;

    static bool loadFromFile(const std::string& filePath,
                             AttendanceSession& outSession);
};

#endif
