#include "MovieDatabase.h"

using namespace std;

MovieDatabase::MovieDatabase()
{
    //initialize files and movies 
    m_files =  unordered_set<string>();
    m_allMovies = unordered_map<string, Movie*>();
    //initialize current trees for movies, directors, actor, and genre
    m_movieTree = new TreeMultimap<string, Movie*>;
    m_directorTree = new TreeMultimap<string, Movie*>;
    m_actorTree = new TreeMultimap<string, Movie*>;
    m_genreTree = new TreeMultimap<string, Movie*>;
}

MovieDatabase::~MovieDatabase()
{
    unordered_map<string, Movie*>::iterator it = m_allMovies.begin();
    while(it != m_allMovies.end())
    {
        delete it->second;
        it++;
    }
}

bool MovieDatabase::load(const string& filename)
{
    //check if the file hasn't been called yet
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

void MovieDatabase::load_helper()
{
    //every movie has the following qualities
    string movieID;
    string title;
    string releaseYear;
    string directors;
    string actors;
    string genres;
    float rating;

    //to catch unncessary values
    string dumper;

    std::ifstream file (m_currentFile);
    for(;;)
    {
        //check if end of file
        if (file.eof())
        {
            break;
        }
        //collect data from a file
        getline(file, movieID);
        getline(file, title);
        getline(file, releaseYear);
        getline(file, directors);
        getline(file, actors);
        getline(file, genres);
        getline(file, dumper); 
        rating = stof(dumper);
        
        //create a new movie
        Movie* newMovie = create_movie(movieID, title, releaseYear, directors, actors, genres, rating);
        m_movieTree->insert(newMovie->get_id(), newMovie);
        m_allMovies.insert(make_pair(newMovie->get_id(), newMovie));

        //add newMovie to director, actor, and genre tree
        add_to_tree(m_directorTree, newMovie->get_directors(), newMovie);
        add_to_tree(m_actorTree, newMovie->get_actors(), newMovie);
        add_to_tree(m_genreTree, newMovie->get_genres(), newMovie);

        //catch empty line
        getline(file, dumper);
    }
}

Movie* MovieDatabase::create_movie(const string& ID, const string& title, const string& releaseYear, string directors, string actors, string genres, const float& rating)
{
    //split each string into vectors
    const vector<string>& directorsVec = split_str(directors);
    const vector<string>& actorsVec = split_str(actors);
    const vector<string>& genresVec = split_str(genres);

    //create and return a new Movie pointer
    return new Movie(ID, title, releaseYear, directorsVec, actorsVec, genresVec, rating);
}

vector<string> MovieDatabase::split_str(string str)
{
    stringstream ss (str);
    string add;
    vector<string> toReturn;

    //split each string by , and populate a vector
    while(getline(ss, add, ','))
    {
        toReturn.push_back(add);
    }

    return toReturn;
}

void MovieDatabase::add_to_tree(TreeMultimap<string, Movie*>* tree, vector<string> values, Movie* newMovie)
{
    //for each director, actor, and genre value in the vector --> add the movie that they're associated with into their respective trees
    for(int i = 0; i < values.size(); i++)
    {
        tree->insert(values[i], newMovie);
    }
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = m_movieTree->find(id);

    if (it.is_valid())
    {
        //a reference to the Movie that it points at
        return it.get_value();
    }

    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    return get_movies_helper(m_directorTree, director);
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    return get_movies_helper(m_actorTree, actor);
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    return get_movies_helper(m_genreTree, genre);
}

vector<Movie*> MovieDatabase::get_movies_helper(TreeMultimap<string, Movie*>* tree, const string& value) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = tree->find(value);

    if (it.is_valid())
    {
        vector<Movie*> movies;

        while (it.is_valid())
        {
            movies.push_back(it.get_value());
            //std::cout << (it).get_value() << std::endl;
            it.advance();
        }

        return movies;
    }

    return vector<Movie*>();
}