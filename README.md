#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_ROWS 500
#define MAX_COLS 15
#define MAX_CELL 120
#define STUDENTS_FILE "students.txt"
#define COURSES_FILE "courses.txt"
#define ENROLLMENTS_FILE "enrollments.txt"
#define ATTENDANCE_FILE "attendance_log.txt"
#define FEES_FILE "fees.txt"

FILE *reportOut = NULL;

void outprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(reportOut == NULL ? stdout : reportOut, format, args);
    va_end(args);
}

void pauseScreen() {
    int ch;
    printf("\nPress Enter to continue...");
    while ((ch = getchar()) != '\n' && ch != EOF) { }
    getchar();
}

void clearInputLine() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

void readLine(char text[], int size) {
    fgets(text, size, stdin);
    int len = strlen(text);
    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }
}

int isDigitChar(char ch) {
    return ch >= '0' && ch <= '9';
}

double textToDouble(const char text[]) {
    return atof(text);
}

void doubleToText(double value, int decimals, char output[]) {
    char format[10];
    sprintf(format, "%%.%df", decimals);
    sprintf(output, format, value);
}

void copyRow(char dest[MAX_COLS][MAX_CELL], char src[MAX_COLS][MAX_CELL], int colCount) {
    int i;
    for (i = 0; i < colCount; i++) {
        strcpy(dest[i], src[i]);
    }
}

void clearRow(char row[MAX_COLS][MAX_CELL], int colCount) {
    int i;
    for (i = 0; i < colCount; i++) {
        row[i][0] = '\0';
    }
}

void parseCSVLine(char line[], char row[MAX_COLS][MAX_CELL], int colCount) {
    int i = 0, col = 0, pos = 0, inQuotes = 0;
    clearRow(row, colCount);
    while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r') {
        char ch = line[i];
        if (ch == '"') {
            if (inQuotes == 1 && line[i + 1] == '"') {
                if (pos < MAX_CELL - 1) {
                    row[col][pos++] = '"';
                    row[col][pos] = '\0';
                }
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && inQuotes == 0) {
            if (col < colCount - 1) {
                col++;
                pos = 0;
            }
        } else {
            if (col < colCount && pos < MAX_CELL - 1) {
                row[col][pos++] = ch;
                row[col][pos] = '\0';
            }
        }
        i++;
    }
}

int readTXT(const char filename[], char rows[MAX_ROWS][MAX_COLS][MAX_CELL], int colCount) {
    FILE *file = fopen(filename, "r");
    char line[1000];
    int count = 0;
    if (file == NULL) {
        return 0;
    }
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ROWS) {
        if (strlen(line) > 1) {
            parseCSVLine(line, rows[count], colCount);
            count++;
        }
    }
    fclose(file);
    return count;
}

void writeField(FILE *file, const char field[]) {
    int needQuotes = 0;
    int i;
    for (i = 0; field[i] != '\0'; i++) {
        if (field[i] == ',' || field[i] == '"') {
            needQuotes = 1;
        }
    }
    if (needQuotes == 0) {
        fprintf(file, "%s", field);
    } else {
        fprintf(file, "\"");
        for (i = 0; field[i] != '\0'; i++) {
            if (field[i] == '"') {
                fprintf(file, "\"\"");
            } else {
                fprintf(file, "%c", field[i]);
            }
        }
        fprintf(file, "\"");
    }
}

void writeTXT(const char filename[], const char header[], char rows[MAX_ROWS][MAX_COLS][MAX_CELL], int rowCount, int colCount) {
    FILE *file = fopen(filename, "w");
    int i, j;
    if (file == NULL) {
        return;
    }
    fprintf(file, "%s\n", header);
    for (i = 0; i < rowCount; i++) {
        for (j = 0; j < colCount; j++) {
            writeField(file, rows[i][j]);
            if (j != colCount - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void appendTXT(const char filename[], char row[MAX_COLS][MAX_CELL], int colCount) {
    FILE *file = fopen(filename, "a");
    int j;
    if (file == NULL) {
        return;
    }
    for (j = 0; j < colCount; j++) {
        writeField(file, row[j]);
        if (j != colCount - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");
    fclose(file);
}

int findRow(const char filename[], int colIndex, const char value[], char output[MAX_COLS][MAX_CELL], int colCount) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(filename, rows, colCount);
    int i;
    clearRow(output, colCount);
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][colIndex], value) == 0) {
            copyRow(output, rows[i], colCount);
            return 1;
        }
    }
    return 0;
}

int rowExists(const char filename[], int colIndex, const char value[], int colCount) {
    char row[MAX_COLS][MAX_CELL];
    return findRow(filename, colIndex, value, row, colCount);
}

int textContains(const char text[], const char key[]) {
    int i, j, matched;
    int textLen = strlen(text);
    int keyLen = strlen(key);
    if (keyLen == 0) {
        return 1;
    }
    if (keyLen > textLen) {
        return 0;
    }
    for (i = 0; i <= textLen - keyLen; i++) {
        matched = 1;
        for (j = 0; j < keyLen; j++) {
            if (text[i + j] != key[j]) {
                matched = 0;
            }
        }
        if (matched == 1) {
            return 1;
        }
    }
    return 0;
}

int startsWithText(const char text[], const char prefix[]) {
    int i;
    int textLen = strlen(text);
    int prefixLen = strlen(prefix);
    if (prefixLen > textLen) {
        return 0;
    }
    for (i = 0; i < prefixLen; i++) {
        if (text[i] != prefix[i]) {
            return 0;
        }
    }
    return 1;
}

int validateRollFormat(const char roll[]) {
    if (strlen(roll) != 11) {
        return 0;
    }
    if (roll[0] != 'B' || roll[1] != 'S' || roll[2] != 'A' || roll[3] != 'I') {
        return 0;
    }
    if (roll[4] != '-' || roll[7] != '-') {
        return 0;
    }
    if (!isDigitChar(roll[5]) || !isDigitChar(roll[6])) {
        return 0;
    }
    if (!isDigitChar(roll[8]) || !isDigitChar(roll[9]) || !isDigitChar(roll[10])) {
        return 0;
    }
    return 1;
}

int validateStudentName(const char name[]) {
    int i;
    if (strlen(name) == 0) {
        return 0;
    }
    for (i = 0; name[i] != '\0'; i++) {
        if (isDigitChar(name[i])) {
            return 0;
        }
    }
    return 1;
}

int addStudent(const char roll[], const char name[], const char dept[], double cgpa) {
    char row[MAX_COLS][MAX_CELL];
    if (!validateRollFormat(roll)) {
        return 0;
    }
    if (!validateStudentName(name)) {
        return 0;
    }
    if (cgpa < 0.0 || cgpa > 4.0) {
        return 0;
    }
    if (rowExists(STUDENTS_FILE, 0, roll, 5)) {
        return 0;
    }
    clearRow(row, 5);
    strcpy(row[0], roll);
    strcpy(row[1], name);
    strcpy(row[2], dept);
    doubleToText(cgpa, 2, row[3]);
    strcpy(row[4], "active");
    appendTXT(STUDENTS_FILE, row, 5);
    return 1;
}

int searchByRoll(const char roll[], char output[MAX_COLS][MAX_CELL]) {
    return findRow(STUDENTS_FILE, 0, roll, output, 5);
}

int searchByName(const char key[], char result[MAX_ROWS][MAX_COLS][MAX_CELL]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(STUDENTS_FILE, rows, 5);
    int i, outCount = 0;
    for (i = 0; i < count; i++) {
        if (textContains(rows[i][1], key)) {
            copyRow(result[outCount], rows[i], 5);
            outCount++;
        }
    }
    return outCount;
}

int updateStudent(const char roll[], int fieldIndex, const char newValue[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(STUDENTS_FILE, rows, 5);
    int i, found = 0;
    if (fieldIndex <= 0 || fieldIndex > 4) {
        return 0;
    }
    if (fieldIndex == 1 && !validateStudentName(newValue)) {
        return 0;
    }
    if (fieldIndex == 3) {
        double cgpa = textToDouble(newValue);
        if (cgpa < 0.0 || cgpa > 4.0) {
            return 0;
        }
    }
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0) {
            strcpy(rows[i][fieldIndex], newValue);
            found = 1;
        }
    }
    if (found) {
        writeTXT(STUDENTS_FILE, "roll,name,dept,cgpa,status", rows, count, 5);
    }
    return found;
}

