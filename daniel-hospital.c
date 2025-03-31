//
// Created by Daniel and Jun on 2025-01-29.
//
// Purpose: To manage a hospital's operations, including keeping patient records,
// searching for and discharging patients, and managing doctors' weekly schedules,
// using a user-friendly menu-driven interface.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DAYS_IN_WEEK 7
#define SHIFTS_IN_DAY 3
#define NAME_MAX_LENGTH 20
#define DIAGNOSIS_MAX_LENGTH 100
#define PATIENT_MIN_AGE 1
#define PATIENT_MAX_AGE 125
#define PATIENT_FILE "patients.dat"
#define SCHEDULE_FILE "schedule.dat"
#define BACKUP_FILE "backup.dat"

// Structure to store patient information
// Changed (by Jun): Changed to Linked List
typedef struct PatientInformation {
    int patientID;
    char name[NAME_MAX_LENGTH];
    int age;
    char diagnosis[DIAGNOSIS_MAX_LENGTH];
    int roomNumber;
    struct PatientInformation *next;
} Patient;

// Structure to store doctor's name
typedef struct {
    char DoctorName[NAME_MAX_LENGTH];
} DoctorSchedule;

// Global variables
// Changed (by Jun): Changed to Linked List
Patient *head = NULL; // Head of the list
int patientCount = 0; // Current patients number

// 2D array for doctor's schedules
DoctorSchedule schedule[DAYS_IN_WEEK][SHIFTS_IN_DAY];

// Function prototypes
void displayMenu();
void addNewPatient();
void viewAllPatients();
void searchForPatient();
void dischargePatient();
void manageDoctorSchedule();
void generateReports();
void saveDataToFile();
void loadDataFromFile();
void backupData();
void restoreData();
void freeAllPatients();
int validatePatientID(int);
int validatePatientAge(struct PatientInformation *);

int main() {
    // Load data from file if available
    loadDataFromFile();
    displayMenu();
    return 0;
}

