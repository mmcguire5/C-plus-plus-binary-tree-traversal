// ============================================================================
// BinaryTree.cpp
// ~~~~~~~~~~~~~~
// hqn & Michael McGuire
// - Implementation of a Very Basic Binary Tree Class
// - Illustration of how to use deque
// - Common traversal order
// ============================================================================

#include <algorithm> // for sort()
#include <stdexcept>
#include <deque>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "BinaryTree.h"
#include "term_control.h"

using namespace std; // BAD PRACTICE

BinaryTree::BinaryTree()
{
    root_ = NULL;
}

BinaryTree::BinaryTree(vector<string>& a, vector<string>& b)
{
    root_ = NULL;
    setTree(a, b);
}

BinaryTree::~BinaryTree()
{
    clearTree(root_);
    root_ = NULL;
}

void BinaryTree::clearTree(Node* node)
{
    if (node != NULL) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}


bool BinaryTree::isEmpty() const
{
    return root_ == NULL;
}

string BinaryTree::levelOrderSequence()
{
    ostringstream oss;
    if (root_ != NULL) {
        deque<Node*> nodeQ;
        nodeQ.push_front(root_);
        while (!nodeQ.empty()) {
            Node* cur = nodeQ.back();
            nodeQ.pop_back();
            if (cur->left != NULL)
                nodeQ.push_front(cur->left);
            if (cur->right != NULL)
                nodeQ.push_front(cur->right);
            oss << cur->payload << " ";
        }
    }
    return oss.str();
}

string BinaryTree::inOrderSequence()
{
    return inOrderSequence(root_);
}

string BinaryTree::preOrderSequence()
{
    return preOrderSequence(root_);
}

string BinaryTree::postOrderSequence()
{
    return postOrderSequence(root_);
}

string BinaryTree::inOrderSequence(Node* node)
{
    ostringstream oss;
    if (node != NULL) {
        oss << inOrderSequence(node->left)
            << node->payload << " "
            << inOrderSequence(node->right);
    }
    return oss.str();
}

string BinaryTree::postOrderSequence(Node* node)
{
    ostringstream oss;
    if (node != NULL) {
        oss << postOrderSequence(node->left)
            << postOrderSequence(node->right)
            << node->payload << " ";
    }
    return oss.str();
}

string BinaryTree::preOrderSequence(Node* node)
{
    ostringstream oss;
    if (node != NULL) {
        oss << node->payload << " "
            << preOrderSequence(node->left)
            << preOrderSequence(node->right);
    }
    return oss.str();
}

bool BinaryTree::setTree(vector<string>& a, vector<string>& b)
{
    if (!isPermutation(a,b))
        return false;
    this->~BinaryTree();
    root_ = constructTree(a, 0, b, 0, a.size());
    return true;
}

BinaryTree::Node* BinaryTree::constructTree(
        vector<string>& a, size_t a_start,
        vector<string>& b, size_t b_start,
        size_t len)
{
    if (len == 0)
        return NULL;

    Node* node = new Node(a[a_start]); // root_ of new tree

    size_t i;
    for (i=0; i<len; i++) {
        if (b[b_start+i].compare(a[a_start]) == 0)
            break;
    }

    node->left  = constructTree(a, a_start+1, b, b_start, i);
    node->right = constructTree(a, a_start+i+1, b, b_start+i+1, len-i-1);

    return node;
}

bool BinaryTree::isPermutation(vector<string> a, vector<string> b)
{
    if (a.size() != b.size())
        return false;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    for (size_t i=0; i<a.size(); i++)
        if (a[i].compare(b[i]) != 0)
            return false;

    return true;
}

string BinaryTree::nodeString(BinaryTree::Node *node)
{
    if (node == NULL)
        return "x";
    else
        return node->payload;
}

// vertical print one node and call recursively for its children
void BinaryTree::verticalRecursive(ostringstream &oss, BinaryTree::Node *node,
        string prefix, bool isLeftChild)
{
    // pre-order right then left

    // print node
    if (!prefix.empty())
        oss << prefix << "__";
    oss << nodeString(node);
    oss << endl;

    // replace last '|' with ' ' for nodes that are left children of their
    // parents
    if (isLeftChild)
        prefix = prefix.substr(0, prefix.length()-1) + " ";

    // call recursively for children
    string newprefix;
    if (prefix.empty())
        newprefix = "|";
    else
        newprefix = prefix + "  |";
    if (node != NULL && (node->left != NULL || node->right != NULL)) {
        verticalRecursive(oss, node->right, newprefix, false);
        // print empty line between left and right children
        oss << newprefix << endl;
        verticalRecursive(oss, node->left, newprefix, true);
    }
}

string BinaryTree::vertical()
{
    ostringstream oss;
    verticalRecursive(oss, root_, "", false);
    return oss.str();
}

