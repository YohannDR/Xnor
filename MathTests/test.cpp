#include "pch.hpp"

// ReSharper disable CppNoDiscardExpression
#include <chrono>
#include <functional>
#include <numeric>

#include "math.hpp"

#pragma warning(push)
#pragma warning(disable: 4834)

namespace TestCalc
{
    TEST(calc, Sign)
    {
        EXPECT_EQ(Calc::Sign(17.8f), 1.f);
        EXPECT_EQ(Calc::Sign(-86.1f), -1.f);
        EXPECT_EQ(Calc::Sign(0.f), 1.f);
    }

    TEST(calc, Abs)
    {
        EXPECT_EQ(Calc::Abs(17.8f), 17.8f);
        EXPECT_EQ(Calc::Abs(-86.1f), 86.1f);
        EXPECT_EQ(Calc::Abs(0.f), 0.f);
    }

    TEST(calc, IsZero)
    {
        EXPECT_TRUE(Calc::IsZero(Calc::Zero));
        EXPECT_TRUE(Calc::IsZero(0.000001f));
    }

    TEST(calc, Equals)
    {
        EXPECT_TRUE(Calc::Equals(1.f, 1.00000075f));
        EXPECT_FALSE(Calc::Equals(1.f, 1.0000075f));
    }
}

namespace TestVector2
{
    constexpr Vector2 Zero = Vector2::Zero();
    constexpr Vector2 UnitX = Vector2::UnitX();
    constexpr Vector2 UnitY = Vector2::UnitY();

    TEST(Vector2, Constants)
    {
        EXPECT_EQ(Zero, Vector2());
    
        EXPECT_EQ(UnitX, Vector2(1.f, 0.f));
        EXPECT_EQ(UnitY, Vector2(0.f, 1.f));
    }

    constexpr Vector2 X4(4.f, 0.f);
    constexpr Vector2 Y4(0.f, 4.f);
    
    TEST(Vector2, LengthAndNormalization)
    {
        EXPECT_EQ(X4.SquaredLength(), 16.f);
        EXPECT_EQ(Y4.Length(), 4.f);

        EXPECT_EQ(Zero.SquaredLength(), 0.f);
        EXPECT_EQ(Zero.Length(), 0.f);
    
        EXPECT_EQ(X4.Normalized().SquaredLength(), 1.f);
        EXPECT_EQ(Y4.Normalized().Length(), 1.f);
    }

    TEST(Vector2, Normal)
    {
        bool check = UnitX.Normal() == UnitY || UnitX.Normal() == -UnitY;
        EXPECT_TRUE(check);
        check = UnitY.Normal() == UnitX || UnitY.Normal() == -UnitX;
        EXPECT_TRUE(check);
    }

    TEST(Vector2, DotProduct)
    {
        EXPECT_EQ(Vector2::Dot(UnitX, UnitY), 0.f);
        EXPECT_EQ(Vector2::Dot(UnitX, UnitX), 1.f);
        EXPECT_EQ(Vector2::Dot(UnitX, -UnitX), -1.f);
    }

    TEST(Vector2, Lerp)
    {
        EXPECT_EQ(Vector2::Lerp(Vector2::Zero(), Vector2(1.f), 0.5f), Vector2(0.5f));
    }
    
    TEST(Vector2, SubscriptOutOfRangeThrow)
    {
        EXPECT_THROW(UnitX[2], std::out_of_range);
    }

    TEST(Vector2, Checks)
    {
        EXPECT_TRUE((Vector2(1.f) / 0.f).IsInfinity());
        EXPECT_TRUE((-(Vector2(-1.f) / 0.f)).IsInfinity());
        EXPECT_TRUE((Zero / 0.f).IsNaN());
    }

