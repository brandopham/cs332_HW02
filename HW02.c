#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/* 
Name: Brandon Pham
BlazerId: vpham
Project #: 3
To compile: <instructions for compiling the program> 
just use make and make run from my makefile
To run: <instructions to run the program> 
    "./HW3 <flag> <Dir>"
        i.e "./HW3 -f" ".c 4" -S -t f"
*/ 
#define TRUE 1
#define MAX_PATH_SIZE 3000 //"Love you 3000 -Ironman"
#define FALSE 0

typedef struct sixFlags
{
    int f1; // "S"
    int f2; // "s"
    int ff2;
    int f3; // "f"
    char *ff3; 
    int deep;
    int f4; // "t"
    char *ff4;
} sixFlags;

sixFlags* init_sixFlags(sixFlags *six)
{
    six->f1 = FALSE;
    six->f2 = FALSE;
    six->ff2 = 0;
    six->f3 = FALSE;
    six->ff3 = NULL;
    six->deep = 0;

    return six;
}
int fileExplore(char *fileName, struct stat *stats, Flags flags, int tabSpaces)
{
    if (flags.is2 && !(stats->st_size <= flags.arg2))
    {
        return FALSE;
    }
    if (flags.is3 && (strstr(fileName, flags.arg3) == NULL || !(tabSpaces <= flags.depth))) 
    {
        return FALSE;
    }
    if (flags.is4 && (strcmp(flags.arg4, "f") == 0) && ((stats->st_mode & S_IFMT) != S_IFREG)) 
    {
        return FALSE;
    }
    if (flags.is4 && (strcmp(flags.arg4, "d") == 0) && ((stats->st_mode & S_IFMT) != S_IFDIR)) 
    {
        return FALSE;
    }

    return TRUE;
}
typedef void holder(char *fileName, struct stat *stats, int *count, int tabSpaces, Flags flags);

void newPrint(char *fileName, struct stat *stats, int *count, int tabSpaces, Flags flags)
{
    if (!flags.is1) 
    {
        printf("%*s[%d] %s\n", 4 * tabSpaces, "", *count, fileName); 
    }
    else 
    {
        if ((stats->st_mode & S_IFMT) == S_IFDIR) 
        {
            printf("%*s[%d] %s %d Bytes, %o, %s\n", 4 * tabSpaces, "", *count, fileName, 0, stats->st_mode & 0777, ctime(&stats->st_mtime)); 
        }
        else 
        {
            printf("%*s[%d] %s %lld Bytes, %o, %s\n", 4 * tabSpaces, "", *count, fileName, stats->st_size, stats->st_mode & 0777, ctime(&stats->st_mtime));
        } 
    }
    *(count) += 1;
}
void directTravel(char *path, int tabSpaces, Flags flags, MyFunc_1 funcPtr)
{
   struct dirent *dirent;
    struct stat stats;
    DIR *parentDir;

    if (tabSpaces == 0) 
    {
        printf("Starting Directory: %s\n", path);
    }

    parentDir = opendir(path);
    if (parentDir == NULL) 
    { 
        printf ("Error opening directory '%s'\n", path); 
        exit(-1);
    }

    int count = 1; 
    while((dirent = readdir(parentDir)) != NULL)
     {

        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) 
        {
            continue;
        }

        char *filePath = (char *) malloc(MAX_PATH_SIZE);
        strcpy(filePath, path);
        strcat(filePath, "/");
        strcat(filePath, dirent->d_name);

        if (stat(filePath, &stats) == -1) 
        {
            printf("Error with stat '%s\n", filePath);
            exit(-1);
        }

        if (check_File(dirent->d_name, &stats, flags, tabSpaces)) 
        {
            funcPtr(dirent->d_name, &stats, &count, tabSpaces, flags);
        }

        if (dirent->d_type == DT_DIR) 
        {
            directTravel(filePath, tabSpaces + 1, flags, funcPtr);
        }

        free(filePath);

    }
    closedir(parentDir); 
}
int main(int argc, char **argv) 
{

    int opt;
    Flags flags;

    init_Flags(&flags);

    while ((opt = getopt(argc, argv, "Ss:f:t:")) != -1) 
    {
        switch(opt) 
        {
            case 'S':
                flags.is1 = 1;
                break;
            case 's':
                flags.is2 = 1;
                flags.arg2 = atoi(optarg);
                break;
            case 'f':
                flags.is3 = 1;
                flags.arg3 = strtok(optarg, " ");
                flags.depth = atoi(strtok(NULL, " "));
                break;
            case 't':
                flags.is4 = 1;
                flags.arg4 = optarg;
        }
    }
    if (optind == argc) 
    {
        traverse_Directory(".", 0, flags, &print_File);
    }
    else 
    {
        traverse_Directory(argv[argc - 1], 0, flags, &print_File);
    }

    return 0;
}
