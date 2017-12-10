//implement first fit algo here
#include <stdbool.h>

int dfrag_t = 0;
int t = 0;
int s_size = FRAME_SIZE;

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
                    printf("time %dms: Process %c arrived (requires %d frames)\n", t+dfrag_t, pl->list[i].id, pl->list[i]._mem);
                    printf("time %dms: Placed process %c:\n", t+dfrag_t, pl->list[i].id);
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
    }
    char frame[FRAME_SIZE];
    unsigned int f_index = 0;
    for (; f_index < FRAME_SIZE; f_index++)
    {
        frame[f_index] = '.';
    }
    printf("time %dms: Simulator started (Contiguous -- First-Fit)\n", 0);
    while (true)
    {
        if (t > max_t)
        {
            break;
        }
        unsigned int i, j;
        for (i = 0; i < pl->_size; i++)
        {
            if ( pl->list[i].t_arrival_1 + pl->list[i].t_running_1 != 0 )
            {
                if ( t == pl->list[i].t_arrival_1 + pl->list[i].t_running_1 )
                {
                    for (j = 0; j < FRAME_SIZE; j++)
                    {
                        if (frame[j] == pl->list[i].id)
                        {
                            frame[j] = '.';
                            s_size++;
                        }
                    }
                    printf("time %dms: Process %c removed:\n", t+dfrag_t, pl->list[i].id);
                    print_frames(stdout, frame);
                }
            }
            if ( pl->list[i].t_arrival_2 + pl->list[i].t_running_2 != 0 )
            {
                if ( t == pl->list[i].t_arrival_2 + pl->list[i].t_running_2 )
                {
                    for (j = 0; j < FRAME_SIZE; j++)
                    {
                        if (frame[j] == pl->list[i].id)
                        {
                            frame[j] = '.';
                            s_size++;
                        }
                    }
                    printf("time %dms: Process %c removed:\n", t+dfrag_t, pl->list[i].id);
                    print_frames(stdout, frame);
                }
            }
        }
        for (i = 0; i < pl->_size; i++)
        {
            if ( t == pl->list[i].t_arrival_1 &&
                 pl->list[i].t_running_1 != 0)
            {
                if (s_size >= pl->list[i]._mem)
                {
                    bool flag = false;
                    add(pl, &flag, frame, i);
                    if ( !flag )
                    {
                        int dfrag = 0;
                        int numofempty = 0;
                        unsigned int j;
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
                                dfrag_t++;
                            }
                            else
                            {
                                frame[j-dfrag] = frame[j];
                            }
                        }
                        for (j = 0; j < numofempty; j++)
                        {
                            frame[FRAME_SIZE-j-1] = '.';
                        }
                        add(pl, &flag, frame, i);
                    }
                }
                else
                {
                    continue;
                }
            }
        }
        for (i = 0; i < pl->_size; i++)
        {
            if ( t == pl->list[i].t_arrival_2 &&
                 pl->list[i].t_running_2 != 0)
            {
                if (s_size >= pl->list[i]._mem)
                {
                    bool flag = false;
                    add(pl, &flag, frame, i);
                    if ( !flag )
                    {
                        int dfrag = 0;
                        int numofempty = 0;
                        unsigned int j;
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
                                dfrag_t++;
                            }
                            else
                            {
                                frame[j-dfrag] = frame[j];
                            }
                        }
                        for (j = 0; j < numofempty; j++)
                        {
                            frame[FRAME_SIZE-j-1] = '.';
                        }
                        add(pl, &flag, frame, i);
                    }
                }
                else
                {
                    continue;
                }
            }
        }
        t++;
    }
}