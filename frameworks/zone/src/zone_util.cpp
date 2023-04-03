/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include <filesystem>
#include <sys/stat.h>
#include "hilog/log.h"
#include "i18n_timezone.h"
#include "ohos/init_data.h"
#include "strenum.h"
#include "unicode/timezone.h"
#include "utils.h"
#include "zone_util.h"

using namespace OHOS::Global::I18n;
using namespace icu;
using namespace std;

static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "ZoneUtil" };
using namespace OHOS::HiviewDFX;
const char *ZoneUtil::COUNTRY_ZONE_DATA_PATH = "/system/usr/ohos_timezone/tzlookup.xml";
const char *ZoneUtil::DEFAULT_TIMEZONE = "GMT";
const char *ZoneUtil::TIMEZONES_TAG = "timezones";
const char *ZoneUtil::ID_TAG = "id";
const char *ZoneUtil::DEFAULT_TAG = "default";
const char *ZoneUtil::BOOSTED_TAG = "defaultBoost";
const char *ZoneUtil::ROOT_TAG = "countryzones";
const char *ZoneUtil::SECOND_TAG = "country";
const char *ZoneUtil::CODE_TAG = "code";
const char *ZoneUtil::TIMEZONE_KEY = "persist.time.timezone";

unordered_map<string, string> ZoneUtil::defaultMap = {
    {"AQ", "Antarctica/McMurdo"},
    {"AR", "America/Argentina/Buenos_Aires"},
    {"AU", "Australia/Sydney"},
    {"BR", "America/Noronha"},
    {"CA", "America/St_Johns"},
    {"CD", "Africa/Kinshasa"},
    {"CL", "America/Santiago"},
    {"CN", "Asia/Shanghai"},
    {"CY", "Asia/Nicosia"},
    {"DE", "Europe/Berlin"},
    {"EC", "America/Guayaquil"},
    {"ES", "Europe/Madrid"},
    {"FM", "Pacific/Pohnpei"},
    {"GL", "America/Godthab"},
    {"ID", "Asia/Jakarta"},
    {"KI", "Pacific/Tarawa"},
    {"KZ", "Asia/Almaty"},
    {"MH", "Pacific/Majuro"},
    {"MN", "Asia/Ulaanbaatar"},
    {"MX", "America/Mexico_City"},
    {"MY", "Asia/Kuala_Lumpur"},
    {"NZ", "Pacific/Auckland"},
    {"PF", "Pacific/Tahiti"},
    {"PG", "Pacific/Port_Moresby"},
    {"PS", "Asia/Gaza"},
    {"PT", "Europe/Lisbon"},
    {"RU", "Europe/Moscow"},
    {"UA", "Europe/Kiev"},
    {"UM", "Pacific/Wake"},
    {"US", "America/New_York"},
    {"UZ", "Asia/Tashkent"},
};

bool ZoneUtil::icuInitialized = ZoneUtil::Init();

string ZoneUtil::GetDefaultZone(const string &country)
{
    string temp(country);
    for (size_t i = 0; i < temp.size(); i++) {
        temp[i] = (char)toupper(temp[i]);
    }
    if (defaultMap.find(temp) != defaultMap.end()) {
        return defaultMap[temp];
    }
    string ret;
    StringEnumeration *strEnum = TimeZone::createEnumeration(temp.c_str());
    GetString(strEnum, ret);
    if (strEnum != nullptr) {
        delete strEnum;
    }
    return ret;
}

string ZoneUtil::GetDefaultZone(const int32_t number)
{
    string *region_code = new(nothrow) string();
    if (!region_code) {
        return "";
    }
    phone_util.GetRegionCodeForCountryCode(number, region_code);
    if (!region_code) {
        return "";
    }
    string ret = GetDefaultZone(*region_code);
    delete region_code;
    return ret;
}

