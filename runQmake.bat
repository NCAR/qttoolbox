REM
REM Run qmake for QtToolbox
REM

cd ColorBar
qmake
cd ..
cd ColorBar\Test
qmake
 cd ..\..

cd Knob
qmake
cd ..
cd Knob\test
qmake
cd ..\..

cd TwoKnobs
qmake
cd ..
cd TwoKnobs\Test
qmake
cd ..\..

cd ScopePlot
qmake
cd ..
cd ScopePlot\test
qmake
cd ..\..


cd PPI
qmake
cd ..
cd PPI\test
qmake
cd ..\..
