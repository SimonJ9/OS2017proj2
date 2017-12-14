// RCS ID: zhangs18
// Name:Shenrui Zhang
// RCS ID: songh4
// Name:Haotian Song
// RCS ID: jiangh5
// Name:Hantian Jiang

int process_exist(char ip[26],char id ){
    unsigned int i;
    int flag = 0;
    // printf("id:%c\n",id);
    // printf("size:%d\n", pl->_size);
    for(i = 0; i < 26; i++){
        if(ip[i] == id){
            flag = 1;
            break;
        }
    }
    // printf("flag:%d\n",flag);

    return flag;
}


int check_num_free_partition(char frame[FRAME_SIZE], unsigned int mem){
    unsigned int i;
    unsigned int counter = 0;
    unsigned int min = FRAME_SIZE;
    unsigned int min_index = FRAME_SIZE;
    unsigned int min_counter;
    min_counter = FRAME_SIZE;
    int reset = 0;

    for(i = 0; i < FRAME_SIZE; i++){
        if(frame[i] == '.'){
            counter ++;
            // printf("counter_in:%d\n",counter );
            // min_index = i;
            reset = 1;
        }else{// next non '.'
            if(reset == 1){
              if(counter >= mem && counter < min){//
                  // printf("counter:%d\n",counter );

                     min_index = i - counter;

                     min = counter;
                     reset = 0;
                     counter = 0;


              }else{//not big enough skip
                  counter = 0;
                  reset = 0;
                //   printf("skip\n" );
                 // continue;s
              }
            }else{

                 continue;
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
int last_time = 0;

void remove_ip(char ip[26], char id){
    unsigned int i;

    for(i = 0; i < 26; i++){
        if(ip[i] == id){
            // flag = 1;
            ip[i] = '.';
            break;
        }
    }

}
void add_ip(char ip[26], char id){
    unsigned int i;

    for(i = 0; i < 26; i++){
        if(ip[i] == '.'){
            // flag = 1;
            ip[i] = id;
            break;
        }
    }

}

void Sim_Best_Fit(struct process_list* pl, FILE* output)
{


    printf("time 0ms: Simulator started (Contiguous -- Best-Fit)\n");
    fflush(stdout);

    int time_t= 0;
    int free_frame = FRAME_SIZE;
    char frame[FRAME_SIZE];
    int z;
    int t_counter = 0;
    char ip[26] ;
    for(z = 0; z < pl->cap ; z++){
        if(pl->list[z].t_running_1 != 0){
            t_counter++;
        }
        if(pl->list[z].t_running_2 != 0){
            t_counter++;
        }
        if(pl->list[z].t_running_3 != 0){
            t_counter++;
        }
    }
    // printf("coount%d",t_counter);
    for(z = 0; z < 26; z++){
        ip[z] = '.';
    }
    for(z = 0; z < FRAME_SIZE; z++)
    {
        frame[z] = '.';
    }
    while( t_counter > 0){
      int i;
      for(i = 0; i < pl->_size; i++){
        if (process_exist(ip,pl->list[i].id) == 1){// process already inside
            //check remove_process
            if((pl->list[i].t_running_1 +  pl->list[i].t_arrival_1 == time_t) ||
            (pl->list[i].t_running_2 +  pl->list[i].t_arrival_2 == time_t)||
            (pl->list[i].t_running_3 +  pl->list[i].t_arrival_3 == time_t)){
              int j =0;
            //   print_list(p_list);
              remove_ip(ip, pl->list[i].id);
              t_counter--;
              // printf("c%d\n",t_counter);
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
              printf("time %dms: Process %c removed:\n",time_t + frame_time,pl->list[i].id  );
              fflush(stdout);
              last_time = time_t + frame_time;
              print_frames(stdout,frame);
            }

        }else{
            //check add_process

            if(pl->list[i].t_arrival_1 == time_t ||
            (pl->list[i].t_arrival_2 == time_t && pl->list[i].t_running_2 != 0)||
            (pl->list[i].t_arrival_3 == time_t && pl->list[i].t_running_3 != 0)){

                printf("time %dms: Process %c arrived (requires %d frames)\n",time_t + frame_time, pl->list[i].id,pl->list[i]._mem);
                fflush(stdout);
                if (free_frame >= pl->list[i]._mem){//
                    unsigned int index;
                    index = check_num_free_partition(frame,pl->list[i]._mem);
                    // printf("index:%d\n",index );

                    if(index == FRAME_SIZE){//defragmatation
                        printf("time %dms: Cannot place process %c -- starting defragmentation\n",time_t + frame_time,pl->list[i].id );
                        fflush(stdout);
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
                            printf("time %dms: Defragmentation complete (moved %d frames: ",time_t + frame_time +remove_frame_num,remove_frame_num);
                            fflush(stdout);
                            int temp = 0;
                            while(temp < z-1){
                                printf("%c, ",letter[temp]);
                                temp++;
                            }
                            printf("%c)\n",letter[z-1]);
                            fflush(stdout);
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
                            printf("time %dms: Placed process %c:\n",time_t + frame_time,pl->list[i].id  );
                            fflush(stdout);
                            add_ip(ip,pl->list[i].id);
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

                            int t;
                            z = 0;
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
                            printf("time %dms: Defragmentation complete (moved %d frames: ",time_t + frame_time,remove_frame_num);
                            fflush(stdout);
                            int temp = 0;
                            while(temp < z-1){
                                printf("%c, ",letter[temp]);
                                temp++;
                            }
                            // int t;
                            printf("%c)\n",letter[z-1]);
                            fflush(stdout);
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
                            printf("time %dms: Placed process %c:\n",time_t + frame_time,pl->list[i].id  );
                            fflush(stdout);
                            add_ip(ip,pl->list[i].id);
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

                        free_frame = free_frame - pl->list[i]._mem;

                        printf("time %dms: Placed process %c:\n",time_t + frame_time,pl->list[i].id  );
                        fflush(stdout);
                        print_frames(stdout,frame);
                        add_ip(ip,pl->list[i].id);
                    }

                }else{//skip processes
                        printf("time %dms: Cannot place process %c -- skipped!\n",time_t + frame_time,pl->list[i].id  );
                        t_counter--;
                        fflush(stdout);
                        print_frames(stdout,frame);
                }

            }else{

            }

        }




      }
      time_t++;
    }
    printf("time %dms: Simulator ended (Contiguous -- Best-Fit)\n",last_time);
    fflush(stdout);
    printf("\n");
    fflush(stdout);
}
