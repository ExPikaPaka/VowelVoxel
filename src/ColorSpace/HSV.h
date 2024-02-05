#pragma once
#include "Common.h"
#include "../dataTypes.h"

namespace ent {
	namespace color {
        struct HSV {
            double h;
            double s;
            double v;
            double alpha;
        };

        /*
         * Converts an HSV (Hue, Saturation, Value) color to its RGB representation.
         *
         * @param hsv An HSV color represented as a struct with hue (h), saturation (s), value (v), and alpha (a) values.
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB HSVtoRGB(const HSV& hsv);

        /*
         * Converts an HSV (Hue, Saturation, Value) color to its RGB representation.
         *
         * @param h The hue value in the range [0.0, 1.0].
         * @param s The saturation value in the range [0.0, 1.0].
         * @param v The brightness value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB HSVtoRGB(f64 h, f64 s, f64 v, f64 a = 1.0f);

        /*
         * Converts an RGB (Red, Green, Blue) color to its HSV representation.
         *
         * @param rgb An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         * @return An HSV color represented as a struct with hue (h), saturation (s), value (v), and alpha (a) values.
         */
        HSV RGBtoHSV(const RGB& rgb);

        /*
         * Converts an RGB (Red, Green, Blue) color to its HSV representation.
         *
         * @param r The red value in the range [0.0, 1.0].
         * @param g The green value in the range [0.0, 1.0].
         * @param b The blue value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An HSV color represented as a struct with hue (h), saturation (s), value (v), and alpha (a) values.
         */
        HSV RGBtoHSV(f64 r, f64 g, f64 b, f64 a = 1.0f);
	}
}


