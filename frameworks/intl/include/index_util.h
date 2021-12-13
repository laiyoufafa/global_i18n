/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#ifndef GLOBAL_I18N_STANDARD_INDEX_UTIL_H
#define GLOBAL_I18N_STANDARD_INDEX_UTIL_H

#include <string>
#include <vector>
#include "unicode/alphaindex.h"

namespace OHOS {
namespace Global {
namespace I18n {
class IndexUtil {
public:
    IndexUtil(const std::string &localeTag);
    ~IndexUtil();
    std::vector<std::string> GetIndexList();
    void AddLocale(const std::string &localeTag);
    std::string GetIndex(const std::string &String);

private:
    icu::AlphabeticIndex *index;
};
}
}
}
#endif