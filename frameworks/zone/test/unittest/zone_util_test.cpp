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

#include <gtest/gtest.h>
#include "zone_util.h"

using namespace OHOS::Global::I18n;
using testing::ext::TestSize;
using namespace std;

namespace OHOS {
namespace Global {
namespace I18n {
class ZoneUtilTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void ZoneUtilTest::SetUpTestCase(void)
{}

void ZoneUtilTest::TearDownTestCase(void)
{}

void ZoneUtilTest::SetUp(void)
{
}

void ZoneUtilTest::TearDown(void)
{}

/**
 * @tc.name: ZoneUtilFuncTest001
 * @tc.desc: Test ZoneUtil GetDefaultZone
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest,  ZoneUtilFuncTest001, TestSize.Level1)
{
    string expects[] = { "Asia/Shanghai", "America/New_York", "Asia/Shanghai", "America/New_York" };
    string countries[] = { "CN", "US", "cn", "us" };
    ZoneUtil util;
    int count = 4;
    for (int i = 0; i < count; ++i) {
        string out = util.GetDefaultZone(countries[i].c_str());
        EXPECT_EQ(out, expects[i]);
    }
}

/**
 * @tc.name: ZoneUtilFuncTest002
 * @tc.desc: Test ZoneUtil GetDefaultZone with offset
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest,  ZoneUtilFuncTest002, TestSize.Level1)
{
    string expects[] = { "Asia/Shanghai", "America/Detroit" };
    int32_t offsets[] = { 3600 * 1000 * 8, -3600 * 1000 * 5 };
    string countries[] = { "CN", "US" };
    int count = 2;
    ZoneUtil util;
    for (int i = 0; i < count; ++i) {
        string out = util.GetDefaultZone(countries[i].c_str(), offsets[i]);
        EXPECT_EQ(out, expects[i]);
    }
}

/**
 * @tc.name: ZoneUtilFuncTest003
 * @tc.desc: Test ZoneUtil GetDefaultZoneList for CN
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest,  ZoneUtilFuncTest003, TestSize.Level1)
{
    vector<string> expects = { "Asia/Shanghai", "Asia/Urumqi"};
    string country = "CN";
    vector<string> out;
    ZoneUtil util;
    util.GetZoneList(country, out);
    EXPECT_EQ(expects.size(), out.size());
    if (expects.size() == out.size()) {
        for (decltype(expects.size()) i = 0; i < expects.size(); ++i) {
            EXPECT_EQ(expects[i], out[i]);
        }
    }
}

/**
 * @tc.name: ZoneUtilFuncTest004
 * @tc.desc: Test ZoneUtil GetDefaultZoneList for GB
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest,  ZoneUtilFuncTest004, TestSize.Level1)
{
    vector<string> expects = { "Europe/London" };
    string country = "GB";
    vector<string> out;
    ZoneUtil util;
    util.GetZoneList(country, out);
    EXPECT_EQ(expects.size(), out.size());
    if (expects.size() == out.size()) {
        for (decltype(expects.size()) i = 0; i < expects.size(); ++i) {
            EXPECT_EQ(expects[i], out[i]);
        }
    }
}

/**
 * @tc.name: ZoneUtilFuncTest003
 * @tc.desc: Test ZoneUtil GetDefaultZoneList for de
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest,  ZoneUtilFuncTest005, TestSize.Level1)
{
    vector<string> expects = { "Europe/Berlin", "Europe/Busingen" };
    string country = "DE";
    vector<string> out;
    ZoneUtil util;
    util.GetZoneList(country, out);
    EXPECT_EQ(expects.size(), out.size());
    if (expects.size() == out.size()) {
        for (decltype(expects.size()) i = 0; i < expects.size(); ++i) {
            EXPECT_EQ(expects[i], out[i]);
        }
    }
}

/**
 * @tc.name: ZoneUtilFuncTest006
 * @tc.desc: Test ZoneUtil GetDefaultZoneList for CN with offset 8 hours
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest,  ZoneUtilFuncTest006, TestSize.Level1)
{
    vector<string> expects = { "Asia/Shanghai" };
    string country = "CN";
    vector<string> out;
    ZoneUtil util;
    util.GetZoneList(country, 3600 * 1000 * 8, out);
    EXPECT_EQ(expects.size(), out.size());
    if (expects.size() == out.size()) {
        for (decltype(expects.size()) i = 0; i < expects.size(); ++i) {
            EXPECT_EQ(expects[i], out[i]);
        }
    }
}

/**
 * @tc.name: ZoneUtilFuncTest007
 * @tc.desc: Test ZoneUtil GetDefaultZoneList for 86 with offset 8 hours
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest, ZoneUtilFuncTest007, TestSize.Level1)
{
    vector<string> expects = { "Asia/Shanghai" };
    int32_t number = 86;
    ZoneUtil util;
    string out = util.GetDefaultZone(number);
    EXPECT_EQ(expects[0], out);
}

/**
 * @tc.name: ZoneUtilFuncTest008
 * @tc.desc: Test ZoneUtil GetDefaultZoneList for 86 with offset 8 hours
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest, ZoneUtilFuncTest008, TestSize.Level1)
{
    vector<string> expects = { "Asia/Shanghai" };
    int32_t number = 86;
    ZoneUtil util;
    string out = util.GetDefaultZone(number, 8 * 3600 * 1000);
    EXPECT_EQ(expects[0], out);
}

/**
 * @tc.name: ZoneUtilFuncTest009
 * @tc.desc: Test ZoneUtil LookupTimezoneByCountryAndNITZ
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest, ZoneUtilFuncTest009, TestSize.Level1)
{
    ZoneUtil util;
    std::string region = "CN";
    NITZData nitzData = { 0, 28800000 , 1679969021999 };
    CountryResult result = util.LookupTimezoneByCountryAndNITZ(region, nitzData);
    EXPECT_TRUE(result.isOnlyMatch);
    EXPECT_EQ(result.timezoneId, "Asia/Shanghai");

    NITZData nitzData2 = { 0, 21600000, 1679969021999 };
    CountryResult result2 = util.LookupTimezoneByCountryAndNITZ(region, nitzData2);
    EXPECT_TRUE(result2.isOnlyMatch);
    EXPECT_EQ(result2.timezoneId, "Asia/Urumqi");
}

/**
 * @tc.name: ZoneUtilFuncTest010
 * @tc.desc: Test ZoneUtil LookupTimezoneByNITZ
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest, ZoneUtilFuncTest010, TestSize.Level1)
{
    ZoneUtil util;
    NITZData nitzData = { 0, 28800000 , 1679969021999 };
    CountryResult result = util.LookupTimezoneByNITZ(nitzData);
    EXPECT_FALSE(result.isOnlyMatch);
    EXPECT_EQ(result.timezoneId, "Asia/Shanghai");

    NITZData nitzData2 = { 1, 10800000, 1679969021999 };
    CountryResult result2 = util.LookupTimezoneByNITZ(nitzData2);
    EXPECT_FALSE(result2.isOnlyMatch);
    EXPECT_EQ(result2.timezoneId, "Asia/Beirut");

    NITZData nitzData3 = { 1, 37800000, 1679969021999 };
    CountryResult result3 = util.LookupTimezoneByNITZ(nitzData3);
    EXPECT_FALSE(result3.isOnlyMatch);
    EXPECT_EQ(result3.timezoneId, "Australia/Adelaide");
}

/**
 * @tc.name: ZoneUtilFuncTest011
 * @tc.desc: Test ZoneUtil LookupTimezoneByCountry
 * @tc.type: FUNC
 */
HWTEST_F(ZoneUtilTest, ZoneUtilFuncTest011, TestSize.Level1)
{
    ZoneUtil util;
    std::string region = "AQ";
    int64_t currentMillis = 1679969021999;
    CountryResult result = util.LookupTimezoneByCountry(region, currentMillis);
    EXPECT_EQ(result.timezoneId, "Antarctica/McMurdo");
    EXPECT_EQ(result.quality, MatchQuality::MULTIPLE_ZONES_DIFFERENT_OFFSET);
}
} // namespace I18n
} // namespace Global
} // namespace OHOS