#include "merge_sort.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("\n");
        printf("Merge Sort v1.0 build %s %s \n", __DATE__, __TIME__);
        printf("Usage: merge_sort input output [hash_num]\n\n");
        printf("  input:\tinput directory of data\n");
        printf("  output:\toutput file\n");
        printf("  hash_num:\tnum of tmp files\n");
        printf("\n");
        return -1;
    }
    MergeSorter cs;
    if (argc == 3)
        cs.MergeAndSort(argv[1], argv[2]);
    else
        cs.MergeAndSort(argv[1], argv[2], atoi(argv[3]));

    return 0;
}
