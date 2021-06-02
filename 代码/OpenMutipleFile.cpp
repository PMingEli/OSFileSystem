#include "header.h"

inode *OpenMutipleFile(string route)
{
    vector<string> directory;
    directory = split(route, "/");
    int n = directory.size();
    for (int i = 0; i < n - 1; i++)
    {
        OpenDir(directory[i].c_str());
    }
    return OpenFile(directory[n - 1].c_str());
}