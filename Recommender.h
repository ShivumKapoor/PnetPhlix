#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <vector>
#include <set>

#include "User.h"
#include "Movie.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
	MovieAndRank(const std::string& id, int score)
		: movie_id(id), compatibility_score(score)
	{}

	std::string movie_id;
	int compatibility_score;
};

class Recommender
{
	public:
		Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database);

		std::vector<MovieAndRank> recommend_movies(const std::string& user_email, int movie_count) const;

	private:
		//user and movie database
		const UserDatabase* m_userDatabase;
		const MovieDatabase* m_movieDatabase;

		//struct for comparison
		struct orderedMovies;

		//consolidate data
		void find_all_data(std::vector<std::string>& uniqueActors, std::vector<std::string>& uniqueDirectors, std::vector<std::string>& uniqueGenres, std::vector<std::string>& userMovies) const;

		//movie recommendations
		void populate_map(std::unordered_map<std::string, int>& m_mapMovieReccs, std::vector<std::string>& uniqueActors, std::vector<std::string>& uniqueDirectors, std::vector<std::string>& uniqueGenres) const;
		void populate_map_helper(std::unordered_map<std::string, int>& m_mapMovieReccs, std::vector<std::string>& uniqueData, std::vector<Movie*>(MovieDatabase::*getMovies)(const std::string&) const, int scoreAmt) const;
		void remove_duplicates(std::unordered_map<std::string, int>& m_mapMovieReccs, std::vector<std::string>& userMovies) const;
		void add_to_map(std::unordered_map<std::string, int>& m_mapMovieReccs, std::vector<Movie*>& movies, int scoreAmt) const;
		void convert_to_set(std::unordered_map<std::string, int>& m_mapMovieReccs, std::set<orderedMovies>& orderedReccs) const;
		void order_recommended_movies(int movieRecCount, std::vector<MovieAndRank>& m_finalRecommendations, std::set<orderedMovies>& reccs) const;
};

#endif // RECOMMENDER_INCLUDED
