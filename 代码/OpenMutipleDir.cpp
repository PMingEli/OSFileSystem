#include "header.h"

void OpenMutipleDir(string route)
{
    vector<string> directory;
    directory = split(route, "/");
    int n = directory.size();
    if (strcmp(directory[0].c_str(), "root") == 0)
    {
        while (dir_pointer > 1)
        {
            OpenDir("..");
        }
        for (int i = 1; i < n; i++)
        {
            OpenDir(directory[i].c_str());
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            OpenDir(directory[i].c_str());
        }
    }
}