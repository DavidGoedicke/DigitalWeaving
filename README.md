# Digital Weaving 

This project is the coding slip scanner and digital simulation of the Communal Loom project. 

## What do you need:
- A laptop with camera and/or a standalone webcam. If both exist, the program will prioritze the standalone webcam. 
- A coding slip from the Communal Loom Project 
- Python 3.x
- If you never installed Xcode Command Line Tool, install Xcode Command Line Tool and reboot your device. 

## How to run the program:
- navigate to `~/python/
- run `./ install.sh`. This will install the Python dependencies needed.
- run `./ run.sh`. This will start the webcam process.
- navigate to `/bin/` and run `DigitalWeavingDebug.app`. This should start the visual simulation of the loom. 

## How to use the program:
- Place a coding slip in front of the webcam. You can visualize it in the webcam. 
- The recognized number of filled circles will display in the top left corner of the loom simulation. When no coding slip is present, it should appear 0.
- Press the `space bar` to generate a simulation of the weave. 
