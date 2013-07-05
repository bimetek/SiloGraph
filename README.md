# Silo Graph

Qt GUI for silo/line/node info displaying.


## Supported Platforms

All major Linux distributions and Mac OS X. Should build with MinGW under
Windows if you handle lib paths correctly, but I don't guarentee anything.


## Usage

Modify `databases.json` and `locations.json` to include currect database info
and credentials, and run the program. Always remember to check `databases.json`
after a new checkout, as it is set to be *assumed unchanged* in Git.


## Dependencies

* Qt 4.8. Qt 5 and other Qt 4 versions should work, but only Qt 4.8 is tested.
* [QJson](http://qjson.sourceforge.net)
