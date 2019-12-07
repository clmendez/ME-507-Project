// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <Servo.h>

/* 
 * Declaring a global variable
 * 
 */
void TaskMotor( void *pvParameters );
void TaskSensor( void *pvParameters );
void TaskSerial( void *pvParameters );
void TaskUser( void *pvParameters );
void TaskServo( void *pvParameters );

QueueHandle_t sensorQueue;
QueueHandle_t motorQueue;
QueueHandle_t currentQueue;
QueueHandle_t userQueue;

Servo myservo;

void setup() {

sensorQueue = xQueueCreate(10, sizeof(int));
  
motorQueue = xQueueCreate(10, sizeof(int));

currentQueue = xQueueCreate(10, sizeof(int));

userQueue = xQueueCreate(10, sizeof(int));



  if (sensorQueue != NULL)
  {
    
    // Create task that consumes the queue if it was created.
    xTaskCreate(TaskMotor, // Task function
                "Motor", // A name just for humans
                128,  // This stack size can be checked & adjusted by reading the Stack Highwater
                NULL, 
                0, // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
                NULL);

  xTaskCreate(TaskSensor,  "Sensor", 64,  NULL, 0, NULL );

  xTaskCreate(TaskSerial, "Serial", 128, NULL,3, NULL );

  xTaskCreate(TaskUser,  "User", 64,  NULL, 3, NULL );

  xTaskCreate(TaskServo,  "Grip", 64,  NULL, 2, NULL );

  }

}

void loop() {}
/**
 * Motor task
 * Receives signal from sensor tasks and turns the motor off or on in clockwise or counterclockwise direction.
 */
void TaskMotor(void *pvParameters __attribute__((unused)))
{

	int motor = 0;
	int valueFromSensor= 0;

	int motorPin = 6;
	int dirPin = 5;

	int thumbmotorPin = 3;
	int thumbdirPin = 2;

	pinMode(motorPin, OUTPUT);
	pinMode(dirPin, OUTPUT);

	pinMode(thumbmotorPin, OUTPUT);
	pinMode(thumbdirPin, OUTPUT);

	for (;;)
	{
		if (motor == 0)
		{
			digitalWrite(motorPin, 0);
			digitalWrite(dirPin, LOW);

			digitalWrite(thumbmotorPin, 0);
			digitalWrite(thumbdirPin, LOW);

			if(xQueueReceive(sensorQueue, &valueFromSensor, portMAX_DELAY) == pdPASS )
			{
				if (valueFromSensor == 1)
				{
					motor = 1;
				}

				if (valueFromSensor == 2)
				{
					motor = 2;
				}
			}
		}

		else if (motor == 1)
		{
			digitalWrite(motorPin, 255);
			digitalWrite(dirPin, LOW);

			digitalWrite(thumbmotorPin, 60);
			digitalWrite(thumbdirPin, LOW);


			xQueueSend(motorQueue, &motor, portMAX_DELAY);


			if(xQueueReceive(sensorQueue, &valueFromSensor, portMAX_DELAY) == pdPASS )
			{
				if (valueFromSensor == 0)
				{
					motor = 0;
				}
			}
		}

		else if (motor == 2)
		{
			digitalWrite(motorPin, 255);
			digitalWrite(dirPin, HIGH);

			digitalWrite(thumbmotorPin, 60);
			digitalWrite(thumbdirPin, HIGH);


			xQueueSend(motorQueue, &motor, portMAX_DELAY);

			if(xQueueReceive(sensorQueue, &valueFromSensor, portMAX_DELAY) == pdPASS )
			{
				if (valueFromSensor == 0)
				{
					motor = 0;
				}

				if (valueFromSensor == 1)
				{
				   motor = 1;
				}
			}
		}

		vTaskDelay(1);
	}
}

/* 
 * Sensor task.
 * reads pins that myosensors are on and sends signal to motor task
 */
