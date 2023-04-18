#include "Recommender.h"

using namespace std;

#include <iostream>


struct Recommender::orderedMovies
{
    orderedMovies(const MovieDatabase* movies, std::string id, int score)
    {
        m_movies = movies; m_id = id; m_compatabilityScore = score; 
        m_rating = (m_movies->get_movie_from_id(m_id))->get_rating();
        m_title = (m_movies->get_movie_from_id(m_id))->get_title();
    }

    const MovieDatabase* m_movies;
    std::string m_id;
    int m_compatabilityScore;
    float m_rating;
    std::string m_title;

    bool operator< (const orderedMovies& other) const
    {
        if (m_compatabilityScore > other.m_compatabilityScore) //this has a higher score
        {
            return true;
        } else if (m_compatabilityScore < other.m_compatabilityScore) //other has a higher score
        {
            return false;
        }

        //both movies have the same score
        if (m_rating > other.m_rating) //this has a higher rating
        {
            return true;
        } else if (m_rating < other.m_rating) //other has a higher rating
        {
            return false;
        }

        //both have the same score AND rating
        if (m_title < other.m_title) //this's title comes before other
        {
            return true;
        } else
        {
            return false;
        }
    }
};

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    m_userDatabase = &user_database;
    m_movieDatabase = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    int movieRecCount = movie_count;
    
    //no reccomendations asked for
    if (movieRecCount <= 0)
    {
        return vector<MovieAndRank>();
    }

    //initialize user's information
    User* m_user = m_userDatabase->get_user_from_email(user_email);
    if(m_user == nullptr)
    {
        return vector<MovieAndRank>();
    }
    vector<string> m_userMovies = m_user->get_watch_history();

    //for every movie that the user watched, create master vector of actors, directors, and genres
    vector<string> uniqueActors;
    vector<string> uniqueDirectors;
    vector<string> uniqueGenres;
    find_all_data(uniqueActors, uniqueDirectors, uniqueGenres, m_userMovies);
    
    //for every one of the actors, directios, and genres, find ALL the unique movies and add them to an unordered map while keeping count of their score
    unordered_map<string, int> m_mapMovieReccs;
    populate_map(m_mapMovieReccs, uniqueActors, uniqueDirectors, uniqueGenres);

    //remove movies that the user has already watched from the map
    remove_duplicates(m_mapMovieReccs, m_userMovies);

    //convert map to set
    set<orderedMovies> reccs;
    convert_to_set(m_mapMovieReccs, reccs);

    //create a vector with AT MOST the number of recommendations asked for
    vector<MovieAndRank> m_finalRecommendations;
    order_recommended_movies(movieRecCount, m_finalRecommendations, reccs);

    return m_finalRecommendations;
}

void Recommender::populate_map(unordered_map<string, int>& m_mapMovieReccs, vector<string>& uniqueActors, vector<string>& uniqueDirectors, vector<string>& uniqueGenres) const
{
    //for each actor of the movie, attempt to add EVERY movie they've acted in to the map with 30 points
    populate_map_helper(m_mapMovieReccs, uniqueActors, &MovieDatabase::get_movies_with_actor, 30);

    //for each director of the movie, attempt to add EVERY movie they've acted in to the map with 20 points
    populate_map_helper(m_mapMovieReccs, uniqueDirectors, &MovieDatabase::get_movies_with_director, 20);

    //for each genre of the movie, attempt to add EVERY movie they've acted in to the map with 1 points
    populate_map_helper(m_mapMovieReccs, uniqueGenres, &MovieDatabase::get_movies_with_genre, 1);
}

void Recommender::populate_map_helper(unordered_map<string, int>& m_mapMovieReccs, vector<string>& uniqueData, vector<Movie*>(MovieDatabase::*getMovies)(const string&) const, int scoreAmt) const
{
    for (int i = 0; i < uniqueData.size(); i++)
    {
        vector<Movie*> moviesWithData = (m_movieDatabase->*getMovies)(uniqueData[i]);
        add_to_map(m_mapMovieReccs, moviesWithData, scoreAmt);
    }
}

void Recommender::remove_duplicates(unordered_map<string, int>& m_mapMovieReccs, vector<string>& m_userMovies) const
{
    //for every movie the user has watched, attempt to remove the movie from the map (if it exists)
    for (int i = 0; i < m_userMovies.size(); i++)
    {
        m_mapMovieReccs.erase(m_userMovies[i]);
    }
}

void Recommender::add_to_map(unordered_map<string, int>& m_mapMovieReccs, vector<Movie*>& movies, int scoreAmt) const
{
    //iterate through the vector of movies
    for (int i = 0; i < movies.size(); i++)
    {
        string ID = movies[i]->get_id();
        unordered_map<string, int>::iterator it = m_mapMovieReccs.find(ID);

        //if the movie is NOT in the map, add it with it's designated score
        if (it == m_mapMovieReccs.end())
        {
            m_mapMovieReccs.insert(make_pair(ID, scoreAmt));
        } else //if the movie IS in the map already, add the designated score to the movie
        {
            m_mapMovieReccs[ID] += scoreAmt; 
        }
    }
}

void Recommender::convert_to_set(unordered_map<string, int>& m_mapMovieReccs, set<orderedMovies>& orderedReccs) const
{
    unordered_map<string, int>::iterator it;
    
    for (it = m_mapMovieReccs.begin(); it != m_mapMovieReccs.end(); it++)
    {  
        orderedMovies toAdd = orderedMovies(m_movieDatabase, it->first, it->second);
        orderedReccs.insert(toAdd);
    } 
}

void Recommender::order_recommended_movies(int movieRecCount, vector<MovieAndRank>& m_finalRecommendations, set<orderedMovies>& reccs) const
{
    set<orderedMovies>::iterator it = reccs.begin();
    int setSize = reccs.size();

    //ensure that AT MOST the number of recommendations are added to the final list (in case the set doesn't have enough)
    for (int i = 0; i < movieRecCount && i < setSize; i++, it++)
    {
        MovieAndRank toAdd = MovieAndRank(it->m_id, it->m_compatabilityScore);
        m_finalRecommendations.push_back(toAdd);
    }

}

void Recommender::find_all_data(vector<string>& uniqueActors, vector<string>& uniqueDirectors, vector<string>& uniqueGenres, vector<std::string>& userMovies) const
{
    for (int i = 0; i < userMovies.size(); i++)
    {
        Movie* mov = m_movieDatabase->get_movie_from_id(userMovies[i]);

        vector<string> actors = mov->get_actors();
        for (int i = 0; i < actors.size(); i++)
        {
            uniqueActors.push_back(actors[i]);
        } 

        vector<string> directors = mov->get_directors();
        for (int i = 0; i < directors.size(); i++)
        {
            uniqueDirectors.push_back(directors[i]);
        } 

        vector<string> genres = mov->get_genres();
        for (int i = 0; i < genres.size(); i++)
        {
            uniqueGenres.push_back(genres[i]);
        }
    }
}