#define WORD_LENGTH 50
#define PATH_LENGTH 200

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

binsearch(char* dictionaryWords[],int listSize,char* keyword);
void loadDictionary(char** dictionary);
int getDictSize();
bool openFile();
void getFilePath();

//Global Vars
FILE* file;
char* filePath;

int main() {

    char* quit = "_QUIT";
    char* _quit = "_quit";
    getFilePath();

    //Terminate Program if Invalid Path
    if(!openFile(file))
    {
        printf("Your program has bee terminated because of an invalid file path.\n");
        return 0;
    }

    int dictSize =  getDictSize();//Get Dictionary Size
    fclose(file);

    char* dictionary[dictSize];
    loadDictionary(dictionary); //Load Dictionary Into array


    //Constantly ask the user for a word until _QUIT is typed
    while (true)
    {
        char tempWord[WORD_LENGTH];
        printf("***Type [_QUIT] to terminate the program***\n");
        printf("ENTER A WORD TO SEARCH: \n");

        scanf("%s", tempWord);
        if(strcmp(tempWord, quit) == 0 || strcmp(tempWord, _quit) == 0)
        {
            printf("Your program has been successfully terminated...");
            break;
        }
        int result = binsearch(dictionary,dictSize,tempWord);
        printf("---------------------------------------------------------\n");
        printf("RESULT: %d\n", result);
        (result != -1) ? printf("WORD [%s] WAS FOUND AT INDEX [%d]\n",tempWord, result):printf("WORD [%s] NOT FOUND..\n", tempWord);
        printf("---------------------------------------------------------\n");
        printf("\n");
    }

    return 0;
}

/*
 * Ask the user for the path of the dictionary
 */
void getFilePath()
{
    filePath = (char*)malloc(sizeof(char) + PATH_LENGTH); //
    printf("ENTER DICTIONARY ABSOLUTE PATH:\n");
    fgets(filePath, PATH_LENGTH, stdin);

    char* pnt;
    //Delete the return Key
    if (pnt = strchr(filePath, '\n'))
        *pnt = '\0';
}

/*
 * Opens the File
 */
bool openFile()
{
    file = fopen(filePath, "r");

    //File is Null
    if(file == NULL)
    {
        printf("------------------ INCOORECT PATH ---------------------\n");
        printf("    The Path of the File provided is Incorrect\n");
        return false;
    }
    return true;
}

/*
 * Returns the Dictionary Size
 */
int getDictSize()
{
    printf("------------------ GETTING DICTIONARY SIZE ---------------------\n");
    printf("    This Operation May take a few minues depending on your      \n");
    printf("    Computer's Speed. Please Wait, for the process to finish.   \n");
    char fileBuffer[WORD_LENGTH];
    int size;

    for(size= 0; !feof(file); size++)
    {
        fscanf(file, "%s", fileBuffer);//This is only to reach the EOF
    }

    printf("DICTIONARY SIZE: %d \n\n",size);
    return size;
}

/*
 * Loads the array with the content of the file
 */
void loadDictionary(char** dictionary)
{
    openFile();
    printf("----------------------LOADING DICTIONARY------------------------\n");
    printf("    This Operation May take a few minues depending on your      \n");
    printf("    Computer's Speed. Please Wait, for the process to finish.   \n");
    char fileBuffer[WORD_LENGTH];
    for(int i = 0; !feof(file); i++)
    {
        dictionary[i] = (char*)malloc( sizeof(char) * WORD_LENGTH);
        fscanf(file, "%s%*c", fileBuffer);
        strcpy(dictionary[i], fileBuffer);
    }
    fclose(file);
    printf("DICTIONARY HAS BEEN LOADED\n\n");
}

/*
 * Looks for the word in the dictionary
 * This method was created by Kianoush
 */
int binsearch(char* dictionaryWords[],int listSize,char* keyword) {
    int mid, low = 0, high = listSize - 1;
    while (high >= low) {
        mid = (high + low) / 2;
        if (strcmp(dictionaryWords[mid], keyword) < 0)
            low = mid + 1;
        else if (strcmp(dictionaryWords[mid], keyword) > 0)
            high = mid - 1;
        else
            return mid;
    }
    return -1; //not found
}

