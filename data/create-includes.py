# This program is part of ETZ (https://github.com/neilharan/etz).
# Extracts rules, enums and enum strings from timezonedb.com CSV files (https://timezonedb.com/files/timezonedb.csv.zip)
# timezonedb.com CSV files are themselves extracted from the well respected IANA database (https://www.iana.org/time-zones)
#
import csv
import re
from itertools import chain

zones = {}
with open("csv/zone.csv") as csvfile:
    r = csv.reader(csvfile)
    for row in r:
        zones[int(row[0])] = row[2]

def abbreviationFix(match):
    s = match.group(0)
    if s == "+":
        return "p"
    if s == "-":
        return "m"
    return s

abbreviations = set()
timezones = {}

with open("csv/timezone.csv") as csvfile:
    r = csv.reader(csvfile)
    for row in r:
        key = int(row[0])
        abbreviations |= { row[1] }
        rule = { "startTime": row[2] + "ll", "abbreviation": re.sub("[-+]", abbreviationFix, row[1]), "gmtOffset": row[3], "isDST": "true" if int(row[4]) == 1 else "false" }
        if key in timezones:
            timezones[key]["rules"].append(rule)
        else:
            timezones[key] = { "name": zones[key], "rules": [rule] }

indent = " " * 4

# write the rules include...
f = open("etz-rules.inl", "w")

# first, the rules themselves...
for k, v in timezones.items():
    _timezone = "static constexpr Rule {}[] = {{\n".format(re.sub("[/-]", "_", v["name"]))
    _timezone += ",\n".join((indent + "Rule({})".format(", ".join(str(v) if k != "abbreviation" else f"Abbreviation::{v}" for k, v in rule.items())) for rule in v["rules"]))
    _timezone += "\n};\n\n"
    f.write(_timezone)

# then the map...
f.write("static constexpr std::pair<TimeZone, std::pair<const Rule*, uint16_t>> TimeZoneRules[] = {\n");
f.write(",\n".join(indent + """std::make_pair(TimeZone::{0}, Rules({0}))""".format(re.sub("[/-]", "_", v["name"])) for v in timezones.values()))
f.write("\n};\n")
f.close()

# write the abbreviation includes...
begin = [ "Invalid" ]
end = [ "_MAX" ]

f = open("etz-abbreviation-enum.inl", "w")
f.write("enum class Abbreviation: uint16_t {\n")
f.write(",\n".join(indent + re.sub("[-+]", abbreviationFix, a) for a in begin + list(abbreviations) + end))
f.write("\n};\n")
f.close()

f = open("etz-abbreviation-names.inl", "w")
f.write("static constexpr const char* AbbreviationNames[] {\n")
f.write(",\n".join(indent + f'"{a}"' for a in begin + list(abbreviations)))
f.write("\n};\n")
f.close()

# write the timezones include...
begin = [{ "name": "Invalid" }]
end = [{ "name": "_MAX" }]

f = open("etz-timezone-enum.inl", "w")
f.write("enum class TimeZone: uint16_t {\n")
f.write(",\n".join(indent + re.sub("[/-]", "_", tz["name"]) for tz in begin + list(timezones.values()) + end))
f.write("\n};\n")
f.close()

f = open("etz-timezone-names.inl", "w")
f.write("static constexpr const char* TimeZoneNames[] {\n")
f.write(",\n".join(indent + '"{}"'.format(tz["name"]) for tz in begin + list(timezones.values())))
f.write("\n};\n")
f.close()

