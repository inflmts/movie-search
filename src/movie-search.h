#include <limits.h>
#include <string>
#include <vector>

struct Movie
{
    std::string title;
    int year;
    int runtime;
    int genre;
    Movie(const std::string& _title, int _year, int _runtime, int _genre) :
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
    int genres = ~0;
};

class MovieSearch
{
  public:
    virtual void load(const std::string& filename) = 0;
    virtual std::vector<const Movie*> search(const Criteria& criteria) = 0;
};

class LinearMovieSearch: public MovieSearch
{
  private:
    std::vector<Movie> movies;

  public:
    virtual void load(const std::string& filename);
    virtual std::vector<const Movie*> search(const Criteria& criteria);
};
