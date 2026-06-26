#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "input/input.h"

#define INPUT_BUFFER_SIZE 128


int input_init(void)
{
    return 0;
}

InputEvent input_read_event(void)
{
    char buf[INPUT_BUFFER_SIZE];
    InputEvent event;
    size_t i;

    event.type = INPUT_EVENT_NONE;
    event.key  = '\0'            ;

    if (NULL == fgets(buf,
                      sizeof(buf),
                 stdin)){

        event.type = INPUT_EVENT_QUIT;
        return event;
    }

    for (i = 0; i < strlen(buf); i++) {
        if (!isspace((unsigned char)buf[i])) {
            event.type = INPUT_EVENT_KEY;
            event.key  = buf[i];
            return event;
        }
    }

    return event;

}


void input_close(void)
{

}
