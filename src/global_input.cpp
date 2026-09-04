#include "global_input.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/variant/array.hpp>

using namespace godot;

GlobalInput *GlobalInput::singleton = nullptr;

// lifecycle

GlobalInput *GlobalInput::get_singleton() {
    return singleton;
}

GlobalInput::GlobalInput() {
    singleton = this;
}

GlobalInput::~GlobalInput() {
    singleton = nullptr;
}

void GlobalInput::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_global_key_pressed", "keycode"), &GlobalInput::is_global_key_pressed);
    ClassDB::bind_method(D_METHOD("is_global_key_just_pressed", "keycode"), &GlobalInput::is_global_key_just_pressed);
    ClassDB::bind_method(D_METHOD("is_global_key_just_released", "keycode"), &GlobalInput::is_global_key_just_released);
    ClassDB::bind_method(D_METHOD("is_global_input_pressed", "action"), &GlobalInput::is_global_input_pressed);
    ClassDB::bind_method(D_METHOD("is_global_input_just_pressed", "action"), &GlobalInput::is_global_input_just_pressed);
    ClassDB::bind_method(D_METHOD("is_global_input_just_released", "action"), &GlobalInput::is_global_input_just_released);

    ClassDB::bind_method(D_METHOD("is_global_mouse_button_pressed", "button_index"), &GlobalInput::is_global_mouse_button_pressed);
    ClassDB::bind_method(D_METHOD("is_global_mouse_button_just_pressed", "button_index"), &GlobalInput::is_global_mouse_button_just_pressed);
    ClassDB::bind_method(D_METHOD("is_global_mouse_button_just_released", "button_index"), &GlobalInput::is_global_mouse_button_just_released);
    ClassDB::bind_method(D_METHOD("get_global_mouse_position"), &GlobalInput::get_global_mouse_position);

    ClassDB::bind_method(D_METHOD("is_caps_lock_active"), &GlobalInput::is_caps_lock_active);
    ClassDB::bind_method(D_METHOD("is_num_lock_active"), &GlobalInput::is_num_lock_active);
    ClassDB::bind_method(D_METHOD("is_scroll_lock_active"), &GlobalInput::is_scroll_lock_active);
}

// vk to godot keycode mapping

static int vk_to_godot(int vk) {
    // ignore mouse buttons
    if (vk >= 0x01 && vk <= 0x06) return 0;

    // ascii range matches directly (0-9, A-Z, space)
    if ((vk >= 0x30 && vk <= 0x39) ||
        (vk >= 0x41 && vk <= 0x5A) ||
        vk == 0x20)
        return vk;

    static const int S = (1 << 22); // KEY_SPECIAL

    switch (vk) {
        case VK_ESCAPE:    return S | 0x01;
        case VK_TAB:       return S | 0x02;
        case VK_BACK:      return S | 0x04;
        case VK_RETURN:    return S | 0x05;
        case VK_INSERT:    return S | 0x07;
        case VK_DELETE:    return S | 0x08;
        case VK_PAUSE:     return S | 0x09;
        case VK_SNAPSHOT:  return S | 0x0A;
        case VK_HOME:      return S | 0x0D;
        case VK_END:       return S | 0x0E;
        case VK_LEFT:      return S | 0x0F;
        case VK_UP:        return S | 0x10;
        case VK_RIGHT:     return S | 0x11;
        case VK_DOWN:      return S | 0x12;
        case VK_PRIOR:     return S | 0x13;
        case VK_NEXT:      return S | 0x14;
        case VK_LSHIFT:
        case VK_RSHIFT:
        case VK_SHIFT:     return S | 0x15;
        case VK_LCONTROL:
        case VK_RCONTROL:
        case VK_CONTROL:   return S | 0x16;
        case VK_LWIN:
        case VK_RWIN:      return S | 0x17;
        case VK_LMENU:
        case VK_RMENU:
        case VK_MENU:      return S | 0x18;
        case VK_CAPITAL:   return S | 0x19;
        case VK_NUMLOCK:   return S | 0x1A;
        case VK_SCROLL:    return S | 0x1B;
        case VK_F1:        return S | 0x1C;
        case VK_F2:        return S | 0x1D;
        case VK_F3:        return S | 0x1E;
        case VK_F4:        return S | 0x1F;
        case VK_F5:        return S | 0x20;
        case VK_F6:        return S | 0x21;
        case VK_F7:        return S | 0x22;
        case VK_F8:        return S | 0x23;
        case VK_F9:        return S | 0x24;
        case VK_F10:       return S | 0x25;
        case VK_F11:       return S | 0x26;
        case VK_F12:       return S | 0x27;
        case VK_MULTIPLY:  return S | 0x81;
        case VK_DIVIDE:    return S | 0x82;
        case VK_SUBTRACT:  return S | 0x83;
        case VK_DECIMAL:   return S | 0x84;
        case VK_ADD:       return S | 0x85;
        case VK_NUMPAD0:   return S | 0x86;
        case VK_NUMPAD1:   return S | 0x87;
        case VK_NUMPAD2:   return S | 0x88;
        case VK_NUMPAD3:   return S | 0x89;
        case VK_NUMPAD4:   return S | 0x8A;
        case VK_NUMPAD5:   return S | 0x8B;
        case VK_NUMPAD6:   return S | 0x8C;
        case VK_NUMPAD7:   return S | 0x8D;
        case VK_NUMPAD8:   return S | 0x8E;
        case VK_NUMPAD9:   return S | 0x8F;
        default:           return vk;
    }
}

