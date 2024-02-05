#include "Common.h"
namespace ent {
    namespace color {
        double normalizeHue(double h) {
            h = fmod(h, 360.0);
            if (h < 0) {
                h += 360.0;
            }
            return h;
        }
    }
}