string ZoneUtil::GetDefaultZone(const string country, const int32_t offset)
{
    UErrorCode status = U_ZERO_ERROR;
    StringEnumeration *strEnum =
        TimeZone::createTimeZoneIDEnumeration(UCAL_ZONE_TYPE_ANY, country.c_str(), &offset, status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    string ret;
    GetString(strEnum, ret);
    if (strEnum != nullptr) {
        delete strEnum;
        strEnum = nullptr;
    }
    return ret;
}

string ZoneUtil::GetDefaultZone(const int32_t number, const int32_t offset)
{
    string *region_code = new(nothrow) string();
    if (!region_code) {
        return "";
    }
    phone_util.GetRegionCodeForCountryCode(number, region_code);
    if (!region_code) {
        return "";
    }
    string ret = GetDefaultZone(*region_code, offset);
    delete region_code;
    return ret;
}

void ZoneUtil::GetZoneList(const string country, vector<string> &retVec)
{
    StringEnumeration *strEnum = TimeZone::createEnumeration(country.c_str());
    GetList(strEnum, retVec);
    if (strEnum != nullptr) {
        delete strEnum;
        strEnum = nullptr;
    }
}

void ZoneUtil::GetZoneList(const string country, const int32_t offset, vector<string> &retVec)
{
    UErrorCode status = U_ZERO_ERROR;
    StringEnumeration *strEnum =
        TimeZone::createTimeZoneIDEnumeration(UCAL_ZONE_TYPE_ANY, country.c_str(), &offset, status);
    if (status != U_ZERO_ERROR) {
        delete strEnum;
        strEnum = nullptr;
        return;
    }
    GetList(strEnum, retVec);
    if (strEnum != nullptr) {
        delete strEnum;
        strEnum = nullptr;
    }
}

void ZoneUtil::GetString(StringEnumeration *strEnum, string& ret)
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString uniString;
    if (!strEnum) {
        return;
    }
    int32_t count = strEnum->count(status);
    if ((status != U_ZERO_ERROR) || count <= 0) {
        return;
    }
    const UnicodeString *uniStr = strEnum->snext(status);
    if ((status != U_ZERO_ERROR) || (!uniStr)) {
        return;
    }
    UnicodeString canonicalUnistring;
    TimeZone::getCanonicalID(*uniStr, canonicalUnistring, status);
    if (status != U_ZERO_ERROR) {
        return;
    }
    canonicalUnistring.toUTF8String(ret);
    return;
}

void ZoneUtil::GetList(StringEnumeration *strEnum, vector<string> &retVec)
{
    if (!strEnum) {
        return;
    }
    UErrorCode status = U_ZERO_ERROR;
    int32_t count = strEnum->count(status);
    if (count <= 0 || status != U_ZERO_ERROR) {
        return;
    }
    while (count > 0) {
        const UnicodeString *uniStr = strEnum->snext(status);
        if ((!uniStr) || (status != U_ZERO_ERROR)) {
            retVec.clear();
            break;
        }
        UnicodeString canonicalUnistring;
        TimeZone::getCanonicalID(*uniStr, canonicalUnistring, status);
        if (status != U_ZERO_ERROR) {
            retVec.clear();
            break;
        }
        string canonicalString = "";
        canonicalUnistring.toUTF8String(canonicalString);
        if ((canonicalString != "") && (find(retVec.begin(), retVec.end(), canonicalString) == retVec.end())) {
            retVec.push_back(canonicalString);
        }
        --count;
    }
    return;
}

bool ZoneUtil::Init()
{
    SetHwIcuDirectory();
    return true;
}

