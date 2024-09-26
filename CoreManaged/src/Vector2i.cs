using System;

namespace Xnor.Core
{
    public struct Vector2i
    {
        public static readonly Vector2i Zero = new();
        
        public static readonly Vector2i UnitX = new(1, 0);
        
        public static readonly Vector2i UnitY = new(0, 1);
        
        public static readonly Vector2i One = new(1);

        public static Vector2i operator+(Vector2i a, Vector2i b) => new(a.X + b.X, a.Y + b.Y);

        public static Vector2i operator-(Vector2i m) => new(-m.X, -m.Y);

        public static Vector2i operator-(Vector2i a, Vector2i b) => a + -b;

        public static Vector2i operator*(Vector2i a, Vector2i b) => new(a.X * b.X, a.Y * b.Y);

        public static Vector2i operator*(Vector2i a, int scalar) => new(a.X * scalar, a.Y * scalar);

        public static Vector2i operator*(int scalar, Vector2i b) => b * scalar;

        public static explicit operator Vector2(Vector2i v) => new(v.X, v.Y);

        public static explicit operator Vector3(Vector2i v) => new(v.X, v.Y, 0f);

        public static explicit operator Vector4(Vector2i v) => new(v.X, v.Y, 0f, 1f);
        
        public int X;
        public int Y;

        public Vector2i(int x, int y)
        {
            X = x;
            Y = y;
        }

        public Vector2i(int xy) : this(xy, xy) { }
        
        public override string ToString() => $"X: {X}, Y: {Y}";
    
        public bool Equals(Vector2i other) => X == other.X && Y == other.Y;

        public override bool Equals(object obj) => obj is Vector2i other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(X, Y);

        public static bool operator ==(Vector2i left, Vector2i right) => left.Equals(right);

        public static bool operator !=(Vector2i left, Vector2i right) => !left.Equals(right);
    }
}
