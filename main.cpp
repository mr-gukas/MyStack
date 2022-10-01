#include "stack.h"

FILE* LogFile = startLog(LogFile);

int main(void)
{
    Stack_t stk1    = {};
    StackCtor(&stk1, 8);
    StackDump(&stk1);
    StackPush(&stk1, 228);
    StackDump(&stk1);

    for (int index = 0; index < 30; ++index)
        StackPush(&stk1, index);
    StackDump(&stk1);
    *((char*) stk1.data - sizeof(unsigned long long)) = 0;

    stk1.data = (Elem_t*) 0x10;
    StackDtor(&stk1);
    StackDump(&stk1);
    
    endLog(LogFile);
    
    return 0;
}

