#include <stdio.h>

struct Student 
{
    int rollNumber;
    char name[50];
    float marks1, marks2, marks3;
    float totalMarks;
    float averageMarks;
    char grade;
};

float calculateTotal(float m1, float m2, float m3) 
{
    return m1 + m2 + m3;
}

float calculateAverage(float total)
 {
    return total / 3.0;
}

char assignGrade(float average)
 {
    if (average >= 85)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 50)
        return 'C';
    else if (average >= 35)
        return 'D';
    else
        return 'F';
}

void displayPerformancePattern(char grade) 
{
    int stars = 0;

    switch (grade) 
    {
        case 'A': 
        stars = 5; 
        break;
        case 'B':
         stars = 4;
          break;
        case 'C': 
        stars = 3;
         break;
        case 'D': 
        stars = 2;
         break;

        default: 
        stars = 0;
         break;
    }

    if (grade == 'F') 
    {
        return;
    }

    for (int i = 0; i < stars; i++) 
    {
        printf("*");
    }
    printf("\n");
}

void printRollNumbersRecursively(int current, int total) 
{
    if (current > total)
     {
        return;
    }


    printf("%d ", current);
    printRollNumbersRecursively(current + 1, total);
}

int main()
 {
    int studentCount;

    printf("Enter number of students ");
    scanf("%d", &studentCount);

    struct Student students[studentCount];

    for (int i = 0; i < studentCount; i++) 
    {
        printf("\nEnter details for Student %d (Roll Name Marks1 Marks2 Marks3):\n", i + 1);
        scanf("%d %s %f %f %f", &students[i].rollNumber, students[i].name,
              &students[i].marks1, &students[i].marks2, &students[i].marks3);

        students[i].totalMarks = calculateTotal(students[i].marks1, students[i].marks2, students[i].marks3);
        students[i].averageMarks = calculateAverage(students[i].totalMarks);
        students[i].grade = assignGrade(students[i].averageMarks);
    }

    printf("\n Student Performance Report \n");

    for (int i = 0; i < studentCount; i++)
     {
        printf("\nRoll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %.2f\n", students[i].totalMarks);
        printf("Average: %.2f\n", students[i].averageMarks);

        printf("Grade: %c\n", students[i].grade);

        if (students[i].grade == 'F') 
        {
            continue; 
        }

        printf("Performance: ");
        displayPerformancePattern(students[i].grade);
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursively(1, studentCount);

    return 0;
}
