#include "gore.h"

int main(int argc, char* argv[]) {
    
    gore_ctx gore = gore_init();

    while (gore_run(&gore))
    {
        if (ImGui::Begin("Hello"))
        {

        }
        ImGui::End();
        gore_frame(&gore);
    }

    gore_quit(&gore);

    return 0;
}