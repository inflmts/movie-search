#include "movie-search.h"
#include <fstream>
#include <string>
#include <vector>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <utility>

/* Parse TSV file by tabs */
static QStringList tabsplit(const QString& line)
{
    QStringList fields;
    int field_start = 0;
    for (int i = 0; i < line.size(); ++i)
    {
        if (line[i] == '\t')
        {
            fields.append(line.mid(field_start, i - field_start));
            field_start = i + 1;
        }
    }
    fields.append(line.mid(field_start));
    return fields;
}

/* Vector - Load Movies */
void LinearMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    movies.clear();

    while (std::getline(file, line))
    {
        QString qLine = QString::fromStdString(line);
        QStringList fields = tabsplit(qLine);
        if (fields.size() < 9) // Ensure number of fields is correct
        {
            continue;
        }
        if (fields[5] == "\\N") // Check if field is not missing value
        {
            continue;
        }
        if (fields[7] == "\\N") // Check if field is not missing value
        {
            continue;
        }
        QString title = fields[2];
        bool checkYear, checkRuntime;
        int year = fields[5].toInt(&checkYear);
        int runtime = fields[7].toInt(&checkRuntime);
        QString genre = (fields.size() > 8) ? fields[8] : "";

        // Verify that year and runtime are valid
        if (checkYear && checkRuntime)
        {
            movies.emplace_back(title.toStdString(), year, runtime, genre.toStdString());
        }
        else
        {
            qDebug() << "Skipping invalid year or runtime:" << qLine;
        }
    }
    qDebug() << "Loaded " << movies.size() << " movies into Vector.";
}

/* Vector - Linear Search for Movies */
std::vector<const Movie*> LinearMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    const QStringList& selectedGenres = criteria.genres;

    for (const Movie& movie : movies)
    {
        bool genreMatch = selectedGenres.isEmpty();
        // If genres are selected, split them as needed and verify each is met
        if (!selectedGenres.isEmpty())
        {
            QString movieGenres = QString::fromStdString(movie.genre).trimmed();
            QStringList movieGenreList = movieGenres.split(',', Qt::SkipEmptyParts);
            for (QString& genre : movieGenreList)
            {
                genre = genre.trimmed();
            }

            genreMatch = true;
            for (const QString& selectedGenre : selectedGenres)
            {
                bool found = false;
                for (const QString& movieGenre : movieGenreList)
                {
                    if (movieGenre.compare(selectedGenre, Qt::CaseInsensitive) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    genreMatch = false;
                    break;
                }
            }
        }

        // Check if movie matches year and runtime
        if (movie.year >= criteria.min_year && movie.year <= criteria.max_year && movie.runtime >= criteria.min_runtime && movie.runtime <= criteria.max_runtime && genreMatch)
        {
            result.push_back(&movie);
        }
    }
    return result;
}

/* BTree - Load Movies */
void BTreeMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    btreeMovies.clear();

    while (std::getline(file, line))
    {
        QString qLine = QString::fromStdString(line);
        QStringList fields = tabsplit(qLine);
        if (fields.size() < 9) // Ensure number of fields is correct
            continue;
        if (fields[5] == "\\N") // Check if field is missing value
            continue;
        if (fields[7] == "\\N") // Check if field is missing value
            continue;
        QString title = fields[2];
        bool checkYear, checkRuntime;
        int year = fields[5].toInt(&checkYear);
        int runtime = fields[7].toInt(&checkRuntime);
        QString genre = (fields.size() > 8) ? fields[8] : "";

        // Verify year and runtime are valid
        if (checkYear && checkRuntime)
        {
            btreeMovies.insert({{title.toStdString(), year}, Movie(title.toStdString(), year, runtime, genre.toStdString())});
        }
        else
        {
            qDebug() << "Skipping invalid year or runtime:" << qLine;
        }
    }
    qDebug() << "Loaded " << btreeMovies.size() << " movies into B-Tree (std::map).";
}

/* BTree - Search for Movies */
std::vector<const Movie*> BTreeMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    const QStringList& selectedGenres = criteria.genres;

    for (const auto& pair : btreeMovies)
    {
        const Movie& movie = pair.second;
        bool genreMatch = selectedGenres.isEmpty();
        // If genres are selected, split them as needed and verify each is met
        if (!selectedGenres.isEmpty())
        {
            QString movieGenres = QString::fromStdString(movie.genre).trimmed();
            QStringList movieGenreList = movieGenres.split(',', Qt::SkipEmptyParts);
            for (QString& genre : movieGenreList)
            {
                genre = genre.trimmed();
            }

            genreMatch = true;
            for (const QString& selectedGenre : selectedGenres)
            {
                bool found = false;
                for (const QString& movieGenre : movieGenreList)
                {
                    if (movieGenre.compare(selectedGenre, Qt::CaseInsensitive) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    genreMatch = false;
                    break;
                }
            }
        }

        // Check if movie matches year and runtime
        if (movie.year >= criteria.min_year && movie.year <= criteria.max_year && movie.runtime >= criteria.min_runtime && movie.runtime <= criteria.max_runtime && genreMatch)
        {
            result.push_back(&movie);
        }
    }
    return result;
}

/* HashMap - Load Movies */
void HashMapMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    hashmapMovies.clear();

    while (std::getline(file, line))
    {
        QString qLine = QString::fromStdString(line);
        QStringList fields = tabsplit(qLine);
        if (fields.size() < 9) // Ensure number of fields is correct
            continue;
        if (fields[5] == "\\N") // Check if field is missing value
            continue;
        if (fields[7] == "\\N") // Check if field is missing value
            continue;
        QString title = fields[2];
        bool checkYear, checkRuntime;
        int year = fields[5].toInt(&checkYear);
        int runtime = fields[7].toInt(&checkRuntime);
        QString genre = (fields.size() > 8) ? fields[8] : "";

        // Verify that year and runtime are valid
        if (checkYear && checkRuntime)
        {
            hashmapMovies.insert(std::make_pair(title.toStdString(), Movie(title.toStdString(), year, runtime, genre.toStdString())));
        }
        else
        {
            qDebug() << "Skipping invalid year or runtime:" << qLine;
        }
    }
    qDebug() << "Loaded " << hashmapMovies.size() << " movies into Hash Map.";
}

/* HashMap - Search for Movies */
std::vector<const Movie*> HashMapMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    const QStringList& selectedGenres = criteria.genres;

    for (const auto& pair : hashmapMovies)
    {
        const Movie& movie = pair.second;
        bool genreMatch = selectedGenres.isEmpty();
        // If genres are selected, split them as needed and verify each is met
        if (!selectedGenres.isEmpty())
        {
            QString movieGenres = QString::fromStdString(movie.genre).trimmed();
            QStringList movieGenreList = movieGenres.split(',', Qt::SkipEmptyParts);
            for (QString& genre : movieGenreList)
            {
                genre = genre.trimmed();
            }

            genreMatch = true;
            for (const QString& selectedGenre : selectedGenres)
            {
                bool found = false;
                for (const QString& movieGenre : movieGenreList)
                {
                    if (movieGenre.compare(selectedGenre, Qt::CaseInsensitive) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    genreMatch = false;
                    break;
                }
            }
        }

        // Check if movie matches year and runtime
        if (movie.year >= criteria.min_year && movie.year <= criteria.max_year && movie.runtime >= criteria.min_runtime && movie.runtime <= criteria.max_runtime && genreMatch)
        {
            result.push_back(&movie);
        }
    }
    return result;
}
