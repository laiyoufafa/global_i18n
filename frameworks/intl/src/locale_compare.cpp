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
#include "locale_compare.h"
#include "ohos/init_data.h"
#include "unicode/locid.h"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "LocaleCompare" };
using namespace OHOS::HiviewDFX;
std::string LocaleCompare::hantSegment = "-Hant-";
std::string LocaleCompare::latnSegment = "-Latn-";
std::string LocaleCompare::qaagSegment = "-Qaag-";
std::set<std::string> LocaleCompare::scriptLocales {
    "zh", "en", "es", "pt"
};
std::map<std::string, std::string> LocaleCompare::hantParent {
    { "zh-MO", "zh-Hant-HK" }
};
std::map<std::string, std::string> LocaleCompare::latnParent {
    { "en-150", "en-001" },
    { "en-AG", "en-001" },
    { "en-AI", "en-001" },
    { "en-AU", "en-001" },
    { "en-BB", "en-001" },
    { "en-BE", "en-001" },
    { "en-BM", "en-001" },
    { "en-BS", "en-001" },
    { "en-BZ", "en-001" },
    { "en-CC", "en-001" },
    { "en-CK", "en-001" },
    { "en-CX", "en-001" },
    { "en-DG", "en-001" },
    { "en-ER", "en-001" },
    { "en-FK", "en-001" },
    { "en-FM", "en-001" },
    { "en-GB", "en-001" },
    { "en-GD", "en-001" },
    { "en-GG", "en-001" },
    { "en-GI", "en-001" },
    { "en-GY", "en-001" },
    { "en-HK", "en-001" },
    { "en-IE", "en-001" },
    { "en-IM", "en-001" },
    { "en-IN", "en-001" },
    { "en-IO", "en-001" },
    { "en-JE", "en-001" },
    { "en-KI", "en-001" },
    { "en-KN", "en-001" },
    { "en-KY", "en-001" },
    { "en-LC", "en-001" },
    { "en-LR", "en-001" },
    { "en-LS", "en-001" },
    { "en-MM", "en-001" },
    { "en-MO", "en-001" },
    { "en-MS", "en-001" },
    { "en-MT", "en-001" },
    { "en-MY", "en-001" },
    { "en-NF", "en-001" },
    { "en-NR", "en-001" },
    { "en-NU", "en-001" },
    { "en-NZ", "en-001" },
    { "en-PG", "en-001" },
    { "en-PK", "en-001" },
    { "en-PN", "en-001" },
    { "en-PW", "en-001" },
    { "en-SB", "en-001" },
    { "en-SC", "en-001" },
    { "en-SD", "en-001" },
    { "en-SG", "en-001" },
    { "en-SH", "en-001" },
    { "en-SL", "en-001" },
    { "en-SS", "en-001" },
    { "en-SX", "en-001" },
    { "en-SZ", "en-001" },
    { "en-TC", "en-001" },
    { "en-TK", "en-001" },
    { "en-TT", "en-001" },
    { "en-TV", "en-001" },
    { "en-VC", "en-001" },
    { "en-VG", "en-001" },
    { "en-WS", "en-001" },
    { "en-ZG", "en-001" },
    { "es-AR", "es-419" },
    { "es-BO", "es-419" },
    { "es-BR", "es-419" },
    { "es-CL", "es-419" },
    { "es-CO", "es-419" },
    { "es-CR", "es-419" },
    { "es-CU", "es-419" },
    { "es-DO", "es-419" },
    { "es-EC", "es-419" },
    { "es-GT", "es-419" },
    { "es-HN", "es-419" },
    { "es-MX", "es-419" },
    { "es-NI", "es-419" },
    { "es-PA", "es-419" },
    { "es-PE", "es-419" },
    { "es-PR", "es-419" },
    { "es-PY", "es-419" },
    { "es-SV", "es-419" },
    { "es-US", "es-419" },
    { "es-UY", "es-419" },
    { "es-VE", "es-419" },
    { "pt-AO", "pt-PT" },
    { "pt-CH", "pt-PT" },
    { "pt-CV", "pt-PT" },
    { "pt-GQ", "pt-PT" },
    { "pt-GW", "pt-PT" },
    { "pt-LU", "pt-PT" },
    { "pt-MO", "pt-PT" },
    { "pt-MZ", "pt-PT" },
    { "pt-ST", "pt-PT" },
    { "pt-TL", "pt-PT" }
};
std::map<std::string, std::string> LocaleCompare::extendedHantParent {};
std::map<std::string, std::string> LocaleCompare::extendedLatnParent {};

