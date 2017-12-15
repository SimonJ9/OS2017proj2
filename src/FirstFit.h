//implement first fit algo here
#include <stdbool.h>

int dfrag_t = 0;
int t = 0;
int s_size = FRAME_SIZE;

bool isvalueinarray(char val, char arr[26], int size){
    int i;
    for (i = 0; i < size; i++) {
        if (arr[i] == val)
        {
            return true;
        }
    }
    return false;
}

void add(struct process_list * pl, bool * flag, char frame[FRAME_SIZE], int i)
{
    unsigned int j;
    for (j = 0; j < FRAME_SIZE; j++)
    {
        if ( *flag )
        {   
            break;
        }
        if (frame[j] == '.')
        {
            int temp = 0;
            while ( frame[j+temp] == '.' )
            {
                if ( *flag )
                {
                    break;
                }
                if (temp + 1 == pl->list[i]._mem)
                {
                    unsigned int k;
                    s_size -= temp + 1;
                    *flag = true;
                    for (k = j; k < j + temp + 1; k++)
                    {
                        frame[k] = pl->list[i].id;
                    }
                    printf("time %dms: Placed process %c:\n", t+dfrag_t, pl->list[i].id);
                    fflush(stdout);
                    print_frames(stdout, frame);
                }
                temp++;
            }
        }
    }
}