    TEST(Vector2, Operators)
    {
        EXPECT_EQ(UnitX + UnitY, Vector2(1.f));
        EXPECT_EQ(UnitX - UnitX, Vector2(0.f));

        EXPECT_EQ(UnitX * 2.f, Vector2(2.f, 0.f));
        EXPECT_EQ(Vector2(1.f) * Vector2(2.f, 0.5f), Vector2(2.f, 0.5f));

        EXPECT_EQ(UnitX / 2.f, Vector2(0.5f, 0.f));
        EXPECT_EQ(Vector2(1.f) / Vector2(2.f, 0.5f), Vector2(0.5f, 2.f));

        Vector2 temp = UnitX;
        EXPECT_EQ(temp += UnitY, Vector2(1.f));
        EXPECT_EQ(temp -= UnitX, UnitY);

        temp = Vector2(1.f);
        EXPECT_EQ(temp *= 2.f, Vector2(2.f));
        EXPECT_EQ(temp *= Vector2(2.f, 0.5f), Vector2(4.f, 1.f));

        EXPECT_EQ(temp /= 2.f, Vector2(2.f, 0.5f));
        EXPECT_EQ(temp /= Vector2(2.f, 0.5f), Vector2(1.f));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Vector2, Formatting)
    {
        EXPECT_EQ(std::format("{0:06.3f}", UnitX), "01.000 ; 00.000");
    }
#endif
}

namespace TestVector2i
{
    constexpr Vector2i Zero = Vector2i::Zero();
    constexpr Vector2i UnitX = Vector2i::UnitX();
    constexpr Vector2i UnitY = Vector2i::UnitY();

    constexpr Vector2i X4(4, 0);
    constexpr Vector2i Y4(0, 4);

    TEST(Vector2i, Constants)
    {
        EXPECT_EQ(Vector2i::Zero(), Vector2i());
    
        EXPECT_EQ(UnitX, Vector2i(1, 0));
        EXPECT_EQ(UnitY, Vector2i(0, 1));
    }
    
    TEST(Vector2i, LengthAndNormalization)
    {
        EXPECT_EQ(X4.SquaredLength(), 16);
        EXPECT_EQ(Y4.Length(), 4);

        EXPECT_EQ(Zero.SquaredLength(), 0.f);
        EXPECT_EQ(Zero.Length(), 0.f);
    
        EXPECT_EQ(X4.Normalized().SquaredLength(), 1);
        EXPECT_EQ(Y4.Normalized().Length(), 1);
    }

    TEST(Vector2i, Normal)
    {
        bool check = UnitX.Normal() == static_cast<Vector2>(UnitY) || UnitX.Normal() == static_cast<Vector2>(-UnitY);
        EXPECT_TRUE(check);
        check = UnitY.Normal() == static_cast<Vector2>(UnitX) || UnitY.Normal() == static_cast<Vector2>(-UnitX);
        EXPECT_TRUE(check);
    }

    TEST(Vector2i, DotProduct)
    {
        EXPECT_EQ(Vector2i::Dot(UnitX, UnitY), 0.f);
        EXPECT_EQ(Vector2i::Dot(UnitX, UnitX), 1.f);
        EXPECT_EQ(Vector2i::Dot(UnitX, -UnitX), -1.f);
    }
    
    TEST(Vector2i, SubscriptOutOfRangeThrow)
    {
        EXPECT_THROW(UnitX[2], std::out_of_range);
    }

    TEST(Vector2i, Operators)
    {
        EXPECT_EQ(UnitX + UnitY, Vector2i(1));
        EXPECT_EQ(UnitX - UnitX, Vector2i(0));
        
        EXPECT_EQ(UnitX * 2, Vector2i(2, 0));
        EXPECT_EQ(Vector2i(1) * Vector2i(2, 0), Vector2i(2, 0));

        Vector2i temp = UnitX;
        EXPECT_EQ(temp += UnitY, Vector2i(1));
        EXPECT_EQ(temp -= UnitX, UnitY);
        
        temp = Vector2i(1);
        EXPECT_EQ(temp *= 2, Vector2i(2));
        EXPECT_EQ(temp *= Vector2i(2, 0), Vector2i(4, 0));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Vector2i, Formatting)
    {
        EXPECT_EQ(std::format("{0:02d}", UnitX), "01 ; 00");
    }
#endif
}

