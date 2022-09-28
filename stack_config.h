typedef int Elem_t;

const unsigned long long LEFT_CANARY       = 0xBAADF00D;
const unsigned long long RIGHT_CANARY      = 0xF00DBAAD;
const unsigned long long DATA_LEFT_CANARY  = 0xDEADBEEF;
const unsigned long long DATA_RIGHT_CANARY = 0xBEEFDEAD;
const size_t             SIZE_DESTRUCT     = 0xDEAD;
const size_t             CAPACITY_DESTRUCT = 0xC0FE;
const Elem_t             POISON            = 1488;
const int                DATA_DESTRUCT     = 0xDED0;
const size_t             MAX_STR_SIZE      = 40;
const size_t             CANARY_DESTRUCT   = 0xD1D0;

