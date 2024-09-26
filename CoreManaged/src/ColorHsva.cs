using System;

namespace Xnor.Core
{
    public struct ColorHsva
    {
        public const float HueCircle = 1f;
        public const float HueCircleOver3 = HueCircle / 3f;
        public const float HueCircleOver6 = HueCircle / 6f;
        
        public static readonly ColorHsva White = new(0, 0, 1f);
        public static readonly ColorHsva Gray = new(0, 0, 0.5f);
        public static readonly ColorHsva Black = new(0, 0, 0);
        
        public static readonly ColorHsva Red = new(0, 1f, 1f);
        public static readonly ColorHsva Green = new(HueCircleOver3, 1f, 1f);
        public static readonly ColorHsva Blue = new(HueCircleOver3 * 2f, 1f, 1f);
        
        public static readonly ColorHsva Yellow = new(HueCircleOver6, 1f, 1f);
        public static readonly ColorHsva LightBlue = new(HueCircleOver3 + HueCircleOver6, 1f, 1f);
        public static readonly ColorHsva Magenta = new(HueCircleOver3 * 2f + HueCircleOver6, 1f, 1f);

        public static ColorHsva operator *(ColorHsva color, float alphaFactor) => new(color.H, color.S, color.V, Math.Clamp(color.A * alphaFactor, 0f, 1f));

        public static explicit operator Color(ColorHsva color)
        {
            if (CoreNative.IsZero(color.S)) // Grayscale
                return new(color.V, color.V, color.V, color.A);

            byte hi = (byte) (color.H / HueCircleOver6);
            float f = CoreNative.Modulo(color.H, HueCircleOver6) * 6f;
            float p = color.V * (1f - color.S);
            float q = color.V * (1f - color.S * f);
            float t = color.V * (1f - color.S * (1f - f));

            return hi switch
            {
                0 => new(color.V, t, p, color.A),
                1 => new(q, color.V, p, color.A),
                2 => new(p, color.V, t, color.A),
                3 => new(p, q, color.V, color.A),
                4 => new(t, p, color.V, color.A),
                _ => new(color.V, p, q, color.A)
            };
        }

        public float H;
        public float S;
        public float V;
        public float A;

        public ColorHsva(float h, float s, float v, float a = 1f)
        {
            H = h;
            S = s;
            V = v;
            A = a;
        }
        
        public override string ToString() => $"H: {H}, S: {S}, V: {V}, A: {A}";
    
        public bool Equals(ColorHsva other) => CoreNative.Equals(H, other.H) && CoreNative.Equals(S, other.S) && CoreNative.Equals(V, other.V) && CoreNative.Equals(A, other.A);

        public override bool Equals(object obj) => obj is ColorHsva other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(H, S, V, A);

        public static bool operator ==(ColorHsva left, ColorHsva right) => left.Equals(right);

        public static bool operator !=(ColorHsva left, ColorHsva right) => !left.Equals(right);
    }
}
