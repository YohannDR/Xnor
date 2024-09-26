using System;

namespace Xnor.Core
{
    public struct Quaternion
    {
        public static readonly Quaternion Zero = new();
        
        public static readonly Quaternion UnitX = new(1f, 0f, 0f, 0f);
        
        public static readonly Quaternion UnitY = new(0f, 1f, 0f, 0f);
        
        public static readonly Quaternion UnitZ = new(0f, 0f, 1f, 0f);
        
        public static readonly Quaternion UnitW = new(0f, 0f, 0f, 1f);
        
        public static readonly Quaternion Identity = UnitW;
        
        public static readonly Quaternion One = new(1f);
        
        float QuaternionSquaredLength() { return Imaginary.X *  Imaginary.X +  Imaginary.Y * Imaginary.Y + Imaginary.Z * Imaginary.Z + Real * Real; }
        
        float Length()
        {
            return MathF.Sqrt(QuaternionSquaredLength());
        }
        public Quaternion Normalize()
        {
            float length = Length();
            return new Quaternion(Imaginary / length, Real / length);
        }
        
        public static Quaternion FromEuler(Vector3 v)
        {
            Quaternion result = Zero;
            
            float cr = MathF.Cos(v.X * 0.5f);
            float sr = MathF.Sin(v.X * 0.5f);
            float cp = MathF.Cos(v.Y * 0.5f);
            float sp = MathF.Sin(v.Y * 0.5f);
            float cy = MathF.Cos(v.Z * 0.5f);
            float sy = MathF.Sin(v.Z * 0.5f);

            result.X = sr * cp * cy - cr * sp * sy;
            result.Y = cr * sp * cy + sr * cp * sy;
            result.Z = cr * cp * sy - sr * sp * cy;
            result.W = cr * cp * cy + sr * sp * sy;

            return result;
        }

        public static Vector3 ToEuler(Quaternion q)
        {
            Vector3 result = Vector3.Zero;
            
            // roll (x-axis q)
            float sinrCosp = 2f * (q.W * q.X + q.Y * q.Z);
            float cosrCosp = 1f - 2f * (q.X * q.X + q.Y * q.Y);
            result.X = MathF.Atan2(sinrCosp, cosrCosp);

            // pitch (y-axis q)
            float sinp = MathF.Sqrt(1f + 2f * (q.W * q.Y - q.X * q.Z));
            float cosp = MathF.Sqrt(1f - 2f * (q.W * q.Y - q.X * q.Z));
            result.Y = 2f * MathF.Atan2(sinp, cosp) - CoreNative.PiOver2;

            // yaw (z-axis q)
            float sinyCosp = 2f * (q.W * q.Z + q.X * q.Y);
            float cosyCosp = 1f - 2f * (q.Y * q.Y + q.Z * q.Z);
            result.Z = MathF.Atan2(sinyCosp, cosyCosp);

            return result;
        }
        
        public static Quaternion operator+(Quaternion a, Quaternion b) => new(a.Imaginary + b.Imaginary, a.Real + b.Real);

        public static Quaternion operator-(Quaternion m) => new(-m.Imaginary, -m.Real);

        public static Quaternion operator-(Quaternion a, Quaternion b) => a + -b;

        public static Quaternion operator*(Quaternion a, float scalar) => new(a.Imaginary * scalar, a.Real * scalar);

        public static Quaternion operator*(float scalar, Quaternion b) => b * scalar;

        public static Quaternion operator*(Quaternion a, Quaternion b)
        {
            return new Quaternion(a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y,
                a.W * b.Y + a.Y * b.W + a.Z * b.X - a.X * b.Z, a.W * b.Z + a.Z * b.W + a.X * b.Y - a.Y * b.X,
                a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z);
        }

        public static Quaternion operator/(Quaternion a, float scalar) => new(a.Imaginary / scalar, a.Real / scalar);

        public static explicit operator Vector3(Quaternion q) => q.Imaginary;

        public static explicit operator Vector4(Quaternion q) => new(q.X, q.Y, q.Z, q.W);
        
        public Vector3 Imaginary;
        public float Real;
        
        public float X { get => Imaginary.X; set => Imaginary.X = value; }
        public float Y { get => Imaginary.Y; set => Imaginary.Y = value; }
        public float Z { get => Imaginary.Z; set => Imaginary.Z = value; }
        public float W { get => Real; set => Real = value; }

        public Quaternion(Vector3 imaginary, float real)
        {
            Imaginary = imaginary;
            Real = real;
        }

        public Quaternion(float xyzw) : this(new(xyzw), xyzw) { }

        public Quaternion(float x, float y, float z, float w) : this(new(x, y, z), w) { }
        
        public override string ToString() => $"X: {X}, Y: {Y}, Z: {Z}, W: {W}";
    
        public bool Equals(Quaternion other) => Imaginary == other.Imaginary && CoreNative.Equals(Real, other.Real);

        public override bool Equals(object obj) => obj is Quaternion other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(Imaginary, Real);

        public static bool operator ==(Quaternion left, Quaternion right) => left.Equals(right);

        public static bool operator !=(Quaternion left, Quaternion right) => !left.Equals(right);
    }
}
