# qttoolbox
General purpose Qt components

The master branch now builds all the Qt toolbox components against Qt5.

Run `scons .` in this directory to build everything, including test
programs.  To use individual components within other source projects, apply
the component tools to the Environment like `Require('qtt_qtconfig')`.
There is no need to call the `SConstruct` file from other `SConscript`
files, the tools will be found automatically by `eol_scons`.
