#include "md5/md5.h"
#include <math.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void help_msg()
{
    fprintf(stderr, "\nUsage command line arguments:\n");
    fprintf(stderr, "\t./md5_hack <md5_hash> <alphabet> <string_lenght>\n");
    fprintf(stderr, "\t<md5_hash> contains 32 symbols [a-f0-9]\n\n");
}

// hash string to hex(int)
uint8_t* hashStoh(char* source)
{
    int source_len = strlen(source);
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

int md5_hack(const char* alphabet, const int word_len, long long lb, long long ub, uint8_t* hash_exp)
{
    char* word = malloc(word_len * sizeof(char));
    int matches = 0;
    for (long long i = lb; i <= ub; i++)
    {
        get_word(alphabet, i, word_len, word);

        uint8_t* hash = md5String(word);
        if (!hashcmp(hash, hash_exp))
        {
            printf("Found a match: [%s] %lld\n", word, i);
            matches += 1;
        }
        free(hash);
    }
    return matches;
}
void get_chunk(long long a, long long b, int commsize, int rank, long long* lb, long long* ub)
{
    /* OpenMP 4.0 spec (Sec. 2.7.1, default schedule for loops)
     * For a team of commsize processes and a sequence of n items, let ceil(n ? commsize) be the integer q
     * that satisfies n = commsize * q - r, with 0 <= r < commsize.
     * Assign q iterations to the first commsize - r procs, and q - 1 iterations to the remaining r processes */
    long n = b - a + 1;
    long q = n / commsize;
    if (n % commsize)
        q++;
    long r = commsize * q - n;
    /* Compute chunk size for the process */
    long chunk = q;
    if (rank >= commsize - r)
        chunk = q - 1;
    *lb = a; /* Determine start item for the process */
    if (rank > 0)
    { /* Count sum of previous chunks */
        if (rank <= commsize - r)
            *lb += q * rank;
        else
            *lb += q * (commsize - r) + (q - 1) * (rank - (commsize - r));
    }
    *ub = *lb + chunk - 1;
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
            help_msg();
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        alphabet = argv[2];
        alphabet_size = strlen(alphabet);
        string_size = atoi(argv[3]);
        hash = hashStoh(argv[1]);
        if (!hash)
        {
            help_msg();
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        MPI_Bcast(&alphabet_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(alphabet, alphabet_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        MPI_Bcast(&string_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Bcast(hash, 16, MPI_UINT8_T, 0, MPI_COMM_WORLD);

        // print_hash(hash);
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

    long long lb, ub;
    long long comb_count = powl(strlen(alphabet), string_size);
    get_chunk(0, comb_count - 1, commsize, rank, &lb, &ub);

    int matches = md5_hack(alphabet, string_size, lb, ub, hash);

    ttotal += MPI_Wtime();

    double Tmax;
    MPI_Reduce(&ttotal, &Tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    // printf("On proc %d %d matches: %lld -> %lld ; time - %lf\n", rank, matches, lb, ub, ttotal);

    free(hash);
    if (rank != 0)
    {
        free(alphabet);
    }
    else
    {
        printf("procs |    time\n%5d     %5.5lf\n", commsize, Tmax);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}