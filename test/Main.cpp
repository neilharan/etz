
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Log.h"
#include "etz.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <list>
#include <sstream>


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr auto LineWidth = 100;
constexpr auto ColumnWidth = 30;

using namespace ETZ;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void locals(const std::string& utc)
{
    Log::test(std::string(LineWidth, '='));

    const auto _utc = Time::fromISOString(utc);
    if (!utc.empty() && !_utc.second) {
        Log::test("Invalid --utc parameter: ", utc);
        return;
    }
    const auto time = _utc.second ? _utc.first : Time::now();
    Log::test("UTC: ", Time::toISOString(time), Log::LF);
    Log::test(std::left, std::setw(ColumnWidth), "Time zone (IANA name)", " | ", "Local time");
    Log::test(std::string(LineWidth, '='));

    std::list<std::string> results;
    auto* const enums = TimeZones::getInstance();
    auto tz = TimeZone::Invalid;
    for (++tz; tz != TimeZone::Invalid; ++tz) {
        std::stringstream ss;
        ss << std::left << std::setw(ColumnWidth) << enums->value(tz) << " | " << Time::toISOString(UTC::toLocal(tz, time).first);
        results.push_back(ss.str());
    }
    results.sort();
    for (const auto& line : results) {
        Log::test(line);
    }
    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void timeZones()
{
    Log::test(std::string(LineWidth, '='));
    Log::test(std::left, std::setw(ColumnWidth / 2), "Enum ordinal", " | ", std::setw(ColumnWidth), "Enum name", " | ", "Time zone (IANA name)");
    Log::test(std::string(LineWidth, '='));

    std::vector<std::string> results;
    results.reserve(static_cast<uint16_t>(TimeZone::_MAX));
    auto* const enums = TimeZones::getInstance();
    auto tz = TimeZone::Invalid;
    for (++tz; tz != TimeZone::Invalid; ++tz) {
        const auto iana = enums->value(tz);
        std::stringstream ss;
        ss << std::left << std::setw(ColumnWidth / 2) << uint16_t(tz) << " | " << std::setw(ColumnWidth) << enums->ianaToEnumName(iana) << " | " << iana;
        results.emplace_back(ss.str());
    }
    for (const auto& line : results) {
        Log::test(line);
    }
    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _MSC_VER

#pragma optimize("", off)
template <class T> static void doNotOptimizeAway(T&& datum) { datum = datum; }
#pragma optimize("", on)

#elif defined(__clang__)

template <class T> __attribute__((__optnone__)) static void doNotOptimizeAway(T&&) { }

#else

template <class T> static void doNotOptimizeAway(T&& datum) { asm volatile("" : "+r"(datum)); }

#endif

static void bench()
{
    Log::test(std::string(LineWidth, '='));
    Log::test(Log::LF, "Benchmarking computations");
    Log::test(std::string(LineWidth, '='));

    static const size_t Queries = 10000000;
    const auto now = Time::now();

    Log::test(Log::LF, "Common use case (single time zone, incremental time)...");

    {
        const auto start = std::chrono::steady_clock::now();
        for (size_t i = 0; i < Queries; ++i) {
            doNotOptimizeAway(UTC::toLocal(TimeZone::Europe_London, now + i).first);
        }
        const auto finish = std::chrono::steady_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        Log::test("Completed in ", ms.count(), "ms (", ms.count() ? Queries / ms.count() * 1000 : 0, " queries/s): ");
    }

    Log::test(Log::LF, "Round-robin each time zone, constant time...");

    {
        auto tz = TimeZone::Invalid;
        const auto start = std::chrono::steady_clock::now();
        for (size_t i = 0; i < Queries; ++i) {
            doNotOptimizeAway(UTC::toLocal(++tz, now).first);
        }
        const auto finish = std::chrono::steady_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        Log::test("Completed in ", ms.count(), "ms (", ms.count() ? Queries / ms.count() * 1000 : 0, " queries/s): ");
    }

    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(const int argc, const char** argv)
{
    const auto param = [](const char** begin, const char** end, const std::string& option) {
        auto* it = std::find(begin, end, option);
        if (it != end && ++it != end) {
            return std::string(*it);
        }
        return std::string();
    };

    const auto hasOption = [](const char** begin, const char** end, const std::string& option) { return std::find(begin, end, option) != end; };

    Log::init();
    Log::test("ETZ: an embeddable timezone library");
    Log::test("Build options:");
    Log::test("    CountTimeZones: ", UTC::CountTimeZones);
    Log::test("    CountTimeZoneRules: ", UTC::CountTimeZoneRules, Log::LF);

    bool command {};
    if (hasOption(argv, argv + argc, "locals")) {
        locals(param(argv, argv + argc, "--utc"));
        command |= true;
    }
    if (hasOption(argv, argv + argc, "time-zones")) {
        timeZones();
        command |= true;
    }
    if (hasOption(argv, argv + argc, "bench")) {
        bench();
        command |= true;
    }
    if (hasOption(argv, argv + argc, "--help") || !command) {
        Log::test("Usage: etz-test [COMMAND]... [--utc ISO_DATETIME, default is now]", Log::LF);
        Log::test("Commands:");
        Log::test("    locals     : list local time for --utc for each supported time zone");
        Log::test("    time-zones : list supported time zones");
        Log::test("    bench      : benchmark queries");
        Log::test("    help       : this screen", Log::LF);
        Log::test("Note: ISO_DATETIME is simplified extended ISO8601-1:2019 format without decimal fractions (milliseconds), and without zone:");
        Log::test("    %4.4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d", Log::LF);
        Log::test("Examples:");
        Log::test("    etz-test locals --utc 2020-11-23T19:20:21");
    }
    return 0;
}
