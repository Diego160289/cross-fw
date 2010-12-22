//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __FSM_H__
#define __FSM_H__

#include "TypeList.h"
#include "Pointers.h"
#include "Exceptions.h"

#include <list>
#include <map>


namespace Common
{

  namespace FSM
  {

    template
    <
      typename TClass,
      typename TParam,
      void (TClass::*Method)(TParam)
    >
    struct Callback
    {
      typedef TClass ClassType;
      typedef TParam ParamType;
      static void Execute(TClass &object, TParam prm)
      {
        (object.*Method)(prm);
      }
    };

    template
    <
      unsigned Event,
      unsigned CurState,
      unsigned NextState,
      typename THandler
    >
    struct Transition
    {
      typedef THandler Handler;
      static unsigned GetEvent()
      {
        return Event;
      }
      static unsigned GetCurState()
      {
        return CurState;
      }
      static unsigned GetNextState()
      {
        return NextState;
      }
    };

    namespace PrivateNSFSM
    {

      template
      <
        typename TMachine,
        typename TParam,
        typename TTransitions
      >
      struct Executer
      {
        void ProcessEvent(TMachine &machine, unsigned event, TParam prm, unsigned *state)
        {
          typedef typename TTransitions::Head CurTransition;
          if (event == CurTransition::GetEvent() && (*state) == CurTransition::GetCurState())
          {
            typedef typename CurTransition::Handler Handler;
            Handler::Execute(dynamic_cast<typename Handler::ClassType &>(machine), prm);
            *state = CurTransition::GetNextState();
            return;
          }
          Executer<TMachine, TParam, typename TTransitions::Tail>().ProcessEvent(machine, event, prm, state);
        }
      };

      template
      <
        typename TMachine,
        typename TParam
      >
      struct Executer<TMachine, TParam, Common::NullType>
      {
        void ProcessEvent(TMachine &machine, unsigned event, TParam prm, unsigned *state)
        {
          throw typename TMachine::TFiniteStateMachineException("bab event for current state");
        }
      };

      template <typename TFirst, typename TSecond>
      struct Pair
      {
        Pair(TFirst first, TSecond second)
          : First(first)
          , Second(second)
        {
        }
        TFirst First;
        TSecond Second;
      };
    }

    DECLARE_LOGIC_EXCEPTION(FiniteStateMachine)

    template
    <
      typename TTransitionTable,
      typename TBaseClasses = NullType,
      typename TException = FiniteStateMachineException
    >
    class FiniteStateMachine
      : public Common::MultipleInheritance<TBaseClasses>
    {
      typedef typename TTransitionTable::Head FirstTransitionType;
      typedef FiniteStateMachine<TTransitionTable, TBaseClasses, TException> ThisType;
      typedef typename FirstTransitionType::Handler HandlerType;
      typedef typename HandlerType::ParamType ParamType;
      typedef PrivateNSFSM::Pair<unsigned, ParamType> EventPair;
    public:
      typedef TException TFiniteStateMachineException;
      FiniteStateMachine()
        : CurState(FirstTransitionType::GetCurState())
        , InProcess(false)
      {
      }
      virtual ~FiniteStateMachine()
      {
      }
      unsigned GetCurState() const
      {
        return CurState;
      }
      void PostEvent(unsigned event, ParamType prm)
      {
        if (InProcess)
        {
          Events.push_back(EventPair(event, prm));
          return;
        }
        InProcess = true;
        try
        {
          typedef PrivateNSFSM::Executer<ThisType, ParamType, TTransitionTable> ExecuterType;
          ExecuterType().ProcessEvent(*this, event, prm, &CurState);
          while (!Events.empty())
          {
            EventPair EventItem = *Events.begin();
            ExecuterType().ProcessEvent(*this, EventItem.First, EventItem.Second, &CurState);
            Events.erase(Events.begin());
          }
        }
        catch (...)
        {
          InProcess = false;
          Events.clear();
          throw;
        }
        Events.clear();
        InProcess = false;
      }
    private:
      typedef std::list<EventPair> ListEvents;
      ListEvents Events;
      unsigned CurState;
      bool InProcess;
    };


    namespace PrivateNSDFSM
    {

      template <typename TEvent, typename TState>
      class TransitionKey
      {
      public:
        typedef TransitionKey<TEvent, TState> ThisType;
        TransitionKey(const TEvent &event, const TState &state)
          : Event(event)
          , State(state)
        {
        }
        const TEvent& GetEvent() const
        {
          return Event;
        }
        const TState& GetState() const
        {
          return State;
        }
        bool operator == (const ThisType &transition) const
        {
          return Event == transition.Event && State == transition.State;
        }
        bool operator < (const ThisType &transition) const
        {
          return Event < transition.Event ? true :
            Event == transition.Event ? State < transition.State :
            false;
        }
      private:
        TEvent Event;
        TState State;
      };

