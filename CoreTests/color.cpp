#include "pch.hpp"

TEST(Color, ConversionRgbaHsva)
{
    {
        constexpr Color rgba = Color::Red();
        constexpr ColorHsva hsva = ColorHsva::Red();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::Green();
        constexpr ColorHsva hsva = ColorHsva::Green();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::Blue();
        constexpr ColorHsva hsva = ColorHsva::Blue();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::Yellow();
        constexpr ColorHsva hsva = ColorHsva::Yellow();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::Magenta();
        constexpr ColorHsva hsva = ColorHsva::Magenta();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::LightBlue();
        constexpr ColorHsva hsva = ColorHsva::LightBlue();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::Black();
        constexpr ColorHsva hsva = ColorHsva::Black();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
    {
        constexpr Color rgba = Color::White();
        constexpr ColorHsva hsva = ColorHsva::White();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<Color>(hsva), rgba);
    }
}

TEST(Color, EqualityRgba)
{
    EXPECT_EQ(Color::LightBlue(), Color::LightBlue());
}

TEST(Color, EqualityHsva)
{
    EXPECT_EQ(ColorHsva::LightBlue(), ColorHsva::LightBlue());
}
