/*
 * Some file's utils, now I only provide one function which is
 * traverse directory.
 *
 * Copyright (c) 2012-2016, Wikie <wikie1989 at gmail dot com>
 * All rights reserved.
 *
 * Website: http://hongweiyi.com */

#ifndef WIKIE_FILEUTILS
#define WIKIE_FILEUTILS

#include <string>
#include <vector>

using std::string;
using std::vector;

int GetFiles(const string &input_dir, vector<string> &v_file, const string &option="");

#endif // WIKIE_FILEUTILS
