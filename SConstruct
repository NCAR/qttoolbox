
import eol_scons

# This SConstruct is meant for building everything in qttoolbox.  It does
# not need to be called when qttoolbox is included in other source trees,
# and likewise there is no SConscript file that needs to be called.
# Instead, require the qttoolbox tools directly wherever they are needed.

# Setup a default global environment with qt5 then require all the
# qttoolbox tools.
env = Environment(tools=['default'])
env.RequireGlobal('qt5')
env.Require(Split("""
 qtt_colorbar qtt_knob qtt_ppi qtt_qtbscan qtt_qtconfig
 qtt_scopeplot qtt_statusgauge qtt_twoknobs
"""))

# These SConscript files load Test SConscripts which are not otherwise
# loaded by the tools required above.
SConscript('ColorBar/SConscript')
SConscript('Knob/SConscript')
SConscript('PPI/SConscript')
SConscript('TwoKnobs/SConscript')
SConscript('ScopePlot/SConscript')
SConscript('StatusGauge/SConscript')
