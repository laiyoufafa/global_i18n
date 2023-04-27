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

#include <gtest/gtest.h>
#include <map>
#include <vector>

#include "collator.h"
#include "date_time_format.h"
#include "locale_config.h"
#include "locale_info.h"
#include "number_format.h"
#include "plural_rules.h"
#include "relative_time_format.h"
#include "utils.h"
#include "intl_test.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class IntlTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void IntlTest::SetUpTestCase(void)
{}

void IntlTest::TearDownTestCase(void)
{}

void IntlTest::SetUp(void)
{}

void IntlTest::TearDown(void)
{}

/**
 * @tc.name: IntlFuncTest001
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest001, TestSize.Level1)
{
    string locale = "zh-CN-u-hc-h12";
    string expects = "公元1970年1月1日星期四 上午8:20:34";
    vector<string> locales;
    locales.push_back("jessie");
    locales.push_back(locale);
    map<string, string> options = { { "year", "numeric" },
                                    { "month", "long" },
                                    { "day", "numeric" },
                                    { "hour", "numeric" },
                                    { "minute", "2-digit" },
                                    { "second", "numeric" },
                                    { "weekday", "long" },
                                    { "era", "short"} };
    DateTimeFormat *dateFormat = new (std::nothrow) DateTimeFormat(locales, options);
    if (!dateFormat) {
        EXPECT_TRUE(false);
        return;
    }
    int64_t milliseconds = 1234567;
    string out = dateFormat->Format(milliseconds);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(dateFormat->GetYear(), "numeric");
    EXPECT_EQ(dateFormat->GetMonth(), "long");
    EXPECT_EQ(dateFormat->GetDay(), "numeric");
    EXPECT_EQ(dateFormat->GetHour(), "numeric");
    EXPECT_EQ(dateFormat->GetMinute(), "2-digit");
    EXPECT_EQ(dateFormat->GetSecond(), "numeric");
    EXPECT_EQ(dateFormat->GetWeekday(), "long");
    EXPECT_EQ(dateFormat->GetEra(), "short");
    EXPECT_EQ(dateFormat->GetHourCycle(), "h12");
    delete dateFormat;
}

/**
 * @tc.name: IntlFuncTest002
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest002, TestSize.Level0)
{
    string locale = "ja-Jpan-JP-u-ca-japanese-hc-h12-co-emoji";
    map<string, string> options = { { "hourCycle", "h11" },
                                    { "numeric", "true" },
                                    { "numberingSystem", "jpan" } };
    LocaleInfo *loc = new (std::nothrow) LocaleInfo(locale, options);
    if (!loc) {
        EXPECT_TRUE(false);
        return;
    }
    EXPECT_EQ(loc->GetLanguage(), "ja");
    EXPECT_EQ(loc->GetScript(), "Jpan");
    EXPECT_EQ(loc->GetRegion(), "JP");
    EXPECT_EQ(loc->GetBaseName(), "ja-Jpan-JP");
    EXPECT_EQ(loc->GetCalendar(), "japanese");
    EXPECT_EQ(loc->GetHourCycle(), "h11");
    EXPECT_EQ(loc->GetNumberingSystem(), "jpan");
    EXPECT_EQ(loc->Minimize(), "ja-u-hc-h11-nu-jpan-ca-japanese-co-emoji-kn-true");
    EXPECT_EQ(loc->Maximize(), "ja-Jpan-JP-u-hc-h11-nu-jpan-ca-japanese-co-emoji-kn-true");
    EXPECT_EQ(loc->GetNumeric(), "true");
    EXPECT_EQ(loc->GetCaseFirst(), "");
    delete loc;
}

/**
 * @tc.name: IntlFuncTest003
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest003, TestSize.Level1)
{
    string locale = "en-GB";
    LocaleInfo *loc = new (std::nothrow) LocaleInfo(locale);
    if (!loc) {
        EXPECT_TRUE(false);
        return;
    }
    string language = "en";
    string script = "";
    string region = "GB";
    string baseName = "en-GB";
    EXPECT_EQ(loc->GetLanguage(), language);
    EXPECT_EQ(loc->GetScript(), script);
    EXPECT_EQ(loc->GetRegion(), region);
    EXPECT_EQ(loc->GetBaseName(), baseName);
    delete loc;
}

/**
 * @tc.name: IntlFuncTest004
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest004, TestSize.Level1)
{
    string locale = "en-GB";
    string expects = "2 January 1970, 18:17 – 12 January 1970, 18:20";
    vector<string> locales;
    locales.push_back(locale);
    string dateStyle = "long";
    string timeStyle = "short";
    map<string, string> options = { { "dateStyle", dateStyle },
                                    { "timeStyle", timeStyle } };
    DateTimeFormat *dateFormat = new (std::nothrow) DateTimeFormat(locales, options);
    if (!dateFormat) {
        EXPECT_TRUE(false);
        return;
    }
    int64_t fromMilliseconds = 123456789;
    int64_t toMilliseconds = 987654321;
    string out = dateFormat->FormatRange(fromMilliseconds, toMilliseconds);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(dateFormat->GetDateStyle(), dateStyle);
    EXPECT_EQ(dateFormat->GetTimeStyle(), timeStyle);
    delete dateFormat;
}

/**
 * @tc.name: IntlFuncTest005
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest005, TestSize.Level1)
{
    string locale = "ja";
    string expects = "1970年1月2日金曜日";
    vector<string> locales;
    locales.push_back(locale);
    map<string, string> options = { { "year", "numeric" },
                                    { "month", "long" },
                                    { "day", "numeric" },
                                    { "weekday", "long"} };
    DateTimeFormat *dateFormat = new (std::nothrow) DateTimeFormat(locales, options);
    if (!dateFormat) {
        EXPECT_TRUE(false);
        return;
    }
    int64_t milliseconds = 123456789;
    string out = dateFormat->Format(milliseconds);
    EXPECT_EQ(out, expects);
    int64_t fromMilliseconds = 123456789;
    int64_t toMilliseconds = 987654321;
    expects = "1970/1/2金曜日～1970/1/12月曜日";
    out = dateFormat->FormatRange(fromMilliseconds, toMilliseconds);
    EXPECT_EQ(out, expects);
    delete dateFormat;
}

/**
 * @tc.name: IntlFuncTest006
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest006, TestSize.Level1)
{
    string locale = "en-IN";
    string expects = "+1,23,456.79 euros";
    vector<string> locales;
    locales.push_back(locale);
    string useGrouping = "true";
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "currency";
    string currency = "EUR";
    map<string, string> options = { { "useGrouping", useGrouping },
                                    { "style", style },
                                    { "currency", currency },
                                    { "currencyDisplay", "name" },
                                    { "currencySign", "accounting" },
                                    { "signDisplay", "always" } };
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetUseGrouping(), useGrouping);
    EXPECT_EQ(numFmt->GetStyle(), style);
    EXPECT_EQ(numFmt->GetCurrency(), currency);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest007
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest007, TestSize.Level1)
{
    string locale = "zh-CN";
    string expects = "0,123,456.79米";
    vector<string> locales;
    locales.push_back("ban");
    locales.push_back(locale);
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "unit";
    map<string, string> options = { { "style", style },
                                    { "minimumIntegerDigits", minimumIntegerDigits },
                                    { "maximumFractionDigits", maximumFractionDigits },
                                    { "unit", "meter" },
                                    { "unitDisplay", "long"} };
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetStyle(), style);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest008
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest008, TestSize.Level1)
{
    string locale = "en-CN";
    string expects = "12,345,678.9%";
    vector<string> locales;
    locales.push_back(locale);
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "percent";
    map<string, string> options = { { "style", style },
                                    { "minimumIntegerDigits", minimumIntegerDigits },
                                    { "maximumFractionDigits", maximumFractionDigits } };
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetStyle(), style);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest009
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest009, TestSize.Level1)
{
    string locale = "en-CN";
    string expects = "0,123,456.79";
    vector<string> locales;
    locales.push_back(locale);
    string minimumIntegerDigits = "7";
    string maximumFractionDigits = "2";
    string style = "decimal";
    map<string, string> options = { { "style", style },
                                    { "minimumIntegerDigits", minimumIntegerDigits },
                                    { "maximumFractionDigits", maximumFractionDigits } };
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetStyle(), style);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest0010
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest010, TestSize.Level1)
{
    string locale = "en-CN";
    string expects = "1.234568E5";
    vector<string> locales;
    locales.push_back(locale);
    string style = "decimal";
    map<string, string> options = { { "style", style },
                                    { "notation", "scientific" } };
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetStyle(), style);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest0011
 * @tc.desc: Test Intl DateTimeFormat.format
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest011, TestSize.Level1)
{
    string locale = "en-CN";
    string expects = "123 thousand";
    vector<string> locales;
    locales.push_back(locale);
    string style = "decimal";
    map<string, string> options = { { "style", style },
                                    { "notation", "compact" },
                                    { "compactDisplay", "long" } };
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    EXPECT_EQ(numFmt->GetStyle(), style);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest0012
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0012, TestSize.Level1)
{
    string locale = "en";
    map<string, string> options = {};
    vector<string> locales;
    locales.push_back(locale);
    std::string expects = "3/11/82";
    DateTimeFormat *dateFormat = new (std::nothrow) DateTimeFormat(locales, options);
    if (!dateFormat) {
        EXPECT_TRUE(false);
        return;
    }
    int64_t milliseconds = 123456789123456;
    string out = dateFormat->Format(milliseconds);
    EXPECT_EQ(out, expects);
    delete dateFormat;
}

/**
 * @tc.name: IntlFuncTest0013
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0013, TestSize.Level1)
{
    string locale = "en-CN";
    vector<string> locales;
    locales.push_back(locale);
    map<string, string> options = {};
    std::string expects = "123,456.789";
    NumberFormat *numFmt = new (std::nothrow) NumberFormat(locales, options);
    if (!numFmt) {
        EXPECT_TRUE(false);
        return;
    }
    string out = numFmt->Format(123456.789);
    EXPECT_EQ(out, expects);
    delete numFmt;
}

/**
 * @tc.name: IntlFuncTest0014
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0014, TestSize.Level1)
{
    string locale = "zh-CN-u-hc-h12";
    string expects = "北美太平洋标准时间";
    vector<string> locales;
    locales.push_back("jessie");
    locales.push_back(locale);
    map<string, string> options = { { "timeZone", "America/Los_Angeles"  }, { "timeZoneName", "long" } };
    DateTimeFormat *dateFormat = new (std::nothrow) DateTimeFormat(locales, options);
    if (!dateFormat) {
        EXPECT_TRUE(false);
        return;
    }
    int64_t milliseconds = 123456789;
    string out = dateFormat->Format(milliseconds);
    EXPECT_TRUE(out.find(expects) != out.npos);
    delete dateFormat;
}

/**
 * @tc.name: IntlFuncTest0015
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0015, TestSize.Level1)
{
    string locale = "zh-CN-u-hc-h12";
    vector<string> locales;
    locales.push_back("jessie");
    locales.push_back(locale);
    map<string, string> options = { { "timeZone", "America/Los_Angeles"  }, { "timeZoneName", "long" } };
    DateTimeFormat *dateFormat = new (std::nothrow) DateTimeFormat(locales, options);
    if (!dateFormat) {
        EXPECT_TRUE(false);
        return;
    }
    EXPECT_EQ(dateFormat->GetTimeZone(), "America/Los_Angeles");
    EXPECT_EQ(dateFormat->GetTimeZoneName(), "long");
    delete dateFormat;
}

/**
 * @tc.name: IntlFuncTest0016
 * @tc.desc: Test Intl Collator
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0016, TestSize.Level1)
{
    // normal test case
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> inputOptions;
    Collator *collator = new Collator(locales, inputOptions);
    const string param1 = "abc";
    const string param2 = "cba";
    CompareResult result = collator->Compare(param1, param2);
    EXPECT_EQ(result, CompareResult::SMALLER);
    
    map<string, string> options;
    collator->ResolvedOptions(options);
    EXPECT_EQ(options.size(), 8);
    map<string, string>::iterator it = options.find("localeMatcher");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "best fit");
    }
    it = options.find("locale");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "en-US");
    }
    it = options.find("usage");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "sort");
    }
    it = options.find("sensitivity");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "variant");
    }
    it = options.find("ignorePunctuation");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "false");
    }
    it = options.find("numeric");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "false");
    }
    it = options.find("caseFirst");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "false");
    }
    it = options.find("collation");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "default");
    }
    delete collator;
}

/**
 * @tc.name: IntlFuncTest0017
 * @tc.desc: Test Intl Collator
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0017, TestSize.Level1)
{
    // normal test case
    vector<string> locales;
    locales.push_back("zh-Hans");
    locales.push_back("en-US");
    map<string, string> inputOptions = {
        { "localeMatcher", "lookup" },
        { "usage", "search"},
        { "sensitivity", "case"},
        { "ignorePunctuation", "true" },
        { "collation", "pinyin"},
        { "numeric", "true"},
        { "caseFirst", "upper"}
    };
    Collator *collator = new Collator(locales, inputOptions);
    const string param1 = "啊";
    const string param2 = "播";
    CompareResult result = collator->Compare(param1, param2);
    EXPECT_EQ(result, CompareResult::SMALLER);
    
    map<string, string> options;
    collator->ResolvedOptions(options);
    EXPECT_EQ(options.size(), 8);
    map<string, string>::iterator it = options.find("localeMatcher");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "lookup");
    }
    it = options.find("locale");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "zh-Hans");
    }
    it = options.find("usage");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "search");
    }
    it = options.find("sensitivity");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "case");
    }
    delete collator;
}

/**
 * @tc.name: IntlFuncTest0018
 * @tc.desc: Test Intl Collator
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0018, TestSize.Level1)
{
    // normal test case
    vector<string> locales;
    locales.push_back("zh-Hans");
    locales.push_back("en-US");
    map<string, string> inputOptions = {
        { "ignorePunctuation", "true" },
        { "collation", "pinyin"},
        { "numeric", "true"},
        { "caseFirst", "upper"}
    };
    Collator *collator = new Collator(locales, inputOptions);
    map<string, string> options;
    collator->ResolvedOptions(options);
    map<string, string>::iterator it = options.find("ignorePunctuation");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "true");
    }
    it = options.find("numeric");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "true");
    }
    it = options.find("caseFirst");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "upper");
    }
    it = options.find("collation");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "pinyin");
    }
    delete collator;
}

/**
 * @tc.name: IntlFuncTest0019
 * @tc.desc: Test Intl Collator
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0019, TestSize.Level1)
{
    // abnormal test case
    vector<string> locales;
    locales.push_back("fake locale");
    map<string, string> inputOptions = {
        { "localeMatcher", "fake value" },
        { "usage", "fake value"},
        { "sensitivity", "fake value"},
        { "ignorePunctuation", "fake value" },
        { "collation", "fake value"},
        { "numeric", "fake value"},
        { "caseFirst", "fake value"},
        { "fake key", "fake value"}
    };
    Collator *collator = new Collator(locales, inputOptions);
    const string param1 = "abc";
    const string param2 = "cba";
    CompareResult result = collator->Compare(param1, param2);
    EXPECT_EQ(result, CompareResult::SMALLER);
    
    map<string, string> options;
    collator->ResolvedOptions(options);
    EXPECT_EQ(options.size(), 8);
    map<string, string>::iterator it = options.find("localeMatcher");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "fake value");
    }
    std::string systemLocale = LocaleConfig::GetSystemLocale();
    it = options.find("locale");
    if (it != options.end()) {
        EXPECT_EQ(it->second, systemLocale);
    }
    it = options.find("usage");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "fake value");
    }
    it = options.find("sensitivity");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "fake value");
    }
    delete collator;
}

/**
 * @tc.name: IntlFuncTest0020
 * @tc.desc: Test Intl Collator
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0020, TestSize.Level1)
{
    // abnormal test case
    vector<string> locales;
    locales.push_back("fake locale");
    map<string, string> inputOptions = {
        { "ignorePunctuation", "fake value" },
        { "collation", "fake value"},
        { "numeric", "fake value"},
        { "caseFirst", "fake value"},
        { "fake key", "fake value"}
    };
    Collator *collator = new Collator(locales, inputOptions);
    map<string, string> options;
    collator->ResolvedOptions(options);
    map<string, string>::iterator it = options.find("ignorePunctuation");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "fake value");
    }
    it = options.find("numeric");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "fake value");
    }
    it = options.find("caseFirst");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "fake value");
    }
    it = options.find("collation");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "default");
    }
    delete collator;
}

/**
 * @tc.name: IntlFuncTest0021
 * @tc.desc: Test Intl PluralRules
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0021, TestSize.Level1)
{
    // normal test case
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options;
    PluralRules *plurals = new PluralRules(locales, options);
    string res = plurals->Select(0);
    EXPECT_EQ(res, "other");
    res = plurals->Select(1);
    EXPECT_EQ(res, "one");
    res = plurals->Select(2);
    EXPECT_EQ(res, "other");
    res = plurals->Select(5);
    EXPECT_EQ(res, "other");
    res = plurals->Select(20);
    EXPECT_EQ(res, "other");
    res = plurals->Select(200);
    EXPECT_EQ(res, "other");
    res = plurals->Select(12.34);
    EXPECT_EQ(res, "other");
    delete plurals;
}

/**
 * @tc.name: IntlFuncTest0022
 * @tc.desc: Test Intl PluralRules
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0022, TestSize.Level1)
{
    // normal test case
    vector<string> locales;
    locales.push_back("ar");
    map<string, string> options;
    PluralRules *plurals = new PluralRules(locales, options);
    string res = plurals->Select(0);
    EXPECT_EQ(res, "zero");
    res = plurals->Select(1);
    EXPECT_EQ(res, "one");
    res = plurals->Select(2);
    EXPECT_EQ(res, "two");
    res = plurals->Select(5);
    EXPECT_EQ(res, "few");
    res = plurals->Select(20);
    EXPECT_EQ(res, "many");
    res = plurals->Select(200);
    EXPECT_EQ(res, "other");
    res = plurals->Select(12.34);
    EXPECT_EQ(res, "other");
    delete plurals;
}

/**
 * @tc.name: IntlFuncTest0023
 * @tc.desc: Test Intl PluralRules
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0023, TestSize.Level1)
{
    // normal test case
    vector<string> locales;
    locales.push_back("ar");
    map<string, string> options = {
        { "localeMatcher", "best fit" },
        { "type", "cardinal" },
        { "minimumIntegerDigits", "1" },
        { "minimumFractionDigits", "0" },
        { "maximumFractionDigits", "21" },
        { "minimumSignificantDigits", "21" },
        { "maximumSignificantDigits", "21" },
    };
    PluralRules *plurals = new PluralRules(locales, options);
    string res = plurals->Select(0);
    EXPECT_EQ(res, "zero");
    res = plurals->Select(1);
    EXPECT_EQ(res, "one");
    res = plurals->Select(2);
    EXPECT_EQ(res, "two");
    res = plurals->Select(5);
    EXPECT_EQ(res, "few");
    res = plurals->Select(20);
    EXPECT_EQ(res, "many");
    res = plurals->Select(200);
    EXPECT_EQ(res, "other");
    res = plurals->Select(12.34);
    EXPECT_EQ(res, "other");
    delete plurals;
}

/**
 * @tc.name: IntlFuncTest0024
 * @tc.desc: Test Intl PluralRules
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0024, TestSize.Level1)
{
    // abnormal test cast
    // normal test case
    vector<string> locales;
    locales.push_back("fake_locale");
    map<string, string> options = {
        { "fake_localeMatcher", "best fit" },
        { "type", "fake_cardinal" },
        { "minimumIntegerDigits", "11111" },
        { "minimumFractionDigits", "-111" },
        { "maximumFractionDigits", "-111" },
        { "minimumSignificantDigits", "11111" },
        { "maximumSignificantDigits", "11111" },
    };
    PluralRules *plurals = new PluralRules(locales, options);
    string res = plurals->Select(0);
    EXPECT_EQ(res, "other");
    res = plurals->Select(1);
    EXPECT_EQ(res, "other");
    res = plurals->Select(2);
    EXPECT_EQ(res, "other");
    res = plurals->Select(5);
    EXPECT_EQ(res, "other");
    res = plurals->Select(20);
    EXPECT_EQ(res, "other");
    res = plurals->Select(200);
    EXPECT_EQ(res, "other");
    res = plurals->Select(12.34);
    EXPECT_EQ(res, "other");
    delete plurals;
}

/**
 * @tc.name: IntlFuncTest0025
 * @tc.desc: Test Intl RelativeTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0025, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options;
    RelativeTimeFormat *formatter = new RelativeTimeFormat(locales, options);
    
    double number = 2022;
    string unit = "year";
    string res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 2,022 years");
    vector<vector<string>> timeVector;
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 5);

    number = 3;
    unit = "quarter";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 3 quarters");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 8);

    number = 11;
    unit = "month";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 11 months");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 11);

    number = 2;
    unit = "week";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 2 weeks");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 14);

    number = 18;
    unit = "day";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 18 days");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 17);
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0026
 * @tc.desc: Test Intl RelativeTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0026, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options;
    RelativeTimeFormat *formatter = new RelativeTimeFormat(locales, options);
    
    double number = 23;
    string unit = "hour";
    string res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 23 hours");
    vector<vector<string>> timeVector;
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 3);

    number = 59;
    unit = "minute";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 59 minutes");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 6);

    number = 1;
    unit = "second";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "in 1 second");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 9);

    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0027
 * @tc.desc: Test Intl RelativeTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0027, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-Hans");
    map<string, string> options = {
        { "localeMatcher", "best fit" },
        { "numeric", "auto" },
        { "style", "long" }
    };
    RelativeTimeFormat *formatter = new RelativeTimeFormat(locales, options);
    
    double number = 2022;
    string unit = "year";
    string res = formatter->Format(number, unit);
    EXPECT_EQ(res, "2,022年后");
    vector<vector<string>> timeVector;
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 4);

    number = 3;
    unit = "quarter";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "3个季度后");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 6);

    number = 11;
    unit = "month";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "11个月后");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 8);

    number = 2;
    unit = "week";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "2周后");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 10);
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0028
 * @tc.desc: Test Intl RelativeTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0028, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-Hans");
    map<string, string> options = {
        { "localeMatcher", "lookup" },
        { "numeric", "auto" },
        { "style", "long" }
    };
    RelativeTimeFormat *formatter = new RelativeTimeFormat(locales, options);
    
    double number = 18;
    string unit = "day";
    string res = formatter->Format(number, unit);
    EXPECT_EQ(res, "18天后");
    vector<vector<string>> timeVector;
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 2);

    number = 23;
    unit = "hour";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "23小时后");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 4);

    number = 59;
    unit = "minute";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "59分钟后");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 6);

    number = 1;
    unit = "second";
    res = formatter->Format(number, unit);
    EXPECT_EQ(res, "1秒钟后");
    formatter->FormatToParts(number, unit, timeVector);
    EXPECT_EQ(timeVector.size(), 8);

    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0029
 * @tc.desc: Test Intl RelativeTimeFormatter
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0029, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("xxxx");
    locales.push_back("zh-Hans");
    map<string, string> options = {
        { "localeMatcher", "best fit" },
        { "numeric", "auto" },
        { "style", "long" }
    };
    RelativeTimeFormat *formatter = new RelativeTimeFormat(locales, options);
    map<string, string> res;
    formatter->GetResolvedOptions(res);
    EXPECT_EQ(res.size(), 4);

    map<string, string>::iterator it = res.find("locale");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "zh-Hans");
    }
    it = res.find("style");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "long");
    }
    it = res.find("numeric");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "auto");
    }
    it = res.find("numberingSystem");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "latn");
    }
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0030
 * @tc.desc: Test Intl NumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0030, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options;
    NumberFormat *formatter = new NumberFormat(locales, options);
    
    string res = formatter->Format(123);
    EXPECT_EQ(res, "123");
    res = formatter->Format(123.456);
    EXPECT_EQ(res, "123.456");
    
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0031
 * @tc.desc: Test Intl NumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0031, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-Hans");
    map<string, string> options = {
        { "locale", "zh-Hans" },
        { "currency", "EUR" },
        { "currencySign", "narrowSymbol" },
        { "currencyDisplay", "symbol" },
        { "unit", "meter" },
        { "unitDisplay", "long" },
        { "unitUsage", "length-person" },
        { "signDisplay", "always" },
        { "compactDisplay", "long" },
        { "notation", "standard" },
        { "localeMatcher", "lookup" },
        { "style", "decimal" },
        { "numberingSystem", "latn" },
        { "useGroup", "true" },
        { "minimumIntegerDigits", "1" },
        { "minimumFractionDigits", "0" },
        { "maximumFractionDigits", "20" },
        { "minimumSignificantDigits", "1" },
        { "maximumSignificantDigits", "20" }
    };
    NumberFormat *formatter = new NumberFormat(locales, options);
    
    string formatRes = formatter->Format(123);
    EXPECT_EQ(formatRes, "+123");
    formatRes = formatter->Format(123.456);
    EXPECT_EQ(formatRes, "+123.456");
    
    map<string, string> res;
    formatter->GetResolvedOptions(res);
    EXPECT_EQ(res.size(), 12);
    map<string, string>::iterator it = res.find("locale");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "zh-Hans");
    }
    it = res.find("signDisplay");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "always");
    }
    it = res.find("notation");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "standard");
    }
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0032
 * @tc.desc: Test Intl NumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0032, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-Hans");
    map<string, string> options = {
        { "locale", "zh-Hans" },
        { "currency", "CNY" },
        { "currencySign", "symbol" },
        { "currencyDisplay", "symbol" },
        { "unit", "meter" },
        { "unitDisplay", "long" },
        { "unitUsage", "length-person" },
        { "signDisplay", "always" },
        { "compactDisplay", "long" },
        { "notation", "standard" },
        { "localeMatcher", "best fit" },
        { "style", "decimal" },
        { "numberingSystem", "latn" },
        { "useGroup", "true" },
        { "minimumIntegerDigits", "1" },
        { "minimumFractionDigits", "0" },
        { "maximumFractionDigits", "21" },
        { "minimumSignificantDigits", "1" },
        { "maximumSignificantDigits", "21" }
    };
    NumberFormat *formatter = new NumberFormat(locales, options);
    map<string, string> res;
    formatter->GetResolvedOptions(res);
    map<string, string>::iterator it = res.find("style");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "decimal");
    }
    it = res.find("numberingSystem");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "latn");
    }
    it = res.find("useGrouping");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "true");
    }
    it = res.find("minimumIntegerDigits");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "1");
    }
    it = res.find("minimumFractionDigits");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "0");
    }
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0033
 * @tc.desc: Test Intl NumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0033, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-Hans");
    map<string, string> options = {
        { "locale", "fake locale" },
        { "currency", "fake currency" },
        { "currencySign", "fake sign" },
        { "currencyDisplay", "symbol" },
        { "unit", "meter" },
        { "unitDisplay", "fake value" },
        { "unitUsage", "length-person" },
        { "signDisplay", "always" },
        { "compactDisplay", "long" },
        { "notation", "fake value" },
        { "localeMatcher", "best fit" },
        { "style", "decimal" },
        { "numberingSystem", "latn" },
        { "useGroup", "fake value" },
        { "minimumIntegerDigits", "1" },
        { "minimumFractionDigits", "0" },
        { "maximumFractionDigits", "21" },
        { "minimumSignificantDigits", "1" },
        { "maximumSignificantDigits", "21" }
    };
    NumberFormat *formatter = new NumberFormat(locales, options);
    map<string, string> res;
    formatter->GetResolvedOptions(res);
    map<string, string>::iterator it = res.find("maximumFractionDigits");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "21");
    }
    it = res.find("minimumSignificantDigits");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "1");
    }
    it = res.find("maximumSignificantDigits");
    if (it != res.end()) {
        EXPECT_EQ(it->second, "21");
    }
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0034
 * @tc.desc: Test Intl NumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0034, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options = {
        { "locale", "fake locale" },
        { "currency", "USD" },
        { "currencySign", "symbol" },
        { "currencyDisplay", "symbol" },
        { "unit", "fake unit" },
        { "unitDisplay", "long" },
        { "unitUsage", "fake usage" },
        { "signDisplay", "always" },
        { "compactDisplay", "long" },
        { "notation", "standard" },
        { "localeMatcher", "lookup" },
        { "style", "currency" },
        { "numberingSystem", "latn" },
        { "useGrouping", "true" },
        { "minimumIntegerDigits", "1" },
        { "minimumFractionDigits", "0" },
        { "maximumFractionDigits", "20" },
        { "minimumSignificantDigits", "1" },
        { "maximumSignificantDigits", "20" }
    };
    NumberFormat *formatter = new NumberFormat(locales, options);
    string res = formatter->Format(123);
    EXPECT_EQ(res, "+$123");
    res = formatter->Format(123.456);
    EXPECT_EQ(res, "+$123.456");
    res = formatter->GetCurrency();
    EXPECT_EQ(res, "USD");
    res = formatter->GetCurrencySign();
    EXPECT_EQ(res, "symbol");
    res = formatter->GetStyle();
    EXPECT_EQ(res, "currency");
    res = formatter->GetNumberingSystem();
    EXPECT_EQ(res, "latn");
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0035
 * @tc.desc: Test Intl NumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0035, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options = {
        { "locale", "fake locale" },
        { "currency", "USD" },
        { "currencySign", "fake sign" },
        { "currencyDisplay", "symbol" },
        { "unit", "fake unit" },
        { "unitDisplay", "long" },
        { "unitUsage", "length-person" },
        { "signDisplay", "fake display" },
        { "compactDisplay", "long" },
        { "notation", "standard" },
        { "localeMatcher", "lookup" },
        { "style", "currency" },
        { "numberingSystem", "latn" },
        { "useGrouping", "false" },
        { "minimumIntegerDigits", "1" },
        { "minimumFractionDigits", "0" },
        { "maximumFractionDigits", "17" },
        { "minimumSignificantDigits", "1" },
        { "maximumSignificantDigits", "17" }
    };
    NumberFormat *formatter = new NumberFormat(locales, options);
    string res = formatter->GetUseGrouping();
    EXPECT_EQ(res, "false");
    res = formatter->GetMinimumIntegerDigits();
    EXPECT_EQ(res, "1");
    res = formatter->GetMinimumFractionDigits();
    EXPECT_EQ(res, "0");
    res = formatter->GetMaximumFractionDigits();
    EXPECT_EQ(res, "17");
    res = formatter->GetMinimumSignificantDigits();
    EXPECT_EQ(res, "1");
    res = formatter->GetMaximumSignificantDigits();
    EXPECT_EQ(res, "17");
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0036
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0036, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> options;
    DateTimeFormat *formatter = new DateTimeFormat(locales, options);

    int64_t milliseconds = 123456789;
    string res = formatter->Format(milliseconds);
    EXPECT_EQ(res, "1/2/70");
    
    int64_t milliseconds2 = 987654321;
    res = formatter->FormatRange(milliseconds, milliseconds2);
    EXPECT_EQ(res, "1/2/70 \xE2\x80\x93 1/12/70");
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0037
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0037, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-CN");
    map<string, string> options = {
        { "locale", "zh-CN" },
        { "dateStyle", "medium" },
        { "timeStyle", "long" },
        { "hourCycle", "h24" },
        { "timeZone", "Asia/Shanghai" },
        { "numberingSystem", "latn" },
        { "hour12", "false" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "2-digit" },
        { "month", "2-digit" },
        { "day", "2-digit" },
        { "hour", "2-digit" },
        { "minute", "2-digit" },
        { "second", "2-digit" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "lookup" },
        { "formatMatcher", "basic" }
    };
    DateTimeFormat *formatter = new DateTimeFormat(locales, options);

    int64_t milliseconds = 123456789;
    string res = formatter->Format(milliseconds);
    // 2022年12月19日 GMT+8 15:18:24
    EXPECT_TRUE(res.length() > 0);
    
    int64_t milliseconds2 = 987654321;
    res = formatter->FormatRange(milliseconds, milliseconds2);
    // 2022/12/19 GMT+8 15:18:24 \xE2\x80\x93 2023/11/18 GMT+8 14:17:23
    EXPECT_TRUE(res.length() > 0);

    res = formatter->GetDateStyle();
    EXPECT_EQ(res, "medium");
    res = formatter->GetTimeStyle();
    EXPECT_EQ(res, "long");
    res = formatter->GetHourCycle();
    EXPECT_EQ(res, "h24");
    res = formatter->GetTimeZone();
    EXPECT_EQ(res, "Asia/Shanghai");
    res = formatter->GetTimeZoneName();
    EXPECT_EQ(res, "long");
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0038
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0038, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("zh-CN");
    map<string, string> options = {
        { "locale", "zh-CN" },
        { "dateStyle", "full" },
        { "timeStyle", "full" },
        { "hourCycle", "h24" },
        { "timeZone", "Asia/Shanghai" },
        { "numberingSystem", "latn" },
        { "hour12", "false" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "numeric" },
        { "month", "numeric" },
        { "day", "numeric" },
        { "hour", "numeric" },
        { "minute", "numeric" },
        { "second", "numeric" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "lookup" },
        { "formatMatcher", "basic" }
    };
    DateTimeFormat *formatter = new DateTimeFormat(locales, options);
    string res = formatter->GetNumberingSystem();
    EXPECT_EQ(res, "latn");
    res = formatter->GetHour12();
    EXPECT_EQ(res, "false");
    res = formatter->GetWeekday();
    EXPECT_EQ(res, "long");
    res = formatter->GetEra();
    EXPECT_EQ(res, "long");
    res = formatter->GetYear();
    EXPECT_EQ(res, "numeric");
    res = formatter->GetMonth();
    EXPECT_EQ(res, "numeric");
    res = formatter->GetDay();
    EXPECT_EQ(res, "numeric");
    res = formatter->GetHour();
    EXPECT_EQ(res, "numeric");
    res = formatter->GetMinute();
    EXPECT_EQ(res, "numeric");
    res = formatter->GetSecond();
    EXPECT_EQ(res, "numeric");
    delete formatter;
}

/**
 * @tc.name: IntlFuncTest0039
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0039, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> inputOptions = {
        { "locale", "en-US" },
        { "dateStyle", "medium" },
        { "timeStyle", "long" },
        { "hourCycle", "h12" },
        { "timeZone", "Asia/Shanghai" },
        { "numberingSystem", "latn" },
        { "hour12", "true" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "2-digit" },
        { "month", "2-digit" },
        { "day", "2-digit" },
        { "hour", "2-digit" },
        { "minute", "2-digit" },
        { "second", "2-digit" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "lookup" },
        { "formatMatcher", "basic" }
    };
    std::unique_ptr<DateTimeFormat> formatter = DateTimeFormat::CreateInstance(locales, inputOptions);
    int64_t milliseconds = 123456789;
    string res = formatter->Format(milliseconds);
    // Dec 19, 2022, 3:18:24 PM GMT+8
    EXPECT_TRUE(res.length() > 0);
    int64_t milliseconds2 = 987654321;
    res = formatter->FormatRange(milliseconds, milliseconds2);
    // Dec 19, 2022, 3:18:24 PM GMT+8 \xE2\x80\x93 Nov 18, 2023, 2:17:23 PM GMT+8
    EXPECT_TRUE(res.length() > 0);
    map<string, string> options;
    formatter->GetResolvedOptions(options);
    EXPECT_EQ(options.size(), 20);
    map<string, string>::iterator it = options.find("locale");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "en-US");
    }
    it = options.find("dateStyle");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "medium");
    }
    it = options.find("timeStyle");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "long");
    }
}

/**
 * @tc.name: IntlFuncTest0040
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0040, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-GB");
    map<string, string> inputOptions = {
        { "locale", "en-GB" },
        { "dateStyle", "medium" },
        { "timeStyle", "long" },
        { "hourCycle", "h12" },
        { "timeZone", "Asia/Shanghai" },
        { "numberingSystem", "latn" },
        { "hour12", "true" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "numeric" },
        { "month", "numeric" },
        { "day", "numeric" },
        { "hour", "numeric" },
        { "minute", "numeric" },
        { "second", "numeric" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "lookup" },
        { "formatMatcher", "basic" }
    };
    std::unique_ptr<DateTimeFormat> formatter = DateTimeFormat::CreateInstance(locales, inputOptions);
    map<string, string> options;
    formatter->GetResolvedOptions(options);
    map<string, string>::iterator it = options.find("hourCycle");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "h12");
    }
    it = options.find("timeZone");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "Asia/Shanghai");
    }
    it = options.find("numberingSystem");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "latn");
    }
    it = options.find("hour12");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "true");
    }
    it = options.find("weekday");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "long");
    }
}

/**
 * @tc.name: IntlFuncTest0041
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0041, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> inputOptions = {
        { "locale", "en-US" },
        { "dateStyle", "medium" },
        { "timeStyle", "medium" },
        { "hourCycle", "h24" },
        { "timeZone", "Asia/Shanghai" },
        { "numberingSystem", "latn" },
        { "hour12", "false" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "2-digit" },
        { "month", "2-digit" },
        { "day", "2-digit" },
        { "hour", "2-digit" },
        { "minute", "2-digit" },
        { "second", "2-digit" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "best fit" },
        { "formatMatcher", "best fit" }
    };
    std::unique_ptr<DateTimeFormat> formatter = DateTimeFormat::CreateInstance(locales, inputOptions);
    map<string, string> options;
    formatter->GetResolvedOptions(options);
    map<string, string>::iterator it = options.find("era");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "long");
    }
    it = options.find("year");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "2-digit");
    }
    it = options.find("month");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "2-digit");
    }
    it = options.find("day");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "2-digit");
    }
    it = options.find("hour");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "2-digit");
    }
}

/**
 * @tc.name: IntlFuncTest0042
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0042, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> inputOptions = {
        { "locale", "en-US" },
        { "dateStyle", "full" },
        { "timeStyle", "long" },
        { "hourCycle", "h12" },
        { "timeZone", "Asia/Singapore" },
        { "numberingSystem", "latn" },
        { "hour12", "true" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "2-digit" },
        { "month", "2-digit" },
        { "day", "2-digit" },
        { "hour", "numeric" },
        { "minute", "numeric" },
        { "second", "numeric" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "lookup" },
        { "formatMatcher", "basic" }
    };
    std::unique_ptr<DateTimeFormat> formatter = DateTimeFormat::CreateInstance(locales, inputOptions);
    map<string, string> options;
    formatter->GetResolvedOptions(options);
    map<string, string>::iterator it = options.find("minute");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "numeric");
    }
    it = options.find("second");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "numeric");
    }
    it = options.find("timeZoneName");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "long");
    }
    it = options.find("dayPeriod");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "long");
    }
}

/**
 * @tc.name: IntlFuncTest0043
 * @tc.desc: Test Intl DateTimeFormat
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0043, TestSize.Level1)
{
    vector<string> locales;
    locales.push_back("en-US");
    map<string, string> inputOptions = {
        { "locale", "en-US" },
        { "dateStyle", "long" },
        { "timeStyle", "long" },
        { "hourCycle", "h12" },
        { "timeZone", "America/Los_Angeles" },
        { "numberingSystem", "latn" },
        { "hour12", "true" },
        { "weekday", "long" },
        { "era", "long" },
        { "year", "numeric" },
        { "month", "numeric" },
        { "day", "numeric" },
        { "hour", "2-digit" },
        { "minute", "2-digit" },
        { "second", "2-digit" },
        { "timeZoneName", "long" },
        { "dayPeriod", "long" },
        { "localeMatcher", "lookup" },
        { "formatMatcher", "basic" }
    };
    std::unique_ptr<DateTimeFormat> formatter = DateTimeFormat::CreateInstance(locales, inputOptions);
    map<string, string> options;
    formatter->GetResolvedOptions(options);
    map<string, string>::iterator it = options.find("localeMatcher");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "lookup");
    }
    it = options.find("formatMatcher");
    if (it != options.end()) {
        EXPECT_EQ(it->second, "basic");
    }
}

/**
 * @tc.name: IntlFuncTest0044
 * @tc.desc: Test Intl LocaleInfo
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0044, TestSize.Level1)
{
    string localeTag = "zh-Hans-CN";
    map<string, string> configs = {
        { "calendar", "chinese" },
        { "collation", "pinyin" },
        { "hourCycle", "h12" },
        { "numberingSystem", "latn" },
        { "numeric", "true" },
        { "caseFirst", "upper" }
    };
    LocaleInfo *locale = new LocaleInfo(localeTag, configs);
    string res = locale->GetLanguage();
    EXPECT_EQ(res, "zh");
    res = locale->GetScript();
    EXPECT_EQ(res, "Hans");
    res = locale->GetRegion();
    EXPECT_EQ(res, "CN");
    res = locale->GetBaseName();
    EXPECT_EQ(res, "zh-Hans-CN");
    res = locale->GetCalendar();
    EXPECT_EQ(res, "chinese");
    res = locale->GetCollation();
    EXPECT_EQ(res, "pinyin");
    res = locale->GetHourCycle();
    EXPECT_EQ(res, "h12");
    res = locale->GetNumberingSystem();
    EXPECT_EQ(res, "latn");
    res = locale->GetNumeric();
    EXPECT_EQ(res, "true");
    res = locale->GetCaseFirst();
    EXPECT_EQ(res, "upper");
    icu::Locale icuLocale = locale->GetLocale();
    res = locale->ToString();
    EXPECT_EQ(res, "zh-Hans-CN-u-hc-h12-nu-latn-ca-chinese-co-pinyin-kf-upper-kn-true");
    delete locale;
}

/**
 * @tc.name: IntlFuncTest0045
 * @tc.desc: Test Intl ReadSystemParameter
 * @tc.type: FUNC
 */
HWTEST_F(IntlTest, IntlFuncTest0045, TestSize.Level1)
{
    string paramKey = "const.global.language";
    int paramLength = 128;
    string res = ReadSystemParameter(paramKey.c_str(), paramLength);
    EXPECT_TRUE(res.length() > 0);

    paramKey = "fake system param";
    res = ReadSystemParameter(paramKey.c_str(), paramLength);
    EXPECT_TRUE(res.length() == 0);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS