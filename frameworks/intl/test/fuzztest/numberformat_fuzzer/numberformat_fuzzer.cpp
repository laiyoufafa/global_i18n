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
#include "number_format.h"
#include "numberformat_fuzzer.h"

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        if (size < 1) {
            return false;
        }
        std::string input(reinterpret_cast<const char*>(data), size);
        std::vector<std::string> localeTags(1, input);
        std::map<std::string, std::string> options;
        options[input] = input;
        NumberFormat formatter(localeTags, options);
        double number = static_cast<double>(data[0]);
        formatter.Format(number);
        formatter.GetResolvedOptions(options);
        formatter.GetCurrency();
        formatter.GetCurrencySign();
        formatter.GetStyle();
        formatter.GetNumberingSystem();
        formatter.GetUseGrouping();
        formatter.GetMinimumIntegerDigits();
        formatter.GetMinimumFractionDigits();
        formatter.GetMaximumFractionDigits();
        formatter.GetMinimumSignificantDigits();
        formatter.GetMaximumSignificantDigits();
        formatter.GetLocaleMatcher();
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

