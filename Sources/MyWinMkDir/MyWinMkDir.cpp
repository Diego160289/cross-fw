#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "No dir name" << std::endl;
    return 1;
  }
  std::vector<std::string> Dirs;
  std::stringstream Path;
  Path << argv[1];
  for (std::string s ; std::getline(Path, s, '/') ; Dirs.push_back(s));
  std::string CurPath;
  for (std::vector<std::string>::const_iterator i = Dirs.begin() ; i != Dirs.end() ; ++i)
  {
    if ((*i) == ".")
      continue;
    CurPath += *i;
    if (!::CreateDirectoryA(CurPath.c_str(), 0) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
      std::cerr << "Can't create dir" << std::endl;
      return 1;
    }
    CurPath += "/";
  }
  return 0;
}
