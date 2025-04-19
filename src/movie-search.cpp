#include <fstream>
#include <string>
#include <vector>
#include "movie-search.h"

static std::vector<std::string> tabsplit(const std::string& line)
{
    std::vector<std::string> fields;
    unsigned int field_start = 0;
    for (unsigned int i = 0; i < line.size(); i++) {
        if (line[i] == '\t') {
            fields.push_back(line.substr(field_start, i - field_start));
            field_start = i + 1;
        }
    }
    fields.push_back(line.substr(field_start));
    return fields;
}

void LinearMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> fields = tabsplit(line);
        if (fields.size() < 9)
            continue;
        if (fields[5] == "\\N")
            continue;
        if (fields[7] == "\\N")
            continue;
        std::string& title = fields[2];
        int year = stoi(fields[5]);
        int runtime = stoi(fields[7]);
        int genre = ~0; // FIXME
        movies.emplace_back(title, year, runtime, genre);
    }
}

std::vector<const Movie*> LinearMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    for (const Movie& movie : movies) {
        if (movie.year >= criteria.min_year &&
            movie.year <= criteria.max_year &&
            movie.runtime >= criteria.min_runtime &&
            movie.runtime <= criteria.max_runtime &&
            movie.genre & criteria.genres
        ) result.push_back(&movie);
    }
    return result;
}
