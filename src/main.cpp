#include <iostream>
#include "movie-search.h"

int main()
{
    Criteria criteria;
    criteria.min_year = 2025;
    criteria.max_year = 2025;
    criteria.min_runtime = 94;
    criteria.max_runtime = 94;

    LinearMovieSearch lin;
    lin.load("movies.tsv");
    for (const Movie *movie : lin.search(criteria)) {
        std::cout
            << "Title:   " << movie->title   << "\n"
            << "Year:    " << movie->year    << "\n"
            << "Runtime: " << movie->runtime << "\n"
            << "Genre:   " << movie->genre   << "\n"
            << "\n";
    }
}
