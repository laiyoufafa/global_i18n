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

#ifndef OHOS_GLOBAL_I18N_TABOO_H
#define OHOS_GLOBAL_I18N_TABOO_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace OHOS {
namespace Global {
namespace I18n {
enum DataFileType {
    CONFIG_FILE,
    DATA_FILE
};

class Taboo {
public:
    Taboo();
    Taboo(const std::string& path);
    ~Taboo();
    std::string ReplaceCountryName(const std::string& region, const std::string& displayLanguage,
        const std::string& name);
    std::string ReplaceLanguageName(const std::string& language, const std::string& displayLanguage,
        const std::string& name);

private:
    void ParseTabooData(const std::string& path, DataFileType fileType, const std::string& Locale = "");
    void ProcessTabooConfigData(const std::string& name, const std::string& value);
    void ProcessTabooLocaleData(const std::string& locale, const std::string& name, const std::string& value);
    void SplitValue(const std::string& value, std::set<std::string>& collation);
    std::vector<std::string> QueryKeyFallBack(const std::string& key);
    std::tuple<std::string, std::string> LanguageFallBack(const std::string& language);
    void ReadResourceList();
    std::string GetLanguageFromFileName(const std::string& fileName);

    std::set<std::string> supportedRegions; // Indicates which regions support name replacement using taboo data.
    std::set<std::string> supportedLanguages; // Indicates which languages support name replacement using taboo data.
    // cache the name replacement taboo data of different locale.
    std::map<std::string, std::map<std::string, std::string>> localeTabooData;
    std::map<std::string, std::string> resources; // Indicates which locales are supported to find taboo data.

    std::string tabooDataPath = "";
    std::string tabooConfigFileName = "taboo-config.xml";
    std::string tabooLocaleDataFileName = "taboo-data.xml";
    std::string filePathSplitor = "/";
    std::string supportedRegionsTag = "regions"; // supported regions key in taboo-config.xml
    std::string supportedLanguagesTag = "languages"; // supported languages key in taboo-config.xml
    std::string regionKey = "region_"; // start part of region name replacement data key.
    std::string languageKey = "language_"; // start part of language name replacement data key.
    bool isTabooDataExist = false;

    static const char* ROOT_TAG;
    static const char* ITEM_TAG;
    static const char* NAME_TAG;
    static const char* VALUE_TAG;
    char tabooDataSplitor = ',';
};
} // namespace I18n
} // namespace Global
} // namespace OHOS
#endif