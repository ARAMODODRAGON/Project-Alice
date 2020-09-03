#ifndef _GENERAL_MACROS_HPP
#define _GENERAL_MACROS_HPP

#define SINGLETON(TYPE)					\
private:								\
TYPE(const TYPE&) = delete; 			\
TYPE& operator=(const TYPE&) = delete;	\
TYPE(TYPE&&) = delete; 					\
TYPE& operator=(TYPE&&) = delete;		\
public:									\
static TYPE* Get() {					\
	static TYPE _instance;				\
	return &_instance;					\
}										\
private:

#define NON_CONSTRUCTABLE(TYPE)				\
TYPE() = delete;							\
TYPE(const TYPE&) = delete;					\
TYPE& operator=(const TYPE&) = delete;		\
TYPE(TYPE&&) = delete;						\
TYPE& operator=(TYPE&&) = delete;

// this only works if the enum has a __SIZE__ value in it
#define ENUM_SIZE(Enum) static_cast<size_t>(Enum::__SIZE__)

#endif // !_GENERAL_MACROS_HPP