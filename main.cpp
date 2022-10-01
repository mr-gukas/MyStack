#include "stack.h"


FILE* LogFile = startLog(LogFile);

int main(void)
{
    Stack_t stk1    = {};
    StackCtor(&stk1, 9);
    StackDump(&stk1);
    
    StackPush(&stk1, 228);
    StackDump(&stk1);

    for (int i = 0; i <= 24; ++i)
    {
        StackPush(&stk1, i);
    }
    StackDump(&stk1);

    for (int i = 0; i <= 9; i++)
    {
        StackPop(&stk1);
    }
    StackDump(&stk1);
    StackPop(&stk1);
    StackDump(&stk1);

    StackPop(&stk1);
    StackDump(&stk1);

    stk1.rightCanary = 2322;
    StackDump(&stk1);
    
    stk1.data[0] = 1010;
    StackDump(&stk1);

    StackDtor(&stk1);
    StackDump(&stk1);
    
    StackPush(&stk1, 32);
    StackDump(&stk1);

    StackDtor(&stk1);
    StackDump(&stk1);
 
    StackPop(&stk1);
    StackDump(&stk1);

    StackPush(&stk1, 32);
    StackDump(&stk1);

    endLog(LogFile);
    
    return 0;
}

