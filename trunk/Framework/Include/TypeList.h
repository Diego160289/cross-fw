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

  template <typename TList>
  class InheritedFromList
    : public TList::Head
    , public InheritedFromList<typename TList::Tail>
  {
  public:
    void CastTo(const char *ifaceId, void **iface)
    {
      typedef typename TList::Head CurBase;
      const char *CurId = CurBase::GetUUID();
      const char *DestId = ifaceId;
      while (*DestId && *CurId && *DestId++ == *CurId++);
      if (*DestId || *CurId)
      {
        InheritedFromList<typename TList::Tail>::CastTo(ifaceId, iface);
        return;
      }
      *iface = static_cast<CurBase*>(this);
    }
  };

  template <>
  class InheritedFromList<NullType>
  {
  public:
    void CastTo(const char *, void **iface)
    {
      *iface = 0;
    }
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
