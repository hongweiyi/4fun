#include <fstream>
#include <sstream>
#include <iostream>

#include "merge_sort.h"
#include "file_utils.h"

using std::ifstream;
using std::ofstream;
using std::ostringstream;

using std::make_pair;
using std::sort;

using std::cout;
using std::cerr;
using std::endl;

MergeSorter::MergeSorter()
{
    m_iTotalInputLine = 0;
}

MergeSorter::~MergeSorter()
{
}

int MergeSorter::MergeAndSort(const string &szInputDir, const string &szOutput, int iHashNum)
{
    cout << "init..." << endl;
    this->Init(szInputDir);

    cout << "begin split files..." << endl;
    this->HashFile(szInputDir, iHashNum);

    cout << "combine splited files and sort..." << endl;
    this->CombineSort(iHashNum);

    cout << "merge all sorted files..." << endl;
    this->MergeSort(szOutput);

    cout << "done..." << endl;

    return 0;
}

/*
 * get all input file */
int MergeSorter::Init(const string &szInputDir)
{
    GetFiles(szInputDir, m_vsInputFile, "");
    return 0;
}

/*
 * hash all input files to
 * it's hash bucket accordingly */
int MergeSorter::HashFile(const string &szInputDir, int iHashNum)
{
    // init hash files
    vector<ofstream *> vfTmpFile;
    for (int i = 0; i < iHashNum; ++i)
    {
        ostringstream os;
        os << szInputDir << "/" << i << ".tmp";

        ofstream *ofs = new ofstream(os.str().c_str());
        vfTmpFile.push_back(ofs);

        m_vsTmpFile.push_back(os.str());
    }

    // read all input file line by line
    // calculate line's hash_code
    // flush lines to file accordingly
    vector<string>::iterator viFileName;
    for (viFileName = m_vsInputFile.begin(); viFileName < m_vsInputFile.end(); ++viFileName)
    {
        ifstream ifs(viFileName->c_str());
        if (!ifs.good())
        {
            cerr << "File read wrong: " << *viFileName << endl;
            return -1;
        }

        string sLine;
        while (getline(ifs, sLine))
        {
            uint64_t iHashCode = GenHashCode(sLine.c_str());
            *vfTmpFile[iHashCode % iHashNum] << sLine << "\n";
            ++ m_iTotalInputLine;
        }

        ifs.close();
    }

    // destory hash files' FD
    vector<ofstream *>::iterator viTmpFile;
    for (viTmpFile = vfTmpFile.begin(); viTmpFile < vfTmpFile.end(); ++ viTmpFile)
    {
        (*viTmpFile)->close();
        delete (*viTmpFile);
    }

    return 0;
}

/*
 * read all hashed files
 * combine repeated lines
 * sort it by line's frequency */
int MergeSorter::CombineSort(int iFileNum)
{
    vsIter iFileName;
    for (iFileName = m_vsTmpFile.begin(); iFileName < m_vsTmpFile.end(); ++iFileName)
    {
        CombineSortSingleFile(*iFileName, m_iTotalInputLine/(iFileNum*2));
    }
    return 0;
}

/*
 * combine one hashed one
 * read lines to memory map
 * sort it by frequency and dump it */
int MergeSorter::CombineSortSingleFile(const string &szPath, int iReserve)
{
    ifstream ifs(szPath.c_str());

    if (!ifs.good())
    {
        cerr << szPath << " file read error!" << endl;
        return -1;
    }

    m_umCombineTable.rehash(iReserve);

    // combine elements
    string sLine;
    while (getline(ifs, sLine))
    {
        umsIter it = m_umCombineTable.find(sLine);
        if (it == m_umCombineTable.end())
            m_umCombineTable.insert(make_pair<string, uint32_t>(sLine, 1));
        else
            it->second += 1;
    }

    // sort elements and dump
    string sSortedPath = szPath;
    sSortedPath.append(".sorted");
    m_vsTmpSortedFile.push_back(sSortedPath);

    ofstream ofs(sSortedPath.c_str());

    vector<Pair> vSort(m_umCombineTable.begin(), m_umCombineTable.end());
    sort(vSort.begin(), vSort.end(), PairCmp());

     vpIter it;
     for (it = vSort.begin(); it < vSort.end(); ++it)
     {
         ofs << it->first << "\t" << it->second << "\n";
     }

     m_umCombineTable.clear();
     remove(szPath.c_str());

     return 0;
}

/*
 * merge all sorted files
 * sort it by frequency */
int MergeSorter::MergeSort(const string &szOutput)
{
    vector<ifstream *> vTmpIfs;
    vsIter it;
    for (it = m_vsTmpSortedFile.begin(); it != m_vsTmpSortedFile.end(); ++it)
    {
        ifstream *ifs = new ifstream((*it).c_str());
        if (!ifs->good())
        {
            cerr << *it << " : is wrong!" << endl;
            return -1;
        }
        vTmpIfs.push_back(ifs);
    }

    int iFileNum = vTmpIfs.size();
    bool *bHasNextArray = new bool[iFileNum]();

    string sLine;
    Query  query;
    int    iFinishedNum = 0;

    // init prioiry_queue
    for (int i = 0; i < iFileNum; ++i)
    {
        if (getline(*vTmpIfs[i], sLine))
            bHasNextArray[i] = true;
        else
        {
            bHasNextArray[i] = false;
            iFinishedNum += 1;
            vTmpIfs[i]->close();
            delete vTmpIfs[i];
            continue;
        }
        ParseString(sLine, query);
        query.num = i;
        m_pqTop.push(query);
    }

    // start merge files
    // dump it line by line
    ofstream ofs(szOutput.c_str());
    while (iFinishedNum != iFileNum)
    {
        Query q = m_pqTop.top();
        m_pqTop.pop();
        ofs << q.query << "\t" << q.count << endl;
        if (bHasNextArray[q.num])
        {
            if (getline(*vTmpIfs[q.num], sLine))
                bHasNextArray[q.num] = true;
            else
            {
                bHasNextArray[q.num] = false;
                iFinishedNum += 1;
                vTmpIfs[q.num]->close();
                delete vTmpIfs[q.num];
                continue;
            }
            ParseString(sLine, query);
            query.num = q.num;
            m_pqTop.push(query);
        }
    }

    // clean tmp files
    for (it = m_vsTmpSortedFile.begin(); it != m_vsTmpSortedFile.end(); ++it)
    {
        remove((*it).c_str());
    }

    delete []bHasNextArray;
    return 0;
}

/*
 * parse string to Query struct */
int MergeSorter::ParseString(const string &str, Query &query)
{
    size_t split = str.rfind("\t");
    if (split != string::npos)
    {
        string   q = str.substr(0, split);
        uint32_t c = atoi(str.substr(split + 1, str.size() - split).c_str());
        query.query = q;
        query.count = c;
    }
    else
    {
        cerr << str << ": query format wrong!" << endl;
        return -1;
    }

    return 0;
}

