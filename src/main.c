#include "md5/md5.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void helpMsg()
{
    printf("\nUsage command line arguments (without \"):\n");
    printf("\t./md5_hack <md5_hash> <alphabet> <string_lenght>\n");
    printf("\t<md5_hash> contains 32 symbols [a-f0-9]\n\n");
}

// hash string to hex(int)
uint8_t* hashStoh(char* source)
{
    int source_len = strlen(source);
    printf("%d\n", source_len);
    uint8_t* hash;
    if (source_len == 32)
    {
        hash = malloc(16);
        for (int i = 0; i < 32; i++)
        {
            int value = 0;
            if (*(source + i) >= '0' && *(source + i) <= '9')
            {
                value = *(source + i) - '0';
            }
            else if ((*(source + i) >= 'a' && *(source + i) <= 'f'))
            {
                value = 10 + *(source + i) - 'a';
            }
            else
            {
                free(hash);
                exit(0);
            }

            int id = i / 2;
            if (i % 2 == 0)
            {
                hash[id] = value;
            }
            else
            {
                hash[id] = hash[id] << 4;
                hash[id] += value;
            }
        }
    }
    else
    {
        exit(0);
    }
    return hash;
}

void print_hash(uint8_t* p)
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        printf("%02x", p[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        helpMsg();
        return 0;
    }
    int string_size = atoi(argv[3]);
    const char* alphabet = argv[2];
    printf("%s\n", alphabet);
    uint8_t* hash = hashStoh(argv[1]);
    print_hash(hash);
    hash = md5String("Hello, World!");
    print_hash(hash);
}