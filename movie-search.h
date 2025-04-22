#ifndef MOVIE_SEARCH_H
#define MOVIE_SEARCH_H

#include <limits.h>
#include <string>
#include <vector>
#include <QStringList>
#include <map>
#include <unordered_map>

struct Movie
{
    std::string title;
    int year;
    int runtime;
    std::string genre;
    Movie(const std::string& _title, int _year, int _runtime, const std::string& _genre) :
        title(_title),
        year(_year),
        runtime(_runtime),
        genre(_genre) {}
};

struct Criteria
{
    int min_year = INT_MIN;
    int max_year = INT_MAX;
    int min_runtime = INT_MIN;
    int max_runtime = INT_MAX;
    QStringList genres;
};

class MovieSearch
{
public:
    virtual ~MovieSearch() {}
    virtual void load(const std::string& filename) = 0;
    virtual std::vector<const Movie*> search(const Criteria& criteria) = 0;
};

class LinearMovieSearch: public MovieSearch
{
private:
    std::vector<Movie> movies;
public:
    virtual void load(const std::string& filename) override;
    virtual std::vector<const Movie*> search(const Criteria& criteria) override;
};

class BTreeMovieSearch: public MovieSearch
{
private:
    std::map<std::pair<std::string, int>, Movie> btreeMovies;
public:
    virtual void load(const std::string& filename) override;
    virtual std::vector<const Movie*> search(const Criteria& criteria) override;
};

class HashMapMovieSearch: public MovieSearch
{
private:
    std::unordered_map<std::string, Movie> hashmapMovies;
public:
    virtual void load(const std::string& filename) override;
    virtual std::vector<const Movie*> search(const Criteria& criteria) override;
};

#endif // MOVIE_SEARCH_H
