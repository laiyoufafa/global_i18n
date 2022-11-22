/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except", "in compliance with the License.
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
#ifndef OHOS_GLOBAL_I18N_TIMEZONE_H
#define OHOS_GLOBAL_I18N_TIMEZONE_H

#include <stdint.h>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "memory"
#include "unicode/timezone.h"

namespace OHOS {
namespace Global {
namespace I18n {
class I18nTimeZone {
public:
    I18nTimeZone(std::string &id, bool isZoneID);
    ~I18nTimeZone();
    int32_t GetOffset(double date);
    int32_t GetRawOffset();
    std::string GetID();
    std::string GetDisplayName();
    std::string GetDisplayName(bool isDST);
    std::string GetDisplayName(std::string localeStr);
    std::string GetDisplayName(std::string localeStr, bool isDST);
    static std::unique_ptr<I18nTimeZone> CreateInstance(std::string &id, bool isZoneID);
    static std::set<std::string> GetAvailableIDs();
    static std::vector<std::string> GetAvailableZoneCityIDs();
    static std::string GetCityDisplayName(std::string &cityID, std::string &locale);

private:
    static const char *TIMEZONE_KEY;
    static const char *DEFAULT_TIMEZONE;
    static const char *DEFAULT_LANGUAGE;
    static const char *DEFAULT_LOCALE;
    static const char *TIMEZONES_PATH;
    static const char *SUPPORT_LOCALES_PATH;
    static const char *rootTag;
    static const char *secondRootTag;
    static const uint32_t ELEMENT_NUM = 3;
    static const char *supportLocalesTag;
    static std::string displayLocale;
    static bool isInitialized;
    static std::set<std::string> availableIDs;
    static std::vector<std::string> availableZoneCityIDs;
    static std::map<std::string, std::string> city2DisplayName;
    static std::map<std::string, std::string> city2TimeZoneID;
    static std::map<std::string, std::string> supportLocales;
    static constexpr int SYS_PARAM_LEN = 128;
    icu::TimeZone *timezone = nullptr;

    static void ReadTimeZoneData(const char *xmlPath);
    static std::string ComputeLocale(std::string &locale);
    icu::TimeZone* GetTimeZone();
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif