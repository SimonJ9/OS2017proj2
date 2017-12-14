#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 512
#define FRAME_SIZE 256
#define error(msg) \
    {perror(msg); exit(EXIT_FAILURE);}


struct process 
{
    char id;
    unsigned int _mem;
    unsigned int t_arrival_1;
    unsigned int t_arrival_2;
    unsigned int t_arrival_3;
    
    unsigned int t_running_1;
    unsigned int t_running_2;
    unsigned int t_running_3;
};

struct process_list
{
    unsigned int _size;
    unsigned int _index;
    
    unsigned int cap;
    struct process* list;
};

/*process_list helper*/
void initialize(struct process p)
{
    p.id = 0;
    p._mem = 0;
    p.t_arrival_1 = 0;
    p.t_running_1 = 0;
    p.t_arrival_2 = 0;
    p.t_running_2 = 0;
    p.t_arrival_3 = 0;
    p.t_running_3 = 0;
}


void initialize_list(struct process_list p)
{
    p._size = 0;
    p._index = 0;
    p.cap = 1;
    p.list = (struct process*)malloc(sizeof(struct process));
}

void add_process(struct process_list* list, const struct process p)
{
    if(list->_size > list->cap)
    {
        return;
    }
    
    if(list->_size >= list->cap)
    {
        list->cap *= 2;
        list->list = (struct process*)realloc(list->list, 
                        list->cap * sizeof(struct process));
    }
    list->list[list->_size] = p;
    list->_size++;
}

int remove_process(struct process_list* list, char id)
{
    unsigned int i, j;
    for(i = 0; i < list->_size; i++)
    {
        if(list->list[i].id == id)
        {
            for(j = i; j < list->_size - 1; j++)
            {
                list->list[j] = list->list[j + 1];
            }
            list->_size--;
            if(list->_size < list->cap/2)
            {
                list->cap /= 2;
                list->list = (struct process*)realloc(list->list, 
                                list->cap * sizeof(struct process));
            }
            return 1;
        }
    }
    return -1;
}


/*file-io*/
void parse_input(FILE* fp, struct process_list* plist, unsigned int num)
{
    char buffer[BUFFER_SIZE];
    
    
    //fgets(buffer, BUFFER_SIZE, (FILE*)fp);
    //num = atoi(buffer);
    
    num = 0;
    while(fgets(buffer, BUFFER_SIZE, (FILE*)fp) != NULL)
    {
        if(isblank(buffer[0]) ||
            buffer[0] == '#' ||
            buffer[0] == '\n' )
        {
            continue;
        }
        num++;
    }
    rewind(fp);
    plist->list = (struct process*)malloc(num * sizeof(struct process));
    plist->cap = num;
    plist->_size = 0;
    plist->_index = 0;
    
    while(fgets(buffer, BUFFER_SIZE, (FILE*)fp) != NULL)
    {
        if(isblank(buffer[0]) ||
            buffer[0] == '#' ||
            buffer[0] == '\n' )
        {
            continue;
        }
        
        //num++;
        //plist->cap = num;
        
        struct process temp;
        initialize(temp);
        char* s = strtok(buffer, " ");
        temp.id = s[0];
        
        s = strtok(NULL, " ");
        temp._mem = atoi(s);
        
        s = strtok(NULL, "/");
        temp.t_arrival_1 = atoi(s);
        
        s = strtok(NULL, " ");
        temp.t_running_1 = atoi(s);
        
        s = strtok(NULL, "/");
        if(s != NULL)
        {
            temp.t_arrival_2 = atoi(s);
            
            s = strtok(NULL, " ");
            temp.t_running_2 = atoi(s);
            
            s = strtok(NULL, "/");
            if(s != NULL)
            {
                temp.t_arrival_3 = atoi(s);
                
                s = strtok(NULL, "\n");
                temp.t_running_3 = atoi(s);
            }
            else
            {
                temp.t_arrival_3 = 0;
                temp.t_running_3 = 0;
            }
        }
        else
        {
            temp.t_arrival_2 = 0;
            temp.t_running_2 = 0;
        }
        add_process(plist, temp);
    }
    
    return;
}

void print_frames(FILE* fp, char frame[FRAME_SIZE])
{
    unsigned int i;
    for(i = 0; i < 32; i++)
    {
        fprintf(fp, "=");
    }
    for(i = 0; i < FRAME_SIZE; i++)
    {
        if(i%32 == 0)
        {
            fprintf(fp, "\n");
        }
        fprintf(fp, "%c", frame[i]);
    }
    fprintf(fp, "\n");
    for(i = 0; i < 32; i++)
    {
        fprintf(fp, "=");
    }
    fprintf(fp, "\n");
    fflush(NULL);
}


//Function for testing list
void print_list(struct process_list* list)
{
    
    printf("There are in total %d processes\n", list->_size);
    unsigned int i;
    for(i = 0; i < list->_size; i++)
    {
        printf("Process id: %c, mem: %d, ta1: %d, tr1: %d, ta2: %d, tr2: %d, ta3: %d, tr3: %d\n", 
            list->list[i].id, list->list[i]._mem, list->list[i].t_arrival_1, 
            list->list[i].t_running_1, list->list[i].t_arrival_2, 
            list->list[i].t_running_2, list->list[i].t_arrival_3, 
            list->list[i].t_running_3);
    }
}