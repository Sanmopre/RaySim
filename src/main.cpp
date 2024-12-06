#include "raySim.h"

int main(void)
{
    ray_engine::RaySim raySim;

    ray_engine::ApplicationState appState = raySim.Init();

    while (appState == ray_engine::ApplicationState::RUNNING)
    {
        appState = raySim.Update();
    }

    raySim.Shutdown();

    return 0;
}