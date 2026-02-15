#include "gore.h"

int main(int argc, char* argv[]) {
    gore::json j {};
    j["dad"] = 2;
    gore::ctx gore = gore::init();

    while (gore::run(&gore))
    {
        if (ImGui::Begin("Hello"))
        {

        }
        ImGui::End();
        gore::frame(&gore);
    }

    gore::quit(&gore);

    return 0;
}