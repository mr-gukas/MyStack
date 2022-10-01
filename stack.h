#include <stdio.h>
#include "LOG.h"
#include <malloc.h>
#include "stack_config.h"

#if HASH_GUARD || CANARY_GUARD 

    #define ASSERT(condition)                                       \
        if (!(condition)){                                           \
            fprintf(stderr, "Error in %s:\n"                          \
                            "FILE: %s\n"                               \
                            "LINE: %d\n"                                \
                            "FUNCTION: %s\n",                            \
                   #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
            abort();}
    
    #define ASSERT_OK(stk)                                                                \
        if (StackVerify(stk) != STACK_STATUS_OK &&  (StackIsEmpty(stk) != STACK_IS_EMPTY)) \
        {                                                                                   \
            StackDump(stk);                                                                  \
            ASSERT(0 && "Crashed stack")                                                      \
        }                                                                                      
#else
    #define ASSERT(condition) ;
    #define ASSERT_OK(stk)    ;

#endif


#define StackCtor(stk, capacity) \
    StackCtorFunc(stk, capacity, #stk, __LINE__, __FILE__, __PRETTY_FUNCTION__) 
    
#define StackDump(stk) \
    StackDumpFunc(stk, __LINE__, __FILE__, __PRETTY_FUNCTION__)

void print(FILE* file, int param);
void print(FILE* file, char param);
void print(FILE* file, char* param);
void print(FILE* file, double param);

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
    STACK_IS_DESTRUCTED            = 1 << 3,
    STACK_UB                       = 1 << 4, 
    STACK_DATA_IS_RUINED           = 1 << 5,
    STACK_BAD_RESIZE               = 1 << 6,
    STACK_SIZE_MORE_THAN_CAPACITY  = 1 << 7,
    CAN_NOT_ALLOCATE_MEMORY        = 1 << 8,
    STACK_IS_EMPTY                 = 1 << 9,

#if CANARY_GUARD
    STACK_DATA_RIGHT_CANARY_RUINED = 1 << 10,
    STACK_DATA_LEFT_CANARY_RUINED  = 1 << 11,
    STACK_LEFT_CANARY_RUINED       = 1 << 12,
    STACK_RIGHT_CANARY_RUINED      = 1 << 13,
#endif

#if HASH_GUARD
    STACK_DATA_HASH_IS_RUINED      = 1 << 14,
    STACK_HASH_IS_RUINED           = 1 << 15,
#endif

};

struct Stack_t
{   
#if CANARY_GUARD
    Canary_t            leftCanary;
#endif

    Elem_t*             data; 
    size_t              size;
    size_t              capacity;
    struct StackInfo_t  info;
    int                 status;

#if HASH_GUARD
    size_t              stackHash;
    size_t              dataHash;
#endif

#if CANARY_GUARD
    Canary_t            rightCanary;
#endif

};

enum CapacityMode {UP, DOWN};

Elem_t*     StackResize(Stack_t* stk, CapacityMode capMode);
Stack_t*    StackCtorFunc(Stack_t* stk,  size_t     capacity,           const char name[MAX_STR_SIZE], 
                          size_t   line, const char file[MAX_STR_SIZE], const char func[MAX_STR_SIZE]); 
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
size_t      SetStackDataHash(Stack_t* stk);
