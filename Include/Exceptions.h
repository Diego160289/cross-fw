//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>


#define DECLARE_EXCEPTION_EX(prefix_class_name_, base_class_) \
	class prefix_class_name_##Exception \
		: public base_class_ \
	{ \
	public: \
		prefix_class_name_##Exception(const std::string &msg, unsigned long code = 0) \
			: base_class_(msg) \
      , Code(code) \
		{ \
		} \
    virtual unsigned long GetCode() const \
    { \
      return Code; \
    } \
  private: \
    unsigned long Code; \
	};

#define DECLARE_RUNTIME_EXCEPTION(prefix_class_name_) \
	DECLARE_EXCEPTION_EX(prefix_class_name_, std::runtime_error)

#define DECLARE_LOGIC_EXCEPTION(prefix_class_name_) \
  DECLARE_EXCEPTION_EX(prefix_class_name_, std::logic_error)

#endif	// !__EXCEPTIONS_H__
