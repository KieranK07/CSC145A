#include "Game.h"

int main() {
    Game rpg;
    if (rpg.Init()) {
        rpg.Run();
    }
    return 0;
}
