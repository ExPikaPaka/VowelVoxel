#pragma once
#include <math.h>
#include <algorithm>
namespace ent {
    namespace color {
        double normalizeHue(double h);

        struct RGB {
            double r;
            double g;
            double b;
            double alpha;
        };
    }
}

