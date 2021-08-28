/**
 * @file        property.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration and implementations of property class.
 * @date        8/28/2021.
 * @copyright   Copyright (c) 2021
 */

#ifndef PROPERTY_PROPERTY_H
#define PROPERTY_PROPERTY_H

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace util {
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// The access policies.
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Declare only a private get and set accessors.
 */
class private_get_set
{ };

/**
 * Declare only a public get accessor and private set.
 */
class public_get
{ };

/**
 * Declare only a public get and set accessors.
 */
class public_get_set
{ };
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace impl {
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @internal
 * @class       data_storage
 * @brief       The internal class data_storage is implemented for storing property value
 *              and prevent user direct access to data.
 *
 * @tparam T    The value type.
 */
template <typename T>
class data_storage
{
protected:
    explicit data_storage(T&& value) noexcept(noexcept(T{std::forward<T>(value)}))
        : m_value { std::forward<T>(value) }
    {
    }

    ~data_storage() noexcept = default;
    data_storage(data_storage&&) noexcept = default;
    data_storage(const data_storage&) noexcept = default;
    data_storage& operator=(data_storage&&) noexcept = default;
    data_storage& operator=(const data_storage&) noexcept = default;

    [[nodiscard]] T& value() noexcept
    {
        return m_value;
    }

    [[nodiscard]] const T& value() const noexcept
    {
        return m_value;
    }

private:
    /*
     * The stored value.
     */
    T m_value;
}; // class data_storage
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper concepts.
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
concept is_access_policy =
       std::is_same_v<T, private_get_set>
    || std::is_same_v<T, public_get>
    || std::is_same_v<T, public_get_set>
;

////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace impl
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class          property
 * @brief          In cases where property accessors do not need to use additional logic,
 *                 properties allow for more concise property declarations. The public or
 *                 private accessors will be generated automatically.
 * @details        The properties are members that provide a flexible reading, writing
 *                 mechanism. Properties can be used as if they are public data members,
 *                 and automatically generates public or private access methods.
 * @example        struct dummy_object
 *                 {
 *                     template <class ... TArgs>
 *                     using property_t = util::property <dummy_object, TArgs...>;
 *                     // The property can change anyone.
 *                     property_t<int, util::public_get_set> public_read_write;
 *                     // The property can read anyone but change only inside of dummy_object class.
 *                     property_t<int, util::public_get> public_read;
 *                     // The access to this object has only inside of dummy_object class.
 *                     property_t<int> private_property;
 *                 };
 *                 // Use example.
 *                 dummy_object obj;
 *                 int val = obj.public_read_write; // Ok.
 *                 obj.public_read_write = 13;      // Ok.
 *                 val = obj.public_read;           // Ok.
 *                 obj.public_read = 13;            // Compile error.
 *                 val = obj.private_property;      // Compile error.
 *                 obj.private_property = 10;       // Compile error.
 * @tparam TOwner  is the type of owner, which should have all accesses.
 * @tparam TValue  is the type of property value.
 * @tparam TAccessPolicy is the access policy for the property.
 *                 Possible variants
 *                     -# public_get - Declare only a public get accessor and private set.
 *                     -# public_get_set - Declare only a public get and set accessors.
 *                     -# private_get_set - Declare only a private get and set accessors.
 *                 The param is optional default value is private_get_set.
 */
template <typename TOwner, typename TValue, typename TAccessPolicy = private_get_set>
    requires(impl::is_access_policy<TAccessPolicy>)
class property : private impl::data_storage<TValue>
{
    friend TOwner;
    static constexpr bool is_public_get = std::is_same_v<TAccessPolicy, public_get>
                                       || std::is_same_v<TAccessPolicy, public_get_set>;
    static constexpr bool is_public_set = std::is_same_v<TAccessPolicy, public_get_set>;

public:
    property(TValue value = TValue {}) noexcept(noexcept(TValue{std::forward<TValue>(value)}))
        : impl::data_storage<TValue> { std::move(value) }
    {
    }

    ~property() noexcept = default;
    property(property&&) noexcept = default;
    property(const property&) noexcept = default;
    property& operator=(property&&) noexcept = default;
    property& operator=(const property&) noexcept = default;

public:
    operator TValue&() requires(is_public_get && is_public_set)
    {
        return this->value();
    }

public:
    operator const TValue&() requires(is_public_get && !is_public_set)
    {
        return this->value();
    }

private:
    operator TValue&() requires(!is_public_get)
    {
        return this->value();
    }

public:
    TValue& operator=(const TValue& new_value) requires(is_public_set)
    {
        return this->value() = new_value;
    }

private:
    TValue& operator=(const TValue& new_value) requires(!is_public_set)
    {
        return this->value() = new_value;
    }
}; // class property

////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace util
////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PROPERTY_PROPERTY_H
