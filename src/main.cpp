#include "raySim.h"

int main(void)
{
    ray_sim::RaySim raySim;

    ray_sim::ApplicationState appState = raySim.Init();

    while (appState == ray_sim::ApplicationState::RUNNING)
    {
        appState = raySim.Update();
    }

    raySim.Shutdown();

    return 0;
}