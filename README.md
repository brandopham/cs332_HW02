Hello this is a README file,
This is an indepth way of explaining Brandon Phams HW02.c
typedef void fnptr(char* string);
void newPrint(fnptr* function, char* string)
{
function(string);
function("\n");
}
void fileExplore (const char *directory, const int root, int s, int S, int f)
{
DIR *dirPtr;
dirPtr = opendir(directory);
struct dirent *entry;
struct stat status;
char path[1000];
if (!dirPtr)
{
return;
}
while ((entry = readdir(dirPtr)) != NULL)
{

        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            for (int i=0; i<root; i++)
            {
                if (i%2 == 0 || i == 0)
                    printf("    ");
                else
                    printf(" ");
            }
            printf("    %s", entry->d_name);
            if (s == 1){
                // printf("\n%s\n", path);
                int stat = lstat(path, &status);
                int size = status.st_size;
                printf(" - %d" , size);
            }
            printf("\n");

            strcpy(path, directory);
            strcat(path, "/");
            strcat(path, entry->d_name);
            fileExplore(path, root + 2,s,S,f);
        }
    }
    if (closedir(dirPtr) == -1)
    {
        printf("Error closing directory.\n");
        exit (1);
    }

}
I used my Lab05.c as a base and built it differently to cope with what i had to do as far as Implementation of search that lists the files and directories in the specified format when executed with no arguments or when the directory name is specified as an argument. I was able to do the Correct use of function pointers to implement the required functionality.Implementation of search that lists the files and directories in the specified format when executed with -s option. Use of Makefile, source code documentation (including README.txt) "This is of course the README.txt"

Main Function was changed a lot compared to my Lab05.c file because i had to make the adjustments for the -s -S and -f
int main(int argc, char _argv[])
{
int f,c,S = 0;
int opt;
char_ copy = argv[1];
printf("Path: ");
newPrint(printf,argv[1]);
if (argc < 2)
{
printf("Usage: %s <path>", argv[0]);
return -1;
}
char s[strlen(argv[1])];
strcpy(s, argv[1]);
char \*initialDir = strtok(s, "/");
printf("%s\n", initialDir);

    char* flagargs[2];
    while((opt = getopt(argc,argv, "f:Ss:"))!= -1)
    {
        switch(opt)
        {
            case 'f':
                f=1;
                flagargs[0] = optarg;
                break;
            case 's':
                c=1;
                flagargs[1] = optarg;
                break;
            case 'S':
                S=1;
                break;
        }
    }
    fileExplore(copy, 0,c,S,f);
    return 0;

}
added a switch case for the 3 different cases -s -S and -f
