project(
  'movie-search', 'cpp',
  default_options: ['cpp_std=c++17'])

qt5_ui_sources = [
  'mainwindow.ui',
]

qt5_moc_headers = [
  'mainwindow.h',
]

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Widgets'])
qt5_ui = qt5.compile_ui(sources: qt5_ui_sources)
qt5_moc = qt5.compile_moc(headers: qt5_moc_headers)

sources = [
  qt5_ui,
  qt5_moc,
  'data.cpp',
  'mainwindow.cpp',
  'main.cpp',
]

executable('movie-search', sources, dependencies: qt5_dep)
