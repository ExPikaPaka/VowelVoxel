#pragma once
#include "Common.h"
#include "../dataTypes.h"

namespace ent {
    namespace color {

        // Function used for RGB-LRGB, LRGB-RGB conversion
        f64 fn(f64 c);

        /*
         * Converts an RGB (Red, Green, Blue) color to its LRGB (Linear RGB) representation.
         *
         * @param rgb An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (a) values.
         * @return An LRGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB RGBtoLRGB(const RGB& rgb);

        /*
         * Converts an RGB (Red, Green, Blue) color to its LRGB (Linear RGB) representation.
         *
         * @param r The red value in the range [0.0, 1.0].
         * @param g The green value in the range [0.0, 1.0].
         * @param b The blue value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An LRGB color represented as a struct with red (r), green (g), blue (b), and alpha (a) values.
         */
        RGB RGBtoLRGB(f64 r, f64 g, f64 b, f64 a = 1.0f);

        /*
         * Converts an LRGB (Linear RGB) color to its RGB (Red, Green, Blue) representation.
         *
         * @param lrgb An LRGB color represented as a struct with red (r), green (g), blue (b), and alpha (a) values.
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB LRGBtoRGB(const RGB& lrgb);

        /*
         * Converts an LRGB (Linear RGB) color to its RGB (Red, Green, Blue) representation.
         *
         * @param r The red value in the range [0.0, 1.0].
         * @param g The green value in the range [0.0, 1.0].
         * @param b The blue value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (a) values.
         */
        RGB LRGBtoRGB(f64 r, f64 g, f64 b, f64 a = 1.0f);
    }
}