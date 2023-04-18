#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>

#include "User.h"
#include "treemm.h"

class User;

class UserDatabase
{
	public:
		UserDatabase();
		~UserDatabase();
		bool load(const std::string& filename);
		User* get_user_from_email(const std::string& email) const;

	private:
		TreeMultimap<std::string, User*>* m_userTree; //maps email address to the user
		std::unordered_set<std::string> m_files;
		std::string m_currentFile; 
		std::unordered_map<std::string, User*> m_allUsers;

		//helper functions
		void load_helper();
		void createUser_addToTree (const std::string& name, const std::string& email, 
			const std::vector<std::string>& movies);

};

#endif // USERDATABASE_INCLUDED