CountryResult ZoneUtil::LookupTimezoneByCountryAndNITZ(std::string &region, NITZData &nitzData)
{
    std::vector<std::string> zones;
    bool isBoosted = false;
    std::string defaultTimezone;
    std::string systemTimezone = ReadSystemParameter(TIMEZONE_KEY, SYS_PARAM_LEN);
    if (systemTimezone.length() == 0) {
        systemTimezone = DEFAULT_TIMEZONE;
    }
    if (CheckFileExist()) {
        HiLog::Info(LABEL, "ZoneUtil::LookupTimezoneByCountryAndNITZ use tzlookup.xml");
        GetCountryZones(region, defaultTimezone, isBoosted, zones);
    } else {
        HiLog::Info(LABEL, "ZoneUtil::LookupTimezoneByCountryAndNITZ use icu data");
        GetICUCountryZones(region, zones, defaultTimezone);
    }
    return Match(zones, nitzData, systemTimezone);
}

CountryResult ZoneUtil::LookupTimezoneByNITZ(NITZData &nitzData)
{
    std::string systemTimezone = ReadSystemParameter(TIMEZONE_KEY, SYS_PARAM_LEN);
    if (systemTimezone.length() == 0) {
        systemTimezone = DEFAULT_TIMEZONE;
    }
    I18nErrorCode status = I18nErrorCode::SUCCESS;
    std::set<std::string> icuTimezones = I18nTimeZone::GetAvailableIDs(status);
    if (status != I18nErrorCode::SUCCESS) {
        HiLog::Error(LABEL, "ZoneUtil::LookupTimezoneByNITZ can not get icu data");
    }
    std::vector<std::string> validZones;
    for (auto it = icuTimezones.begin(); it != icuTimezones.end(); ++it) {
        validZones.push_back(*it);
    }

    CountryResult result = Match(validZones, nitzData, systemTimezone);
    if (result.timezoneId.length() == 0 && nitzData.isDST >= 0) {
        NITZData newNITZData = { -1, nitzData.totalOffset, nitzData.currentMillis };  // -1 means not consider DST
        result = Match(validZones, newNITZData, systemTimezone);
    }
    return result;
}

CountryResult ZoneUtil::LookupTimezoneByCountry(std::string &region, int64_t currentMillis)
{
    std::vector<std::string> zones;
    bool isBoosted = false;
    std::string defaultTimezone;
    CountryResult result = { true, MatchQuality::DEFAULT_BOOSTED, defaultTimezone };
    if (CheckFileExist()) {
        HiLog::Info(LABEL, "ZoneUtil::LookupTimezoneByCountry use tzlookup.xml");
        GetCountryZones(region, defaultTimezone, isBoosted, zones);
        if (defaultTimezone.length() == 0) {
            HiLog::Error(LABEL, "ZoneUtil::LookupTimezoneByCountry can't find default timezone for region %{public}s",
                region.c_str());
        }
    } else {
        HiLog::Info(LABEL, "ZoneUtil::LookupTimezoneByCountry use icu data");
        GetICUCountryZones(region, zones, defaultTimezone);
    }
    result.timezoneId = defaultTimezone;
    if (isBoosted) {
        return result;
    }
    if (zones.size() == 0) {
        result.quality = MatchQuality::MULTIPLE_ZONES_DIFFERENT_OFFSET;
    } else if (zones.size() == 1) {
        result.quality = MatchQuality::SINGLE_ZONE;
    } else if (CheckSameDstOffset(zones, defaultTimezone, currentMillis)) {
        result.quality = MatchQuality::MULTIPLE_ZONES_SAME_OFFSET;
    } else {
        result.quality = MatchQuality::MULTIPLE_ZONES_DIFFERENT_OFFSET;
    }
    return result;
}

bool ZoneUtil::CheckFileExist()
{
    using std::filesystem::directory_iterator;
    struct stat s;
    if (stat(COUNTRY_ZONE_DATA_PATH, &s) == 0) {
        return true;
    } else {
        return false;
    }
}

