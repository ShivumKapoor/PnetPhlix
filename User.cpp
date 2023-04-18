#include "User.h"

#include <iostream>
using namespace std;

User::User(const string& full_name, const string& email, const vector<string>& watch_history)
{
    //initialize member variables
    m_full_name = full_name;
    m_email = email;
    for (int i = 0; i < watch_history.size(); i++)
    {
        m_watch_history.push_back(watch_history[i]);
    }
}

string User::get_full_name() const
{
    return m_full_name;
}

string User::get_email() const
{
    return m_email;
}

vector<string> User::get_watch_history() const
{
    return m_watch_history;
}
