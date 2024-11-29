# State Machine Library Code

State Machine Library is a header-only Platform IO library with Arduino framework
that provides state management with extensible state machine implement,
with complete unit testing (on Raspberry Pi Pico) which could be use as reference.
Below is a sample code snippet to demonstrate the usage of this library.

```cpp

/// This is a simple traffic light implementation with state machine.

/// Shared store across all the state.
class Store {
public:
    using StateType = State<Store>;
private:
public:
    StateType* red_light_state;
    StateType* yellow_light_state;
    StateType* green_light_state;

    constexpr Store(StateType* p_red_light_state, StateType* p_yellow_light_state, StateType* p_green_light_state)
    : red_light_state(p_red_light_state), yellow_light_state(p_yellow_light_state), green_light_state(p_green_light_state) {}
};

constexpr const uint8_t red_light_pin = 1;
constexpr const uint8_t yellow_light_pin = 2;
constexpr const uint8_t green_light_pin = 3;

/// Red light state.
class RedLightState : public State<Store> {
public:
private:
    uint32_t duration;

public:
    auto enter(StateMachineType & p_state_machine) -> void override {
        digitalWrite(red_light_pin, HIGH);
        duration = 0;
    }

    auto process(StateMachineType &p_state_machine, uint32_t p_delta) -> void override {
        duration += p_delta;

        if(duration > 5000) // 5 second passed
            p_state_machine.to(p_state_machine.store.yellow_light_state);
    }

    auto exit(StateMachineType& p_state_machine) -> void override {
        digitalWrite(red_light_pin, LOW);
    }
};

/// Yellow light state.
class YellowLightState: public State<Store> {
public:
private:
    uint32_t duration;

public:
    auto enter(StateMachineType & p_state_machine) -> void override {
        digitalWrite(yellow_light_pin, HIGH);
        duration = 0;
    }

    auto process(StateMachineType &p_state_machine, uint32_t p_delta) -> void override {
        duration += p_delta;

        if(duration > 5000) // 5 second passed
            p_state_machine.to(p_state_machine.store.green_light_state);
    }

    auto exit(StateMachineType& p_state_machine) -> void override {
        digitalWrite(yellow_light_pin, LOW);
    }
};

/// Green light state.
class GreenLightState: public State<Store> {
public:
private:
    uint32_t duration;

public:
    auto enter(StateMachineType & p_state_machine) -> void override {
        digitalWrite(green_light_pin, HIGH);
        duration = 0;
    }

    auto process(StateMachineType &p_state_machine, uint32_t p_delta) -> void override {
        duration += p_delta;

        if(duration > 5000) // 5 second passed
            p_state_machine.to(p_state_machine.store.red_light_state);
    }

    auto exit(StateMachineType& p_state_machine) -> void override {
        digitalWrite(green_light_pin, LOW);
    }
};

RedLightState red_light_state;
YellowLightState yellow_light_state;
GreenLightState green_light_state;

Store store(&red_light_state, &yellow_light_state, &green_light_state);

StateMachine<Store> state_machine(store);


void setup() {
    pinMode(red_light_pin, OUTPUT);
    pinMode(yellow_light_pin, OUTPUT);
    pinMode(green_light_pin, OUTPUT);

    state_machine.to(red_light_state); // Begin at red light state;
}

void loop() {
    static uint32_t previous_timestamp = 0;
    const uint32_t current_timestamp = millis();
    const uint32_t delta = current_timestamp - previous_timestamp;
    previous_timestamp = current_timestamp;

    state_machine.process(current_timestamp);
}

```

## Installation

In your `platform.io` library, append the URL of this github repository to the `lib_deps` attribute.

```ini
lib_deps=https://github.com/rumelab/state_machine_library_code
```
