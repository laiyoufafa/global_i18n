/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "i18n_timezone.h"

#include <filesystem>
#include <sys/stat.h>
#include "hilog/log.h"
#include "libxml/globals.h"
#include "libxml/tree.h"
#include "libxml/xmlstring.h"
#include "locale_config.h"
#include "locale_info.h"
#include "map"
#include "set"
#include "string"
#include "type_traits"
#include "umachine.h"
#include "utility"
#include "utils.h"
#include "utypes.h"
#include "vector"
#include "unicode/locid.h"
#include "unicode/unistr.h"
#include "utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "I18nJs" };
using namespace OHOS::HiviewDFX;

const char *I18nTimeZone::TIMEZONE_KEY = "persist.time.timezone";
const char *I18nTimeZone::DEFAULT_TIMEZONE = "GMT";

const char *I18nTimeZone::CITY_TIMEZONE_DATA_PATH = "/system/usr/ohos_timezone/timezones.xml";
const char *I18nTimeZone::DEVICE_CITY_TIMEZONE_DATA_PATH = "/system/usr/device_timezone/timezones.xml";
const char *I18nTimeZone::DEFAULT_LOCALE = "root";
const char *I18nTimeZone::CITY_DISPLAYNAME_PATH = "/system/usr/ohos_timezone/ohos_city_dispname/";
const char *I18nTimeZone::DEVICE_CITY_DISPLAYNAME_PATH = "/system/usr/device_timezone/device_city_dispname/";

const char *I18nTimeZone::TIMEZONE_ROOT_TAG = "timezones";
const char *I18nTimeZone::TIMEZONE_SECOND_ROOT_TAG = "timezone";
const char *I18nTimeZone::CITY_DISPLAYNAME_ROOT_TAG = "display_names";
const char *I18nTimeZone::CITY_DISPLAYNAME_SECOND_ROOT_TAG = "display_name";
const char *I18nTimeZone::ZONEINFO_PATH = "/system/etc/zoneinfo";

std::set<std::string> I18nTimeZone::supportedLocales {};
std::set<std::string> I18nTimeZone::availableZoneCityIDs {};
std::map<std::string, std::string> I18nTimeZone::city2TimeZoneID {};
bool I18nTimeZone::useDeviceCityDispName = false;

I18nTimeZone::I18nTimeZone(std::string &id, bool isZoneID)
{
    if (isZoneID) {
        if (id.empty()) {
            std::string systemTimezone = ReadSystemParameter(TIMEZONE_KEY, SYS_PARAM_LEN);
            if (systemTimezone.length() == 0) {
                systemTimezone = DEFAULT_TIMEZONE;
            }
            icu::UnicodeString unicodeZoneID(systemTimezone.data(), systemTimezone.length());
            timezone = icu::TimeZone::createTimeZone(unicodeZoneID);
        } else {
            icu::UnicodeString unicodeZoneID(id.data(), id.length());
            timezone = icu::TimeZone::createTimeZone(unicodeZoneID);
        }
    } else {
        if (city2TimeZoneID.size() == 0) {
            GetAvailableZoneCityIDs();
        }
        if (city2TimeZoneID.find(id) == city2TimeZoneID.end()) {
            timezone = icu::TimeZone::createDefault();
        } else {
            std::string timezoneID = city2TimeZoneID.at(id);
            icu::UnicodeString unicodeZoneID(timezoneID.data(), timezoneID.length());
            timezone = icu::TimeZone::createTimeZone(unicodeZoneID);
        }
    }
}

I18nTimeZone::~I18nTimeZone()
{
    if (timezone != nullptr) {
        delete timezone;
        timezone = nullptr;
    }
}

icu::TimeZone* I18nTimeZone::GetTimeZone()
{
    return timezone;
}

std::unique_ptr<I18nTimeZone> I18nTimeZone::CreateInstance(std::string &id, bool isZoneID)
{
    std::unique_ptr<I18nTimeZone> i18nTimeZone = std::make_unique<I18nTimeZone>(id, isZoneID);
    if (i18nTimeZone->GetTimeZone() == nullptr) {
        return nullptr;
    }
    return i18nTimeZone;
}

int32_t I18nTimeZone::GetOffset(double date)
{
    int32_t rawOffset = 0;
    int32_t dstOffset = 0;
    bool local = false;
    UErrorCode status = U_ZERO_ERROR;
    if (timezone == nullptr) {
        return 0;
    }
    timezone->getOffset(date, (UBool)local, rawOffset, dstOffset, status);
    if (status != U_ZERO_ERROR) {
        return 0;
    }
    return rawOffset + dstOffset;
}

int32_t I18nTimeZone::GetRawOffset()
{
    if (timezone == nullptr) {
        return 0;
    }
    return timezone->getRawOffset();
}

std::string I18nTimeZone::GetID()
{
    if (timezone == nullptr) {
        return "";
    }
    icu::UnicodeString zoneID;
    timezone->getID(zoneID);
    std::string result;
    zoneID.toUTF8String(result);
    return result;
}