namespace TestVector3
{
    constexpr Vector3 Zero = Vector3::Zero();
    constexpr Vector3 UnitX = Vector3::UnitX();
    constexpr Vector3 UnitY = Vector3::UnitY();
    constexpr Vector3 UnitZ = Vector3::UnitZ();

    constexpr Vector3 X4(4.f, 0.f, 0.f);
    constexpr Vector3 Y4(0.f, 4.f, 0.f);

    TEST(Vector3, Constants)
    {
        EXPECT_EQ(Vector3::Zero(), Vector3());
    
        EXPECT_EQ(UnitX, Vector3(1.f, 0.f, 0.f));
        EXPECT_EQ(UnitY, Vector3(0.f, 1.f, 0.f));
        EXPECT_EQ(UnitZ, Vector3(0.f, 0.f, 1.f));
    }

    TEST(Vector3, LengthAndNormalization)
    {
        EXPECT_EQ(X4.SquaredLength(), 16.f);
        EXPECT_EQ(Y4.Length(), 4.f);

        EXPECT_EQ(Zero.SquaredLength(), 0.f);
        EXPECT_EQ(Zero.Length(), 0.f);
    
        EXPECT_EQ(X4.Normalized().SquaredLength(), 1.f);
        EXPECT_EQ(Y4.Normalized().Length(), 1.f);
    }

    TEST(Vector3, DotProduct)
    {
        EXPECT_EQ(Vector3::Dot(UnitX, UnitY), 0.f);
        EXPECT_EQ(Vector3::Dot(UnitX, UnitX), 1.f);
        EXPECT_EQ(Vector3::Dot(UnitX, -UnitX), -1.f);
    }

    TEST(Vector3, Lerp)
    {
        EXPECT_EQ(Vector3::Lerp(Vector3::Zero(), Vector3(1.f), 0.5f), Vector3(0.5f));
    }

    TEST(Vector3, SubscriptOutOfRangeThrow)
    {
        EXPECT_THROW(UnitX[3], std::out_of_range);
    }

    TEST(Vector3, Checks)
    {
        EXPECT_TRUE((Vector3(1.f) / 0.f).IsInfinity());
        EXPECT_TRUE((-(Vector3(-1.f) / 0.f)).IsInfinity());
        EXPECT_TRUE((Zero / 0.f).IsNaN());
    }

    TEST(Vector3, Operators)
    {
        EXPECT_EQ(UnitX + UnitY, Vector3(1.f, 1.f, 0.f));
        EXPECT_EQ(UnitX - UnitX, Vector3(0.f));
        
        EXPECT_EQ(UnitX * 2.f, Vector3(2.f, 0.f, 0.f));
        EXPECT_EQ(Vector3(1.f) * Vector3(2.f, 0.5f, 0.f), Vector3(2.f, 0.5f, 0.f));
        
        EXPECT_EQ(UnitX / 2.f, Vector3(0.5f, 0.f, 0.f));
        EXPECT_EQ(Vector3(1.f) / Vector3(2.f, 0.5f, 1.f), Vector3(0.5f, 2.f, 1.f));

        Vector3 temp = UnitX;
        EXPECT_EQ(temp += UnitY, Vector3(1.f, 1.f, 0.f));
        EXPECT_EQ(temp -= UnitX, UnitY);
        
        temp = Vector3(1.f);
        EXPECT_EQ(temp *= 2.f, Vector3(2.f));
        EXPECT_EQ(temp *= Vector3(2.f, 0.5f, 0.f), Vector3(4.f, 1.f, 0.f));
        
        EXPECT_EQ(temp /= 2.f, Vector3(2.f, 0.5f, 0.f));
        EXPECT_EQ(temp /= Vector3(2.f, 0.5f, 1.f), Vector3(1.f, 1.f, 0.f));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Vector3, Formatting)
    {
        EXPECT_EQ(std::format("{0:06.3f}", UnitX), "01.000 ; 00.000 ; 00.000");
    }
#endif
}

namespace TestVector4
{
    constexpr Vector4 Zero = Vector4::Zero();
    constexpr Vector4 UnitX = Vector4::UnitX();
    constexpr Vector4 UnitY = Vector4::UnitY();
    constexpr Vector4 UnitZ = Vector4::UnitZ();
    constexpr Vector4 UnitW = Vector4::UnitW();

