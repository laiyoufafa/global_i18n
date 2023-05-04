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

#include "taboo_utils.h"

namespace OHOS {
namespace Global {
namespace I18n {
TabooUtils::TabooUtils()
{
    systemTaboo = std::make_shared<Taboo>(systemTabooDataPath);
}

TabooUtils::~TabooUtils()
{
}

std::string TabooUtils::ReplaceCountryName(const std::string& region, const std::string& displayLanguage,
    const std::string& name)
{
    std::shared_ptr<Taboo> taboo = GetLatestTaboo();
    return taboo->ReplaceCountryName(region, displayLanguage, name);
}

std::string TabooUtils::ReplaceLanguageName(const std::string& language, const std::string& displayLanguage,
    const std::string& name)
{
    std::shared_ptr<Taboo> taboo = GetLatestTaboo();
    return taboo->ReplaceLanguageName(language, displayLanguage, name);
}

std::shared_ptr<Taboo> TabooUtils::GetLatestTaboo()
{
    return systemTaboo;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS