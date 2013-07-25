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
* [Qwt] 6.1.0 or greater.
* [QJson] if using Qt 4.


## Licensing

All source files (files inside `src` directory) in this project, unless
stated otherwise, may be distributed under the terms of Public License version
3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
license should have been included inside the `LICENSE` directory.

**[Qwt]** is used under the terms of the Qwt License, Version 1.0. You may find
contents of the license at [http://qwt.sourceforge.net/qwtlicense.html].

**[QJson]** is used under the terms of the Lesser GNU General Public License version
2.1 (LGPL 2.1). Visit QJson's homepage for more information.

**[QProgressIndicator]** is used and redistributed under the terms of the Lesser GNU
General Public License version 2.1 (LGPL 2.1). You may find more information for
the project at its project page.


[Qwt]: http://qwt.sourceforge.net
[QJson]: http://qjson.sourceforge.net
[http://qwt.sourceforge.net/qwtlicense.html]: http://qwt.sourceforge.net/qwtlicense.html
[QProgressIndicator]: https://github.com/mojocorp/QProgressIndicator