#pragma once
#include <chrono>
#include <random>

namespace templates
{
    //-' GladiatorVS
    template <class... Durations, class DurationIn>
    std::tuple<Durations...> break_down_durations(DurationIn d) //-' Время работы приложения
    {
        std::tuple<Durations...> retval;
        using discard = int[];
        (void)discard
        {
            0, (
                void
                    (
                        (
                            (
                                std::get<Durations>(retval) = std::chrono::duration_cast<Durations>(d)
                            ),
                            (
                                d -= std::chrono::duration_cast<DurationIn>
                                (
                                    std::get<Durations>(retval)
                                )
                            )
                        )
                    ),
                0
                )
             ...
        };

        return retval;
    };

    /*
        template using:
        ... = templates::break_down_durations<std::chrono::hours, std::chrono::minutes, std::chrono::seconds>(elapsed);
    */

    //-' ForserX
    enum class PropertyType
    {
        ReadOnly,
        ReadWrite,
        WriteOnly
    };

    template <typename T, PropertyType Type = PropertyType::ReadWrite>
    class Property
    {
    private:
        T& value;

    public:
        Property() : value(nullptr) {}
        Property(T& RVal) : value(RVal) {}

        ~Property() = default;

        operator const T() const
        {
            static_assert(Type != PropertyType::WriteOnly, "Error: [Property] Set not support");
            return value;
        };

        operator T&()
        {
            static_assert(Type == PropertyType::ReadWrite, "Error: [Property] Set not support");
            return value;
        };

        void operator=(const T& RVal)
        {
            static_assert(Type != PropertyType::ReadOnly, "Error: [Property] Set not support");
            *value = RVal;
        }
    };

    /*
        template using:
        int x = 0;
        Property<int> as = x;
        as += 2;
    */

    template <typename K, class V, class Hasher = std::hash<K>, class Traits = std::equal_to<K>, typename allocator = xalloc<std::pair<const K, V>>>

    using xr_unordered_map = std::unordered_map<K, V, Hasher, Traits, allocator>;
}