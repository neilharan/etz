
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "etz.h"
#include "Log.h"

#include <algorithm>
#include <functional>


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr auto LineWidth = 100;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void locals()
{
    Log::test(std::string(LineWidth, '='));
    Log::test("...", Log::LF);
    Log::test(std::string(LineWidth, '='), Log::LF);
    // NJH-TODO
    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void timeZones()
{
    Log::test(std::string(LineWidth, '='));
    Log::test("...", Log::LF);
    Log::test(std::string(LineWidth, '='), Log::LF);
    // NJH-TODO
    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void utcLocal(const std::string& utc, const int tz)
{
    Log::test(std::string(LineWidth, '='));
    Log::test("...", Log::LF);
    Log::test(std::string(LineWidth, '='), Log::LF);
    // NJH-TODO
    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void localUtc(const std::string& local, const int tz)
{
    Log::test(std::string(LineWidth, '='));
    Log::test("...", Log::LF);
    Log::test(std::string(LineWidth, '='), Log::LF);
    // NJH-TODO
    Log::test(std::string(LineWidth, '='), Log::LF);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(const int argc, const char** argv)
{
    const auto param = [](const char** begin, const char** end, const std::string& option) {
        auto it = std::find(begin, end, option);
        if (it != end && ++it != end) {
            return std::string(*it);
        }
        return std::string();
    };

    const auto option = [](const char** begin, const char** end, const std::string& option) { return std::find(begin, end, option) != end; };

    Log::init();
    Log::test("ETZ: an embeddable timezone library");

    const auto utc = param(argv, argv + argc, "--utc");
    const auto local = param(argv, argv + argc, "--local");
    const auto tz = std::atoi(param(argv, argv + argc, "--tz").c_str());

    bool command {};
    if (option(argv, argv + argc, "locals")) {
        locals();
        command |= true;
    }
    if (option(argv, argv + argc, "timezones")) {
        timeZones();
        command |= true;
    }
    if (option(argv, argv + argc, "one-many")) {
        utcLocal(utc, tz);
        command |= true;
    }
    if (option(argv, argv + argc, "many-many")) {
        localUtc(local, tz);
        command |= true;
    }
    if (option(argv, argv + argc, "--help") || !command) {
        Log::test("Usage: etz-test [COMMAND]... [--utc ISO_DATETIME] [--local ISO_DATETIME] [--tz]", Log::LF);
        Log::test("Commands:");
        Log::test("    locals    : list current local time for each supported timezone");
        Log::test("    timezones : list supported timezones");
        Log::test("    utc-local : convert --utc to local time in --tz");
        Log::test("    local-utc : convert --local to UTC in --tz");
        Log::test("Examples:");
        Log::test("    etz-test locals");
        Log::test("    etz-test utc-local --utc 2020-11-18T19:20:21 --tz 35");
    }
    return 0;
}
