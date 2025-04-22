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

static QStringList tabsplit(const QString& line)
{
    QStringList fields;
    int field_start = 0;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '\t') {
            fields.append(line.mid(field_start, i - field_start));
            field_start = i + 1;
        }
    }
    fields.append(line.mid(field_start));
    return fields;
}

// --- LinearMovieSearch Implementation ---
void LinearMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    movies.clear();

    while (std::getline(file, line)) {
        QString qLine = QString::fromStdString(line);
        QStringList fields = tabsplit(qLine);
        if (fields.size() < 9)
            continue;
        if (fields[5] == "\\N")
            continue;
        if (fields[7] == "\\N")
            continue;
        QString title = fields[2];
        bool okYear, okRuntime;
        int year = fields[5].toInt(&okYear);
        int runtime = fields[7].toInt(&okRuntime);
        QString genre = (fields.size() > 8) ? fields[8] : "";

        if (okYear && okRuntime) {
            movies.emplace_back(title.toStdString(), year, runtime, genre.toStdString());
        } else {
            qDebug() << "Skipping invalid year or runtime:" << qLine;
        }
    }
    qDebug() << "Loaded " << movies.size() << " movies into Vector.";
}

std::vector<const Movie*> LinearMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    const QStringList& selectedGenres = criteria.genres;

    for (const Movie& movie : movies) {
        bool genreMatch = selectedGenres.isEmpty();
        if (!selectedGenres.isEmpty()) {
            QString movieGenres = QString::fromStdString(movie.genre).trimmed();
            QStringList movieGenreList = movieGenres.split(',', Qt::SkipEmptyParts);
            for (QString& genre : movieGenreList) {
                genre = genre.trimmed();
            }

            genreMatch = true;
            for (const QString& selectedGenre : selectedGenres) {
                bool found = false;
                for (const QString& movieGenre : movieGenreList) {
                    if (movieGenre.compare(selectedGenre, Qt::CaseInsensitive) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    genreMatch = false;
                    break;
                }
            }
        }

        if (movie.year >= criteria.min_year &&
            movie.year <= criteria.max_year &&
            movie.runtime >= criteria.min_runtime &&
            movie.runtime <= criteria.max_runtime &&
            genreMatch) {
            result.push_back(&movie);
        }
    }
    return result;
}

// --- BTreeMovieSearch Implementation ---
void BTreeMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    btreeMovies.clear();

    while (std::getline(file, line)) {
        QString qLine = QString::fromStdString(line);
        QStringList fields = tabsplit(qLine);
        if (fields.size() < 9)
            continue;
        if (fields[5] == "\\N")
            continue;
        if (fields[7] == "\\N")
            continue;
        QString title = fields[2];
        bool okYear, okRuntime;
        int year = fields[5].toInt(&okYear);
        int runtime = fields[7].toInt(&okRuntime);
        QString genre = (fields.size() > 8) ? fields[8] : "";

        if (okYear && okRuntime) {
            btreeMovies[{title.toStdString(), year}] = Movie(title.toStdString(), year, runtime, genre.toStdString());
        } else {
            qDebug() << "Skipping invalid year or runtime:" << qLine;
        }
    }
    qDebug() << "Loaded " << btreeMovies.size() << " movies into B-Tree (std::map).";
}

std::vector<const Movie*> BTreeMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    const QStringList& selectedGenres = criteria.genres;

    for (const auto& pair : btreeMovies) {
        const Movie& movie = pair.second;
        bool genreMatch = selectedGenres.isEmpty();
        if (!selectedGenres.isEmpty()) {
            QString movieGenres = QString::fromStdString(movie.genre).trimmed();
            QStringList movieGenreList = movieGenres.split(',', Qt::SkipEmptyParts);
            for (QString& genre : movieGenreList) {
                genre = genre.trimmed();
            }

            genreMatch = true;
            for (const QString& selectedGenre : selectedGenres) {
                bool found = false;
                for (const QString& movieGenre : movieGenreList) {
                    if (movieGenre.compare(selectedGenre, Qt::CaseInsensitive) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    genreMatch = false;
                    break;
                }
            }
        }

        if (movie.year >= criteria.min_year &&
            movie.year <= criteria.max_year &&
            movie.runtime >= criteria.min_runtime &&
            movie.runtime <= criteria.max_runtime &&
            genreMatch) {
            result.push_back(&movie);
        }
    }
    return result;
}

// --- HashMapMovieSearch Implementation ---
void HashMapMovieSearch::load(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    hashmapMovies.clear();

    while (std::getline(file, line)) {
        QString qLine = QString::fromStdString(line);
        QStringList fields = tabsplit(qLine);
        if (fields.size() < 9)
            continue;
        if (fields[5] == "\\N")
            continue;
        if (fields[7] == "\\N")
            continue;
        QString title = fields[2];
        bool okYear, okRuntime;
        int year = fields[5].toInt(&okYear);
        int runtime = fields[7].toInt(&okRuntime);
        QString genre = (fields.size() > 8) ? fields[8] : "";

        if (okYear && okRuntime) {
            hashmapMovies[title.toStdString()] = Movie(title.toStdString(), year, runtime, genre.toStdString());
        } else {
            qDebug() << "Skipping invalid year or runtime:" << qLine;
        }
    }
    qDebug() << "Loaded " << hashmapMovies.size() << " movies into Hash Map.";
}

std::vector<const Movie*> HashMapMovieSearch::search(const Criteria& criteria)
{
    std::vector<const Movie*> result;
    const QStringList& selectedGenres = criteria.genres;

    for (const auto& pair : hashmapMovies) {
        const Movie& movie = pair.second;
        bool genreMatch = selectedGenres.isEmpty();
        if (!selectedGenres.isEmpty()) {
            QString movieGenres = QString::fromStdString(movie.genre).trimmed();
            QStringList movieGenreList = movieGenres.split(',', Qt::SkipEmptyParts);
            for (QString& genre : movieGenreList) {
                genre = genre.trimmed();
            }

            genreMatch = true;
            for (const QString& selectedGenre : selectedGenres) {
                bool found = false;
                for (const QString& movieGenre : movieGenreList) {
                    if (movieGenre.compare(selectedGenre, Qt::CaseInsensitive) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    genreMatch = false;
                    break;
                }
            }
        }

        if (movie.year >= criteria.min_year &&
            movie.year <= criteria.max_year &&
            movie.runtime >= criteria.min_runtime &&
            movie.runtime <= criteria.max_runtime &&
            genreMatch) {
            result.push_back(&movie);
        }
    }
    return result;
}
