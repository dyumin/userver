#include <gtest/gtest.h>

#include <chrono>
#include <optional>

#include <optional>

#include <formats/common/type.hpp>
#include <formats/parse/common_containers.hpp>

template <class T>
struct Conversion : public ::testing::Test {};
TYPED_TEST_CASE_P(Conversion);

TYPED_TEST_P(Conversion, Missing) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;

  ValueBuilder vb;
  vb["a"] = ValueBuilder(formats::common::Type::kArray);
  vb["b"] = ValueBuilder(formats::common::Type::kObject);

  const Value value = vb.ExtractValue();
  for (const auto& elem : {value["b"]["c"], value["d"]}) {
    EXPECT_FALSE(elem.template ConvertTo<bool>());
    EXPECT_EQ(0, elem.template ConvertTo<int32_t>());
    EXPECT_EQ(0, elem.template ConvertTo<int64_t>());
    EXPECT_EQ(0, elem.template ConvertTo<size_t>());
    EXPECT_DOUBLE_EQ(0.0, elem.template ConvertTo<double>());
    EXPECT_TRUE(elem.template ConvertTo<std::string>().empty());

    EXPECT_EQ(true, elem.template ConvertTo<bool>(true));
    EXPECT_EQ(1, elem.template ConvertTo<int>(1));
    EXPECT_EQ("test", elem.template ConvertTo<std::string>("test"));
    EXPECT_EQ("test", elem.template ConvertTo<std::string>("test123", 4));
  }
}

TYPED_TEST_P(Conversion, Null) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;

  const Value elem = ValueBuilder(formats::common::Type::kNull).ExtractValue();

  EXPECT_FALSE(elem.template ConvertTo<bool>());
  EXPECT_EQ(0, elem.template ConvertTo<int32_t>());
  EXPECT_EQ(0, elem.template ConvertTo<int64_t>());
  EXPECT_EQ(0, elem.template ConvertTo<size_t>());
  EXPECT_DOUBLE_EQ(0.0, elem.template ConvertTo<double>());
  EXPECT_TRUE(elem.template ConvertTo<std::string>().empty());
}

TYPED_TEST_P(Conversion, Bool) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;

  ValueBuilder vb;
  vb["a"].PushBack(true);
  vb["a"].PushBack(false);
  vb["et"] = true;
  vb["ef"] = false;
  const Value value = vb.ExtractValue();

  bool ethalon = false;
  for (const auto& elem :
       {value["a"][0], value["a"][1], value["et"], value["ef"]}) {
    ethalon = !ethalon;

    EXPECT_EQ(ethalon, elem.template ConvertTo<bool>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<int32_t>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<int64_t>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<size_t>());
    EXPECT_DOUBLE_EQ(ethalon, elem.template ConvertTo<double>());
    if (ethalon) {
      EXPECT_EQ("true", elem.template ConvertTo<std::string>());
    } else {
      EXPECT_EQ("false", elem.template ConvertTo<std::string>());
    }
  }
}

TYPED_TEST_P(Conversion, Double) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;

  ValueBuilder vb;
  vb["a"].PushBack(0.0);
  vb["a"].PushBack(0.123);
  vb["a"].PushBack(-0.123);
  vb["ez"] = 0.0;
  vb["en"] = -3.14;
  vb["ep"] = 3.14;
  const Value value = vb.ExtractValue();

  for (const auto& [elem, ethalon] : {
           std::pair{value["a"][0], 0.0},
           std::pair{value["a"][1], 0.123},
           std::pair{value["a"][2], -0.123},
           std::pair{value["ez"], 0.0},
           std::pair{value["en"], -3.14},
           std::pair{value["ep"], 3.14},
       }) {
    EXPECT_EQ(!!ethalon, elem.template ConvertTo<bool>());
    EXPECT_EQ(static_cast<int32_t>(ethalon),
              elem.template ConvertTo<int32_t>());
    EXPECT_EQ(static_cast<int64_t>(ethalon),
              elem.template ConvertTo<int64_t>());
    if (ethalon > -1.0) {
      EXPECT_EQ(static_cast<size_t>(ethalon),
                elem.template ConvertTo<size_t>());
    } else {
      EXPECT_ANY_THROW(elem.template ConvertTo<size_t>())
          << "ethalon=" << ethalon;
    }
    EXPECT_DOUBLE_EQ(ethalon, elem.template ConvertTo<double>());
    EXPECT_EQ(std::to_string(ethalon), elem.template ConvertTo<std::string>());
  }
}