    constexpr Vector4 X4(4.f, 0.f, 0.f, 0.f);
    constexpr Vector4 Y4(0.f, 4.f, 0.f, 0.f);

    TEST(Vector4, Constants)
    {
        EXPECT_EQ(Vector4::Zero(), Vector4());
    
        EXPECT_EQ(UnitX, Vector4(1.f, 0.f, 0.f, 0.f));
        EXPECT_EQ(UnitY, Vector4(0.f, 1.f, 0.f, 0.f));
        EXPECT_EQ(UnitZ, Vector4(0.f, 0.f, 1.f, 0.f));
        EXPECT_EQ(UnitW, Vector4(0.f, 0.f, 0.f, 1.f));
    }
    TEST(Vector4, LengthAndNormalization)
    {
        EXPECT_EQ(X4.SquaredLength(), 16.f);
        EXPECT_EQ(Y4.Length(), 4.f);

        EXPECT_EQ(Zero.SquaredLength(), 0.f);
        EXPECT_EQ(Zero.Length(), 0.f);
    
        EXPECT_EQ(X4.Normalized().SquaredLength(), 1.f);
        EXPECT_EQ(Y4.Normalized().Length(), 1.f);
    }

    TEST(Vector4, DotProduct)
    {
        EXPECT_EQ(Vector4::Dot(UnitX, UnitY), 0.f);
        EXPECT_EQ(Vector4::Dot(UnitX, UnitX), 1.f);
        EXPECT_EQ(Vector4::Dot(UnitX, -UnitX), -1.f);
    }

    TEST(Vector4, Lerp)
    {
        EXPECT_EQ(Vector4::Lerp(Vector4::Zero(), Vector4(1.f), 0.5f), Vector4(0.5f));
    }
    
    TEST(Vector4, SubscriptOutOfRangeThrow)
    {
        EXPECT_THROW(UnitX[4], std::out_of_range);
    }

    TEST(Vector4, Checks)
    {
        EXPECT_TRUE((Vector4(1.f) / 0.f).IsInfinity());
        EXPECT_TRUE((-(Vector4(-1.f) / 0.f)).IsInfinity());
        EXPECT_TRUE((Zero / 0.f).IsNaN());
    }

    TEST(Vector4, Operators)
    {
        EXPECT_EQ(UnitX + UnitY, Vector4(1.f, 1.f, 0.f, 0.f));
        EXPECT_EQ(UnitX - UnitX, Vector4(0.f));
        
        EXPECT_EQ(UnitX * 2.f, Vector4(2.f, 0.f, 0.f, 0.f));
        EXPECT_EQ(Vector4(1.f) * Vector4(2.f, 0.5f, 0.f, 0.f), Vector4(2.f, 0.5f, 0.f, 0.f));
        
        EXPECT_EQ(UnitX / 2.f, Vector4(0.5f, 0.f, 0.f, 0.f));
        EXPECT_EQ(Vector4(1.f) / Vector4(2.f, 0.5f, 1.f, 1.f), Vector4(0.5f, 2.f, 1.f, 1.f));

        Vector4 temp = UnitX;
        EXPECT_EQ(temp += UnitY, Vector4(1.f, 1.f, 0.f, 0.f));
        EXPECT_EQ(temp -= UnitX, UnitY);
        
        temp = Vector4(1.f);
        EXPECT_EQ(temp *= 2.f, Vector4(2.f));
        EXPECT_EQ(temp *= Vector4(2.f, 0.5f, 0.f, 0.f), Vector4(4.f, 1.f, 0.f, 0.f));
        
        EXPECT_EQ(temp /= 2.f, Vector4(2.f, 0.5f, 0.f, 0.f));
        EXPECT_EQ(temp /= Vector4(2.f, 0.5f, 1.f, 1.f), Vector4(1.f, 1.f, 0.f, 0.f));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Vector4, Formatting)
    {
        EXPECT_EQ(std::format("{0:06.3f}", UnitX), "01.000 ; 00.000 ; 00.000 ; 00.000");
    }
#endif
}

