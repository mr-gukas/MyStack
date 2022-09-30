#include "stack.h"

void print(int param)
{
    fprintf(logFile, "%d", param);
}

void print(char param)
{
    fprintf(logFile, "%c", param);
}

void print(char* param)
{
    fprintf(logFile, "%s", param);
}

void print(double param)
{
    fprintf(logFile, "%lf", param);
}

Elem_t* StackResize(Stack_t* stk, CapacityMode capMode)
{
    ASSERT_OK(stk);

    size_t updCapacity = 0;
    
    switch(capMode)
    {
        case UP: updCapacity   = (stk->capacity == 0) ? 10 : stk->capacity * 2; break;
        case DOWN: updCapacity = stk->capacity / 2; break;
        default: stk->status  |= STACK_BAD_RESIZE; StackDump(stk); return NULL;
    }
    
    char* dataptr = NULL;

    if (stk->capacity == 0)
    {
        dataptr = (char*) calloc(1, 2 * sizeof(unsigned long long) + updCapacity * sizeof(Elem_t));
    }

    else
    {
        dataptr = (char*) realloc((char*) stk->data - sizeof(unsigned long long), 2 * sizeof(unsigned long long) + updCapacity * sizeof(Elem_t));
    }

    if (dataptr == NULL)
    {
        stk->status |= CAN_NOT_ALLOCATE_MEMORY;
        StackDump(stk);
        
        return NULL;
    }

    stk->data            = (Elem_t*) (dataptr + sizeof(unsigned long long));
    stk->dataLeftCanary  = (unsigned long long*)  dataptr;
    stk->dataRightCanary = (unsigned long long*) (dataptr + sizeof(unsigned long long) + updCapacity * sizeof(Elem_t));
    
    *(stk->dataLeftCanary)  = DATA_LEFT_CANARY;
    *(stk->dataRightCanary) = DATA_RIGHT_CANARY; 
    
    stk->capacity = updCapacity;
    
    for (size_t index = stk->size; index < stk->capacity; ++index)
    {
        stk->data[index] = POISON;
    }
    
    stk->stackHash = SetStackHash(stk);
    stk->dataHash  = SetStackDataHash(stk);

    ASSERT_OK(stk);

    return stk->data;
}

Stack_t* StackCtorFunc(Stack_t* stk, size_t capacity, const char name[MAX_STR_SIZE], size_t line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE])
{  
    if (stk == NULL) return NULL;
    
    (stk->info).stackName = name;         
    (stk->info).stackFunc = func;
    (stk->info).stackFile = file;            
    (stk->info).stackLine = line;
    stk->leftCanary       = LEFT_CANARY;
    stk->rightCanary      = RIGHT_CANARY;
    
    if (capacity == 0)
    {
        stk->size        = 0;
        stk->capacity    = capacity;
        stk->status      = STACK_IS_EMPTY;
        stk->stackHash   = SetStackHash(stk);
        stk->dataHash    = 0;
        
        return stk;
    }
    
    capacity = (capacity % 10 != 0) ? ((capacity / 10) + 1) * 10 : capacity;

    char* dataptr = (char*) calloc(1, 2 * sizeof(unsigned long long) + capacity * sizeof(Elem_t));

    if (dataptr == NULL)
    {
        stk->status = CAN_NOT_ALLOCATE_MEMORY;  
        
        return stk;
    }

    stk->dataLeftCanary  = (unsigned long long*)  dataptr;
    stk->dataRightCanary = (unsigned long long*) (dataptr + sizeof(unsigned long long) + capacity * sizeof(Elem_t));
    stk->data            = (Elem_t*) (dataptr + sizeof(unsigned long long));
    stk->size            = 0;
    stk->capacity        = capacity;
    stk->status          = STACK_STATUS_OK;
    
    *(stk->dataLeftCanary)  = DATA_LEFT_CANARY;
    *(stk->dataRightCanary) = DATA_RIGHT_CANARY; 

    for (size_t index = 0; index < capacity; ++index)
    {
        stk->data[index] = POISON;
    }
    
    stk->stackHash = SetStackHash(stk);
    stk->dataHash  = SetStackDataHash(stk);
    
    ASSERT_OK(stk);

    return stk;
}

