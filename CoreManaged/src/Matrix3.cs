using System;

namespace Xnor.Core
{
    public struct Matrix3
    {
        public static readonly Matrix3 Identity = new(
            1f, 0f, 0f,
            0f, 1f, 0f,
            0f, 0f, 1f
        );

        public static Matrix3 operator +(Matrix3 a, Matrix3 b)
        {
            return new(
                a.M00 + b.M00, a.M01 + b.M01, a.M02 + b.M02,
                a.M10 + b.M10, a.M11 + b.M11, a.M12 + b.M12,
                a.M20 + b.M20, a.M21 + b.M21, a.M22 + b.M22
            );
        }

        public static Matrix3 operator -(Matrix3 m)
        {
            return new(
                -m.M00, -m.M01, -m.M02,
                -m.M10, -m.M11, -m.M12,
                -m.M20, -m.M21, -m.M22
            );
        }

        public static Matrix3 operator-(Matrix3 a, Matrix3 b) => a + -b;

        public static Matrix3 operator *(Matrix3 a, float scalar)
        {
            return new(
                a.M00 + scalar, a.M01 + scalar, a.M02 + scalar,
                a.M10 + scalar, a.M11 + scalar, a.M12 + scalar,
                a.M20 + scalar, a.M21 + scalar, a.M22 + scalar
            );
        }

        public static Matrix3 operator*(float scalar, Matrix3 b) => b * scalar;

        public static Vector3 operator *(Matrix3 a, Vector3 b)
        {
            return new(
                b.X * a.M00 + b.Y * a.M01 + b.Z * a.M02,
                b.X * a.M10 + b.Y * a.M11 + b.Z * a.M12,
                b.X * a.M20 + b.Y * a.M21 + b.Z * a.M22
            );
        }

        public static Matrix3 operator *(Matrix3 a, Matrix3 b)
        {
            return new(
                a.M00 * b.M00 + a.M01 * b.M10 + a.M02 * b.M20,
                a.M00 * b.M01 + a.M01 * b.M11 + a.M02 * b.M21,
                a.M00 * b.M02 + a.M01 * b.M12 + a.M02 * b.M22,
        
                a.M10 * b.M00 + a.M11 * b.M10 + a.M12 * b.M20,
                a.M10 * b.M01 + a.M11 * b.M11 + a.M12 * b.M21,
                a.M10 * b.M02 + a.M11 * b.M12 + a.M12 * b.M22,
        
                a.M20 * b.M00 + a.M21 * b.M10 + a.M22 * b.M20,
                a.M20 * b.M01 + a.M21 * b.M11 + a.M22 * b.M21,
                a.M20 * b.M02 + a.M21 * b.M12 + a.M22 * b.M22
            );
        }

        public static bool operator==(Matrix3 a, Matrix3 b) => a.Equals(b);

        public static bool operator!=(Matrix3 a, Matrix3 b) => !a.Equals(b);

        public static explicit operator Matrix(Matrix3 m)
        {
            return new(
                m.M00, m.M01, m.M02, 0f,
                m.M10, m.M11, m.M12, 0f,
                m.M20, m.M21, m.M22, 0f,
                0f, 0f, 0f, 1f
            );
        }
        
        public float M00;
        public float M01;
        public float M02;
        
        public float M10;
        public float M11;
        public float M12;
        
        public float M20;
        public float M21;
        public float M22;

        public Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
        {
            M00 = m00;
            M01 = m01;
            M02 = m02;
            
            M10 = m10;
            M11 = m11;
            M12 = m12;
            
            M20 = m20;
            M21 = m21;
            M22 = m22;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (obj.GetType() != typeof(Matrix3)) return false;
            
            Matrix other = (Matrix) obj;
            return CoreNative.Equals(M00, other.M00) && CoreNative.Equals(M01, other.M01) && CoreNative.Equals(M02, other.M02)
                && CoreNative.Equals(M10, other.M10) && CoreNative.Equals(M11, other.M11) && CoreNative.Equals(M12, other.M12)
                && CoreNative.Equals(M20, other.M20) && CoreNative.Equals(M21, other.M21) && CoreNative.Equals(M22, other.M22);
        }

        public override int GetHashCode()
        {
            HashCode hash = new();

            hash.Add(M00);
            hash.Add(M01);
            hash.Add(M02);

            hash.Add(M10);
            hash.Add(M11);
            hash.Add(M12);

            hash.Add(M20);
            hash.Add(M21);
            hash.Add(M22);

            return hash.ToHashCode();
        }
    }
}
