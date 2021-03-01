#define OLC_PGE_APPLICATION
#include "../inc/Engine.h"

int main() {
    Engine ca = Engine();
    if(ca.Construct(600, 500, 2, 2))
        ca.Start();
    return 0;
}
