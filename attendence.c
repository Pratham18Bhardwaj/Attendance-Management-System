#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50
#define STUDENT_FILE "students_names.txt"

// Function prototypes
void record_attendance(const char *filename, const char student_names[][NAME_LENGTH], int total_students);
void read_attendance(const char *filename);
void add_student_name(char student_names[][NAME_LENGTH], int *total_students);
void display_menu();
int load_student_names(char student_names[][NAME_LENGTH]);

int main() {
    char student_names[MAX_STUDENTS][NAME_LENGTH];
    int total_students = load_student_names(student_names); // Load student names from file
    int user_choice;
    char filename[100];

    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &user_choice);
        getchar(); // Consume the newline character after the choice

        switch (user_choice) {
            case 1:
                // Record attendance
                printf("Enter the filename in format 'batch number_dd-mm-yyyy': ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0; // Remove newline character
                record_attendance(filename, student_names, total_students);
                break;
            case 2:
                // Read attendance records
                printf("Enter the filename to read attendance: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0; // Remove newline character
                read_attendance(filename);
                break;
            case 3:
                // Add a new student name
                add_student_name(student_names, &total_students);
                break;
            case 4:
                // Exit the program
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void display_menu() {
    // Display the menu options
    printf("\nMenu:\n");
    printf("1. Record Attendance\n");
    printf("2. Read Attendance\n");
    printf("3. Add Student Name\n");
    printf("4. Exit\n");
}

int load_student_names(char student_names[][NAME_LENGTH]) {
    // Load student names from the file
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        perror("Error opening student names file");
        return 0; // Return 0 if no students are loaded
    }

    int count = 0;
    while (count < MAX_STUDENTS && fgets(student_names[count], NAME_LENGTH, file) != NULL) {
        student_names[count][strcspn(student_names[count], "\n")] = 0; // Remove newline character
        count++;
    }

    fclose(file);
    return count; // Return the number of students loaded
}

void record_attendance(const char *filename, const char student_names[][NAME_LENGTH], int total_students) {
    // Open the attendance file in append mode
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Get the current time for the attendance record
    time_t current_time = time(NULL);
    char *time_str = ctime(&current_time); // Convert to string
    time_str[strcspn(time_str, "\n")] = 0; // Remove newline character

    // Write the attendance record header
    fprintf(file, "Attendance recorded at: %s\n", time_str);
    fprintf(file, "Students present:\n");

    // Loop through each student and ask for their presence
    for (int i = 0; i < total_students; i++) {
        char response;
        printf("Is %s present? (p for present, any other key for absent): ", student_names[i]);
        scanf(" %c", &response); // Note the space before %c to consume any newline characters

        if (response == 'p' || response == 'P') {
            fprintf(file, "%s\n", student_names[i]); // Record the student's name if present
            printf("%s is marked as present.\n", student_names[i]);
        } else {
            printf("%s is marked as absent.\n", student_names[i]);
        }
    }

    fclose(file);
    printf("Attendance recorded successfully in %s.\n", filename);
}

void read_attendance(const char *filename) {
    // Open the attendance file in read mode
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening attendance file");
        return;
    }

    char line[256];
    printf("Attendance records in %s:\n", filename);
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line); // Print each line of the attendance record
    }

    fclose(file);
}

void add_student_name(char student_names[][NAME_LENGTH], int *total_students) {
    // Check if we can add more students
    if (*total_students >= MAX_STUDENTS) {
        printf("Cannot add more students. Maximum limit reached.\n");
        return;
    }

    char name[NAME_LENGTH];
    printf("Enter the name of the student to add: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline character

    // Open the student names file in append mode
    FILE *file = fopen(STUDENT_FILE, "a");
    if (file == NULL) {
        perror("Error opening student names file");
        return;
    }

    // Write the new name to the file
    fprintf(file, "%s\n", name);
    fclose(file);

    // Add the new name to the in-memory array
    strcpy(student_names[*total_students], name);
    (*total_students)++; // Increment the student count

    printf("Student name '%s' added successfully.\n", name);
}