#ifndef __TYPELIST_H__
#define __TYPELIST_H__


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

  template <typename TList, unsigned index>
  struct TypeListItem
  {
    typedef typename TypeListItem<typename TList::Tail, index - 1>::Item Item;
  };

  template <typename TList>
  struct TypeListItem<TList, 0>
  {
    typedef typename TList::Head Item;
  };

  template <typename TList>
  struct TypeListLength
  {
    enum
    {
      Len = TypeListLength<typename TList::Tail>::Len + 1
    };
  };

  template <>
  struct TypeListLength<NullType>
  {
    enum
    {
      Len = 0
    };
  };

  template
    <
      typename TFirst,
      typename TSecond,
      bool First
    >
  struct SelectType
  {
    typedef TFirst Type;
  };

  template
  <
    typename TFirst,
    typename TSecond
  >
  struct SelectType<TFirst, TSecond, false>
  {
    typedef TSecond Type;
  };


  template <typename T1, typename T2>
  struct IsEqualTypes
  {
    enum { IsEqual = 0 };
  };

  template <typename T1>
  struct IsEqualTypes<T1, T1>
  {
    enum { IsEqual = 1 };
  };

  template <typename TList, typename THead>
  struct CutNullTail
  {
    typedef typename
      SelectType
        <
          TypeList
            <
              typename TList::Head,
              typename CutNullTail
                <
                  typename TList::Tail,
                  typename TList::Head
                >::TNewList
            >,
          NullType,
          !IsEqualTypes<typename TList::Head, NullType>::IsEqual
        >::Type TNewList;
  };

  template <typename TList>
  struct CutNullTail<TList, NullType>
  {
    typedef NullType TNewList;
  };


  template
  <
    typename T1,
    typename T2 = NullType,
    typename T3 = NullType,
    typename T4 = NullType,
    typename T5 = NullType,
    typename T6 = NullType,
    typename T7 = NullType,
    typename T8 = NullType,
    typename T9 = NullType,
    typename T10 = NullType
  >
  class TypeListAdapter
  {
    typedef TYPE_LIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) TListRaw;
    typedef typename CutNullTail<TListRaw, typename TListRaw::Head>::TNewList TList;
  public:
    typedef typename TList::Head Head;
    typedef typename TList::Tail Tail;
  };

}

#endif	// !__TYPELIST_H__
