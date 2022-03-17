#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "calendar.h"

char *string_from_Calendar(Calendar *cal)
{
    return NULL;
}

char *string_from_event(event *e)
{
    return NULL;
}

event *event_from_string(char *s)
{
    return NULL;
}

Calendar *add_event(Calendar *cal, event *e)
{
    event *ptr = cal->head;
    while (ptr->next) {
        if (ptr->name == e->name)
            return NULL;
        ptr = ptr->next;
    }
    ptr->next = e;
    e->prev = ptr;
    cal->count++;
    return cal;
}

event *create_event(char *name, char *date, char *time,
    char *duration, char *location, char* identifier)
{
    if (!(name && date && time && duration && location && identifier))
        return NULL;
    event *new_event = malloc(sizeof(event));
    new_event->name = name;
    new_event->date = date;
    new_event->time = time;
    new_event->duration = duration;
    new_event->location = location;
    new_event->identifier = identifier;
    return new_event;
}

int remove_event(Calendar *cal, char *event_name)
{
    event *ptr = cal->head;
    while (strcmp(ptr->name,event_name)!=0);
    if (!ptr)
        return 1;
    event *temp = ptr->next;
    ptr->prev->next = ptr->next;
    if (ptr->next)
        ptr->next->prev = ptr->prev;
    free(ptr);
    cal->count--;
    return 0;
}

// Rememver to free the event array!
event** get_events_by_date(Calendar *cal, char* date)
{
    /*
    First, find how many events we have, then create an malloc an event array to
    hold them, then add them. That's C for ya
    */
    int count = 0;
    event *ptr = cal->head;
    // No events
    if (!ptr)
        return NULL;
    // go though and find events that count
    while (ptr->next){
        if (strcmp(ptr->name, date)){
            count++;
        }
    }
    // we found none
    if (count == 0)
        return NULL;
    int index = 0;
    // create array for events (should be FREEED!)
    event **events = (event**)malloc(count * sizeof(event));
    while (ptr->next){
        if (strcmp(ptr->name, date)){
            // add our event
            events[index] = ptr;
            index++;
        }
    }
    return events;
}

event** get_events_by_range(Calendar *cal, char* start_date, char* end_date)
{
    /*
    Literally just the same as the by date, but with the helper
    */
    int count = 0;
    event *ptr = cal->head;
    // No events
    if (!ptr)
        return NULL;
    // go though and find events that count
    while (ptr->next){
        if (in_date_range(start_date, end_date, ptr->date)){
            count++;
        }
    }
    // we found none
    if (count == 0)
        return NULL;
    int index = 0;
    // create array for events (should be FREEED!)
    event ** events = (event**)malloc(count * sizeof(event));
    while (ptr->next){
        if (in_date_range(start_date, end_date, ptr->date)){
            // add our event
            events[index] = ptr;
            index++;
        }
    }
    return events;
}

int in_date_range(char* start, char* end, char *date)
{
    /*
        Dates are 6 numbers: month, day, year
        (if you follow the layers of abstractions far enough,
            the code will be trash)
    */
    // within year range
    if (!(date[4] >= start[4] && date[5] >= start[5] &&
          date[4] <= end[4]   && date[5] <= end[5]   )){
        return 1;
    }
    // month range
    if (!(date[2] >= start[2] && date[3] >= start[3] &&
          date[2] <= end[2]   && date[3] <= end[3]   )){
        return 1;
    }
    // day range
    if (!(date[0] >= start[0] && date[1] >= start[1] &&
          date[0] <= end[0]   && date[1] <= end[1]   )){
        return 1;
    }
    return 0;
}

int is_loaded(Calendar *cal){
    if (cal->file){
        return 1;
    }
    return 0;
}

/*

The following code is for dealing with the data on disk (in /data)

Each calendar is in its own file, and is loaded into a linked list of events
called a calendar. It is a linked list because that is the easist DS for me
to impliment in C. There is no other reason.

*/
Calendar *load_calendar(char *file_path, char *name)
{
    FILE *fp = open(file_path, "r");
    if (!fp)
        return NULL;

    Calendar *cal = (Calendar *)malloc(sizeof(Calendar));
    cal->name = name;
    cal->file = fp;
    cal->count = 0;
    cal->head = NULL;

    char *request = NULL;
    size_t read;
    size_t len = 0;
    while ((read = getline(&request, &len, fp)) != -1)
    {
        printf("Adding request from file %s to calendar %s:\n", file_path, name);
        printf("%s", request);
        if (process_request(request, cal) != 0)
            printf("ADD FAILED");
    }

    return cal;
}

/*
    Takes a raw string request and adds it to a calendar
    (used primarily for load operation)
*/
Calendar* process_edit_request(char* request, char *type, Calendar *cal)
{
    // add event
    switch (type):
        case "ADD":
            event *e = event_from_string(request);
            if (add_event(cal, e) != 0)
                return NULL;
        case "REMOVE":
            char *event_id = NULL;
            remove_event(cal, event_id);
        case "UPDATE":
            // update event
            // remove, then add

    return cal;
}

int add_request(char* request, FILE *fp)
{

}
