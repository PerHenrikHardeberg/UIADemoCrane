This folder includes software for the small knuckele boom demo model crane from UIA, developed for Per Henrik Hardeberg's Master Thesis.

The software contains:
- Beckhoff PLC program : See under
- Arduino Input program (Cpp) : Contains program based on EasyCAT template file including several arduino libraries to handle sensor data
- Arduino Outout program (Cpp) : Contains program based on EasyCAT template file including several arduino libraries to handle servo motors
- EasyCAT Slave file for input slave : EasyConfigurator project includes configuration file for input shield
- EasyCAT Slave file for output slave : EasyConfigurator project includes configuration file for output shield


Beckhoff PLC program: Contains the logic of the crane controller.
- The control box folder is copied from couse example project from Daniel Hagen, in course MAS418 at University of Agder. His github repo:  https://github.com/hagenmek. The program modified to include one extra push button on each joystick.
- The kinematic function blocks and functions are generated from Matlab Simulink. These functons comes from github repo: https://github.com/NxRLab/ModernRobotics/tree/master/packages/MATLAB/mr, a repository representing the functions presented in the book: Modern Robotics, by Kevin M. Lynch and Frank C. Park.