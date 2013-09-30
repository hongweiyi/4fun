/*
 * Merge sort class, it can merge come string files and sort it
 * by frequency.
 *
 * Copyright (c) 2012-2016, Wikie <wikie1989 at gmail dot com>
 * All rights reserved.
 *
 * Website: http://hongweiyi.com */

#ifndef WIKIE_MERGESORT
#define WIKIE_MEGERSORT

#include <string>
#include <vector>

#include <queue>
#include <tr1/unordered_map>

using std::priority_queue;

using std::tr1::unordered_map;
using std::pair;

using std::string;
using std::vector;

class MergeSorter
{
public:
    MergeSorter();
    virtual ~MergeSorter();

    int MergeAndSort(const string  &szInputDir, const string &szOutput, int iHashNum=13);

protected:
    int Init(const string &szInputDir);
    int HashFile(const string &szInputDir, int iHashNum);
    int CombineSort(int iFileNum);
    int MergeSort(const string &szOutput);

    int CombineSortSingleFile(const string &szPath, int iReserve);

private:
    typedef unordered_map<string, uint32_t>::iterator umsIter;
    typedef pair<string, uint32_t>                    Pair;
    typedef vector<Pair>::iterator                    vpIter;
    typedef vector<string>::iterator                  vsIter;

    struct PairCmp
    {
        bool operator() (const Pair &p1, const Pair &p2)
        {
            return p1.second < p2.second;
        }
    };

    struct Query
    {
        string query;
        uint8_t num;
        uint32_t count;
    };

    struct QueryCmp
    {
        bool operator() (const Query &q1, const Query &q2)
        {
            return q1.count < q2.count;
        }
    };

private:
    inline int ParseString(const string &str, Query &q);

    inline uint64_t GenHashCode(const char *str)
    {
        unsigned int seed = 131;
        unsigned int hash = 0;

        while (*str)
        {
            hash = hash * seed + (*str++);
        }

        return (hash &0x7FFFFFFF);
    }


private:
    vector<string> m_vsInputFile;
    vector<string> m_vsTmpFile;
    vector<string> m_vsTmpSortedFile;

    unordered_map<string, uint32_t> m_umCombineTable;
    priority_queue<Query, vector<Query>, QueryCmp> m_pqTop;

    uint32_t m_iTotalInputLine;
};

#endif // WIKIE_MERGESORT
