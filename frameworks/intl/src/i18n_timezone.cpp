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

namespace OHOS {
namespace Global {
namespace I18n {
const char *I18nTimeZone::TIMEZONE_KEY = "persist.time.timezone";
const char *I18nTimeZone::DEFAULT_TIMEZONE = "GMT";
const char *I18nTimeZone::DEFAULT_LANGUAGE = "/system/usr/ohos_timezone/en-Latn.xml";
const char *I18nTimeZone::DEFAULT_LOCALE = "en-Latn";
const char *I18nTimeZone::TIMEZONES_PATH = "/system/usr/ohos_timezone/";
const char *I18nTimeZone::SUPPORT_LOCALES_PATH = "/system/usr/ohos_timezone/supported_locales.xml";
const char *I18nTimeZone::rootTag = "timezones";
const char *I18nTimeZone::secondRootTag = "timezone";
const char *I18nTimeZone::supportLocalesTag = "supported_locales";
std::string I18nTimeZone::displayLocale = "";
bool I18nTimeZone::isInitialized = false;

std::map<std::string, std::string> I18nTimeZone::supportLocales {};
std::set<std::string> I18nTimeZone::availableIDs {};
std::vector<std::string> I18nTimeZone::availableZoneCityIDs {};
std::map<std::string, std::string> I18nTimeZone::city2DisplayName {};
std::map<std::string, std::string> I18nTimeZone::city2TimeZoneID {};

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
        if (!isInitialized) {
            ReadTimeZoneData(DEFAULT_LANGUAGE);
            isInitialized = true;
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

void I18nTimeZone::ReadTimeZoneData(const char *xmlPath)
{
    xmlKeepBlanksDefault(0);
    if (xmlPath == nullptr) {
        return;
    }
    xmlDocPtr doc = xmlParseFile(xmlPath);
    if (!doc) {
        return;
    }
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(rootTag))) {
        xmlFreeDoc(doc);
        return;
    }
    cur = cur->xmlChildrenNode;
    while (cur != nullptr && !xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(secondRootTag))) {
        xmlNodePtr value = cur->xmlChildrenNode;
        xmlChar *contents[ELEMENT_NUM] = { 0 }; // 3 represent cityid, zoneid, displayname;
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (value != nullptr) {
                contents[i] = xmlNodeGetContent(value);
                value = value->next;
            } else {
                break;
            }
        }
        if (!isInitialized) {
            // 0 represents cityid index, 1 represents zoneid index
            availableZoneCityIDs.insert(availableZoneCityIDs.end(), reinterpret_cast<const char *>(contents[0]));
            availableIDs.insert(reinterpret_cast<const char *>(contents[1]));
            city2TimeZoneID.insert(
                std::make_pair<std::string, std::string>(reinterpret_cast<const char *>(contents[0]),
                                                         reinterpret_cast<const char *>(contents[1])));
        }
        // 0 represents cityid index, 2 represents displayname index
        city2DisplayName.insert(
            std::make_pair<std::string, std::string>(reinterpret_cast<const char *>(contents[0]),
                                                     reinterpret_cast<const char *>(contents[2])));
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (contents[i] != nullptr) {
                xmlFree(contents[i]);
            }
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
}

std::set<std::string> I18nTimeZone::GetAvailableIDs()
{
    if (!isInitialized) {
        ReadTimeZoneData(DEFAULT_LANGUAGE);
        isInitialized = true;
    }
    return availableIDs;
}

std::vector<std::string> I18nTimeZone::GetAvailableZoneCityIDs()
{
    if (!isInitialized) {
        ReadTimeZoneData(DEFAULT_LANGUAGE);
        isInitialized = true;
    }
    return availableZoneCityIDs;
}

std::string I18nTimeZone::ComputeLocale(std::string &locale)
{
    if (supportLocales.size() == 0) {
        xmlKeepBlanksDefault(0);
        xmlDocPtr doc = xmlParseFile(SUPPORT_LOCALES_PATH);
        if (!doc) {
            return DEFAULT_LOCALE;
        }
        xmlNodePtr cur = xmlDocGetRootElement(doc);
        if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(supportLocalesTag))) {
            xmlFreeDoc(doc);
            return DEFAULT_LOCALE;
        }
        cur = cur->xmlChildrenNode;
        while (cur != nullptr) {
            xmlChar *content = xmlNodeGetContent(cur);
            if (content == nullptr) {
                break;
            }
            std::map<std::string, std::string> configs = {};
            LocaleInfo localeinfo(reinterpret_cast<const char*>(content), configs);
            std::string language = localeinfo.GetLanguage();
            std::string script = localeinfo.GetScript();
            std::string languageAndScript = (script.length() == 0) ? language : language + "-" + script;
            LocaleInfo newLocaleInfo(languageAndScript, configs);
            std::string maximizeLocale = newLocaleInfo.Maximize();
            supportLocales.insert(
                std::make_pair<std::string, std::string>(maximizeLocale.c_str(),
                                                         reinterpret_cast<const char*>(content)));
            xmlFree(content);
            cur = cur->next;
        }
    }
    std::map<std::string, std::string> configs = {};
    LocaleInfo localeinfo(locale, configs);
    std::string language = localeinfo.GetLanguage();
    std::string script = localeinfo.GetScript();
    std::string languageAndScript = (script.length() == 0) ? language : language + "-" + script;
    LocaleInfo newLocaleInfo(languageAndScript, configs);
    std::string maximizeLocale = newLocaleInfo.Maximize();
    if (supportLocales.find(maximizeLocale) != supportLocales.end()) {
        return supportLocales.at(maximizeLocale);
    }
    return DEFAULT_LOCALE;
}

std::string I18nTimeZone::GetCityDisplayName(std::string &cityID, std::string &locale)
{
    std::string finalLocale = ComputeLocale(locale);
    if (finalLocale.compare(displayLocale) != 0) {
        std::string xmlPath = TIMEZONES_PATH + finalLocale + ".xml";
        city2DisplayName.clear();
        ReadTimeZoneData(xmlPath.c_str());
        displayLocale = finalLocale;
    }
    if (city2DisplayName.find(cityID) == city2DisplayName.end()) {
        return "";
    }
    return city2DisplayName.at(cityID);
}
}
}
}
