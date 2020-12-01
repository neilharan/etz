#ifndef ETZ_H
#define ETZ_H


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <ctime>
#include <string>
#include <unordered_map>


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ETZ
{

// Cache delivers best results for common use case (repetitive queries for the same time zone and an incrementing time parameter).
// When query parameters are less consistent, performance may be improved by disabling the cache...
//
static constexpr bool EnableRuleCache = true; 


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static_assert(sizeof(time_t) == 8);


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Enums.
//
#include "etz-data/abbreviation-enum.inl"
#include "etz-data/timezone-enum.inl"


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
    constexpr time_t timeStart() const { return (m_data & 0x08) ? -((static_cast<time_t>(m_data & 0x07) << 32) + m_timeStart) : ((static_cast<time_t>(m_data & 0x07) << 32) + m_timeStart); }
    constexpr Abbreviation abbreviation() const { return Abbreviation(m_data >> 6); }
    constexpr int32_t gmtOffset() const { return (m_data & 0x10) ? -static_cast<int32_t>(m_gmtOffset) : m_gmtOffset; }
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
// UTC<->local/civil class.
//
class RulesBase {
protected:
    // Declare members in ancestor class - clang/gcc require members used in constant expressions to be in a complete class (https://stackoverflow.com/a/29662526)...
    //
    using RulesType = std::pair<const Rule*, uint16_t>;
    template <typename T, size_t N> static constexpr auto Rules(T (&r)[N]) { return std::make_pair(r, static_cast<uint16_t>(N)); }
};

class UTC: public RulesBase
{
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Time zone rules.
    // This file is built by data/create-includes.py which consumes CSV from timezonedb.com (https://timezonedb.com/files/timezonedb.csv.zip)
    // timezonedb.com is itself extracted from the well respected IANA database (https://www.iana.org/time-zones)
    //
#include "etz-data/rules.inl"

public:
    static constexpr size_t CountTimeZones = sizeof(TimeZoneRules) / sizeof(TimeZoneRules[0]);

private:
    static auto ruleLu(const TimeZone timeZone, const std::time_t utc)
    {
        thread_local static struct {
            TimeZone timeZone { TimeZone::Invalid };
            Rule rule;
        } lastQuery;

        if constexpr (EnableRuleCache == true) {
            if (timeZone == lastQuery.timeZone && utc >= lastQuery.rule.timeStart()) {
                return lastQuery.rule;
            }
        }
        const auto rules = TimeZoneRulesMap.find(timeZone);
        if (rules == TimeZoneRulesMap.end()) {
            return Rule();
        }
        // Start with the last rule and work backwards...
        for (auto it = rules->second->first + rules->second->second - 1;; --it) {
            if (it->timeStart() <= utc) {
                if constexpr (EnableRuleCache == true) {
                    lastQuery.timeZone = timeZone;
                    lastQuery.rule = *it;
                }
                return *it;
            }
            if (it == rules->second->first) {
                return Rule(); // no match
            }
        }
    }

    using Map = std::unordered_map<TimeZone, const std::pair<const Rule*, uint16_t>*>;

    static inline const Map TimeZoneRulesMap = []() {
        Map map;
        map.reserve(CountTimeZones);
        for (size_t i = 0; i < CountTimeZones; ++i) {
            map.emplace(TimeZoneRules[i].first, &TimeZoneRules[i].second);
        }
        return map;
    }();

public:
    static constexpr size_t CountTimeZoneRules = []{
        size_t count { };
        for (size_t i = 0; i < CountTimeZones; ++i) {
            count += TimeZoneRules[i].second.second;
        }
        return count;
    }();

    static inline auto toLocal(const TimeZone timeZone, const std::time_t utc)
    {
        const auto rule = ruleLu(timeZone, utc);
        if (!rule.isValid()) {
            return std::make_pair(static_cast<std::time_t>(-1), false);
        }
        return std::make_pair(utc + static_cast<std::time_t>(rule.gmtOffset()), true);
    }
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Time
{
public:
    static auto now()
    {
        thread_local static std::time_t t;
        return time(&t);
    }

    // Simplified extended ISO8601-1:2019 format without decimal fractions (milliseconds), and without zone (as we want to render local time)...
    static auto toISOString(const std::time_t time)
    {
        std::tm tm;

#ifdef _MSC_VER
        if (gmtime_s(&tm, &time)) {
            return std::string();
        }
#else
        if (!gmtime_r(&time, &tm)) {
            return std::string();
        }
#endif
        return format("%4.4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }

    // e.g. 2020-11-23T19:20:21...
    static auto fromISOString(const std::string& time)
    {
        const auto checkedScan = [](const int count, const char* buf, const char* fmt, ...) {
            va_list ap;
            va_start(ap, fmt);
            const auto c = vsscanf(buf, fmt, ap);
            va_end(ap);
            return c == count;
        };

        if (time.length() != 19) {
            return std::make_pair(static_cast<std::time_t>(-1), false);
        }
        static const size_t Params = 6;
        std::tm tm {};
        if (!checkedScan(Params, time.c_str(), "%4d-%2d-%2dT%2d:%2d:%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) || !tm.tm_mday) {
            return std::make_pair(static_cast<std::time_t>(-1), false);
        }
        tm.tm_year -= 1900;
        tm.tm_mon--;
        tm.tm_isdst = -1;
        const auto t = std::mktime(&tm);
        if (t == -1) {
            return std::make_pair(static_cast<std::time_t>(-1), false);
        }
        return std::make_pair(t, true);
    }

private:
    template <typename... A> static std::string format(const std::string& format, A... args)
    {
        const auto size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
        if (size <= 0) {
            return "";
        }
        const std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1);
    }
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Helper lookup class.
//
template <typename Enum> class Enums
{
public:
    constexpr Enums() { }

    Enums(const Enums&) = delete;
    Enums& operator=(const Enums&) = delete;

    auto key(const std::string& value)
    {
        const auto it = m_values.find(value);
        if (it == m_values.end()) {
            return Enum::Invalid;
        }
        return Enum(it->second);
    }

    auto value(const Enum key)
    {
        for (const auto& [k, v] : m_values) {
            if (v == uint16_t(key)) {
                return k;
            }
        }
        return std::string();
    }

protected:
    using Map = std::unordered_map<std::string, uint16_t>;

    // Don't use initializer_lists for static initialization as most compilers will generate a temporary variable with a large stack requirement...
    template <typename T, size_t N> constexpr auto makeMap(T (&names)[N])
    {
        m_values.reserve(N);
        for (uint16_t i = 0; i < N; ++i) {
            m_values.emplace(std::make_pair(names[i], i));
        }
    }

    Map m_values;
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Abbreviations : public Enums<Abbreviation>
{
#include "etz-data/abbreviation-names.inl"

public:
    constexpr Abbreviations() { makeMap(AbbreviationNames); }

    static Abbreviations* getInstance()
    {
        static Abbreviations instance;
        return &instance;
    }
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimeZones : public Enums<TimeZone>
{
#include "etz-data/timezone-names.inl"

public:
    constexpr TimeZones() { makeMap(TimeZoneNames); }

    auto ianaToEnumName(const std::string& key)
    {
        auto v = key;
        std::replace(v.begin(), v.end(), '/', '_');
        std::replace(v.begin(), v.end(), '-', '_');
        return v;
    }

    static TimeZones* getInstance()
    {
        static TimeZones instance;
        return &instance;
    }
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline TimeZone& operator++(TimeZone& tz)
{
    using IntType = typename std::underlying_type<TimeZone>::type;
    tz = static_cast<TimeZone>(static_cast<IntType>(tz) + 1);
    if (tz == TimeZone::_MAX) {
        tz = TimeZone::Invalid;
    }
    return tz;
}

inline TimeZone operator++(TimeZone& tz, int)
{
    auto result = tz;
    ++tz;
    return result;
}

}

#endif // ETZ_H
