#include "Game.h"

//dis da big guy
int main() {
    Game rpg;
    if (rpg.Init()) {
        rpg.Run();
    }
    return 0;
}
