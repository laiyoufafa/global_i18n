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
#include "i18n_normalizer.h"
#include "locale_config.h"
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
    }
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
} // namespace I18n
} // namespace Global
} // namespace OHOS