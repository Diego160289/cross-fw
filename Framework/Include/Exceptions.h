#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>

#define DECLARE_EXCEPTION_EX(prefix_class_name_, base_class_) \
	class prefix_class_name_##Exception \
		: public base_class_ \
	{ \
	public: \
		prefix_class_name_##Exception(const std::string &msg) \
			: base_class_(msg) \
		{ \
		} \
	};

#define DECLARE_RUNTIME_EXCEPTION(prefix_class_name_) \
	DECLARE_EXCEPTION_EX(prefix_class_name_, std::runtime_error)

#endif	// !__EXCEPTIONS_H__