namespace TestQuaternion
{
    constexpr Quaternion Zero = Quaternion::Zero();
    constexpr Quaternion UnitX = Quaternion::UnitX();
    constexpr Quaternion UnitY = Quaternion::UnitY();
    constexpr Quaternion UnitZ = Quaternion::UnitZ();
    constexpr Quaternion UnitW = Quaternion::UnitW();

    constexpr Quaternion X4(4.f, 0.f, 0.f, 0.f);
    constexpr Quaternion Y4(0.f, 4.f, 0.f, 0.f);

    const Quaternion RotationHalfCircleZ = Quaternion::FromAxisAngle(Vector3::UnitZ(), Calc::PiOver2);
    const Vector3 RotatedUnitX = Quaternion::Rotate(Vector3::UnitX(), RotationHalfCircleZ);

    TEST(Quaternion, Constants)
    {
        EXPECT_EQ(Quaternion::Zero(), Quaternion());
    
        EXPECT_EQ(UnitX, Quaternion(1.f, 0.f, 0.f, 0.f));
        EXPECT_EQ(UnitY, Quaternion(0.f, 1.f, 0.f, 0.f));
        EXPECT_EQ(UnitZ, Quaternion(0.f, 0.f, 1.f, 0.f));
        EXPECT_EQ(UnitW, Quaternion(0.f, 0.f, 0.f, 1.f));
    
        EXPECT_EQ(UnitW, Quaternion::Identity());
    }

    TEST(Quaternion, LengthAndNormalization)
    {
        EXPECT_EQ(X4.SquaredLength(), 16.f);
        EXPECT_EQ(Y4.Length(), 4.f);

        EXPECT_EQ(Zero.SquaredLength(), 0.f);
        EXPECT_EQ(Zero.Length(), 0.f);
    
        EXPECT_EQ(X4.Normalized().SquaredLength(), 1.f);
        EXPECT_EQ(Y4.Normalized().Length(), 1.f);
    }

    TEST(Quaternion, DotProduct)
    {
        EXPECT_EQ(Quaternion::Dot(UnitX, UnitY), 0.f);
        EXPECT_EQ(Quaternion::Dot(UnitX, UnitX), 1.f);
        EXPECT_EQ(Quaternion::Dot(UnitX, -UnitX), -1.f);
    }

    TEST(Quaternion, Lerp)
    {
        EXPECT_EQ(Quaternion::Lerp(Quaternion::Zero(), Quaternion(1.f), 0.5f), Quaternion(0.5f));
        EXPECT_EQ(Quaternion::Slerp(Quaternion::Zero(), Quaternion(1.f), 0.5f), Quaternion(0.707107f));
    }

    TEST(Quaternion, SubscriptOutOfRangeThrow)
    {
        EXPECT_THROW(UnitX[4], std::out_of_range);
    }

    TEST(Quaternion, Checks)
    {
        EXPECT_TRUE((Quaternion(1.f) / 0.f).IsInfinity());
        EXPECT_TRUE((-(Quaternion(-1.f) / 0.f)).IsInfinity());
        EXPECT_TRUE((Zero / 0.f).IsNaN());
    }

    TEST(Quaternion, Convertions)
    {
        EXPECT_EQ(Quaternion::ToEuler(RotationHalfCircleZ), Vector3(0.f, 0.f, Calc::PiOver2));
    }

    TEST(Quaternion, Rotation)
    {
        EXPECT_EQ(RotatedUnitX, Vector3::UnitY());
        
        EXPECT_EQ(Quaternion(RotatedUnitX), RotationHalfCircleZ * Vector3::UnitX() * RotationHalfCircleZ.Conjugate());
        
        EXPECT_EQ(RotationHalfCircleZ, Quaternion::FromEuler(Vector3(0.f, 0.f, Calc::PiOver2)));
        EXPECT_EQ(RotationHalfCircleZ, Quaternion::FromRotationMatrix(Matrix::RotationZ(Calc::PiOver2)));
    }