int softDelete(const char roll[]) {
    return updateStudent(roll, 4, "inactive");
}

void swapRows(char rows[MAX_ROWS][MAX_COLS][MAX_CELL], int a, int b, int colCount) {
    char temp[MAX_COLS][MAX_CELL];
    copyRow(temp, rows[a], colCount);
    copyRow(rows[a], rows[b], colCount);
    copyRow(rows[b], temp, colCount);
}

int listActiveStudents(char active[MAX_ROWS][MAX_COLS][MAX_CELL]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(STUDENTS_FILE, rows, 5);
    int i, j, minIndex, outCount = 0;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][4], "active") == 0) {
            copyRow(active[outCount], rows[i], 5);
            outCount++;
        }
    }
    for (i = 0; i < outCount - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < outCount; j++) {
            if (strcmp(active[j][0], active[minIndex][0]) < 0) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swapRows(active, i, minIndex, 5);
        }
    }
    return outCount;
}

void printStudentsTable(char rows[MAX_ROWS][MAX_COLS][MAX_CELL], int count) {
    int i;
    printf("%-14s%-25s%-12s%-8s%-10s\n", "Roll", "Name", "Dept", "CGPA", "Status");
    printf("---------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-14s%-25s%-12s%-8s%-10s\n", rows[i][0], rows[i][1], rows[i][2], rows[i][3], rows[i][4]);
    }
}

void searchAsYouType() {
    char prefix[MAX_CELL] = "";
    char input[MAX_CELL];
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    char matched[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(STUDENTS_FILE, rows, 5);
    int i, matchedCount, len;
    printf("Type one character at a time. Enter 0 to stop.\n");
    while (1) {
        printf("Next character: ");
        scanf("%119s", input);
        if (strcmp(input, "0") == 0) {
            break;
        }
        len = strlen(prefix);
        if (strlen(input) > 0 && len < MAX_CELL - 2) {
            prefix[len] = input[0];
            prefix[len + 1] = '\0';
        }
        matchedCount = 0;
        for (i = 0; i < count; i++) {
            if (startsWithText(rows[i][1], prefix) || startsWithText(rows[i][0], prefix)) {
                copyRow(matched[matchedCount], rows[i], 5);
                matchedCount++;
            }
        }
        printf("\nPrefix: %s\n", prefix);
        printStudentsTable(matched, matchedCount);
        printf("\n");
    }
}

int findCourseByCode(const char courseCode[], char output[MAX_COLS][MAX_CELL]) {
    return findRow(COURSES_FILE, 0, courseCode, output, 5);
}

int getCourseCredits(const char courseCode[]) {
    char course[MAX_COLS][MAX_CELL];
    if (findCourseByCode(courseCode, course)) {
        return (int)textToDouble(course[2]);
    }
    return 0;
}

int getCourseSeats(const char courseCode[]) {
    char course[MAX_COLS][MAX_CELL];
    if (findCourseByCode(courseCode, course)) {
        return (int)textToDouble(course[3]);
    }
    return 0;
}

int activeEnrollCount(const char courseCode[], const char semester[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, rows, 10);
    int i, total = 0;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][1], courseCode) == 0 && strcmp(rows[i][2], semester) == 0 && strcmp(rows[i][3], "enrolled") == 0) {
            total++;
        }
    }
    return total;
}

int alreadyEnrolled(const char roll[], const char courseCode[], const char semester[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, rows, 10);
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], courseCode) == 0 && strcmp(rows[i][2], semester) == 0 && strcmp(rows[i][3], "enrolled") == 0) {
            return 1;
        }
    }
    return 0;
}

int getCreditLoad(const char roll[], const char semester[]) {
    char enrollments[MAX_ROWS][MAX_COLS][MAX_CELL];
    char courses[MAX_ROWS][MAX_COLS][MAX_CELL];
    int enrollmentCount = readTXT(ENROLLMENTS_FILE, enrollments, 10);
    int courseCount = readTXT(COURSES_FILE, courses, 5);
    int i, j, totalCredits = 0;
    for (i = 0; i < enrollmentCount; i++) {
        if (strcmp(enrollments[i][0], roll) == 0 && strcmp(enrollments[i][2], semester) == 0 && strcmp(enrollments[i][3], "enrolled") == 0) {
            for (j = 0; j < courseCount; j++) {
                if (strcmp(courses[j][0], enrollments[i][1]) == 0) {
                    totalCredits += (int)textToDouble(courses[j][2]);
                }
            }
        }
    }
    return totalCredits;
}