void ZoneUtil::GetCountryZones(std::string &region, std::string &defaultTimzone, bool &isBoosted,
    std::vector<std::string> &zones)
{
    xmlKeepBlanksDefault(0);
    xmlDocPtr doc = xmlParseFile(COUNTRY_ZONE_DATA_PATH);
    if (!doc) {
        HiLog::Error(LABEL, "ZoneUtil::GetCountryZones can not open tzlookup.xml");
        return;
    }
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(ROOT_TAG)) != 0) {
        xmlFreeDoc(doc);
        HiLog::Error(LABEL, "ZoneUtil::GetCountryZones invalid Root_tag");
        return;
    }
    cur = cur->xmlChildrenNode;
    xmlNodePtr value;
    bool findCountry = false;
    while (cur != nullptr && xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(SECOND_TAG)) == 0) {
        value = cur->xmlChildrenNode;
        if (xmlStrcmp(value->name, reinterpret_cast<const xmlChar*>(CODE_TAG)) != 0) {
            HiLog::Error(LABEL, "ZoneUtil::GetCountryZones invalid code_tag");
            return;
        }
        xmlChar *codePtr = xmlNodeGetContent(value);
        if (strcmp(region.c_str(), reinterpret_cast<const char*>(codePtr)) == 0) {
            findCountry = true;
            xmlFree(codePtr);
            break;
        } else {
            xmlFree(codePtr);
            cur = cur->next;
            continue;
        }
    }
    if (findCountry) {
        value = value->next;
        GetDefaultAndBoost(value, defaultTimzone, isBoosted, zones);        
    }
    xmlFreeDoc(doc);
    return;
}

void ZoneUtil::GetDefaultAndBoost(xmlNodePtr &value, std::string &defaultTimezone, bool &isBoosted,
    std::vector<std::string> &zones)
{
    if (value == nullptr || xmlStrcmp(value->name, reinterpret_cast<const xmlChar*>(DEFAULT_TAG)) != 0) {
        HiLog::Error(LABEL, "ZoneUtil::GetDefaultAndBoost invalid default_tag");
        return;
    }
    xmlChar *defaultPtr = xmlNodeGetContent(value);
    defaultTimezone = reinterpret_cast<const char*>(defaultPtr);
    xmlFree(defaultPtr);
    value = value->next;
    if (value == nullptr) {
        HiLog::Error(LABEL, "ZoneUtil::GetDefaultAndBoost doesn't contains id");
        return;
    }
    if (xmlStrcmp(value->name, reinterpret_cast<const xmlChar *>(BOOSTED_TAG)) == 0) {
        isBoosted = true;
        value = value->next;
    } else {
        isBoosted = false;
    }
    GetTimezones(value, zones);
}

void ZoneUtil::GetTimezones(xmlNodePtr &value, std::vector<std::string> &zones)
{
    if (xmlStrcmp(value->name, reinterpret_cast<const xmlChar *>(TIMEZONES_TAG)) != 0) {
        HiLog::Error(LABEL, "ZoneUtil::GetTimezones invalid timezones_tag");
        return;
    }
    value = value->xmlChildrenNode;
    while (value != nullptr) {
        if (xmlStrcmp(value->name, reinterpret_cast<const xmlChar *>(ID_TAG)) != 0) {
            HiLog::Error(LABEL, "ZoneUtil::GetTimezones invalid id_tag");
            return;
        }
        xmlChar *idPtr = xmlNodeGetContent(value);
        zones.push_back(reinterpret_cast<const char*>(idPtr));
        xmlFree(idPtr);
        value = value->next;
    }
}