// Function to display the menu interface
void displayMenu() {
    int userChoice;

    do {
        printf("HOSPITAL MANAGEMENT SYSTEM\n");
        printf("----------------------------\n");
        printf("Enter your choice:\n");
        printf("1. Add Patient Record\n");
        printf("2. View All Patient Records\n");
        printf("3. Search for a Patient\n");
        printf("4. Discharge a Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Save and Exit\n");
        printf("7. Backup Data\n");
        printf("8. Restore Data\n");

        scanf("%d", &userChoice);
        // Consume newline left by scanf
        getchar();

        switch (userChoice) {
            case 1:
                addNewPatient();
                break;

            case 2:
                viewAllPatients();
                break;

            case 3:
                searchForPatient();
                break;

            case 4:
                dischargePatient();
                break;

            case 5:
                manageDoctorSchedule();
                break;

            case 6:
                saveDataToFile();
                printf("Data saved successfully.\n");
                freeAllPatients(); // Free memory before exiting
                exit(0);

            case 7:
                backupData();
                break;

            case 8:
                restoreData();
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (userChoice != 6);
}

// Function to save patient data and doctor schedule to files
void saveDataToFile() {
    FILE *patientFile = fopen(PATIENT_FILE, "wb");
    if (patientFile == NULL) {
        printf("Error saving patient data.\n");
        return;
    }

    // Linked List
    fwrite(&patientCount, sizeof(int), 1, patientFile);

    Patient *current = head;
    while (current) {
        fwrite(current, sizeof(Patient), 1, patientFile);
        current = current->next;
    }

    fclose(patientFile);
}

// Function to load patient data and doctor schedule from files
void loadDataFromFile() {
    FILE *patientFile = fopen(PATIENT_FILE, "rb");
    if (patientFile != NULL) {
        fread(&patientCount, sizeof(int), 1, patientFile);
        for (int i = 0; i < patientCount; i++) {
            Patient *p = malloc(sizeof(Patient));
            fread(p, sizeof(Patient), 1, patientFile);
            p->next = head;
            head = p;
        }
        fclose(patientFile);
    }

    FILE *scheduleFile = fopen(SCHEDULE_FILE, "rb");
    if (scheduleFile != NULL) {
        fread(schedule, sizeof(DoctorSchedule), DAYS_IN_WEEK * SHIFTS_IN_DAY, scheduleFile);
        fclose(scheduleFile);
    }
}

// Function to back up the data
void backupData() {
    FILE *backupFile = fopen(BACKUP_FILE, "wb");
    if (backupFile == NULL) {
        printf("Error creating backup file.\n");
        return;
    }

    fwrite(&patientCount, sizeof(int), 1, backupFile);
    Patient *p = head;
    while (p) {
        fwrite(p, sizeof(Patient), 1, backupFile);
        p = p->next;
    }

    fwrite(schedule, sizeof(DoctorSchedule), DAYS_IN_WEEK * SHIFTS_IN_DAY, backupFile);
    fclose(backupFile);

    printf("Data backup successful.\n");
}

// Function to restore data from backup
void restoreData() {
    FILE *backupFile = fopen(BACKUP_FILE, "rb");
    if (backupFile == NULL) {
        printf("Error: No backup file found.\n");
        return;
    }

    freeAllPatients(); // clears all the records that added after the user's back up.

    fread(&patientCount, sizeof(int), 1, backupFile);
    for (int i = 0; i < patientCount; i++) {
        Patient *p = malloc(sizeof(Patient));
        fread(p, sizeof(Patient), 1, backupFile);
        p->next = head;
        head = p;
    }
    fclose(backupFile);

    printf("Data restored from backup.\n");
}

// 1. Add a New Patient
void addNewPatient() {
  Patient *newPatient = (Patient *)malloc(sizeof(Patient)); // Changed (by Jun): Adding patient using Linked List
//    if (currentPatientCount >= patientCapacity) {
//        // If the system is full, reallocate more memory
//        reallocatePatientMemory();
//        printf("Memory reallocated. New patient capacity is %d.\n", patientCapacity);
//    }
  if (!newPatient) {
    printf("Memory allocation failed!\n");
    return;
  }

  // Get patient details
  printf("Enter Patient ID: ");
  scanf("%d", &newPatient->patientID);
  getchar(); // Consume newline after entering the ID

    // Validate the patient's ID
    if (validatePatientID(newPatient -> patientID) == 1) {
        return;
    }

    // Get patient's name
    printf("Enter Patient Name: ");
    fgets(newPatient-> name, NAME_MAX_LENGTH, stdin);
    newPatient -> name[strcspn(newPatient -> name, "\n")] = 0;

    // Get patient's age
    printf("Enter Patient Age: ");
    scanf("%d", &newPatient -> age);
    getchar(); // Consume newline

    // Validate the patient's age
    if (validatePatientAge(newPatient) == 1) {
        return;
    }

    // Get patient's diagnosis
    printf("Enter Patient Diagnosis: ");
    fgets(newPatient -> diagnosis, DIAGNOSIS_MAX_LENGTH, stdin);
    newPatient -> diagnosis[strcspn(newPatient -> diagnosis, "\n")] = 0;

    // Get patient's room number
    printf("Enter Room Number: ");
    scanf("%d", &newPatient -> roomNumber);
    getchar();

    // Save the new patient record in the array
//    patients[currentPatientCount] = newPatient;
//    currentPatientCount++;
    newPatient->next = head; // Changed (by Jun): Adding new patient in the head of the list
    head = newPatient;
    patientCount++;

    printf("%s Added!\n\n", newPatient -> name);
}

// 2. View all Patients on File
void viewAllPatients() {
    if (head == NULL) {
        printf("No patients found!\n\n");
        return;
    }

    printf("%-12s %-20s %-6s %-30s %-12s\n", "Patient ID", "Name", "Age", "Diagnosis", "Room Number");

    // Scan the Linked List
    Patient *current = head;
    while (current != NULL) {
      printf("%-12d %-20s %-6d %-30s %-12d\n",
             current->patientID,
             current->name,
             current->age,
             current->diagnosis,
             current->roomNumber);
      current = current->next; // To the next node
    }
    printf("\n");

//    for (int i = 0; i < currentPatientCount; i++) {
//        printf("%-12d %-20s %-6d %-30s %-12d\n",
//               patients[i].patientID,
//               patients[i].name,
//               patients[i].age,
//               patients[i].diagnosis,
//               patients[i].roomNumber);
//    }
//    printf("\n");
}

// 3. Search for a Patient
// Changed(by Jun): From for-loop to while(current) for scanning the Linked List
void searchForPatient() {
    if (head == NULL) {
      printf("Error: No patients found!\n");
      return;
    }

    int userChoice, id;
    char name[NAME_MAX_LENGTH];

    printf("Search by:\n1. ID\n2. Name\nChoice: ");
    scanf("%d", &userChoice);
    getchar();

    if (userChoice == 1) {
      printf("Enter Patient ID: ");
      scanf("%d", &id);
      getchar();

      Patient *current = head;
      while (current) {
        if (current->patientID == id) {
          printf("Found Patient: %s (ID: %d, Age: %d, Diagnosis: %s, Room: %d)\n",
                 current->name,
                 current->patientID,
                 current->age,
                 current->diagnosis,
                 current->roomNumber);
          return;
        }
        current = current->next;
      }
        printf("Patients with ID %d not found.\n", id);
    } else if (userChoice == 2) {
      printf("Enter Patient Name: ");
      fgets(name, NAME_MAX_LENGTH, stdin);
      name[strcspn(name, "\n")] = 0;

      Patient *current = head;
      while (current) {
        if (strcmp(current->name, name) == 0) {
          printf("Found Patinet: %s (ID: %d, Age: %d, Diagnosis: %s, Room: %d)\n",
                 current->name,
                 current->patientID,
                 current->age,
                 current->diagnosis,
                 current->roomNumber);
          return;
        }
        current = current->next;
      }
      printf("Patients with Name %s not found.\n", name);
    } else {
      printf("Invalid choice.\n");
    }
}

// 4. Discharge a Patient by ID
void dischargePatient() {
    if (head == NULL) {
        printf("No patients found!\n\n");
        return;
    }

    int id;
    printf("Enter ID of patient to discharge: ");
    scanf("%d", &id);
    getchar();

    Patient *current = head, *prev = NULL;

    while (current) {
        if (current->patientID == id) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            free(current);
            patientCount--;
            printf("Patient #%d has been discharged.\n\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Patient with ID %d not found.\n\n", id);
}

// 5. Manage Doctors' Weekly Schedules
void manageDoctorSchedule() {
    int userChoice;
    printf("What would you like to do with the doctors' schedule?\n");
    printf("1. Assign a doctor to a shift\n");
    printf("2. Display the full weekly schedule\n");
    scanf("%d", &userChoice);
    getchar(); // Consume newline

    switch (userChoice) {
        case 1: {
            int dayOfWeek;
            int shift;
            char doctorName[NAME_MAX_LENGTH];

            printf("Enter Day: (0 for Sunday, 1 for Monday, 2 for Tuesday...) ");
            scanf("%d", &dayOfWeek);
            getchar(); // Consume newline

            if (dayOfWeek < 0 || dayOfWeek > 6) {
                printf("Error: Day of week out of range.\n\n");
                break;
            }

            printf("Enter Shift: (0 for morning, 1 for afternoon, 2 for evening) ");
            scanf("%d", &shift);
            getchar(); // Consume newline

            if (shift < 0 || shift > 2) {
                printf("Error: Shift out of range.\n\n");
                break;
            }

            printf("Enter the doctor's name: ");
            fgets(doctorName, NAME_MAX_LENGTH, stdin);
            doctorName[strcspn(doctorName, "\n")] = 0;

            strcpy(schedule[dayOfWeek][shift].DoctorName, doctorName);
            printf("Doctor %s has been added to the schedule on day %d, shift %d\n\n",
                   doctorName,
                   dayOfWeek,
                   shift);
            break;
        }
        case 2: {
            char* daysOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
            char* shifts[] = {"Morning", "Afternoon", "Evening"};

            printf("Doctor Weekly Schedule:\n");
            printf("%-12s %-12s %-30s\n", "Day", "Shift", "Doctor Name");

            for (int day = 0; day <= 6; day++) {
                for (int shift = 0; shift <= 2; shift++) {
                    printf("%-12s %-12s %-30s\n",
                           daysOfWeek[day],
                           shifts[shift],
                           strlen(schedule[day][shift].DoctorName) > 0 ? schedule[day][shift].DoctorName : "No doctor assigned");
                }
            }
            printf("\n");
            break;
        }
        default:
            printf("Error: Invalid choice. Please try again.\n\n");
            break;
    }
}

// Helper function to display one patient's record
void displayOnePatientDetails(Patient *patient) {
    if (patient == NULL) return;
    printf("Patient ID: %d, Name: %s, Age: %d, Diagnosis: %s, Room Number: %d\n",
           patient -> patientID,
           patient -> name,
           patient -> age,
           patient -> diagnosis,
           patient -> roomNumber);
}

// fixed validatePatientID (Linked List)
int validatePatientID(int newPatientID) {
    Patient *current = head;
    while (current != NULL) {
      if (current -> patientID == newPatientID) {
        printf("Error: Patient #&d already exists.\n\n", newPatientID);
        return 1;
      }
      current = current -> next;
    }
    return 0;
}

// Helper function to validate a patient's age
int validatePatientAge(struct PatientInformation *patient) {
    if (patient->age < PATIENT_MIN_AGE || patient->age > PATIENT_MAX_AGE) {
        printf("Error: Patient age must be between %d and %d.\n\n", PATIENT_MIN_AGE, PATIENT_MAX_AGE);
        return 1;
    }
    return 0;
}

void freeAllPatients() {
    Patient *current = head;
    while (current != NULL) {
        Patient *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    patientCount = 0;
}


void generateReports() {
    int choice;
    printf("REPORTING MENU: \n");
    printf("1. Total number of patients\n");
    printf("2. List of discharged patients (not tracked yet)\n");
    printf("3. Total shifts covered by each doctor(in a week)\n");
    printf("4. Room usage report\n");
    printf("5. Back to main menu\n");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1:
            printf("Total number of current patients: %d\n", patientCount);
        break;

        case 3: {
            int doctorShifts[100] = {0};
            char doctorNames[100][NAME_MAX_LENGTH];
            int count = 0;
            for (int i = 0; i < DAYS_IN_WEEK; i++) {
                for (int j = 0; j < SHIFTS_IN_DAY; j++) {
                    char *name = schedule[i][j].DoctorName;
                    if (strlen(name) == 0) continue;
                    int found = 0;
                    for (int k = 0; k < count; k++) {
                        if (strcmp(doctorNames[k], name) == 0) {
                            doctorShifts[k]++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strcpy(doctorNames[count], name);
                        doctorShifts[count++] = 1;
                    }
                }
            }
            printf("Doctor shift Summary:\n");
            for (int i = 0; i < count; i++) {
                printf("%s: %d shift\n", doctorNames[i], doctorShifts[i]);
            }
            break;
        }
        default:
            printf("Feature not implemented yet or invalid choice.\n");
        break;
    }
}