StackStatus StackDtor(Stack_t* stk)
{  
    if (stk == NULL)
    {   
        StackDump(stk);
        return STACK_NULL_PTR;
    }
    
    if (StackIsDestructed(stk) == STACK_IS_DESTRUCTED)
    {
        StackDump(stk);
        return STACK_IS_DESTRUCTED;
    }

    free((void*) stk->dataLeftCanary);
    
    stk->data            = (Elem_t *) DATA_DESTRUCT;
    stk->size            = SIZE_DESTRUCT;
    stk->capacity        = CAPACITY_DESTRUCT;
    stk->dataLeftCanary  = (unsigned long long*) CANARY_DESTRUCT;
    stk->dataRightCanary = (unsigned long long*) CANARY_DESTRUCT;
    stk->status          = STACK_IS_DESTRUCTED;
    stk->leftCanary      = CANARY_DESTRUCT;
    stk->rightCanary     = CANARY_DESTRUCT;
    stk->stackHash       = HASH_DESTRUCT;
    stk->dataHash        = HASH_DESTRUCT;

    return  (StackStatus) stk->status;
}

StackStatus StackPush(Stack_t* stk, Elem_t value)
{
    if (stk == NULL)
    {
        StackDump(stk);

        return STACK_NULL_PTR;
    }
    
    ASSERT_OK(stk);

    if (stk->capacity == 0 || stk->size == stk->capacity)
    {
        Elem_t* resizeptr = StackResize(stk, UP);
        
        if (resizeptr == NULL)
        {
            stk->status |= CAN_NOT_ALLOCATE_MEMORY;
            StackDump(stk);

            return (StackStatus) stk->status;
        }
    }

    stk->data[stk->size++] = value;
    
    stk->stackHash = SetStackHash(stk);
    stk->dataHash  = SetStackDataHash(stk);

    ASSERT_OK(stk);

    return STACK_STATUS_OK;
}

Elem_t StackPop(Stack_t* stk)
{
    if (stk == NULL)
    {
        StackDump(stk);

        return POISON;
    }
    
    ASSERT_OK(stk);

    if (stk->size == 0)
    {
        stk->stackHash = SetStackHash(stk);
        StackDump(stk);
        
        return POISON;
    }
    
    Elem_t delElem       = stk->data[--stk->size];
    stk->data[stk->size] = POISON;
    
    stk->stackHash = SetStackHash(stk);
    stk->dataHash  = SetStackDataHash(stk);

    if (stk->capacity > 10 && stk->size <= stk->capacity * 3 / 8) 
    {
        Elem_t* resizeptr = StackResize(stk, DOWN);

        if (resizeptr == NULL)
        {
            stk->status |= CAN_NOT_ALLOCATE_MEMORY;
            StackDump(stk);
            
            return stk->status;
        }
    }
    
    stk->stackHash = SetStackHash(stk);
    stk->dataHash  = SetStackDataHash(stk);

    ASSERT_OK(stk);

    return delElem;
}

