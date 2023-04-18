#include "UserDatabase.h"

#include <iostream>

using namespace std;

UserDatabase::UserDatabase()
{
    //initialize files and movies
    m_files = unordered_set<string>();
    m_allUsers = unordered_map<string, User*>();

    m_userTree = new TreeMultimap<std::string, User*>();
}

UserDatabase::~UserDatabase()
{
    unordered_map<string, User*>::iterator it = m_allUsers.begin();
    while(it != m_allUsers.end())
    {
        delete it->second;
        it++;
    }
}

bool UserDatabase::load(const string& filename)
{
    if (m_files.empty() || m_files.find(filename) == m_files.end())
    {
        //initialize the file variables
        m_files.insert(filename);
        m_currentFile = filename;
        std::ifstream file (m_currentFile);

        //check if the file was successfully opened
        if (file.is_open())
        {
            load_helper();
            return true;
        }
    }

    return false; 
}

void UserDatabase::load_helper()
{
    //loop through every line in file
    std::ifstream file (m_currentFile);
    for(;;)
    {
            //every User has the following
        std::string name;
        std::string email;
        int numMovies;
        string dumper; 
        std::vector<std::string> movies;
        
        std::getline(file, name); //get the name from the file
        std::getline(file, email); //get the email from the file
        std::getline(file, dumper); numMovies = std::stoi(dumper); //get string version of # of movies and covert to string

        if (numMovies > 0) //if they have at least 1 movie
        {
            std::getline(file, dumper);
            movies.push_back(dumper);
        }
        
        //collect the rest of the movies
        for (int i = 1; i < numMovies; i++)
        {
            std::getline(file, dumper);
            movies.push_back(dumper);
        }

        //create a new User and add it to the binary tree
        createUser_addToTree(name, email, movies);

        //catch empty line and check if end of file
        std::getline(file, dumper);
        if (file.eof())
        {
            break;
        }
    }

}

void UserDatabase::createUser_addToTree(const std::string& name, const std::string& email, const std::vector<std::string>& movies)
{
    User* toAdd = new User(name, email, movies);

    //a user is identified by their distinct email
    m_userTree->insert(email, toAdd);

    //add the user to the map of users
    m_allUsers.insert(make_pair(email, toAdd));
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string, User*>::Iterator it = m_userTree->find(email);

    if (it.is_valid())
    {
        //a reference to the User that it points at
        return (it.get_value());
    }

    return nullptr;
}
