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

#ifndef OHOS_GLOBAL_I18N_TABOO_UTILS_H
#define OHOS_GLOBAL_I18N_TABOO_UTILS_H

#include "taboo.h"

namespace OHOS {
namespace Global {
namespace I18n {
class TabooUtils {
public:
    TabooUtils();
    ~TabooUtils();
    std::string ReplaceCountryName(const std::string& region, const std::string& displayLanguage,
        const std::string& name);
    std::string ReplaceLanguageName(const std::string& language, const std::string& displayLanguage,
        const std::string& name);

private:
    std::shared_ptr<Taboo> GetLatestTaboo();
    std::shared_ptr<Taboo> systemTaboo;
    std::string systemTabooDataPath = "/system/etc/taboo/";
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif