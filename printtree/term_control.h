// ============================================================================
// term_control.h
// ~~~~~~~~~~~~~~~~
// author: hqn
// may 04, 2014
// - a few terminal control routines for pretty output printing
// ============================================================================

#ifndef TERM_CONTROL_H_
#define TERM_CONTROL_H_

#include <string>

// the attributes
enum term_attrib_t {
    DEFAULT_ATTRIB = '0',
    BRIGHT    = '1',
    DIM       = '2',
    UNDERLINE = '4',
    BLINK     = '5',
    REVERSE   = '7',
    HIDDEN    = '8'
};

// the colors, background or foreground
enum term_colors_t {
    BLACK   = '0',
    RED     = '1',
    GREEN   = '2',
    YELLOW  = '3',
    BLUE    = '4',
    MAGENTA = '5',
    CYAN    = '6',
    WHITE   = '7',
    DEFAULT_COLOR = '9'
};


/**
 * outputs the corresponding terminal control escape sequence (TCES)
 * that sets the text foreground, background, and attribute requested
 * by the three arguments
 *
 * usage:
 * cout << term_cc(YELLOW, BLUE) 
 *      << "This text is yellow on a blue background\n" << term_cc();
 */
std::string term_cc(term_colors_t fg=DEFAULT_COLOR, 
                    term_colors_t bg=DEFAULT_COLOR, 
                    term_attrib_t attr=DEFAULT_ATTRIB);

/**
 * similar to term_cc() but only changes the forground color
 */
std::string term_fg(term_colors_t fg=DEFAULT_COLOR);

/**
 * similar to term_cc() but only changes the background color
 */
std::string term_bg(term_colors_t fg=DEFAULT_COLOR);

/**
 * similar to term_cc() but only changes the attribute
 */
std::string term_attrib(term_attrib_t attr=DEFAULT_ATTRIB);

/**
 * term_clear() outputs the TCES which clears the screen
 */
std::string term_clear();

#endif // end #ifndef TERM_CONTROL_H_
