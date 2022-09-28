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

#define StackCtor(stk, capacity) \
    StackCtorFunc(stk, capacity, #stk+1, __LINE__, __FILE__, __PRETTY_FUNCTION__); 
    
#define StackDump(stk) \
    StackDumpFunc(stk, __LINE__, __FILE__, __PRETTY_FUNCTION__);

#define ASSERT_OK(stk)                                        \
    if (StackVerify(stk) != STACK_STATUS_OK &&  (StackIsEmpty(stk) != STACK_IS_EMPTY)) \
    {                                                           \
        StackDump(stk);                                          \
        assert(0 && "Crashed stack\n");                           \
    }                                                              \

void print(int param);

void print(char param);

void print(char* param);

void print(double param);

struct StackInfo_t
{
    const char* stackName;
    const char* stackFunc;
    const char* stackFile;
    size_t   stackLine;
};

enum StackStatus
{
    STACK_STATUS_OK                = 0     ,
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
    STACK_IS_DESTRUCTED            = 1 << 12
};


struct Stack_t
{   
    unsigned long long leftCanary;
    Elem_t* data; 
    unsigned long long* dataLeftCanary;
    unsigned long long* dataRightCanary;
    size_t size;
    size_t capacity;
    struct StackInfo_t info;
    int status;
    unsigned long long rightCanary;

};

enum CapacityMode {UP, DOWN};

Elem_t* StackResize(Stack_t* stk, CapacityMode capMode);

Stack_t* StackCtorFunc(Stack_t* stk, size_t capacity, const char name[MAX_STR_SIZE], size_t line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE]);

StackStatus StackDtor(Stack_t* stk);

StackStatus StackPush(Stack_t* stk, Elem_t value);

Elem_t StackPop(Stack_t* stk);

void StackDumpFunc(Stack_t* stk, size_t line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE]);

int  StackVerify(Stack_t *stk);

StackStatus StackIsDestructed(Stack_t *stk);

StackStatus StackIsEmpty(Stack_t *stk);

void fillPoison(Stack_t* stk);


