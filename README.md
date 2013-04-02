

Todoist Backup
==============

This  is a backup tool for the Todoist task manager (http://todoist.com).
When run, it downloads the list of projects, and then active tasks from all
the project, and puts that, in raw JSON format, in file todoist.txt.
The task history, or labels, are not downloaded.

Building
--------

Before building, you need:

   - Sources of Boost C++ Libraries (http://boost.org)
   - Boost.Build. You can just run "./boostrap.sh" in the root folder of Boost to get the `b2` executable.

Build as follows:

   - Rename `project-config.jam.in` to `project-config.jam`.
   - Edit `project-config.jam` to set your Todoist API token and path to Boost.
   - Run `b2`.

Using
-----

Just run the binary. It will produce file `todoist.txt` with saved projects.
