using System;

namespace Xnor.Core
{
    public struct Vector4
    {
        public static readonly Vector4 Zero = new();
        
        public static readonly Vector4 UnitX = new(1f, 0f, 0f, 0f);
        
        public static readonly Vector4 UnitY = new(0f, 1f, 0f, 0f);
        
        public static readonly Vector4 UnitZ = new(0f, 0f, 1f, 0f);
        
        public static readonly Vector4 UnitW = new(0f, 0f, 0f, 1f);
        
        public static readonly Vector4 One = new(1f);

        public static Vector4 operator+(Vector4 a, Vector4 b) => new(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);

        public static Vector4 operator-(Vector4 m) => new(-m.X, -m.Y, -m.Z, -m.W);

        public static Vector4 operator-(Vector4 a, Vector4 b) => a + -b;

        public static Vector4 operator*(Vector4 a, Vector4 b) => new(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W);

        public static Vector4 operator*(Vector4 a, float scalar) => new(a.X * scalar, a.Y * scalar, a.Z * scalar, a.W * scalar);

        public static Vector4 operator*(float scalar, Vector4 b) => b * scalar;

        public static Vector4 operator/(Vector4 a, Vector4 b) => new(a.X / b.X, a.Y / b.Y, a.Z / b.Z, a.W / b.W);

        public static Vector4 operator/(Vector4 a, float scalar) => new(a.X / scalar, a.Y / scalar, a.Z / scalar, a.W / scalar);

        public static explicit operator Vector2(Vector4 v) => new(v.X, v.Y);

        public static explicit operator Vector3(Vector4 v) => new(v.X, v.Y, v.Z);
        
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(float xyzw) : this(xyzw, xyzw, xyzw, xyzw) { }

        public float SquaredLength => X * X + Y * Y + Z * Z + W * W;

        public float Length => MathF.Sqrt(SquaredLength);

        public Vector4 Normalized()
        {
            float invLength = 1f / Length;
            return new(X * invLength, Y * invLength, Z * invLength, W * invLength);
        }
        
        public override string ToString() => $"X: {X}, Y: {Y}, Z: {Z}, W: {W}";
    
        public bool Equals(Vector4 other) => CoreNative.Equals(X, other.X) && CoreNative.Equals(Y, other.Y) && CoreNative.Equals(Z, other.Z) && CoreNative.Equals(W, other.W);

        public override bool Equals(object obj) => obj is Vector4 other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(X, Y, Z, W);

        public static bool operator ==(Vector4 left, Vector4 right) => left.Equals(right);

        public static bool operator !=(Vector4 left, Vector4 right) => !left.Equals(right);
    }
}
