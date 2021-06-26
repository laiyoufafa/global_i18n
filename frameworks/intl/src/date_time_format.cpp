/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "date_time_format.h"
#include "ohos/init_data.h"
#include <algorithm>
#include <cmath>

namespace OHOS {
namespace Global {
namespace I18n {
using namespace icu;
std::map<std::string, DateFormat::EStyle> DateTimeFormat::dateTimeStyle = {
    { "full", DateFormat::EStyle::kFull },
    { "long", DateFormat::EStyle::kLong },
    { "medium", DateFormat::EStyle::kMedium },
    { "short", DateFormat::EStyle::kShort }
};

DateTimeFormat::DateTimeFormat(std::string localeTag)
{
    UErrorCode status = U_ZERO_ERROR;
    auto builder = std::make_unique<LocaleBuilder>();
    locale = builder->setLanguageTag(StringPiece(localeTag)).build(status);
    if (status != U_ZERO_ERROR) {
        locale = Locale::getDefault();
    }
    dateFormat = DateFormat::createDateInstance(DateFormat::DEFAULT, locale);
    if (dateFormat == nullptr) {
        dateFormat = DateFormat::createDateInstance();
    }
    calendar = Calendar::createInstance(locale, status);
    if (status != U_ZERO_ERROR) {
        calendar = Calendar::createInstance(status);
    }
    dateFormat->setCalendar(*calendar);
}

void DateTimeFormat::GetValidLocales()
{
    int32_t validCount = 1;
    const Locale *validLocales = Locale::getAvailableLocales(validCount);
    for (int i = 0; i < validCount; i++) {
        allValidLocales.insert(validLocales[i].getLanguage());
    }
}

DateTimeFormat::DateTimeFormat(const std::vector<std::string> &localeTags, std::map<std::string, std::string> &configs)
{
    UErrorCode status = U_ZERO_ERROR;
    auto builder = std::make_unique<LocaleBuilder>();
    ParseConfigsPartOne(configs);
    ParseConfigsPartTwo(configs);
    GetValidLocales();
    for (size_t i = 0; i < localeTags.size(); i++) {
        std::string curLocale = localeTags[i];
        locale = Locale::forLanguageTag(StringPiece(curLocale), status);
        if (allValidLocales.count(locale.getLanguage()) > 0) {
            localeInfo = new LocaleInfo(curLocale, configs);
            locale = localeInfo->GetLocale();
            localeTag = localeInfo->GetBaseName();
            if (hourCycle.empty()) {
                hourCycle = localeInfo->GetHourCycle();
            }
            ComputeHourCycleChars();
            ComputeSkeleton();
            InitDateFormat(status);
            if (dateFormat == nullptr) {
                continue;
            }
            break;
        }
    }
    if (dateFormat == nullptr) {
        locale = Locale::getDefault();
        localeTag = locale.getBaseName();
        std::replace(localeTag.begin(), localeTag.end(), '_', '-');
        dateFormat = DateFormat::createInstance();
    }
    calendar = Calendar::createInstance(locale, status);
}

DateTimeFormat::~DateTimeFormat()
{
    if (calendar != nullptr) {
        delete calendar;
        calendar = nullptr;
    }
    if (dateFormat != nullptr) {
        delete dateFormat;
        dateFormat = nullptr;
    }
    if (localeInfo != nullptr) {
        delete localeInfo;
        localeInfo = nullptr;
    }
}

void DateTimeFormat::InitDateFormat(UErrorCode &status)
{
    if (!dateStyle.empty() && timeStyle.empty()) {
        dateFormat = DateFormat::createDateInstance(dateTimeStyle[dateStyle], locale);
    } else if (dateStyle.empty() && !timeStyle.empty()) {
        dateFormat = DateFormat::createTimeInstance(dateTimeStyle[timeStyle], locale);
    } else if (!dateStyle.empty() && !timeStyle.empty()) {
        dateFormat = DateFormat::createDateTimeInstance(dateTimeStyle[dateStyle], dateTimeStyle[timeStyle], locale);
    } else {
        auto patternGenerator =
            std::unique_ptr<DateTimePatternGenerator>(DateTimePatternGenerator::createInstance(locale, status));
        ComputePattern();
        pattern = patternGenerator->replaceFieldTypes(patternGenerator->getBestPattern(pattern, status), pattern, status);
        pattern = patternGenerator->getBestPattern(pattern, status);
        dateFormat = new SimpleDateFormat(pattern, locale, status);
    }
}

void DateTimeFormat::ParseConfigsPartOne(std::map<std::string, std::string> &configs)
{
    if (configs.count("dateStyle") > 0) {
        dateStyle = configs["dateStyle"];
    }
    if (configs.count("timeStyle") > 0) {
        timeStyle = configs["timeStyle"];
    }
    if (configs.count("hourCycle") > 0) {
        hourCycle = configs["hourCycle"];
    }
    if (configs.count("timeZone") > 0) {
        timeZone = configs["timeZone"];
    }
    if (configs.count("numberingSystem") > 0) {
        numberingSystem = configs["numberingSystem"];
    }
    if (configs.count("hour12") > 0) {
        hour12 = configs["hour12"];
    }
    if (configs.count("weekday") > 0) {
        weekday = configs["weekday"];
    }
    if (configs.count("era") > 0) {
        era = configs["era"];
    }
}

void DateTimeFormat::ParseConfigsPartTwo(std::map<std::string, std::string> &configs)
{
    if (configs.count("year") > 0) {
        year = configs["year"];
    }
    if (configs.count("month") > 0) {
        month = configs["month"];
    }
    if (configs.count("day") > 0) {
        day = configs["day"];
    }
    if (configs.count("hour") > 0) {
        hour = configs["hour"];
    }
    if (configs.count("minute") > 0) {
        minute = configs["minute"];
    }
    if (configs.count("second") > 0) {
        second = configs["second"];
    }
    if (configs.count("fractionalSecondDigits") > 0) {
        fractionalSecondDigits = configs["fractionalSecondDigits"];
    }
    if (configs.count("timeZoneName") > 0) {
        timeZoneName = configs["timeZoneName"];
    }
}

void DateTimeFormat::ComputeSkeleton()
{
    AddOptions(year, yearChar);
    AddOptions(month, monthChar);
    AddOptions(day, dayChar);
    AddOptions(hour, hourChar);
    AddOptions(minute, minuteChar);
    AddOptions(second, secondChar);
    if (hourCycle == "h12" || hourCycle == "h11" || hour12 == "true") {
        pattern.append(amPmChar);
    }
    AddOptions(fractionalSecondDigits, fractionalSecondChar);
    AddOptions(timeZoneName, timeZoneChar);
    AddOptions(weekday, weekdayChar);
    AddOptions(era, eraChar);
}

void DateTimeFormat::AddOptions(std::string option, char16_t optionChar)
{
    if (!option.empty()) {
        pattern.append(optionChar);
    }
}

void DateTimeFormat::ComputeHourCycleChars()
{
    if (!hour12.empty()) {
        if (hour12 == "true") {
            hourNumericString = "h";
            hourTwoDigitString = "hh";
        } else {
            hourNumericString = "H";
            hourTwoDigitString = "HH";
        }
    } else {
        if (hourCycle == "h11") {
            hourNumericString = "K";
            hourTwoDigitString = "KK";
        } else if (hourCycle == "h12") {
            hourNumericString = "h";
            hourTwoDigitString = "hh";
        } else if (hourCycle == "h23") {
            hourNumericString = "H";
            hourTwoDigitString = "HH";
        } else if (hourCycle == "h24") {
            hourNumericString = "k";
            hourTwoDigitString = "kk";
        }
    }
}

void DateTimeFormat::ComputePattern()
{
    ComputePartOfPattern(year, yearChar, "yy", "Y");
    ComputePartOfPattern(day, dayChar, "dd", "d");
    ComputePartOfPattern(hour, hourChar, hourTwoDigitString, hourNumericString);
    ComputePartOfPattern(minute, minuteChar, "mm", "mm");
    ComputePartOfPattern(second, secondChar, "ss", "ss");
    if (!month.empty()) {
        UnicodeString monthOfPattern = UnicodeString(monthChar);
        int32_t length = monthOfPattern.length();
        if (month == "numeric" && length != NUMERIC_LENGTH) {
            pattern.findAndReplace(monthOfPattern, UnicodeString::fromUTF8(StringPiece("M")));
        } else if (month == "2-digit" && length != TWO_DIGIT_LENGTH) {
            pattern.findAndReplace(monthOfPattern, UnicodeString::fromUTF8(StringPiece("MM")));
        } else if (month == "long" && length != LONG_LENGTH) {
            pattern.findAndReplace(monthOfPattern, UnicodeString::fromUTF8(StringPiece("MMMM")));
        } else if (month == "short" && length != SHORT_LENGTH) {
            pattern.findAndReplace(monthOfPattern, UnicodeString::fromUTF8(StringPiece("MMM")));
        } else if (month == "narrow" && length != NARROW_LENGTH) {
            pattern.findAndReplace(monthOfPattern, UnicodeString::fromUTF8(StringPiece("MMMMM")));
        }
    }
    if (!timeZoneName.empty()) {
        UnicodeString timeZoneOfPattern = UnicodeString(timeZoneChar);
        if (timeZoneName == "long") {
            pattern.findAndReplace(timeZoneOfPattern, UnicodeString::fromUTF8(StringPiece("zzzz")));
        } else if (timeZoneName == "short") {
            pattern.findAndReplace(timeZoneOfPattern, UnicodeString::fromUTF8(StringPiece("O")));
        }
    }

    ComputeWeekdayOrEraOfPattern(weekday, weekdayChar, "EEEE", "E", "EEEEE");
    ComputeWeekdayOrEraOfPattern(era, eraChar, "GGGG", "G", "GGGGG");
}

void DateTimeFormat::ComputePartOfPattern(std::string option, char16_t character, std::string twoDigitChar,
    std::string numericChar)
{
    if (!option.empty()) {
        UnicodeString curPartOfPattern = UnicodeString(character);
        int32_t length = curPartOfPattern.length();
        if (option == "2-digit" && length != TWO_DIGIT_LENGTH) {
            pattern.findAndReplace(curPartOfPattern, UnicodeString::fromUTF8(StringPiece(twoDigitChar)));
        } else if (option == "numeric" && length != NUMERIC_LENGTH) {
            pattern.findAndReplace(curPartOfPattern, UnicodeString::fromUTF8(StringPiece(numericChar)));
        }
    }
}

void DateTimeFormat::ComputeWeekdayOrEraOfPattern(std::string option, char16_t character, std::string longChar,
    std::string shortChar, std::string narrowChar)
{
    if (!option.empty()) {
        UnicodeString curPartOfPattern = UnicodeString(character);
        int32_t length = curPartOfPattern.length();
        if (option == "long" && length != LONG_ERA_LENGTH) {
            pattern.findAndReplace(curPartOfPattern, UnicodeString::fromUTF8(StringPiece(longChar)));
        } else if (option == "short" && length != SHORT_ERA_LENGTH) {
            pattern.findAndReplace(curPartOfPattern, UnicodeString::fromUTF8(StringPiece(shortChar)));
        } else if (option == "narrow" && length != NARROW_LENGTH) {
            pattern.findAndReplace(curPartOfPattern, UnicodeString::fromUTF8(StringPiece(narrowChar)));
        }
    }
}

bool DateTimeFormat::icuInitialized = DateTimeFormat::Init();

std::string DateTimeFormat::Format(int year, int month, int day, int hour, int minute, int second)
{
    UErrorCode status = U_ZERO_ERROR;
    std::string result;
    UnicodeString dateString;

    calendar->clear();
    calendar->set(year, month, day, hour, minute, second);
    if (!timeZone.empty()) {
        UDate timestamp = calendar->getTime(status);
        auto zone = std::unique_ptr<TimeZone>(TimeZone::createTimeZone(timeZone.c_str()));
        calendar->setTimeZone(*zone);
        dateFormat->setTimeZone(*zone);
        calendar->setTime(timestamp, status);
    }
    dateFormat->format(calendar->getTime(status), dateString, status);
    dateString.toUTF8String(result);
    return result;
}

void DateTimeFormat::GetResolvedOptions(std::map<std::string, std::string> &map)
{
    UErrorCode status = U_ZERO_ERROR;
    map.insert(std::make_pair("locale", localeTag));
    if (!(localeInfo->GetCalendar()).empty()) {
        map.insert(std::make_pair("calendar", localeInfo->GetCalendar()));
    } else {
        map.insert(std::make_pair("calendar", calendar->getType()));
    }
    if (!dateStyle.empty()) {
        map.insert(std::make_pair("dateStyle", dateStyle));
    }
    if (!timeStyle.empty()) {
        map.insert(std::make_pair("timeStyle", timeStyle));
    }
    if (!hourCycle.empty()) {
        map.insert(std::make_pair("hourCycle", hourCycle));
    } else if (!(localeInfo->GetHourCycle()).empty()) {
        map.insert(std::make_pair("hourCycle", localeInfo->GetHourCycle()));
    }
    if (!timeZone.empty()) {
        map.insert(std::make_pair("timeZone", timeZone));
    } else {
        UnicodeString timeZoneID("");
        std::string timeZoneString;
        dateFormat->getTimeZone().getID(timeZoneID).toUTF8String(timeZoneString);
        map.insert(std::make_pair("timeZone", timeZoneString));
    }
    if (!timeZoneName.empty()) {
        map.insert(std::make_pair("timeZoneName", timeZoneName));
    }
    if (!numberingSystem.empty()) {
        map.insert(std::make_pair("numberingSystem", numberingSystem));
    } else if (!(localeInfo->GetNumberingSystem()).empty()) {
        map.insert(std::make_pair("numberingSystem", localeInfo->GetNumberingSystem()));
    } else {
        NumberingSystem *numSys = NumberingSystem::createInstance(locale, status);
        map.insert(std::make_pair("numberingSystem", numSys->getName()));
        delete numSys;
    }
    GetAdditionalResolvedOptions(map);
}

void DateTimeFormat::GetAdditionalResolvedOptions(std::map<std::string, std::string> &map)
{
    if (!hour12.empty()) {
        map.insert(std::make_pair("hour12", hour12));
    }
    if (!weekday.empty()) {
        map.insert(std::make_pair("weekday", weekday));
    }
    if (!era.empty()) {
        map.insert(std::make_pair("era", era));
    }
    if (!year.empty()) {
        map.insert(std::make_pair("year", year));
    }
    if (!month.empty()) {
        map.insert(std::make_pair("month", month));
    }
    if (!day.empty()) {
        map.insert(std::make_pair("day", day));
    }
    if (!hour.empty()) {
        map.insert(std::make_pair("hour", hour));
    }
    if (!minute.empty()) {
        map.insert(std::make_pair("minute", minute));
    }
    if (!second.empty()) {
        map.insert(std::make_pair("second", second));
    }
    if (!fractionalSecondDigits.empty()) {
        map.insert(std::make_pair("fractionalSecondDigits", fractionalSecondDigits));
    }
}

std::string DateTimeFormat::GetDateStyle() const
{
    return dateStyle;
}

std::string DateTimeFormat::GetTimeStyle() const
{
    return timeStyle;
}

std::string DateTimeFormat::GetHourCycle() const
{
    return hourCycle;
}

std::string DateTimeFormat::GetTimeZone() const
{
    return timeZone;
}

std::string DateTimeFormat::GetTimeZoneName() const
{
    return timeZoneName;
}

std::string DateTimeFormat::GetNumberingSystem() const
{
    return numberingSystem;
}

std::string DateTimeFormat::GetHour12() const
{
    return hour12;
}

std::string DateTimeFormat::GetWeekday() const
{
    return weekday;
}

std::string DateTimeFormat::GetEra() const
{
    return era;
}

std::string DateTimeFormat::GetYear() const
{
    return year;
}

std::string DateTimeFormat::GetMonth() const
{
    return month;
}

std::string DateTimeFormat::GetDay() const
{
    return day;
}

std::string DateTimeFormat::GetHour() const
{
    return hour;
}

std::string DateTimeFormat::GetMinute() const
{
    return minute;
}

std::string DateTimeFormat::GetSecond() const
{
    return second;
}

std::string DateTimeFormat::GetFractionalSecondDigits() const
{
    return fractionalSecondDigits;
}

bool DateTimeFormat::Init()
{
    SetHwIcuDirectory();
    return true;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS