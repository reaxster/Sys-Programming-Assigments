#define PATH_SIZE 500

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void Fgets(char* userInput, int length);

void fixTR9(char* word);
void fixTC9(char* word, int parity);
bool convertAnother();
void allToCSV();
void allToTL5();
void allToTR9();
void allToTC9();
void menu(char*);
void welcomeMSG();
void removeSpaces(char* word);
bool loadFileFrom();
void saveFileTo();

//Global Variables
char sourcePath[500];
char destPath[500];
FILE* sourceFile = NULL;
FILE* destFile = NULL;

int main() {
    char command[10];
    welcomeMSG();
    do{
        if(!loadFileFrom())
            continue;
        saveFileTo();

        //Load menu
        menu(command);

        fclose(sourceFile); //Closing Source File
        fclose(destFile); //Closing Destination File
    }while (!(strcmp(command, "Q") == 0 || strcmp(command, "q") == 0) && convertAnother());

    return 0;
}

void menu(char* command)
{
    printf("+----------------------------SELECT AN OPTION-------------------------------+\n");
    printf("|   [S] Converts the File To .CSV                                           |\n");
    printf("|   [T] Converts the File To .TL5                                           |\n");
    printf("|   [R] Converts the File To .TR9                                           |\n");
    printf("|   [C] Converts the File To .TC9                                           |\n");
    printf("|   [Q] Quit the Application                                                |\n");
    printf("+---------------------------------------------------------------------------+\n");

    Fgets(command, 10);

    if(strcmp(command, "S") == 0 || strcmp(command, "s") == 0){
        printf("+-----CONVERTING TO CSV----+\n");
        allToCSV();
        printf("+-----FILE CONVERTED-------+\n");
    }
    else if(strcmp(command, "T") == 0 || strcmp(command, "t") == 0){
        printf("+-----CONVERTING TO TL5----+\n");
        allToTL5();
        printf("+-----FILE CONVERTED-------+\n");
    }
    else if(strcmp(command, "R") == 0 || strcmp(command, "r") == 0){
        printf("+-----CONVERTING TO TR9----+\n");
        allToTR9();
        printf("+-----FILE CONVERTED-------+\n");
    }
    else if(strcmp(command, "C") == 0 || strcmp(command, "c") == 0){
        printf("+-----CONVERTING TO TC9----+\n");
        allToTC9();
        printf("+-----FILE CONVERTED-------+\n");
    }
    else if(strcmp(command, "Q") == 0 || strcmp(command, "q") == 0)
    {
        printf("+---------------------------  EXIT REQUEST  --------------------------------+\n");
        printf("|   Exit request Received. Your Program Has been Terminated...              |\n");
        printf("+---------------------------------------------------------------------------+\n");
    }
}


//Convert from all sourceFile type to CSV
void allToCSV()
{
    char* filebuffer[20];
    bool first = true;
    while (!feof(sourceFile))
    {
        fscanf(sourceFile,"%[^|^,]%*c", filebuffer);
        char* pnt;

        if(first)
        {
            removeSpaces(filebuffer);
            fprintf(destFile, "%s", filebuffer);
            first = false;
        } else{
            if((pnt=strchr(filebuffer, '\n')) != NULL)
            {
                *pnt=' ';
                removeSpaces(filebuffer);
                fprintf(destFile, ",\n%s", filebuffer);
            }
            else{
                removeSpaces(filebuffer);
                fprintf(destFile, ",%s", filebuffer);
            }
        }
    }
}

//Convert from all sourceFile type to TL5
void allToTL5()
{
    char* filebuffer[20];
    bool first = true;
    while (!feof(sourceFile))
    {
        fscanf(sourceFile,"%[^,^|]%*c", filebuffer);
        char* pnt = NULL;

        if(first)
        {
            removeSpaces(filebuffer);
            fprintf(destFile, "%-5.5s", filebuffer);
            first = false;
        } else{
            if((pnt=strchr(filebuffer, '\n')) != NULL)
            {
                *pnt=' ';
                removeSpaces(filebuffer);
                fprintf(destFile, "|\n%-5.5s", filebuffer);
            }
            else{
                removeSpaces(filebuffer);
                fprintf(destFile, "|%-5.5s", filebuffer);
            }
        }
    }
}