int checkPrerequisite(const char roll[], const char courseCode[]) {
    char course[MAX_COLS][MAX_CELL];
    char enrollments[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count, i;
    if (!findCourseByCode(courseCode, course)) {
        return 0;
    }
    if (strcmp(course[4], "NONE") == 0) {
        return 1;
    }
    count = readTXT(ENROLLMENTS_FILE, enrollments, 10);
    for (i = 0; i < count; i++) {
        if (strcmp(enrollments[i][0], roll) == 0 && strcmp(enrollments[i][1], course[4]) == 0) {
            if (strlen(enrollments[i][9]) > 0 && strcmp(enrollments[i][9], "F") != 0) {
                return 1;
            }
        }
    }
    return 0;
}

int enrollStudent(const char roll[], const char courseCode[], const char semester[]) {
    char student[MAX_COLS][MAX_CELL];
    char course[MAX_COLS][MAX_CELL];
    char row[MAX_COLS][MAX_CELL];
    int newLoad;
    if (!searchByRoll(roll, student) || strcmp(student[4], "active") != 0) {
        return 1;
    }
    if (!findCourseByCode(courseCode, course)) {
        return 2;
    }
    if (activeEnrollCount(courseCode, semester) >= getCourseSeats(courseCode)) {
        return 3;
    }
    if (alreadyEnrolled(roll, courseCode, semester)) {
        return 4;
    }
    newLoad = getCreditLoad(roll, semester) + getCourseCredits(courseCode);
    if (newLoad > 21) {
        return 5;
    }
    if (!checkPrerequisite(roll, courseCode)) {
        return 6;
    }
    clearRow(row, 10);
    strcpy(row[0], roll);
    strcpy(row[1], courseCode);
    strcpy(row[2], semester);
    strcpy(row[3], "enrolled");
    appendTXT(ENROLLMENTS_FILE, row, 10);
    return 0;
}

void enrollResultToString(int result, char message[]) {
    if (result == 0) strcpy(message, "Enrollment successful.");
    else if (result == 1) strcpy(message, "Student is not active or does not exist.");
    else if (result == 2) strcpy(message, "Course does not exist.");
    else if (result == 3) strcpy(message, "No seats available.");
    else if (result == 4) strcpy(message, "Student is already enrolled.");
    else if (result == 5) strcpy(message, "Credit load exceeds 21 hours.");
    else strcpy(message, "Prerequisite not passed.");
}

int dropCourse(const char roll[], const char courseCode[], const char semester[]) {
    char attendance[MAX_ROWS][MAX_COLS][MAX_CELL];
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int attCount = readTXT(ATTENDANCE_FILE, attendance, 4);
    int count = readTXT(ENROLLMENTS_FILE, rows, 10);
    int i, changed = 0;
    for (i = 0; i < attCount; i++) {
        if (strcmp(attendance[i][0], roll) == 0 && strcmp(attendance[i][1], courseCode) == 0) {
            return 0;
        }
    }
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], courseCode) == 0 && strcmp(rows[i][2], semester) == 0 && strcmp(rows[i][3], "enrolled") == 0) {
            strcpy(rows[i][3], "dropped");
            changed = 1;
        }
    }
    if (changed) {
        writeTXT(ENROLLMENTS_FILE, "roll,course_code,semester,status,quiz_avg,assignment_avg,mid,final,total,grade", rows, count, 10);
    }
    return changed;
}

int listEnrolledStudents(const char courseCode[], const char semester[], char result[MAX_ROWS][MAX_COLS][MAX_CELL]) {
    char enrollments[MAX_ROWS][MAX_COLS][MAX_CELL];
    char student[MAX_COLS][MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, enrollments, 10);
    int i, outCount = 0;
    for (i = 0; i < count; i++) {
        if (strcmp(enrollments[i][1], courseCode) == 0 && strcmp(enrollments[i][2], semester) == 0 && strcmp(enrollments[i][3], "enrolled") == 0) {
            if (searchByRoll(enrollments[i][0], student) && strcmp(student[4], "active") == 0) {
                copyRow(result[outCount], student, 5);
                outCount++;
            }
        }
    }
    return outCount;
}

void printCourseList() {
    char courses[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(COURSES_FILE, courses, 5);
    int i;
    printf("%-10s%-25s%-8s%-8s%-10s\n", "Code", "Title", "Credit", "Seats", "Prereq");
    printf("------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-10s%-25s%-8s%-8s%-10s\n", courses[i][0], courses[i][1], courses[i][2], courses[i][3], courses[i][4]);
    }
}

char attendanceBackup[MAX_ROWS][MAX_COLS][MAX_CELL];
int attendanceBackupCount = 0;
int attendanceBackupReady = 0;

int validAttendanceStatus(const char status[]) {
    return strcmp(status, "P") == 0 || strcmp(status, "A") == 0 || strcmp(status, "L") == 0;
}

int markAttendance(const char courseCode[], const char semester[], const char date[]) {
    char students[MAX_ROWS][MAX_COLS][MAX_CELL];
    char row[MAX_COLS][MAX_CELL];
    char status[MAX_CELL];
    int count = listEnrolledStudents(courseCode, semester, students);
    int i;
    if (count == 0) {
        return 0;
    }
    attendanceBackupCount = readTXT(ATTENDANCE_FILE, attendanceBackup, 4);
    attendanceBackupReady = 1;
    printf("Mark P, A, or L for each student.\n");
    for (i = 0; i < count; i++) {
        do {
            printf("%s - %s: ", students[i][0], students[i][1]);
            scanf("%119s", status);
        } while (!validAttendanceStatus(status));
        clearRow(row, 4);
        strcpy(row[0], students[i][0]);
        strcpy(row[1], courseCode);
        strcpy(row[2], date);
        strcpy(row[3], status);
        appendTXT(ATTENDANCE_FILE, row, 4);
    }
    return 1;
}

double getAttendancePct(const char roll[], const char courseCode[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(ATTENDANCE_FILE, rows, 4);
    int i, totalSessions = 0;
    double score = 0.0;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], courseCode) == 0) {
            totalSessions++;
            if (strcmp(rows[i][3], "P") == 0) score += 1.0;
            else if (strcmp(rows[i][3], "L") == 0) score += 0.5;
        }
    }
    if (totalSessions == 0) return 0.0;
    return (score / totalSessions) * 100.0;
}