std::string I18nTimeZone::GetDisplayName()
{
    if (timezone == nullptr) {
        return "";
    }
    std::string localeStr = LocaleConfig::GetSystemLocale();
    return GetDisplayName(localeStr, false);
}

std::string I18nTimeZone::GetDisplayName(bool isDST)
{
    std::string localeStr = LocaleConfig::GetSystemLocale();
    return GetDisplayName(localeStr, isDST);
}

std::string I18nTimeZone::GetDisplayName(std::string localeStr)
{
    return GetDisplayName(localeStr, false);
}

std::string I18nTimeZone::GetDisplayName(std::string localeStr, bool isDST)
{
    icu::TimeZone::EDisplayType style = icu::TimeZone::EDisplayType::LONG_GENERIC;
    icu::Locale locale(localeStr.data());
    icu::UnicodeString name;
    timezone->getDisplayName((UBool)isDST, style, locale, name);
    std::string result;
    name.toUTF8String(result);
    return result;
}

bool I18nTimeZone::ReadTimeZoneData(const char *xmlPath)
{
    xmlKeepBlanksDefault(0);
    if (xmlPath == nullptr) {
        return false;
    }
    xmlDocPtr doc = xmlParseFile(xmlPath);
    if (!doc) {
        return false;
    }
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(TIMEZONE_ROOT_TAG))) {
        xmlFreeDoc(doc);
        return false;
    }
    cur = cur->xmlChildrenNode;
    while (cur != nullptr && !xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(TIMEZONE_SECOND_ROOT_TAG))) {
        xmlNodePtr value = cur->xmlChildrenNode;
        xmlChar *contents[ELEMENT_NUM] = { 0 }; // 2 represent cityid, zoneid;
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (value != nullptr) {
                contents[i] = xmlNodeGetContent(value);
                value = value->next;
            } else {
                break;
            }
        }
        // 0 represents cityid index, 1 represents zoneid index
        availableZoneCityIDs.insert(reinterpret_cast<const char *>(contents[0]));
        city2TimeZoneID.insert(
            std::make_pair<std::string, std::string>(reinterpret_cast<const char *>(contents[0]),
                                                     reinterpret_cast<const char *>(contents[1])));
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (contents[i] != nullptr) {
                xmlFree(contents[i]);
            }
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
    return true;
}

void I18nTimeZone::GetTimezoneIDFromZoneInfo(std::set<std::string> &availableIDs, std::string &parentPath,
    std::string &parentName)
{
    using std::filesystem::directory_iterator;

    for (const auto &dirEntry : directory_iterator{parentPath}) {
        std::string zonePath = dirEntry.path();
        std::string zoneName = zonePath.substr(parentPath.length() + 1); // 1 add length of path splitor
        availableIDs.insert(parentName + "/" + zoneName);
    }
}

std::set<std::string> I18nTimeZone::GetAvailableIDs(I18nErrorCode &errorCode)
{
    using std::filesystem::directory_iterator;

    std::set<std::string> availableIDs;
    struct stat s;
    for (const auto &dirEntry : directory_iterator{ZONEINFO_PATH}) {
        std::string parentPath = dirEntry.path();
        if (stat(parentPath.c_str(), &s) != 0) {
            HiLog::Error(LABEL, "zoneinfo path %{public}s not exist.", parentPath.c_str());
            errorCode = I18nErrorCode::FAILED;
            return availableIDs;
        }
        std::string parentName = parentPath.substr(strlen(ZONEINFO_PATH) + 1);
        if (s.st_mode & S_IFDIR) {
            GetTimezoneIDFromZoneInfo(availableIDs, parentPath, parentName);
        } else {
            availableIDs.insert(parentName);
        }
    }
    return availableIDs;
}

std::set<std::string> I18nTimeZone::GetAvailableZoneCityIDs()
{
    if (availableZoneCityIDs.size() != 0) {
        return availableZoneCityIDs;
    }
    struct stat s;
    if (stat(DEVICE_CITY_TIMEZONE_DATA_PATH, &s) == 0) {
        ReadTimeZoneData(DEVICE_CITY_TIMEZONE_DATA_PATH);
    } else {
        ReadTimeZoneData(CITY_TIMEZONE_DATA_PATH);
    }
    return availableZoneCityIDs;
}

