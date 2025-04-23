#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>

/* Implementation for Genre Selection */
GenreSelectionDialog::GenreSelectionDialog(const QStringList& availableGenres, QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Select Genres (Max of Three)");
    genreListWidget = new QListWidget();
    genreListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    genreListWidget->addItems(availableGenres);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(genreListWidget);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QStringList GenreSelectionDialog::selectedGenres() const
{
    QStringList selected;
    QList<QListWidgetItem*> selectedItems = genreListWidget->selectedItems();
    for (QListWidgetItem* item : selectedItems)
    {
        selected.append(item->text());
    }
    return selected;
}

/* Implementation for Main Window */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), availableGenres({"Action", "Adventure", "Animation", "Biography", "Comedy", "Crime", "Documentary", "Drama", "Family", "Fantasy", "Film-Noir", "History", "Horror", "Music", "Musical", "Mystery", "Romance", "Sci-Fi", "Sport", "Thriller", "War", "Western"}), movieSearch(new LinearMovieSearch()) // Initialize with the default search
{
    setWindowTitle("Movie Search"); // Title for Main Window

    // Create UI elements
    QLabel* dataStructureLabel = new QLabel("Data Structure:");
    dataStructureCombo = new QComboBox();
    dataStructureCombo->addItem("Vector");
    dataStructureCombo->addItem("B-Tree");
    dataStructureCombo->addItem("Hash Map");
    refreshButton = new QPushButton("Refresh");

    QLabel* yearLabel = new QLabel("Year Range:");
    minYearEdit = new QLineEdit();
    maxYearEdit = new QLineEdit();
    minYearEdit->setValidator(new QIntValidator());
    maxYearEdit->setValidator(new QIntValidator());

    QLabel* runtimeLabel = new QLabel("Runtime Range:");
    minRuntimeEdit = new QLineEdit();
    maxRuntimeEdit = new QLineEdit();
    minRuntimeEdit->setValidator(new QIntValidator());
    maxRuntimeEdit->setValidator(new QIntValidator());

    QLabel* genreLabel = new QLabel("Genres (Max of Three):");
    genreButton = new QPushButton("Select Genres...");

    searchButton = new QPushButton("Search");
    resultsList = new QListWidget();

    // Layout for data structure selection
    QHBoxLayout* dataStructureLayout = new QHBoxLayout();
    dataStructureLayout->addWidget(dataStructureLabel);
    dataStructureLayout->addWidget(dataStructureCombo);
    dataStructureLayout->addWidget(refreshButton);

    // Layout for year
    QHBoxLayout* yearLayout = new QHBoxLayout();
    yearLayout->addWidget(minYearEdit);
    yearLayout->addWidget(new QLabel("-"));
    yearLayout->addWidget(maxYearEdit);

    // Layout for runtime
    QHBoxLayout* runtimeLayout = new QHBoxLayout();
    runtimeLayout->addWidget(minRuntimeEdit);
    runtimeLayout->addWidget(new QLabel("-"));
    runtimeLayout->addWidget(maxRuntimeEdit);

    // Main Window Layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(dataStructureLayout);
    mainLayout->addWidget(yearLabel);
    mainLayout->addLayout(yearLayout);
    mainLayout->addWidget(runtimeLabel);
    mainLayout->addLayout(runtimeLayout);
    mainLayout->addWidget(genreLabel);
    mainLayout->addWidget(genreButton);
    mainLayout->addWidget(searchButton);
    mainLayout->addWidget(resultsList);

    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Load movie data initially (using the default - Vector - implementation)
    movieSearch->load("movies.tsv");

    // Connect signals and slots
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchButtonClicked);
    connect(genreButton, &QPushButton::clicked, this, &MainWindow::showGenreSelectionDialog);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshDataStructure);
}

/* Main Window Destructor */
MainWindow::~MainWindow()
{
    delete movieSearch;
}

