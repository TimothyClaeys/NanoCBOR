#include <gtest/gtest.h>
#include <nanocbor/nanocbor.h>

using namespace std;

#define MAX_ARRAY_SIZE        (8)


TEST(DecoderInit, SetupDecoder){
    nanocbor_value_t val;
    const uint8_t test_vector[] = {0x3A, 0x00, 0x0D, 0x6B, 0xB9};

    nanocbor_decoder_init(&val, (uint8_t *) &test_vector, (size_t) sizeof(test_vector));

    EXPECT_EQ(val.cur, test_vector);
    EXPECT_EQ(val.end, test_vector + sizeof(test_vector));
    EXPECT_EQ(val.flags, 0);
}

class CBORType
        : public ::testing::TestWithParam<tuple<array<uint8_t, MAX_ARRAY_SIZE>, int, int>> {
};

TEST_P(CBORType, CheckCBORType){
    nanocbor_value_t val;

    auto test_vector = get<0>(GetParam());
    auto length = get<1>(GetParam());
    auto type = get<2>(GetParam());

    nanocbor_decoder_init(&val, (uint8_t *) &test_vector, (size_t) length);

    EXPECT_EQ(nanocbor_get_type(&val), type);
}

INSTANTIATE_TEST_CASE_P(ParametrizedCBORType,
                        CBORType,
                        testing::Values(
                                make_tuple(array<uint8_t, MAX_ARRAY_SIZE>{0x05}, 1, NANOCBOR_TYPE_UINT),
                                make_tuple(array<uint8_t, MAX_ARRAY_SIZE>{0x39, 0x22, 0x5A}, 3, NANOCBOR_TYPE_NINT),
                                make_tuple(array<uint8_t, MAX_ARRAY_SIZE>{0x42, 0x20, 0x5a}, 3, NANOCBOR_TYPE_BSTR),
                                make_tuple(array<uint8_t, MAX_ARRAY_SIZE>{0x62, 0x68, 0x69}, 3, NANOCBOR_TYPE_TSTR),
                                make_tuple(array<uint8_t, MAX_ARRAY_SIZE>{0x82, 0x01, 0x02}, 3, NANOCBOR_TYPE_ARR),
                                make_tuple(array<uint8_t, MAX_ARRAY_SIZE>{0xA1, 0x01, 0x40}, 3, NANOCBOR_TYPE_MAP)
                                ));

class IndefiniteArrayDecoder
        : public ::testing::TestWithParam<tuple<array<uint8_t, MAX_ARRAY_SIZE>, int, array<int, MAX_ARRAY_SIZE>>> {
};

TEST_P(IndefiniteArrayDecoder, ArrayElements) {
    nanocbor_value_t val;
    nanocbor_value_t cont;
    uint32_t tmp = 0;

    auto test_vector = get<0>(GetParam());
    auto length = get<1>(GetParam());
    auto contents = get<2>(GetParam());

    nanocbor_decoder_init(&val, (uint8_t *) &test_vector, (size_t) length);

    EXPECT_EQ(nanocbor_enter_array(&val, &cont), NANOCBOR_OK);
    EXPECT_EQ(nanocbor_container_indefinite(&cont), true);

    for (int i = 2; i < length; i++) {
        EXPECT_GT(nanocbor_get_uint32(&cont, &tmp), 0);
        EXPECT_EQ(tmp, contents[i - 2]);
    }
}

INSTANTIATE_TEST_SUITE_P(ParametrizedIndefiniteArrayDecoder,
                         IndefiniteArrayDecoder,
                         testing::Values(
                                 make_tuple(
                                         array<uint8_t, MAX_ARRAY_SIZE>{0x9f, 0xff},
                                         2,
                                         array<int, MAX_ARRAY_SIZE>{}),
                                 make_tuple(
                                         array<uint8_t, MAX_ARRAY_SIZE>{0x9f, 0x01, 0xff},
                                         3,
                                         array<int, MAX_ARRAY_SIZE>{1}),
                                 make_tuple(
                                         array<uint8_t, MAX_ARRAY_SIZE>{0x9f, 0x01, 0x02, 0x03, 0x04, 0xff},
                                         6,
                                         array<int, MAX_ARRAY_SIZE>{1, 2, 3, 4})));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}