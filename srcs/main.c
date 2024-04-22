#include <ncurses.h>
#include <cmath>

#define ITER 42

// Function to calculate whether a point belongs to the Julia set
int julia(double x, double y) {
    double z_real = x, z_imag = y;
    double c_real = -0.7, c_imag = 0.27015; // Constants for the Julia set
    int iteration = 0;
    
    while (z_real * z_real + z_imag * z_imag <= 4 && iteration < ITER) {
        double temp = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2 * z_real * z_imag + c_imag;
        z_real = temp;
        iteration++;
    }
    
    if (iteration == ITER) 
        return 0;
    else
        return iteration;
}

float x_pos = 0, y_pos = 0;
float zoom = 1;

// Function to draw the Julia set on the terminal window
void draw() {
    int x, y;
    getmaxyx(stdscr, y, x);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            double x_coord = (((double)i - (double)x / 2.) * (4. / (double)x)) * zoom + x_pos;
            double y_coord = y_pos + zoom * ((double)j - (double)y / 2.) * (4. / (double)y * ((2.5 * 9.) / 16.));
            
            int result = julia(x_coord, y_coord);

            // Color mapping
            if (result == 0)
                attron(COLOR_PAIR(1));
            else if (result % 7 == 1)
                attron(COLOR_PAIR(2));
            else if (result % 7 == 2)
                attron(COLOR_PAIR(3));
            else if (result % 7 == 3)
                attron(COLOR_PAIR(4));
            else if (result % 7 == 4)
                attron(COLOR_PAIR(5));
            else if (result % 7 == 5)
                attron(COLOR_PAIR(6));
            else
                attron(COLOR_PAIR(7));
            
            // Display character
            if (result == 0)
                mvprintw(j, i, "#");
            else
                mvprintw(j, i, " ");

            // Turn off color pairs
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(6));
            attroff(COLOR_PAIR(7));
        }
    }

    refresh();
}

int main(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    
    // Initializing color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // Inside Julia set
    init_pair(2, COLOR_BLACK, COLOR_BLUE);    // Deep blue
    init_pair(3, COLOR_BLACK, COLOR_CYAN);    // Cyan
    init_pair(4, COLOR_BLACK, COLOR_GREEN);   // Green
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);  // Yellow
    init_pair(6, COLOR_BLACK, COLOR_RED);     // Red
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA); // Magenta
    
    draw(); // Initial drawing
    
     while (1)
    {
        int ch = getch();
        switch (ch) {
            case 'w':
                y_pos -= 0.15 * zoom;
                break;
            case 's':
                y_pos += 0.15 * zoom;
                break;
            case 'a':
                x_pos -= 0.15 * zoom;
                break;
            case 'd':
                x_pos += 0.15 * zoom;
                break;
            case 'q':
                zoom *= 0.9;
                break;
            case 'e':
                zoom /= 0.9;
                break;
            case ' ':
                endwin();
                return 0;
                break;
        }
        draw();
    }
    return 0;
}