int getShortageList(const char courseCode[], const char semester[], char result[MAX_ROWS][MAX_COLS][MAX_CELL]) {
    char students[MAX_ROWS][MAX_COLS][MAX_CELL];
    char pctText[MAX_CELL];
    int count = listEnrolledStudents(courseCode, semester, students);
    int i, outCount = 0;
    double pct;
    for (i = 0; i < count; i++) {
        pct = getAttendancePct(students[i][0], courseCode);
        if (pct < 75.0) {
            clearRow(result[outCount], 4);
            strcpy(result[outCount][0], students[i][0]);
            strcpy(result[outCount][1], students[i][1]);
            strcpy(result[outCount][2], courseCode);
            doubleToText(pct, 2, pctText);
            strcpy(result[outCount][3], pctText);
            outCount++;
        }
    }
    return outCount;
}

int undoLastSession() {
    if (!attendanceBackupReady) {
        return 0;
    }
    writeTXT(ATTENDANCE_FILE, "roll,course_code,date,status", attendanceBackup, attendanceBackupCount, 4);
    attendanceBackupReady = 0;
    return 1;
}

void printDailySheet(const char courseCode[], const char semester[], const char date[]) {
    char students[MAX_ROWS][MAX_COLS][MAX_CELL];
    char logs[MAX_ROWS][MAX_COLS][MAX_CELL];
    int studentCount = listEnrolledStudents(courseCode, semester, students);
    int logCount = readTXT(ATTENDANCE_FILE, logs, 4);
    int i, j;
    char status[MAX_CELL];
    printf("\nDaily Attendance Sheet\n");
    printf("Course: %s   Semester: %s   Date: %s\n", courseCode, semester, date);
    printf("%-14s%-25s%-10s\n", "Roll", "Name", "Status");
    printf("-----------------------------------------------------\n");
    for (i = 0; i < studentCount; i++) {
        strcpy(status, "-");
        for (j = 0; j < logCount; j++) {
            if (strcmp(logs[j][0], students[i][0]) == 0 && strcmp(logs[j][1], courseCode) == 0 && strcmp(logs[j][2], date) == 0) {
                strcpy(status, logs[j][3]);
            }
        }
        printf("%-14s%-25s%-10s\n", students[i][0], students[i][1], status);
    }
}

int validRange(double value, double low, double high) {
    return value >= low && value <= high;
}

double averageArray(double marks[], int count) {
    int i;
    double sum = 0.0;
    if (count == 0) return 0.0;
    for (i = 0; i < count; i++) sum += marks[i];
    return sum / count;
}

double bestThreeOfFive(double marks[], int count) {
    int i, r, lowIndex, removeCount;
    int removed[5];
    double sum = 0.0;
    int used = 0;
    if (count == 0) return 0.0;
    if (count < 4) return averageArray(marks, count);
    for (i = 0; i < 5; i++) removed[i] = 0;
    removeCount = count - 3;
    for (r = 0; r < removeCount; r++) {
        lowIndex = -1;
        for (i = 0; i < count; i++) {
            if (!removed[i]) {
                if (lowIndex == -1 || marks[i] < marks[lowIndex]) {
                    lowIndex = i;
                }
            }
        }
        if (lowIndex != -1) removed[lowIndex] = 1;
    }
    for (i = 0; i < count; i++) {
        if (!removed[i]) {
            sum += marks[i];
            used++;
        }
    }
    if (used == 0) return 0.0;
    return sum / used;
}

double computeWeightedTotal(double quiz, double asgn, double mid, double finalMark) {
    return quiz * 0.10 + asgn * 0.10 + mid * 0.30 + finalMark * 0.50;
}

void getLetterGrade(double total, char grade[]) {
    if (total >= 85.0) strcpy(grade, "A");
    else if (total >= 80.0) strcpy(grade, "B+");
    else if (total >= 70.0) strcpy(grade, "B");
    else if (total >= 65.0) strcpy(grade, "C+");
    else if (total >= 60.0) strcpy(grade, "C");
    else if (total >= 50.0) strcpy(grade, "D");
    else strcpy(grade, "F");
}

double gradePoint(const char grade[]) {
    if (strcmp(grade, "A") == 0) return 4.00;
    if (strcmp(grade, "B+") == 0) return 3.50;
    if (strcmp(grade, "B") == 0) return 3.00;
    if (strcmp(grade, "C+") == 0) return 2.50;
    if (strcmp(grade, "C") == 0) return 2.00;
    if (strcmp(grade, "D") == 0) return 1.00;
    return 0.00;
}

void applyAttendancePenalty(const char roll[], const char courseCode[], char grade[]) {
    if (getAttendancePct(roll, courseCode) < 75.0) {
        strcpy(grade, "F");
    }
}

int enterMarks(const char roll[], const char courseCode[], const char semester[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, rows, 10);
    int i, rowIndex = -1, quizCount, asgnCount;
    double quizMarks[5], asgnMarks[5];
    double mark, quizAvg, asgnAvg, mid, finalMark, midPct, finalPct, total;
    char totalText[MAX_CELL], grade[MAX_CELL];
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], courseCode) == 0 && strcmp(rows[i][2], semester) == 0 && strcmp(rows[i][3], "enrolled") == 0) {
            rowIndex = i;
        }
    }
    if (rowIndex == -1) return 0;
    printf("How many quiz marks? 1 to 5: ");
    scanf("%d", &quizCount);
    if (quizCount < 1) quizCount = 1;
    if (quizCount > 5) quizCount = 5;
    for (i = 0; i < quizCount; i++) {
        do {
            printf("Quiz %d out of 100: ", i + 1);
            scanf("%lf", &mark);
        } while (!validRange(mark, 0.0, 100.0));
        quizMarks[i] = mark;
    }
    quizAvg = bestThreeOfFive(quizMarks, quizCount);
    printf("How many assignment marks? 1 to 5: ");
    scanf("%d", &asgnCount);
    if (asgnCount < 1) asgnCount = 1;
    if (asgnCount > 5) asgnCount = 5;
    for (i = 0; i < asgnCount; i++) {
        do {
            printf("Assignment %d out of 100: ", i + 1);
            scanf("%lf", &mark);
        } while (!validRange(mark, 0.0, 100.0));
        asgnMarks[i] = mark;
    }
    asgnAvg = averageArray(asgnMarks, asgnCount);
    do {
        printf("Mid marks out of 40: ");
        scanf("%lf", &mid);
    } while (!validRange(mid, 0.0, 40.0));
    do {
        printf("Final marks out of 60: ");
        scanf("%lf", &finalMark);
    } while (!validRange(finalMark, 0.0, 60.0));
    midPct = (mid / 40.0) * 100.0;
    finalPct = (finalMark / 60.0) * 100.0;
    total = computeWeightedTotal(quizAvg, asgnAvg, midPct, finalPct);
    getLetterGrade(total, grade);
    applyAttendancePenalty(roll, courseCode, grade);
    doubleToText(quizAvg, 2, rows[rowIndex][4]);
    doubleToText(asgnAvg, 2, rows[rowIndex][5]);
    doubleToText(mid, 2, rows[rowIndex][6]);
    doubleToText(finalMark, 2, rows[rowIndex][7]);
    doubleToText(total, 2, rows[rowIndex][8]);
    strcpy(rows[rowIndex][9], grade);
    writeTXT(ENROLLMENTS_FILE, "roll,course_code,semester,status,quiz_avg,assignment_avg,mid,final,total,grade", rows, count, 10);
    doubleToText(total, 2, totalText);
    printf("Total: %s   Grade: %s\n", totalText, grade);
    return 1;
}

