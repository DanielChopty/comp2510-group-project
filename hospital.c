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

// addNewPatient() {}