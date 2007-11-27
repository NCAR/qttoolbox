toolname = 'qtt_common'
from SCons.Script.SConscript import global_exports
if not global_exports.has_key(toolname):
    import SCons.Defaults
    SCons.Defaults.DefaultEnvironment().SConscript('#/tool_qtt_common.py')

def generate(env):
    global_exports[toolname](env)
        
def exists(env):
    return True