double computeGPA(const char roll[], const char semester[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, rows, 10);
    int i, credits, totalCredits = 0;
    double weighted = 0.0;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][2], semester) == 0 && strcmp(rows[i][3], "enrolled") == 0 && strlen(rows[i][9]) > 0) {
            credits = getCourseCredits(rows[i][1]);
            weighted += gradePoint(rows[i][9]) * credits;
            totalCredits += credits;
        }
    }
    if (totalCredits == 0) return 0.0;
    return weighted / totalCredits;
}

void computeCourseStats(const char courseCode[], const char semester[], double *highest, double *lowest, double *mean, double *median) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    double totals[MAX_ROWS], sum = 0.0, temp;
    int count = readTXT(ENROLLMENTS_FILE, rows, 10);
    int i, j, n = 0;
    *highest = 0.0; *lowest = 0.0; *mean = 0.0; *median = 0.0;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][1], courseCode) == 0 && strcmp(rows[i][2], semester) == 0 && strcmp(rows[i][3], "enrolled") == 0 && strlen(rows[i][8]) > 0) {
            totals[n] = textToDouble(rows[i][8]);
            n++;
        }
    }
    if (n == 0) return;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (totals[j] < totals[i]) {
                temp = totals[i]; totals[i] = totals[j]; totals[j] = temp;
            }
        }
    }
    *lowest = totals[0];
    *highest = totals[n - 1];
    for (i = 0; i < n; i++) sum += totals[i];
    *mean = sum / n;
    if (n % 2 == 1) *median = totals[n / 2];
    else *median = (totals[n / 2 - 1] + totals[n / 2]) / 2.0;
}

int partToInt(const char s[], int start, int len) {
    int value = 0;
    int i;
    for (i = start; i < start + len; i++) {
        if (isDigitChar(s[i])) value = value * 10 + (s[i] - '0');
    }
    return value;
}

int isLeapYear(int year) {
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

int monthDays(int month, int year) {
    int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    if (month >= 1 && month <= 12) return mdays[month - 1];
    return 0;
}

int isValidDateFormat(const char date[]) {
    int i, day, month, year;
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;
    for (i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isDigitChar(date[i])) return 0;
    }
    day = partToInt(date, 0, 2);
    month = partToInt(date, 3, 2);
    year = partToInt(date, 6, 4);
    if (year < 1900 || month < 1 || month > 12) return 0;
    if (day < 1 || day > monthDays(month, year)) return 0;
    return 1;
}

int totalDays(const char date[]) {
    int day = partToInt(date, 0, 2);
    int month = partToInt(date, 3, 2);
    int year = partToInt(date, 6, 4);
    int total = 0, y, m;
    for (y = 1900; y < year; y++) total += isLeapYear(y) ? 366 : 365;
    for (m = 1; m < month; m++) total += monthDays(m, year);
    total += day;
    return total;
}

int daysBetween(const char startDate[], const char endDate[]) {
    if (!isValidDateFormat(startDate) || !isValidDateFormat(endDate)) return 0;
    return totalDays(endDate) - totalDays(startDate);
}

int recordPayment(const char roll[], const char semester[], double amount, const char paidDate[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(FEES_FILE, rows, 6);
    int i, found = 0;
    double currentPaid;
    if (!isValidDateFormat(paidDate) || amount <= 0.0) return 0;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], semester) == 0) {
            currentPaid = textToDouble(rows[i][3]);
            currentPaid += amount;
            doubleToText(currentPaid, 2, rows[i][3]);
            strcpy(rows[i][5], paidDate);
            found = 1;
        }
    }
    if (found) writeTXT(FEES_FILE, "roll,semester,amount_due,amount_paid,due_date,paid_date", rows, count, 6);
    return found;
}

double computeLateFine(const char roll[], const char semester[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(FEES_FILE, rows, 6);
    int i, lateDays, weeks;
    double amountDue;
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], semester) == 0) {
            if (!isValidDateFormat(rows[i][4]) || !isValidDateFormat(rows[i][5])) return 0.0;
            lateDays = daysBetween(rows[i][4], rows[i][5]);
            if (lateDays <= 0) return 0.0;
            weeks = lateDays / 7;
            amountDue = textToDouble(rows[i][2]);
            return amountDue * 0.02 * weeks;
        }
    }
    return 0.0;
}

void generateReceipt(const char roll[], const char semester[]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    char student[MAX_COLS][MAX_CELL];
    char text[MAX_CELL];
    int count = readTXT(FEES_FILE, rows, 6);
    int i;
    double amountDue, amountPaid, fine, totalDue, balance;
    searchByRoll(roll, student);
    for (i = 0; i < count; i++) {
        if (strcmp(rows[i][0], roll) == 0 && strcmp(rows[i][1], semester) == 0) {
            amountDue = textToDouble(rows[i][2]);
            amountPaid = textToDouble(rows[i][3]);
            fine = computeLateFine(roll, semester);
            totalDue = amountDue + fine;
            balance = totalDue - amountPaid;
            if (balance < 0.0) balance = 0.0;
            printf("================================================\n");
            printf("UNIVERSITY FEE RECEIPT\n");
            printf("================================================\n");
            printf("%-18s: %s\n", "Roll", roll);
            if (strlen(student[1]) > 0) printf("%-18s: %s\n", "Name", student[1]);
            printf("%-18s: %s\n", "Semester", semester);
            doubleToText(amountDue, 2, text); printf("%-18s: %s\n", "Amount Due", text);
            doubleToText(fine, 2, text); printf("%-18s: %s\n", "Late Fine", text);
            doubleToText(totalDue, 2, text); printf("%-18s: %s\n", "Total Due", text);
            doubleToText(amountPaid, 2, text); printf("%-18s: %s\n", "Paid", text);
            doubleToText(balance, 2, text); printf("%-18s: %s\n", "Balance", text);
            printf("================================================\n");
            return;
        }
    }
    printf("Fee record not found.\n");
}

