void print_page(struct process_list* pl, char frames[FRAME_SIZE], FILE* output)
{
    int i, j;
    int len = 10, n = 0;
    int p, s;
    fprintf(output, "PAGE TABLE [page,frame]:\n");
    for(i = 0; i < pl->_size; i++)
    {
        p = 0, n = 0, s = 0;
        for(j = 0; j < FRAME_SIZE; j++)
        {
            if(frames[j] == pl->list[i].id)
            {
                fprintf(output, "%c: ", pl->list[i].id);
                break;
            }
        }
        for(j = 0; j < FRAME_SIZE; j++)
        {
            if(frames[j] == pl->list[i].id)
            {
                fprintf(output, "[%d,%d]", p, j);
                p++;
                n++;
                s++;
                if(n == len)
                {
                    n = 0;
                    fprintf(output, "\n");
                }
                else
                {
                    if(s < pl->list[i]._mem)
                        fprintf(output, " ");
                }
            }
            else
            {
                continue;
            }
        }
        
        if(n != len && n > 0)
        {
            fprintf(output, "\n");
        }
    }
}


void Sim_Non_Con(struct process_list* pl, FILE* output)
{
    int sim_time = 0;
    char frames[FRAME_SIZE];
    int remained = FRAME_SIZE;
    int counter = 0;
    
    int i, j, frame_ind;
    for(i = 0; i < FRAME_SIZE; i++)
    {
        frames[i] = '.';
    }
    frame_ind = 0;
    
    /*copy list*/
    struct process_list* plist = (struct process_list*)malloc(sizeof(struct process_list));
    initialize_list(*plist);
    plist->cap = pl->cap;
    plist->_size = 0;
    plist->_index = pl->_index;
    plist->list = (struct process*)realloc(plist->list, sizeof(struct process) * pl->cap);
    counter = pl->_size;
    
    for(i = 0; i < pl->_size; i++)
    {
        add_process(plist, pl->list[i]);
        if(pl->list[i].t_arrival_2 > 0)
        {
            counter++;
        }
        if(pl->list[i].t_arrival_3 > 0)
        {
            counter++;
        }
    }
    //algo
    printf("time %dms: Simulator started (Non-contiguous)\n", sim_time);
    fflush(stdout);

    while(counter > 0)
    {
        
        for(i = 0; i < plist->_size; i++)
        {
            
            /*remove*/
            if(sim_time == (plist->list[i].t_running_1 + plist->list[i].t_arrival_1) ||
                (sim_time == (plist->list[i].t_running_2 + plist->list[i].t_arrival_2) &&
                sim_time > 0) ||
                (sim_time == (plist->list[i].t_running_3 + plist->list[i].t_arrival_3) &&
                sim_time > 0))
            {
                for(j = 0; j < FRAME_SIZE; j++)
                {
                    if(frames[j] == plist->list[i].id)
                    {
                        frames[j] = '.';
                        remained++;
                    }
                }
                fprintf(output, "time %dms: Process %c removed:\n", 
                    sim_time, plist->list[i].id);
                fflush(stdout);
                print_frames(output, frames);
                print_page(plist, frames, output);
                counter--;
            }
            
            /*finished process*/
            //3 phases || 2 phases || 1 phase
            if((sim_time == plist->list[i].t_running_3 + plist->list[i].t_arrival_3 && 
                sim_time > 0) || 
                (sim_time == plist->list[i].t_running_2 + plist->list[i].t_arrival_2 && 
                plist->list[i].t_running_3 == 0 && sim_time > 0) || 
                (sim_time == plist->list[i].t_running_1 + plist->list[i].t_arrival_1 &&
                plist->list[i].t_running_3 == 0 && plist->list[i].t_running_2 == 0))
            {
                remove_process(plist, plist->list[i].id);
                i--;
            }
        }
        
        for(i = 0; i < plist->_size; i++)
        {
            
            if(plist->list[i].t_arrival_1 == sim_time ||
                (plist->list[i].t_arrival_2 == sim_time &&
                sim_time > 0) ||
                (plist->list[i].t_arrival_3 == sim_time &&
                sim_time > 0))
            {
                fprintf(output, "time %dms: Process %c arrived (requires %d frames)\n", 
                    sim_time, plist->list[i].id, plist->list[i]._mem);
                        fflush(stdout);
                //place process / skip 
                if(plist->list[i]._mem > remained)
                {
                    fprintf(output, "time %dms: Cannot place process %c -- skipped!\n", 
                        sim_time, plist->list[i].id);
                        fflush(stdout);
                    print_frames(output, frames);
                    print_page(plist, frames, output);
                    //total 1, now 1
                    if(plist->list[i].t_running_2 == 0 &&
                        plist->list[i].t_running_3 == 0)
                    {
                        remove_process(plist, plist->list[i].id);
                        counter--;
                        i--;
                    }
                    //total 2, now 2
                    else if(plist->list[i].t_running_3 == 0 && 
                        sim_time == plist->list[i].t_arrival_2)
                    {
                        remove_process(plist, plist->list[i].id);
                        counter--;
                        i--;
                    }
                    //total 2, now 1
                    else if(plist->list[i].t_running_3 == 0 && 
                        sim_time == plist->list[i].t_arrival_1)
                    {
                        counter--;
                        plist->list[i].t_arrival_1 = plist->list[i].t_arrival_2;
                        plist->list[i].t_running_1 = plist->list[i].t_running_2;
                        plist->list[i].t_arrival_2 = 0;
                        plist->list[i].t_running_2 = 0;
                    }
                    //total 3, now 3
                    else if(sim_time == plist->list[i].t_arrival_3 &&
                        sim_time > 0)
                    {
                        remove_process(plist, plist->list[i].id);
                        counter--;
                        i--;
                    }
                    //total 3, now 1: change to total 2
                    else
                    {
                        counter--;
                        plist->list[i].t_arrival_1 = plist->list[i].t_arrival_2;
                        plist->list[i].t_running_1 = plist->list[i].t_running_2;
                        plist->list[i].t_arrival_2 = plist->list[i].t_arrival_3;
                        plist->list[i].t_running_2 = plist->list[i].t_running_3;
                        plist->list[i].t_arrival_3 = 0;
                        plist->list[i].t_running_3 = 0;
                    }
                }
                //placing
                else
                {
                    remained -= plist->list[i]._mem;
                    int temp = plist->list[i]._mem;
                    frame_ind = 0;
                    while( temp > 0 )
                    {
                        if(frames[frame_ind] == '.')
                        {
                            frames[frame_ind] = plist->list[i].id;
                            temp--;
                        }
                        frame_ind++;
                    }
                    
                    fprintf(output, "time %dms: Placed process %c:\n", 
                        sim_time, plist->list[i].id);
                    fflush(stdout);
                
                    print_frames(output, frames);
                    print_page(plist, frames, output);
                }
                /*finish placing process*/
            }
            
        }
        
        sim_time++;
        
    }
    sim_time--;
    fprintf(output, "time %dms: Simulator ended (Non-contiguous)\n", sim_time);
    free_list(*plist);
    free(plist);
}