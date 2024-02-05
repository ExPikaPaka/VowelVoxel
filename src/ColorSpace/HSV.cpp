#include "HSV.h"
#include "HSL.h"

namespace ent {
    namespace color {
        RGB HSVtoRGB(const HSV& hsv) {
            double h = normalizeHue(hsv.h);
            double f = std::abs(fmod((h / 60), 2.0) - 1);
            RGB res;

            switch (static_cast<int>(h / 60)) {
                case 0:
                    res.r = hsv.v;
                    res.g = hsv.v * (1 - hsv.s * f);
                    res.b = hsv.v * (1 - hsv.s);
                    break;
                case 1:
                    res.r = hsv.v * (1 - hsv.s * f);
                    res.g = hsv.v;
                    res.b = hsv.v * (1 - hsv.s);
                    break;
                case 2:
                    res.r = hsv.v * (1 - hsv.s);
                    res.g = hsv.v;
                    res.b = hsv.v * (1 - hsv.s * f);
                    break;
                case 3:
                    res.r = hsv.v * (1 - hsv.s);
                    res.g = hsv.v * (1 - hsv.s * f);
                    res.b = hsv.v;
                    break;
                case 4:
                    res.r = hsv.v * (1 - hsv.s * f);
                    res.g = hsv.v * (1 - hsv.s);
                    res.b = hsv.v;
                    break;
                case 5:
                    res.r = hsv.v;
                    res.g = hsv.v * (1 - hsv.s);
                    res.b = hsv.v * (1 - hsv.s * f);
                    break;
                default:
                    res.r = hsv.v * (1 - hsv.s);
                    res.g = hsv.v * (1 - hsv.s);
                    res.b = hsv.v * (1 - hsv.s);
            }


            res.alpha = hsv.alpha;
            return res;
        }

        RGB HSVtoRGB(f64 h, f64 s, f64 v, f64 a) {
            return HSVtoRGB({ h, s, v, a });
        }

        HSV RGBtoHSV(const RGB& rgb) {
            double M = std::max({ rgb.r, rgb.g, rgb.b });
            double m = std::min({ rgb.r, rgb.g, rgb.b });
            HSV res;

            res.s = (M == 0) ? 0 : 1 - m / M;
            res.v = M;

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

        HSV RGBtoHSV(f64 r, f64 g, f64 b, f64 a) {
            return RGBtoHSV({ r, g, b, a });
        }


    }
}
