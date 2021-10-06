#include "../include/function_def.h"
#include "../include/jobs.h"

void sort_jobs(printprc *stopped, int n)
{
    // Sort jobs on the basis of process name
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (strcmp(stopped[j].name, stopped[j + 1].name) > 0)
            {
                printprc temp;
                temp = stopped[j];
                stopped[j] = stopped[j + 1];
                stopped[j + 1] = temp;
            }
        }
    }
}

void jobs_decider(char *str, int *proc_size)
{
    char nstr[strlen(str) + 1];
    strcpy(nstr, str);
    char *token, *saveptr = NULL;
    token = strtok_r(nstr, " \t\n", &saveptr);
    char args[max_number_args][max_size_args];
    int c = 0;
    while (token != NULL)
    {
        strcpy(args[c], token);
        c++;
        token = strtok_r(NULL, " \t\n", &saveptr);
    }
    if (c > 3)
    {
        printf("goyshell: jobs: Invalid number of arguments\n");
        return;
    }
    int rt = 0, st = 0;
    for (int i = 0; i < c; i++)
    {
        if (strcmp(args[i], "-r") == 0)
        {
            rt = 1;
        }
        else if (strcmp(args[i], "-s") == 0)
        {
            st = 1;
        }
    }
    if (c == 2 && rt == 1)
    {
        jobs(str, proc_size, 'R');
    }
    else if (c == 2 && st == 1)
    {
        jobs(str, proc_size, 'T');
    }
    else if (c == 1)
    {
        jobs(str, proc_size, '\0');
    }
    else if (c == 3 && st == 1 && rt == 1)
    {
        jobs(str, proc_size, '\0');
    }
    else
    {
        printf("goyshell: jobs: Invalid Arguments\n");
    }
}

// T for Stopped and other for Running

void jobs(char *str, int *proc_size, char character)
{
    int print_counter = 0;
    printprc selected[max_process_num];
    for (int i = 0; i < (*proc_size); i++)
    {
        if (proc[i].proc_id != -1)
        {
            char generate[max_size] = "/proc/";
            sprintf(generate, "/proc/%d/", proc[i].proc_id);
            char path_status[max_path_size];
            path_status[0] = '\0';
            strcpy(path_status, generate);
            strcat(path_status, "stat");
            FILE *fp = fopen(path_status, "r");
            char a[max_size], t[max_size];
            fscanf(fp, "%s %s %s", a, a, t);
            if (character == '\0')
            {
                selected[print_counter].job_num = proc[i].job_num;
                selected[print_counter].pid = proc[i].proc_id;
                strcpy(selected[print_counter].name, proc[i].proc_name);
                selected[print_counter].status = t[0];
                print_counter++;
            }
            else
            {
                if (t[0] == character || (character == 'R' && t[0] == 'S'))
                {
                    selected[print_counter].job_num = proc[i].job_num;
                    selected[print_counter].pid = proc[i].proc_id;
                    strcpy(selected[print_counter].name, proc[i].proc_name);
                    selected[print_counter].status = t[0];
                    print_counter++;
                }
            }
        }
    }
    sort_jobs(selected, print_counter);
    for (int i = 0; i < print_counter; i++)
    {
        if (selected[i].status == 'R' || selected[i].status == 'S')
            printf("[%d] Running %s [%d]\n", selected[i].job_num, selected[i].name, selected[i].pid);
        else if (selected[i].status == 'T')
            printf("[%d] Stopped %s [%d]\n", selected[i].job_num, selected[i].name, selected[i].pid);
    }
    return;
}