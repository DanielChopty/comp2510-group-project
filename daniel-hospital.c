//
// Created by Daniel on 2025-01-29.
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
#define MAX_PATIENTS 50
#define NAME_MAX_LENGTH 20
#define DIAGNOSIS_MAX_LENGTH 100
#define PATIENT_MIN_AGE 1
#define PATIENT_MAX_AGE 125

// Structure to store patient information
struct PatientInformation
{
    int patientID;
    char name[NAME_MAX_LENGTH];
    int age;
    char diagnosis[DIAGNOSIS_MAX_LENGTH];
    int roomNumber;
};

// Structure to store doctor's name
typedef struct
{
    char DoctorName[NAME_MAX_LENGTH];
} DoctorSchedule;

// 2D array for doctor's schedules
DoctorSchedule schedule[DAYS_IN_WEEK][SHIFTS_IN_DAY];

// Global variables
struct PatientInformation patients[MAX_PATIENTS];
int currentPatientCount = 0;

// Prototype functions
void displayMenu();
void addNewPatient();
void viewAllPatients();
void searchForPatient();
void dischargePatient();
void manageDoctorSchedule();
void displayOnePatientDetails();
int validatePatientID();
int validatePatientAge();

int main()
{
    displayMenu();
    return 0;
}

// Function to display the menu interface
void displayMenu()
{
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
        printf("6. Exit\n");

        scanf("%d", &userChoice);
        getchar(); // Consume newline character left by scanf

        switch (userChoice)
        {
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
                printf("Exiting program. Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    while (userChoice != 6);
}

// 1. Add a New Patient
void addNewPatient()
{
    if (currentPatientCount >= MAX_PATIENTS)
    {
        printf("Error: Cannot add more patients, the system is full!\n");
        printf("\n");
        return;
    }

    struct PatientInformation newPatient;

    // Get patient details
    // Get patient's ID
    printf("Enter Patient ID: ");
    scanf("%d", &newPatient.patientID);
    getchar(); // Consume newline after entering the ID

    // Validate the patient's ID
    if (validatePatientID(newPatient.patientID) == 1)
    {
        return; // If patient ID is not unique, return to the main menu
    }

    // Get patient's name
    printf("Enter Patient Name: ");
    fgets(newPatient.name, NAME_MAX_LENGTH, stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = 0; // Remove trailing newline

    // Get patient's age
    printf("Enter Patient Age: ");
    scanf("%d", &newPatient.age);
    getchar(); // Consume newline

    // Validate the patient's age
    if (validatePatientAge(&newPatient) == 1)
    {
        return; // If age is invalid, return to the main menu
    }

    // Get patient's diagnosis
    printf("Enter Patient Diagnosis: ");
    fgets(newPatient.diagnosis, DIAGNOSIS_MAX_LENGTH, stdin);
    newPatient.diagnosis[strcspn(newPatient.diagnosis, "\n")] = 0; // Remove trailing newline

    // Get patient's room number
    printf("Enter Room Number: ");
    scanf("%d", &newPatient.roomNumber);
    getchar(); // Consume newline

    // Save the new patient record in the array
    patients[currentPatientCount] = newPatient;
    currentPatientCount++;

    // Feedback for user
    printf("%s Added!\n", newPatient.name);
    printf("\n");
}

// 2. View all Patients on File
void viewAllPatients()
{
    if (currentPatientCount == 0)
    {
        printf("No patients found!\n");
        printf("\n");
        return;
    }

    // Print the header with fixed width columns
    printf("%-12s %-20s %-6s %-30s %-12s\n", "Patient ID", "Name", "Age", "Diagnosis", "Room Number");

    for (int i = 0; i < currentPatientCount; i++)
    {
        // Print each patient's information with fixed column widths
        printf("%-12d %-20s %-6d %-30s %-12d\n",
               patients[i].patientID,
               patients[i].name,
               patients[i].age,
               patients[i].diagnosis,
               patients[i].roomNumber);
    }
    printf("\n");
}

// 3. Search for a Patient
void searchForPatient()
{
    if (currentPatientCount == 0)
    {
        printf("Error: No patients found!\n");
        printf("\n");
        return;
    }

    int userChoice;
    printf("How would you like to search for a patient?\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    scanf("%d", &userChoice);
    getchar(); // Consume newline

    switch (userChoice)
    {
        case 1:
        {
            int id;
            int found = 0;
            printf("Enter Patient ID: ");
            scanf("%d", &id);
            getchar(); // Consume newline

            for (int i = 0; i < currentPatientCount; i++)
            {
                if (patients[i].patientID == id)
                {
                    printf("Here is patient #%d's record:\n", patients[i].patientID);
                    displayOnePatientDetails(patients[i]);
                    printf("\n");
                    found = 1;
                    break;
                }
            }
            if (found == 0)
            {
                printf("Error: Patient with ID %d not found.\n", id);
                printf("\n");
            }
            break;
        }
        case 2:
        {
            char name[NAME_MAX_LENGTH];
            int found = 0;
            printf("Enter Patient Name: ");
            fgets(name, NAME_MAX_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0; // Remove trailing newline

            for (int i = 0; i < currentPatientCount; i++)
            {
                if (strcmp(patients[i].name, name) == 0)
                {
                    printf("Here is %s's record:\n", patients[i].name);
                    displayOnePatientDetails(patients[i]);
                    printf("\n");
                    found = 1;
                    break;
                }
            }
            if (found == 0)
            {
                printf("Error: Patient with name %s not found.\n", name);
                printf("\n");
            }
            break;
        }
        default:
        {
            printf("Invalid choice. Please try again.\n");
            printf("\n");
            break;
        }
    }
}

// 4. Discharge a Patient by ID
void dischargePatient()
{
    if (currentPatientCount == 0)
    {
        printf("Error: No patients found!\n");
        printf("\n");
        return;
    }

    int id;
    int found = 0;
    printf("Enter ID of patient to be discharged: ");
    scanf("%d", &id);
    getchar(); // Consume newline

    for (int i = 0; i < currentPatientCount; i++)
    {
        if (patients[i].patientID == id)
        {
            // Discharge the patient by shifting records
            for (int j = i; j < currentPatientCount - 1; j++)
            {
                patients[j] = patients[j + 1];
            }
            currentPatientCount--;
            printf("Patient #%d has been discharged.\n", id);
            printf("\n");
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("Error: Patient with ID %d not found.\n", id);
        printf("\n");
    }
}

// 5. Manage Doctors' Weekly Schedules
void manageDoctorSchedule()
{
    int userChoice;
    printf("What would you like to do with the doctors' schedule?\n");
    printf("1. Assign a doctor to a shift\n");
    printf("2. Display the full weekly schedule\n");
    scanf("%d", &userChoice);
    getchar(); // Consume newline

    switch (userChoice)
    {
        case 1:
        {
            int dayOfWeek;
            int shift;
            char doctorName[NAME_MAX_LENGTH];

            printf("Enter Day: (0 for Sunday, 1 for Monday, 2 for Tuesday...) ");
            scanf("%d", &dayOfWeek);
            getchar(); // Consume newline

            if (dayOfWeek < 0 || dayOfWeek > 6)
            {
                printf("Error: Day of week out of range.\n");
                printf("\n");
                break;
            }

            printf("Enter Shift: (0 for morning, 1 for afternoon, 2 for evening) ");
            scanf("%d", &shift);
            getchar(); // Consume newline

            if (shift < 0 || shift > 2)
            {
                printf("Error: Shift out of range.\n");
                printf("\n");
                break;
            }

            printf("Enter the doctor's name: ");
            fgets(doctorName, NAME_MAX_LENGTH, stdin);
            doctorName[strcspn(doctorName, "\n")] = 0; // Remove trailing newline

            strcpy(schedule[dayOfWeek][shift].DoctorName, doctorName);
            printf("Doctor %s has been added to the schedule on day %d, shift %d\n", doctorName, dayOfWeek, shift);
            printf("\n");
            break;
        }

        case 2: {
            char* daysOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
            char* shifts[] = {"Morning", "Afternoon", "Evening"};

            printf("Doctor Weekly Schedule:\n");

            // Print schedule with improved spacing
            printf("%-12s %-12s %-30s\n", "Day", "Shift", "Doctor Name");

            for (int day = 0; day <= 6; day++)
            {
                for (int shift = 0; shift <= 2; shift++)
                {
                    // Align columns properly with fixed width for each
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
        {
            printf("Error: Invalid choice. Please try again.\n");
            printf("\n");
            break;
        }
    }
}

// Helper function to display one patient's record
void displayOnePatientDetails(struct PatientInformation patient)
{
    printf("Patient ID: %d, "
               "Name: %s, "
               "Age: %d, "
               "Diagnosis: %s, "
               "Room Number: %d\n",
            patient.patientID,
            patient.name,
            patient.age,
            patient.diagnosis,
            patient.roomNumber);
}

// Helper function to validate a patient's ID
int validatePatientID(int newPatientID)
{
    for (int i = 0; i < currentPatientCount; i++)
    {
        if (patients[i].patientID == newPatientID)
        {
            printf("Error: Patient #%d already exists.\n", newPatientID);
            printf("\n");
            return 1; // Not unique
        }
    }
    return 0; // Unique
}

// Helper function to validate a patient's age
int validatePatientAge(struct PatientInformation patient)
{
    if (patient.age < PATIENT_MIN_AGE || patient.age > PATIENT_MAX_AGE)
    {
        printf("Error: Patient age must be a valid positive integer (from 1 to 125).\n");
        printf("\n");
        return 1; // Invalid age
    }
    return 0; // Valid age
}
