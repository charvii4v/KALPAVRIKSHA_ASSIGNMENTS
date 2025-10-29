#include <stdio.h>

struct Student 
{
    int rollNumber;
    char name[50];
    float subjectOneMarks;
    float subjectTwoMarks;
    float subjectThreeMarks;
    float totalMarks;
    float averageMarks;
    char grade;
};

float calculateTotal(float subjectOneMarks, float subjectTwoMarks, float subjectThreeMarks) 
{
    return subjectOneMarks + subjectTwoMarks + subjectThreeMarks;
}

float calculateAverage(float totalMarks)
{
    return totalMarks / 3.0;
}

char assignGrade(float averageMarks)
{
    if (averageMarks >= 85)
        return 'A';
    else if (averageMarks >= 70)
        return 'B';
    else if (averageMarks >= 50)
        return 'C';
    else if (averageMarks >= 35)
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

void printRollNumbersRecursively(int currentRollNumber, int totalStudents) 
{
    if (currentRollNumber > totalStudents)
    {
        return;
    }

    printf("%d ", currentRollNumber);
    printRollNumbersRecursively(currentRollNumber + 1, totalStudents);
}

int main()
{
    int studentCount;

    printf("Enter number of students: ");
    scanf("%d", &studentCount);

    struct Student students[studentCount];

    for (int i = 0; i < studentCount; i++) 
    {
        printf("\nEnter details for Student %d (RollNumber Name Subject1 Subject2 Subject3):\n", i + 1);
        scanf("%d %s %f %f %f", &students[i].rollNumber,  students[i].name, &students[i].subjectOneMarks,  &students[i].subjectTwoMarks,  &students[i].subjectThreeMarks);

        students[i].totalMarks = calculateTotal(students[i].subjectOneMarks, students[i].subjectTwoMarks, students[i].subjectThreeMarks);
        students[i].averageMarks = calculateAverage(students[i].totalMarks);
        students[i].grade = assignGrade(students[i].averageMarks);
    }

    printf("\n Student Performance Report \n");

    for (int i = 0; i < studentCount; i++)
    {
        printf("\nRoll Number: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total Marks: %.2f\n", students[i].totalMarks);
        printf("Average Marks: %.2f\n", students[i].averageMarks);
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
