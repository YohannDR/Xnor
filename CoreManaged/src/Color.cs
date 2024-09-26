using System;

namespace Xnor.Core
{
    public struct Color
    {
        public static readonly Color Transparent = new(0f, 0f, 0f, 0f);
        public static readonly Color AliceBlue = new(0xf0 / 255f, 0xf8 / 255f, 0xff / 255f);
        public static readonly Color AntiqueWhite = new(0xfa / 255f, 0xeb / 255f, 0xd7 / 255f);
        public static readonly Color Aqua = new(0x00 / 255f, 0xff / 255f, 0xff / 255f);
        public static readonly Color Aquamarine = new(0x7f / 255f, 0xff / 255f, 0xd4 / 255f);
        public static readonly Color Azure = new(0xf0 / 255f, 0xff / 255f, 0xff / 255f);
        public static readonly Color Beige = new(0xf5 / 255f, 0xf5 / 255f, 0xdc / 255f);
        public static readonly Color Bisque = new(0xff / 255f, 0xe4 / 255f, 0xc4 / 255f);
        public static readonly Color Black = new(0f, 0f, 0f);
        public static readonly Color BlanchedAlmond = new(0xff / 255f, 0xeb / 255f, 0xcd / 255f);
        public static readonly Color Blue = new(0x00 / 255f, 0x00 / 255f, 0xff / 255f);
        public static readonly Color BlueViolet = new(0x8a / 255f, 0x2b / 255f, 0xe2 / 255f);
        public static readonly Color Brown = new(0xa5 / 255f, 0x2a / 255f, 0x2a / 255f);
        public static readonly Color BurlyWood = new(0xde / 255f, 0xb8 / 255f, 0x87 / 255f);
        public static readonly Color CadetBlue = new(0x5f / 255f, 0x9e / 255f, 0xa0 / 255f);
        public static readonly Color Chartreuse = new(0x7f / 255f, 0xff / 255f, 0x00 / 255f);
        public static readonly Color Chocolate = new(0xd2 / 255f, 0x69 / 255f, 0x1e / 255f);
        public static readonly Color Coral = new(0xff / 255f, 0x7f / 255f, 0x50 / 255f);
        public static readonly Color CornflowerBlue = new(0x64 / 255f, 0x95 / 255f, 0xed / 255f);
        public static readonly Color Cornsilk = new(0xff / 255f, 0xf8 / 255f, 0xdc / 255f);
        public static readonly Color Crimson = new(0xdc / 255f, 0x14 / 255f, 0x3c / 255f);
        public static readonly Color Cyan = new(0x00 / 255f, 0xff / 255f, 0xff / 255f);
        public static readonly Color DarkBlue = new(0x00 / 255f, 0x00 / 255f, 0x8b / 255f);
        public static readonly Color DarkCyan = new(0x00 / 255f, 0x8b / 255f, 0x8b / 255f);
        public static readonly Color DarkGoldenrod = new(0xb8 / 255f, 0x86 / 255f, 0x0b / 255f);
        public static readonly Color DarkGray = new(0xa9 / 255f, 0xa9 / 255f, 0xa9 / 255f);
        public static readonly Color DarkGreen = new(0x00 / 255f, 0x64 / 255f, 0x00 / 255f);
        public static readonly Color DarkKhaki = new(0xbd / 255f, 0xb7 / 255f, 0x6b / 255f);
        public static readonly Color DarkMagenta = new(0x8b / 255f, 0x00 / 255f, 0x8b / 255f);
        public static readonly Color DarkOliveGreen = new(0x55 / 255f, 0x6b / 255f, 0x2f / 255f);
        public static readonly Color DarkOrange = new(0xff / 255f, 0x8c / 255f, 0x00 / 255f);
        public static readonly Color DarkOrchid = new(0x99 / 255f, 0x32 / 255f, 0xcc / 255f);
        public static readonly Color DarkRed = new(0x8b / 255f, 0x00 / 255f, 0x00 / 255f);
        public static readonly Color DarkSalmon = new(0xe9 / 255f, 0x96 / 255f, 0x7a / 255f);
        public static readonly Color DarkSeaGreen = new(0x8f / 255f, 0xbc / 255f, 0x8b / 255f);
        public static readonly Color DarkSlateBlue = new(0x48 / 255f, 0x3d / 255f, 0x8b / 255f);
        public static readonly Color DarkSlateGray = new(0x2f / 255f, 0x4f / 255f, 0x4f / 255f);
        public static readonly Color DarkTurquoise = new(0x00 / 255f, 0xce / 255f, 0xd1 / 255f);
        public static readonly Color DarkViolet = new(0x94 / 255f, 0x00 / 255f, 0xd3 / 255f);
        public static readonly Color DeepPink = new(0xff / 255f, 0x14 / 255f, 0x93 / 255f);
        public static readonly Color DeepSkyBlue = new(0x00 / 255f, 0xbf / 255f, 0xff / 255f);
        public static readonly Color DimGray = new(0x69 / 255f, 0x69 / 255f, 0x69 / 255f);
        public static readonly Color DodgerBlue = new(0x1e / 255f, 0x90 / 255f, 0xff / 255f);
        public static readonly Color Firebrick = new(0xb2 / 255f, 0x22 / 255f, 0x22 / 255f);
        public static readonly Color FloralWhite = new(0xff / 255f, 0xfa / 255f, 0xf0 / 255f);
        public static readonly Color ForestGreen = new(0x22 / 255f, 0x8b / 255f, 0x22 / 255f);
        public static readonly Color Fuchsia = new(0xff / 255f, 0x00 / 255f, 0xff / 255f);
        public static readonly Color Gainsboro = new(0xdc / 255f, 0xdc / 255f, 0xdc / 255f);
        public static readonly Color GhostWhite = new(0xf8 / 255f, 0xf8 / 255f, 0xff / 255f);
        public static readonly Color Gold = new(0xff / 255f, 0xd7 / 255f, 0x00 / 255f);
        public static readonly Color Goldenrod = new(0xda / 255f, 0xa5 / 255f, 0x20 / 255f);
        public static readonly Color Gray = new(0x80 / 255f, 0x80 / 255f, 0x80 / 255f);
        public static readonly Color Green = new(0x00 / 255f, 0x80 / 255f, 0x00 / 255f);
        public static readonly Color GreenYellow = new(0xad / 255f, 0xff / 255f, 0x2f / 255f);
        public static readonly Color Honeydew = new(0xf0 / 255f, 0xff / 255f, 0xf0 / 255f);
        public static readonly Color HotPink = new(0xff / 255f, 0x69 / 255f, 0xb4 / 255f);
        public static readonly Color IndianRed = new(0xcd / 255f, 0x5c / 255f, 0x5c / 255f);
        public static readonly Color Indigo = new(0x4b / 255f, 0x00 / 255f, 0x82 / 255f);
        public static readonly Color Ivory = new(0xff / 255f, 0xff / 255f, 0xf0 / 255f);
        public static readonly Color Khaki = new(0xf0 / 255f, 0xe6 / 255f, 0x8c / 255f);
        public static readonly Color Lavender = new(0xe6 / 255f, 0xe6 / 255f, 0xfa / 255f);
        public static readonly Color LavenderBlush = new(0xff / 255f, 0xf0 / 255f, 0xf5 / 255f);
        public static readonly Color LawnGreen = new(0x7c / 255f, 0xfc / 255f, 0x00 / 255f);
        public static readonly Color LemonChiffon = new(0xff / 255f, 0xfa / 255f, 0xcd / 255f);
        public static readonly Color LightBlue = new(0xad / 255f, 0xd8 / 255f, 0xe6 / 255f);
        public static readonly Color LightCoral = new(0xf0 / 255f, 0x80 / 255f, 0x80 / 255f);
        public static readonly Color LightCyan = new(0xe0 / 255f, 0xff / 255f, 0xff / 255f);
        public static readonly Color LightGoldenrodYellow = new(0xfa / 255f, 0xfa / 255f, 0xd2 / 255f);
        public static readonly Color LightGray = new(0xd3 / 255f, 0xd3 / 255f, 0xd3 / 255f);
        public static readonly Color LightGreen = new(0x90 / 255f, 0xee / 255f, 0x90 / 255f);
        public static readonly Color LightPink = new(0xff / 255f, 0xb6 / 255f, 0xc1 / 255f);
        public static readonly Color LightSalmon = new(0xff / 255f, 0xa0 / 255f, 0x7a / 255f);
        public static readonly Color LightSeaGreen = new(0x20 / 255f, 0xb2 / 255f, 0xaa / 255f);
        public static readonly Color LightSkyBlue = new(0x87 / 255f, 0xce / 255f, 0xfa / 255f);
        public static readonly Color LightSlateGray = new(0x77 / 255f, 0x88 / 255f, 0x99 / 255f);
        public static readonly Color LightSteelBlue = new(0xb0 / 255f, 0xc4 / 255f, 0xde / 255f);
        public static readonly Color LightYellow = new(0xff / 255f, 0xff / 255f, 0xe0 / 255f);
        public static readonly Color Lime = new(0x00 / 255f, 0xff / 255f, 0x00 / 255f);
        public static readonly Color LimeGreen = new(0x32 / 255f, 0xcd / 255f, 0x32 / 255f);
        public static readonly Color Linen = new(0xfa / 255f, 0xf0 / 255f, 0xe6 / 255f);
        public static readonly Color Magenta = new(0xff / 255f, 0x00 / 255f, 0xff / 255f);
        public static readonly Color Maroon = new(0x80 / 255f, 0x00 / 255f, 0x00 / 255f);
        public static readonly Color MediumAquamarine = new(0x66 / 255f, 0xcd / 255f, 0xaa / 255f);
        public static readonly Color MediumBlue = new(0x00 / 255f, 0x00 / 255f, 0xcd / 255f);
        public static readonly Color MediumOrchid = new(0xba / 255f, 0x55 / 255f, 0xd3 / 255f);
        public static readonly Color MediumPurple = new(0x93 / 255f, 0x70 / 255f, 0xdb / 255f);
        public static readonly Color MediumSeaGreen = new(0x3c / 255f, 0xb3 / 255f, 0x71 / 255f);
        public static readonly Color MediumSlateBlue = new(0x7b / 255f, 0x68 / 255f, 0xee / 255f);
        public static readonly Color MediumSpringGreen = new(0x00 / 255f, 0xfa / 255f, 0x9a / 255f);
        public static readonly Color MediumTurquoise = new(0x48 / 255f, 0xd1 / 255f, 0xcc / 255f);
        public static readonly Color MediumVioletRed = new(0xc7 / 255f, 0x15 / 255f, 0x85 / 255f);
        public static readonly Color MidnightBlue = new(0x19 / 255f, 0x19 / 255f, 0x70 / 255f);
        public static readonly Color MintCream = new(0xf5 / 255f, 0xff / 255f, 0xfa / 255f);
        public static readonly Color MistyRose = new(0xff / 255f, 0xe4 / 255f, 0xe1 / 255f);
        public static readonly Color Moccasin = new(0xff / 255f, 0xe4 / 255f, 0xb5 / 255f);
        public static readonly Color MonoGameOrange = new(0xe7 / 255f, 0x3c / 255f, 0x00 / 255f);
        public static readonly Color NavajoWhite = new(0xff / 255f, 0xde / 255f, 0xad / 255f);
        public static readonly Color Navy = new(0x00 / 255f, 0x00 / 255f, 0x80 / 255f);
        public static readonly Color OldLace = new(0xfd / 255f, 0xf5 / 255f, 0xe6 / 255f);
        public static readonly Color Olive = new(0x80 / 255f, 0x80 / 255f, 0x00 / 255f);
        public static readonly Color OliveDrab = new(0x6b / 255f, 0x8e / 255f, 0x23 / 255f);
        public static readonly Color Orange = new(0xff / 255f, 0xa5 / 255f, 0x00 / 255f);
        public static readonly Color OrangeRed = new(0xff / 255f, 0x45 / 255f, 0x00 / 255f);
        public static readonly Color Orchid = new(0xda / 255f, 0x70 / 255f, 0xd6 / 255f);
        public static readonly Color PaleGoldenrod = new(0xee / 255f, 0xe8 / 255f, 0xaa / 255f);
        public static readonly Color PaleGreen = new(0x98 / 255f, 0xfb / 255f, 0x98 / 255f);
        public static readonly Color PaleTurquoise = new(0xaf / 255f, 0xee / 255f, 0xee / 255f);
        public static readonly Color PaleVioletRed = new(0xdb / 255f, 0x70 / 255f, 0x93 / 255f);
        public static readonly Color PapayaWhip = new(0xff / 255f, 0xef / 255f, 0xd5 / 255f);
        public static readonly Color PeachPuff = new(0xff / 255f, 0xda / 255f, 0xb9 / 255f);
        public static readonly Color Peru = new(0xcd / 255f, 0x85 / 255f, 0x3f / 255f);
        public static readonly Color Pink = new(0xff / 255f, 0xc0 / 255f, 0xcb / 255f);
        public static readonly Color Plum = new(0xdd / 255f, 0xa0 / 255f, 0xdd / 255f);
        public static readonly Color PowderBlue = new(0xb0 / 255f, 0xe0 / 255f, 0xe6 / 255f);
        public static readonly Color Purple = new(0x80 / 255f, 0x00 / 255f, 0x80 / 255f);
        public static readonly Color Red = new(0xff / 255f, 0x00 / 255f, 0x00 / 255f);
        public static readonly Color RosyBrown = new(0xbc / 255f, 0x8f / 255f, 0x8f / 255f);
        public static readonly Color RoyalBlue = new(0x41 / 255f, 0x69 / 255f, 0xe1 / 255f);
        public static readonly Color SaddleBrown = new(0x8b / 255f, 0x45 / 255f, 0x13 / 255f);
        public static readonly Color Salmon = new(0xfa / 255f, 0x80 / 255f, 0x72 / 255f);
        public static readonly Color SandyBrown = new(0xf4 / 255f, 0xa4 / 255f, 0x60 / 255f);
        public static readonly Color SeaGreen = new(0x2e / 255f, 0x8b / 255f, 0x57 / 255f);
        public static readonly Color SeaShell = new(0xff / 255f, 0xf5 / 255f, 0xee / 255f);
        public static readonly Color Sienna = new(0xa0 / 255f, 0x52 / 255f, 0x2d / 255f);
        public static readonly Color Silver = new(0xc0 / 255f, 0xc0 / 255f, 0xc0 / 255f);
        public static readonly Color SkyBlue = new(0x87 / 255f, 0xce / 255f, 0xeb / 255f);
        public static readonly Color SlateBlue = new(0x6a / 255f, 0x5a / 255f, 0xcd / 255f);
        public static readonly Color SlateGray = new(0x70 / 255f, 0x80 / 255f, 0x90 / 255f);
        public static readonly Color Snow = new(0xff / 255f, 0xfa / 255f, 0xfa / 255f);
        public static readonly Color SpringGreen = new(0x00 / 255f, 0xff / 255f, 0x7f / 255f);
        public static readonly Color SteelBlue = new(0x46 / 255f, 0x82 / 255f, 0xb4 / 255f);
        public static readonly Color Tan = new(0xd2 / 255f, 0xb4 / 255f, 0x8c / 255f);
        public static readonly Color Teal = new(0x00 / 255f, 0x80 / 255f, 0x80 / 255f);
        public static readonly Color Thistle = new(0xd8 / 255f, 0xbf / 255f, 0xd8 / 255f);
        public static readonly Color Tomato = new(0xff / 255f, 0x63 / 255f, 0x47 / 255f);
        public static readonly Color Turquoise = new(0x40 / 255f, 0xe0 / 255f, 0xd0 / 255f);
        public static readonly Color Violet = new(0xee / 255f, 0x82 / 255f, 0xee / 255f);
        public static readonly Color Wheat = new(0xf5 / 255f, 0xde / 255f, 0xb3 / 255f);
        public static readonly Color White = new(1f, 1f, 1f);
        public static readonly Color WhiteSmoke = new(0xf5 / 255f, 0xf5 / 255f, 0xf5 / 255f);
        public static readonly Color Yellow = new(0xff / 255f, 0xff / 255f, 0x00 / 255f);
        public static readonly Color YellowGreen = new(0x9a / 255f, 0xcd / 255f, 0x32 / 255f);

