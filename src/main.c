#include "md5/md5.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct programm_stat
{
    double t_md5;
    double t_get_word;
    double t_cmp;
};

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

void get_word(const char* alphabet, int word_spec, int word_len, char* word_dest)
{
    int alphabet_size = strlen(alphabet);

    for (int i = word_len - 1; i >= 0; i--)
    {
        word_dest[i] = alphabet[word_spec % alphabet_size];
        word_spec /= alphabet_size;
    }

    word_dest[word_len] = '\0';
}

struct programm_stat
md5Hack(const char* alphabet, const int word_len, int lb, long ub, uint8_t* hash_exp)
{
    struct programm_stat stat;
    clock_t A = 0, B = 0, C = 0;
    char* word = malloc(word_len * sizeof(char));
    int in = 0;
    for (long i = lb; i <= ub; i++)
    {
        A -= clock();
        get_word(alphabet, i, word_len, word);
        A += clock();

        B -= clock();
        uint8_t* hash = md5String(word);
        B += clock();

        C -= clock();
        if (!hashcmp(hash, hash_exp))
        {
            printf("%s\n", word);
        }
        C += clock();
        in = i;

        // debug
        /*if (i % 1000000 == 0)
        {
             printf("%ld-> %ld\n", i, ub);
        }*/
    }
    stat.t_get_word = (double)A / CLOCKS_PER_SEC;
    stat.t_md5 = (double)B / CLOCKS_PER_SEC;
    stat.t_cmp = (double)C / CLOCKS_PER_SEC;
    printf("%d, %lf, %lf, %lf\n", in + 1, stat.t_get_word, stat.t_md5, stat.t_cmp);
    return stat;
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
    // printf("%s\n", alphabet);
    uint8_t* hash = hashStoh(argv[1]);
    long ub = powl(strlen(alphabet), string_size) - 1;
    md5Hack(alphabet, string_size, 0, ub, hash);
    // print_hash(hash);
}