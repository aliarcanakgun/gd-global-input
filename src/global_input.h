#pragma once

#include <Windows.h>
#include <set>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class GlobalInput : public Object {
    GDCLASS(GlobalInput, Object)

    static GlobalInput *singleton;

    std::set<int> prev_keys;
    std::set<int> current_keys;
    std::set<int> just_pressed_keys;
    std::set<int> just_released_keys;
    uint64_t last_synced_frame = UINT64_MAX;

    void sync_if_needed();

protected:
    static void _bind_methods();

public:
    static GlobalInput *get_singleton();

    GlobalInput();
    ~GlobalInput();

    bool is_global_key_pressed(int p_keycode);
    bool is_global_key_just_pressed(int p_keycode);
    bool is_global_key_just_released(int p_keycode);

    bool is_global_input_pressed(const String &p_action);
    bool is_global_input_just_pressed(const String &p_action);
    bool is_global_input_just_released(const String &p_action);
};

} // namespace godot