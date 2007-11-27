toolname = "qtt_knob"
from SCons.Script.SConscript import global_exports
if not global_exports.has_key(toolname):
    import SCons.Defaults
    SCons.Defaults.DefaultEnvironment().SConscript('#/Knob/tool_qtt_knob.py')

def generate(env):
    global_exports[toolname](env)

def exists(env):
    return True
