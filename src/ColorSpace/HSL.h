#pragma once
#include "Common.h"
#include "../dataTypes.h"

namespace ent {
    namespace color {
        struct HSL {
            double h;
            double s;
            double l;
            double alpha;
        };

        /*
         * Converts an HSL (Hue, Saturation, Lightness) color to its RGB representation.
         *
         * @param hsl An HSL color represented as a struct with hue (h), saturation (s), lightness (l), and alpha (a) values.
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB HSLtoRGB(const HSL& hsl);

        /*
         * Converts an HSL (Hue, Saturation, Lightness) color to its RGB representation.
         *
         * @param h The hue value in the range [0.0, 360.0].
         * @param s The saturation value in the range [0.0, 1.0].
         * @param l The lightness value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB HSLtoRGB(f64 h, f64 s, f64 l, f64 a = 1.0f);

        /*
         * Converts an RGB (Red, Green, Blue) color to its HSL representation.
         *
         * @param rgb An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         * @return An HSL color represented as a struct with hue (h), saturation (s), lightness (l), and alpha (a) values.
         */
        HSL RGBtoHSL(const RGB& rgb);

        /*
         * Converts an RGB (Red, Green, Blue) color to its HSL representation.
         *
         * @param r The red value in the range [0.0, 1.0].
         * @param g The green value in the range [0.0, 1.0].
         * @param b The blue value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An HSL color represented as a struct with hue (h), saturation (s), lightness (l), and alpha (a) values.
         */
        HSL RGBtoHSL(f64 r, f64 g, f64 b, f64 a = 1.0f);
       
    }
}