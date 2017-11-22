
#pragma once

#include <array>
#include <type_traits>
#include <common/float.h>

#if DATA_ENUM_DEBUGGING == 1
#include <stdexcept>
#define DATA_ENUM_ERROR(condition) \
	if (condition) { \
		throw ::std::exception("enum ordinal out of range"); \
	}
#elif DATA_ENUM_DEBUGGING == 2
#include <cassert>
#define DATA_ENUM_ERROR(condition) assert(!(condition));
#else
#define DATA_ENUM_ERROR(condition)
#endif

namespace util {

template <class EnumDefinition>
class DataEnum : public EnumDefinition
{
public:
	using enum_type       = typename EnumDefinition::enum_type;
	using ordinal_type    = typename EnumDefinition::ordinal_type;
	using underlying_type = typename EnumDefinition::underlying_type;
	using string_type     = typename EnumDefinition::string_type;
	using meta_type       = typename EnumDefinition::meta_type;
	using enum_array      = typename EnumDefinition::enum_array;
	using string_array    = typename EnumDefinition::string_array;
	using meta_array      = typename EnumDefinition::meta_array;

private:
	template <class Type, class Return>
	using TEnableIfFloating = typename std::enable_if<std::is_floating_point<Type>::value, Return>::type;
	template <class Type, class Return>
	using TEnableIfIntegral = typename std::enable_if<std::is_integral<Type>::value, Return>::type;

	enum_type m_enum;

public:
	constexpr DataEnum() : m_enum(m_enums[0]) {}
	constexpr DataEnum(enum_type value) : m_enum(value) {}
	constexpr DataEnum(const DataEnum& other) : DataEnum(other.m_enum) {}

	constexpr static bool is_incremental() noexcept {
		ordinal_type i = 0;
		for (const enum_type val : m_enums)
			if (i++ != static_cast<ordinal_type>(val))
				return false;
		return true;
	}

	constexpr static const enum_type* begin() noexcept {
		return &m_enums[0];
	}

	constexpr static const enum_type* end() noexcept {
		return &m_enums[0] + m_enums.size();
	}

	constexpr static ordinal_type count() noexcept {
		return m_enums.size();
	}

	constexpr static DataEnum get_by_ordinal(const ordinal_type ordinal) {
		DATA_ENUM_ERROR(ordinal >= count())
		return DataEnum(m_enums[ordinal]);
	}

	constexpr static DataEnum get_by_value(const underlying_type value) {
		DataEnum instance(static_cast<enum_type>(value));
		DATA_ENUM_ERROR(instance.ordinal() >= instance.count())
		return instance;
	}

	constexpr static const enum_array& enums() noexcept {
		return m_enums;
	}

	constexpr static const string_array& names() noexcept {
		return m_names;
	}

	constexpr static const meta_array& metas() noexcept {
		return m_metas;
	}

	static ordinal_type ordinal_of(const DataEnum& instance) noexcept {
		return instance.ordinal();
	}

	static underlying_type value_of(const DataEnum& instance) noexcept {
		return instance.value();
	}

	static string_type name_of(const DataEnum& instance) noexcept {
		return instance.name();
	}

	static const meta_type& meta_of(const DataEnum& instance) noexcept {
		return instance.meta();
	}

	template <class Type = underlying_type>
	constexpr TEnableIfIntegral<Type, bool> equals(enum_type value) const {
		return m_enum == value;
	}

	template <class Type = underlying_type>
	constexpr TEnableIfFloating<Type, bool> equals(enum_type value) const {
		return ::util::AlmostEqual(m_enum, value);
	}

	constexpr ordinal_type ordinal() const {
		for (ordinal_type i = 0, c = count(); i < c; ++i)
			if (equals(m_enums[i]))
				return i;
		DATA_ENUM_ERROR(true)
		return ~static_cast<ordinal_type>(0);
	}

	constexpr underlying_type value() const noexcept {
		return static_cast<underlying_type>(m_enum);
	}

	constexpr string_type name() const {
		return m_names[ordinal()];
	}

	constexpr const meta_type& meta() const {
		return m_metas[ordinal()];
	}

	constexpr bool is_valid() const noexcept {
		for (const enum_type val : m_enums)
			if (m_enum == val)
				return true;
		return false;
	}

	constexpr operator enum_type() const {
		return m_enum;
	}
};

template <class Underlying, class Meta>
class DataEnumBase
{
protected:
	using ordinal_type    = size_t;
	using string_type     = const char* const;
	using underlying_type = Underlying;
	using meta_type       = Meta;
};

} // namespace util

#define DATA_ENUM_UNROLL_MEMBERS(enumName, enumValue, ...)  constexpr static enum_type enumName = enum_type::enumName;
#define DATA_ENUM_UNROLL_MEMBERSF(enumName, enumValue, ...) constexpr static enum_type enumName = enumValue;
#define DATA_ENUM_UNROLL_VALUES(enumName, enumValue, ...)  enum_type::enumName,
#define DATA_ENUM_UNROLL_VALUESF(enumName, enumValue, ...) enumValue,
#define DATA_ENUM_UNROLL_NAMES(enumName, enumValue, ...) #enumName,
#define DATA_ENUM_UNROLL_VALUE(enumName, enumValue, ...) enumName enumValue,
#define DATA_ENUM_UNROLL_META(enumName, enumValue, meta, ...) meta,
#define DATA_ENUM_UNROLL_COUNT(...) + 1
#define DATA_ENUM_SIZE(list) 0 list(DATA_ENUM_UNROLL_COUNT)

#define DEFINE_REGULAR_ENUM_CLASS(rawName, underlying, list) \
enum class rawName : underlying { list(DATA_ENUM_UNROLL_VALUE) };

#define DEFINE_REGULAR_FLOAT(rawName, underlying) \
typedef underlying rawName;


