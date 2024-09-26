using System;

namespace Xnor.Core
{
    public struct Vector2
    {
        public static readonly Vector2 Zero = new();
        
        public static readonly Vector2 UnitX = new(1f, 0f);
        
        public static readonly Vector2 UnitY = new(0f, 1f);
        
        public static readonly Vector2 One = new(1f);

        public static Vector2 operator+(Vector2 a, Vector2 b) => new(a.X + b.X, a.Y + b.Y);

        public static Vector2 operator-(Vector2 m) => new(-m.X, -m.Y);

        public static Vector2 operator-(Vector2 a, Vector2 b) => a + -b;

        public static Vector2 operator*(Vector2 a, Vector2 b) => new(a.X * b.X, a.Y * b.Y);

        public static Vector2 operator*(Vector2 a, float scalar) => new(a.X * scalar, a.Y * scalar);

        public static Vector2 operator*(float scalar, Vector2 b) => b * scalar;

        public static Vector2 operator/(Vector2 a, Vector2 b) => new(a.X / b.X, a.Y / b.Y);

        public static Vector2 operator/(Vector2 a, float scalar) => new(a.X / scalar, a.Y / scalar);

        public static explicit operator Vector2i(Vector2 v) => new((int) v.X, (int) v.Y);

        public static explicit operator Vector3(Vector2 v) => new(v.X, v.Y, 0f);

        public static explicit operator Vector4(Vector2 v) => new(v.X, v.Y, 0f, 1f);
        
        public float X;
        public float Y;

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public Vector2(float xy) : this(xy, xy) { }

        public float SquaredLength => X * X + Y * Y;

        public float Length => MathF.Sqrt(SquaredLength);

        public Vector2 Normalized()
        {
            float invLength = 1f / Length;
            return new(X * invLength, Y * invLength);
        }
        
        public override string ToString() => $"X: {X}, Y: {Y}";
    
        public bool Equals(Vector2 other) => CoreNative.Equals(X, other.X) && CoreNative.Equals(Y, other.Y);

        public override bool Equals(object obj) => obj is Vector2 other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(X, Y);

        public static bool operator ==(Vector2 left, Vector2 right) => left.Equals(right);

        public static bool operator !=(Vector2 left, Vector2 right) => !left.Equals(right);
    }
}
