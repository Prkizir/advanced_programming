/*
*  Exam 3: Coaster problem with semaphores
*  Sergio Mercado A01020382
*  19/11/2019
*/

For this problem I used semaphores.

Order of execution:

Compile files

-> gcc coaster.c -o coaster
-> gcc car.c -o car
-> gcc passenger.c -o passenger

Generate Resources:

-> ./coaster

Start the car:

-> ./car [number_of_rides]

*NOTES: The number of rides defines how many times the car process will run

Add people:

-> ./passenger [number_of_people]

*NOTES: To change the max number of people in the car, go to header.h and 
	change the definition of MAX to any number you'd like.

	The number of people should be preferrably larger than the max number
	of people in the car to see it working.