//Convert from all sourceFile type to TR9
void allToTR9()
{
    char* filebuffer[20];
    bool first = true;
    while (!feof(sourceFile))
    {
        fscanf(sourceFile, "%[^,^|]%*c", filebuffer);
        char* pnt = NULL;
        if(first)
        {
            removeSpaces(filebuffer);
            fixTR9(filebuffer);
            fprintf(destFile, "%9.9s", filebuffer);
            first = false;
        } else{
            if((pnt=strchr(filebuffer, '\n')) != NULL)
            {
                *pnt=' ';
                removeSpaces(filebuffer);
                fixTR9(filebuffer);
                fprintf(destFile, "|\n%9.9s", filebuffer);
            }
            else{
                removeSpaces(filebuffer);
                fixTR9(filebuffer);
                fprintf(destFile, "|%9.9s", filebuffer);
            }
        }
    }
}

//Convert from all sourceFile type to TC9
void allToTC9()
{
    char* filebuffer[20];
    bool first = true;
    int parity;
    while (!feof(sourceFile))
    {
        fscanf(sourceFile, "%[^,^|]%*c", filebuffer);
        char* pnt = NULL;
        parity = strlen(filebuffer);
        if(first)
        {
            removeSpaces(filebuffer);
            fixTC9(filebuffer, parity);
            fprintf(destFile, "%9.9s", filebuffer);
            first = false;
        } else{
            if((pnt=strchr(filebuffer, '\n')) != NULL)
            {
                *pnt=' ';
                removeSpaces(filebuffer);
                fixTC9(filebuffer, (parity-1));
                fprintf(destFile, "|\n%9.9s", filebuffer);
            }
            else{
                removeSpaces(filebuffer);
                fixTC9(filebuffer, parity);
                fprintf(destFile, "|%9.9s", filebuffer);
            }
        }
    }
}


void Fgets(char* userInput, int length)
{
    fgets(userInput, length, stdin);
    char* pnt = strchr(userInput, '\n');
    *pnt = '\0';
}

/*This method removes the spaces od the word before the first letter and after the last one*/
void removeSpaces(char* word)
{
    char* content = (char*)malloc(sizeof(char) + strlen(word) + 30);
    int index = 0;
    for (int i = 0; i < strlen(word); ++i) {
        if(word[i] != ' ')
            content[index++]=word[i];
    }
    content[index] = '\0';
    strcpy(word, content);
    free(content);
}

/*
 * This method will format the word to be in the TR9 Format
 */
void fixTR9(char* word)
{
    char temp[10];
    int counter = 0;

    if(strlen(word)<=9)
        return;

    for (int i = (strlen(word)-9); i < (strlen(word)); i++) {
        temp[counter++] = word[i];
    }
    strcpy(word, temp);
}


/*
 * This method will format the word to be in the TC9 Format
 */
void fixTC9(char* word, int parity)
{
    char* temp = (char*)malloc(sizeof(char) + strlen(word) + 10);
    int counter = 0;
    int even = 8;
    int odd = 9;
    int maxSize = 9;
    int div;

    if(parity%2 == 0) { //Even Cases
        if(strlen(word) > maxSize){ //Greater than
            div = (strlen(word) - even) / 2;
            for (int i = (div/2) +1; i < (strlen(word) - (div/2)-1); i++) {
                temp[counter++] = word[i];
            }
        }else{//Less than
            div = ((maxSize - strlen(word)) / 2) ;

            for (int i = 0; i < div; i++)
                temp[counter++] = ' ';
            for (int i = 0; i < strlen(word); i++)
                temp[counter++] = word[i];
            for (int i = 0; i < div; i++)
                temp[counter++] = ' ';
        }
    } else{//Odd cases
        if(strlen(word) > maxSize){ //Greater than
            div = (strlen(word) - odd ) / 2;
            for (int i = (div/2)+1; i < (strlen(word) - (div/2)) - 1; i++) {
                temp[counter] = word[i];
                counter++;
            }
        }
        else{ //Less than
            div = ((maxSize - strlen(word)) / 2) ;
            for (int i = 0; i < div; i++)
                temp[counter++] = ' ';

            for (int i = 0; i < strlen(word); i++) {
                temp[counter++] = word[i];
            }

            for (int i = 0; i < div ; i++)
                temp[counter++] = ' ';
        }
    }
    temp[counter] = '\0';
    strcpy(word, temp);
    free(temp);
}