int getDefaulters(const char currentDate[], char result[MAX_ROWS][MAX_COLS][MAX_CELL]) {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = readTXT(FEES_FILE, rows, 6);
    int i, j, outCount = 0, overdueDays;
    double due, paid, balance, a, b;
    char text[MAX_CELL];
    for (i = 0; i < count; i++) {
        due = textToDouble(rows[i][2]);
        paid = textToDouble(rows[i][3]);
        balance = due - paid;
        overdueDays = daysBetween(rows[i][4], currentDate);
        if (balance > 0.0 && overdueDays > 0) {
            clearRow(result[outCount], 8);
            for (j = 0; j < 6; j++) strcpy(result[outCount][j], rows[i][j]);
            doubleToText(balance, 2, result[outCount][6]);
            doubleToText((double)(overdueDays / 7), 0, text);
            strcpy(result[outCount][7], text);
            outCount++;
        }
    }
    for (i = 0; i < outCount - 1; i++) {
        for (j = 0; j < outCount - i - 1; j++) {
            a = textToDouble(result[j][6]);
            b = textToDouble(result[j + 1][6]);
            if (a < b) swapRows(result, j, j + 1, 8);
        }
    }
    return outCount;
}

void studentNameByRoll(const char roll[], char name[]) {
    char s[MAX_COLS][MAX_CELL];
    if (searchByRoll(roll, s)) strcpy(name, s[1]);
    else strcpy(name, "-");
}

void printMeritList() {
    char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
    int count = listActiveStudents(rows);
    int i, j, maxIndex;
    for (i = 0; i < count - 1; i++) {
        maxIndex = i;
        for (j = i + 1; j < count; j++) {
            if (textToDouble(rows[j][3]) > textToDouble(rows[maxIndex][3])) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) swapRows(rows, i, maxIndex, 5);
    }
    outprintf("\nMerit List\n");
    outprintf("%-8s%-14s%-25s%-12s%-8s\n", "Rank", "Roll", "Name", "Dept", "CGPA");
    outprintf("-------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        outprintf("%-8d%-14s%-25s%-12s%-8s\n", i + 1, rows[i][0], rows[i][1], rows[i][2], rows[i][3]);
    }
}

void printAttendanceDefaulters() {
    char enrollments[MAX_ROWS][MAX_COLS][MAX_CELL];
    char name[MAX_CELL], pctText[MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, enrollments, 10);
    int i;
    double pct;
    outprintf("\nAttendance Defaulters\n");
    outprintf("%-14s%-25s%-12s%-12s\n", "Roll", "Name", "Course", "Percent");
    outprintf("----------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        if (strcmp(enrollments[i][3], "enrolled") == 0) {
            pct = getAttendancePct(enrollments[i][0], enrollments[i][1]);
            if (pct < 75.0) {
                studentNameByRoll(enrollments[i][0], name);
                doubleToText(pct, 2, pctText);
                outprintf("%-14s%-25s%-12s%-12s\n", enrollments[i][0], name, enrollments[i][1], pctText);
            }
        }
    }
}

void printFeeDefaulters(const char currentDate[]) {
    char defaulters[MAX_ROWS][MAX_COLS][MAX_CELL];
    char name[MAX_CELL];
    int count = getDefaulters(currentDate, defaulters);
    int i;
    outprintf("\nFee Defaulters\n");
    outprintf("%-14s%-25s%-12s%-14s%-10s\n", "Roll", "Name", "Semester", "Outstanding", "Weeks");
    outprintf("----------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        studentNameByRoll(defaulters[i][0], name);
        outprintf("%-14s%-25s%-12s%-14s%-10s\n", defaulters[i][0], name, defaulters[i][1], defaulters[i][6], defaulters[i][7]);
    }
}

void printSemesterResult(const char semester[]) {
    char enrollments[MAX_ROWS][MAX_COLS][MAX_CELL];
    char name[MAX_CELL], gpaText[MAX_CELL], attStatus[MAX_CELL];
    int count = readTXT(ENROLLMENTS_FILE, enrollments, 10);
    int i;
    double pct;
    outprintf("\n");
    outprintf("================================================================================================\n");
    outprintf("SEMESTER RESULT SHEET: %s\n", semester);
    outprintf("================================================================================================\n");
    outprintf("%-14s%-22s%-12s%-10s%-8s%-8s%-14s\n", "Roll", "Name", "Course", "Total", "Grade", "GPA", "Attendance");
    outprintf("------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        if (strcmp(enrollments[i][2], semester) == 0 && strcmp(enrollments[i][3], "enrolled") == 0) {
            pct = getAttendancePct(enrollments[i][0], enrollments[i][1]);
            if (pct < 75.0) strcpy(attStatus, "Shortage");
            else strcpy(attStatus, "OK");
            studentNameByRoll(enrollments[i][0], name);
            doubleToText(computeGPA(enrollments[i][0], semester), 2, gpaText);
            outprintf("%-14s%-22s%-12s%-10s%-8s%-8s%-14s\n", enrollments[i][0], name, enrollments[i][1], enrollments[i][8], enrollments[i][9], gpaText, attStatus);
        }
    }
    outprintf("================================================================================================\n");
}

