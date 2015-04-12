// ============================================================================
// error_handling.h
// ~~~~~~~~~~~~~~~~
// author: hqn
// may 04, 2014
// - error reporting functions
// ============================================================================

#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_

void error_quit(std::string);    // report err msg and quit
void error_return(std::string);  // report err msg but doesn't quit
void print_warning(std::string); // print a warning
void note(std::string);          // print a note
void report(std::string);        // print a report

#endif
