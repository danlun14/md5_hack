#include "md5/md5.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void helpMsg()
{
    printf("\nUsage command line arguments:\n");
    printf("\t./md5_hack <md5_hash> <alphabet> <string_lenght>\n");
    printf("\t<md5_hash> contains 32 symbols [a-f0-9]\n\n");
}

// hash string to hex(int)
uint8_t* hashStoh(char* source)
{
    int source_len = strlen(source);
    // printf("%d\n", source_len);
    uint8_t* hash;
    if (source_len == 32)
    {
        hash = malloc(16 * sizeof(uint8_t));
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
                hash[id] = hash[id] << 4 | value;
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

int hashcmp(uint8_t* hashA, uint8_t* hashB)
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        if (hashA[i] != hashB[i])
        {
            return -1;
        }
    }
    return 0;
}

void get_word(const char* alphabet, long word_spec, int word_len, char* word_dest)
{
    int alphabet_size = strlen(alphabet);

    for (int i = word_len - 1; i >= 0; i--)
    {
        word_dest[i] = alphabet[word_spec % alphabet_size];
        word_spec /= alphabet_size;
    }

    word_dest[word_len] = '\0';
}

int md5Hack(const char* alphabet, const int word_len, int lb, long ub, uint8_t* hash_exp)
{
    char* word = malloc(word_len * sizeof(char));
    int matches = 0;
    for (long i = lb; i <= ub; i++)
    {
        get_word(alphabet, i, word_len, word);
        uint8_t* hash = md5String(word);
        if (!hashcmp(hash, hash_exp))
        {
            printf("%s\n", word);
            matches++;
        }
        free(hash);
    }
    // printf("%d, %lf, %lf, %lf\n", in + 1, stat.t_get_word, stat.t_md5, stat.t_cmp);
    return matches;
}

int main(int argc, char* argv[])
{
    clock_t clockk = -clock();
    if (argc != 4)
    {
        helpMsg();
        return 0;
    }
    int string_size = atoi(argv[3]);
    const char* alphabet = argv[2];
    // printf("%s\n", alphabet);
    uint8_t* hash = hashStoh(argv[1]);
    long ub = powl(strlen(alphabet), string_size) - 1;
    int matches = md5Hack(alphabet, string_size, 0, ub, hash);
    clockk += clock();
    printf("matches - %d_%lf\n", matches, (double)clockk / CLOCKS_PER_SEC);
    free(hash);
    // print_hash(hash);
}