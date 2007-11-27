toolname = 'qtt_qtconfig'
from SCons.Script.SConscript import global_exports
if not global_exports.has_key(toolname):
    import SCons.Defaults
    SCons.Defaults.DefaultEnvironment().SConscript('#/QtConfig/tool_qtt_qtconfig.py')

def generate(env):
    global_exports[toolname](env)
        
def exists(env):
    return True
