#include "header.h"

bool OpenMutipleDir(string route)
{
    vector<string> directory;
    directory = split(route, "/");
    int n = directory.size();
    for (int i = 0; i < n; i++)
    {
        if (OpenDir(directory[i].c_str()))
        {
            return false;
            break;
        }
    }
    return true;
}