#pragma once

#include <Windows.h>
#include <set>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

class GlobalInput : public Object {
    GDCLASS(GlobalInput, Object)

    static GlobalInput *singleton;

    std::set<int> prev_keys;
    std::set<int> current_keys;
    std::set<int> just_pressed_keys;
    std::set<int> just_released_keys;

    std::set<int> prev_mouse_buttons;
    std::set<int> current_mouse_buttons;
    std::set<int> just_pressed_mouse_buttons;
    std::set<int> just_released_mouse_buttons;

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

    bool is_global_mouse_button_pressed(int p_button);
    bool is_global_mouse_button_just_pressed(int p_button);
    bool is_global_mouse_button_just_released(int p_button);
    Vector2 get_global_mouse_position();

    bool is_caps_lock_active();
    bool is_num_lock_active();
    bool is_scroll_lock_active();

    bool is_global_input_pressed(const String &p_action);
    bool is_global_input_just_pressed(const String &p_action);
    bool is_global_input_just_released(const String &p_action);
};

} // namespace godot