// ============================================================================
// error_handling.cpp
// ~~~~~~~~~~~~~~~~~~
// author: hqn
// may 04, 2014
// - error reporting functions
// ============================================================================
#include <string>
#include <iostream>
#include <stdlib.h>       // for exit()
#include "term_control.h"
#include "error_handling.h"

using std::cerr;
using std::endl;
using std::string;

void error_quit(string msg) 
{
    cerr << term_cc(RED) << "   FATAL ERROR: " << msg << endl << term_cc();
    exit(1); // exit on error
}

void error_return(string msg) 
{
    cerr << term_cc(RED) << "** ERROR **\n" << msg << endl << term_cc();
}

void print_warning(string msg) 
{
    cerr << term_cc(YELLOW) << "== Warning ==\n" << msg << endl << term_cc();
}

void note(string msg) 
{
    cerr << term_cc(MAGENTA) << "-- Note --\n" << msg << endl << term_cc();
}

void report(string msg) 
{
    cerr << term_cc(CYAN) << msg << endl << term_cc();
}
