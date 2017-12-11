
int process_exist(struct process_list* pl,char id ){
    unsigned int i;
    int flag = 0;
    // printf("id:%c\n",id);
    // printf("size:%d\n", pl->_size);
    for(i = 0; i < pl->_size; i++){
        if(pl->list[i].id == id){
            flag = 1;
            break;
        }
    }
    // printf("flag:%d\n",flag);

    return flag;
}

// int num_free_part(char frame[FRAME_SIZE]){
//
// }

int check_num_free_partition(char frame[FRAME_SIZE], unsigned int mem){
    unsigned int i;
    unsigned int counter = 0;
    unsigned int min = FRAME_SIZE;
    unsigned int min_index = FRAME_SIZE;
    unsigned int min_counter;
    min_counter = FRAME_SIZE;
    // unsigned int ff[10] = {0,0,0,0,0,0,0,0,0,0};
    // int

    // int
    // num_free_part(frame);
    // printf("mem:%d\n",mem);
    for(i = 0; i < FRAME_SIZE; i++){
        if(frame[i] == '.'){
            counter ++;
            // printf("counter_in:%d\n",counter );
            // min_index = i;
        }else{// next non '.'

              if(counter >= mem && counter < min){//
                  // printf("counter:%d\n",counter );
                  min_index = i - counter;
                  // printf("min_index:%d\n",min_index );
                  min_counter = counter;
                  counter = 0;
              }else{//not big enough skip
                  counter = 0;
                //   printf("skip\n" );
                  // continue;s
              }

        }

    }
    // printf("counter==:%d\n",counter);
    if(counter > mem && counter <= min){//edge case
      min_index = i - counter;
      min_counter = counter;
    }
    // printf("min_index_infunc:%d\n",min_index );
    if(min_counter>0){
      min_index++;
      min_index--;
    }
    return min_index;


}

int de_char_exist(char def[24], char id){
    int counter = 0;
    int flag = 0;
    // printf("test3\n" );
    while(counter < 24){
        if(def[counter] == id){
            flag = 1;
            break;
        }
        counter++;
    }
    // printf("test\n" );
    return flag;
}

int frame_time = 0;

