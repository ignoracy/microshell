#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>


#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define PURPLE "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m]"
#define DEFAULT_COLOR "\x1b[0m"
#define REDBG "\x1b[41m"
#define GREENBG "\x1b[42m"
#define YELLOWBG "\x1b[43m"
#define BLUEBG "\x1b[44m"
#define PURPLEBG "\x1b[45m"
#define CYANBG "\x1b[46m"

void Help()
{
    printf(CYAN);
    printf("%s\n", "Microshell by Ignacy Cieszynski");
    printf(PURPLE);
    printf("%s", "List of all possible commands:");
    printf(DEFAULT_COLOR);
    printf("%s", "\n");
    printf("%s", "help: lists all possible commands");
    printf("%s", "\n");
    printf("%s\n", "cd XXX: changes the working directory to XXX");
    printf("%s\n", "pwd: prints current working directory");
    printf("%s\n", "ls: lists contents of current working directory");
    printf("%s\n", "cat file1 file2: prints concatenated files file1 and file2");
    printf("%s\n", "touch file: creates file at current working directory");
    printf("%s\n", "cp file1 file2: copies file1 to file2");
    printf("%s", "exit: exits the program");
    printf("%s", "\n");
}

int main()
{
    char cwd[256];

    char previouscwd[256];
    getcwd(previouscwd, sizeof(previouscwd));

    while (1 == 1)
    {
        getcwd(cwd, sizeof(cwd));
        printf(BLUE "%s", cwd);
        printf("%s", "$" DEFAULT_COLOR);
        char command[100];
        fgets(command, 100, stdin);

        char d[] = "  \t \r\v\f\n";
        int counter = 0;
        char *command1[100];
        char *part = strtok(command, d);
        if (part == NULL)
        {
            continue;
        }
        while (part != NULL)
        {
            command1[counter] = part;
            counter++;
            part = strtok(NULL, d);
        }
        command1[counter] = NULL;

        if (strcmp(command1[0], "help") == 0)
        {
            Help();
        }

        else if (strcmp(command1[0], "cd") == 0)
        {
            if (strcmp(command1[1], "~") == 0)
            {
                chdir(getenv("HOME"));
                strcpy(previouscwd, cwd);
            }

            if (strcmp(command1[1], "-") == 0)
            {
                chdir(previouscwd);
                strcpy(previouscwd, cwd);
            }
            else
            {
                strcpy(previouscwd, cwd);
                chdir(command1[1]);
            }
        }
        else if (strcmp(command1[0], "pwd") == 0)
        {
            printf("%s\n", cwd);
        }

        else if (strcmp(command1[0], "ls") == 0)
        {

            DIR *dir;
            struct dirent *entry;

            dir = opendir(cwd);

            if (dir != NULL)
            {
                while ((entry = readdir(dir)) != NULL)
                {
                    printf(YELLOW "%s\n", entry->d_name);
                }

                closedir(dir);
            }
            else
            {
                perror("Error: Could not open the directory");
            }
        }
        else if (strcmp(command1[0], "cp") == 0)
        {

            FILE *src_fp = fopen(command1[1], "r");

            if (src_fp != NULL)
            {
                FILE *dest_fp = fopen(command1[2], "w");

                if (dest_fp != NULL)
                {
                    char ch;

                    while ((ch = fgetc(src_fp)) != EOF)
                    {
                        fputc(ch, dest_fp);
                    }

                    fclose(dest_fp);
                }
                else
                {
                    perror("Error: Could not open the destination file");
                }

                fclose(src_fp);
            }
            else
            {
                perror("Error: Could not open the source file");
            }
        }
        else if (strcmp(command1[0], "cat") == 0)
        {

            FILE *fp = fopen(command1[1], "r");

            FILE *fp1 = fopen(command1[2], "r");

            if (fp != NULL)
            {
                char ch;

                while ((ch = fgetc(fp)) != EOF)
                {
                    putchar(ch);
                }

                fclose(fp);
            }

            else
            {
                perror("Error: Could not open the file");
            }
            if (fp1 != NULL)
            {
                char ch1;
                while ((ch1 = fgetc(fp1)) != EOF)
                {

                    putchar(ch1);
                }
                fclose(fp1);
            }
        }
        else if (strcmp(command1[0], "touch") == 0)
        {

            FILE *fp = fopen(command1[1], "w");

            if (fp != NULL)
            {
                fclose(fp);
            }
            else
            {
                perror("Error: Could not create the file");
            }
        }

        else if (strcmp(command1[0], "exit") == 0)
        {
            printf(DEFAULT_COLOR);
            return 0;
        }
        else if (strcmp(command1[0], "\n") == 0)
        {
            continue;
        }

        else
        {

            if (fork() == 0)
            {
                exit(execvp(command1[0], command1));
            }
        }
    }
    return 0;
}
