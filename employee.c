#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "employees.dat"
#define MAX_NAME 50
#define MAX_DEPT 30

typedef struct {
    int id;
    char name[MAX_NAME];
    char department[MAX_DEPT];
    float salary;
} Employee;

// Add employee to file
void add_employee() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("File cannot be opened.\n");
        return;
    }

    Employee e;
    printf("Enter Employee ID: ");
    scanf("%d", &e.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", e.name);
    printf("Enter Department: ");
    scanf(" %[^\n]", e.department);
    printf("Enter Salary: ");
    scanf("%f", &e.salary);

    fwrite(&e, sizeof(Employee), 1, fp);
    fclose(fp);
    printf("Employee added successfully.\n");
}

// Display all employees
void display_employees() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    Employee e;
    printf("\n%-10s %-25s %-20s %-10s\n", "ID", "Name", "Department", "Salary");
    printf("-------------------------------------------------------------\n");
    while (fread(&e, sizeof(Employee), 1, fp)) {
        printf("%-10d %-25s %-20s %-10.2f\n", e.id, e.name, e.department, e.salary);
    }

    fclose(fp);
}

// Search by ID
void search_employee() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No data found.\n");
        return;
    }

    int id;
    printf("Enter Employee ID to search: ");
    scanf("%d", &id);

    Employee e;
    int found = 0;
    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.id == id) {
            printf("\nFound Employee:\n");
            printf("ID: %d\nName: %s\nDepartment: %s\nSalary: %.2f\n", e.id, e.name, e.department, e.salary);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No employee found with ID %d.\n", id);

    fclose(fp);
}

// Delete by ID
void delete_employee() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No data to delete.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    int id, found = 0;
    printf("Enter ID of Employee to delete: ");
    scanf("%d", &id);

    Employee e;
    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.id != id) {
            fwrite(&e, sizeof(Employee), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Employee deleted successfully.\n");
    else
        printf("No employee found with ID %d.\n", id);
}

// Update employee
void update_employee() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("No data found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter ID of employee to update: ");
    scanf("%d", &id);

    Employee e;
    while (fread(&e, sizeof(Employee), 1, fp)) {
        if (e.id == id) {
            printf("Enter new Name: ");
            scanf(" %[^\n]", e.name);
            printf("Enter new Department: ");
            scanf(" %[^\n]", e.department);
            printf("Enter new Salary: ");
            scanf("%f", &e.salary);

            fseek(fp, -sizeof(Employee), SEEK_CUR);
            fwrite(&e, sizeof(Employee), 1, fp);
            found = 1;
            break;
        }
    }

    if (found)
        printf("Employee updated.\n");
    else
        printf("Employee not found.\n");

    fclose(fp);
}

// Menu function
void menu() {
    int choice;
    while (1) {
        printf("\n====== Employee Management System ======\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee by ID\n");
        printf("4. Delete Employee\n");
        printf("5. Update Employee\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_employee(); break;
            case 2: display_employees(); break;
            case 3: search_employee(); break;
            case 4: delete_employee(); break;
            case 5: update_employee(); break;
            case 6: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}

