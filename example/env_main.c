#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

    char *VAR_TSK_BEFORE, *VAR_TSK_AFTER = NULL;

    VAR_TSK_BEFORE = getenv("VAR_TSK");
    _putenv("VAR_TSK=Peter");
    VAR_TSK_AFTER = getenv("VAR_TSK");
    printf(" 'Before' %s,  'After' %s\n", VAR_TSK_BEFORE, VAR_TSK_AFTER);
    _putenv("VAR_TSK=");
    printf(" 'Before' %s,  'After' %s\n", VAR_TSK_BEFORE, getenv("VAR_TSK"));

    return EXIT_SUCCESS;
     /* EXIT_FAILURE;*/
}