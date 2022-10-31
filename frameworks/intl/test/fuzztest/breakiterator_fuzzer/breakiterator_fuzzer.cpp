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
#include "breakiterator_fuzzer.h"
#include "i18n_break_iterator.h"

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        using namespace Global::I18n;
        if (size < 1) {
            return false;
        }
        std::string text(reinterpret_cast<const char*>(data), size - 1);
        int32_t number = static_cast<int32_t>(data[size - 1]);
        I18nBreakIterator iterator(text);
        iterator.setText(text.c_str());
        std::string str;
        iterator.getText(str);
        iterator.isBoundary(number);
        iterator.current();
        iterator.first();
        iterator.last();
        iterator.next(number);
        iterator.next();
        iterator.previous();
        iterator.following(number);
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

