/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "hisysevent.h"
#include "hisysevent_adapter.h"

namespace OHOS {
namespace Global {
namespace I18n {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "I18n_Hisysevent" };
using OHOS::HiviewDFX::HiLog;
using HiSysEventNameSpace = OHOS::HiviewDFX::HiSysEvent;
const std::string DOMAIN_STR = std::string(HiSysEventNameSpace::Domain::GLOBAL_I18N);

void ReportInitI18nFail(const std::string& errMsg)
{
    int ret = HiSysEventNameSpace::Write(DOMAIN_STR, "INIT_I18N_FAILED",
        HiSysEventNameSpace::EventType::FAULT,
        "ERROR_MSG", errMsg);
    if (ret != 0) {
        HiLog::Error(LABEL, "I18n Hisysevent write failed");
    }
}

void ReportI18nParamError(const std::string& errMsg)
{
    int ret = HiSysEventNameSpace::Write(DOMAIN_STR, "I18N_PARAM_ERROR",
        HiSysEventNameSpace::EventType::FAULT,
        "ERROR_MSG", errMsg);
    if (ret != 0) {
        HiLog::Error(LABEL, "I18n Hisysevent write failed");
    }
}
} // I18n
} // Global
} // OHOS