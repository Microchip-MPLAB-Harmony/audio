
//#include "app.h"
#include "user.h"

void user_assert_func( char * file, int line, const char * func, char * msg, char * expr) 
{
    SYS_PRINT("ASSERT %s in file %s at line %d (%s)\n",expr, file, line, expr);
    //__builtin_software_breakpoint();
}
