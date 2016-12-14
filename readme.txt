-----------------------------------------------------------------
Flight AR.Drone 2
Copyright (C) 2013 kzhong1991
-----------------------------------------------------------------

INTRODUCTION
  Flight AR.Drone 2 is free software; you can redistribute it and/or
  modify it under the terms of EITHER:
   (1) The GNU Lesser General Public License as published by the Free
       Software Foundation; either version 2.1 of the License, or (at
       your option) any later version. The text of the GNU Lesser
       General Public License is included with this library in the
       file Flight_AR.Drone_2-license-LGPL.txt.
   (2) The BSD-style license that is included with this library in
       the file Flight_AR.Drone_2-license-BSD.txt.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
  Flight_AR.Drone_2-license-LGPL.txt and Flight_AR.Drone_2-license-BSD.txt for more details.

HOW TO INSTALL
  Please download and compile it.

HOW TO UNINSTALL
  Please delete it.

BEFORE YOU BUILD
  1. Microsoft Kinect SDK
  The Microsoft Kinect SDK is needed to build the project, see http://www.microsoft.com/en-us/kinectforwindows/develop/overview.aspx;
  2. Qt
  The Qt4.8.4 source code is included in the src\3rdparty\Qt4.8.4, but you still need the qmake tool to complie the project source code;
  3. VS 2010
  You should install Visual Studio before you build Flight AR.Drone 2, after you installed the VS2010, the qt-vs-addin is also need to build a Qt project.
  To download VS, please see http://www.microsoft.com/visualstudio/eng/downloads .

HOW TO USE
  Before you use it, you have to compile the source code:
  1. Open build\vs2008\ardrone2.sln
  2. Press F7 to build.
  3. Press F5 (or Ctrl+F5) to run.
  If you don't want to compile it, you can use the pre-build binary file bin\vs2008\ardrone2.exe to control your ardrone.
  
HOW TO CONTROL AR.DRONE2:
  1. Keyboard control:
	W: forward
	S: backward
	A: turn left
	D: turn right
	I: go up
	K: go down
	J: rotate clockwise
	L: rotate anticlockwise
	SPACE: landing/takeoff
	F11: emergency mode (cut off the engine immediately!!!)
	F12: disable/enable keyboard control
  2. Flight Stick control. I use Cyborg V.1, see http://www.cyborggaming.com/prod/v1stick.htm. Just map the Flight Stick action to the keyboard event to control your adrone. You can DIY or use my profile file Ardrone.pr0.
  3. Kinect control. See the wiki page about how to control ar.drone with MS Kinect.
	 
	 

LIBRARY DEPENDENCIES
  Flight AR.Drone 2 uses following libraries.
  - OpenCV 2.4.4
    http://opencv.org/
  - FFmpeg 1.2
    http://www.ffmpeg.org/
  - Kinect SDK 1.7
    http://www.microsoft.com/en-us/kinectforwindows/develop/overview.aspx;
  - Qt 4.8.4
    http://qt-project.org/downloads

  License files for each library can be found in the 'licenses' folder.

Thank you.
