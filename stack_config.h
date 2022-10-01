#define CANARY_GUARD 01
#define HASH_GUARD   02

typedef int                Elem_t;
typedef unsigned long long Canary_t;
extern FILE* LogFile;

#if CANARY_GUARD
const Canary_t LEFT_CANARY       = 0xBAADF00DBAADF00D;
const Canary_t RIGHT_CANARY      = 0xF00DBAADF00DBAAD;
const Canary_t DATA_LEFT_CANARY  = 0xDEADBEEFDEADBEEF;
const Canary_t DATA_RIGHT_CANARY = 0xBEEFDEADBEEFDEAD;
#endif

#if HASH_GUARD
const size_t   HASH_DESTRUCT     = 0xDEADBABE;
#endif

const size_t   SIZE_DESTRUCT     = 0xDEAD;
const size_t   CAPACITY_DESTRUCT = 0xC0FE;
const Elem_t   POISON            = 31415926;
const int      DATA_DESTRUCT     = 0xDED0;
const size_t   MAX_STR_SIZE      = 40;
const Canary_t CANARY_DESTRUCT   = 0xDEADBABADEADBABA;
const size_t   BASED_CAPACITY    = 10;

