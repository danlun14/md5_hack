#include "md5/md5.h"
#include <math.h>
#include <mpi.h>
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
    fprintf(stderr, "\nUsage command line arguments:\n");
    fprintf(stderr, "\t./md5_hack <md5_hash> <alphabet> <string_lenght>\n");
    fprintf(stderr, "\t<md5_hash> contains 32 symbols [a-f0-9]\n\n");
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
                return NULL;
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
        return NULL;
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

struct programm_stat md5Hack(const char* alphabet, const int word_len, int lb, long ub, uint8_t* hash_exp)
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
    MPI_Init(&argc, &argv);

    double ttotal = -MPI_Wtime();

    int rank;
    int commsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    int string_size;
    char* alphabet;
    int alphabet_size;
    uint8_t* hash;
    // Broadcast command line arguments
    if (rank == 0)
    {
        if (argc != 4)
        {
            helpMsg();
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        alphabet = argv[2];
        alphabet_size = strlen(alphabet);
        string_size = atoi(argv[3]);
        hash = hashStoh(argv[1]);
        if (!hash)
        {
            helpMsg();
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        print_hash(hash);
        MPI_Bcast(&alphabet_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(alphabet, alphabet_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        MPI_Bcast(&string_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Bcast(hash, 16, MPI_UINT8_T, 0, MPI_COMM_WORLD);
    }
    else
    {
        char* alp;
        int alp_size;
        int str_size;
        uint8_t* hh = malloc(16 * sizeof(uint8_t));

        MPI_Bcast(&alp_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        alp = malloc(alp_size * sizeof(char) + 1);
        MPI_Bcast(alp, alp_size, MPI_CHAR, 0, MPI_COMM_WORLD);
        alp[alp_size] = '\0';
        MPI_Bcast(&str_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(hh, 16, MPI_UINT8_T, 0, MPI_COMM_WORLD);

        alphabet = alp;
        string_size = str_size;
        hash = hh;
    }

    // long ub = powl(strlen(alphabet), string_size) - 1;
    // md5Hack(alphabet, string_size, 0, ub, hash);
    ttotal += MPI_Wtime();
    printf("On proc %d (Time - %lf): alp - %s; word lenght - %d; hash: ", rank, ttotal, alphabet, string_size);
    print_hash(hash);

    free(hash);
    if (rank != 0)
    {
        free(alphabet);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}