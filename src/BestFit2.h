
void Sim_Best_Fit(struct process_list* pl, FILE* output)
{
    unsigned int sim_time = 0;
    char frames[FRAME_SIZE];
    unsigned int remained = FRAME_SIZE;
    unsigned int counter = 0;
    
    unsigned int i, j, frame_ind;
    for(i = 0; i < FRAME_SIZE; i++)
    {
        frames[i] = '.';
    }
    frame_ind = 0;
    
    /*copy list*/
    struct process_list* plist = (struct process_list*)malloc(sizeof(struct process_list));
    initialize_list(*plist);
    plist->cap = pl->cap;
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
    printf("time 0ms: Simulator started (Contiguous -- Best-Fit)\n");
    fflush(stdout);
    
    
    while(counter > 0)
    {   
        for(i = 0; i < plist->_size; i++)
        {
            //remove
            if(sim_time == (plist->list[i].t_running_1 + plist->list[i].t_arrival_1) ||
                (sim_time == (plist->list[i].t_running_2 + plist->list[i].t_arrival_2) &&
                sim_time > 0) ||
                (sim_time == (plist->list[i].t_running_3 + plist->list[i].t_arrival_3) &&
                sim_time > 0))
            {
                for(j = FRAME_SIZE; j > 0; j--)
                {
                    if(frames[j - 1] == plist->list[i].id)
                    {
                        frames[j - 1] = '.';
                        //frame_ind = j - 1;
                        remained++;
                        
                    }
                }
                fprintf(output, "time %dms: Process %c removed:\n", 
                    sim_time, plist->list[i].id);
                fflush(stdout);
                print_frames(output, frames);
                counter--;
            }
            
            //finished proceee
            //3 phases || 2 phases || 1 phase
            if((sim_time == plist->list[i].t_running_3 + plist->list[i].t_arrival_3 && 
                sim_time > 0) || 
                (sim_time == plist->list[i].t_running_2 + plist->list[i].t_arrival_2 && 
                plist->list[i].t_running_3 == 0 && sim_time > 0) || 
                (sim_time == plist->list[i].t_running_1 + plist->list[i].t_arrival_1 &&
                plist->list[i].t_running_3 == 0 && plist->list[i].t_running_2 == 0))
            {
                remove_process(plist, plist->list[i].id);
            }
            
            
            if(plist->list[i].t_arrival_1 == sim_time ||
                (plist->list[i].t_arrival_2 == sim_time &&
                sim_time > 0) ||
                (plist->list[i].t_arrival_3 == sim_time &&
                sim_time > 0))
            {
                fprintf(output, "time %dms: Process %c arrived (requires %d frames)\n", 
                    sim_time, plist->list[i].id, plist->list[i]._mem);
                        fflush(stdout);
            
                //place process
                if(plist->list[i]._mem > remained)
                {
                    fprintf(output, "time %dms: Cannot place process %c -- skipped!\n", 
                        sim_time, plist->list[i].id);
                        fflush(stdout);
                    print_frames(output, frames);
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
                    //total 3, now 2: already handled
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
                else
                {
                    //find the smallest slot suitable
                    int slot_len = 0, slot_ind = 0, temp = remained, slot_th = 0;
                    
                    for(frame_ind = 0; frame_ind < FRAME_SIZE; frame_ind++)
                    {
                        if(frames[frame_ind] == '.')
                        {
                            slot_len++;
                        }
                        else{
                            if(slot_len >= plist->list[i]._mem && slot_len < temp)
                            {
                                temp = slot_len;
                                slot_ind = frame_ind - slot_len;
                            }
                            if(slot_len > slot_th)
                            {
                                slot_th = slot_len;
                            }
                            slot_len = 0;
                        }
                    }
                    
                    //corner case
                    if(slot_len >= plist->list[i]._mem)
                    {
                        if(slot_len < temp || temp == remained)
                        {
                            temp = slot_len;
                            slot_ind = FRAME_SIZE - slot_len;
                        }
                        if(slot_len > slot_th)
                        {
                            slot_th = slot_len;
                        }
                    }
                    
                    if(temp >= plist->list[i]._mem && temp <= slot_th)
                    {
                        frame_ind = slot_ind;
                        remained -= plist->list[i]._mem;
                        //unsigned int temp = FRAME_SIZE + frame_ind;

                        for(j = frame_ind; j < frame_ind + plist->list[i]._mem; j++)
                        {
                            frames[j] = plist->list[i].id;
                        }
                        fprintf(output, "time %dms: Placed process %c:\n", 
                                sim_time, plist->list[i].id);
                        fflush(stdout);
                            //printf("frame_ind: %d\n", frame_ind);
                        print_frames(output, frames);
                    }
                    //defrag otherwise
                    else 
                    {
                        //defrag
                        fprintf(output, "time %dms: Cannot place process %c -- starting defragmentation\n", 
                            sim_time, plist->list[i].id);
                        fflush(stdout);
                        
                        unsigned int dtime = 0;
                        
                        unsigned int newid, oldid, charid;
                        char moved[BUFFER_SIZE];
                        charid = 0;
                        newid = 0;
                        moved[0] = '\0';
                        for(oldid = 0; oldid < FRAME_SIZE; oldid++)
                        {
                            if(oldid != 0 && frames[oldid] != '.')
                            {
                                frames[newid] = frames[oldid];
                                newid++;
                                dtime++;
                                
                                if(moved[charid] != frames[oldid])
                                {
                                    if(moved[charid] == '\0')
                                    {
                                        moved[charid] = frames[oldid];
                                        moved[charid + 1] = ',';
                                        moved[charid + 2] = ' ';
                                    }
                                    else
                                    {
                                        charid += 3;
                                        moved[charid] = frames[oldid];
                                        moved[charid + 1] = ',';
                                        moved[charid + 2] = ' ';
                                    }
                                }
                            }
                        }
                        moved[charid + 1] = '\0';
                        
                        frame_ind = newid;
                        for(; newid < FRAME_SIZE; newid++)
                        {
                            frames[newid] = '.';
                        }
                        
                        //add pending time
                        for(j = 0; j < plist->_size; j++)
                        {
                            if(plist->list[j].t_arrival_1 > sim_time)
                            {
                                plist->list[j].t_arrival_1 += dtime;
                            }
                            else
                            {
                                plist->list[j].t_running_1 += dtime;
                            }
                            if(plist->list[j].t_arrival_2 > sim_time)
                            {
                                plist->list[j].t_arrival_2 += dtime;
                            }
                            else
                            {
                                plist->list[j].t_running_2 += dtime;
                            }
                            if(plist->list[j].t_arrival_3 > sim_time)
                            {
                                plist->list[j].t_arrival_3 += dtime;
                            }
                            else
                            {
                                plist->list[j].t_running_3 += dtime;
                            }
                        }
                        sim_time += dtime;
                        
                        fprintf(output, "time %dms: Defragmentation complete (moved %d frames: %s)\n", sim_time, dtime, moved);
                        fflush(stdout);
                        print_frames(output, frames);
                        
                        //////////////////////////////////////////////////////////////
                        
                        unsigned int a, b;
                        for(a = 0; a < plist->list[i]._mem; a++)
                        {
                            frames[frame_ind + a] = plist->list[i].id;
                            b = frame_ind + a;
                        }
                        frame_ind = b;
                        fprintf(output, "time %dms: Placed process %c:\n", 
                            sim_time, plist->list[i].id);
                        fflush(stdout);
                        print_frames(output, frames);
                        //frame_ind = 0;
                        
                    }
                    
                }
                //finish placing process
            }
        }
        sim_time++;
    }
    
    
    sim_time--;
    
    fprintf(output, "time %dms: Simulator ended (Contiguous -- Best-Fit)\n\n",sim_time);
    fflush(stdout);
    
    
    
    free_list(*plist);
    free(plist);
}