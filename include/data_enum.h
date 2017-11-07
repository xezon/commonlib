
#pragma once

#if DATA_ENUM_DEBUGGING == 1
#include <stdexcept>
#define DATA_ENUM_ERROR(condition) \
	if (condition) { \
		throw std::exception("enum ordinal out of range"); \
	}
#elif DATA_ENUM_DEBUGGING == 2
#include <cassert>
#define DATA_ENUM_ERROR(condition) assert(!(condition));
#else
#define DATA_ENUM_ERROR(condition)
#endif

template <class EnumDefinition>
class CDataEnum : public EnumDefinition {
public:
	using enum_type       = typename EnumDefinition::enum_type;
	using ordinal_type    = typename EnumDefinition::ordinal_type;
	using underlying_type = typename EnumDefinition::underlying_type;
	using string_type     = typename EnumDefinition::string_type;
	using meta_type       = typename EnumDefinition::meta_type;

private:
	enum_type m_value;

public:
	constexpr CDataEnum() : m_value(m_values[0]) {}
	constexpr CDataEnum(enum_type value) : m_value(value) {}
	constexpr CDataEnum(const CDataEnum& other) : CDataEnum(other.m_value) {}
	constexpr static ordinal_type count() noexcept {
		return sizeof(m_values) / sizeof(enum_type);
	}
	constexpr static CDataEnum get_by_ordinal(const ordinal_type ordinal) {
		DATA_ENUM_ERROR(ordinal >= count())
		return CDataEnum(m_values[ordinal]);
	}
	constexpr static CDataEnum get_by_value(const underlying_type value) {
		CDataEnum instance(static_cast<enum_type>(value));
		DATA_ENUM_ERROR(instance.ordinal() >= instance.count())
		return instance;
	}
	static ordinal_type ordinal_of(const CDataEnum& instance) noexcept {
		return instance.ordinal();
	}
	static underlying_type value_of(const CDataEnum& instance) noexcept {
		return instance.value();
	}
	static string_type name_of(const CDataEnum& instance) noexcept {
		return instance.name();
	}
	static const meta_type& meta_of(const CDataEnum& instance) noexcept {
		return instance.meta();
	}
	constexpr ordinal_type ordinal() const {
		for (ordinal_type i = 0, c = count(); i < c; ++i)
			if (m_value == m_values[i])
				return i;
		DATA_ENUM_ERROR(true)
		return ~0u;
	}
	constexpr underlying_type value() const noexcept {
		return static_cast<underlying_type>(m_value);
	}
	constexpr string_type name() const {
		return m_names[ordinal()];
	}
	constexpr const meta_type& meta() const {
		return m_metas[ordinal()];
	}
	constexpr bool operator==(const CDataEnum& other) const noexcept {
		return m_value == other.m_value;
	}
	constexpr bool operator!=(const CDataEnum& other) const noexcept {
		return m_value != other.m_value;
	}
	constexpr bool operator<(const CDataEnum& other) const noexcept {
		return m_value < other.m_value;
	}
	constexpr bool operator>(const CDataEnum& other) const noexcept {
		return m_value > other.m_value;
	}
	constexpr bool operator<=(const CDataEnum& other) const noexcept {
		return m_value <= other.m_value;
	}
	constexpr bool operator>=(const CDataEnum& other) const noexcept {
		return m_value >= other.m_value;
	}
};

#define DATA_ENUM_UNROLL_MEMBERS(enumName, enumValue, ...) constexpr static enum_type enumName{enum_type::enumName};
#define DATA_ENUM_UNROLL_VALUES(enumName, enumValue, ...) enum_type::enumName,
#define DATA_ENUM_UNROLL_NAMES(enumName, enumValue, ...) #enumName,
#define DATA_ENUM_UNROLL_VALUE(enumName, enumValue, ...) enumName enumValue,
#define DATA_ENUM_UNROLL_META(enumName, enumValue, meta, ...) meta,

#define DEFINE_NORMAL_ENUM_CLASS(clazz, underlying_t, list) \
enum class clazz : underlying_t { \
	list(DATA_ENUM_UNROLL_VALUE) \
};

#define DEFINE_DATA_ENUM_CLASS(clazz, underlying_t, list, meta_t) \
class clazz##Definition { \
public: \
	using ordinal_type    = size_t;       \
	using underlying_type = underlying_t; \
	using string_type     = const char*;  \
	using meta_type       = meta_t;       \
protected: \
	enum class enum_type : underlying_type    { list(DATA_ENUM_UNROLL_VALUE)  }; \
	constexpr static enum_type   m_values[] = { list(DATA_ENUM_UNROLL_VALUES) }; \
	constexpr static string_type m_names[]  = { list(DATA_ENUM_UNROLL_NAMES)  }; \
	constexpr static meta_type   m_metas[]  = { list(DATA_ENUM_UNROLL_META)   }; \
public: \
	list(DATA_ENUM_UNROLL_MEMBERS); \
}; \
typedef CDataEnum<clazz##Definition> clazz;


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
	e( Apple     , = 44 , (FruitMeta("Apples are rare"         , FruitColor::Red   )) ) \
	e( Orange    , = 55 , (FruitMeta("Oranges are orange"      , FruitColor::Orange)) ) \
	e( Banana    , = 66 , (FruitMeta("Bananas are long"        , FruitColor::Yellow)) ) \
	e( Pineapple ,      , (FruitMeta("Pineapples are the tasty", FruitColor::Brown )) ) \

DEFINE_DATA_ENUM_CLASS(Fruit, int, FRUIT_ENUM_LIST, FruitMeta)

void CustomEnumExample()
{
	auto orangeOrdinal = Fruit::ordinal_of(Fruit::Orange); // 1
	auto orangeValue = Fruit::value_of(Fruit::Orange);     // 55
	auto orangeName = Fruit::name_of(Fruit::Orange);       // "Orange"
	auto orangeMeta = Fruit::meta_of(Fruit::Orange);       // FruitMeta

	// Fruit illegalFruit = 0;                             // compile error
	// Fruit illegalFruit = Fruit::get_by_ordinal(5);      // illegal, exception

	Fruit fruit = Fruit::Banana;                           // fruit is Banana
	size_t fruitOrdinal = fruit.ordinal();                 // 2
	int fruitValue = fruit.value();                        // 66
	const char* fruitName = fruit.name();                  // "Banana"
	auto description = fruit.meta().description;           // "Bananas are long"
	auto color = fruit.meta().color;                       // FruitColor::Yellow

	Fruit fruit2(Fruit::Apple);

	bool same = (fruit == fruit2);                         // false

	int valueOf = Fruit::value_of(Fruit::Pineapple);       // 67
	size_t count = Fruit::count();                         // 4

	for (size_t ordinal = 0; ordinal < count; ++ordinal)
	{
		Fruit fruitByOrdinal = Fruit::get_by_ordinal(ordinal);
	}
}

// The Meta data cannot be omitted.
// If there is no meta data required, just specify a meaningless literal type like so:

#define DUMMY_ENUM_LIST(e) \
	e(Enum1 , , false) \
	e(Enum2 , , false) \
	e(Enum3 , , false) \
	e(Enum4 , , false) \

DEFINE_DATA_ENUM_CLASS(DummyEnum, int, DUMMY_ENUM_LIST, bool)

#pragma warning(pop)
#endif