/* Display the Dialog for Genre Selection */
void MainWindow::showGenreSelectionDialog()
{
    GenreSelectionDialog dialog(availableGenres, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        selectedMovieGenres = dialog.selectedGenres();
        if (selectedMovieGenres.size() > 3) // If select more than 3 genres, display message
        {
            QMessageBox::warning(this, "Genre Selection", "You can select up to three genres.");
            selectedMovieGenres = selectedMovieGenres.mid(0, 3);
        }
        genreButton->setText(selectedMovieGenres.join(", "));
        if (selectedMovieGenres.isEmpty())
        {
            genreButton->setText("Select Genres...");
        }
    }
}

/* Searching for Results */
void MainWindow::searchButtonClicked()
{
    Criteria criteria;
    // Verify inputs to search
    bool checkMinYear, checkMaxYear, checkMinRuntime, checkMaxRuntime;

    int minYear = minYearEdit->text().toInt(&checkMinYear);
    int maxYear = maxYearEdit->text().toInt(&checkMaxYear);
    int minRuntime = minRuntimeEdit->text().toInt(&checkMinRuntime);
    int maxRuntime = maxRuntimeEdit->text().toInt(&checkMaxRuntime);

    if (!minYearEdit->text().isEmpty() && !checkMinYear)
    {
        QMessageBox::warning(this, "Input Error", "Invalid minimum year.");
        return;
    }
    if (!maxYearEdit->text().isEmpty() && !checkMaxYear)
    {
        QMessageBox::warning(this, "Input Error", "Invalid maximum year.");
        return;
    }
    if (!minRuntimeEdit->text().isEmpty() && !checkMinRuntime)
    {
        QMessageBox::warning(this, "Input Error", "Invalid minimum runtime.");
        return;
    }
    if (!maxRuntimeEdit->text().isEmpty() && !checkMaxRuntime)
    {
        QMessageBox::warning(this, "Input Error", "Invalid maximum runtime.");
        return;
    }

    // Assign criteria
    criteria.min_year = checkMinYear ? minYear : INT_MIN;
    criteria.max_year = checkMaxYear ? maxYear : INT_MAX;
    criteria.min_runtime = checkMinRuntime ? minRuntime : INT_MIN;
    criteria.max_runtime = checkMaxRuntime ? maxRuntime : INT_MAX;
    criteria.genres = selectedMovieGenres;

    // Search for results
    std::vector<const Movie*> results = movieSearch->search(criteria);

    // Display results
    resultsList->clear();
    for (const Movie* movie : results)
    {
        QString movieInfo = QString("Title: %1\nYear: %2\nRuntime: %3\nGenre: %4").arg(QString::fromStdString(movie->title)).arg(movie->year).arg(movie->runtime).arg(QString::fromStdString(movie->genre));
        resultsList->addItem(movieInfo);
    }
}

/* Update the Data Structure Being Used */
void MainWindow::refreshDataStructure()
{
    QString selected = dataStructureCombo->currentText();

    // If data structure is already set to the chosen structure
    if (selected == currentDataStructure && movieSearch != nullptr)
    {
        QMessageBox::information(this, "Refresh", "Data structure is already set to " + currentDataStructure + ".");
        return;
    }

    // Otherwise, clear the old data structure
    if (movieSearch != nullptr)
    {
        delete movieSearch; // Delete the old data structure
        movieSearch = nullptr;
    }

    // Create selected data structure
    if (selected == "Vector")
    {
        movieSearch = new LinearMovieSearch();
    }
    else if (selected == "B-Tree")
    {
        movieSearch = new BTreeMovieSearch();
    }
    else if (selected == "Hash Map")
    {
        movieSearch = new HashMapMovieSearch();
    }

    // Populate the new data structure
    if (movieSearch != nullptr)
    {
        movieSearch->load("movies.tsv");
        currentDataStructure = selected;
        resultsList->clear(); // Clear previous search results
        QMessageBox::information(this, "Refresh", "Movie data reorganized using " + currentDataStructure + ".");
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to create the selected data structure.");
    }
}
