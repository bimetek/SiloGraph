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


## Licensing

All source files (files inside `src` directory) in this project, unless
stated otherwise, may be distributed under the terms of the GNU Public License
version 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of
the license should have been included inside the `LICENSE` directory.

### Re-deployed Thrid-party Sources

**[QProgressIndicator]** is used and redistributed under the terms of the
Lesser GNU General Public License version 2.1 (LGPL 2.1). You may find more
information for the project at its project page.

**[qjson-backport]** is a collection of JSON-related classes extracted from Qt
5, and is modified and redistributed under the terms of the GNU Public License
version 3.0 (GPL 3.0). Backporting efforts go to [@5in4] on GitHub.

### Libraries Bundled with the Deployed Binaries

**[Qwt]** is used under the terms of the Qwt License, Version 1.0. You may find
contents of the license at [http://qwt.sourceforge.net/qwtlicense.html].

**MySQL Connector/C (libmysql)** is used under the terms of the GNU Public
License version 3.0 (GPL 3.0). Detailed information may be found at
[http://www.mysql.com].


[Qwt]: http://qwt.sourceforge.net
[http://qwt.sourceforge.net/qwtlicense.html]: http://qwt.sourceforge.net/qwtlicense.html
[QProgressIndicator]: https://github.com/mojocorp/QProgressIndicator
[qjson-backport]: https://github.com/5in4/qjson-backport
[http://www.mysql.com]: http://www.mysql.com
[@5in4]: https://github.com/5in4