void Sim_Best_Fit(struct process_list* pl, FILE* output)
{

    // print_list(pl);
    // printf("%d\n",pl->_size );
    // printf("%d\n",pl->_index);
    // printf("%d\n",pl->cap );

    printf("time 0ms: Simulator started (Contiguous -- Next-Fit)\n");
    struct process_list* p_list = (struct process_list*)malloc(sizeof(struct process_list));
    initialize_list(*p_list);
    p_list->cap = pl->cap;
    p_list->_index = pl->_index;
    p_list->list = (struct process*)realloc(p_list->list, sizeof(struct process) * pl->cap);
    // counter = pl->_size;
    // p_list->cap = 1;
    // printf("===%d, %d\n",p_list->_size, p_list->cap );
    int time = 0;
    int free_frame = FRAME_SIZE;
    // int counter = pl->_size;
    // int flag = 0;
    char frame[FRAME_SIZE];
    int z;
    for(z = 0; z < FRAME_SIZE; z++)
    {
        frame[z] = '.';
    }
    // print_frames(stdout,frame);
    while( time < 3000){
      int i;
    //   printf("time:%d\n",time );
      //go though the pllist for arrival time
      // printf("size:%d\n",pl->_size );
      for(i = 0; i < pl->_size; i++){
        // do remove only when we have process inside p_list
        // do arrival only when not inside p_list
        // printf("size:%d\n",pl->_size );
        // char tid = pl->list[i].id;
        // process_exist(&p_list,tid)
        // printf("size:%d\n",pl->_size );
        if (process_exist(p_list,pl->list[i].id) == 1){// process already inside
            //check remove_process
            if(pl->list[i].t_running_1 +  pl->list[i].t_arrival_1 == time || pl->list[i].t_running_2 +  pl->list[i].t_arrival_2 == time ){
              //check the p_list for remove
              // print_frames(stdout,frame);
              // printf("time:%d\n",time );
              int j =0;
            //   print_list(p_list);
              remove_process(p_list, pl->list[i].id);
              // print_list(p_list);
              free_frame = free_frame + pl->list[i]._mem;
              // printf("free_frame:%d\n",free_frame );
              char id = pl->list[i].id;
              while(j < 256){//remove
                if(frame[j] == id){
                  frame[j] = '.';
                }
                j++;
              }
              printf("time %dms: Process %c removed:\n",time + frame_time,pl->list[i].id  );
              print_frames(stdout,frame);
            }

        }else{
            //check add_process

            if(pl->list[i].t_arrival_1 == time || (pl->list[i].t_arrival_2 == time && pl->list[i].t_running_2 != 0)){

                printf("time %dms: Process %c arrived (requires %d frames)\n",time + frame_time, pl->list[i].id,pl->list[i]._mem);
                if (free_frame >= pl->list[i]._mem){//
                    unsigned int index;
                    index = check_num_free_partition(frame,pl->list[i]._mem);
                    // printf("index:%d\n",index );

                    if(index == FRAME_SIZE){//defragmatation
                        printf("time %dms: Cannot place process %c -- starting defragmentation\n",time + frame_time,pl->list[i].id );
                        int remove_frame_num = 0;
                        char letter[24];
                        int flag = 0;
                        int z = 0;

                        if(frame[0] == '.'){
                            int t;
                            z = 0;
                            for(t = 0; t<FRAME_SIZE; t++){//find all letter need to move
                                if(frame[t] != '.'){
                                    remove_frame_num++;
                                    // printf("test1\n" );
                                    flag = de_char_exist(letter,frame[t]);
                                    // printf("test2\n" );
                                    if(flag == 0){
                                        letter[z] = frame[t];
                                        z++;
                                    }
                                }
                            }
                            printf("time %dms: Defragmentation complete (moved %d frames: ",time + frame_time,remove_frame_num);
                            int temp = 0;
                            while(temp < z-1){
                                printf("%c, ",letter[temp]);
                                temp++;
                            }
                            printf("%c)\n",letter[z-1]);
                            int new_index = 0;
                            for(t = 0; t < z; t++){
                                for(temp = 0; temp < pl->_size; temp++){
                                    if(letter[t] == pl->list[temp].id){
                                        unsigned int o = 0;
                                        while(o < pl->list[temp]._mem){
                                              frame[new_index] = pl->list[temp].id;
                                              new_index ++;
                                              o++;
                                        }
                                    }
                                }
                            }
                            int temp_index = new_index;
                            while(new_index < FRAME_SIZE){
                                frame[new_index] = '.';
                                new_index++;
                            }
                            frame_time = frame_time + remove_frame_num;
                            print_frames(stdout,frame);
                            printf("time %dms: Placed process %c:\n",time + frame_time,pl->list[i].id  );
                            add_process(p_list,pl->list[i]);
                            free_frame = free_frame - pl->list[i]._mem;
                            t = 0;
                            while(t < pl->list[i]._mem){
                                frame[temp_index] = pl->list[i].id;
                                t++;
                                temp_index++;
                            }
                            print_frames(stdout,frame);
                            int ttt = 0;
                            while(ttt < 24){
                                letter[ttt] = 'z';
                               ttt++;
                            }



                        }else{//when frame[0] is not a dot
                            int first_char_index = 0;
                            while(first_char_index < FRAME_SIZE){
                                if(frame[first_char_index] == '.'){
                                   break;
                                }
                                first_char_index++;
                            }
                            // first_char_index++;
                            int t;
                            z = 0;
                            // printf("char%c,%d\n",letter[z],z);
                            // printf("char%c,%d\n",letter[z+1],z+1);
                            for(t = first_char_index; t<FRAME_SIZE; t++){
                                if(frame[t] != '.'){
                                    remove_frame_num++;
                                    // printf("test1\n" );
                                    flag = de_char_exist(letter,frame[t]);
                                    // printf("test2\n" );
                                    if(flag == 0){
                                        letter[z] = frame[t];
                                        z++;
                                    }
                                }
                            }
                            // printf("char0%c,%d\n",letter[0],z);
                            // printf("char1%c,%d\n",letter[1],z+1);
                            printf("time %dms: Defragmentation complete (moved %d frames: ",time + frame_time,remove_frame_num);
                            int temp = 0;
                            while(temp < z-1){
                                printf("%c, ",letter[temp]);
                                temp++;
                            }
                            // int t;
                            printf("%c)\n",letter[z-1]);
                            int new_index = first_char_index;
                            for(t = 0; t < z; t++){
                                for(temp = 0; temp < pl->_size; temp++){
                                    if(letter[t] == pl->list[temp].id){//if id same
                                        unsigned int o = 0;
                                        while(o < pl->list[temp]._mem){//assign char
                                              frame[new_index] = pl->list[temp].id;
                                              new_index ++;
                                              o++;
                                        }
                                    }
                                }
                            }
                            int temp_index = new_index;
                            while(new_index < FRAME_SIZE){
                                frame[new_index] = '.';
                                new_index++;
                            }
                            frame_time = frame_time + remove_frame_num;
                            print_frames(stdout,frame);
                            printf("time %dms: Placed process %c:\n",time + frame_time,pl->list[i].id  );
                            add_process(p_list,pl->list[i]);
                            free_frame = free_frame - pl->list[i]._mem;
                            t = 0;
                            while(t < pl->list[i]._mem){// add the current char process
                                frame[temp_index] = pl->list[i].id;
                                t++;
                                temp_index++;
                            }
                            print_frames(stdout,frame);
                            int ttt = 0;
                            while(ttt < 24){
                                letter[ttt] = 'z';
                               ttt++;
                            }
                        }
                    }else{//add process
                        int c;
                        for(c = 0; c < pl->list[i]._mem; c++){
                            frame[index+c] = pl->list[i].id;

                        }
                        // print_frames(stdout,frame);
                        // printf("time:%d\n",time );

                        free_frame = free_frame - pl->list[i]._mem;
                        // printf("free_frame:%d\n",free_frame );
                        // printf("size:%d\n",pl->_size );
                        // printf("index2:%d\n",index );
                        // print_list(pl);
                        // print_list(p_list);
                        // print_list(p_list);
                        // printf("%d, %d\n",p_list->_size, p_list->cap );
                        // time 0ms: Process A arrived (requires 45 frames)
                        // printf("time %dms: Process %c arrived (requires %d frames)\n",time, pl->list[i].id,pl->list[i]._mem);
                        printf("time %dms: Placed process %c:\n",time + frame_time,pl->list[i].id  );
                        print_frames(stdout,frame);
                        add_process(p_list,pl->list[i]);
                        // print_list(p_list);
                        // printf("index3:%d\n",index );

                    }

                }else{//skip processes
                        printf("time %dms: Cannot place process %c -- skipped!\n",time + frame_time,pl->list[i].id  );
                }

            }else{

            }

        }




      }
      time++;
    }
}
