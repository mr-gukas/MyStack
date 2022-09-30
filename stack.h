#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "LOG.h"
#include <stdlib.h>
#include <malloc.h>
#include "stack_config.h"
#include <assert.h>

#define ASSERT(condition)                                       \
    if (!(condition)){                                           \
        fprintf(stderr, "Error in %s:\n"                          \
                        "FILE: %s\n"                               \
                        "LINE: %d\n"                                \
                        "FUNCTION: %s\n",                            \
               #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
        abort();}

#define StackCtor(stk, capacity) \
    StackCtorFunc(stk, capacity, #stk, __LINE__, __FILE__, __PRETTY_FUNCTION__) 
    
#define StackDump(stk) \
    StackDumpFunc(stk, __LINE__, __FILE__, __PRETTY_FUNCTION__)

#define ASSERT_OK(stk)                                        \
    if (StackVerify(stk) != STACK_STATUS_OK &&  (StackIsEmpty(stk) != STACK_IS_EMPTY)) \
    {                                                           \
        StackDump(stk);                                          \
        ASSERT(0 && "Crashed stack")                              \
    }                                                              \

#define StackHash(stk) \
    HashCalculate((char*) stk, sizeof(Stack_t))

#define StackDataHash(stk) \
    HashCalculate((char*) stk->data, stk->capacity  * sizeof(Elem_t))

void print(int param);
void print(char param);
void print(char* param);
void print(double param);

struct StackInfo_t
{
    const char* stackName;
    const char* stackFunc;
    const char* stackFile;
    size_t      stackLine;
};

enum StackStatus
{
    STACK_STATUS_OK                = 0 << 0,
    STACK_NULL_PTR                 = 1 << 0,
    STACK_CAPACITY_NULL            = 1 << 1,
    STACK_DATA_NULL_PTR            = 1 << 2,
    STACK_LEFT_CANARY_RUINED       = 1 << 3,
    STACK_RIGHT_CANARY_RUINED      = 1 << 4, 
    STACK_DATA_LEFT_CANARY_RUINED  = 1 << 5,
    STACK_DATA_RIGHT_CANARY_RUINED = 1 << 6,
    STACK_SIZE_MORE_THAN_CAPACITY  = 1 << 7,
    CAN_NOT_ALLOCATE_MEMORY        = 1 << 8,
    STACK_IS_EMPTY                 = 1 << 9,
    STACK_BAD_RESIZE               = 1 << 10,
    STACK_UB                       = 1 << 11,
    STACK_IS_DESTRUCTED            = 1 << 12,
    STACK_HASH_IS_RUINED           = 1 << 13,
    STACK_DATA_HASH_IS_RUINED      = 1 << 14,
    STACK_DATA_IS_RUINED           = 1 << 15
};


struct Stack_t
{   
    unsigned long long  leftCanary;
    Elem_t*             data; 
    unsigned long long* dataLeftCanary; // can be moved to function
    unsigned long long* dataRightCanary;
    size_t              size;
    size_t              capacity;
    struct StackInfo_t  info;
    int                 status;
    size_t              stackHash;
    size_t              dataHash;
    unsigned long long  rightCanary;

};

enum CapacityMode {UP, DOWN};

Elem_t*     StackResize(Stack_t* stk, CapacityMode capMode);

Stack_t*    StackCtorFunc(Stack_t* stk, size_t capacity, const char name[MAX_STR_SIZE], size_t line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE]); // _ in the end

StackStatus StackDtor(Stack_t* stk);

StackStatus StackPush(Stack_t* stk, Elem_t value);

Elem_t      StackPop(Stack_t* stk);

void        StackDumpFunc(Stack_t* stk, size_t line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE]);

int         StackVerify(Stack_t *stk);

StackStatus StackIsDestructed(Stack_t *stk);

StackStatus StackIsEmpty(Stack_t *stk);

void        fillPoison(Stack_t* stk);

size_t      HashCalculate(char* key, size_t len);

StackStatus HashCheck(Stack_t* stk);

StackStatus SetHash(Stack_t* stk);

size_t      SetStackHash(Stack_t* stk);
size_t SetStackDataHash(Stack_t* stk);
