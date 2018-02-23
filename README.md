# AutonomousCooler
The code and documention for the IEEE Innovation Team's Autonomous Cooler

The file CoolerTestDrive.ino implements multiple functionalities into a program that will eventually contain more autonomy algorithms. At the moment it has the ability to:

  -Reads the current being drawn by the motors
  -Test if the RC transmitter (paired with the onboard reciever) is powered on (interrupt)
  -Drive the cooler using either serial line inputs or the RC transmitter
	-Come to a stop if too much current is drawn (one of the motors stall)
