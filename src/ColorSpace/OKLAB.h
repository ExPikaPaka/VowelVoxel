#pragma once
#include "Common.h"
#include "LRGB.h"
#include "../dataTypes.h"

namespace ent {
    namespace color {
        struct OKLAB {
            double l;
            double a;
            double b;
            double alpha;
        };

        /*
         * Converts an LRGB (Linear RGB) color to its OKLAB representation.
         *
         * @param lrgb An LRGB color represented as a struct with red (r), green (g), blue (b), and alpha (a) values.
         * @return An OKLAB color represented as a struct with lightness (l), green-red chromaticity (a), and blue-yellow chromaticity (b), and alpha (alpha) values.
         */
        OKLAB LRGBtoOKLAB(const RGB& lrgb);

        /*
         * Converts an LRGB (Linear RGB) color to its OKLAB representation.
         *
         * @param r The red value in the range [0.0, 1.0].
         * @param g The green value in the range [0.0, 1.0].
         * @param b The blue value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An OKLAB color represented as a struct with lightness (l), green-red chromaticity (a), blue-yellow chromaticity (b), and alpha (alpha) values.
         */
        OKLAB LRGBtoOKLAB(f64 r, f64 g, f64 b, f64 a = 1.0f);

        /*
         * Converts an OKLAB color to its LRGB (Linear RGB) representation.
         *
         * @param oklab An OKLAB color represented as a struct with lightness (l), green-red chromaticity (a), blue-yellow chromaticity (b), and alpha (alpha) values.
         * @return An LRGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB OKLABtoLRGB(const OKLAB& oklab);

        /*
         * Converts an OKLAB color to its LRGB (Linear RGB) representation.
         *
         * @param l The lightness value in the range [0.0, 1.0].
         * @param a The green-red chromaticity value in the range [-0.4, 0.4].
         * @param b The blue-yellow chromaticity value in the range [-0.4, 0.4].
         * @param alpha The alpha value in the range [0.0, 1.0].
         * @return An LRGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB OKLABtoLRGB(f64 l, f64 a, f64 b, f64 alpha = 1.0f);

        /*
         * Converts an RGB (Red, Green, Blue) color to its OKLAB representation.
         *
         * @param rgb An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (a) values.
         * @return An OKLAB color represented as a struct with lightness (l), green-red chromaticity (a), blue-yellow chromaticity (b), and alpha (alpha) values.
         */
        OKLAB RGBtoOKLAB(const RGB& rgb);

        /*
         * Converts an RGB (Red, Green, Blue) color to its OKLAB representation.
         *
         * @param r The red value in the range [0.0, 1.0].
         * @param g The green value in the range [0.0, 1.0].
         * @param b The blue value in the range [0.0, 1.0].
         * @param a The alpha value in the range [0.0, 1.0].
         * @return An OKLAB color represented as a struct with lightness (l), green-red chromaticity (a), blue-yellow chromaticity (b), and alpha (a) values.
         */
        OKLAB RGBtoOKLAB(f64 r, f64 g, f64 b, f64 a = 1.0f);

        /*
         * Converts an OKLAB color to its RGB (Red, Green, Blue) representation.
         *
         * @param oklab An OKLAB color represented as a struct with lightness (l), green-red chromaticity (a), blue-yellow chromaticity (b), and alpha (alpha) values.
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB OKLABtoRGB(const OKLAB& oklab);

        /*
         * Converts an OKLAB color to its RGB (Red, Green, Blue) representation.
         *
         * @param l The lightness value in the range [0.0, 1.0].
         * @param a The green-red chromaticity value in the range [-0.4, 0.4].
         * @param b The blue-yellow chromaticity value in the range [-0.4, 0.4].
         * @param alpha The alpha value in the range [0.0, 1.0].
         * @return An RGB color represented as a struct with red (r), green (g), blue (b), and alpha (alpha) values.
         */
        RGB OKLABtoRGB(f64 l, f64 a, f64 b, f64 alpha = 1.0f);
    }
}