void StackDumpFunc(Stack_t* stk, size_t line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE])
{   
    
    fprintf(logFile, "\n---------------------------StackDump---------------------------------------\n");
   
    if (stk->status & STACK_NULL_PTR)
    {
        fprintf(logFile, "Stack's pointer is null\n");
    }

    fprintf(logFile, "Called at %s at %s(%lu)\n", file, func, line);
    fprintf(logFile, "Stack[%p] <%s> at %s at %s(%lu)\n", stk, (stk->info).stackName, (stk->info).stackFunc,
                                                  (stk->info).stackFile, (stk->info).stackLine);
    fprintf(logFile, "Stack status:\n");
    

    StackVerify(stk);
    
    if (stk->status & STACK_DATA_IS_RUINED)
    {
        fprintf(logFile, "\t\t!!!STACK'S DATA IS RUINED!!!\n");
    }
    if (stk->status & STACK_NULL_PTR)
    {
        fprintf(logFile, ">>>Stack's pointer is null\n");
    }
    if (stk->status & STACK_IS_EMPTY) 
    {
        fprintf(logFile, ">>>Stack is empty\n");
    }
    if (stk->status & STACK_IS_DESTRUCTED) 
    {
        fprintf(logFile, ">>>Stack is destructed\n");
    }
    if (stk->status & STACK_UB) 
    {
        fprintf(logFile, ">>>Stack has undefined behavior\n");
    }
    if (stk->status & STACK_BAD_RESIZE ) 
    {
        fprintf(logFile, ">>>Stack has resize problem\n");
    }
    if (stk->status & CAN_NOT_ALLOCATE_MEMORY) 
    {
        fprintf(logFile, ">>>Allocate memory problems\n");
    }
    if (stk->status & STACK_SIZE_MORE_THAN_CAPACITY) 
    {
        fprintf(logFile, ">>>Stack's size more than capacity\n");
    }
    if (stk->status & STACK_DATA_NULL_PTR)
    {
        fprintf(logFile, ">>>Stack data's pointer is null\n");
    }
    if (stk->status &  STACK_LEFT_CANARY_RUINED)
    {
        fprintf(logFile, ">>>Stack's left canary is ruined\n");  
    }
    if (stk->status &  STACK_RIGHT_CANARY_RUINED)
    {
        fprintf(logFile, ">>>Stack's right canary is ruined\n");  
    }
    if (stk->status &   STACK_DATA_LEFT_CANARY_RUINED)
    {
        fprintf(logFile, ">>>Stack data's left canary is ruined\n");  
    }
    if (stk->status &  STACK_DATA_RIGHT_CANARY_RUINED)
    {
        fprintf(logFile, ">>>Stack data's right canary is ruined\n");
    }
    if (stk->status & STACK_HASH_IS_RUINED)
    {
        fprintf(logFile, ">>>Stack's hash is ruined\n");
    }
    if (stk->status & STACK_DATA_HASH_IS_RUINED)
    {
        fprintf(logFile, ">>>Stack data's hash is ruined\n");
    }
    if (!stk->status)
    {
        fprintf(logFile, ">>>Stack is OK\n");
    }
    
    fprintf(logFile, "{\n");
    fprintf(logFile, "    STACK HASH:  %lx    STACK DATA HASH: %lx\n", stk->stackHash, stk->dataHash);
    fprintf(logFile, "    LEFT CANARY: %llx   RIGHT CANARY: %llx\n", stk->leftCanary, stk->rightCanary);
        
    fprintf(logFile, "    data pointer = %p\n", stk->data);
    
    if (StackIsDestructed(stk) == STACK_IS_DESTRUCTED)
    {        
        fprintf(logFile, "    size         = %lx\n", stk->size);
        fprintf(logFile, "    capacity     = %lx\n", stk->capacity);

    }
    else if (!(stk->status & (STACK_DATA_NULL_PTR | STACK_IS_EMPTY))) 
    {
        fprintf(logFile, "    size         = %lu\n", stk->size);
        fprintf(logFile, "    capacity     = %lu\n", stk->capacity);
        fprintf(logFile, "    DATA LEFT CANARY: %llx   DATA RIGHT CANARY: %llx\n", *(stk->dataLeftCanary), *(stk->dataRightCanary));
        fprintf(logFile, "    {\n");
        for (size_t index = 0; index < stk->capacity; ++index)
        {
            fprintf(logFile, "\t");
            if (index < stk->size)
            {
                fprintf(logFile, "*");
            }
            else
            {
                fprintf(logFile, " ");
            }
            fprintf(logFile, "[%lu] = ", index);
            print(stk->data[index]);
            if (index >= stk->size)
            {
               fprintf(logFile, " (POISON)");
            }
            fprintf(logFile, "\n");
        }
        fprintf(logFile, "    }\n");
    }
    fprintf(logFile, "}\n");
    fprintf(logFile, "\n---------------------------------------------------------------------------\n");
}