void printDepartmentSummary(const char semester[]) {
    char students[MAX_ROWS][MAX_COLS][MAX_CELL];
    char enrollments[MAX_ROWS][MAX_COLS][MAX_CELL];
    char s[MAX_COLS][MAX_CELL];
    char depts[50][MAX_CELL];
    int counts[50], passCounts[50], gradeCounts[50];
    double cgpaSums[50];
    int studentCount = listActiveStudents(students);
    int enrollmentCount = readTXT(ENROLLMENTS_FILE, enrollments, 10);
    int deptCount = 0;
    int i, j, index;
    char avgText[MAX_CELL], passRateText[MAX_CELL];
    double avg, passRate;
    for (i = 0; i < 50; i++) {
        depts[i][0] = '\0'; counts[i] = 0; passCounts[i] = 0; gradeCounts[i] = 0; cgpaSums[i] = 0.0;
    }
    for (i = 0; i < studentCount; i++) {
        index = -1;
        for (j = 0; j < deptCount; j++) {
            if (strcmp(depts[j], students[i][2]) == 0) index = j;
        }
        if (index == -1 && deptCount < 50) {
            strcpy(depts[deptCount], students[i][2]);
            index = deptCount;
            deptCount++;
        }
        if (index != -1) {
            counts[index]++;
            cgpaSums[index] += textToDouble(students[i][3]);
        }
    }
    for (i = 0; i < enrollmentCount; i++) {
        if (strcmp(enrollments[i][2], semester) == 0 && strlen(enrollments[i][9]) > 0) {
            if (searchByRoll(enrollments[i][0], s)) {
                index = -1;
                for (j = 0; j < deptCount; j++) {
                    if (strcmp(depts[j], s[2]) == 0) index = j;
                }
                if (index != -1) {
                    gradeCounts[index]++;
                    if (strcmp(enrollments[i][9], "F") != 0) passCounts[index]++;
                }
            }
        }
    }
    outprintf("\nDepartment Summary\n");
    outprintf("%-12s%-10s%-12s%-12s\n", "Dept", "Count", "Avg CGPA", "Pass Rate");
    outprintf("------------------------------------------------\n");
    for (i = 0; i < deptCount; i++) {
        avg = counts[i] > 0 ? cgpaSums[i] / counts[i] : 0.0;
        passRate = gradeCounts[i] > 0 ? ((double)passCounts[i] / gradeCounts[i]) * 100.0 : 0.0;
        doubleToText(avg, 2, avgText);
        doubleToText(passRate, 2, passRateText);
        outprintf("%-12s%-10d%-12s%-12s\n", depts[i], counts[i], avgText, passRateText);
    }
}

int exportReportToFile(int reportChoice, const char filename[], const char extraValue[]) {
    reportOut = fopen(filename, "w");
    if (reportOut == NULL) return 0;
    if (reportChoice == 1) printMeritList();
    else if (reportChoice == 2) printAttendanceDefaulters();
    else if (reportChoice == 3) printFeeDefaulters(extraValue);
    else if (reportChoice == 4) printSemesterResult(extraValue);
    else if (reportChoice == 5) printDepartmentSummary(extraValue);
    fclose(reportOut);
    reportOut = NULL;
    return 1;
}

void studentMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- Student Operations ---\n");
        printf("1. Add student\n");
        printf("2. Search by roll\n");
        printf("3. Search by name\n");
        printf("4. Update student\n");
        printf("5. Soft delete student\n");
        printf("6. List active students\n");
        printf("7. Search as you type\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            char roll[MAX_CELL], name[MAX_CELL], dept[MAX_CELL];
            double cgpa;
            printf("Roll BSAI-YY-XXX: "); scanf("%119s", roll);
            clearInputLine();
            printf("Name: "); readLine(name, MAX_CELL);
            printf("Department: "); readLine(dept, MAX_CELL);
            printf("CGPA: "); scanf("%lf", &cgpa);
            if (addStudent(roll, name, dept, cgpa)) printf("Student added.\n");
            else printf("Student not added. Check validation or duplicate roll.\n");
            pauseScreen();
        } else if (choice == 2) {
            char roll[MAX_CELL];
            char one[MAX_ROWS][MAX_COLS][MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            if (searchByRoll(roll, one[0])) printStudentsTable(one, 1);
            else printStudentsTable(one, 0);
            pauseScreen();
        } else if (choice == 3) {
            char key[MAX_CELL];
            char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
            int count;
            clearInputLine();
            printf("Name substring: "); readLine(key, MAX_CELL);
            count = searchByName(key, rows);
            printStudentsTable(rows, count);
            pauseScreen();
        } else if (choice == 4) {
            char roll[MAX_CELL], value[MAX_CELL];
            int field;
            printf("Roll: "); scanf("%119s", roll);
            printf("Field 1=name 2=dept 3=cgpa 4=status: "); scanf("%d", &field);
            clearInputLine();
            printf("New value: "); readLine(value, MAX_CELL);
            if (updateStudent(roll, field, value)) printf("Student updated.\n");
            else printf("Update failed.\n");
            pauseScreen();
        } else if (choice == 5) {
            char roll[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            if (softDelete(roll)) printf("Student marked inactive.\n");
            else printf("Soft delete failed.\n");
            pauseScreen();
        } else if (choice == 6) {
            char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
            int count = listActiveStudents(rows);
            printStudentsTable(rows, count);
            pauseScreen();
        } else if (choice == 7) {
            searchAsYouType();
            pauseScreen();
        }
    }
}

void courseMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- Course Operations ---\n");
        printf("1. List courses\n");
        printf("2. Enroll student\n");
        printf("3. Drop course\n");
        printf("4. Credit load\n");
        printf("5. Check prerequisite\n");
        printf("6. List enrolled students\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            printCourseList(); pauseScreen();
        } else if (choice == 2) {
            char roll[MAX_CELL], code[MAX_CELL], sem[MAX_CELL], msg[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            enrollResultToString(enrollStudent(roll, code, sem), msg);
            printf("%s\n", msg); pauseScreen();
        } else if (choice == 3) {
            char roll[MAX_CELL], code[MAX_CELL], sem[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            if (dropCourse(roll, code, sem)) printf("Course dropped.\n");
            else printf("Drop failed. Attendance may already exist.\n");
            pauseScreen();
        } else if (choice == 4) {
            char roll[MAX_CELL], sem[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Semester: "); scanf("%119s", sem);
            printf("Credit load: %d\n", getCreditLoad(roll, sem)); pauseScreen();
        } else if (choice == 5) {
            char roll[MAX_CELL], code[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Course code: "); scanf("%119s", code);
            if (checkPrerequisite(roll, code)) printf("Prerequisite condition passed.\n");
            else printf("Prerequisite condition not passed.\n");
            pauseScreen();
        } else if (choice == 6) {
            char code[MAX_CELL], sem[MAX_CELL];
            char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
            int count;
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            count = listEnrolledStudents(code, sem, rows);
            printStudentsTable(rows, count); pauseScreen();
        }
    }
}

void attendanceMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- Attendance Operations ---\n");
        printf("1. Mark attendance\n");
        printf("2. Attendance percentage\n");
        printf("3. Shortage list\n");
        printf("4. Undo last session\n");
        printf("5. Daily sheet\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            char code[MAX_CELL], sem[MAX_CELL], date[MAX_CELL];
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            printf("Date DD-MM-YYYY: "); scanf("%119s", date);
            if (markAttendance(code, sem, date)) printf("Attendance saved.\n");
            else printf("No enrolled students found.\n");
            pauseScreen();
        } else if (choice == 2) {
            char roll[MAX_CELL], code[MAX_CELL], text[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Course code: "); scanf("%119s", code);
            doubleToText(getAttendancePct(roll, code), 2, text);
            printf("Attendance: %s%%\n", text); pauseScreen();
        } else if (choice == 3) {
            char code[MAX_CELL], sem[MAX_CELL];
            char rows[MAX_ROWS][MAX_COLS][MAX_CELL];
            int count, i;
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            count = getShortageList(code, sem, rows);
            printf("%-14s%-25s%-12s%-10s\n", "Roll", "Name", "Course", "Percent");
            for (i = 0; i < count; i++) {
                printf("%-14s%-25s%-12s%-10s\n", rows[i][0], rows[i][1], rows[i][2], rows[i][3]);
            }
            pauseScreen();
        } else if (choice == 4) {
            if (undoLastSession()) printf("Last session restored.\n");
            else printf("No backup available.\n");
            pauseScreen();
        } else if (choice == 5) {
            char code[MAX_CELL], sem[MAX_CELL], date[MAX_CELL];
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            printf("Date DD-MM-YYYY: "); scanf("%119s", date);
            printDailySheet(code, sem, date); pauseScreen();
        }
    }
}

void gradesMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- Grade Operations ---\n");
        printf("1. Enter marks\n");
        printf("2. Compute GPA\n");
        printf("3. Course stats\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            char roll[MAX_CELL], code[MAX_CELL], sem[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            if (!enterMarks(roll, code, sem)) printf("Enrollment not found.\n");
            pauseScreen();
        } else if (choice == 2) {
            char roll[MAX_CELL], sem[MAX_CELL], text[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Semester: "); scanf("%119s", sem);
            doubleToText(computeGPA(roll, sem), 2, text);
            printf("GPA: %s\n", text); pauseScreen();
        } else if (choice == 3) {
            char code[MAX_CELL], sem[MAX_CELL], text[MAX_CELL];
            double highest, lowest, mean, median;
            printf("Course code: "); scanf("%119s", code);
            printf("Semester: "); scanf("%119s", sem);
            computeCourseStats(code, sem, &highest, &lowest, &mean, &median);
            doubleToText(highest, 2, text); printf("Highest: %s\n", text);
            doubleToText(lowest, 2, text); printf("Lowest: %s\n", text);
            doubleToText(mean, 2, text); printf("Mean: %s\n", text);
            doubleToText(median, 2, text); printf("Median: %s\n", text);
            pauseScreen();
        }
    }
}

void feeMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- Fee Operations ---\n");
        printf("1. Record payment\n");
        printf("2. Compute late fine\n");
        printf("3. Generate receipt\n");
        printf("4. Fee defaulters\n");
        printf("5. Days between two dates\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            char roll[MAX_CELL], sem[MAX_CELL], date[MAX_CELL];
            double amount;
            printf("Roll: "); scanf("%119s", roll);
            printf("Semester: "); scanf("%119s", sem);
            printf("Amount: "); scanf("%lf", &amount);
            printf("Paid date DD-MM-YYYY: "); scanf("%119s", date);
            if (recordPayment(roll, sem, amount, date)) printf("Payment recorded.\n");
            else printf("Payment not recorded.\n");
            pauseScreen();
        } else if (choice == 2) {
            char roll[MAX_CELL], sem[MAX_CELL], text[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Semester: "); scanf("%119s", sem);
            doubleToText(computeLateFine(roll, sem), 2, text);
            printf("Late fine: %s\n", text); pauseScreen();
        } else if (choice == 3) {
            char roll[MAX_CELL], sem[MAX_CELL];
            printf("Roll: "); scanf("%119s", roll);
            printf("Semester: "); scanf("%119s", sem);
            generateReceipt(roll, sem); pauseScreen();
        } else if (choice == 4) {
            char date[MAX_CELL];
            printf("Current date DD-MM-YYYY: "); scanf("%119s", date);
            printFeeDefaulters(date); pauseScreen();
        } else if (choice == 5) {
            char d1[MAX_CELL], d2[MAX_CELL];
            printf("Start date DD-MM-YYYY: "); scanf("%119s", d1);
            printf("End date DD-MM-YYYY: "); scanf("%119s", d2);
            printf("Days: %d\n", daysBetween(d1, d2)); pauseScreen();
        }
    }
}

void reportMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- Reports ---\n");
        printf("1. Merit list\n");
        printf("2. Attendance defaulters\n");
        printf("3. Fee defaulters\n");
        printf("4. Semester result\n");
        printf("5. Department summary\n");
        printf("6. Export report to file\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            printMeritList(); pauseScreen();
        } else if (choice == 2) {
            printAttendanceDefaulters(); pauseScreen();
        } else if (choice == 3) {
            char date[MAX_CELL];
            printf("Current date DD-MM-YYYY: "); scanf("%119s", date);
            printFeeDefaulters(date); pauseScreen();
        } else if (choice == 4) {
            char sem[MAX_CELL];
            printf("Semester: "); scanf("%119s", sem);
            printSemesterResult(sem); pauseScreen();
        } else if (choice == 5) {
            char sem[MAX_CELL];
            printf("Semester: "); scanf("%119s", sem);
            printDepartmentSummary(sem); pauseScreen();
        } else if (choice == 6) {
            int reportChoice;
            char filename[MAX_CELL], extra[MAX_CELL] = "";
            printf("Report number 1 Merit 2 Attendance 3 Fee 4 Semester 5 Department: "); scanf("%d", &reportChoice);
            printf("Output file name: "); scanf("%119s", filename);
            if (reportChoice == 3) { printf("Current date DD-MM-YYYY: "); scanf("%119s", extra); }
            else if (reportChoice == 4 || reportChoice == 5) { printf("Semester: "); scanf("%119s", extra); }
            if (exportReportToFile(reportChoice, filename, extra)) printf("Report exported to %s\n", filename);
            else printf("Export failed.\n");
            pauseScreen();
        }
    }
}

int main() {
    int choice = -1;
    while (choice != 0) {
        printf("\n========================================\n");
        printf("Campus Analytics Engine\n");
        printf("========================================\n");
        printf("1. Students\n");
        printf("2. Courses and Enrollments\n");
        printf("3. Attendance\n");
        printf("4. Grades\n");
        printf("5. Fees\n");
        printf("6. Reports\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        if (choice == 1) studentMenu();
        else if (choice == 2) courseMenu();
        else if (choice == 3) attendanceMenu();
        else if (choice == 4) gradesMenu();
        else if (choice == 5) feeMenu();
        else if (choice == 6) reportMenu();
    }
    printf("Program ended.\n");
    return 0;
}
