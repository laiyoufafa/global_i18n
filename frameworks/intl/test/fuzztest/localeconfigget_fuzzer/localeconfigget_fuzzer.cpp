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

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "locale_config.h"
#include "localeconfigget_fuzzer.h"

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        LocaleConfig::GetSystemLanguage();
        LocaleConfig::GetSystemRegion();
        LocaleConfig::GetSystemLocale();
        std::vector<std::string> ret;
        LocaleConfig::GetSystemLanguages(ret);
        LocaleConfig::GetSystemCountries(ret);

        std::string input(reinterpret_cast<const char*>(data), size);
        LocaleConfig::IsSuggested(input);
        const size_t minimalLocaleLength = 2;
        if (input.length() > minimalLocaleLength) {
            std::string firstInput(input, 0, minimalLocaleLength);
            std::string secondInput(input, minimalLocaleLength);
            LocaleConfig::IsSuggested(firstInput, secondInput);
            LocaleConfig::GetDisplayLanguage(firstInput, secondInput, true);
            LocaleConfig::GetDisplayLanguage(firstInput, secondInput, false);
            LocaleConfig::GetDisplayRegion(firstInput, secondInput, true);
            LocaleConfig::GetDisplayRegion(firstInput, secondInput, false);
        }
        LocaleConfig::IsRTL(input);
        LocaleConfig::GetValidLocale(input);

        return true;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}

