#include <stdio.h>
#include <stdlib.h>

void help_msg()
{
    printf("\nUsage command line arguments (without \"):\n");
    printf("\t./md5_hack <md5_hash> <alphabet> <string_lenght>\n\n");
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        help_msg();
        return 0;
    }
    int string_size = atoi(argv[3]);
    const char *alphabet = argv[2];
}