std::string I18nTimeZone::FindCityDisplayNameFromXml(std::string &cityID, std::string &locale)
{
    xmlKeepBlanksDefault(0);
    std::string xmlPath;
    if (useDeviceCityDispName) {
        xmlPath = DEVICE_CITY_DISPLAYNAME_PATH + locale + ".xml";
    } else {
        xmlPath = CITY_DISPLAYNAME_PATH + locale + ".xml";
    }
    xmlDocPtr doc = xmlParseFile(xmlPath.c_str());
    if (!doc) {
        HiLog::Error(LABEL, "can't parse city displayname file %{public}s", xmlPath.c_str());
        return "";
    }
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(CITY_DISPLAYNAME_ROOT_TAG))) {
        xmlFreeDoc(doc);
        HiLog::Error(LABEL, "city displayname file %{public}s has wrong root tag.", xmlPath.c_str());
        return "";
    }
    cur = cur->xmlChildrenNode;
    std::string displayName;
    while (cur != nullptr && !xmlStrcmp(cur->name,
        reinterpret_cast<const xmlChar *>(CITY_DISPLAYNAME_SECOND_ROOT_TAG))) {
        xmlNodePtr value = cur->xmlChildrenNode;
        xmlChar *contents[ELEMENT_NUM] = { 0 }; // 2 represent cityid, displayName;
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (value != nullptr) {
                contents[i] = xmlNodeGetContent(value);
                value = value->next;
            } else {
                break;
            }
        }
        if (strcmp(cityID.c_str(), reinterpret_cast<const char *>(contents[0])) == 0) {
            displayName = reinterpret_cast<const char *>(contents[1]);
        }
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (contents[i] != nullptr) {
                xmlFree(contents[i]);
            }
        }
        if (displayName.length() != 0) {
            break;
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
    return displayName;
}

bool I18nTimeZone::GetSupportedLocales()
{
    using std::filesystem::directory_iterator;

    struct stat s;
    std::string displayNamePath;
    if (stat(DEVICE_CITY_DISPLAYNAME_PATH, &s) == 0) {
        useDeviceCityDispName = true;
        displayNamePath = DEVICE_CITY_DISPLAYNAME_PATH;
    } else {
        displayNamePath = CITY_DISPLAYNAME_PATH;
    }
    for (const auto &dirEntry : directory_iterator{displayNamePath}) {
        std::string xmlPath = dirEntry.path();
        if (stat(xmlPath.c_str(), &s) != 0) {
            HiLog::Error(LABEL, "city displayname file %{public}s not exist.", xmlPath.c_str());
            return false;
        }
        int32_t localeStrLen = static_cast<int32_t>(xmlPath.length()) - static_cast<int32_t>(
            displayNamePath.length()) - 4;  // 4 is the length of ".xml"
        std::string localeStr = xmlPath.substr(displayNamePath.length(), localeStrLen);
        supportedLocales.insert(localeStr);
    }
    return true;
}

std::string I18nTimeZone::GetFallBack(std::string &localeStr)
{
    if (strcmp(localeStr.c_str(), DEFAULT_LOCALE) == 0) {
        return "";
    }
    size_t begin = 0;
    size_t end = 0;
    std::vector<std::string> localeParts;
    while (true) {
        end = localeStr.find('_', begin);
        localeParts.push_back(localeStr.substr(begin, end - begin));
        if (end == std::string::npos) {
            break;
        }
        begin = end + 1;
    }

    if (localeParts.size() == 1) {
        return DEFAULT_LOCALE;
    }
    std::string fallBackLocale;
    for (size_t i = 0; i < localeParts.size() - 1; i++) {
        fallBackLocale += localeParts[i];
        if (i != localeParts.size() - 2) { // -2 represent the last part
            fallBackLocale += "_";
        }
    }
    return fallBackLocale;
}

std::string I18nTimeZone::GetCityDisplayName(std::string &cityID, std::string &localeStr)
{
    if (availableZoneCityIDs.size() == 0) {
        GetAvailableZoneCityIDs();
    }
    if (availableZoneCityIDs.find(cityID) == availableZoneCityIDs.end()) {
        HiLog::Error(LABEL, "%{public}s is not supported cityID.", cityID.c_str());
        return "";
    }
    if (supportedLocales.size() == 0) {
        bool status = GetSupportedLocales();
        if (!status) {
            HiLog::Error(LABEL, "get supported Locales failed");
            return "";
        }
    }
    UErrorCode errorCode = U_ZERO_ERROR;
    icu::Locale locale = icu::Locale::forLanguageTag(localeStr, errorCode);
    if (U_FAILURE(errorCode)) {
        HiLog::Error(LABEL, "create icu Locale for %{public}s failed", localeStr.c_str());
        return "";
    }
    std::string requestLocaleStr = locale.getBaseName();
    std::string displayName;
    if (supportedLocales.find(requestLocaleStr) != supportedLocales.end()) {
        displayName = FindCityDisplayNameFromXml(cityID, requestLocaleStr);
        if (displayName.length() != 0) {
            return displayName;
        }
    }
    locale.addLikelySubtags(errorCode);
    if (U_FAILURE(errorCode)) {
        HiLog::Error(LABEL, "add likely subtags for %{public}s failed", localeStr.c_str());
        return "";
    }
    requestLocaleStr = locale.getBaseName();
    while (requestLocaleStr.length() != 0) {
        if (supportedLocales.find(requestLocaleStr) != supportedLocales.end() || strcmp(requestLocaleStr.c_str(),
            DEFAULT_LOCALE) == 0) {
            displayName = FindCityDisplayNameFromXml(cityID, requestLocaleStr);
            if (displayName.length() != 0) {
                return displayName;
            }
        }
        requestLocaleStr = GetFallBack(requestLocaleStr);
    }
    return displayName;
}
}
}
}