        public static Color Lerp(Color a, Color b, float t)
        {
            return new(
                CoreNative.Lerp(a.R, b.R, t),
                CoreNative.Lerp(a.G, b.G, t),
                CoreNative.Lerp(a.B, b.B, t),
                CoreNative.Lerp(a.A, b.A, t)
            );
        }

        public static Color operator +(Color a, Color b) => new(MathF.Min(a.R + b.R, 1f), MathF.Min(a.G + b.G, 1f), MathF.Min(a.B + b.B, 1f), MathF.Min(a.A + b.A, 1f));

        public static Color operator *(Color a, Color b) => new(MathF.Min(a.R * b.R, 1f), MathF.Min(a.G * b.G, 1f), MathF.Min(a.B * b.B, 1f), MathF.Min(a.A * b.A, 1f));

        public static Color operator *(Color color, float alphaFactor) => new(color.R, color.G, color.B, MathF.Min(color.A * alphaFactor, 1f));

        public static explicit operator ColorHsva(Color color)
        {
            ColorHsva hsv = new()
            {
                A = color.A
            };
            float minVal = MathF.Min(MathF.Min(color.R, color.G), color.B);
            float maxVal = MathF.Max(MathF.Max(color.R, color.G), color.B);
            hsv.V = maxVal;
            float delta = maxVal - minVal;
    
            if (delta == 0) // Black
            {
                hsv.H = 0;
                hsv.S = 0;
            }
            else
            {
                hsv.S = delta / maxVal;
                float deltaInverse = 1f / delta;
                if (CoreNative.Equals(color.R, maxVal))
                    hsv.H = ColorHsva.HueCircleOver6 * (color.G - color.B) * deltaInverse;
                else if (CoreNative.Equals(color.G, maxVal))
                    hsv.H = ColorHsva.HueCircleOver3 + ColorHsva.HueCircleOver6 * (color.B - color.R) * deltaInverse;
                else
                    hsv.H = ColorHsva.HueCircleOver3 * 2 + ColorHsva.HueCircleOver6 * (color.R - color.G) * deltaInverse;

                float floor = MathF.Floor(hsv.H);
                if (hsv.H < 0f)
                    hsv.H += MathF.Abs(floor);
                else
                    hsv.H -= floor;
            }
            return hsv;
        }

        public static explicit operator Vector3(Color color) => new(color.R, color.G, color.B);

        public static explicit operator Vector4(Color color) => new(color.R, color.G, color.B, color.A);

        public static bool operator ==(Color left, Color right) => left.Equals(right);

        public static bool operator !=(Color left, Color right) => !left.Equals(right);
        
        public float R;
        public float G;
        public float B;
        public float A;

        public Color(float r, float g, float b, float a = 1f)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public Color(float rgb) : this(rgb, rgb, rgb) { }
        
        public override string ToString() => $"R: {R}, G: {G}, B: {B}, A: {A}";
    
        public bool Equals(Color other) => CoreNative.Equals(R, other.R) && CoreNative.Equals(G, other.G) && CoreNative.Equals(B, other.B) && CoreNative.Equals(A, other.A);

        public override bool Equals(object obj) => obj is Color other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(R, G, B, A);
    }
}
