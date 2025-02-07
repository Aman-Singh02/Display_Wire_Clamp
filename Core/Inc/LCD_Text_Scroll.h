#ifndef __LCD_TEXT_SCROLL_H
#define __LCD_TEXT_SCROLL_H

// Define a structure to manage the scrolling state
typedef struct 
{
    char *text;               // Text to scroll
    int x_start;              // Starting x position
    int y_start;              // Starting y position
    int width;                // Width of the display area
    int height;               // Height of the display area
    int num_segments_to_show; // Number of segments to display at a time
    int segment_start;        // Current start segment index
    int delay_count;          // Timer count for periodic updates
    int total_segments;       // Total segments in the text
    bool active;              // Flag to indicate if scrolling is active
} ScrollState_Typedef;

extern ScrollState_Typedef ScrollState;

void Initialize_ScrollState(ScrollState_Typedef *state, const char *text, int x_start, int y_start, int width, int height, int num_segments_to_show);
void Free_ScrollState(ScrollState_Typedef *state);
void Scroll_Text_StateMachine(ScrollState_Typedef *state, int update_interval_ms);

#endif // __LCD_TEXT_SCROLL_H