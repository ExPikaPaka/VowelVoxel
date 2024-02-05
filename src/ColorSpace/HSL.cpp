#include "HSL.h"

namespace ent {
    namespace color {
        RGB HSLtoRGB(const HSL& hsl) {
            double h = normalizeHue(hsl.h);
            double m1 = hsl.l + hsl.s * (hsl.l < 0.5 ? hsl.l : 1 - hsl.l);
            double m2 = m1 - (m1 - hsl.l) * 2 * std::abs(fmod((h / 60), 2.0) - 1);
            RGB res;

            switch (static_cast<int>(h / 60)) {
                case 0:
                    res.r = m1;
                    res.g = m2;
                    res.b = 2 * hsl.l - m1;
                    break;
                case 1:
                    res.r = m2;
                    res.g = m1;
                    res.b = 2 * hsl.l - m1;
                    break;
                case 2:
                    res.r = 2 * hsl.l - m1;
                    res.g = m1;
                    res.b = m2;
                    break;
                case 3:
                    res.r = 2 * hsl.l - m1;
                    res.g = m2;
                    res.b = m1;
                    break;
                case 4:
                    res.r = m2;
                    res.g = 2 * hsl.l - m1;
                    res.b = m1;
                    break;
                case 5:
                    res.r = m1;
                    res.g = 2 * hsl.l - m1;
                    res.b = m2;
                    break;
                default:
                    res.r = 2 * hsl.l - m1;
                    res.g = 2 * hsl.l - m1;
                    res.b = 2 * hsl.l - m1;
            }

            res.alpha = hsl.alpha;
            return res;
        }

        RGB HSLtoRGB(f64 h, f64 s, f64 l, f64 a) {
            return HSLtoRGB({ h, s, l ,a });
        }

        HSL RGBtoHSL(const RGB& rgb) {
            double M = std::max({ rgb.r, rgb.g, rgb.b });
            double m = std::min({ rgb.r, rgb.g, rgb.b });
            HSL res;

            res.s = (M == m) ? 0 : (M - m) / (1 - std::abs(M + m - 1));
            res.l = 0.5 * (M + m);

            if (M - m != 0) {
                if (M == rgb.r) {
                    res.h = (rgb.g - rgb.b) / (M - m) + (rgb.g < rgb.b ? 6 : 0);
                } else if (M == rgb.g) {
                    res.h = (rgb.b - rgb.r) / (M - m) + 2;
                } else {
                    res.h = (rgb.r - rgb.g) / (M - m) + 4;
                }
                res.h *= 60;
            }

            res.alpha = rgb.alpha;
            return res;
        }

        HSL RGBtoHSL(f64 r, f64 g, f64 b, f64 a) {
            return RGBtoHSL({ r, g, b, a });
        }

    }
}
