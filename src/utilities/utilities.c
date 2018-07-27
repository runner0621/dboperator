#include "utilities.h"


char* UtilStristr (const char * str, const char * subStr)
{
    char *cp = (char *) str;
    char *s1, *s2;

    if (!*subStr)
    {
        return((char *)str);
    }

    while (*cp)
    {
        s1 = cp;
        s2 = (char *) subStr;

        while ( *s1 && *s2 && (!(*s1-*s2) || !(*s1-*s2-32) || !(*s1-*s2+32)) )
        {
            s1++, s2++;
        }

        if (!*s2)
        {
            return(cp);
        }

        cp++;
    }

    return(NULL);
}


