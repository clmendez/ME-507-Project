/**@file mainPage.c */

/**  Includes documentation for the entire project shown on the documentation landing page. */
/** */
/*!  @author Aaliyah Ramos , Claudia Mendez */
/** 
* @mainpage  
* @section sec_intro Introduction  
* Our driver is used to help in the creation of a myoelectric hand project. The hand is able to 
* open and close from signals it gets from myosensors. The thumb of the hand is able to move from either the 
* thumb in or thumb out position when a user presses a button. All the code for this project was created in a single file
* and all the functions in the file are explained in detail. 
* @section sec_sensor Sensor Task
* @subsection subsec_pur Purpose 
* The purpose of this task is to read the myosensors and signal to the motor task what to do.
* @subsection subsec_usage Usage 
* To use the task the following format is used:
*
* void TaskSensor(void *pvParameters __attribute__((unused)))
*
* No parameters needed for this task. 
* @subsection subsec_testing Testing 
* To test if this task worked properly, we used the serial task to see if a variable that was in the sensor task
* was outputting the correct state. For example, if it should be outputting state 1 which meant closed, the serial
* output would give a 1. 
* @subsection subsec_bug Bugs and Limitations
* Some limitations to the myosensors that we used was that they were a bit finicky to use. They were very
* sensitive to movement which made it difficult to control the movement for the hand. They 
* would take some time to notice that you were not flexing before it would shut off. 

* @section sec_motor Motor Task
* @subsection subsec_pur Purpose
* The purpose of this task is to read the signal received from the myosensor task and then either 
* turn the motors off or turn the motors on in the clockwise(Close) or counterclockwise direction (Open).
* @subsection subsec_usage Usage
* To use the task the following format is used:
*
* void TaskMotor(void *pvParameters __attribute__((unused)))
*
* No parameters needed for this task.
* @subsection subsec_testing Testing
* To test if this task worked properly, we used the serial task to signal whenever it was in a certain state when it 
* was suppose to. For example, if the first myosensor was over the threshold the motor task should be in state 1, which
* means the serial task should output a 1. 
* @subsection subsec_bug Bugs and Limitations
* Some limitations to the motors we used was that they were not fast enough when opening the hand which resulting in the hand 
* not opening fully. 

* @section sec_user User Interface Task
* @subsection subsec_pur Purpose
* The purpose of this task is to read a signal that the task receives from a button that the user can press and sends
* the signal to the servo task to use. 
* @subsection subsec_usage Usage
* To use the task the following format is used:
*
* void TaskUser(void *pvParameters __attribute__((unused)))
*
* No parameters needed for this task.
* @subsection subsec_testing Testing
* To test if this task worked properly, we used the serial task to see if a variable that was in the user task
* was outputting the correct state. For example, if it should be outputting state 1 which meant thumb in, the serial
* output would give a 1.
* @subsection subsec_bug Bugs and Limitations
* One limitation was that the motors did not have encoders. It would have been beneficial to
* know the position and speed of the motors. 

* @section sec_servo Servo Task
* @subsection subsec_pur Purpose
* The purpose of this task was to use the servo to move the thumb either in the in ( 100 degrees) or out( 0 degrees) position using the 
* signal that it received from the user task. 
* @subsection subsec_usage Usage
* To use the task the following format is used:
*
* void TaskServo(void *pvParameters __attribute__((unused)))
*
* No parameters needed for this task.
* @subsection subsec_testing Testing
* To test if this task worked properly, we used the serial task to signal whenever it was in a certain state when it
* was suppose to. For example, if the button was pressed for the first time the servo task should be in state 1, which
* means the serial task should output a 1.
* @subsection subsec_bug Bugs and Limitations
* A limitation that we had with the servo was that it didn't have enough torque to overcome
* gravity or the load of the thumb motor which would resulted in some instability and shaking. 

* @section sec_serial Serial Task
* @subsection subsec_pur Purpose
* The purpose of this task was get output from the other tasks for testing and debugging.
* @subsection subsec_usage Usage
* To use the task the following format is used:
*
* void TaskSerial(void *pvParameters __attribute__((unused)))
*
* No parameters needed for this task.
* @subsection subsec_testing Testing
* To test if this task worked properly, we used the sensor task to see if we could get 
* readings onto the serial monitor. For example, if the the hand was flexing then you would 
* get a value of around 900 on the serial monitor. 
* @subsection subsec_bug Bugs and Limitations
* No limitations did as it was suppose to. 
*/