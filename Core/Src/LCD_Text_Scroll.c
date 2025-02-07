#include "main.h"

ScrollState_Typedef ScrollState;

// Helper function to initialize the scroll state
void Initialize_ScrollState(ScrollState_Typedef *state, const char *text, int x_start, int y_start, int width, int height, int num_segments_to_show)
 {
    state->text = strdup(text); // Duplicate the input string
    state->x_start = x_start;
    state->y_start = y_start;
    state->width = width;
    state->height = height;
    state->num_segments_to_show = num_segments_to_show;
    state->segment_start = 0;
    state->delay_count = 0;
    state->active = true;

    // Count the total number of segments (IDs) in the text
    state->total_segments = 0;
    for (int i = 0; i < strlen(text); i++) {
        if (text[i] == ',' || text[i + 1] == '\0') {
            state->total_segments++;
        }
    }
}

// Helper function to free scroll state resources
void Free_ScrollState(ScrollState_Typedef *state) 
{
    if (state->text) {
        free(state->text);
        state->text = NULL;
    }
}

void Scroll_Text_StateMachine(ScrollState_Typedef *state, int update_interval_ms) 
{
    if (!state->active) return;

    // Increment delay count
    state->delay_count += update_interval_ms;

    // Update only if the delay exceeds the interval
    if (state->delay_count >= 2) // 2 seconds in ms
    {
        state->delay_count = 0;

        // Dynamically allocate buffer
        char *buffer = (char *)malloc((state->width + 1) * sizeof(char));
        if (buffer == NULL) return;

        memset(buffer, ' ', state->width);
        buffer[state->width] = '\0';

        int buffer_index = 0;
        int current_start = state->segment_start;
        int segment_count = 0;
        int segment_len = 0;

        while (current_start < strlen(state->text) && segment_count < state->num_segments_to_show) 
        {
            segment_len = 0;
            while (current_start + segment_len < strlen(state->text) && state->text[current_start + segment_len] != ',') {
                segment_len++;
            }

            if (current_start + segment_len < strlen(state->text) && state->text[current_start + segment_len] == ',') {
                segment_len++;
            }

            if (buffer_index + segment_len <= state->width) {
                strncpy(&buffer[buffer_index], &state->text[current_start], segment_len);
                buffer_index += segment_len;
                if (buffer_index < state->width) {
                    buffer[buffer_index++] = ' ';
                }
                segment_count++;
            } 
            else break;

            current_start += segment_len;
        }

        buffer[buffer_index] = '\0';

        Clear_Area(state->x_start, (state->y_start - 3), 128, state->y_start + state->height);
        LCDWrite_Text(state->x_start, state->y_start, buffer);
        
        uint16_t Len = strlen(state->text);
        if ((state->segment_start + segment_len < Len) && (Len > 10))
            state->segment_start += segment_len;
        else state->segment_start = 0;

        free(buffer);
    }
}
