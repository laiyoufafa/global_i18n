/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "hilog/log.h"
#include "locale_config.h"
#include "system_locale_manager.h"
#include "utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "SystemLocaleManager" };
using namespace OHOS::HiviewDFX;

const char* SystemLocaleManager::SIM_COUNTRY_CODE_KEY = "telephony.sim.countryCode0";

SystemLocaleManager::SystemLocaleManager()
{
    tabooUtils = std::make_unique<TabooUtils>();
}

SystemLocaleManager::~SystemLocaleManager()
{
}

/**
 * Language arrays are sorted according to the following steps:
 * 1. Remove blocked languages.
 * 2. Compute language locale displayName; If options.isUseLocalName is true, compute language local displayName.
 *    replace display name with taboo data.
 * 3. Judge whether language is suggested with system region and sim card region.
 * 4. Sort the languages use locale displayName, local displyName and suggestion infomation.
 */
std::vector<LocaleItem> SystemLocaleManager::GetLanguageInfoArray(const std::vector<std::string> &languages,
        const SortOptions &options)
{
    std::unordered_set<std::string> blockedLanguages = LocaleConfig::GetBlockedLanguages();
    std::vector<LocaleItem> localeItemList;
    for (auto it = languages.begin(); it != languages.end(); ++it) {
        if (blockedLanguages.find(*it) != blockedLanguages.end()) {
            continue;
        }
        std::string languageDisplayName = LocaleConfig::GetDisplayLanguage(*it, options.localeTag, true);
        languageDisplayName = tabooUtils->ReplaceLanguageName(*it, options.localeTag, languageDisplayName);
        std::string languageNativeName;
        if (options.isUseLocalName) {
            languageNativeName = LocaleConfig::GetDisplayLanguage(*it, *it, true);
            languageNativeName = tabooUtils->ReplaceLanguageName(*it, *it, languageNativeName);
        }
        bool isSuggestedWithSystemRegion = LocaleConfig::IsSuggested(*it, LocaleConfig::GetSystemRegion());
        std::string simRegion = ReadSystemParameter(SIM_COUNTRY_CODE_KEY, CONFIG_LEN);
        bool isSuggestedWithSimRegion = false;
        if (simRegion.length() > 0) {
            isSuggestedWithSimRegion = LocaleConfig::IsSuggested(*it, simRegion);
        }
        SuggestionType suggestionType = SuggestionType::SUGGESTION_TYPE_NONE;
        if (isSuggestedWithSimRegion) {
            suggestionType = SuggestionType::SUGGESTION_TYPE_SIM;
        } else if (isSuggestedWithSystemRegion) {
            suggestionType = SuggestionType::SUGGESTION_TYPE_RELATED;
        }
        LocaleItem item { *it, suggestionType, languageDisplayName, languageNativeName };
        localeItemList.push_back(item);
    }
    SortLocaleItemList(localeItemList, options);
    return localeItemList;
}

/**
 * Region arrays are sorted according to the following steps:
 * 1. Remove blocked regions and blocked regions under system Language.
 * 2. Compute region locale displayName; replace display name with taboo data.
 * 3. Judge whether region is suggested with system language.
 * 4. Sort the regions use locale displayName and suggestion infomation.
 */
std::vector<LocaleItem> SystemLocaleManager::GetCountryInfoArray(const std::vector<std::string> &countries,
    const SortOptions &options)
{
    std::unordered_set<std::string> blockedRegions = LocaleConfig::GetBlockedRegions();
    std::unordered_set<std::string> blockedLanguageRegions = LocaleConfig::GetLanguageBlockedRegions();
    std::vector<LocaleItem> localeItemList;
    for (auto it = countries.begin(); it != countries.end(); ++it) {
        if (blockedRegions.find(*it) != blockedRegions.end() || blockedLanguageRegions.find(*it) !=
            blockedLanguageRegions.end()) {
            continue;
        }
        std::string regionDisplayName = LocaleConfig::GetDisplayRegion(*it, options.localeTag, true);
        regionDisplayName = tabooUtils->ReplaceCountryName(*it, options.localeTag, regionDisplayName);
        bool isSuggestedRegion = LocaleConfig::IsSuggested(LocaleConfig::GetSystemLanguage(), *it);
        SuggestionType suggestionType = SuggestionType::SUGGESTION_TYPE_NONE;
        if (isSuggestedRegion) {
            suggestionType = SuggestionType::SUGGESTION_TYPE_RELATED;
        }
        LocaleItem item { *it, suggestionType, regionDisplayName, "" };
        localeItemList.push_back(item);
    }
    SortLocaleItemList(localeItemList, options);
    return localeItemList;
}

void SystemLocaleManager::SortLocaleItemList(std::vector<LocaleItem> &localeItemList, const SortOptions &options)
{
    std::vector<std::string> collatorLocaleTags { options.localeTag };
    std::map<std::string, std::string> collatorOptions {};
    Collator *collator = new Collator(collatorLocaleTags, collatorOptions);
    auto compareFunc = [collator](LocaleItem item1, LocaleItem item2) {
        if (item1.suggestionType < item2.suggestionType) {
            return false;
        } else if (item1.suggestionType > item2.suggestionType) {
            return true;
        }
        CompareResult result = CompareResult::INVALID;
        if (item1.localName.length() != 0) {
            result = collator->Compare(item1.localName, item2.localName);
            if (result == CompareResult::SMALLER) {
                return true;
            }
            if (result == CompareResult::INVALID) {
                HiLog::Error(LABEL, "SystemLocaleManager: invalid compare result for local name.");
            }
            return false;
        }
        result = collator->Compare(item1.displayName, item2.displayName);
        if (result == CompareResult::SMALLER) {
            return true;
        }
        if (result == CompareResult::INVALID) {
            HiLog::Error(LABEL, "SystemLocaleManager: invalid compare result for display name.");
        }
        return false;
    };
    if (options.isSuggestedFirst) {
        std::sort(localeItemList.begin(), localeItemList.end(), compareFunc);
    } else {
        std::sort(localeItemList.begin(), localeItemList.end(), compareFunc);
    }
    delete collator;
}
} // I18n
} // Global
} // OHOS