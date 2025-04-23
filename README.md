# Movie Search

- Name: Project 3
- Course: COP3530, University of Florida
- Authors: Group 83 (Sean Muller, Isaac Zeng, Daniel Li)
- Date: April 2025

## Build Instructions

This project uses the
[IMDb Non-Commercial Datasets](https://developer.imdb.com/non-commercial-datasets/).
The data can be found at <https://datasets.imdbws.com>.

To get started, download and extract
<https://datasets.imdbws.com/title.basics.tsv.gz>
to a file named `movies.tsv` in the current directory, selecting only movies:

```
curl -Lf https://datasets.imdbws.com/title.basics.tsv.gz \
  | gunzip \
  | grep -F "$(printf '\tmovie\t')" > movies.tsv
```

### CMake

As necessary

### Meson

This project requires a C++17 compiler and the Qt5 development libraries.

```
meson setup build
ninja -C build
```

Run with:

```
build/movie-search
```