void ZoneUtil::GetICUCountryZones(std::string &region, std::vector<std::string> &zones, std::string &defaultTimezone)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    std::set<std::string> validZoneIds = I18nTimeZone::GetAvailableIDs(errorCode);
    if (errorCode != I18nErrorCode::SUCCESS) {
        HiLog::Error(LABEL, "ZoneUtil::LookupTimezoneByNITZ can not get icu data");
    }
    std::set<std::string> countryZoneIds;
    StringEnumeration *strEnum = TimeZone::createEnumeration(region.c_str());
    UErrorCode status = U_ZERO_ERROR;
    const UnicodeString *timezoneIdUStr = strEnum->snext(status);
    while (timezoneIdUStr != nullptr && U_SUCCESS(status)) {
        UnicodeString canonicalUnistring;
        TimeZone::getCanonicalID(*timezoneIdUStr, canonicalUnistring, status);
        std::string timezoneId;
        canonicalUnistring.toUTF8String(timezoneId);
        if (validZoneIds.find(timezoneId) != validZoneIds.end()) {
            countryZoneIds.insert(timezoneId);
        }
        timezoneIdUStr = strEnum->snext(status);
    }
    for (auto it = countryZoneIds.begin(); it != countryZoneIds.end(); ++it) {
        zones.push_back(*it);
    }
    if (defaultMap.find(region) != defaultMap.end()) {
        defaultTimezone = defaultMap[region];
    } else {
        if (zones.size() > 0) {
            defaultTimezone = zones[0];
        }
    }
}

CountryResult ZoneUtil::Match(std::vector<std::string> &zones, NITZData &nitzData, std::string &systemTimezone)
{
    bool isOnlyMatch = true;
    std::string matchedZoneId;
    bool local = false;
    bool useSystemTimezone = false;
    for (size_t i = 0; i < zones.size(); i++) {
        std::string zoneId = zones[i];
        UnicodeString unicodeZoneID(zoneId.data(), zoneId.length());
        TimeZone *timezone = TimeZone::createTimeZone(unicodeZoneID);
        int32_t rawOffset;
        int32_t dstOffset;
        UErrorCode status = UErrorCode::U_ZERO_ERROR;
        timezone->getOffset(nitzData.currentMillis, static_cast<UBool>(local), rawOffset, dstOffset, status);
        if ((nitzData.totalOffset - rawOffset == dstOffset) &&
            (nitzData.isDST < 0 || nitzData.isDST == (dstOffset != 0))) {
            if (matchedZoneId.length() == 0) {
                matchedZoneId = zoneId;
            } else {
                isOnlyMatch = false;
            }
            if (strcmp(zoneId.c_str(), systemTimezone.c_str()) == 0) {
                matchedZoneId = systemTimezone;
                useSystemTimezone = true;
            }
            if (!isOnlyMatch && useSystemTimezone) {
                break;
            }
        }
    }
    CountryResult result = {isOnlyMatch, MatchQuality::DEFAULT_BOOSTED, matchedZoneId};
    return result;
}

bool ZoneUtil::CheckSameDstOffset(std::vector<std::string> &zones, std::string &defaultTimezoneId,
    int64_t currentMillis) {
    UnicodeString defaultID(defaultTimezoneId.data(), defaultTimezoneId.length());
    TimeZone *defaultTimezone = TimeZone::createTimeZone(defaultID);
    int32_t rawOffset = 0;
    int32_t dstOffset = 0;
    bool local = false;
    UErrorCode status = U_ZERO_ERROR;
    defaultTimezone->getOffset(currentMillis, (UBool)local, rawOffset, dstOffset, status);
    if (U_FAILURE(status)) {
        HiLog::Error(LABEL, "ZoneUtil::CheckSameDstOffset can not get timezone offset");
        return false;
    }
    int32_t totalOffset = rawOffset + dstOffset;
    for (int32_t i = 0; i < zones.size(); i++) {
        UnicodeString unicodeZoneID(zones[i].data(), zones[i].length());
        TimeZone *timezone = TimeZone::createTimeZone(unicodeZoneID);
        timezone->getOffset(currentMillis, (UBool)local, rawOffset, dstOffset, status);
        if (U_FAILURE(status)) {
            HiLog::Error(LABEL, "ZoneUtil::CheckSameDstOffset can not get timezone offset");
            return false;
        }
        if (totalOffset - rawOffset != dstOffset) {
            return false;
        }
    }
    return true;
}
