# Silo Graph

Qt GUI for silo/line/node info displaying.


## Supported Platforms

All major Linux distributions and Mac OS X, and Windows with VC++ compiler.
Should build with MinGW too, but I make no guarentees.


## Usage

Modify `databases.json` and `locations.json` to include currect database info
and credentials, and run the program. Always remember to check `databases.json`
after a new checkout, as it is set to be *assumed unchanged* in Git.


## Dependencies

* Qt 4.8 or Qt 5+ with WebKit. Some other Qt 4 versions should work, but they
  are not tested against, and compliance with them may break in the future.
* Appropriate Qt database driver(s) and dependencies.
  See [documentation](http://qt-project.org/doc/qt-4.8/sql-driver.html) for
  details.
* [Qwt](http://qwt.sourceforge.net) 6.1.0 or greater.
* [QJson](http://qjson.sourceforge.net) if using Qt 4.
