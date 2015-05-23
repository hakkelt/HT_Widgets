#include "node.h"

using namespace std;

void node::add(widget *newMember)
{
    members.push_back( newMember );
}

widget &node::operator[] (int index)
{
    return *(members[index]);
}

node& operator<< (node &G, widget *newMember)
{
    G.add(newMember);
    return G;
}
