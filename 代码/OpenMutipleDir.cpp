#include "header.h"

void OpenMutipleDir(string route)
{
    vector<string> directory;
    directory = split(route, "/");
    int n = directory.size();
    for (int i = 0; i < n; i++)
    {
        OpenDir(directory[i].c_str());
    }
}