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

#include <gtest/gtest.h>
#include "character.h"
#include "i18n_break_iterator.h"
#include "i18n_calendar.h"
#include "i18n_normalizer.h"
#include "i18n_timezone.h"
#include "index_util.h"
#include "locale_config.h"
#include "measure_data.h"
#include "phone_number_format.h"
#include "preferred_language.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class I18nTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void I18nTest::SetUpTestCase(void)
{}

void I18nTest::TearDownTestCase(void)
{}

void I18nTest::SetUp(void)
{}

void I18nTest::TearDown(void)
{}

/**
 * @tc.name: I18nFuncTest001
 * @tc.desc: Test I18n PreferredLanguage GetPreferredLocale
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest001, TestSize.Level1)
{
    string preferredLocale = PreferredLanguage::GetPreferredLocale();
    EXPECT_TRUE(preferredLocale.size() > 0);
    string systemLocale = "zh-Hans-CN";
    if (LocaleConfig::SetSystemLocale(systemLocale)) {
        I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
        PreferredLanguage::AddPreferredLanguage("en-US", 0, errorCode);
        if (errorCode == I18nErrorCode::SUCCESS) {
            preferredLocale = PreferredLanguage::GetPreferredLocale();
            EXPECT_EQ(preferredLocale, "en-CN");
        }
        bool status = PreferredLanguage::RemovePreferredLanguage(0);
        EXPECT_TRUE(status);
    }

    std::vector<std::string> languageList = PreferredLanguage::GetPreferredLanguageList();
    bool res = languageList.size() > 0;
    EXPECT_TRUE(res);

    std::string preferredLanguage = PreferredLanguage::GetFirstPreferredLanguage();
    res = preferredLanguage.length() > 0;
    EXPECT_TRUE(res);
}

/**
 * @tc.name: I18nFuncTest002
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest002, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFD, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 3); // 3 is the NFD normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\uFB01");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 4); // 4 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u2075");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 6); // 6 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u017F\u0323\u0307");
}

/**
 * @tc.name: I18nFuncTest003
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest003, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFC, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 3); // 3 is the NFC normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\uFB01");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 4); // 4 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u2075");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 5); // 5 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u1E9B\u0323");
}

/**
 * @tc.name: I18nFuncTest004
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest004, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFKD, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the NFKD normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\u0066\u0069");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u0035");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 5); // 5 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0073\u0323\u0307");
}

/**
 * @tc.name: I18nFuncTest005
 * @tc.desc: Test I18n Normalizer
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest005, TestSize.Level1)
{
    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    I18nNormalizer normalizer(I18nNormalizerMode::NFKC, errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);

    string text = "\uFB01"; // \uFB01 is ﬁ
    string normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the NFKC normalized length of ﬁ.
    EXPECT_EQ(normalizedText, "\u0066\u0069");

    text = "\u0032\u2075";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 2); // 2 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u0032\u0035");

    text = "\u1E9B\u0323";
    normalizedText = normalizer.Normalize(text.c_str(), text.length(), errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    EXPECT_EQ(normalizedText.length(), 3); // 3 is the expected normalized text length.
    EXPECT_EQ(normalizedText, "\u1E69");
}

/**
 * @tc.name: I18nFuncTest006
 * @tc.desc: Test I18n BreakIterator
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest006, TestSize.Level1)
{
    I18nBreakIterator breakIterator("en-US");
    std::string testText = "this is the test Text for I18nBreakIterator";
    breakIterator.SetText(testText.c_str());
    std::string outputText;
    breakIterator.GetText(outputText);
    EXPECT_EQ(testText, outputText);

    int32_t currentIdx = breakIterator.Current();
    EXPECT_EQ(currentIdx, 0);

    int32_t firstIdx = breakIterator.First();
    EXPECT_EQ(firstIdx, 0);

    int32_t nextIdx = breakIterator.Next();
    EXPECT_EQ(nextIdx, 5);
    nextIdx = breakIterator.Next(8);
    EXPECT_EQ(nextIdx, -1);
    nextIdx = breakIterator.Next(-2);
    EXPECT_EQ(nextIdx, 22);

    int32_t previousIdx = breakIterator.Previous();
    EXPECT_EQ(previousIdx, 17);

    int32_t followingIdx = breakIterator.Following(6);
    EXPECT_EQ(followingIdx, 8);

    bool isBoundary = breakIterator.IsBoundary(18);
    EXPECT_FALSE(isBoundary);
}

/**
 * @tc.name: I18nFuncTest007
 * @tc.desc: Test I18n Calendar
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest007, TestSize.Level1)
{
    std::string locale = "zh-Hans";
    I18nCalendar calendar1(locale);
    calendar1.SetTime(12345678999);
    int32_t month = calendar1.Get(UCalendarDateFields::UCAL_MONTH);
    EXPECT_EQ(month, 4);

    calendar1.Set(2023, 3, 23);
    int32_t year = calendar1.Get(UCalendarDateFields::UCAL_YEAR);
    EXPECT_EQ(year, 2023);

    calendar1.Set(UCalendarDateFields::UCAL_YEAR, 2022);
    year = calendar1.Get(UCalendarDateFields::UCAL_YEAR);
    EXPECT_EQ(year, 2022);

    calendar1.SetTimeZone("Asia/Shanghai");
    std::string timezone = calendar1.GetTimeZone();
    EXPECT_EQ(timezone, "China Standard Time");

    calendar1.SetMinimalDaysInFirstWeek(4);
    int32_t minimalDaysInFirstWeek = calendar1.GetMinimalDaysInFirstWeek();
    EXPECT_EQ(minimalDaysInFirstWeek, 4);

    calendar1.SetFirstDayOfWeek(2);
    int32_t firstDayOfWeek = calendar1.GetFirstDayOfWeek();
    EXPECT_EQ(firstDayOfWeek, 2);

    bool isWeekend = calendar1.IsWeekend();
    EXPECT_TRUE(isWeekend);
    UErrorCode status = U_ZERO_ERROR;
    isWeekend = calendar1.IsWeekend(123456789, status);
    EXPECT_TRUE(U_SUCCESS(status));
    EXPECT_FALSE(isWeekend);

    locale = "en-US";
    I18nCalendar calendar2(locale, CalendarType::BUDDHIST);
    std::string displayName = calendar2.GetDisplayName(locale);
    EXPECT_EQ(displayName, "Buddhist Calendar");
}

/**
 * @tc.name: I18nFuncTest008
 * @tc.desc: Test I18n IndexUtil
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest008, TestSize.Level1)
{
    IndexUtil indexUtil("en-US");
    std::vector<std::string> indexList = indexUtil.GetIndexList();
    int32_t num = indexList.size();
    EXPECT_EQ(num, 28);

    indexUtil.AddLocale("ru");
    indexList = indexUtil.GetIndexList();
    num = indexList.size();
    EXPECT_EQ(num, 59);

    std::string index = indexUtil.GetIndex("IndexUtil");
    EXPECT_EQ(index, "I");
}

/**
 * @tc.name: I18nFuncTest009
 * @tc.desc: Test I18n PhoneNumberFormat
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest009, TestSize.Level1)
{
    std::map<std::string, std::string> options {
        { "type", "RFC3966" }
    };
    auto formatter = PhoneNumberFormat::CreateInstance("CN", options);
    bool isValid = formatter->isValidPhoneNumber("12312341234");
    EXPECT_FALSE(isValid);
    isValid = formatter->isValidPhoneNumber("15866617376");
    EXPECT_TRUE(isValid);

    std::string formatRes = formatter->format("12312341234");
    EXPECT_EQ(formatRes, "tel:+86-12312341234");

    std::string locationName = formatter->getLocationName("15812312345", "zh-CN");
    EXPECT_EQ(locationName, "广东省湛江市");
}

/**
 * @tc.name: I18nFuncTest010
 * @tc.desc: Test I18n TimeZone
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest010, TestSize.Level1)
{
    std::string zone = "Asia/Shanghai";
    std::unique_ptr<I18nTimeZone> timezone1 = I18nTimeZone::CreateInstance(zone, true);
    int32_t offset = timezone1->GetOffset(123456789);
    EXPECT_EQ(offset, 28800000);
    int32_t rawOffset = timezone1->GetRawOffset();
    EXPECT_EQ(rawOffset, 28800000);
    std::string timezoneId = timezone1->GetID();
    EXPECT_EQ(timezoneId, "Asia/Shanghai");

    std::string cityName = "Shanghai";
    std::unique_ptr<I18nTimeZone> timezone2 = I18nTimeZone::CreateInstance(cityName, false);
    std::string displayName = timezone2->GetDisplayName();
    EXPECT_EQ(displayName, "中国标准时间");
    displayName = timezone2->GetDisplayName(false);
    EXPECT_EQ(displayName, "中国标准时间");
    std::string locale = "en-US";
    displayName = timezone2->GetDisplayName(locale);
    EXPECT_EQ(displayName, "China Standard Time");
    displayName = timezone2->GetDisplayName(locale, false);
    EXPECT_EQ(displayName, "China Standard Time");

    I18nErrorCode errorCode = I18nErrorCode::SUCCESS;
    std::set<std::string> availableIDs = I18nTimeZone::GetAvailableIDs(errorCode);
    EXPECT_EQ(errorCode, I18nErrorCode::SUCCESS);
    int32_t count = availableIDs.size();
    EXPECT_EQ(count, 416);

    std::set<std::string> availableZoneCityIDs = I18nTimeZone::GetAvailableZoneCityIDs();
    count = availableZoneCityIDs.size();
    EXPECT_EQ(count, 24);

    displayName = I18nTimeZone::GetCityDisplayName(cityName, locale);
    EXPECT_EQ(displayName, "Shanghai (China)");
}

/**
 * @tc.name: I18nFuncTest011
 * @tc.desc: Test I18n Character
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest011, TestSize.Level1)
{
    std::string testString = "1";
    bool res = IsDigit(testString);
    EXPECT_TRUE(res);
    testString = "A";
    res = IsDigit(testString);
    EXPECT_FALSE(res);

    testString = " ";
    res = IsSpaceChar(testString);
    EXPECT_TRUE(res);
    testString = "\n";
    res = IsSpaceChar(testString);
    EXPECT_FALSE(res);

    testString = " ";
    res = IsWhiteSpace(testString);
    EXPECT_TRUE(res);
    testString = "A";
    res = IsWhiteSpace(testString);
    EXPECT_FALSE(res);

    testString = "مرحبا";
    res = IsRTLCharacter(testString);
    EXPECT_TRUE(res);
    testString = "A";
    res = IsRTLCharacter(testString);
    EXPECT_FALSE(res);
}

/**
 * @tc.name: I18nFuncTest012
 * @tc.desc: Test I18n Character
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest012, TestSize.Level1)
{
    std::string testString = "表意";
    bool res = IsIdeoGraphic(testString);
    EXPECT_TRUE(res);
    testString = "A";
    res = IsIdeoGraphic(testString);
    EXPECT_FALSE(res);

    testString = "A";
    res = IsLetter(testString);
    EXPECT_TRUE(res);
    testString = " ";
    res = IsLetter(testString);
    EXPECT_FALSE(res);

    testString = "a";
    res = IsLowerCase(testString);
    EXPECT_TRUE(res);
    testString = "A";
    res = IsLowerCase(testString);
    EXPECT_FALSE(res);

    testString = "A";
    res = IsUpperCase(testString);
    EXPECT_TRUE(res);
    testString = "a";
    res = IsUpperCase(testString);
    EXPECT_FALSE(res);

    testString = "A";
    std::string type = GetType(testString);
    EXPECT_EQ(type, "U_UPPERCASE_LETTER");
}

/**
 * @tc.name: I18nFuncTest013
 * @tc.desc: Test I18n LocaleConfig
 * @tc.type: FUNC
 */
HWTEST_F(I18nTest, I18nFuncTest013, TestSize.Level1)
{
    double value = 1000;
    std::string fromUnit = "cup";
    std::string fromMeasureSystem = "US";
    std::string toUnit = "liter";
    std::string toMeasureSystem = "SI";

    int convertRes = Convert(value, fromUnit, fromMeasureSystem, toUnit, toMeasureSystem);
    EXPECT_EQ(convertRes, 1);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS