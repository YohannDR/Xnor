#include "pch.hpp"

TEST(Utils, IntToPointer)

{
    EXPECT_EQ(Utils::IntToPointer<char*>(0), nullptr);
}

TEST(Utils, HumanizeString)
{
    EXPECT_EQ(Utils::HumanizeString("stringToHumanize"), "String To Humanize");
    EXPECT_EQ(Utils::HumanizeString("RequiresUIReload"), "Requires UI Reload");
}

struct A
{
    int32_t i = 1;

    A() = default;

    explicit A(const int32_t i) : i(i) {}

    virtual ~A() = default;
};

struct B : A
{
    B() : A(2) {}
};

TEST(Utils, DynamicPointerCast)
{
    Pointer<B> b = Pointer<B>::New();
    EXPECT_EQ(b->i, 2);
    const Pointer<A> a = Utils::DynamicPointerCast<A>(b);
    EXPECT_EQ(a->i, 2);
    b = Utils::DynamicPointerCast<B>(Pointer<A>::New());
    EXPECT_FALSE(b.IsValid());
}