      template <typename TState, typename TCallback>
      class TransitionValue
      {
      public:
        TransitionValue(const TState &state, const TCallback &callback)
          : State(state)
          , Callback(callback)
        {
        }
        const TState& GetState() const
        {
          return State;
        }
        TCallback GetCallback() const
        {
          return Callback;
        }
      private:
        TState State;
        TCallback Callback;
      };

    }

    namespace PrivateNSDFSM
    {

      template <typename TParam>
      struct ICallback
      {
        virtual ~ICallback()
        {
        }
        virtual void Do(TParam param) = 0;
      };

      template <typename TClass, typename TParam>
      class ICallbackImpl
        : public ICallback<TParam>
      {
      public:
        typedef void (TClass::*CallbackMethod)(TParam);
        ICallbackImpl(TClass &obj, CallbackMethod mtd)
          : Obj(obj)
          , Mtd(mtd)
        {
        }
        virtual void Do(TParam param)
        {
          (Obj.*Mtd)(param);
        }
      private:
        TClass &Obj;
        CallbackMethod Mtd;
      };

    }

    template
    <
      typename TEvent,
      typename TState,
      typename TParam,
      typename TBaseClasses = NullType,
      typename TException = FiniteStateMachineException
    >
    class DynamicFiniteStateMachine
      : public Common::MultipleInheritance<TBaseClasses>
    {
    public:
      DynamicFiniteStateMachine()
        : InProcess(false)
      {
      }
      virtual ~DynamicFiniteStateMachine()
      {
      }
      template <typename THandlerClass>
      void AddTransition(const TEvent &event, const TState &curState,
        const TState &newState, THandlerClass &obj, void (THandlerClass::*callback)(TParam))
      {
        TransitionKey Key(event, curState);
        if (Transitions.find(Key) != Transitions.end())
          throw TException("Transition already exists");
        Transitions[Key] = TransitionValuePtr(new TransitionValue(newState,
          ICallbackPtr(new PrivateNSDFSM::ICallbackImpl<THandlerClass, TParam>(obj, callback))));
      }
      void SetStartState(const TState &startState)
      {
        if (CurState.Get())
          throw TException("Finite state machine is not in the unknown state");
        CurState.Reset(new TState(startState));
      }
      void PostEvent(const TEvent &event, const TParam &params, bool afterAll = false)
      {
        if (!CurState.Get())
          throw TException("Finite state machine in the unknown state");
        if (InProcess)
        {
          (afterAll ? TailEvents : Events).push_back(EventPair(event, params));
          return;
        }
        InProcess = true;
        try
        {
          typename TransitionPool::iterator Iter = Transitions.find(TransitionKey(event, *CurState.Get()));
          if (Iter == Transitions.end())
            throw TException("Transition not found");
          Iter->second->GetCallback()->Do(params);
          *CurState.Get() = Iter->second->GetState();
          while (!Events.empty())
          {
            EventPair EventItem = *Events.begin();
            Iter = Transitions.find(TransitionKey(EventItem.first, *CurState.Get()));
            if (Iter == Transitions.end())
              throw TException("Transition not found");
            Iter->second->GetCallback()->Do(EventItem.second);
            *CurState.Get() = Iter->second->GetState();
            Events.erase(Events.begin());
            if (Events.empty() && !TailEvents.empty())
              Events.swap(TailEvents);
          }
        }
        catch (...)
        {
          InProcess = false;
          Events.clear();
          TailEvents.clear();
          throw;
        }
        Events.clear();
        TailEvents.clear();
        InProcess = false;
      }
    private:
      typedef PrivateNSDFSM::TransitionKey<TEvent, TState> TransitionKey;
      typedef SharedPtr<PrivateNSDFSM::ICallback<TParam> > ICallbackPtr;
      typedef PrivateNSDFSM::TransitionValue<TState, ICallbackPtr> TransitionValue;
      typedef SharedPtr<TransitionValue> TransitionValuePtr;
      typedef std::map<TransitionKey, TransitionValuePtr> TransitionPool;
      typedef std::pair<TEvent, TParam> EventPair;
      typedef std::list<EventPair> ListEvents;
      TransitionPool Transitions;
      ListEvents Events;
      ListEvents TailEvents;
      Common::ValuePtr<TState> CurState;
      bool InProcess;
    };

  }

}


#define BEGIN_TRANSITIONS_TABLE() \
  typedef Common::TypeListAdapter<

#define END_TRANSITIONS_TABLE(tableName) \
  Common::NullType \
  > tableName;

#define DECLARE_TRANSITION(event, state, newState, handlerObjectType, handler, handlerParam) \
  Common::FSM::Transition \
    < \
      event, \
      state, \
      newState, \
      Common::FSM::Callback<handlerObjectType, handlerParam, &handlerObjectType::handler> \
    >,


#endif  // !__FSM_H__
