using System;

namespace Xnor.Core
{
    public struct Matrix
    {
        public static readonly Matrix Identity = new(
            1f, 0f, 0f, 0f,
            0f, 1f, 0f, 0f,
            0f, 0f, 1f, 0f,
            0f, 0f, 0f, 1f
        );

        public static Matrix operator +(Matrix a, Matrix b)
        {
            return new(
                a.M00 + b.M00, a.M01 + b.M01, a.M02 + b.M02, a.M03 + b.M03,
                a.M10 + b.M10, a.M11 + b.M11, a.M12 + b.M12, a.M13 + b.M13,
                a.M20 + b.M20, a.M21 + b.M21, a.M22 + b.M22, a.M23 + b.M23,
                a.M30 + b.M30, a.M31 + b.M31, a.M32 + b.M32, a.M33 + b.M33
            );
        }

        public static Matrix operator -(Matrix m)
        {
            return new(
                -m.M00, -m.M01, -m.M02, -m.M03,
                -m.M10, -m.M11, -m.M12, -m.M13,
                -m.M20, -m.M21, -m.M22, -m.M23,
                -m.M30, -m.M31, -m.M32, -m.M33
            );
        }

        public static Matrix operator-(Matrix a, Matrix b) => a + -b;

        public static Matrix operator *(Matrix a, float scalar)
        {
            return new(
                a.M00 + scalar, a.M01 + scalar, a.M02 + scalar, a.M03 + scalar,
                a.M10 + scalar, a.M11 + scalar, a.M12 + scalar, a.M13 + scalar,
                a.M20 + scalar, a.M21 + scalar, a.M22 + scalar, a.M23 + scalar,
                a.M30 + scalar, a.M31 + scalar, a.M32 + scalar, a.M33 + scalar
            );
        }

        public static Matrix operator*(float scalar, Matrix b) => b * scalar;

        public static Vector3 operator *(Matrix a, Vector3 b)
        {
            return new(
                b.X * a.M00 + b.Y * a.M01 + b.Z * a.M02 + a.M03,
                b.X * a.M10 + b.Y * a.M11 + b.Z * a.M12 + a.M13,
                b.X * a.M20 + b.Y * a.M21 + b.Z * a.M22 + a.M23
            );
        }

        public static Vector4 operator *(Matrix a, Vector4 b)
        {
            return new(
                b.X * a.M00 + b.Y * a.M01 + b.Z * a.M02 + a.M03,
                b.X * a.M10 + b.Y * a.M11 + b.Z * a.M12 + a.M13,
                b.X * a.M20 + b.Y * a.M21 + b.Z * a.M22 + a.M23,
                b.X * a.M30 + b.Y * a.M31 + b.Z * a.M32 + a.M33
            );
        }

        public static Matrix operator *(Matrix a, Matrix b)
        {
            return new(
                a.M00 * b.M00 + a.M01 * b.M10 + a.M02 * b.M20 + a.M03 * b.M30,
                a.M00 * b.M01 + a.M01 * b.M11 + a.M02 * b.M21 + a.M03 * b.M31,
                a.M00 * b.M02 + a.M01 * b.M12 + a.M02 * b.M22 + a.M03 * b.M32,
                a.M00 * b.M03 + a.M01 * b.M13 + a.M02 * b.M23 + a.M03 * b.M33,
        
                a.M10 * b.M00 + a.M11 * b.M10 + a.M12 * b.M20 + a.M13 * b.M30,
                a.M10 * b.M01 + a.M11 * b.M11 + a.M12 * b.M21 + a.M13 * b.M31,
                a.M10 * b.M02 + a.M11 * b.M12 + a.M12 * b.M22 + a.M13 * b.M32,
                a.M10 * b.M03 + a.M11 * b.M13 + a.M12 * b.M23 + a.M13 * b.M33,
        
                a.M20 * b.M00 + a.M21 * b.M10 + a.M22 * b.M20 + a.M23 * b.M30,
                a.M20 * b.M01 + a.M21 * b.M11 + a.M22 * b.M21 + a.M23 * b.M31,
                a.M20 * b.M02 + a.M21 * b.M12 + a.M22 * b.M22 + a.M23 * b.M32,
                a.M20 * b.M03 + a.M21 * b.M13 + a.M22 * b.M23 + a.M23 * b.M33,
        
                a.M30 * b.M00 + a.M31 * b.M10 + a.M32 * b.M20 + a.M33 * b.M30,
                a.M30 * b.M01 + a.M31 * b.M11 + a.M32 * b.M21 + a.M33 * b.M31,
                a.M30 * b.M02 + a.M31 * b.M12 + a.M32 * b.M22 + a.M33 * b.M32,
                a.M30 * b.M03 + a.M31 * b.M13 + a.M32 * b.M23 + a.M33 * b.M33
            );
        }

        public static bool operator==(Matrix a, Matrix b) => a.Equals(b);

        public static bool operator!=(Matrix a, Matrix b) => !a.Equals(b);

        public static explicit operator Matrix3(Matrix m) => new(m.M00, m.M01, m.M02, m.M10, m.M11, m.M12, m.M20, m.M21, m.M22);
        
        public float M00;
        public float M01;
        public float M02;
        public float M03;
        
        public float M10;
        public float M11;
        public float M12;
        public float M13;
        
        public float M20;
        public float M21;
        public float M22;
        public float M23;
        
        public float M30;
        public float M31;
        public float M32;
        public float M33;

        public Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22,
            float m23, float m30, float m31, float m32, float m33)
        {
            M00 = m00;
            M01 = m01;
            M02 = m02;
            M03 = m03;
            
            M10 = m10;
            M11 = m11;
            M12 = m12;
            M13 = m13;
            
            M20 = m20;
            M21 = m21;
            M22 = m22;
            M23 = m23;
            
            M30 = m30;
            M31 = m31;
            M32 = m32;
            M33 = m33;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (obj.GetType() != typeof(Matrix)) return false;

            Matrix other = (Matrix) obj;
            return CoreNative.Equals(M00, other.M00) && CoreNative.Equals(M01, other.M01) && CoreNative.Equals(M02, other.M02) && CoreNative.Equals(M03, other.M03)
                && CoreNative.Equals(M10, other.M10) && CoreNative.Equals(M11, other.M11) && CoreNative.Equals(M12, other.M12) && CoreNative.Equals(M13, other.M13)
                && CoreNative.Equals(M20, other.M20) && CoreNative.Equals(M21, other.M21) && CoreNative.Equals(M22, other.M22) && CoreNative.Equals(M23, other.M23)
                && CoreNative.Equals(M30, other.M30) && CoreNative.Equals(M31, other.M31) && CoreNative.Equals(M32, other.M32) && CoreNative.Equals(M33, other.M33);
        }

        public override int GetHashCode()
        {
            HashCode hash = new();

            hash.Add(M00);
            hash.Add(M01);
            hash.Add(M02);
            hash.Add(M03);

            hash.Add(M10);
            hash.Add(M11);
            hash.Add(M12);
            hash.Add(M13);

            hash.Add(M20);
            hash.Add(M21);
            hash.Add(M22);
            hash.Add(M23);

            hash.Add(M30);
            hash.Add(M31);
            hash.Add(M32);
            hash.Add(M33);

            return hash.ToHashCode();
        }
    }
}
