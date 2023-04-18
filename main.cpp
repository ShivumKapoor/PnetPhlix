#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include "Recommender.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to close this window . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

int main() //clang++ main.cpp UserDatabase.cpp User.cpp MovieDatabase.cpp Movie.cpp Recommender.cpp utility.cpp -o n
{
	//START MEASURING TIME
	auto start = std::chrono::high_resolution_clock::now();

	MovieDatabase* m = new MovieDatabase();
	UserDatabase* u = new UserDatabase();

	if (!m->load("movies.txt"))
	{
		cout << "movies load error!" << endl;
	}

	if (!u->load("users.txt"))
	{
		cout << "users load error!" << endl;
	}
	
	//assert(!u->load("users.txt"));

	Recommender* r = new Recommender(*u, *m);
	vector<MovieAndRank> movie_reccs = r->recommend_movies("KabiL@aol.com", 10);

	for (int i = 0; i < movie_reccs.size(); i++)
	{
		Movie* mov = m->get_movie_from_id(movie_reccs[i].movie_id);
		cout << i + 1<< ".  " << mov->get_title() << endl;
		cout << "    Rating: " << mov->get_rating() << endl;
		cout << "    Compatability Score: " << movie_reccs[i].compatibility_score << endl << endl;
	}

	//END TIME MEASUREMENT
	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " ms" << std::endl;

}
