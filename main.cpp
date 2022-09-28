#include "stack.h"


FILE* logFile = startLog(logFile);

int main(void)
{
    Stack_t stk1    = {};
    StackCtor(&stk1, 0);
    StackDump(&stk1);
    
    StackPush(&stk1, 228);
    StackDump(&stk1);

    for (int i = 0; i <= 24; ++i)
    {
        StackPush(&stk1, i);
    }
    StackDump(&stk1);

    for (int i = 0; i <= 7; i++)
    {
        StackPop(&stk1);
    }
    StackDump(&stk1);
    StackPop(&stk1);
    StackDump(&stk1);

    StackPop(&stk1);
    StackPop(&stk1);
    StackDump(&stk1);

    unsigned long long* ptr = stk1.dataLeftCanary;
    *ptr = 3238423;
    StackDump(&stk1);

    StackDtor(&stk1);
    StackDump(&stk1);
    endLog(logFile);
    
    return 0;
}

