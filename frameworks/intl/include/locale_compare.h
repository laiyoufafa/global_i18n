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
#ifndef OHOS_GLOBAL_I18N_LOCALE_COMPARE_H
#define OHOS_GLOBAL_I18N_LOCALE_COMPARE_H

#include <map>
#include <set>
#include <string>

namespace OHOS {
namespace Global {
namespace I18n {
class LocaleCompare {
public:
    static int32_t Compare(const std::string& localeTag1, const std::string& localeTag2);

private:
    static bool IsSameLanguage(const std::string& langTag1, const std::string& langTag2);
    static bool IsSameEnglishScript(const std::string& scriptTag1, const std::string& scriptTag2);
    static bool HasMapRelation(const std::string& languageTag, const std::string& localeTag1,
        const std::string& localeTag2);
    static std::string hantSegment;
    static std::string latnSegment;
    static std::string qaagSegment;
    static std::set<std::string> scriptLocales;
    static std::map<std::string, std::string> hantParent;
    static std::map<std::string, std::string> latnParent;
    static std::map<std::string, std::string> extendedHantParent;
    static std::map<std::string, std::string> extendedLatnParent;
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif