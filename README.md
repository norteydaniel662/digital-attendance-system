Digital Attendance System (C++)
This project is a Mid-Semester Capstone Project for EEE227 – Introduction to C++ Programming, developed using Visual Studio Code, g++, and GitHub version control.

 Project Overview
A console-based Digital Attendance System for hour-based lectures.
It supports:

Student registration
Lecture session creation
Attendance marking (Present / Absent / Late)
Updating attendance
Viewing attendance summaries
Saving & loading sessions
Exporting attendance to Excel-friendly CSV


Final Repository Structure
digital-attendance-system/
│── main.cpp
│── Student.h
│── Student.cpp
│── AttendanceSession.h
│── AttendanceSession.cpp
│── students.txt
│── attendance_export.csv
│── README.md


 Weekly Progress
 Week 1

Implemented Student class
Fields:

indexNumber
fullName


Added:

Add Student
View Students


Created basic project structure
Set up GitHub repository with initial commits


 Week 2

Implemented AttendanceSession class
Created lecture sessions with:

Course Code
Date
Start Time
Duration


Added Attendance Session menu system
Display current session details
Added meaningful commits showing progress


Week 3

Added full attendance features:

Mark attendance (Present / Absent / Late)
Update existing attendance records
Display attendance list
Attendance summary (Present / Absent / Late)


Improved input validation and menu navigation
Tested all functionalities


 Week 4
 File Persistence

Save students to students.txt
Load students automatically at startup
Save sessions to:

session_<COURSE>_<DATE>.txt


Load session from file

Final Touch: Export to Excel CSV

Added attendance_export.csv
Exported attendance list + summary into a format readable by Microsoft Excel
This file is included in the repository as per lecturer’s requirement


 Excel Output (CSV)
After running the program, attendance is exported into:
attendance_export.csv

This file includes:

Index Number
Status (PRESENT / ABSENT / LATE)
Summary counts

This file opens in Microsoft Excel without any conversion.
