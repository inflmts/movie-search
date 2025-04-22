#include <QMessageBox>
#include <QDebug>
#include "data.h"
#include "mainwindow.h"

// --- GenreSelectionDialog Implementation ---
GenreSelectionDialog::GenreSelectionDialog(const QStringList& availableGenres, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Select Genres (Up to 3)");
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
    for (QListWidgetItem* item : selectedItems) {
        selected.append(item->text());
    }
    return selected;
}

// --- MainWindow Implementation ---
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    availableGenres({"Action", "Adventure", "Animation", "Biography", "Comedy", "Crime",
                     "Documentary", "Drama", "Family", "Fantasy", "Film-Noir", "History",
                     "Horror", "Music", "Musical", "Mystery", "Romance", "Sci-Fi",
                     "Sport", "Thriller", "War", "Western"}),
    movieSearch(new LinearMovieSearch()) // Initialize with the default search
{
    setWindowTitle("Movie Search");

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

    QLabel* genreLabel = new QLabel("Genres (Up to 3):");
    genreButton = new QPushButton("Select Genres...");

    searchButton = new QPushButton("Search");
    resultsList = new QListWidget();

    // Layout for data structure selection
    QHBoxLayout* dataStructureLayout = new QHBoxLayout();
    dataStructureLayout->addWidget(dataStructureLabel);
    dataStructureLayout->addWidget(dataStructureCombo);
    dataStructureLayout->addWidget(refreshButton);

    // Layout
    QHBoxLayout* yearLayout = new QHBoxLayout();
    yearLayout->addWidget(minYearEdit);
    yearLayout->addWidget(new QLabel("-"));
    yearLayout->addWidget(maxYearEdit);

    QHBoxLayout* runtimeLayout = new QHBoxLayout();
    runtimeLayout->addWidget(minRuntimeEdit);
    runtimeLayout->addWidget(new QLabel("-"));
    runtimeLayout->addWidget(maxRuntimeEdit);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(dataStructureLayout); // Add the data structure selection
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

    // Load movie data initially (using the default Vector implementation)
    movieSearch->load("movies.tsv");

    // Connect signals and slots
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchButtonClicked);
    connect(genreButton, &QPushButton::clicked, this, &MainWindow::showGenreSelectionDialog);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshDataStructure);
}

MainWindow::~MainWindow()
{
    delete movieSearch; // Clean up the dynamically allocated MovieSearch object
}

void MainWindow::showGenreSelectionDialog()
{
    GenreSelectionDialog dialog(availableGenres, this);
    if (dialog.exec() == QDialog::Accepted) {
        selectedMovieGenres = dialog.selectedGenres();
        if (selectedMovieGenres.size() > 3) {
            QMessageBox::warning(this, "Genre Selection", "You can select up to three genres.");
            selectedMovieGenres = selectedMovieGenres.mid(0, 3);
        }
        genreButton->setText(selectedMovieGenres.join(", "));
        if (selectedMovieGenres.isEmpty()) {
            genreButton->setText("Select Genres...");
        }
    }
}

void MainWindow::searchButtonClicked()
{
    Criteria criteria;
    bool okMinYear, okMaxYear, okMinRuntime, okMaxRuntime;

    int minYear = minYearEdit->text().toInt(&okMinYear);
    int maxYear = maxYearEdit->text().toInt(&okMaxYear);
    int minRuntime = minRuntimeEdit->text().toInt(&okMinRuntime);
    int maxRuntime = maxRuntimeEdit->text().toInt(&okMaxRuntime);

    if (!minYearEdit->text().isEmpty() && !okMinYear) {
        QMessageBox::warning(this, "Input Error", "Invalid minimum year.");
        return;
    }
    if (!maxYearEdit->text().isEmpty() && !okMaxYear) {
        QMessageBox::warning(this, "Input Error", "Invalid maximum year.");
        return;
    }
    if (!minRuntimeEdit->text().isEmpty() && !okMinRuntime) {
        QMessageBox::warning(this, "Input Error", "Invalid minimum runtime.");
        return;
    }
    if (!maxRuntimeEdit->text().isEmpty() && !okMaxRuntime) {
        QMessageBox::warning(this, "Input Error", "Invalid maximum runtime.");
        return;
    }

    criteria.min_year = okMinYear ? minYear : INT_MIN;
    criteria.max_year = okMaxYear ? maxYear : INT_MAX;
    criteria.min_runtime = okMinRuntime ? minRuntime : INT_MIN;
    criteria.max_runtime = okMaxRuntime ? maxRuntime : INT_MAX;
    criteria.genres = selectedMovieGenres;

    std::vector<const Movie*> results = movieSearch->search(criteria);

    resultsList->clear();
    for (const Movie* movie : results) {
        QString movieInfo = QString("Title: %1\nYear: %2\nRuntime: %3\nGenre: %4")
        .arg(QString::fromStdString(movie->title))
            .arg(movie->year)
            .arg(movie->runtime)
            .arg(QString::fromStdString(movie->genre));
        resultsList->addItem(movieInfo);
    }
}

void MainWindow::refreshDataStructure()
{
    QString selected = dataStructureCombo->currentText();

    if (selected == currentDataStructure && movieSearch != nullptr) {
        QMessageBox::information(this, "Refresh", "Data structure is already set to " + currentDataStructure + ".");
        return;
    }

    if (movieSearch != nullptr) {
        delete movieSearch; // Delete the old data structure
        movieSearch = nullptr;
    }

    if (selected == "Vector") {
        movieSearch = new LinearMovieSearch();
    } else if (selected == "B-Tree") {
        movieSearch = new BTreeMovieSearch(); // Assuming you'll create this class
    } else if (selected == "Hash Map") {
        movieSearch = new HashMapMovieSearch(); // Assuming you'll create this class
    }

    if (movieSearch != nullptr) {
        movieSearch->load("movies.tsv");
        currentDataStructure = selected;
        resultsList->clear(); // Clear previous search results
        QMessageBox::information(this, "Refresh", "Movie data reorganized using " + currentDataStructure + ".");
    } else {
        QMessageBox::critical(this, "Error", "Failed to create the selected data structure.");
    }
}