int32_t LocaleCompare::Compare(const std::string& localeTag1, const std::string& localeTag2)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale locale1 = icu::Locale::forLanguageTag(icu::StringPiece(localeTag1), status);
    icu::Locale locale2 = icu::Locale::forLanguageTag(icu::StringPiece(localeTag2), status);
    int32_t segmentScore = 3;
    int32_t mapScore = 0;
    int32_t score = 0;
    std::string language1 = locale1.getLanguage();
    std::string language2 = locale2.getLanguage();
    if (IsSameLanguage(language1, language2)) {
        score += segmentScore;
    } else {
        return -1;
    }
    std::string localeBaseName1 = locale1.getBaseName();
    std::string localeBaseName2 = locale2.getBaseName();
    if (HasMapRelation(language1, localeBaseName1, localeBaseName2)) {
        return mapScore;
    }
    std::string region1 = locale1.getCountry();
    std::string region2 = locale2.getCountry();
    locale1.addLikelySubtags(status);
    locale2.addLikelySubtags(status);
    if (U_FAILURE(status)) {
        HiLog::Error(LABEL, "LocaleCompare::Compare add likely subtags failed.");
        return -1;
    }
    std::string script1 = locale1.getScript();
    std::string script2 = locale2.getScript();
    if (script1.compare(script2) == 0 || (language1.compare("en") == 0 && IsSameEnglishScript(script1, script2))) {
        score += segmentScore;
        if (region2.length() == 0) {
            ++score;
        }
    } else {
        return -1;
    }
    if (region1.length() != 0 && region1.compare(region2) == 0) {
        score += segmentScore;
    }
    return score;
}

bool LocaleCompare::IsSameLanguage(const std::string& langTag1, const std::string& langTag2)
{
    if (langTag1.compare(langTag2) == 0) {
        return true;
    }
    if (langTag1.compare("tl") == 0 && langTag2.compare("fil") == 0) {
        return true;
    }
    if (langTag1.compare("fil") == 0 && langTag2.compare("tl") == 0) {
        return true;
    }
    return false;
}

bool LocaleCompare::IsSameEnglishScript(const std::string& scriptTag1, const std::string& scriptTag2)
{
    if (scriptTag1.compare("Qaag") == 0 && scriptTag2.compare("Latn") == 0) {
        return true;
    }
    if (scriptTag1.compare("Latn") == 0 && scriptTag2.compare("Qaag") == 0) {
        return true;
    }
    return false;
}

bool LocaleCompare::HasMapRelation(const std::string& languageTag, const std::string& localeTag1,
        const std::string& localeTag2)
{
    if (scriptLocales.find(languageTag) == scriptLocales.end()) {
        return false;
    }
    if (hantParent.find(localeTag1) != hantParent.end()) {
        if (localeTag2.compare(hantParent[localeTag1]) == 0) {
            return true;
        }
    }
    if (latnParent.find(localeTag1) != latnParent.end()) {
        if (localeTag2.compare(hantParent[localeTag1]) == 0) {
            return true;
        }
    }
    if (extendedHantParent.size() == 0) {
        for (auto it = hantParent.begin(); it != hantParent.end(); ++it) {
            std::string key = it->first;
            size_t languageLength = key.find("-");
            std::string language = key.substr(0, languageLength);
            std::string region = key.substr(languageLength + 1);
            extendedHantParent[language + hantSegment + region] = it->second;
        }
    }
    if (extendedHantParent.find(localeTag1) != extendedHantParent.end()) {
        if (localeTag2.compare(extendedHantParent[localeTag1]) == 0) {
            return true;
        }
    }
    if (extendedLatnParent.size() == 0) {
        for (auto it = latnParent.begin(); it != latnParent.end(); ++it) {
            std::string key = it->first;
            size_t languageLength = key.find("-");
            std::string language = key.substr(0, languageLength);
            std::string region = key.substr(languageLength + 1);
            extendedHantParent[language + latnSegment + region] = it->second;
            if (language.compare("en") == 0) {
                extendedLatnParent[language + qaagSegment + region] = it->second;
            }
        }
    }
    if (extendedLatnParent.find(localeTag1) != extendedLatnParent.end()) {
        if (localeTag2.compare(extendedLatnParent[localeTag1]) == 0) {
            return true;
        }
    }
    return false;
}
} // namespace I18n
} // namespace Global
} // OHOS