    TEST(Quaternion, Inversion)
    {
        EXPECT_EQ(Quaternion::Rotate(RotatedUnitX, RotationHalfCircleZ.Inverted()), Vector3::UnitX());
    }

    TEST(Quaternion, Operators)
    {
        EXPECT_EQ(UnitX + UnitY, Quaternion(1.f, 1.f, 0.f, 0.f));
        EXPECT_EQ(UnitX - UnitX, Quaternion(0.f));
        
        EXPECT_EQ(UnitX * 2.f, Quaternion(2.f, 0.f, 0.f, 0.f));
        EXPECT_EQ(Quaternion(2.f, 3.f, 4.f, 1.f) * Quaternion(6.f, 7.f, 8.f, 5.f), Quaternion(12.f, 30.f, 24.f, -60.f));
        EXPECT_EQ(Quaternion(6.f, 7.f, 8.f, 5.f) * Quaternion(2.f, 3.f, 4.f, 1.f), Quaternion(20.f, 14.f, 32.f, -60.f));
        
        EXPECT_EQ(UnitX / 2.f, Quaternion(0.5f, 0.f, 0.f, 0.f));

        Quaternion temp = UnitX;
        EXPECT_EQ(temp += UnitY, Quaternion(1.f, 1.f, 0.f, 0.f));
        EXPECT_EQ(temp -= UnitX, UnitY);
        
        temp = Quaternion(1.f);
        EXPECT_EQ(temp *= 2.f, Quaternion(2.f));
        temp = Quaternion(2.f, 3.f, 4.f, 1.f);
        EXPECT_EQ(temp *= Quaternion(6.f, 7.f, 8.f, 5.f), Quaternion(12.f, 30.f, 24.f, -60.f));
        temp = Quaternion(6.f, 7.f, 8.f, 5.f);
        EXPECT_EQ(temp *= Quaternion(2.f, 3.f, 4.f, 1.f), Quaternion(20.f, 14.f, 32.f, -60.f));
        
        temp = Quaternion(1.f);
        EXPECT_EQ(temp /= 2.f, Quaternion(0.5f));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Quaternion, Formatting)
    {
        EXPECT_EQ(std::format("{0:06.3f}", UnitX), "01.000 ; 00.000 ; 00.000 ; 00.000");
    }
#endif
}

namespace TestMatrix3
{
    constexpr Matrix3 Zero = Matrix3();
    constexpr Matrix3 Identity = Matrix3::Identity();

    constexpr Vector3 One(1.f);
    const Matrix3 RotationHalfCircleZ = Matrix3::RotationZ(Calc::PiOver2);

    constexpr Matrix3 Symmetric(
            1.f, 2.f, 3.f,
            2.f, 4.f, 5.f,
            3.f, 5.f, 6.f
    );
    constexpr Matrix3 Antisymmetric(
            1.f, 2.f, 3.f,
            -2.f, 4.f, 5.f,
            -3.f, -5.f, 6.f
    );

    constexpr Vector3 OneTwoThree(1.f, 2.f, 3.f);

    TEST(Matrix3, Constants)
    {
        EXPECT_EQ(Identity, Matrix3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f));
    }

    TEST(Matrix3, CheckFunctions)
    {
        EXPECT_TRUE(Identity.IsDiagonal());
        EXPECT_FALSE(RotationHalfCircleZ.IsDiagonal());

        EXPECT_TRUE(Identity.IsIdentity());
        EXPECT_FALSE(Zero.IsIdentity());
        EXPECT_FALSE(RotationHalfCircleZ.IsIdentity());

        EXPECT_TRUE(Zero.IsNull());
        EXPECT_FALSE(RotationHalfCircleZ.IsNull());

        EXPECT_TRUE(Identity.IsSymmetric());
        EXPECT_TRUE(Symmetric.IsSymmetric());
        EXPECT_FALSE(Antisymmetric.IsSymmetric());

        EXPECT_TRUE(Identity.IsAntisymmetric());
        EXPECT_TRUE(Antisymmetric.IsAntisymmetric());
        EXPECT_FALSE(Symmetric.IsAntisymmetric());
    }

    TEST(Matrix3, SmallFunctions)
    {
        EXPECT_EQ(Identity.Diagonal(), Vector3(1.f));

        EXPECT_EQ(Identity.Determinant(), 1.f);
        EXPECT_EQ(RotationHalfCircleZ.Determinant(), 1.f);
        EXPECT_EQ(Zero.Determinant(), 0.f);

        EXPECT_EQ(Identity, Identity.Transposed());
    }

    TEST(Matrix3, Multiplication)
    {
        Matrix3 a = Matrix3::Scaling(Vector3(2.f));

        EXPECT_EQ(a * a, Matrix3::Scaling(Vector3(4.f)));

        constexpr Matrix3 b = Matrix3::Scaling(Vector3(-5.f));

        Matrix3 ab = a * b;

        EXPECT_EQ(ab * One, Vector3(-10.f));
    }

    TEST(Matrix3, Inversion)
    {
        EXPECT_EQ(RotationHalfCircleZ * RotationHalfCircleZ.Inverted(), Identity);

        constexpr Matrix3 temp(
            1.f, 1.f, 0.f,
            1.f, 1.f, 0.f,
            0.f, 0.f, 0.f
        );

        EXPECT_THROW(temp.Inverted(), std::invalid_argument);
    }

    TEST(Matrix3, Rotation)
    {
        EXPECT_EQ(Matrix3::RotationZ(Calc::PiOver2) * Vector3::UnitX(), Vector3::UnitY());
    }

    TEST(Matrix3, Scaling)
    {
        EXPECT_EQ(Matrix3::Scaling(OneTwoThree) * One, OneTwoThree);
    }

    TEST(Matrix3, Subscript)
    {
        EXPECT_THROW(Zero.At(3, 0), std::out_of_range);
        EXPECT_THROW(Zero.At(0, 3), std::out_of_range);
        EXPECT_THROW(Zero.At(3, 3), std::out_of_range);
        EXPECT_NO_THROW(Zero.At(1, 2));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Matrix3, Formatting)
    {
        EXPECT_EQ(std::format("{0:06.3f}", Identity), "[ 01.000 ; 00.000 ; 00.000 ] [ 00.000 ; 01.000 ; 00.000 ] [ 00.000 ; 00.000 ; 01.000 ]");
        EXPECT_EQ(std::format("{0:m06.3f}", Identity), "[ 01.000 ; 00.000 ; 00.000 ]\n[ 00.000 ; 01.000 ; 00.000 ]\n[ 00.000 ; 00.000 ; 01.000 ]");
    }
#endif
}

namespace TestMatrix
{
    constexpr Matrix Zero = Matrix();
    constexpr Matrix Identity = Matrix::Identity();

    constexpr Vector3 One(1.f);
    const Matrix RotationHalfCircleZ = Matrix::RotationZ(Calc::PiOver2);
    const Matrix Trs = Matrix::Trs(One, RotationHalfCircleZ, Vector3(2.f));

    constexpr Matrix Symmetric(
        1.f, 2.f, 3.f, 4.f,
        2.f, 5.f, 6.f, 7.f,
        3.f, 6.f, 8.f, 9.f,
        4.f, 7.f, 9.f, 10.f
    );
    constexpr Matrix Antisymmetric(
        1.f, 2.f, 3.f, 4.f,
       -2.f, 5.f, 6.f, 7.f,
       -3.f,-6.f, 8.f, 9.f,
       -4.f,-7.f,-9.f, 10.f
    );

    constexpr Vector3 OneTwoThree(1.f, 2.f, 3.f);

    TEST(Matrix, Constants)
    {
        EXPECT_EQ(Identity, Matrix(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f));
    }

