# gd-global-input

A GDExtension for Godot 4.5+ that provides global (system-wide) keyboard input detection on Windows. It allows your Godot application to receive and respond to keyboard input even when the game window is not in focus—perfect for overlays, macro tools, or background utilities.

Currently, it only supports **Windows**. Support for other platforms is planned for future updates.

## Features

- **System-Wide Input Detection:** Detects key presses globally on Windows using `GetAsyncKeyState`.
- **Background Support:** Continues to work reliably when the Godot window loses focus.
- **InputMap Integration:** Transparently integrates with Godot's built-in `InputMap` system for action-based input.
- **High Performance & Stability:** Designed with a lightweight polling mechanism for low latency and high reliability, avoiding the pitfalls of system-wide hooks.

## Installation

### Method 1: Pre-built Releases (Recommended)

1. Go to the [Releases](https://github.com/aliarcanakgun/gd-global-input/releases) page.
2. Download the latest release `.zip` archive.
3. Extract the contents directly into your Godot project's folder. It should create an `addons/gd-global-input/` structure.
4. Reload your Godot project. The extension will automatically load and be ready to use.

### Method 2: Building from Source

If you prefer to compile the extension yourself:

1. Clone this repository and initialize submodules:
   ```bash
   git clone https://github.com/aliarcanakgun/gd-global-input.git
   cd gd-global-input
   git submodule update --init --recursive
   ```

2. Make sure you have [SCons](https://scons.org/) and a modern C++ compiler (like MSVC on Windows) installed.

3. Compile the extension for your desired target:
   ```bash
   scons target=template_debug
   scons target=template_release
   ```

4. The compiled extension libraries will be naturally generated under the `demo/addons/gd-global-input/<platform>/` folder. You can safely copy the entire `addons/gd-global-input` folder to your Godot project.

## Usage

The extension provides a `GlobalInput` singleton that you can access from any GDScript file.

### Checking Key States

**Check if a key is currently held down:**
```gdscript
if GlobalInput.is_global_key_pressed(KEY_A):
    print("Global 'A' key is currently pressed")
```

**Check if a key was just pressed this frame:**
```gdscript
if GlobalInput.is_global_key_just_pressed(KEY_SPACE):
    print("Global Spacebar just pressed")
```

**Check if a key was just released this frame:**
```gdscript
if GlobalInput.is_global_key_just_released(KEY_ESCAPE):
    print("Global Escape just released")
```

### Checking Input Map Actions

You can also check for custom actions defined in your project's `InputMap` (Project -> Project Settings -> Input Map). 

**Check if a custom action is just pressed:**
```gdscript
if GlobalInput.is_global_input_just_pressed("my_custom_action"):
    print("Custom action just pressed globally")
```

**Check if a custom action is just released:**
```gdscript
if GlobalInput.is_global_input_just_released("my_custom_action"):
    print("Custom action just released globally")
```

**Check if a custom action is currently held:**
```gdscript
if GlobalInput.is_global_input_pressed("my_custom_action"):
    print("Custom action is being held down")
```
