
This program is backup tool for the Todoist task manager (http://todoist.com).
When run, it downloads the list of projects, and then active tasks from all
the project, and puts that, in raw JSON format, in file todoist.txt.
The task history, or labels, are not downloaded.

To build it, you need:

   - source checkout of C++ Boost
   - Boost.Build

Edit project-config.jam to specify the path to the checkout. Also, in the 
same file, specify the todoist API token. Then, run "bjam".

