Corrcheck
=========

Experimental tool to check for silent corruption.

[![Build Status](http://ci.mardev.net/buildStatus/icon?job=Corrcheck)](http://ci.mardev.net/job/Corrcheck/)

Description
-----------
Silent file corruption can and does happen.
Without ECC RAM, it could be happening to you and you wouldn't know it until the file 'could not be read' or something like that.
This tool aims to detect such phenomenons and notify you about the files you care about.
By running this tool, it can create a hidden database file to keep track of the file checksums.
When run again, the tool can detect the corruption of files by simply checksumming them and comparing those hashes with what it last recorded.

What It Cannot Do (Warnings)
-----------------
This utility cannot fix your files.
It does not keep backups, and it simply isn't feasible to recover a file simply from a past checksum.
This tool only aims to alert you when it happens.
To do so, it must be run regularly, and the directory database must be updated regularly for files that change eventually.
It should also be run in the 'verification mode' periodically (depending on how paranoid you are).

License
-------
Corrcheck is licensed under the MIT license.
See the LICENSE file included in the top level of the repository.
