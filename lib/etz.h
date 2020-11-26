#ifndef ETZ_H
#define ETZ_H


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <cstdint>
#include <ctime>
#include <string>
#include <unordered_map>


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ETZ
{


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Time zone abbreviations.
//
#include "etz-abbreviations.inl"


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Packed rule structure. 8 bytes per rule for all architecture word sizes.
//
class Rule
{
    template <class T> constexpr T abs(const T x) { return x < 0 ? -x : x; }

public:
    constexpr Rule() = default;
    constexpr Rule(const time_t timeStart, const Abbreviation abbreviation, const int32_t gmtOffset, const bool isDST)
        : m_timeStart(static_cast<uint32_t>(abs(timeStart))) // [-4260212373,16720524000]
        , m_gmtOffset(static_cast<uint16_t>(abs(gmtOffset))) // [-57360,54822]
        , m_data(static_cast<uint16_t>(abbreviation) << 6 | isDST << 5 | (gmtOffset < 1) << 4 | (timeStart < 1) << 3 | static_cast<uint16_t>(abs(timeStart) >> 32))
    {
    }

    constexpr bool isValid() const { return m_data != 0; } // the lowest valid Abbreviation ordinal is > 0
    constexpr time_t timeStart() const { return m_data & 0x08 ? -((static_cast<time_t>(m_data & 0x07) << 32) + m_timeStart) : ((static_cast<time_t>(m_data & 0x07) << 32) + m_timeStart); }
    constexpr Abbreviation abbreviation() const { return Abbreviation(m_data >> 6); }
    constexpr int32_t gmtOffset() const { return m_data & 0x10 ? -static_cast<int32_t>(m_gmtOffset) : m_gmtOffset; }
    constexpr bool isDST() const { return m_data & 0x20; }

private:
    uint32_t m_timeStart {};
    uint16_t m_gmtOffset {};

    /**
    *  1
    *  5------7------0
    *  ^^^^^^^^^        abbreviation  | 9 bits | [0,316]
    *           ^       isDST         | 1 bit  |
    *            ^      gmtOffsetSign | 1 bit  |
    *             ^     timeStartSign | 1 bit  |
    *              ^^^  timeStart32   | 3 bits | Bits 32, 33 and 34 from timeStart
                                      | Combined with timeStartSign this allows for [-34359738367,34359738367], we only require [-4260212373,16720524000]
                                      | This schema supports timeStart values into the year 3058
    */
    uint16_t m_data {};
};

static_assert(sizeof(Rule) == 8);


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Time zone names.
//
#include "etz-timezones.inl"


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Time zone rules.
// This file is built by data/create-includes.py which consumes CSV from timezonedb.com (https://timezonedb.com/files/timezonedb.csv.zip)
// timezonedb.com is itself extracted from the well respected IANA database (https://www.iana.org/time-zones)
//
template <typename T, size_t N> constexpr auto Rules(T (&r)[N]) { return std::make_pair(r, static_cast<uint16_t>(N)); }

#include "etz-rules.inl"


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Helper lookup class.
//
class Names
{
    using Map = std::unordered_map<std::string, uint16_t>;

    // Don't use initializer_lists for static initialization as most compilers will generate a temporary variable with a large stack requirement...
    template <typename T, size_t N> constexpr auto makeMap(Map& map, T (&names)[N])
    {
        map.reserve(N);
        for (uint16_t i = 0; i < N; ++i) {
            map.emplace(std::make_pair(names[i], i));
        }
    }

public:
    Names()
    {
        makeMap(m_abbreviations, _AbbreviationNames);
        makeMap(m_timeZones, _TimeZoneNames);
    }

    Abbreviation abbreviation(const std::string& abbreviation)
    {
        const auto it = m_abbreviations.find(abbreviation);
        if (it == m_abbreviations.end()) {
            return Abbreviation::Invalid;
        }
        return Abbreviation(it->second);
    }

    std::string abbreviation(const Abbreviation abbreviation)
    {
        for (const auto& [k, v] : m_abbreviations) {
            if (v == uint16_t(abbreviation)) {
                return k;
            }
        }
    }

    TimeZone timeZone(const std::string& timeZone)
    {
        const auto it = m_timeZones.find(timeZone);
        if (it == m_timeZones.end()) {
            return TimeZone::Invalid;
        }
        return TimeZone(it->second);
    }

    std::string timeZone(const TimeZone timeZone)
    {
        for (const auto& [k, v] : m_timeZones) {
            if (v == uint16_t(timeZone)) {
                return k;
            }
        }
    }

private:
    Map m_abbreviations;
    Map m_timeZones;
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// UTC<->local/civil class.
//
class UTC
{
    static auto ruleLu(const TimeZone timeZone, const std::time_t utc)
    {
        const auto rules = _TimeZoneRules.find(timeZone);
        if (rules == _TimeZoneRules.end()) {
            return Rule();
        }
        // Start with the last rule and work backwards...
        for (auto it = rules->second.first + rules->second.second - 1;; --it) {
            if (it->timeStart() <= utc) {
                return *it;
            }
            if (it == rules->second.first) {
                return Rule(); // no match
            }
        }
    }

public:
    static auto toLocal(const TimeZone timeZone, const std::time_t utc, std::time_t& local)
    {
        const auto rule = ruleLu(timeZone, utc);
        if (!rule.isValid()) {
            return false;
        }
        local = utc + static_cast<std::time_t>(rule.gmtOffset());
        return true;
    }

    static auto fromLocal(const TimeZone timeZone, const std::time_t local, std::time_t& utc)
    {
        const auto rule = ruleLu(timeZone, local);
        if (!rule.isValid()) {
            return false;
        }
        // NJH-TODO finish me, this is only approximate...
        utc = local - static_cast<std::time_t>(rule.gmtOffset());
        return true;
    }
};

}

#endif // ETZ_H
