#include "file_header.h"

namespace Twoc::Dat {

ITEM_DEFINE(FileHeaderRaw, FILE_HEADER_ITEM)

bool FileHeaderRaw::validate()
{
    return true;
}

}
