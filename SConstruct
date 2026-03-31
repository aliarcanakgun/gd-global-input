#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/"])
env.Append(LIBS=["user32"])

sources = Glob("src/*.cpp")

platform = env["platform"]
name = "gd-global-input"
lib_filename = f"{env.subst('$SHLIBPREFIX')}{name}{env['suffix']}{env.subst('$SHLIBSUFFIX')}"

library = env.SharedLibrary(
    f"demo/addons/{name}/{platform}/{lib_filename}",
    source=sources,
)

Default(library)