void TaskSensor(void *pvParameters __attribute__((unused)))
{
	int waiting = 0;
	int open = 2;
	int close = 1;

	int sensorThreshold = 500;
	int currentThreshold = 500;

	int open_close = waiting;

	for (;;)
	{
	    int sensorValue0 = analogRead(A0);

	   int sensorValue1 = analogRead(A1);

	    if (open_close == waiting) // 0
	     {
	    	xQueueSend(sensorQueue, &open_close, portMAX_DELAY);

	    	if (sensorValue0 > sensorThreshold)
	    	{
	    	   open_close = close;
	    	}

	    	else if (sensorValue1 > sensorThreshold)
	    	{
	    	  open_close = open;
	    	}
	    }

	    else if (open_close == close) // 1
	   	    {
	   	    	xQueueSend(sensorQueue, &open_close, portMAX_DELAY);


	   	    		if (sensorValue0 < currentThreshold)
	   	    		{
	   	    			open_close = waiting;
	   	    		}
	   	    }

	   else if (open_close == open) // 2
	   {
	    	xQueueSend(sensorQueue, &open_close, portMAX_DELAY);

	        if (sensorValue1 < sensorThreshold)
	        {
	          open_close = waiting;

	        }
	        if (sensorValue0 > sensorThreshold)
	        {
	        	open_close = close;

	        }


       }
      vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
   }
}

/*
 * Serial task.
 * reads queue variables and outputs to serial monitor.
 */
void TaskSerial(void *pvParameters __attribute__((unused)))
{

	int valueFromSensor = 0;
	int valueFromMotor = 0;
	int valueFromUser = 0;

	Serial.begin(9600);

	while (!Serial)
	{
	  vTaskDelay(1) ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
	}

	for (;;)
	{
		if (xQueueReceive(sensorQueue, &valueFromSensor, portMAX_DELAY) == pdPASS)
		{
		      Serial.println(valueFromSensor);
		}

		if (xQueueReceive(userQueue, &valueFromUser, portMAX_DELAY) == pdPASS)
		{
			Serial.println("Button:");
			Serial.println(valueFromUser);
		}


		if (xQueueReceive(motorQueue, &valueFromMotor, portMAX_DELAY) == pdPASS)
		{
			if (valueFromMotor == 1)
			{
				 Serial.println("Clockwise");
			}

			if (valueFromMotor == 2)
			{
				 Serial.println("CounterClock");
			}
		}
	}

}
/*
 * User task.
 * reads signal from button and sends to servo task.
 */

void TaskUser(void *pvParameters __attribute__((unused)))
{

	int Button = 4;

	int gripState = 0;
	int buttonCurrent;
	int buttonPrevious = LOW;

	pinMode(Button, INPUT);


	for (;;)
	{
		buttonCurrent = digitalRead(Button);

		if(buttonCurrent == HIGH && buttonPrevious == LOW)
		{
			if (gripState == 1)
			{
				gripState = 0;
			}

			else

			{
				gripState = 1;
			}
		}

		xQueueSend(userQueue, &gripState, portMAX_DELAY);
		buttonPrevious = buttonCurrent;
	}
}

/*
 * Servo task.
 * receives signal from user task and moves the thumb either in out or in position.
 */
void TaskServo(void *pvParameters __attribute__((unused)))
{
	int servo = 0;
	int valueFromUser= 0;

	myservo.attach(8);

	for (;;)
	{
		if (servo == 0) // thumb out
		{
			myservo.write(0);
			delay(15);

			if(xQueueReceive(userQueue, &valueFromUser, portMAX_DELAY) == pdPASS )
			{
				if (valueFromUser == 1)
				{
					servo = 1;
				}
			}
		}

		else if (servo == 1) // thumb in
		{
			myservo.write(130);
			delay(15);

			if(xQueueReceive(userQueue, &valueFromUser, portMAX_DELAY) == pdPASS )
			{
				if (valueFromUser == 0)
				{
					servo = 0;
				}
			}
		}
		vTaskDelay(1);
	}
}
