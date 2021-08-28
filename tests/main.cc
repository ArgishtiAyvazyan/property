/**
 * @file        main.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Unit tests implementations.
 * @date        8/28/2021.
 * @copyright   Copyright (c) 2021
 */

#include <iostream>

#include <gtest/gtest.h>

#include "property.h"

namespace opened
{
struct dummy_object
{
    util::property<dummy_object, int, util::public_get_set> property;
};
}

namespace closed
{
struct dummy_object
{
    util::property<dummy_object, int> property;
};
}

namespace opened_read
{
struct dummy_object
{
    util::property<dummy_object, int, util::public_get> property;
};
}

template <typename T>
constexpr bool is_public_read = requires(T obj, int val)
{
    { obj.property } -> std::convertible_to<int>;
};

template <typename T>
constexpr bool is_public_write = requires(T obj, int val)
{
    obj.property = val;
};


TEST(property_api_testing, access_test)
{
    ASSERT_TRUE(is_public_read<opened::dummy_object>);
    ASSERT_TRUE(is_public_write<opened::dummy_object>);
    ASSERT_TRUE(is_public_read<opened_read::dummy_object>);
    ASSERT_FALSE(is_public_write<opened_read::dummy_object>);
    ASSERT_FALSE(is_public_read<closed::dummy_object>);
    ASSERT_FALSE(is_public_write<closed::dummy_object>);
}

TEST(property_api_testing, value_test)
{
    opened::dummy_object obj;
    obj.property = 12;
    ASSERT_EQ (12, static_cast<int>(obj.property));
    obj.property++;
    ASSERT_EQ (13, static_cast<int>(obj.property));
}

TEST(property_api_testing, copy_test)
{
    opened::dummy_object obj;
    obj.property = 13;
    auto copy { obj };
    ASSERT_EQ (13, static_cast<int>(copy.property));
}

TEST(property_api_testing, copy_assigne_test)
{
    opened::dummy_object obj;
    opened::dummy_object obj2;
    obj.property = 13;
    obj2.property = obj.property;
    ASSERT_EQ (13, static_cast<int>(obj2.property));
}

TEST(property_api_testing, move_test)
{
    opened::dummy_object obj;
    obj.property = 13;
    auto moved { std::move(obj) };
    ASSERT_EQ (13, static_cast<int>(moved.property));
}

TEST(property_api_testing, move_assigne_test)
{
    opened::dummy_object obj;
    opened::dummy_object obj2;
    obj.property = 13;
    obj2.property = std::move(obj.property);
    ASSERT_EQ (13, static_cast<int>(obj2.property));
}

int main(int argc, char** argv)
{
    std::cout << "Property lib testing..." << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
