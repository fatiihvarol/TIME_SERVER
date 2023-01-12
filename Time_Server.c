#include <stdio.h>
#include <string.h> // for strlen

#include <unistd.h> // for write
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 60000

int main(int argc, char *argv[])
{

  time_t t;
  struct tm tm;
  int socket_desc, new_socket, c;
  struct sockaddr_in server, client;
  char *message;
  char *gettimecommand = "GET_TIME";
  char *getdatecommand = "GET_DATE";
  char *gettimedate = "GET_TIME_DATE";
  char *getwday = "GET_DAY_OF_WEEK";
  char *gettimezone = "GET_TIME_ZONE";
  char *getquitinfo = "CLOSE_SERVER";
  char *dayinfo;

  char timeString[80];
  time_t timeT = time(NULL);
 
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    puts("Could not create socket");
    return 1;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    puts("Bind failed");
    return 1;
  }
  puts("Socket is binded");

  // Listen
  listen(socket_desc, 3);

  // Accept and incoming connection
  puts("Waiting for incoming connections...");

  c = sizeof(struct sockaddr_in);
  char input[50];
  char timee[40] = "";
 //for new socket
  new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
  puts("Connection accepted");

 //main loop 
  while (1)
  {

    bzero(input, 50);//clear input array
    read(new_socket, input, sizeof(input));//input from users

    for (int i = 0; i < 50; i++)//input variables comes with \r\n that's why we try to delete them here
    {
      if (input[i] == '\r')
      {
        input[i] = '\0';
      }
      else if (input[i] == '\n')
      {
        input[i] = '\0';
      }
      else if (input[i] == '\0')
      {
        break;
      }
    }

    t = time(NULL);
    tm = *localtime(&t);
    if (strcmp(input, gettimecommand) == 0)//FOR GET TIME REQUEST
    {
      sprintf(timee, "%s%02d%s%02d%s%02d\n", timee, tm.tm_hour, ":", tm.tm_min, ":", tm.tm_sec);//combine the hour,min,sec
      write(new_socket, timee, 40);//writing 
      bzero(timee, 40);//clear time array
    }
    else if (strcmp(input, getdatecommand) == 0)//FOR GET DATE REQUEST
    {
      sprintf(timee, "%s%d%s%d%s%d\n", timee, tm.tm_mday, ":", tm.tm_mon + 1, ":", tm.tm_year + 1900);//for combining month day and year
      write(new_socket, timee, 40);//writing 
      bzero(timee, 40);//clear time array
    }
    else if (strcmp(input, gettimedate) == 0)//FOR GET TIME AND DATE REQUEST
    {
      //for combining minute ,second, hour and day ,month ,year variables.
      sprintf(timee, "%s%02d%s%02d%s%02d%s%d%s%d%s%d\n", timee, tm.tm_hour, ":", tm.tm_min, ":", tm.tm_sec, " ", tm.tm_mday, ":", tm.tm_mon + 1, ":", tm.tm_year + 1900);

      write(new_socket, timee, sizeof(timee));//writing 
      bzero(timee, 40);//clear time array
    }
    else if (strcmp(input, getwday) == 0)//FOR GET DAY OF WEEK REQUEST
    {
      int i = tm.tm_wday;
    //in the time.h library it return 0 to 6 integer number for day of week that's why we are equaling the string for every integer between 0-6
      if (i == 0)
      {
        dayinfo = "SUNDAY\n";
      }
      else if (i == 1)
      {
        dayinfo = "MONDAY\n";
      }
      else if (i == 2)
      {
        dayinfo = "TUESDAY\n";
      }
      else if (i == 3)
      {
        dayinfo = "WEDNESDAY\n";
      }
      else if (i == 4)
      {
        dayinfo = "THURSDAY\n";
      }
      else if (i == 5)
      {
        dayinfo = "FRIDAY\n";
      }
      else if (i == 6)
      {
        dayinfo = "SATURDAY\n";
      }

      write(new_socket, dayinfo, strlen(dayinfo));
      bzero(timee, 40);
    }

    else if (strcmp(input, gettimezone) == 0)//FOR GET TIME ZONE REQUEST
    {
      //we used localtime function for get to time zone
      strftime(timeString, sizeof(timeString), "%Z\n", localtime(&timeT));
      write(new_socket, timeString, strlen(timeString));
      bzero(timee, 40);
    }
    else if (strcmp(input, getquitinfo) == 0)//CLOSING SERVER REQUEST
    {
      message = "GOOD BYE\n";
      write(new_socket, message, strlen(message));
      break;
    }
    else//FOR ALL OTHER INCORRECT REQUESTS
    {
      message = "INCORRECT REQUEST\n";
      write(new_socket, message, strlen(message));
    }

    bzero(input, 50);//clear input array
  }

  return 0;
}
