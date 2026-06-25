#ifndef LUMINAFRAME_INPUT_H
#define LUMINAFRAME_INPUT_H

/**
 * @brief Input event type.
 */
typedef enum{
    /* No valid input read */
    INPUT_EVENT_NONE = 0,

    /* Read a valid character command */
    INPUT_EVENT_KEY     ,

    /* Input channel ended or input read failed */
    INPUT_EVENT_QUIT    ,
} InputEventType ;


/**
 * @brief Input event object.
 *
 * The first version only supports keyboard-like input from terminal.
 * Future versions may add touch coordinates, button id, gesture type,
 * or device-specific data.
 */
typedef struct{
    /** Event type. */
    InputEventType type;

    /** Command key. Valid only when type is INPUT_EVENT_KEY. */
    char key;
} InputEvent ;

/**
 * @brief Initialize input module.
 *
 * @return 0 on success, -1 on failure.
 */
int input_init(void);

/**
 * @brief Read one input event.
 *
 * Read one line from stdin and extract the first non-space character
 * as command key.
 *
 * @return Input event.
 */
InputEvent input_read_event(void);

/**
 * @brief Close input module.
 */
void input_close(void);

#endif /* LUMINAFRAME_INPUT_H */
