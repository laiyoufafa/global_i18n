/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "phone_number_format.h"

#include <dlfcn.h>

#include "locid.h"
#include "map"
#include "new"
#include "set"
#include "string"
#include "utility"

namespace OHOS {
namespace Global {
namespace I18n {
const int RECV_CHAR_LEN = 100;
using i18n::phonenumbers::PhoneNumberUtil;
using ExposeLocationName = void (*)(const char*, const char*, char*);
void* g_dynamicHandler;
ExposeLocationName g_func;

PhoneNumberFormat::PhoneNumberFormat(const std::string &countryTag,
                                     const std::map<std::string, std::string> &options)
{
    util = PhoneNumberUtil::GetInstance();
    country = countryTag;
    std::string type = "";
    auto search = options.find("type");
    if (search != options.end()) {
        type = search->second;
    }

    std::map<std::string, PhoneNumberUtil::PhoneNumberFormat> type2PhoneNumberFormat = {
        {"E164", PhoneNumberUtil::PhoneNumberFormat::E164},
        {"RFC3966", PhoneNumberUtil::PhoneNumberFormat::RFC3966},
        {"INTERNATIONAL", PhoneNumberUtil::PhoneNumberFormat::INTERNATIONAL},
        {"NATIONAL", PhoneNumberUtil::PhoneNumberFormat::NATIONAL}
    };

    std::set<std::string> validType = {"E164", "RFC3966", "INTERNATIONAL", "NATIONAL"};
    if (validType.find(type) != validType.end()) {
        phoneNumberFormat = type2PhoneNumberFormat[type];
    } else {
        phoneNumberFormat = PhoneNumberUtil::PhoneNumberFormat::NATIONAL;
    }
}

PhoneNumberFormat::~PhoneNumberFormat()
{
    if (g_dynamicHandler != NULL) {
        dlclose(g_dynamicHandler);
        g_dynamicHandler = nullptr;
        g_func = nullptr;
    }
}

std::unique_ptr<PhoneNumberFormat> PhoneNumberFormat::CreateInstance(const std::string &countryTag,
    const std::map<std::string, std::string> &options)
{
    std::unique_ptr<PhoneNumberFormat> phoneNumberFormat = std::make_unique<PhoneNumberFormat>(countryTag, options);
    if (phoneNumberFormat->GetPhoneNumberUtil() == nullptr) {
        return nullptr;
    }
    return phoneNumberFormat;
}

PhoneNumberUtil* PhoneNumberFormat::GetPhoneNumberUtil()
{
    return util;
}

bool PhoneNumberFormat::isValidPhoneNumber(const std::string &number) const
{
    i18n::phonenumbers::PhoneNumber phoneNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, country, &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        return false;
    }
    return util->IsValidNumber(phoneNumber);
}

std::string PhoneNumberFormat::format(const std::string &number) const
{
    i18n::phonenumbers::PhoneNumber phoneNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, country, &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        return "";
    }
    std::string formatted_number;
    util->Format(phoneNumber, phoneNumberFormat, &formatted_number);
    return formatted_number;
}

std::string PhoneNumberFormat::getLocationName(const std::string &number, const std::string &locale) const
{
    const char* error = NULL;
    if (g_dynamicHandler == NULL) {
        const char* geocodingSO = "libgeocoding.z.so";
        g_dynamicHandler = dlopen(geocodingSO, RTLD_NOW);
        dlerror();
    }
    if (!g_func) {
        g_func = reinterpret_cast<ExposeLocationName>(dlsym(g_dynamicHandler, "exposeLocationName"));
    }
    error = dlerror();
    if (error != NULL) {
        return "";
    }
    const char* numberStr = number.c_str();
    const char* localeStr = locale.c_str();
    char recvArr[RECV_CHAR_LEN];
    g_func(numberStr, localeStr, recvArr);
    std::string locName = recvArr;
    return locName;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
