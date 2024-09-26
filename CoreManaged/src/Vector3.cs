using System;

namespace Xnor.Core
{
    public struct Vector3
    {
        public static readonly Vector3 Zero = new();
        
        public static readonly Vector3 UnitX = new(1f, 0f, 0f);
        
        public static readonly Vector3 UnitY = new(0f, 1f, 0f);
        
        public static readonly Vector3 UnitZ = new(0f, 0f, 1f);
        
        public static readonly Vector3 One = new(1f);

        public static Vector3 operator+(Vector3 a, Vector3 b) => new(a.X + b.X, a.Y + b.Y, a.Z + b.Z);

        public static Vector3 operator-(Vector3 m) => new(-m.X, -m.Y, -m.Z);

        public static Vector3 operator-(Vector3 a, Vector3 b) => a + -b;

        public static Vector3 operator*(Vector3 a, Vector3 b) => new(a.X * b.X, a.Y * b.Y, a.Z * b.Z);

        public static Vector3 operator*(Vector3 a, float scalar) => new(a.X * scalar, a.Y * scalar, a.Z * scalar);

        public static Vector3 operator*(float scalar, Vector3 b) => b * scalar;

        public static Vector3 operator/(Vector3 a, Vector3 b) => new(a.X / b.X, a.Y / b.Y, a.Z / b.Z);

        public static Vector3 operator/(Vector3 a, float scalar) => new(a.X / scalar, a.Y / scalar, a.Z / scalar);

        public static float Dot(Vector3 a, Vector3 b) => a.X * b.X + a.Y * b.Y + a.Z * b.Z; 

        public static explicit operator Vector2(Vector3 v) => new(v.X, v.Y);

        public static explicit operator Vector4(Vector3 v) => new(v.X, v.Y, 0f, 1f);
        
        public float X;
        public float Y;
        public float Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(float xyz) : this(xyz, xyz, xyz) { }

        public float SquaredLength => X * X + Y * Y + Z * Z;

        public float Length => MathF.Sqrt(SquaredLength);

        public Vector3 Normalized()
        {
            float invLength = 1f / Length;
            return new(X * invLength, Y * invLength, Z * invLength);
        }
        
        public override string ToString() => $"X: {X}, Y: {Y}, Z: {Z}";
    
        public bool Equals(Vector3 other) => CoreNative.Equals(X, other.X) && CoreNative.Equals(Y, other.Y) && CoreNative.Equals(Z, other.Z);

        public override bool Equals(object obj) => obj is Vector3 other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(X, Y, Z);

        public static bool operator ==(Vector3 left, Vector3 right) => left.Equals(right);

        public static bool operator !=(Vector3 left, Vector3 right) => !left.Equals(right);
    }
}