    TEST(Matrix, CheckFunctions)
    {
        EXPECT_TRUE(Identity.IsDiagonal());
        EXPECT_FALSE(Trs.IsDiagonal());

        EXPECT_TRUE(Identity.IsIdentity());
        EXPECT_FALSE(Zero.IsIdentity());
        EXPECT_FALSE(Trs.IsIdentity());

        EXPECT_TRUE(Zero.IsNull());
        EXPECT_FALSE(Trs.IsNull());
            
        EXPECT_TRUE(Identity.IsSymmetric());
        EXPECT_TRUE(Symmetric.IsSymmetric());
        EXPECT_FALSE(Antisymmetric.IsSymmetric());
        EXPECT_FALSE(Trs.IsSymmetric());
            
        EXPECT_TRUE(Identity.IsAntisymmetric());
        EXPECT_TRUE(Antisymmetric.IsAntisymmetric());
        EXPECT_FALSE(Symmetric.IsAntisymmetric());
        EXPECT_FALSE(Trs.IsAntisymmetric());
    }

    TEST(Matrix, SmallFunctions)
    {
        EXPECT_EQ(Identity.Diagonal(), Vector4(1.f));
            
        EXPECT_EQ(Identity.Determinant(), 1.f);
        EXPECT_EQ(Trs.Determinant(), 8.f);
        EXPECT_EQ(Zero.Determinant(), 0.f);

        EXPECT_EQ(Identity, Identity.Transposed());
    }

    TEST(Matrix, Multiplication)
    {
        constexpr Matrix t = Matrix::Translation(One);

        EXPECT_EQ(t * t, Matrix::Translation(Vector3(2.f)));

        constexpr Matrix s = Matrix::Scaling(Vector3(-5.f));

        constexpr Matrix ts = t * s;

        EXPECT_EQ(ts * One, Vector3(-4.f));
    }

    TEST(Matrix, Inversion)
    {
        EXPECT_EQ(Trs * Trs.Inverted(), Identity);

        constexpr Matrix temp(
            1.f, 2.f, 3.f, 4.f,
            5.f, 6.f, 7.f, 8.f,
            9.f, 10.f, 11.f, 12.f,
            13.f, 14.f, 15.f, 16.f
        );

        EXPECT_THROW(temp.Inverted(), std::invalid_argument);
    }

    TEST(Matrix, Translation)
    {
        EXPECT_EQ(Matrix::Translation(OneTwoThree) * One, Vector3(2.f, 3.f, 4.f));
    }

    TEST(Matrix, Rotation)
    {
        EXPECT_EQ(Matrix::RotationZ(Calc::PiOver2) * Vector3::UnitX(), Vector3::UnitY());
    }

    TEST(Matrix, Scaling)
    {
        EXPECT_EQ(Matrix::Scaling(OneTwoThree) * One, OneTwoThree);
    }

    TEST(Matrix, TRS)
    {
        EXPECT_EQ(Matrix::Trs(One, Vector3(0.f, 0.f, Calc::PiOver2), Vector3(2.f)) * One, Vector3(-1.f, 3.f, 3.f));
    }

    TEST(Matrix, Subscript)
    {
        EXPECT_THROW(Zero.At(4, 0), std::out_of_range);
        EXPECT_THROW(Zero.At(0, 4), std::out_of_range);
        EXPECT_THROW(Zero.At(4, 4), std::out_of_range);
        EXPECT_NO_THROW(Zero.At(1, 2));
    }

#ifdef MATH_DEFINE_FORMATTER
    TEST(Matrix, Formatting)
    {
        EXPECT_EQ(std::format("{0:m06.3f}", Identity), "[ 01.000 ; 00.000 ; 00.000 ; 00.000 ] [ 00.000 ; 01.000 ; 00.000 ; 00.000 ] [ 00.000 ; 00.000 ; 01.000 ; 00.000 ] [ 00.000 ; 00.000 ; 00.000 ; 01.000 ]");
        EXPECT_EQ(std::format("{0:06.3f}", Identity), "[ 01.000 ; 00.000 ; 00.000 ; 00.000 ]\n[ 00.000 ; 01.000 ; 00.000 ; 00.000 ]\n[ 00.000 ; 00.000 ; 01.000 ; 00.000 ]\n[ 00.000 ; 00.000 ; 00.000 ; 01.000 ]");
    }
#endif
}

#pragma warning(pop)
