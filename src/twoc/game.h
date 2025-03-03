#ifndef GAME_H
#define GAME_H

#include "executable.h"
namespace Twoc
{
class Game
{
private:
    std::string m_root_dir;
    Twoc::Executable *m_executable;

public:
    Game(Twoc::Executable *executable, std::string const& root_dir);
};

Game::Game(Twoc::Executable *executable, std::string const& root_dir) : m_root_dir(root_dir), m_executable(executable)
{

}


}
#endif // GAME_H
