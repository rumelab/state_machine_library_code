#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <memory>

namespace RumeLab::Detail
{

    template <typename PStoreType>
    class StateMachine;

    template <typename PStoreType>
    class State
    {
    public:
        using StateMachineType = StateMachine<PStoreType>;

    private:
    public:
        virtual ~State() {}
        virtual auto enter(StateMachineType &p_state_machine) -> void {}
        virtual auto process(StateMachineType &p_state_machine, uint32_t p_delta) -> void {}
        virtual auto exit(StateMachineType &p_state_machine) -> void {}
    };

    template <typename PStoreType>
    class StateMachine
    {
    public:
        using StoreType = PStoreType;
        using StateType = State<StoreType>;

    private:
    protected:
        StateType *current_state;

    public:
        StoreType store;

        constexpr StateMachine(StoreType p_store, StateType *p_current_state = nullptr)
            : current_state(p_current_state), store(std::move(p_store))
        {
            if (current_state != nullptr)
                current_state->enter(*this);
        }

        constexpr StateMachine(StateMachine &p_state_machine)
            : StateMachine(p_state_machine.store, p_state_machine.current_state) {}

        constexpr StateMachine(StateMachine &&p_state_machine)
            : store(std::move(p_state_machine.store)), current_state(p_state_machine.current_state)
        {
            p_state_machine.current_state = nullptr;
        }

        ~StateMachine()
        {
            to(nullptr);
        }

        inline auto operator=(StateMachine &p_state_machine) -> StateMachine &
        {
            if (current_state != nullptr)
                current_state->exit(*this);

            store = p_state_machine.store;

            current_state = p_state_machine.current_state;
            if (current_state != nullptr)
                current_state->enter(*this);
        }

        inline auto operator=(StateMachine &&p_state_machine) -> StateMachine &
        {
            if (current_state != nullptr)
                current_state->exit(*this);

            store = std::move(p_state_machine.store);
            current_state = p_state_machine.current_state;

            p_state_machine.current_state = nullptr;
        }

        inline auto to(StateType *p_state) -> void
        {
            if (current_state != nullptr)
                current_state->exit(*this);

            current_state = p_state;

            if (current_state != nullptr)
                current_state->enter(*this);
        }

        inline auto get_current_state() const -> StateType *
        {
            return current_state;
        }

        auto process(uint32_t p_delta) -> void
        {
            if (current_state != nullptr)
                current_state->process(*this, p_delta);
        }
    };

} // namespace RumeLab::Detail

template <typename PStoreType>
using StateMachine = RumeLab::Detail::StateMachine<PStoreType>;

template <typename PStoreType>
using State = RumeLab::Detail::State<PStoreType>;

#endif // STATE_MACHINE_HPP