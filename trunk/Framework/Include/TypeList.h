#ifndef __TYPELIST_H__
#define __TYPELIST_H__

namespace Common
{
	struct NullType
	{
	};

	template <typename H, typename T>
	struct TypeList
	{
		typedef H Head;
		typedef T Tail;
	};

	template <typename TList>
	struct TypeListLength
	{
		enum { Len = TypeListLength<typename TList::Tail>::Len + 1 };
	};

	template <>
	struct TypeListLength<NullType>
	{
		enum { Len = 0 };
	};

	template <typename TList>
	class InheritedFromList
		: public TList::Head
		, public InheritedFromList<typename TList::Tail>
	{
	};

	template <>
	class InheritedFromList<NullType>
	{
	};

}

#define TYPE_LIST_1(t1) \
	Common::TypeList<t1, Common::NullType>

#define TYPE_LIST_2(t1, t2) \
	Common::TypeList<t1, TYPE_LIST_1(t2)>

#define TYPE_LIST_3(t1, t2, t3) \
	Common::TypeList<t1, TYPE_LIST_2(t2, t3)>

#define TYPE_LIST_4(t1, t2, t3, t4) \
	Common::TypeList<t1, TYPE_LIST_3(t2, t3, t4)>

#define TYPE_LIST_5(t1, t2, t3, t4, t5) \
	Common::TypeList<t1, TYPE_LIST_4(t2, t3, t4, t5)>

#define TYPE_LIST_6(t1, t2, t3, t4, t5, t6) \
	Common::TypeList<t1, TYPE_LIST_5(t2, t3, t4, t5, t6)>

#define TYPE_LIST_7(t1, t2, t3, t4, t5, t6, t7) \
	Common::TypeList<t1, TYPE_LIST_6(t2, t3, t4, t5, t6, t7)>

#define TYPE_LIST_8(t1, t2, t3, t4, t5, t6, t7, t8) \
	Common::TypeList<t1, TYPE_LIST_7(t2, t3, t4, t5, t6, t7, t8)>

#define TYPE_LIST_9(t1, t2, t3, t4, t5, t6, t7, t8, t9) \
	Common::TypeList<t1, TYPE_LIST_8(t2, t3, t4, t5, t6, t7, t8, t9)>

#define TYPE_LIST_10(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10) \
	Common::TypeList<t1, TYPE_LIST_9(t2, t3, t4, t5, t6, t7, t8, t9, t10)>


#endif	// !__TYPELIST_H__
