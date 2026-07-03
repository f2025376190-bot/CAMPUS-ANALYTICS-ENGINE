#include <iostream>
using namespace std;
struct Course {
    string name;
    int credits, marks, attendance;
};
struct Student {
    string name, id;
    Course courses[3];
    float gpa;
};
int main()
{
    int choice;
   do{
    mainmenu:
 cout << "STUDENT TOOLKIT-DEMO VERSION\n";
 cout << "=========================================\n";
 cout << "1. Admissions & Scholarship Inquiry\n";
 cout << "2. Check Fee Status\n";
 cout << "3. Academic Records (GPA & Attendance)\n";
 cout << "4. View Registered Students\n";
 cout << "5. Exit\n";
 cout << "Enter Choice: ";
 cin >> choice;
 switch (choice) {
 case 1: {
    cout << "\n--- Available Departments ---\n";
    cout << "1. Computer Science\n 2. Information Technology\n 3. Cyber Security\n";
    cout << "4. Software Engineering\n 5. Artificial Intelligence\n 6. BS English\n";
     cout << "7. BS Math\n 8. BS Chemistry \n 9.Main Menu\n";
     int depchoice;
    cout << "Choose a field for details: ";
      cin >> depchoice;
                if (depchoice==9)
                goto mainmenu;
    float userMarks;
  cout << "Enter your Intermediate Percentage: ";
  cin >> userMarks;
  cout << "Admission Criteria: Minimum 50% marks required in Intermediate.\n";
  if (userMarks >= 50.0) {
        float scholarship = 0;
       if (userMarks >= 80.0)
         scholarship = 50.0;
      else if (userMarks >= 70.0)
      scholarship = 35.0;
      else if (userMarks >= 60.0)
      scholarship = 15.0;
      cout << "Congratulations! You are eligible for admission.\n";
     cout << "Your Scholarship: " << scholarship << "%\n";
         char registerchoice;
      cout << "Would you like to register now? (y/n): ";
         cin >> registerchoice;
       if (registerchoice== 'y' || registerchoice == 'Y') {
               long rollnum;
      cout << "Enter your Intermediate Roll Number: ";
         cin >> rollnum;
              int genid = rollnum / 6;
          string selecteddep;
                 switch(depchoice) {
   case 1: selecteddep = "Computer Science";
   break;
      case 2: selecteddep = "Information Technology";
      break;
      case 3: selecteddep = "Cyber Security";
      break;
     case 4: selecteddep = "Software Engineering";
     break;
     case 5: selecteddep = "Artificial Intelligence";
     break;
     case 6: selecteddep = "BS English";
     break;
     case 7: selecteddep = "BS MATH";
     break;
     case 8: selecteddep = "BS CHEMISTRY";
     break;



    default: selecteddep = "Selected Department";
   break;          }
cout << "\n------------------------------------------------\n";
cout << "COMPLIMENTARY: Welcome to our University family!\n";
 cout << "You are now registered in " << selecteddep << ".\n";
  cout << "Your generated Student ID is: " << genid << endl;
 cout << "PLEASE VISIT UNIVERSITY WITH ID " << genid << " FOR FURTHER DETAILS.\n";
     cout << "------------------------------------------------\n";
     } else {
        cout << "\nCOMPLIMENTARY: Thank you for your interest! We wish you the best of luck in your future endeavors.\n";
                    }
}
else cout << "Sorry, you do not meet the admission criteria.\n";
}
break;
case 2:
{  mainmenu2:
    int option;
 cout << "\n========================================\n";
    cout << "           STUDENT PORTAL\n";
    cout << "========================================\n";

    cout << "1. Check Fee Status\n";
    cout << "2. Installment Plans\n";
    cout << "3. Check Fines\n";
    cout << "4. Check Rewards\n";
    cout << "5. Exit Portal\n";

    cout << "\nEnter Your Choice: ";
    cin >> option;

    switch(option) {

        // ================= FEE STATUS =================
        case 1: {

            int studentId;
            float interMarks;
            int scholarship = 0;
            int totalFee = 50000;
            int finalFee;

            cout << "\nEnter Student ID: ";
            cin >> studentId;

            cout << "Enter Intermediate Percentage: ";
            cin >> interMarks;

            // Scholarship Logic
            if (interMarks >= 80)
                scholarship = 50;
            else if (interMarks >= 70)
                scholarship = 35;
            else if (interMarks >= 60)
                scholarship = 15;
            else
                scholarship = 0;

            finalFee = totalFee - (totalFee * scholarship / 100);

            cout << "\n========================================\n";
            cout << "             FEE DETAILS\n";
            cout << "========================================\n";

            cout << "Scholarship: " << scholarship << "%\n";
            cout << "Total Fee: 50000\n";
            cout << "Fee After Scholarship: " << finalFee << endl;

            // Paid / Pending Logic
            if (studentId % 2 != 0) {

                cout << "\nStatus: FEE PAID SUCCESSFULLY\n";
                cout << "Paid Amount: " << finalFee << endl;
            }

            else {

                cout << "\nStatus: FEE PENDING\n";
                cout << "Pending Amount: " << finalFee << endl;
            }
            int option;
            cout<<"\nEnter 0 for main menu:";
            cin>>option;
            if(option==0)
goto mainmenu2;
            break;
        }

        // ================= INSTALLMENT =================
        case 2: {
            int scholarship1;
            int finalFee;
            int id1;
            float per;
            int installmentChoice;
            int totalFee = 50000;
            cout<< "Enter yourid:";
            cin>>id1;
            cout<<"your percentage:";
            cin>>per;
                        if (per >= 80)
                scholarship1 = 50;
            else if (per >= 70)
                scholarship1 = 35;
            else if (per >= 60)
                scholarship1 = 15;
            else
                scholarship1 = 0;

            finalFee = totalFee - (totalFee * scholarship1 / 100);
            cout << "\n========================================\n";
            cout << "         INSTALLMENT PLANS\n";
            cout << "========================================\n";
            cout << "1. Two Installments\n";
            cout << "2. Three Installments\n";
            cout << "3. Main Menu\n";
            cout << "\nChoose Plan: ";
            cin >> installmentChoice;

            if (installmentChoice == 1) {

                cout << "\nRequest Submitted Successfully.\n";
                cout << "Please collect your NEW FEE CHALLAN from Registrar Office.\n";

                cout << "Fee Per Installment: "
                     << finalFee / 2 << endl;
            }

            else if (installmentChoice == 2) {

                cout << "\nRequest Submitted Successfully.\n";
                cout << "Please collect your NEW FEE CHALLAN from Registrar Office.\n";

                cout << "Fee Per Installment: "
                     << finalFee / 3 << endl;
            }
            else if (installmentChoice == 3)
                goto mainmenu2;
            else {

                cout << "Invalid Option.\n";
            }
                        int option1;
            cout<<"\nEnter 0 for main menu:";
            cin>>option1;
            if(option1==0)
goto mainmenu2;

            break;
        }

        // ================= FINES =================
        case 3: {

            int studentId;

            cout << "\nEnter Student ID: ";
            cin >> studentId;

            cout << "\n========================================\n";
            cout << "             STUDENT FINES\n";
            cout << "========================================\n";

            if (studentId % 2 == 0) {

                cout << "Total Fines: 2\n";
                cout << "1. Short Attendance Fine\n";
                cout << "2. Rules Violation Fine\n";
            }

            else {
                cout << "Total Fines: 2\n";
                cout << "1. Misbehavior Fine\n";
                cout << "2. Stuck off warning Fine\n";
}
                        int option1;
            cout<<"\nEnter 0 for main menu:";
            cin>>option1;
            if(option1==0)
             goto mainmenu2;
            break;
        }

        // ================= REWARDS =================
        case 4: {

            int studentId;

            cout << "\nEnter Student ID: ";
            cin >> studentId;

            cout << "\n========================================\n";
            cout << "            STUDENT REWARDS\n";
            cout << "========================================\n";

            if (studentId % 3 == 0) {

                cout << "Reward: Winner of Coding Competition\n";
                cout << "Prize: Gold Medal & Certificate\n";
            }

            else if (studentId % 5 == 0) {

                cout << "Reward: Sports Competition Winner\n";
                cout << "Prize: Cash Prize & Appreciation Certificate\n";
            }
            else {
                cout << "No rewards recorded yet.\n";
            }
                                    int option2;
            cout<<"\nEnter 0 for main menu:";
            cin>>option2;
            if(option2==0)
             goto mainmenu2;
            break;
        }
        case 5:
            cout << "\nExiting Student Portal...\n";
            break;
        default:
            cout << "\nInvalid Choice.\n";
            break;
    }
while(option != 5);
} break;
case 3:
{  const int NUM_STUDENTS = 10;
    string names[NUM_STUDENTS] = {"Ali Khan", "Ayesha Ahmed", "Bilal Hassan", "Zainab Fatima", "Dawood Ibrahim", "Hamza Yusuf", "Esha Faisal", "Mustafa Ali", "Hania Imran", "Zayan Malik"};
    string ids[NUM_STUDENTS] = {"S101", "S102", "S103", "S104", "S105", "S106", "S107", "S108", "S109", "S110"};
    string courseNames[3] = {"Programming Fund.", "Calculus", "English"};
    int credits[3] = {4, 3, 2};

    // 2D marks and 2D attendance arrays
    int marksGrid[NUM_STUDENTS][3] = {{82,65,91}, {76,54,88}, {88,79,70}, {61,85,82}, {45,58,67}, {92,88,90}, {73,62,81}, {55,70,60}, {80,77,85}, {68,50,55}};
    int attendGrid[NUM_STUDENTS][3] = {{78,72,88}, {90,60,95}, {85,71,80}, {92,89,68}, {64,80,76}, {95,91,93}, {70,74,85}, {82,65,78}, {88,84,90}, {77,55,73}};

    Student students[NUM_STUDENTS];

    // Data Initialization using basic pointer syntax
    for (int i = 0; i < NUM_STUDENTS; i++) {
        Student* sPtr = &students[i]; // Pointing directly to the current student
        (*sPtr).name = names[i];
        (*sPtr).id = ids[i];

        for (int j = 0; j < 3; j++) {
            Course* cPtr = &((*sPtr).courses[j]); // Pointing directly to the current course
            (*cPtr).name = courseNames[j];
            (*cPtr).credits = credits[j];
            (*cPtr).marks = marksGrid[i][j];
            (*cPtr).attendance = attendGrid[i][j];
        }
    }

    int choice;
    do {
        cout << "\n=============================================\n MODULE 3: ACADEMICS, ATTENDANCE & GRADING \n=============================================\n1. View Academic & Grading Tracker (Student Portal)\n2. Interactive Roll-Call Simulation (Faculty Portal)\n3. Student Complain Section\n4. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n--- Student Profiles Available ---\n";
            for (int i = 0; i < NUM_STUDENTS; i++) {
                Student* sPtr = &students[i];
                cout << i + 1 << ". " << (*sPtr).name << " (" << (*sPtr).id << ")\n";
            }

            cout << "Select student number to view profile (1-10): ";
            int s; cin >> s;

            if (s >= 1 && s <= NUM_STUDENTS) {
                Student* ptr = &students[s - 1]; // Basic pointer to selected student
                cout << "\n----------------------------------------------------------------------\nStudent Name: " << (*ptr).name << " | ID: " << (*ptr).id << "\n----------------------------------------------------------------------\nCourse Name\t\tCr. Hrs\tMarks\tAttendance\tEligibility\n----------------------------------------------------------------------\n";

                float pts = 0.0f; int totCr = 0;
                for (int j = 0; j < 3; j++) {
                    Course* c = &((*ptr).courses[j]); // Basic pointer to current course

                    float g = ((*c).marks >= 85) ? 4.0 : ((*c).marks >= 80) ? 3.7 : ((*c).marks >= 75) ? 3.3 : ((*c).marks >= 70) ? 3.0 : ((*c).marks >= 65) ? 2.7 : ((*c).marks >= 60) ? 2.3 : ((*c).marks >= 50) ? 2.0 : 0.0;
                    pts += g * (float)(*c).credits;
                    totCr += (*c).credits;

                    cout << (*c).name << "\t" << ((*c).name == "Calculus" ? "\t" : "") << (*c).credits << "\t" << (*c).marks << "\t" << (*c).attendance << "%\t\t" << ((*c).attendance < 75 ? "BARRED (Below 75%)" : "Eligible") << endl;
                }
                (*ptr).gpa = pts / (float)totCr;
                cout << "----------------------------------------------------------------------\nCalculated Cumulative GPA: " << (*ptr).gpa << " / 4.0\n----------------------------------------------------------------------\n";
            } else {
                cout << "Invalid selection range assignment entered.\n";
            }

        } else if (choice == 2) {
            cout << "\n--- Faculty Roll-Call Simulation ---\n";
            for (int i = 0; i < NUM_STUDENTS; i++) {
                Student* sPtr = &students[i];
                cout << i + 1 << ". " << (*sPtr).name << "\n";
            }

            cout << "Select student index reference to update logs (1-10): ";
            int s; cin >> s;

            if (s >= 1 && s <= NUM_STUDENTS) {
                Student* ptr = &students[s - 1];
                cout << "\nSelect course to increment log fields:\n";
                for (int j = 0; j < 3; j++) {
                    Course* cPtr = &((*ptr).courses[j]);
                    cout << j + 1 << ". " << (*cPtr).name << " (Current: " << (*cPtr).attendance << "%)\n";
                }

                cout << "Enter active course selection (1-3): ";
                int c; cin >> c;

                if (c >= 1 && c <= 3) {
                    Course* crs = &((*ptr).courses[c - 1]);
                    if ((*crs).attendance < 100) {
                        (*crs).attendance = ((*crs).attendance + 5 > 100) ? 100 : (*crs).attendance + 5;
                        cout << "Log field verified and updated. Current Status: " << (*crs).attendance << "%\n";
                    } else {
                        cout << "System notification: Class logging state is already maximized (100%).\n";
                    }
                } else {
                    cout << "Selection target array out of bounds.\n";
                }
            } else {
                cout << "Target identifier assignment reference error.\n";
            }

        } else if (choice == 3) {
            string comp; cin.ignore();
            cout << "\n--- Welcome to the Student Complain Portal ---\nPlease input issues regarding evaluation trackers, attendance records, or errors: \n";
            getline(cin, comp);
            cout << "\nSystem Feedback: Data packet recorded successfully. The academic evaluation cell will follow up.\n";

        } else if (choice == 4) {
            cout << "Terminating active tracking instance. Session secure.\n";
        } else {
            cout << "Input flag exception. Verify choices between 1 and 4.\n";
        }
    } while (choice != 4);

}
break;
default:
cout<<"wrong input";

}
} while(choice<5);
return 0;
}
