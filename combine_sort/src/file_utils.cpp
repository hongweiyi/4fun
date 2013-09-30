#include <dirent.h>
#include <sys/stat.h>

#include <iostream>

#include "file_utils.h"

using std::cout;
using std::endl;

int GetFiles(const string &input_dir, vector<string> &v_file, const string &option)
{
    struct stat buf;
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(input_dir.c_str())) == NULL)
    {
        cout << "Can not open " << input_dir << endl;
        return -1;
    }

    while (((dirp = readdir(dp)) != NULL))
    {
        if (!strncmp(dirp->d_name, ".", 1))
        {
            continue;
        }

        string ab_path = input_dir;
        ab_path.append("/").append(dirp->d_name);

        if (stat(ab_path.c_str(), &buf) == -1)
        {
            cout << ab_path << " :stat error" << endl;
            continue;
        }

        if (S_ISREG(buf.st_mode))
        {
            if (ab_path.find(option) != string::npos)
                v_file.push_back(ab_path);
        }
    }
    closedir(dp);

    return 0;
}
