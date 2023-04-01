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

#ifndef OHOS_GLOBAL_I18N_ZONE_UTIL_H
#define OHOS_GLOBAL_I18N_ZONE_UTIL_H

#include <string>
#include <unordered_map>
#include <vector>
#include "libxml/globals.h"
#include "libxml/tree.h"
#include "libxml/xmlstring.h"
#include "phonenumbers/phonenumberutil.h"
#include "strenum.h"

namespace OHOS {
namespace Global {
namespace I18n {
struct NITZData {
    // smaller than 0 means null, 0 means false, 1 means true
    int32_t isDST;
    int32_t totalOffset;
    int64_t currentMillis;
};

enum MatchQuality {
    DEFAULT_BOOSTED = 0,
    SINGLE_ZONE,
    MULTIPLE_ZONES_SAME_OFFSET,
    MULTIPLE_ZONES_DIFFERENT_OFFSET
};

struct CountryResult {
    bool isOnlyMatch;
    MatchQuality quality;
    std::string timezoneId;
};

class ZoneUtil {
public:
    /**
     * default constructor
     */
    ZoneUtil() : phone_util(*i18n::phonenumbers::PhoneNumberUtil::GetInstance()) {}

    /**
     * deconstructor
     */
    ~ZoneUtil() {}

    /**
     * @brief Get the default timezone for the given country
     *
     * @param country Indicating the country code
     * @return Returns the default timezone if the country code is valid, otherwise
     * returns an empty string.
     */
    std::string GetDefaultZone(const std::string &country);

    /**
     * @brief Get the default timezone for the given region code
     *
     * @param number Indicating the region code, for example 86 can
     * be used to retrieve the default timezone of China.
     * @return Returns the default timezone name if the region code is valid, otherwise
     * returns an empty string.
     */
    std::string GetDefaultZone(const int32_t number);

    /**
     * @brief Get the default timezone name for the given country code
     *
     * @param country Indicating the country code
     * @param offset Indicating the offset from GMT(in milliseconds)
     * @return Returns the default timezone name if the country code is valid, otherwise
     * returns an empty string.
     */
    std::string GetDefaultZone(const std::string country, const int32_t offset);

    /**
     * @brief Get the default timezone name for the given region code
     *
     * @param number Indicating the region code, for example 86 can
     * be used to retrieve the default timezone of China.
     * @param offset Indicating the offset from GMT(in milliseconds).
     * @return Returns the default timezone name if the country code is valid, otherwise
     * returns an empty string.
     */
    std::string GetDefaultZone(const int32_t number, const int32_t offset);

    /**
     * @brief Get the timezone list for the given country code
     *
     * @param country Indicating the country code
     * @param retVec used to store the returned timezones
     */
    void GetZoneList(const std::string country, std::vector<std::string> &retVec);

    /**
     * @brief Get the timezone list for the given country code
     *
     * @param country Indicating the country code
     * @param offset Indicating the offset from GMT(in milliseconds)
     * @param retVec used to store the returned timezones
     */
    void GetZoneList(const std::string country, const int32_t offset, std::vector<std::string> &retVec);

    /**
     * @brief Get recommended timezone by country and nitz information.
     *
     * @param region Indicating the country code
     * @param nitzData Indicating the nitz information
     * @return Returns the CountryResult object which contains recommended timezone id and match information.
     */
    CountryResult LookupTimezoneByCountryAndNITZ(std::string &region, NITZData &nitzData);

    /**
     * @brief Get recommended timezone nitz information. This method is used when country code is not available.
     *
     * @param nitzData Indicating the nitz information
     * @return Returns the CountryResult object which contains recommended timezone id and match information.
     */
    CountryResult LookupTimezoneByNITZ(NITZData &nitzData);

    /**
     * @brief Get recommended timezone by country information.
     *
     * @param region Indicating the country code
     * @param currentMillis Indicating the current utc time.
     * @return Returns the CountryResult object which contains recommended timezone id and match information.
     */
    CountryResult LookupTimezoneByCountry(std::string &region, int64_t currentMillis);

private:
    const i18n::phonenumbers::PhoneNumberUtil &phone_util;
    static std::unordered_map<std::string, std::string> defaultMap;
    static bool icuInitialized;
    static void GetList(icu::StringEnumeration *strEnum, std::vector<std::string> &ret);
    static void GetString(icu::StringEnumeration *strEnum, std::string &ret);
    static bool Init();
    bool CheckFileExist();
    bool CheckSameDstOffset(std::vector<std::string> &zones, std::string &defaultTimezone,
        int64_t currentMillis);
    void GetTimezones(xmlNodePtr &value, std::vector<std::string> &zones);
    void GetDefaultAndBoost(xmlNodePtr &value, std::string &defaultTimezone, bool &isBoosted,
        std::vector<std::string> &zones);
    void GetCountryZones(std::string &region, std::string &defaultTimezone, bool &isBoosted,
        std::vector<std::string> &zones);
    void GetICUCountryZones(std::string &region, std::vector<std::string> &zones, std::string &defaultTimezone);
    CountryResult Match(std::vector<std::string> &zones, NITZData &nitzData, std::string &systemTimezone);

    static const char *COUNTRY_ZONE_DATA_PATH;
    static const char *DEFAULT_TIMEZONE;
    static const char *TIMEZONES_TAG;
    static const char *ID_TAG;
    static const char *DEFAULT_TAG;
    static const char *BOOSTED_TAG;
    static const char *ROOT_TAG;
    static const char *SECOND_TAG;
    static const char *CODE_TAG;
    static const char *TIMEZONE_KEY;
    static constexpr int SYS_PARAM_LEN = 128;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif
