#include "directory.h"

bool Directory::cd(const std::filesystem::path &relative_path)
{
    for (auto const& element : relative_path)
    {
        if (element == "..")
        {
            cd_up();
        } else if (element == ".")
        {
            continue;
        } else {
            if (!cd_element(element))
                return false;
        }
    }
    return true;
}
