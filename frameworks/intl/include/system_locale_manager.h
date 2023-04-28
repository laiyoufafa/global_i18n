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

#ifndef OHOS_GLOBAL_SYSTEM_LOCALE_MANAGER_H
#define OHOS_GLOBAL_SYSTEM_LOCALE_MANAGER_H

#include <string>
#include <vector>
#include "collator.h"
#include "taboo_utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
enum SuggestionType {
    // language or region is not suggested with system region or system language.
    SUGGESTION_TYPE_NONE = 0,

    // language is suggested with system region or region is suggested with system language.
    SUGGESTION_TYPE_RELATED = 1,

    // language is suggested with sim card region.
    SUGGESTION_TYPE_SIM = 2,
};

struct LocaleItem {
    // language or region code.
    std::string id;

    // the suggestion type of language or region.
    SuggestionType suggestionType;

    // the name of language or region in specified language.
    std::string displayName;

    // the name of language or region in local language.
    std::string localName;
};

struct SortOptions {
    // locale use to sort language or region.
    std::string localeTag;

    // whether to use local name to sort language or region.
    bool isUseLocalName;

    // whether to put the suggested item at the top
    bool isSuggestedFirst;
};

class SystemLocaleManager {
public:
    SystemLocaleManager();
    ~SystemLocaleManager();

    /**
     * @brief sort input languages according to SortOptions and obtain sorted result.
     *
     * @param languages input language array to be sorted.
     * @param options sort options.
     * @return std::vector<LocaleItem> sort result.
     */
    std::vector<LocaleItem> GetLanguageInfoArray(const std::vector<std::string> &languages,
        const SortOptions &options);

    /**
     * @brief sort input countries according to SortOptions and obtain sorted result.
     *
     * @param regions input country array to be sorted.
     * @param options sort options.
     * @return std::vector<LocaleItem> sort result.
     */
    std::vector<LocaleItem> GetCountryInfoArray(const std::vector<std::string> &countries, const SortOptions &options);

private:
    void SortLocaleItemList(std::vector<LocaleItem> &localeItemList, const SortOptions &options);
    std::unique_ptr<TabooUtils> tabooUtils;
    static const char* SIM_COUNTRY_CODE_KEY;
    static constexpr int CONFIG_LEN = 128;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif