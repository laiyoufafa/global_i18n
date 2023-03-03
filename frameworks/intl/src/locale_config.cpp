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
#include <regex>
#include "accesstoken_kit.h"
#ifdef SUPPORT_GRAPHICS
#include "app_mgr_client.h"
#include "ability_manager_client.h"
#include "configuration.h"
#endif
#include <cctype>
#include "hilog/log.h"
#include "ipc_skeleton.h"
#include "libxml/parser.h"
#include "locale_info.h"
#include "localebuilder.h"
#include "locdspnm.h"
#include "locid.h"
#include "ohos/init_data.h"
#include "parameter.h"
#include "securec.h"
#include "string_ex.h"
#include "ucase.h"
#include "ulocimp.h"
#include "unistr.h"
#include "ureslocs.h"
#include "ustring.h"
#include "ustr_imp.h"
#include "utils.h"
#include "locale_config.h"

namespace OHOS {
namespace Global {
namespace I18n {
using namespace std;
using namespace OHOS::HiviewDFX;

static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "LocaleConfig" };
const char *LocaleConfig::LANGUAGE_KEY = "persist.global.language";
const char *LocaleConfig::LOCALE_KEY = "persist.global.locale";
const char *LocaleConfig::HOUR_KEY = "persist.global.is24Hour";
const char *LocaleConfig::DEFAULT_LOCALE_KEY = "const.global.locale";
const char *LocaleConfig::DEFAULT_LANGUAGE_KEY = "const.global.language";
const char *LocaleConfig::DEFAULT_REGION_KEY = "const.global.region";
const char *LocaleConfig::SIM_COUNTRY_CODE_KEY = "telephony.sim.countryCode0";
const char *LocaleConfig::SUPPORTED_LOCALES_NAME = "supported_locales";
const char *LocaleConfig::SUPPORTED_REGIONS_NAME = "supported_regions";
const char *LocaleConfig::WHITE_LANGUAGES_NAME = "white_languages";
const char *LocaleConfig::FORBIDDEN_LANGUAGES_NAME = "forbidden_languages";
const char *LocaleConfig::FORBIDDEN_REGIONS_NAME = "forbidden_regions";
const char *LocaleConfig::FORBIDDEN_LANGUAGES_PATH = "/system/usr/ohos_locale_config/forbidden_languages.xml";
const char *LocaleConfig::FORBIDDEN_REGIONS_PATH = "/system/usr/ohos_locale_config/forbidden_regions.xml";
const char *LocaleConfig::SUPPORTED_LOCALES_PATH = "/system/usr/ohos_locale_config/supported_locales.xml";
const char *LocaleConfig::SUPPORTED_REGIONS_PATH = "/system/usr/ohos_locale_config/supported_regions.xml";
const char *LocaleConfig::WHITE_LANGUAGES_PATH = "/system/usr/ohos_locale_config/white_languages.xml";
const char *LocaleConfig::SUPPORT_LOCALES_PATH = "/etc/ohos_lang_config/supported_locales.xml";
const char *LocaleConfig::DEFAULT_LOCALE = "en-Latn";
const char *LocaleConfig::supportLocalesTag = "supported_locales";
const char *LocaleConfig::LANG_PATH = "/etc/ohos_lang_config/";
const char *LocaleConfig::rootTag = "languages";
const char *LocaleConfig::secondRootTag = "lang";
unordered_set<string> LocaleConfig::supportedLocales;
unordered_set<string> LocaleConfig::supportedRegions;
unordered_set<string> LocaleConfig::whiteLanguages;
unordered_map<string, string> LocaleConfig::dialectMap {
    { "es-Latn-419", "es-Latn-419" },
    { "es-Latn-BO", "es-Latn-419" },
    { "es-Latn-BR", "es-Latn-419" },
    { "es-Latn-BZ", "es-Latn-419" },
    { "es-Latn-CL", "es-Latn-419" },
    { "es-Latn-CO", "es-Latn-419" },
    { "es-Latn-CR", "es-Latn-419" },
    { "es-Latn-CU", "es-Latn-419" },
    { "es-Latn-DO", "es-Latn-419" },
    { "es-Latn-EC", "es-Latn-419" },
    { "es-Latn-GT", "es-Latn-419" },
    { "es-Latn-HN", "es-Latn-419" },
    { "es-Latn-MX", "es-Latn-419" },
    { "es-Latn-NI", "es-Latn-419" },
    { "es-Latn-PA", "es-Latn-419" },
    { "es-Latn-PE", "es-Latn-419" },
    { "es-Latn-PR", "es-Latn-419" },
    { "es-Latn-PY", "es-Latn-419" },
    { "es-Latn-SV", "es-Latn-419" },
    { "es-Latn-US", "es-Latn-419" },
    { "es-Latn-UY", "es-Latn-419" },
    { "es-Latn-VE", "es-Latn-419" },
    { "pt-Latn-PT", "pt-Latn-PT" },
    { "en-Latn-US", "en-Latn-US" }
};

unordered_map<string, string> LocaleConfig::localDigitMap {
    { "ar", "arab" },
    { "as", "beng" },
    { "bn", "beng" },
    { "fa", "arabext" },
    { "mr", "deva" },
    { "my", "mymr" },
    { "ne", "deva" },
    { "ur", "latn" }
};

std::map<std::string, std::string> LocaleConfig::supportedDialectLocales;
std::map<string, string> LocaleConfig::locale2DisplayName {};
std::string LocaleConfig::currentDialectLocale = "";

set<std::string> LocaleConfig::validCaTag {
    "buddhist",
    "chinese",
    "coptic",
    "dangi",
    "ethioaa",
    "ethiopic",
    "gregory",
    "hebrew",
    "indian",
    "islamic",
    "islamic-umalqura",
    "islamic-tbla",
    "islamic-civil",
    "islamic-rgsa",
    "iso8601",
    "japanese",
    "persian",
    "roc",
    "islamicc",
};
set<std::string> LocaleConfig::validCoTag {
    "big5han",
    "compat",
    "dict",
    "direct",
    "ducet",
    "eor",
    "gb2312",
    "phonebk",
    "phonetic",
    "pinyin",
    "reformed",
    "searchjl",
    "stroke",
    "trad",
    "unihan",
    "zhuyin",
};
set<std::string> LocaleConfig::validKnTag {
    "true",
    "false",
};
set<std::string> LocaleConfig::validKfTag {
    "upper",
    "lower",
    "false",
};
set<std::string> LocaleConfig::validNuTag {
    "adlm", "ahom", "arab", "arabext", "bali", "beng",
    "bhks", "brah", "cakm", "cham", "deva", "diak",
    "fullwide", "gong", "gonm", "gujr", "guru", "hanidec",
    "hmng", "hmnp", "java", "kali", "khmr", "knda",
    "lana", "lanatham", "laoo", "latn", "lepc", "limb",
    "mathbold", "mathdbl", "mathmono", "mathsanb", "mathsans", "mlym",
    "modi", "mong", "mroo", "mtei", "mymr", "mymrshan",
    "mymrtlng", "newa", "nkoo", "olck", "orya", "osma",
    "rohg", "saur", "segment", "shrd", "sind", "sinh",
    "sora", "sund", "takr", "talu", "tamldec", "telu",
    "thai", "tibt", "tirh", "vaii", "wara", "wcho",
};
set<std::string> LocaleConfig::validHcTag {
    "h12",
    "h23",
    "h11",
    "h24",
};
set<std::string> LocaleConfig::dialectLang {
    "zh",
    "ro",
    "fa",
};

static unordered_map<string, string> g_languageMap = {
    { "zh-Hans", "zh-Hans" },
    { "zh-Hant", "zh-Hant" },
    { "my-Qaag", "my-Qaag" },
    { "es-Latn-419", "es-419" },
    { "es-Latn-US", "es-419" },
    { "az-Latn", "az-Latn" },
    { "bs-Latn", "bs-Latn" },
    { "en-Qaag", "en-Qaag" },
    { "uz-Latn", "uz-Latn" },
    { "sr-Latn", "sr-Latn" },
    { "jv-Latn", "jv-Latn" },
    { "pt-Latn-BR", "pt-BR" },
    { "pa-Guru", "pa-Guru" },
    { "mai-Deva", "mai-Deva" }
};

string Adjust(const string &origin)
{
    for (auto iter = g_languageMap.begin(); iter != g_languageMap.end(); ++iter) {
        string key = iter->first;
        if (!origin.compare(0, key.length(), key)) {
            return iter->second;
        }
    }
    return origin;
}

int32_t GetDialectName(const char *localeName, char *name, size_t nameCapacity, UErrorCode &status)
{
    icu::Locale locale = icu::Locale::forLanguageTag(localeName, status);
    if (status != U_ZERO_ERROR) {
        return 0;
    }
    const char *lang = locale.getLanguage();
    const char *script = locale.getScript();
    const char *country = locale.getCountry();
    bool hasScript = (script != nullptr) && strlen(script) > 0;
    bool hasCountry = (country != nullptr) && strlen(country) > 0;
    string temp = lang;
    if (hasScript && hasCountry) {
        temp.append("_");
        temp.append(script);
        temp.append("_");
        temp.append(country);
    } else if (hasScript) {
        temp.append("_");
        temp.append(script);
    } else if (hasCountry) {
        temp.append("_");
        temp.append(country);
    }
    if (strcpy_s(name, nameCapacity, temp.data()) != EOK) {
        return 0;
    }
    return temp.size();
}

string GetDisplayLanguageInner(const string &language, const string &displayLocaleTag, bool sentenceCase)
{
    icu::UnicodeString unistr;
    // 0 is the start position of language, 2 is the length of zh and fa
    if (!language.compare(0, 2, "zh") || !language.compare(0, 2, "fa") || !language.compare(0, 2, "ro")) {
        UErrorCode status = U_ZERO_ERROR;
        icu::Locale displayLocale = icu::Locale::forLanguageTag(displayLocaleTag.c_str(), status);
        if (status != U_ZERO_ERROR) {
            return "";
        }
        icu::LocaleDisplayNames *dspNames = icu::LocaleDisplayNames::createInstance(displayLocale,
            UDialectHandling::ULDN_DIALECT_NAMES);
        icu::Locale tempLocale = icu::Locale::forLanguageTag(language.c_str(), status);
        if (status != U_ZERO_ERROR) {
            return "";
        }
        dspNames->localeDisplayName(tempLocale, unistr);
        delete dspNames;
    } else {
        UErrorCode status = U_ZERO_ERROR;
        icu::Locale displayLoc = icu::Locale::forLanguageTag(displayLocaleTag, status);
        if (status != U_ZERO_ERROR) {
            return "";
        }
        icu::Locale locale = icu::Locale::forLanguageTag(language, status);
        if (status != U_ZERO_ERROR) {
            return "";
        }
        locale.getDisplayName(displayLoc, unistr);
    }
    if (sentenceCase) {
        UChar32 ch = ucase_toupper(unistr.char32At(0));
        unistr.replace(0, 1, ch);
    }
    string out;
    unistr.toUTF8String(out);
    return out;
}

bool LocaleConfig::listsInitialized = LocaleConfig::InitializeLists();

string LocaleConfig::GetSystemLanguage()
{
    std::string systemLanguage = ReadSystemParameter(LANGUAGE_KEY, CONFIG_LEN);
    if (systemLanguage.empty()) {
        systemLanguage = ReadSystemParameter(DEFAULT_LANGUAGE_KEY, CONFIG_LEN);
    }
    return systemLanguage;
}

string LocaleConfig::GetSystemRegion()
{
    UErrorCode status = U_ZERO_ERROR;
    const char *country = nullptr;
    std::string systemRegion = ReadSystemParameter(LOCALE_KEY, CONFIG_LEN);
    if (!systemRegion.empty()) {
        icu::Locale origin = icu::Locale::forLanguageTag(systemRegion, status);
        if (U_SUCCESS(status)) {
            country = origin.getCountry();
            if (country != nullptr) {
                return country;
            }
        }
    }
    systemRegion = ReadSystemParameter(DEFAULT_LOCALE_KEY, CONFIG_LEN);
    if (!systemRegion.empty()) {
        status = U_ZERO_ERROR;
        icu::Locale origin = icu::Locale::forLanguageTag(systemRegion, status);
        if (U_SUCCESS(status)) {
            country = origin.getCountry();
            if (country != nullptr) {
                return country;
            }
        }
    }
    return "";
}

string LocaleConfig::GetSystemLocale()
{
    std::string systemLocale = ReadSystemParameter(LOCALE_KEY, CONFIG_LEN);
    if (systemLocale.empty()) {
        systemLocale = ReadSystemParameter(DEFAULT_LOCALE_KEY, CONFIG_LEN);
    }
    return systemLocale;
}

bool LocaleConfig::CheckPermission()
{
    Security::AccessToken::AccessTokenID callerToken = IPCSkeleton::GetCallingTokenID();
    int result = Security::AccessToken::PermissionState::PERMISSION_GRANTED;
    if (Security::AccessToken::AccessTokenKit::GetTokenTypeFlag(callerToken)
        == Security::AccessToken::ATokenTypeEnum::TOKEN_HAP) {
        result = Security::AccessToken::AccessTokenKit::VerifyAccessToken(callerToken,
                                                                          "ohos.permission.UPDATE_CONFIGURATION");
    } else {
        HiLog::Info(LABEL, "Invlid tokenID");
        return false;
    }
    if (result != Security::AccessToken::PermissionState::PERMISSION_GRANTED) {
        HiLog::Info(LABEL, "Verify permission failed");
        return false;
    }
    return true;
}

bool LocaleConfig::SetSystemLanguage(const string &language)
{
    if (!CheckPermission()) {
        return false;
    }
    if (!IsValidTag(language)) {
        return false;
    }
    if (SetParameter(LANGUAGE_KEY, language.data()) == 0) {
#ifdef SUPPORT_GRAPHICS
        auto appMgrClient = std::make_unique<AppExecFwk::AppMgrClient>();
        AppExecFwk::Configuration configuration;
        configuration.AddItem(AAFwk::GlobalConfigurationKey::SYSTEM_LANGUAGE, language);
        appMgrClient->UpdateConfiguration(configuration);
#endif
        return true;
    }
    return false;
}

bool LocaleConfig::SetSystemRegion(const string &region)
{
    if (!CheckPermission()) {
        return false;
    }
    if (!IsValidRegion(region)) {
        return false;
    }
    char value[CONFIG_LEN];
    int code = GetParameter(LOCALE_KEY, "", value, CONFIG_LEN);
    string newLocale;
    if (code > 0) {
        string tag(value, code);
        newLocale = GetRegionChangeLocale(tag, region);
        if (newLocale == "") {
            return false;
        }
    } else {
        icu::Locale temp("", region.c_str());
        UErrorCode status = U_ZERO_ERROR;
        temp.addLikelySubtags(status);
        if (status != U_ZERO_ERROR) {
            return false;
        }
        newLocale = temp.toLanguageTag<string>(status);
        if (status != U_ZERO_ERROR) {
            return false;
        }
    }
    return SetParameter(LOCALE_KEY, newLocale.data()) == 0;
}

bool LocaleConfig::SetSystemLocale(const string &locale)
{
    if (!CheckPermission()) {
        return false;
    }
    if (!IsValidTag(locale)) {
        return false;
    }
    return SetParameter(LOCALE_KEY, locale.data()) == 0;
}

bool LocaleConfig::IsValidLanguage(const string &language)
{
    string::size_type size = language.size();
    if ((size != LANGUAGE_LEN) && (size != LANGUAGE_LEN + 1)) {
        return false;
    }
    for (size_t i = 0; i < size; ++i) {
        if ((language[i] > 'z') || (language[i] < 'a')) {
            return false;
        }
    }
    return true;
}

bool LocaleConfig::IsValidScript(const string &script)
{
    string::size_type size = script.size();
    if (size != LocaleInfo::SCRIPT_LEN) {
        return false;
    }
    char first = script[0];
    if ((first < 'A') || (first > 'Z')) {
        return false;
    }
    for (string::size_type i = 1; i < LocaleInfo::SCRIPT_LEN; ++i) {
        if ((script[i] > 'z') || (script[i] < 'a')) {
            return false;
        }
    }
    return true;
}

bool LocaleConfig::IsValidRegion(const string &region)
{
    string::size_type size = region.size();
    if (size != LocaleInfo::REGION_LEN) {
        return false;
    }
    for (size_t i = 0; i < LocaleInfo::REGION_LEN; ++i) {
        if ((region[i] > 'Z') || (region[i] < 'A')) {
            return false;
        }
    }
    return true;
}

bool LocaleConfig::IsValidTag(const string &tag)
{
    if (!tag.size()) {
        return false;
    }
    vector<string> splits;
    Split(tag, "-", splits);
    if (!IsValidLanguage(splits[0])) {
        return false;
    }
    return true;
}

void LocaleConfig::Split(const string &src, const string &sep, vector<string> &dest)
{
    string::size_type begin = 0;
    string::size_type end = src.find(sep);
    while (end != string::npos) {
        dest.push_back(src.substr(begin, end - begin));
        begin = end + sep.size();
        end = src.find(sep, begin);
    }
    if (begin != src.size()) {
        dest.push_back(src.substr(begin));
    }
}

// language in white languages should have script.
void LocaleConfig::GetSystemLanguages(vector<string> &ret)
{
    std::copy(whiteLanguages.begin(), whiteLanguages.end(), std::back_inserter(ret));
}

const unordered_set<string>& LocaleConfig::GetSupportedLocales()
{
    return supportedLocales;
}

const unordered_set<string>& LocaleConfig::GetSupportedRegions()
{
    return supportedRegions;
}

void LocaleConfig::GetSystemCountries(vector<string> &ret)
{
    std::copy(supportedRegions.begin(), supportedRegions.end(), std::back_inserter(ret));
}

bool LocaleConfig::IsSuggested(const string &language)
{
    unordered_set<string> relatedLocales;
    vector<string> simCountries;
    GetCountriesFromSim(simCountries);
    GetRelatedLocales(relatedLocales, simCountries);
    for (auto iter = relatedLocales.begin(); iter != relatedLocales.end();) {
        if (whiteLanguages.find(*iter) == whiteLanguages.end()) {
            iter = relatedLocales.erase(iter);
        } else {
            ++iter;
        }
    }
    string mainLanguage = GetMainLanguage(language);
    return relatedLocales.find(mainLanguage) != relatedLocales.end();
}

bool LocaleConfig::IsSuggested(const std::string &language, const std::string &region)
{
    unordered_set<string> relatedLocales;
    vector<string> countries { region };
    GetRelatedLocales(relatedLocales, countries);
    for (auto iter = relatedLocales.begin(); iter != relatedLocales.end();) {
        if (whiteLanguages.find(*iter) == whiteLanguages.end()) {
            iter = relatedLocales.erase(iter);
        } else {
            ++iter;
        }
    }
    string mainLanguage = GetMainLanguage(language);
    return relatedLocales.find(mainLanguage) != relatedLocales.end();
}

void LocaleConfig::GetRelatedLocales(unordered_set<string> &relatedLocales, vector<string> countries)
{
    // remove unsupported countries
    const unordered_set<string> &regions = GetSupportedRegions();
    for (auto iter = countries.begin(); iter != countries.end();) {
        if (regions.find(*iter) == regions.end()) {
            iter = countries.erase(iter);
        } else {
            ++iter;
        }
    }
    const unordered_set<string> &locales = GetSupportedLocales();
    for (string locale : locales) {
        bool find = false;
        for (string country : countries) {
            if (locale.find(country) != string::npos) {
                find = true;
                break;
            }
        }
        if (!find) {
            continue;
        }
        string mainLanguage = GetMainLanguage(locale);
        if (mainLanguage != "") {
            relatedLocales.insert(mainLanguage);
        }
    }
}

void LocaleConfig::GetCountriesFromSim(vector<string> &simCountries)
{
    simCountries.push_back(GetSystemRegion());
    char value[CONFIG_LEN];
    int code = GetParameter(SIM_COUNTRY_CODE_KEY, "", value, CONFIG_LEN);
    if (code > 0) {
        simCountries.push_back(value);
    }
}

void LocaleConfig::GetListFromFile(const char *path, const char *resourceName, unordered_set<string> &ret)
{
    xmlKeepBlanksDefault(0);
    if (!path) {
        return;
    }
    xmlDocPtr doc = xmlParseFile(path);
    if (!doc) {
        return;
    }
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(resourceName))) {
        xmlFreeDoc(doc);
        return;
    }
    cur = cur->xmlChildrenNode;
    xmlChar *content = nullptr;
    while (cur != nullptr) {
        content = xmlNodeGetContent(cur);
        if (content != nullptr) {
            ret.insert(reinterpret_cast<const char*>(content));
            xmlFree(content);
            cur = cur->next;
        } else {
            break;
        }
    }
    xmlFreeDoc(doc);
}

void LocaleConfig::Expunge(unordered_set<string> &src, const unordered_set<string> &another)
{
    for (auto iter = src.begin(); iter != src.end();) {
        if (another.find(*iter) != another.end()) {
            iter = src.erase(iter);
        } else {
            ++iter;
        }
    }
}

bool LocaleConfig::InitializeLists()
{
    SetHwIcuDirectory();
    GetListFromFile(SUPPORTED_REGIONS_PATH, SUPPORTED_REGIONS_NAME, supportedRegions);
    unordered_set<string> forbiddenRegions;
    GetListFromFile(FORBIDDEN_REGIONS_PATH, FORBIDDEN_REGIONS_NAME, forbiddenRegions);
    Expunge(supportedRegions, forbiddenRegions);
    GetListFromFile(WHITE_LANGUAGES_PATH, WHITE_LANGUAGES_NAME, whiteLanguages);
    unordered_set<string> forbiddenLanguages;
    GetListFromFile(FORBIDDEN_LANGUAGES_PATH, FORBIDDEN_LANGUAGES_NAME, forbiddenLanguages);
    Expunge(whiteLanguages, forbiddenLanguages);
    GetListFromFile(SUPPORTED_LOCALES_PATH, SUPPORTED_LOCALES_NAME, supportedLocales);
    return true;
}

string LocaleConfig::GetRegionChangeLocale(const string &languageTag, const string &region)
{
    UErrorCode status = U_ZERO_ERROR;
    const icu::Locale origin = icu::Locale::forLanguageTag(languageTag, status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    icu::LocaleBuilder builder = icu::LocaleBuilder().setLanguage(origin.getLanguage()).
        setScript(origin.getScript()).setRegion(region);
    icu::Locale temp = builder.setExtension('u', "").build(status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    string ret = temp.toLanguageTag<string>(status);
    return (status != U_ZERO_ERROR) ? "" : ret;
}

string LocaleConfig::GetMainLanguage(const string &language)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale origin = icu::Locale::forLanguageTag(language, status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    origin.addLikelySubtags(status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    icu::LocaleBuilder builder = icu::LocaleBuilder().setLanguage(origin.getLanguage()).
        setScript(origin.getScript()).setRegion(origin.getCountry());
    icu::Locale temp = builder.setExtension('u', "").build(status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    string fullLanguage = temp.toLanguageTag<string>(status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    if (dialectMap.find(fullLanguage) != dialectMap.end()) {
        return dialectMap[fullLanguage];
    }
    builder.setRegion("");
    temp = builder.build(status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    fullLanguage = temp.toLanguageTag<string>(status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    return fullLanguage;
}

string LocaleConfig::GetDisplayLanguage(const string &language, const string &displayLocale, bool sentenceCase)
{
    string adjust = Adjust(language);
    if (adjust == language) {
        UErrorCode status = U_ZERO_ERROR;
        icu::Locale displayLoc = icu::Locale::forLanguageTag(displayLocale, status);
        if (status != U_ZERO_ERROR) {
            return "";
        }
        icu::Locale locale = icu::Locale::forLanguageTag(language, status);
        if (status != U_ZERO_ERROR) {
            return "";
        }
        icu::UnicodeString unistr;
        std::string lang(locale.getLanguage());
        std::string result;
        if (dialectLang.find(lang) != dialectLang.end()) {
            result = GetDsiplayLanguageWithDialect(language, displayLocale);
        } else {
            locale.getDisplayLanguage(displayLoc, unistr);
            unistr.toUTF8String(result);
        }
        if (sentenceCase) {
            char ch = static_cast<char>(toupper(result[0]));
            return result.replace(0, 1, 1, ch);
        }
        return result;
    }
    return GetDisplayLanguageInner(adjust, displayLocale, sentenceCase);
}

std::string LocaleConfig::ComputeLocale(const std::string &displayLocale)
{
    if (supportedDialectLocales.size() == 0) {
        xmlKeepBlanksDefault(0);
        xmlDocPtr doc = xmlParseFile(SUPPORT_LOCALES_PATH);
        if (!doc) {
            return DEFAULT_LOCALE;
        }
        xmlNodePtr cur = xmlDocGetRootElement(doc);
        if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(supportLocalesTag))) {
            xmlFreeDoc(doc);
            HiLog::Info(LABEL, "can not parse language supported locale file");
            return DEFAULT_LOCALE;
        }
        cur = cur->xmlChildrenNode;
        while (cur != nullptr) {
            xmlChar *content = xmlNodeGetContent(cur);
            if (content == nullptr) {
                HiLog::Info(LABEL, "get xml node content failed");
                break;
            }
            std::map<std::string, std::string> localeInfoConfigs = {};
            LocaleInfo localeinfo(reinterpret_cast<const char*>(content), localeInfoConfigs);
            std::string language = localeinfo.GetLanguage();
            std::string script = localeinfo.GetScript();
            std::string languageAndScript = (script.length() == 0) ? language : language + "-" + script;
            LocaleInfo newLocaleInfo(languageAndScript, localeInfoConfigs);
            std::string maximizeLocale = newLocaleInfo.Maximize();
            supportedDialectLocales.insert(
                std::make_pair<std::string, std::string>(maximizeLocale.c_str(),
                                                         reinterpret_cast<const char*>(content)));
            xmlFree(content);
            cur = cur->next;
        }
    }
    std::map<std::string, std::string> configs = {};
    LocaleInfo localeinfo(displayLocale, configs);
    std::string language = localeinfo.GetLanguage();
    std::string script = localeinfo.GetScript();
    std::string languageAndScript = (script.length() == 0) ? language : language + "-" + script;
    LocaleInfo newLocaleInfo(languageAndScript, configs);
    std::string maximizeLocale = newLocaleInfo.Maximize();
    if (supportedDialectLocales.find(maximizeLocale) != supportedDialectLocales.end()) {
        return supportedDialectLocales.at(maximizeLocale);
    }
    return DEFAULT_LOCALE;
}

void LocaleConfig::ReadLangData(const char *langDataPath)
{
    xmlKeepBlanksDefault(0);
    if (langDataPath == nullptr) {
        return;
    }
    xmlDocPtr doc = xmlParseFile(langDataPath);
    if (!doc) {
        HiLog::Info(LABEL, "can not open language data file");
        return;
    }
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    if (!cur || xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(rootTag))) {
        xmlFreeDoc(doc);
        HiLog::Info(LABEL, "parse language data file failed");
        return;
    }
    cur = cur->xmlChildrenNode;
    while (cur != nullptr && !xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>(secondRootTag))) {
        xmlChar *langContents[ELEMENT_NUM] = { 0 }; // 2 represent langid, displayname;
        xmlNodePtr langValue = cur->xmlChildrenNode;
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (langValue != nullptr) {
                langContents[i] = xmlNodeGetContent(langValue);
                langValue = langValue->next;
            } else {
                break;
            }
        }
        // 0 represents langid index, 1 represents displayname index
        locale2DisplayName.insert(
            std::make_pair<std::string, std::string>(reinterpret_cast<const char *>(langContents[0]),
                                                     reinterpret_cast<const char *>(langContents[1])));
        for (size_t i = 0; i < ELEMENT_NUM; i++) {
            if (langContents[i] != nullptr) {
                xmlFree(langContents[i]);
            }
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
}

string LocaleConfig::GetDsiplayLanguageWithDialect(const std::string &localeStr, const std::string &displayLocale)
{
    std::string finalLocale = ComputeLocale(displayLocale);
    if (finalLocale.compare(currentDialectLocale) != 0) {
        std::string xmlPath = LANG_PATH + finalLocale + ".xml";
        locale2DisplayName.clear();
        ReadLangData(xmlPath.c_str());
        currentDialectLocale = finalLocale;
    }
    if (locale2DisplayName.find(localeStr) != locale2DisplayName.end()) {
        return locale2DisplayName.at(localeStr);
    }
    std::map<std::string, std::string> configs = {};
    LocaleInfo locale(localeStr, configs);
    std::string language = locale.GetLanguage();
    std::string scripts = locale.GetScript();
    std::string region = locale.GetRegion();
    if (scripts.length() != 0) {
        std::string languageAndScripts = language + "-" + scripts;
        if (locale2DisplayName.find(languageAndScripts) != locale2DisplayName.end()) {
            return locale2DisplayName.at(languageAndScripts);
        }
    }
    if (region.length() != 0) {
        std::string languageAndRegion = language + "-" + region;
        if (locale2DisplayName.find(languageAndRegion) != locale2DisplayName.end()) {
            return locale2DisplayName.at(languageAndRegion);
        }
    }
    if (locale2DisplayName.find(language) != locale2DisplayName.end()) {
        return locale2DisplayName.at(language);
    }
    return "";
}

string LocaleConfig::GetDisplayRegion(const string &region, const string &displayLocale, bool sentenceCase)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale originLocale;
    if (IsValidRegion(region)) {
        icu::LocaleBuilder builder = icu::LocaleBuilder().setRegion(region);
        originLocale = builder.build(status);
    } else {
        originLocale = icu::Locale::forLanguageTag(region, status);
    }
    std::string country(originLocale.getCountry());
    if (country.length() == 0) {
        return "";
    }
    if (status != U_ZERO_ERROR) {
        return "";
    }
    icu::Locale locale = icu::Locale::forLanguageTag(displayLocale, status);
    if (status != U_ZERO_ERROR) {
        return "";
    }
    icu::UnicodeString displayRegion;
    originLocale.getDisplayCountry(locale, displayRegion);
    if (sentenceCase) {
        UChar32 ch = ucase_toupper(displayRegion.char32At(0));
        displayRegion.replace(0, 1, ch);
    }
    string temp;
    displayRegion.toUTF8String(temp);
    return temp;
}

bool LocaleConfig::IsRTL(const string &locale)
{
    icu::Locale curLocale(locale.c_str());
    return curLocale.isRightToLeft();
}

void parseExtension(const std::string &extension, std::map<std::string, std::string> &map)
{
    std::string pattern = "-..-";
    std::regex express(pattern);

    std::regex_token_iterator<std::string::const_iterator> begin1(extension.cbegin(), extension.cend(), express);
    std::regex_token_iterator<std::string::const_iterator> begin2(extension.cbegin(), extension.cend(), express, -1);
    begin2++;
    for (; begin1 != std::sregex_token_iterator() && begin2 != std::sregex_token_iterator(); begin1++, begin2++) {
        map.insert(std::pair<std::string, std::string>(begin1->str(), begin2->str()));
    }
}

void setExtension(std::string &extension, const std::string &tag, const std::set<string> &validValue,
    const std::map<std::string, std::string> &extensionMap,
    const std::map<std::string, std::string> &defaultExtensionMap)
{
    std::string value;
    auto it = extensionMap.find(tag);
    if (it != extensionMap.end()) {
        value = it->second;
        if (validValue.find(value) == validValue.end()) {
            return;
        } else {
            extension += tag;
            extension += value;
        }
    } else {
        it = defaultExtensionMap.find(tag);
        if (it != defaultExtensionMap.end()) {
            value = it->second;
            if (validValue.find(value) == validValue.end()) {
                return;
            } else {
                extension += tag;
                extension += value;
            }
        }
    }
}

void setOtherExtension(std::string &extension, std::map<std::string, std::string> &extensionMap,
    std::map<std::string, std::string> &defaultExtensionMap)
{
    std::set<std::string> tags;
    tags.insert("-ca-");
    tags.insert("-co-");
    tags.insert("-kn-");
    tags.insert("-kf-");
    tags.insert("-nu-");
    tags.insert("-hc-");

    for (auto it = tags.begin(); it != tags.end(); it++) {
        extensionMap.erase(*it);
        defaultExtensionMap.erase(*it);
    }

    for (auto it = defaultExtensionMap.begin(); it != defaultExtensionMap.end(); it++) {
        extensionMap.insert(std::pair<std::string, std::string>(it->first, it->second));
    }

    for (auto it = extensionMap.begin(); it != extensionMap.end(); it++) {
        extension += it->first;
        extension += it->second;
    }
}

std::string LocaleConfig::GetValidLocale(const std::string &localeTag)
{
    std::string baseLocale = "";
    std::string extension = "";
    std::size_t found = localeTag.find("-u-");
    baseLocale = localeTag.substr(0, found);
    if (found != std::string::npos) {
        extension = localeTag.substr(found);
    }
    std::map<std::string, std::string> extensionMap;
    if (extension != "") {
        parseExtension(extension, extensionMap);
    }

    std::string systemLocaleTag = GetSystemLocale();
    std::string defaultExtension = "";
    found = systemLocaleTag.find("-u-");
    if (found != std::string::npos) {
        defaultExtension = systemLocaleTag.substr(found);
    }
    std::map<std::string, std::string> defaultExtensionMap;
    if (defaultExtension != "") {
        parseExtension(defaultExtension, defaultExtensionMap);
    }

    std::string ext = "";
    setExtension(ext, "-ca-", validCaTag, extensionMap, defaultExtensionMap);
    setExtension(ext, "-co-", validCoTag, extensionMap, defaultExtensionMap);
    setExtension(ext, "-kn-", validKnTag, extensionMap, defaultExtensionMap);
    setExtension(ext, "-kf-", validKfTag, extensionMap, defaultExtensionMap);
    setExtension(ext, "-nu-", validNuTag, extensionMap, defaultExtensionMap);
    setExtension(ext, "-hc-", validHcTag, extensionMap, defaultExtensionMap);

    std::string otherExt = "";
    setOtherExtension(otherExt, extensionMap, defaultExtensionMap);
    if (ext != "" || otherExt != "") {
        return baseLocale + "-u" + ext + otherExt;
    } else {
        return baseLocale;
    }
}

bool LocaleConfig::Is24HourClock()
{
    std::string is24Hour = ReadSystemParameter(HOUR_KEY, CONFIG_LEN);
    if (is24Hour.empty()) {
        return false;
    }
    if (is24Hour.compare("true") == 0) {
        return true;
    }
    return false;
}

bool LocaleConfig::Set24HourClock(bool option)
{
    if (!CheckPermission()) {
        return false;
    }
    std::string optionStr = "";
    if (option) {
        optionStr = "true";
    } else {
        optionStr = "false";
    }
    return SetParameter(HOUR_KEY, optionStr.data()) == 0;
}

bool LocaleConfig::SetUsingLocalDigit(bool flag)
{
    if (!CheckPermission()) {
        return false;
    }
    std::string locale = GetSystemLocale();
    LocaleInfo localeInfo(locale);
    std::string language = localeInfo.GetLanguage();
    if (localDigitMap.find(language) == localDigitMap.end()) {
        return false;
    }
    std::string numberSystem = "-nu-" + localDigitMap.at(language);
    if (flag) {
        if (locale.find("-u-") == std::string::npos) {
            locale += "-u" + numberSystem;
        } else if (locale.find("-nu-") == std::string::npos) {
            locale += numberSystem;
        } else {
            std::string oldNumberSystem = "-nu-" + localeInfo.GetNumberingSystem();
            locale.replace(locale.find("-nu-"), oldNumberSystem.length(), numberSystem);
        }
    } else {
        size_t pos = locale.find(numberSystem);
        if (pos != std::string::npos) {
            locale.replace(pos, numberSystem.length(), "");
        }
        // 2 is string -u length
        if (locale.find("-u") == (locale.length() - 2)) {
            // 2 is string -u length
            locale.resize(locale.length() - 2);
        }
    }
    if (!SetSystemLocale(locale)) {
        return false;
    }
    return true;
}

bool LocaleConfig::GetUsingLocalDigit()
{
    std::string locale = GetSystemLocale();
    LocaleInfo localeInfo(locale);
    std::string language = localeInfo.GetLanguage();
    if (localDigitMap.find(language) == localDigitMap.end()) {
        return false;
    }
    std::string localNumberSystem = localDigitMap.at(language);
    if (localNumberSystem.compare(localeInfo.GetNumberingSystem()) != 0) {
        return false;
    }
    return true;
}
} // namespace I18n
} // namespace Global
} // namespace OHOS
