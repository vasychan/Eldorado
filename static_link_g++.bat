SET QTDIR=C:\Qt\4.8.2
SET PATH=C:\Qt\4.7.4\bin;%PATH%
configure -opensource -release ^
-nomake examples -nomake demos -nomake tools ^
-nomake translations -nomake docs ^
-no-exceptions -no-stl -no-rtti ^
-no-qt3support -no-scripttools -no-openssl -no-opengl ^
-no-dsp -no-webkit -no-phonon ^
-no-style-motif -no-style-cde -no-style-cleanlooks ^
-no-style-plastique ^
-no-script -no-scripttools -no-declarative ^
-static -platform win32-g++