// compute width for horizontal printing
int BinaryTree::computeHorWidth(BinaryTree::Node *node, int offset,
        int level)
{
    // empty node, it displays 'x'
    if (node == NULL)
        return 1;

    else {
        node->horOffset = offset;
        node->level = level;
        if (node->left != NULL || node->right != NULL) {
            // compute width of left child
            node->horLeftWidth = computeHorWidth(node->left, offset,
                    level + 1);
            if ((int) node->payload.length() >= node->horLeftWidth)
                node->horLeftWidth = node->payload.length();
            else
                node->horLeftWidth += 2; // add two more underscores
            int horRightWidth = computeHorWidth(node->right, offset +
                    node->horLeftWidth + 1, level + 1);
            node->horWidth = node->horLeftWidth + 1 + horRightWidth;
        }
        else
            node->horWidth = node->payload.length();

        return node->horWidth;
    }
}

string BinaryTree::horizontal()
{
    // compute width and offset for nodes
    // also store node levels
    computeHorWidth(root_, 0, 0);

    // level-order traversal
    ostringstream oss;
    int level = -1;
    if (root_ != NULL) {
        deque<Node*> nodeQ1, nodeQ2;
        nodeQ1.push_front(root_);
        nodeQ2.push_front(root_);

        int offs = 0;
        while (!nodeQ1.empty()) {
            Node* cur = nodeQ1.back();
            nodeQ1.pop_back();

            // new level
            if (cur->level > level && level >= 0) {
                oss << endl;
                offs = 0;

                // need to draw the lines, use nodeQ2 for this
                while (nodeQ2.back()->level == level) {
                    Node *c2 = nodeQ2.back();
                    nodeQ2.pop_back();

                    // move to offset
                    for (int i = offs; i < c2->horOffset; i++)
                        oss << " ";

                    offs = c2->horOffset;
                    if (c2->left != NULL || c2->right != NULL) {
                        oss << "|";
                        offs++;
                        for (int i = offs; i < c2->horOffset +
                                c2->horLeftWidth; i++)
                            oss << " ";
                        oss << "\\";
                        offs = c2->horOffset + c2->horLeftWidth + 1;
                    }

                    // free temporary 'fake' nodes
                    if (c2->payload.length() == 0)
                        free(c2);
                }
                oss << endl;
                offs = 0;
            }
            level = cur->level;

            for (int i = offs; i < cur->horOffset; i++)
                oss << " ";
            offs = cur->horOffset;
            if (cur->payload.length() > 0) {
                oss << cur->payload;
                offs += cur->payload.length();
            }
            else {
                oss << "x";
                offs++;
            }

            // process children
            if (cur->left != NULL || cur->right != NULL) {
                // draw underscores if needed
                for (int i = offs; i < cur->horOffset + cur->horLeftWidth;
                        i++)
                    oss << "_";
                offs = cur->horOffset + cur->horLeftWidth;

                // call recursively for children
                if (cur->left == NULL) {
                    // add 'fake' children for null nodes that need to be
                    // printed
                    BinaryTree::Node *n = new Node;
                    n->level = cur->level + 1;
                    n->horOffset = cur->horOffset;
                    n->horWidth = 1;
                    nodeQ1.push_front(n);
                    nodeQ2.push_front(n);
                }
                else {
                    nodeQ1.push_front(cur->left);
                    nodeQ2.push_front(cur->left);
                }
                if (cur->right == NULL) {
                    BinaryTree::Node *n = new Node;
                    n->level = cur->level + 1;
                    n->horOffset = cur->horOffset + cur->horLeftWidth + 1;
                    n->horWidth = 1;
                    nodeQ1.push_front(n);
                    nodeQ2.push_front(n);
                }
                else {
                    nodeQ1.push_front(cur->right);
                    nodeQ2.push_front(cur->right);
                }
            }
        }

        // free temporary 'fake' nodes on the last level
        while (!nodeQ2.empty()) {
            Node *c2 = nodeQ2.back();
            nodeQ2.pop_back();

            if (c2->payload.length() == 0)
                free(c2);
        }

        oss << endl;
    }
    return oss.str();
}

// compute width for symmetric printing
void BinaryTree::computeSymWidth(BinaryTree::Node *node, int level)
{
    node->level = level;

    // see how children fit
    if (node->left != NULL || node->right != NULL) {
        int ll, lr, rl, rr;
        if (node->left == NULL) {
            ll = 0;
            lr = 1;
        }
        else {
            computeSymWidth(node->left, level + 1);
            ll = node->left->symLeftWidth;
            lr = node->left->symRightWidth;
        }
        if (node->right == NULL) {
            rl = 0;
            rr = 1;
        }
        else {
            computeSymWidth(node->right, level + 1);
            rl = node->right->symLeftWidth;
            rr = node->right->symRightWidth;
        }

        node->symTitleWidth = lr - 1 + rl - 1;
        if (node->symTitleWidth < (int) node->payload.length())
            node->symTitleWidth = node->payload.length();
        node->symLeftWidth = ll + 2 + node->symTitleWidth / 2;
        node->symRightWidth = node->symTitleWidth - node->symTitleWidth / 2 +
                1 + rr;
    }

    // no children, node is centered
    else {
        node->symTitleWidth = node->payload.length();
        node->symLeftWidth = node->symTitleWidth/2;
        node->symRightWidth = node->symTitleWidth - node->symTitleWidth/2;
    }
}

// compute offset for symmetric printing
void BinaryTree::computeSymOffset(BinaryTree::Node *node, int offset)
{
    node->symOffset = offset;
    node->symTitleOffset = node->symOffset + node->symLeftWidth -
            node->symTitleWidth/2;

    // any children?
    if (node->left != NULL || node->right != NULL) {
        if (node->left != NULL)
            computeSymOffset(node->left, offset);
        if (node->right != NULL)
            computeSymOffset(node->right, node->symTitleOffset +
                    node->symTitleWidth + 1 - node->right->symLeftWidth);
    }
}

string BinaryTree::symmetric()
{
    if (root_ != NULL) {
        computeSymWidth(root_, 0);
        computeSymOffset(root_, 0);
    }

    // level-order traversal
    ostringstream oss;
    int level = -1;
    if (root_ != NULL) {
        deque<Node*> nodeQ1, nodeQ2;
        nodeQ1.push_front(root_);
        nodeQ2.push_front(root_);

        int offs = 0;
        while (!nodeQ1.empty()) {
            Node* cur = nodeQ1.back();
            nodeQ1.pop_back();

            // new level
            if (cur->level > level && level >= 0) {
                oss << endl;
                offs = 0;

                // need to draw the lines, use nodeQ2 for this
                while (nodeQ2.back()->level == level) {
                    Node *c2 = nodeQ2.back();
                    nodeQ2.pop_back();

                    // free temporary 'fake' nodes
                    if (c2->payload.length() == 0)
                        free(c2);
                    else if (c2->left != NULL || c2->right != NULL) {
                        // move to offset
                        for (int i = offs; i < c2->symTitleOffset-1; i++)
                            oss << " ";
                        oss << "/";
                        offs = c2->symTitleOffset;
                        for (int i = 0; i < c2->symTitleWidth; i++)
                            oss << " ";
                        oss << "\\";
                        offs += c2->symTitleWidth+1;
                    }
                }
                oss << endl;
                offs = 0;
            }
            level = cur->level;

            // print title
            for (int i = offs; i < cur->symTitleOffset; i++)
                oss << " ";
            string pl = cur->payload;
            if (pl.length() == 0)
                pl = "x";
            int delta = cur->symTitleWidth - pl.length();
            for (int i = 0; i < delta/2; i++)
                oss << "_";
            /*
            if (pl.length() % 2 == 1 && cur->symTitleWidth % 2 == 0) {
                oss << "_";
                delta -= 2;
            }
            */
            oss << pl;
            for (int i = 0; i < delta - delta/2; i++)
                oss << "_";
            offs = cur->symTitleOffset + cur->symTitleWidth;

            // process children
            if (cur->left != NULL || cur->right != NULL) {
                // call recursively for children
                if (cur->left == NULL) {
                    // add 'fake' children for null nodes that need to be
                    // printed
                    BinaryTree::Node *n = new Node;
                    n->level = cur->level + 1;
                    n->symTitleWidth = 1;
                    n->symTitleOffset = cur->symOffset;
                    nodeQ1.push_front(n);
                    nodeQ2.push_front(n);
                }
                else {
                    nodeQ1.push_front(cur->left);
                    nodeQ2.push_front(cur->left);
                }
                if (cur->right == NULL) {
                    BinaryTree::Node *n = new Node;
                    n->level = cur->level + 1;
                    n->symTitleWidth = 1;
                    n->symTitleOffset = cur->symTitleOffset +
                            cur->symTitleWidth + 1;
                    nodeQ1.push_front(n);
                    nodeQ2.push_front(n);
                }
                else {
                    nodeQ1.push_front(cur->right);
                    nodeQ2.push_front(cur->right);
                }
            }
        }

        // free temporary 'fake' nodes on the last level
        while (!nodeQ2.empty()) {
            Node *c2 = nodeQ2.back();
            nodeQ2.pop_back();

            if (c2->payload.length() == 0)
                free(c2);
        }

        oss << endl;
    }
    return oss.str();
}