StackStatus StackIsEmpty(Stack_t* stk)
{
   if (stk == NULL)
   {
       return STACK_NULL_PTR;
   }

   if ( stk->size     == 0 &&
        stk->capacity == 0)
   {   
        return STACK_IS_EMPTY;
   }

   return STACK_UB;
}

StackStatus StackIsDestructed(Stack_t* stk)
{
   if (stk == NULL)
       return STACK_NULL_PTR;

   if (stk->data            == (Elem_t*) DATA_DESTRUCT && 
       stk->capacity        == CAPACITY_DESTRUCT       && 
       stk->size            == SIZE_DESTRUCT           &&
       stk->stackHash       == HASH_DESTRUCT)    
    {
        return STACK_IS_DESTRUCTED;
    }
    
    return STACK_UB;
}
       

int StackVerify(Stack_t* stk)
{
    if (stk == NULL)
        return STACK_NULL_PTR;
    if (StackIsEmpty(stk) == STACK_IS_EMPTY)
        return STACK_IS_EMPTY;
    if (StackIsDestructed(stk) == STACK_IS_DESTRUCTED)
        return STACK_IS_DESTRUCTED;
    
    int status = STACK_STATUS_OK;
    
    if (stk->stackHash != SetStackHash(stk))
    {
        status |= STACK_DATA_IS_RUINED | STACK_HASH_IS_RUINED;
    }
    if ((status & STACK_DATA_IS_RUINED) == 0 && stk->dataHash != SetStackDataHash(stk))
    {
        status |= STACK_DATA_IS_RUINED | STACK_DATA_HASH_IS_RUINED;
    }
    if (stk->size > stk->capacity)
        status |= STACK_SIZE_MORE_THAN_CAPACITY | STACK_DATA_IS_RUINED;
    if (stk->data == NULL && stk->capacity > 0)
        status |= STACK_DATA_NULL_PTR;
    if (stk->leftCanary != LEFT_CANARY)
        status |= STACK_LEFT_CANARY_RUINED | STACK_DATA_IS_RUINED;
    if (stk->rightCanary != RIGHT_CANARY)
        status |= STACK_RIGHT_CANARY_RUINED | STACK_DATA_IS_RUINED;
    if (stk->dataLeftCanary != NULL &&
        *(stk->dataLeftCanary) != DATA_LEFT_CANARY)
    {     
        status |= STACK_DATA_LEFT_CANARY_RUINED | STACK_DATA_IS_RUINED;
    }
    if (stk->dataRightCanary != NULL &&
        *(stk->dataRightCanary) != DATA_RIGHT_CANARY)
    {    
        status |= STACK_DATA_RIGHT_CANARY_RUINED | STACK_DATA_IS_RUINED;
    }

    stk->status    = status;
    stk->stackHash = SetStackHash(stk);

    return status;
}

size_t HashCalculate(char* key, size_t len)
{
    size_t hash = 0, index = 0;

    for (hash = 0, index = 0; index < len; ++index)
    {
        hash += (size_t) key[index];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

size_t SetStackHash(Stack_t* stk)
{
    if (stk == NULL)
    {
        return (size_t) STACK_NULL_PTR;
    }

    size_t oldStackHash = stk->stackHash;
    size_t oldDataHash  = stk->dataHash;
    stk->stackHash      = 0;   
    stk->dataHash       = 0;

    size_t hash = HashCalculate((char*) stk, sizeof(*stk));

    stk->stackHash = oldStackHash;
    stk->dataHash  = oldDataHash;

    return hash;
}

size_t SetStackDataHash(Stack_t* stk)
{
    if (stk == NULL)
    {
        return (size_t) STACK_NULL_PTR;
    }

    size_t hash = 0;
    if (stk->data != NULL && StackIsDestructed(stk) != STACK_IS_DESTRUCTED && stk->stackHash == SetStackHash(stk))
    {
        hash += HashCalculate((char *) stk->data, stk->capacity * sizeof(Elem_t));
    }
    else
    {
        stk->status |= STACK_DATA_IS_RUINED;
        StackDump(stk);
    }
        
    return hash;
}