#define DEFINE_DATA_ENUM_CLASS(rawName, dataName, underlying, list, meta) \
enum class rawName : underlying { list(DATA_ENUM_UNROLL_VALUE) }; \
class dataName##Definition : public ::util::DataEnumBase<underlying, meta> { \
protected: \
	using enum_type    = rawName; \
	using enum_array   = ::std::array<enum_type,   DATA_ENUM_SIZE(list)>; \
	using string_array = ::std::array<string_type, DATA_ENUM_SIZE(list)>; \
	using meta_array   = ::std::array<meta_type,   DATA_ENUM_SIZE(list)>; \
	constexpr static enum_array   m_enums = { list(DATA_ENUM_UNROLL_VALUES) }; \
	constexpr static string_array m_names = { list(DATA_ENUM_UNROLL_NAMES)  }; \
	constexpr static meta_array   m_metas = { list(DATA_ENUM_UNROLL_META)   }; \
public: \
	list(DATA_ENUM_UNROLL_MEMBERS); \
}; \
typedef ::util::DataEnum<dataName##Definition> dataName; \


#define DEFINE_DATA_FLOAT(rawName, dataName, underlying, list, meta) \
typedef underlying rawName; \
class dataName##Definition : public ::util::DataEnumBase<underlying, meta> { \
protected: \
    using enum_type    = rawName; \
	using enum_array   = ::std::array<enum_type,   DATA_ENUM_SIZE(list)>; \
	using string_array = ::std::array<string_type, DATA_ENUM_SIZE(list)>; \
	using meta_array   = ::std::array<meta_type,   DATA_ENUM_SIZE(list)>; \
	constexpr static enum_array   m_enums = { list(DATA_ENUM_UNROLL_VALUESF) }; \
	constexpr static string_array m_names = { list(DATA_ENUM_UNROLL_NAMES)   }; \
	constexpr static meta_array   m_metas = { list(DATA_ENUM_UNROLL_META)    }; \
public: \
	list(DATA_ENUM_UNROLL_MEMBERSF); \
}; \
typedef ::util::DataEnum<dataName##Definition> dataName; \


#ifdef DATA_ENUM_SAMPLE
#pragma warning(push)
#pragma warning(disable:4189)

// This is an enum class macro for enums with ordinal and count awareness.
// Additionally it can store meta data as defined in the macro.

enum class FruitColor { Red, Orange, Yellow, Brown };

struct FruitMeta
{
	constexpr FruitMeta(const char* const description, const FruitColor color)
		: description(description)
		, color(color)
	{}
	const char* const description;
	const FruitColor color;
};

#define FRUIT_ENUM_LIST(e) \
	e( Apple     , = 44 , (FruitMeta("Apples are rare"     , FruitColor::Red   )) ) \
	e( Orange    , = 55 , (FruitMeta("Oranges are orange"  , FruitColor::Orange)) ) \
	e( Banana    , = 66 , (FruitMeta("Bananas are long"    , FruitColor::Yellow)) ) \
	e( Pineapple ,      , (FruitMeta("Pineapples are tasty", FruitColor::Brown )) ) \

DEFINE_DATA_ENUM_CLASS(Fruit, FruitData, int, FRUIT_ENUM_LIST, FruitMeta)

inline void CustomEnumExample()
{
	auto orangeOrdinal = FruitData::ordinal_of(FruitData::Orange); // 1
	auto orangeValue   = FruitData::value_of(FruitData::Orange); // 55
	auto orangeName    = FruitData::name_of(FruitData::Orange); // "Orange"
	auto orangeMeta    = FruitData::meta_of(FruitData::Orange); // FruitMeta

	const FruitData::enum_array& values = FruitData::enums();
	const FruitData::string_array& names = FruitData::names();
	const FruitData::meta_array& metas = FruitData::metas();

	// FruitData illegalFruit = 0; // compile error
	// Fruit illegalFruit = FruitData::get_by_ordinal(5); // illegal, exception

	FruitData   fruit            = Fruit::Banana; // fruit is Banana
	size_t      fruitOrdinal     = fruit.ordinal(); // 2
	int         fruitValue       = fruit.value(); // 66
	const char* fruitName        = fruit.name(); // "Banana"
	auto        fruitdescription = fruit.meta().description; // "Bananas are long"
	auto        fruitcolor       = fruit.meta().color; // FruitColor::Yellow

	FruitData fruit2(FruitData::Apple);

	bool same = (fruit == fruit2); // false

	int valueOf = FruitData::value_of(FruitData::Pineapple); // 67
	size_t count = FruitData::count(); // 4

	for (size_t ordinal = 0; ordinal < count; ++ordinal)
	{
		Fruit fruitByOrdinal = FruitData::get_by_ordinal(ordinal);
	}

	for (Fruit fruit3 : FruitData::enums())
	{
		Fruit fruitFromRangeLoop = fruit3;
	}
}

// The Meta data cannot be omitted.
// If there is no meta data required, just specify a meaningless literal type like so:

#define DUMMY_ENUM_LIST(e) \
	e(Enum1 , , false) \
	e(Enum2 , , false) \
	e(Enum3 , , false) \
	e(Enum4 , , false) \

DEFINE_DATA_ENUM_CLASS(DummyEnum, DummyEnumData, int, DUMMY_ENUM_LIST, bool)

#define DUMMY_FLOAT_LIST(e) \
	e(Enum1 , 1.1f , false) \
	e(Enum2 , 2.2f , false) \
	e(Enum3 , 3.3f , false) \
	e(Enum4 , 4.4f , false) \

DEFINE_DATA_FLOAT(DummyFloat, DummyFloatData, float, DUMMY_FLOAT_LIST, bool)

#pragma warning(pop)
#endif
