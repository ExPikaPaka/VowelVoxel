#include "LRGB.h"

namespace ent {
    namespace color {
        f64 fn(f64 c) {
            const double abs = std::abs(c);
            if (abs < 0.04045) {
                return c / 12.92;
            }
            return (std::signbit(c) ? -1 : 1) * std::pow((abs + 0.055) / 1.055, 2.4);

            
        }

        RGB RGBtoLRGB(const RGB& rgb) {
            RGB lrgb;

            lrgb.r = fn(rgb.r);
            lrgb.g = fn(rgb.g);
            lrgb.b = fn(rgb.b);
            if (rgb.alpha != 0.0) {
                lrgb.alpha = rgb.alpha;
            }
            return lrgb;
        }

        RGB RGBtoLRGB(f64 r, f64 g, f64 b, f64 a) {
            return RGBtoLRGB({ r, g, b, a });
        }

        RGB LRGBtoRGB(const RGB& lrgb) {
            RGB rgb;

            rgb.r = fn(lrgb.r);
            rgb.g = fn(lrgb.g);
            rgb.b = fn(lrgb.b);
            if (lrgb.alpha != 0.0) {
                rgb.alpha = lrgb.alpha;
            }
            return rgb;
        }

        RGB LRGBtoRGB(f64 r, f64 g, f64 b, f64 a) {
            return LRGBtoRGB({ r, g, b, a });
        }
    }
}
