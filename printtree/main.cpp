// ============================================================================
// main.cpp
// ~~~~~~~~
// hqn
// - read a tree, print in nice formats
// ============================================================================

#include <iostream>
#include <iterator>
#include <stdexcept>
#include <map>
#include <sstream>
#include <vector>
#include <cstdlib>   // for exit()

#include "BinaryTree.h"
#include "term_control.h"
#include "error_handling.h"

using namespace std; // BAD PRACTICE

typedef void (*cmd_t)();

// -----------------------------------------------------------------------------
// prototypes of some other functions used in this driver
// -----------------------------------------------------------------------------
void prompt() { cout << term_cc(BLUE) << "> " << term_cc() << flush; }
void new_tree();   // returns a new tree or throw an exception if syntax error
void preorder();   // print the current tree in preorder
void postorder();  // print the current tree in postorder
void inorder();    // print the current tree in inorder
void levelorder(); // print the current tree in levelorder
void vertical();   // print the current tree vertically
void horizontal(); // print the current tree horizontally
void symmetric();  // print the current tree symmetrically

void bye() { exit(0); } // simply quit

/**
 * global variable, new_tree() changes this variable
 */
BinaryTree tree;   

int main() 
{
    const string usage_msg = "UB Tree Program\n";

    cout << term_cc(YELLOW) << usage_msg << endl;

    map<string,cmd_t> cmd_map;
    cmd_map["newtree"]    = &new_tree;
    cmd_map["exit"]       = &bye;
    cmd_map["bye"]        = &bye;
    cmd_map["preorder"]   = &preorder;
    cmd_map["postorder"]  = &postorder;
    cmd_map["levelorder"] = &levelorder;
    cmd_map["inorder"]    = &inorder;
    cmd_map["ver"]        = &vertical;
    cmd_map["hor"]        = &horizontal;
    cmd_map["sym"]        = &symmetric;

    string cmd;
    while (cin) {
        prompt(); 
        getline(cin, cmd);
        if (cmd_map.find(cmd) != cmd_map.end()) {
            try {
                cmd_map[cmd]();
            } catch (runtime_error &e) {
                error_return(e.what());
            }
        } else {
            error_return(string("Unknown command ") + cmd);
        }
    }
    return 0;
}

vector<string> tokenize_line(string const& line)
{
    istringstream iss(line);
    istream_iterator<string> cur(iss), end;
    return vector<string>(cur, end);
}

// -----------------------------------------------------------------------------
// construct a new tree by reading two lines - a preorder line and an 
// inorder line
// -----------------------------------------------------------------------------
void new_tree() 
{
    string poline; // preorder line
    string ioline; // inorder line

    cout << term_cc(YELLOW) 
        << "Enter the preorder and inorder sequences for"
        << " your tree, use unique names\n";

    cout << term_cc(CYAN) << "PREor. seq.: " << term_cc();
    getline(cin, poline);
    cout << term_cc(CYAN) << "INor. seq. : " << term_cc();
    getline(cin, ioline);
    vector<string> povec = tokenize_line(poline);
    vector<string> iovec = tokenize_line(ioline);

    if (!tree.setTree(povec, iovec))
        throw runtime_error("Improper Preorder/Inorder Pair");
}

void levelorder() 
{
    cout << term_cc(CYAN) << tree.levelOrderSequence() << endl << term_cc();
}

void postorder() 
{
    cout << term_cc(CYAN) << tree.postOrderSequence() << endl << term_cc();
}

void preorder() 
{
    cout << term_cc(CYAN) << tree.preOrderSequence() << endl << term_cc();
}

void inorder() 
{
    cout << term_cc(CYAN) << tree.inOrderSequence() << endl << term_cc();
}

void symmetric() 
{
    cout << term_cc(CYAN) << tree.symmetric() << endl << term_cc();
}

void horizontal() 
{
    cout << term_cc(CYAN) << tree.horizontal() << endl << term_cc();
}

void vertical() 
{
    cout << term_cc(CYAN) << tree.vertical() << endl << term_cc();
}
