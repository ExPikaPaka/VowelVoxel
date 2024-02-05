#include "OKLAB.h"

namespace ent {
	namespace color {
        OKLAB LRGBtoOKLAB(const RGB& lrgb) {
            f64 L = std::cbrt(0.41222147079999993 * lrgb.r + 0.5363325363 * lrgb.g + 0.0514459929 * lrgb.b);
            f64 M = std::cbrt(0.2119034981999999 * lrgb.r + 0.6806995450999999 * lrgb.g + 0.1073969566 * lrgb.b);
            f64 S = std::cbrt(0.08830246189999998 * lrgb.r + 0.2817188376 * lrgb.g + 0.6299787005000002 * lrgb.b);

            OKLAB res;
            res.l = 0.2104542553 * L + 0.793617785 * M - 0.0040720468 * S;
            res.a = 1.9779984951 * L - 2.428592205 * M + 0.4505937099 * S;
            res.b = 0.0259040371 * L + 0.7827717662 * M - 0.808675766 * S;
        

            if (lrgb.alpha != 0) {
                res.alpha = lrgb.alpha;
            }

            return res;
        }

        OKLAB LRGBtoOKLAB(f64 r, f64 g, f64 b, f64 a) {
            return LRGBtoOKLAB({r, g, b, a});
        }

        RGB OKLABtoLRGB(const OKLAB& oklab) {
			f64 L = std::pow(oklab.l * 0.99999999845051981432 + 0.39633779217376785678 * oklab.a + 0.21580375806075880339 * oklab.b, 3);
			f64 M = std::pow(oklab.l * 1.0000000088817607767 - 	0.1055613423236563494 * oklab.a - 0.063854174771705903402 * oklab.b, 3);
			f64 S = std::pow(oklab.l * 1.0000000546724109177 - 0.089484182094965759684 * oklab.a - 1.2914855378640917399 * oklab.b,	3);

			RGB res;
			res.r = 4.076741661347994 * L - 3.307711590408193 * M + 0.230969928729428 * S;
			res.g = -1.2684380040921763 * L + 2.6097574006633715 * M - 0.3413193963102197 * S;
			res.b = -0.004196086541837188 * L - 0.7034186144594493 * M + 1.7076147009309444 * S;

			if (oklab.alpha != 0) {
				res.alpha = oklab.alpha;
			}

			return res;
        }

        RGB OKLABtoLRGB(f64 l, f64 a, f64 b, f64 alpha) {
            return OKLABtoLRGB({l, a, b, alpha});
        }

        OKLAB RGBtoOKLAB(const RGB& rgb) {
            OKLAB res = LRGBtoOKLAB(RGBtoLRGB(rgb));
            if (rgb.r == rgb.b && rgb.b == rgb.g) {
                res.a = res.b = 0;
            }
            return res;
		}

        OKLAB RGBtoOKLAB(f64 r, f64 g, f64 b, f64 a) {
            return RGBtoOKLAB({ r,g,b,a });
        }

        RGB OKLABtoRGB(const OKLAB& oklab) {
            return LRGBtoRGB(OKLABtoLRGB(oklab));
        }
        RGB OKLABtoRGB(f64 l, f64 a, f64 b, f64 alpha) {
            return OKLABtoRGB({ l, a, b, alpha });
        }
	}
}