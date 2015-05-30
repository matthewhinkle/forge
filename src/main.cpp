#include <iostream>

#include "common/logger.h"
#include "math/vec.h"
#include "math/mat.h"

using namespace std;

int main() {
    forge::Logger l("main");
    l.trace("Hello, %s!\n", "matt");

    forge::vec2 v(1.0f, 5.5f);
    forge::vec2 v2(2.0f, 5.5f);

    forge::vec2 r = v + v2 - v;

    l.fatal("Result = { %f, %f }\n", r.x, r.y);

    forge::mat4 m;
    l.error("mat4 = { %f }", m(0, 3));

    return 0;
}