#ifndef TEST_STATE_MACHINE_HPP
#define TEST_STATE_MACHINE_HPP

#include <Arduino.h>
#include <unity.h>
#include <state_machine.hpp>

namespace RumeLab::Detail
{

    template <uint8_t PNumber = 5>
    class SampleState : public ::State<uint8_t>
    {
    public:
        static constexpr const uint8_t NUMBER = PNumber;

    private:
    public:
        auto enter(StateMachineType &p_state_machine) -> void override
        {
            p_state_machine.store *= NUMBER;
        }

        auto process(StateMachineType &p_state_machine, uint32_t p_delta) -> void override
        {
            p_state_machine.store += NUMBER;
        }

        auto exit(StateMachineType &p_state_machine) -> void override
        {
            p_state_machine.store /= NUMBER;
        }
    };

    using SampleStateMachine = ::StateMachine<uint8_t>;

    template <typename = void>
    auto test_state_machine() -> void
    {
        SampleState<3> sample_state_3;
        SampleState<5> sample_state_5;

        SampleStateMachine state_machine(0);

        // Enter sample state 3.
        state_machine.to(&sample_state_3);
        // Based on the implementation, the state machine's store is multiplied by 3 by sample state 3.
        TEST_ASSERT_EQUAL_UINT8(0, state_machine.store);

        state_machine.process(0);
        // Based on the implementation, the state machine's store is increment by 3 by sample state 3.
        TEST_ASSERT_EQUAL_UINT8(3, state_machine.store);

        // Switch state to sample state 5.
        state_machine.to(&sample_state_5);
        // Based on the implementation,
        // the state machine's store is divided by 3 as exit from sample state 3,
        // and it is multiplied by 5 as enter to sample state 5.
        TEST_ASSERT_EQUAL_UINT8(5, state_machine.store);

        state_machine.process(0);
        // Based on the implementation, the state machine's store is increment by 5 by sample state 5.
        TEST_ASSERT_EQUAL_UINT8(10, state_machine.store);

        // Exit sample state 5.
        state_machine.to(nullptr);
        // Based on the implementation, the state machine's store is divided by 5 as it exits from sample state 5.
        TEST_ASSERT_EQUAL_UINT16(2, state_machine.store);
    }

} // namespace RumeLab::Detail

constexpr auto test_state_machine = &RumeLab::Detail::test_state_machine<>;

#endif // TEST_STATE_MACHINE_HPP