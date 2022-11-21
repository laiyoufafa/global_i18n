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

#include "locale_config_test.h"
#include <gtest/gtest.h>
#include "locale_config.h"
#include "parameter.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class LocaleConfigTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void LocaleConfigTest::SetUpTestCase(void)
{}

void LocaleConfigTest::TearDownTestCase(void)
{}

void LocaleConfigTest::SetUp(void)
{}

void LocaleConfigTest::TearDown(void)
{}

/**
 * @tc.name: LocaleConfigFuncTest001
 * @tc.desc: Test LocaleConfig GetSystemLanguage default
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest001, TestSize.Level1)
{
    int ret = SetParameter("hm.sys.language", "");
    if (!ret) {
        EXPECT_EQ(LocaleConfig::GetSystemLanguage(), "zh-Hans");
    }
}

/**
 * @tc.name: LocaleConfigFuncTest002
 * @tc.desc: Test LocaleConfig GetSystemRegion default.
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest002, TestSize.Level1)
{
    int ret = SetParameter("hm.sys.locale", "");
    if (!ret) {
        EXPECT_EQ(LocaleConfig::GetSystemRegion(), "CN");
    }
}

/**
 * @tc.name: LocaleConfigFuncTest003
 * @tc.desc: Test LocaleConfig GetSystemLocale default
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest003, TestSize.Level1)
{
    int ret = SetParameter("hm.sys.locale", "");
    if (!ret) {
        EXPECT_EQ(LocaleConfig::GetSystemLocale(), "zh-Hans-CN");
    }
}

/**
 * @tc.name: LocaleConfigFuncTest004
 * @tc.desc: Test LocaleConfig SetSystemLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest004, TestSize.Level1)
{
    string language = "pt-PT";
    bool ret = LocaleConfig::SetSystemLanguage(language);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: LocaleConfigFuncTest005
 * @tc.desc: Test LocaleConfig SetSystemLocale
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest005, TestSize.Level1)
{
    string locale = "zh-Hant-TW";
    bool ret = LocaleConfig::SetSystemLocale(locale);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: LocaleConfigFuncTest006
 * @tc.desc: Test LocaleConfig SetSystemRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest006, TestSize.Level1)
{
    string locale = "zh-Hant-TW";
    bool ret = LocaleConfig::SetSystemLocale(locale);
    ret = LocaleConfig::SetSystemRegion("HK");
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: LocaleConfigFuncTest007
 * @tc.desc: Test LocaleConfig GetSystemCountries
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest007, TestSize.Level1)
{
    vector<string> countries;
    LocaleConfig::GetSystemCountries(countries);
    unsigned int size = 240;
    EXPECT_EQ(countries.size(), size);
}

/**
 * @tc.name: LocaleConfigFuncTest008
 * @tc.desc: Test LocaleConfig GetDisplayLanguage
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest008, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayLanguage("pt", "zh-Hans-CN", true), "葡萄牙语");
}

/**
 * @tc.name: LocaleConfigFuncTest009
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest009, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("JP", "zh-Hans-CN", false), "日本");
}

/**
 * @tc.name: LocaleConfigFuncTest010
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest010, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("zh-Hans-CN", "en-US", true), "China");
}

/**
 * @tc.name: LocaleConfigFuncTest011
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest011, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("zh", "en-US", true), "");
}

/**
 * @tc.name: LocaleConfigFuncTest012
 * @tc.desc: Test LocaleConfig GetDisplayRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest012, TestSize.Level1)
{
    EXPECT_EQ(LocaleConfig::GetDisplayRegion("zh-Hans", "en-US", true), "");
}

/**
 * @tc.name: LocaleConfigFuncTest013
 * @tc.desc: Test LocaleConfig LocalDigit
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest013, TestSize.Level1)
{
    bool current = LocaleConfig::GetUsingLocalDigit();
    if (LocaleConfig::SetUsingLocalDigit(true)) {
        current = LocaleConfig::GetUsingLocalDigit();
        EXPECT_TRUE(current);
    }
}

/**
 * @tc.name: LocaleConfigFuncTest014
 * @tc.desc: Test LocaleConfig 24 hour clock
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest014, TestSize.Level1)
{
    bool current = LocaleConfig::Is24HourClock();
    if (LocaleConfig::Set24HourClock(true)) {
        current = LocaleConfig::Is24HourClock();
        EXPECT_TRUE(current);
    }
}

/**
 * @tc.name: LocaleConfigFuncTest015
 * @tc.desc: Test LocaleConfig valid locales
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest015, TestSize.Level1)
{
    string localeTag = "zh";
    string validLocaleTag = LocaleConfig::GetValidLocale(localeTag);
    EXPECT_EQ(validLocaleTag, "zh");

    localeTag = "zh-u-hc-h12-nu-latn-ca-chinese-co-pinyin-kf-upper-kn-true";
    validLocaleTag = LocaleConfig::GetValidLocale(localeTag);
    EXPECT_EQ(validLocaleTag, "zh-u-ca-chinese-co-pinyin-kn-true-kf-upper-nu-latn-hc-h12");
}

/**
 * @tc.name: LocaleConfigFuncTest016
 * @tc.desc: Test LocaleConfig isRTL
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest016, TestSize.Level1)
{
    string localeTag = "zh";
    bool isRTL = LocaleConfig::IsRTL(localeTag);
    EXPECT_TRUE(!isRTL);
    localeTag = "ar";
    isRTL = LocaleConfig::IsRTL(localeTag);
    EXPECT_TRUE(isRTL);
}

/**
 * @tc.name: LocaleConfigFuncTest017
 * @tc.desc: Test LocaleConfig isSuggested
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest017, TestSize.Level1)
{
    string language = "zh";
    bool isSuggested = LocaleConfig::IsSuggested(language);
    EXPECT_TRUE(isSuggested);
    string region = "CN";
    isSuggested = LocaleConfig::IsSuggested(language, region);
    EXPECT_TRUE(isSuggested);
}

/**
 * @tc.name: LocaleConfigFuncTest018
 * @tc.desc: Test LocaleCOnfig systemRegion
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest018, TestSize.Level1)
{
    string currentRegion = LocaleConfig::GetSystemRegion();
    EXPECT_TRUE(currentRegion.length() > 0);
    string region = "CN";
    if (LocaleConfig::SetSystemRegion(region)) {
        currentRegion = LocaleConfig::GetSystemRegion();
        EXPECT_EQ(currentRegion, region);
    }
}

/**
 * @tc.name: LocaleConfigFuncTest019
 * @tc.desc: Test LocaleCOnfig systemLocale
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest019, TestSize.Level1)
{
    string currentLocale = LocaleConfig::GetSystemLocale();
    EXPECT_TRUE(currentLocale.length() > 0);
    string locale = "zh-Hans-CN";
    if (LocaleConfig::SetSystemLocale(locale)) {
        currentLocale = LocaleConfig::GetSystemLocale();
        EXPECT_EQ(currentLocale, locale);
    }
}

/**
 * @tc.name: LocaleConfigFuncTest020
 * @tc.desc: Test LocaleCOnfig systemLanguages
 * @tc.type: FUNC
 */
HWTEST_F(LocaleConfigTest, LocaleConfigFuncTest020, TestSize.Level1)
{
    vector<string> languages;
    LocaleConfig::GetSystemLanguages(languages);
    EXPECT_EQ(languages.size(), 2);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS