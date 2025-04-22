#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>
#include <QComboBox>
#include <QListWidget> // For the popup list
#include <QDialog>     // For the popup dialog
#include <QDialogButtonBox>
#include <QStringList>
#include "movie-search.h"

class GenreSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    GenreSelectionDialog(const QStringList& availableGenres, QWidget* parent = nullptr);
    QStringList selectedGenres() const;

private:
    QListWidget* genreListWidget;
    QDialogButtonBox* buttonBox;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void searchButtonClicked();
    void showGenreSelectionDialog();
    void refreshDataStructure();

private:
    QLineEdit* minYearEdit;
    QLineEdit* maxYearEdit;
    QLineEdit* minRuntimeEdit;
    QLineEdit* maxRuntimeEdit;
    QPushButton* searchButton;
    QListWidget* resultsList;
    QPushButton* genreButton; // Button to open genre selection
    QStringList selectedMovieGenres;
    MovieSearch* movieSearch; // Pointer to the base class
    QStringList availableGenres; // Standard IMDb genres
    QComboBox* dataStructureCombo; // New combo box for data structure selection
    QPushButton* refreshButton;    // New button to refresh data structure
    QString currentDataStructure; // To store the currently selected data structure
};

#endif // MAINWINDOW_H
