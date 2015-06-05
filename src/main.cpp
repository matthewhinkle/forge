#include <iostream>

#include "common/logger.h"
#include "math/vec.h"
#include "math/mat.h"

#include "system/x-window-manager.h"

#include <unistd.h>

using namespace std;

int main() {
    forge::Logger l("main");

    struct forge::wm wm;
    forge::init_wm(&wm, forge::WM_OPTION_DEFAULT);

    struct forge::window window;
    forge::wm_init_window(&wm, &window, "Forge", 0, 0, 200, 200, forge::WINDOW_OPTION_DEFAULT);

    forge::window_set_name(&window, "Forge 2");
    forge::window_set_bounds(&window, 100, 100, 300, 300);

    for(;;);

    return 0;
}
