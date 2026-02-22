# Digital Attendance System (C++)

This project is a Mid-Semester Capstone Project for EEE227 – Introduction to C++ Programming.

## Week 1 Progress

### Implemented Student Class
- Fields:
  - `indexNumber`
  - `fullName`
- Includes constructor, getters, and display method.

### Student Management (Week 1 Requirements)
- Add student
- View all registered students

### Project Structure (Week 1)

## Week 2 Progress
- Implemented `AttendanceSession` class
- Added menu-driven flow for sessions
- Can create a lecture session with: course code, date, start time, and duration
- Shows the current session details after creation

## Week 3 Progress
- Mark attendance with statuses: **Present / Absent / Late**
- Update existing attendance records
- Display attendance list for a session
- Show summary counts (Present, Absent, Late)
- Basic input validation (numeric choices, safe reads)

## Week 4 Progress
- Implemented **file persistence** using plain text files (`fstream`)
  - Students saved/loaded from `students.txt` (format: `INDEX|Full Name`)
  - Sessions saved/loaded from `session_<COURSE>_<DATE>.txt`
    - First line: `COURSE|DATE|STARTTIME|DURATION`
    - Following lines: `Index|STATUS` where STATUS ∈ {PRESENT, ABSENT, LATE}
- Loaded students automatically at program start
- Added menu options:
  - **Main Menu:** Save Students to File
  - **Sessions Menu:** Save Current Session, Load Session From File
- Minor refactors & comments for readability
