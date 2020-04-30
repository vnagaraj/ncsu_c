/** ballistics table generator program.
    @file ballistics.c
    @author Vivekanand Ganapathy Nagarajan vganapa
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** Gravitation constant used to calculate flight time. */
#define GRAVITATIONAL_ACCELERATION 9.81

/** Angle increment per step. */
#define ANGLE_INCREMENT 5

/** Number of rows to display. */
#define ROW_LIMIT 19

/** Converts the angle from degrees to radian.
  * @param degree The angle in degrees.
  * @return The angle in radian.
  */
double degreeToRadian(int degree)
{
  return degree * M_PI/180;
}

/** Read the initial velocity from user input.
  * @return The initial velocity.
  */
double readUserInput()
{
  double velocity=0;
  int match = scanf( "%lf", &velocity);
  if (match != 1){
    exit(EXIT_FAILURE);
  }
  return velocity;
}

/** Print the table header.
  * @return the initial velocity.
  */
void printTableHeader()
{
  printf("V0: \n");
  printf("     angle |         v0 |       time |   distance\n");
  printf("-----------+------------+------------+-----------\n");
}

/** Return the time the projectile is in the air.
  * @param angle The angle in degrees.
  * @param v0 The initial velocity in meters/second.
  * @return The flight time in seconds.
  */
double flightTime( int angle, double v0 )
{
  return (2 * v0 * sin(degreeToRadian(angle)))/GRAVITATIONAL_ACCELERATION;
}

/** Print out the row of the given table.
  * @param angle The angle in degrees.
  * @param v0 The initial velocity in meters/second.
  * @param t The flight time in seconds.
  */
void tableRow( int angle, double v0, double t )
{
  double distance= v0*t*cos(degreeToRadian(angle));
  printf("%10d | %10.3f | %10.3f | %10.3f\n", angle, v0, t, distance);
}

/** Starting point of the program.
  * @return The exit status.
  */
int main()
{
  double velocity = readUserInput();
  printTableHeader();
  for (int angle=0; angle< (ROW_LIMIT*ANGLE_INCREMENT); angle+=ANGLE_INCREMENT){
    tableRow(angle, velocity, flightTime(angle, velocity));
  }
  return EXIT_SUCCESS;
}