//ToBe Displayed at the beginning
void welcomeMSG() {
    printf("+-----------------------------  ASSIGMENT 5  -------------------------------+\n");
    printf("|           Name:       Richar Marshall         PID:6169881                 |\n");
    printf("|           Class:      COP4338                 System Programming          |\n");
    printf("|           Professor:  Kianoosh G. Boroojeni   Term: SUMMER A              |\n");
    printf("|           Date:       June 16, 2020                                       |\n");
    printf("+---------------------------------------------------------------------------+\n");
    printf("\n");
    printf("+-----------------------------IMPORTANT NOTE--------------------------------+\n");
    printf("|   This Program Supports the Following File Conversions:                   |\n");
    printf("|   [.CSV]  [.TL5]  [.TR9]  [.TC9]                                          |\n");
    printf("|                                                                           |\n");
    printf("|   WARNING: Only Single Words are Accepted. Spaces Will be Removed Even    |\n");
    printf("|            When Converting Between the Same File Types (i.e CSV to CSV)   |\n");
    printf("|   WARNING: If the Extension of the Source File does not Reflect the       |\n");
    printf("|            Content of it, Conversion will be Carried out With Errors      |\n");
    printf("|   WARNING: The Delimiters for These formats are [|] and [,] and [\\n]      |\n");
    printf("|            If any Delimiter is Found at the Beginning or the End of the   |\n");
    printf("|            File, Conversion will be Carried out With Errors               |\n");
    printf("|   WARNING: It's the User Reponsability to Verify and Enter a Valid        |\n");
    printf("|            Destination Path. Otherwise sourceFile will be converted             |\n");
    printf("+---------------------------------------------------------------------------+\n");
}


/*
 * This method will load the source sourceFile path
 */
bool loadFileFrom()
{
    printf("\n\n+--------------------------  SOURCE FILE PATH  -----------------------------+\n");
    printf("|   Enter the Absolute Source File Path. IT MUST BE THE ABSOLUTE PATH       |\n");
    printf("+---------------------------------------------------------------------------+\n");
    printf("Enter File Source Path:\n");

    Fgets(sourcePath, PATH_SIZE);
    sourceFile = fopen(sourcePath, "r");

    if(sourceFile == NULL)
    {
        printf("+------------------------  ERROR OPENING FILE  -----------------------------+\n");
        printf("|   The File Cannot be Opened at This Time.                                 |\n");
        printf("|   Please Check if the File Exist or if it's Being Used by Another Program |\n");
        printf("+---------------------------------------------------------------------------+\n");
        return false;
    }

    return true;
}

/*
 * This method will load the destination sourceFile path
 */
void saveFileTo()
{
    printf("Enter Destination Path\n");
    Fgets(destPath, PATH_SIZE);
    destFile = fopen(destPath, "w");
}

/*
 * Prompts for another file conversion
 */
bool convertAnother()
{
    char convert[10];
    printf("DO YOU WANT TO CONVERT ANOTHER FILE: \n");
    printf("Type [Y] to convert. Type anu other key to QUIT: \n");
    Fgets(convert, 10);
    if(strcmp(convert, "Y") == 0 || strcmp(convert, "y") == 0)
        return true;

    printf("+---------------------------  EXIT REQUEST  --------------------------------+\n");
    printf("|   Exit request Received. Your Program Has been Terminated...              |\n");
    printf("+---------------------------------------------------------------------------+\n");
    return false;
}
