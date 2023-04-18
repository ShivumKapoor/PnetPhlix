#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <sstream>

#include "treemm.h"
#include "utility.h"
#include "Movie.h"

class Movie;

class MovieDatabase
{
	public:
		MovieDatabase();
		~MovieDatabase();
		bool load(const std::string& filename);
		Movie* get_movie_from_id(const std::string& id) const;
		std::vector<Movie*> get_movies_with_director(const std::string& director) const;
		std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
		std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

	private:
		std::unordered_set<std::string> m_files;
		std::string m_currentFile; 
		std::unordered_map<std::string, Movie*> m_allMovies;

		void load_helper();
		Movie* create_movie(const std::string& ID, const std::string& title, const std::string& releaseYear, std::string directors, std::string actors, std::string genres, const float& rating);
		std::vector<std::string> split_str(std::string str);
		
		//maps movie ID to a pointer to the movie
		TreeMultimap<std::string, Movie*>* m_movieTree;

		//maps a director's name to a Movie pointer
		TreeMultimap<std::string, Movie*>* m_directorTree;

		//maps an actor's name to a Movie pointer
		TreeMultimap<std::string, Movie*>* m_actorTree;

		//maps a genre to a Movie pointer
		TreeMultimap<std::string, Movie*>* m_genreTree;

		void add_to_tree(TreeMultimap<std::string, Movie*>* tree, std::vector<std::string> values, Movie* newMovie);
		std::vector<Movie*> get_movies_helper(TreeMultimap<std::string, Movie*>* tree, const std::string& value) const;
};

#endif // MOVIEDATABASE_INCLUDED
