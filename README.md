# COP3530 - Project 3

Group 83 - Sean Muller, Isaac Zeng, Daniel Li (MZL)

**Project Title:** Movie Search

- **Problem:**
  Searching for movies manually is difficult, so we are looking for a way to do
  this automatically.

- **Motivation:**
  A person may want to find a movie of a specific genre of a different year
  because they might want to find a movie that is different from what is
  released in the current year that gives a different feeling or experience.

- **Features:**
  The program will return a list of all movies in the dataset that match the
  user’s input criteria, which may include specifications for:

  - Year (specific value or a range).
  - Genre (one, any in a set, or all in a set).
  - Runtime (a range).
  - Others may be added in the future.

- **Data:**
  IMDb Non-Commercial Datasets

  - <https://developer.imdb.com/non-commercial-datasets/> (documentation)
  - <https://datasets.imdbws.com> (downloads)
  - <https://datasets.imdbws.com/title.basics.tsv.gz> (basic movie information)

- **Tools:**

  - Programming language: C++
  - Test framework: [Catch2](https://github.com/catchorg/Catch2/tree/devel/docs)
  - UI: [Qt](https://doc.qt.io)

- **Visuals:**

  ```
  +------------------+
  | TITLE OF PROJECT |
  |                  |
  | Parameters...    |
  |                  |
  |     [Enter]      |
  +------------------+
           |
           v
  +------------------+
  | +--------------+ |
  | | Title        | |
  | | Year Runtime | |
  | +--------------+ |
  | ...              |
  +------------------+
  ```

- **Strategy:**
  B tree, hash map, ...

- **Distribution of Responsibility and Roles:**

  - All: Peer feedback
  - Sean: User interface
  - Isaac: Implementation
  - Daniel: Tests