TYPED_TEST_P(Conversion, Int32) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;

  ValueBuilder vb;
  vb["a"].PushBack(0);
  vb["a"].PushBack(123);
  vb["a"].PushBack(-123);
  vb["ez"] = 0;
  vb["en"] = -314;
  vb["ep"] = 314;
  const Value value = vb.ExtractValue();

  for (const auto& [elem, ethalon] : {
           std::pair{value["a"][0], 0},
           std::pair{value["a"][1], 123},
           std::pair{value["a"][2], -123},
           std::pair{value["ez"], 0},
           std::pair{value["en"], -314},
           std::pair{value["ep"], 314},
       }) {
    EXPECT_EQ(!!ethalon, elem.template ConvertTo<bool>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<int32_t>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<int64_t>());
    if (ethalon >= 0) {
      EXPECT_EQ(ethalon, elem.template ConvertTo<size_t>());
    } else {
      EXPECT_ANY_THROW(elem.template ConvertTo<size_t>())
          << "ethalon=" << ethalon;
      ;
    }
    EXPECT_DOUBLE_EQ(ethalon, elem.template ConvertTo<double>());
    EXPECT_EQ(std::to_string(ethalon), elem.template ConvertTo<std::string>());
  }
}

TYPED_TEST_P(Conversion, Int64) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;

  ValueBuilder vb;
  vb["a"].PushBack(int64_t{0});
  vb["a"].PushBack(int64_t{123});
  vb["a"].PushBack(int64_t{-123});
  vb["ez"] = int64_t{0};
  vb["en"] = int64_t{-314};
  vb["ep"] = int64_t{314};
  const Value value = vb.ExtractValue();

  for (const auto& [elem, ethalon] : {
           std::pair{value["a"][0], 0},
           std::pair{value["a"][1], 123},
           std::pair{value["a"][2], -123},
           std::pair{value["ez"], 0},
           std::pair{value["en"], -314},
           std::pair{value["ep"], 314},
       }) {
    EXPECT_EQ(!!ethalon, elem.template ConvertTo<bool>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<int32_t>());
    EXPECT_EQ(ethalon, elem.template ConvertTo<int64_t>());
    if (ethalon >= 0) {
      EXPECT_EQ(ethalon, elem.template ConvertTo<size_t>());
    } else {
      EXPECT_ANY_THROW(elem.template ConvertTo<size_t>())
          << "ethalon=" << ethalon;
      ;
    }
    EXPECT_DOUBLE_EQ(ethalon, elem.template ConvertTo<double>());
    EXPECT_EQ(std::to_string(ethalon), elem.template ConvertTo<std::string>());
  }
}

TYPED_TEST_P(Conversion, Utf8) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;
  using Exception = typename TestFixture::Exception;

  ValueBuilder vb;
  vb["a"] = "\377\377";
  vb["b"] = "0";
  vb["c"] = "10";
  vb["d"] = "-10";
  const Value value = vb.ExtractValue();
  for (const auto& [elem, ethalon] : {
           std::pair{value["a"], std::string{"\377\377"}},
           std::pair{value["b"], std::string{"0"}},
           std::pair{value["c"], std::string{"10"}},
           std::pair{value["d"], std::string{"-10"}},
       }) {
    EXPECT_TRUE(elem.template ConvertTo<bool>()) << "ethalon=" << ethalon;
    EXPECT_THROW(elem.template ConvertTo<int32_t>(), Exception)
        << "ethalon=" << ethalon;
    EXPECT_THROW(elem.template ConvertTo<int64_t>(), Exception)
        << "ethalon=" << ethalon;
    EXPECT_THROW(elem.template ConvertTo<size_t>(), Exception)
        << "ethalon=" << ethalon;
    EXPECT_THROW(elem.template ConvertTo<double>(), Exception)
        << "ethalon=" << ethalon;
    EXPECT_EQ(ethalon, elem.template ConvertTo<std::string>())
        << "ethalon=" << ethalon;
  }
}

TYPED_TEST_P(Conversion, Containers) {
  using ValueBuilder = typename TestFixture::ValueBuilder;
  using Value = typename TestFixture::Value;
  using Exception = typename TestFixture::Exception;

  ValueBuilder vb;
  vb["a"].PushBack(0.0);
  vb["a"].PushBack(1);
  vb["a"].PushBack(2);

  vb["d"]["one"] = 1.0;
  vb["d"]["two"] = 2;

  vb["n"] = ValueBuilder(formats::common::Type::kNull);
  const Value value = vb.ExtractValue();

  EXPECT_THROW(
      (value["a"].template ConvertTo<std::unordered_map<std::string, int>>()),
      Exception);
  EXPECT_THROW(value["d"].template ConvertTo<std::vector<int>>(), Exception);

  auto arr = value["a"].template ConvertTo<std::vector<int>>();
  for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
    EXPECT_EQ(i, arr[i]) << "mismatch at position " << i;
  }

  auto umap =
      value["d"].template ConvertTo<std::unordered_map<std::string, int>>();
  EXPECT_EQ(1, umap["one"]);
  EXPECT_EQ(2, umap["two"]);

  EXPECT_TRUE((value["n"]
                   .template ConvertTo<std::unordered_map<std::string, int>>()
                   .empty()));
  EXPECT_TRUE(value["n"].template ConvertTo<std::vector<int>>().empty());
  EXPECT_FALSE(value["n"].template ConvertTo<boost::optional<std::string>>());
  EXPECT_FALSE(value["n"].template ConvertTo<std::optional<std::string>>());
}

REGISTER_TYPED_TEST_CASE_P(Conversion,

                           Missing, Null, Bool, Double, Int32, Int64, Utf8,
                           Containers);