// per-frame polling via GetAsyncKeyState

static int vk_to_godot_mouse(int vk) {
    switch (vk) {
        case VK_LBUTTON: return 1; // MOUSE_BUTTON_LEFT
        case VK_RBUTTON: return 2; // MOUSE_BUTTON_RIGHT
        case VK_MBUTTON: return 3; // MOUSE_BUTTON_MIDDLE
        case VK_XBUTTON1: return 8; // MOUSE_BUTTON_XBUTTON1
        case VK_XBUTTON2: return 9; // MOUSE_BUTTON_XBUTTON2
        default: return 0;
    }
}

void GlobalInput::sync_if_needed() {
    uint64_t frame = Engine::get_singleton()->get_process_frames();
    if (frame == last_synced_frame)
        return;

    prev_keys = current_keys;
    current_keys.clear();

    prev_mouse_buttons = current_mouse_buttons;
    current_mouse_buttons.clear();

    for (int vk = 1; vk <= 254; vk++) {
        if (GetAsyncKeyState(vk) & 0x8000) {
            int godot_key = vk_to_godot(vk);
            if (godot_key != 0) current_keys.insert(godot_key);
            
            int godot_mouse = vk_to_godot_mouse(vk);
            if (godot_mouse != 0) current_mouse_buttons.insert(godot_mouse);
        }
    }

    just_pressed_keys.clear();
    just_released_keys.clear();
    
    just_pressed_mouse_buttons.clear();
    just_released_mouse_buttons.clear();

    for (int k : current_keys) {
        if (!prev_keys.count(k))
            just_pressed_keys.insert(k);
    }
    for (int k : prev_keys) {
        if (!current_keys.count(k))
            just_released_keys.insert(k);
    }

    for (int mb : current_mouse_buttons) {
        if (!prev_mouse_buttons.count(mb))
            just_pressed_mouse_buttons.insert(mb);
    }
    for (int mb : prev_mouse_buttons) {
        if (!current_mouse_buttons.count(mb))
            just_released_mouse_buttons.insert(mb);
    }

    last_synced_frame = frame;
}

// key and mouse queries

bool GlobalInput::is_global_mouse_button_pressed(int p_button) {
    sync_if_needed();
    return current_mouse_buttons.count(p_button) > 0;
}

bool GlobalInput::is_global_mouse_button_just_pressed(int p_button) {
    sync_if_needed();
    return just_pressed_mouse_buttons.count(p_button) > 0;
}

bool GlobalInput::is_global_mouse_button_just_released(int p_button) {
    sync_if_needed();
    return just_released_mouse_buttons.count(p_button) > 0;
}

Vector2 GlobalInput::get_global_mouse_position() {
    POINT p;
    if (GetCursorPos(&p)) {
        return Vector2(p.x, p.y);
    }
    return Vector2();
}

bool GlobalInput::is_global_key_pressed(int p_keycode) {
    sync_if_needed();
    return current_keys.count(p_keycode) > 0;
}

bool GlobalInput::is_global_key_just_pressed(int p_keycode) {
    sync_if_needed();
    return just_pressed_keys.count(p_keycode) > 0;
}

bool GlobalInput::is_global_key_just_released(int p_keycode) {
    sync_if_needed();
    return just_released_keys.count(p_keycode) > 0;
}

// input action queries

struct ActionEvents {
    std::set<int> keys;
    std::set<int> mouse_buttons;
};

static ActionEvents keycodes_for_action(const String &action) {
    ActionEvents out;
    InputMap *im = InputMap::get_singleton();
    if (!im || !im->has_action(action))
        return out;
    Array events = im->action_get_events(action);
    for (int i = 0; i < events.size(); i++) {
        Ref<InputEventKey> k = events[i];
        if (k.is_valid()) {
            int kc = (int)k->get_keycode();
            if (kc != 0) out.keys.insert(kc);
            int pkc = (int)k->get_physical_keycode();
            if (pkc != 0) out.keys.insert(pkc);
        }
        Ref<InputEventMouseButton> m = events[i];
        if (m.is_valid()) {
            out.mouse_buttons.insert((int)m->get_button_index());
        }
    }
    return out;
}

bool GlobalInput::is_global_input_pressed(const String &p_action) {
    sync_if_needed();
    ActionEvents evs = keycodes_for_action(p_action);
    for (int k : evs.keys)
        if (current_keys.count(k)) return true;
    for (int mb : evs.mouse_buttons)
        if (current_mouse_buttons.count(mb)) return true;
    return false;
}

// lock key queries

bool GlobalInput::is_caps_lock_active() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool GlobalInput::is_num_lock_active() {
    return (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
}

bool GlobalInput::is_scroll_lock_active() {
    return (GetKeyState(VK_SCROLL) & 0x0001) != 0;
}

bool GlobalInput::is_global_input_just_pressed(const String &p_action) {
    sync_if_needed();
    ActionEvents evs = keycodes_for_action(p_action);
    for (int k : evs.keys)
        if (just_pressed_keys.count(k)) return true;
    for (int mb : evs.mouse_buttons)
        if (just_pressed_mouse_buttons.count(mb)) return true;
    return false;
}

bool GlobalInput::is_global_input_just_released(const String &p_action) {
    sync_if_needed();
    ActionEvents evs = keycodes_for_action(p_action);
    for (int k : evs.keys)
        if (just_released_keys.count(k)) return true;
    for (int mb : evs.mouse_buttons)
        if (just_released_mouse_buttons.count(mb)) return true;
    return false;
}