void Sim_First_Fit(struct process_list* pl, FILE* output)
{
    int max_t = 0;
    unsigned int z = 0;
    for (; z < pl->_size; z++)
    {
        if (pl->list[z].t_running_1 + pl->list[z].t_arrival_1 > max_t)
        {
            max_t = pl->list[z].t_running_1 + pl->list[z].t_arrival_1;
        }
        if (pl->list[z].t_running_2 + pl->list[z].t_arrival_2 > max_t)
        {
            max_t = pl->list[z].t_running_2 + pl->list[z].t_arrival_2;
        }
        if (pl->list[z].t_running_3 + pl->list[z].t_arrival_3 > max_t)
        {
            max_t = pl->list[z].t_running_3 + pl->list[z].t_arrival_3;
        }
    }
    char frame[FRAME_SIZE];
    unsigned int f_index = 0;
    for (; f_index < FRAME_SIZE; f_index++)
    {
        frame[f_index] = '.';
    }
    printf("time %dms: Simulator started (Contiguous -- First-Fit)\n", 0);
    fflush(stdout);
    while (true)
    {
        if (t > max_t)
        {
            printf("time %dms: Simulator ended (Contiguous -- First-Fit)\n", t+dfrag_t-1);
            fflush(stdout);
            printf("\n");
            fflush(stdout);
            break;
        }
        unsigned int i, j;
        for (i = 0; i < pl->_size; i++)
        {
            bool flag2 = false;
            if ( pl->list[i].t_arrival_1 + pl->list[i].t_running_1 != 0 )
            {
                if ( t == pl->list[i].t_arrival_1 + pl->list[i].t_running_1 )
                {
                    for (j = 0; j < FRAME_SIZE; j++)
                    {
                        if (frame[j] == pl->list[i].id)
                        {
                            flag2 = true;
                            frame[j] = '.';
                            s_size++;
                        }
                    }
                    if (flag2)
                    {
                        printf("time %dms: Process %c removed:\n", t+dfrag_t, pl->list[i].id);
                        fflush(stdout);
                        print_frames(stdout, frame);
                    }
                }
            }
            flag2 = false;
            if ( pl->list[i].t_arrival_2 + pl->list[i].t_running_2 != 0 )
            {
                if ( t == pl->list[i].t_arrival_2 + pl->list[i].t_running_2 )
                {
                    for (j = 0; j < FRAME_SIZE; j++)
                    {
                        if (frame[j] == pl->list[i].id)
                        {
                            flag2 = true;
                            frame[j] = '.';
                            s_size++;
                        }
                    }
                    if (flag2)
                    {
                        printf("time %dms: Process %c removed:\n", t+dfrag_t, pl->list[i].id);
                        fflush(stdout);
                        print_frames(stdout, frame);
                    }
                }
            }
            flag2 = false;
            if ( pl->list[i].t_arrival_3 + pl->list[i].t_running_3 != 0 )
            {
                if ( t == pl->list[i].t_arrival_3 + pl->list[i].t_running_3 )
                {
                    for (j = 0; j < FRAME_SIZE; j++)
                    {
                        if (frame[j] == pl->list[i].id)
                        {
                            flag2 = true;
                            frame[j] = '.';
                            s_size++;
                        }
                    }
                    if (flag2)
                    {
                        printf("time %dms: Process %c removed:\n", t+dfrag_t, pl->list[i].id);
                        fflush(stdout);
                        print_frames(stdout, frame);
                    }
                }
            }
        }
        for (i = 0; i < pl->_size; i++)
        {
            if ( t == pl->list[i].t_arrival_1 &&
                 pl->list[i].t_running_1 != 0)
            {
                printf("time %dms: Process %c arrived (requires %d frames)\n", t+dfrag_t, pl->list[i].id, pl->list[i]._mem);
                fflush(stdout);
                if (s_size >= pl->list[i]._mem)
                {
                    bool flag = false;
                    add(pl, &flag, frame, i);
                    if ( !flag )
                    {
                        printf("time %dms: Cannot place process %c -- starting defragmentation\n", t+dfrag_t, pl->list[i].id);
                        fflush(stdout);
                        int dfrag = 0;
                        int numofempty = 0;
                        int dfrag_t2 = 0;
                        int dfrag_s = 0;
                        char t_arr[26];
                        unsigned int j;
                        for (j = 0; j < 26; j++)
                        {
                            t_arr[j] = '.';
                        }
                        for (j = 0; j < FRAME_SIZE; j++)
                        {
                            if (frame[j] == '.')
                            {
                                numofempty++;
                            }
                        }
                        for (j = 0; j < FRAME_SIZE; j++)
                        {
                            if (frame[j] == '.')
                            {
                                dfrag++;
                            }
                            else
                            {
                                frame[j-dfrag] = frame[j];
                                if ( dfrag != 0 )
                                {
                                    if (!isvalueinarray(frame[j], t_arr, 26))
                                    {
                                        z = 0;
                                        for (; z < 26; z++)
                                        {
                                            if (t_arr[z] == '.')
                                            {
                                                dfrag_s++;
                                                t_arr[z] = frame[j];
                                                break;
                                            }
                                        }
                                    }
                                    dfrag_t++;
                                    dfrag_t2++;
                                }
                            }
                        }
                        for (j = 0; j < numofempty; j++)
                        {
                            frame[FRAME_SIZE-j-1] = '.';
                        }
                        printf("time %dms: Defragmentation complete (moved %d frames:", t+dfrag_t, dfrag_t2);
                        fflush(stdout);
                        for (j = 0; j < dfrag_s; j++)
                        {
                            printf(" %c", t_arr[j]);
                            if (j != dfrag_s - 1)
                            {
                                printf(",");
                                fflush(stdout);
                            }
                            else
                            {
                                printf(")\n");
                                fflush(stdout);
                            }
                        }
                        print_frames(stdout, frame);
                        add(pl, &flag, frame, i);
                    }
                }
                else
                {
                    printf("time %dms: Cannot place process %c -- skipped!\n", t+dfrag_t, pl->list[i].id);
                    fflush(stdout);
                    print_frames(stdout, frame);
                    continue;
                }
            }
            if ( t == pl->list[i].t_arrival_2 &&
                 pl->list[i].t_running_2 != 0)
            {
                printf("time %dms: Process %c arrived (requires %d frames)\n", t+dfrag_t, pl->list[i].id, pl->list[i]._mem);
                fflush(stdout);
                if (s_size >= pl->list[i]._mem)
                {
                    bool flag = false;
                    add(pl, &flag, frame, i);
                    if ( !flag )
                    {
                        printf("time %dms: Cannot place process %c -- starting defragmentation\n", t+dfrag_t, pl->list[i].id);
                        fflush(stdout);
                        int dfrag = 0;
                        int numofempty = 0;
                        int dfrag_s = 0;
                        int dfrag_t2 = 0;
                        char t_arr[26];
                        unsigned int j;
                        for (j = 0; j < 26; j++)
                        {
                            t_arr[j] = '.';
                        }
                        for (j = 0; j < FRAME_SIZE; j++)
                        {
                            if (frame[j] == '.')
                            {
                                numofempty++;
                            }
                        }
                        for (j = 0; j < FRAME_SIZE; j++)
                        {
                            if (frame[j] == '.')
                            {
                                dfrag++;
                            }
                            else
                            {
                                frame[j-dfrag] = frame[j];
                                if ( dfrag != 0 )
                                {
                                    if (!isvalueinarray(frame[j], t_arr, 26))
                                    {
                                        z = 0;
                                        for (; z < 26; z++)
                                        {
                                            if (t_arr[z] == '.')
                                            {
                                                dfrag_s++;
                                                t_arr[z] = frame[j];
                                                break;
                                            }
                                        }
                                    }
                                    dfrag_t++;
                                    dfrag_t2++;
                                }
                            }
                        }
                        for (j = 0; j < numofempty; j++)
                        {
                            frame[FRAME_SIZE-j-1] = '.';
                        }
                        printf("time %dms: Defragmentation complete (moved %d frames:", t+dfrag_t, dfrag_t2);
                        fflush(stdout);
                        for (j = 0; j < dfrag_s; j++)
                        {
                            printf(" %c", t_arr[j]);
                            if (j != dfrag_s - 1)
                            {
                                printf(",");
                                fflush(stdout);
                            }
                            else
                            {
                                printf(")\n");
                                fflush(stdout);
                            }
                        }
                        print_frames(stdout, frame);
                        add(pl, &flag, frame, i);
                    }
                }
                else
                {
                    printf("time %dms: Cannot place process %c -- skipped!\n", t+dfrag_t, pl->list[i].id);
                    fflush(stdout);
                    print_frames(stdout, frame);
                    continue;
                }
            }
            if ( t == pl->list[i].t_arrival_3 &&
                 pl->list[i].t_running_3 != 0)
            {
                printf("time %dms: Process %c arrived (requires %d frames)\n", t+dfrag_t, pl->list[i].id, pl->list[i]._mem);
                fflush(stdout);
                if (s_size >= pl->list[i]._mem)
                {
                    bool flag = false;
                    add(pl, &flag, frame, i);
                    if ( !flag )
                    {
                        printf("time %dms: Cannot place process %c -- starting defragmentation\n", t+dfrag_t, pl->list[i].id);
                        fflush(stdout);
                        int dfrag = 0;
                        int numofempty = 0;
                        int dfrag_s = 0;
                        int dfrag_t2 = 0;
                        char t_arr[26];
                        unsigned int j;
                        for (j = 0; j < 26; j++)
                        {
                            t_arr[j] = '.';
                        }
                        for (j = 0; j < FRAME_SIZE; j++)
                        {
                            if (frame[j] == '.')
                            {
                                numofempty++;
                            }
                        }
                        for (j = 0; j < FRAME_SIZE; j++)
                        {
                            if (frame[j] == '.')
                            {
                                dfrag++;
                            }
                            else
                            {
                                frame[j-dfrag] = frame[j];
                                if ( dfrag != 0 )
                                {
                                    if (!isvalueinarray(frame[j], t_arr, 26))
                                    {
                                        z = 0;
                                        for (; z < 26; z++)
                                        {
                                            if (t_arr[z] == '.')
                                            {
                                                dfrag_s++;
                                                t_arr[z] = frame[j];
                                                break;
                                            }
                                        }
                                    }
                                    dfrag_t++;
                                    dfrag_t2++;
                                }
                            }
                        }
                        for (j = 0; j < numofempty; j++)
                        {
                            frame[FRAME_SIZE-j-1] = '.';
                        }
                        printf("time %dms: Defragmentation complete (moved %d frames:", t+dfrag_t, dfrag_t2);
                        fflush(stdout);
                        for (j = 0; j < dfrag_s; j++)
                        {
                            printf(" %c", t_arr[j]);
                            if (j != dfrag_s - 1)
                            {
                                printf(",");
                                fflush(stdout);
                            }
                            else
                            {
                                printf(")\n");
                                fflush(stdout);
                            }
                        }
                        print_frames(stdout, frame);
                        add(pl, &flag, frame, i);
                    }
                }
                else
                {
                    printf("time %dms: Cannot place process %c -- skipped!\n", t+dfrag_t, pl->list[i].id);
                    fflush(stdout);
                    print_frames(stdout, frame);
                    continue;
                }
            }
        }
        t++;
    }
}