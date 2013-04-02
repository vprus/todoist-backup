// Copyright 2009 Vladimir Prus
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <urdl/istream.hpp>

#include "tinyjson.hpp"
using namespace json;

#include <boost/progress.hpp>
#include <boost/format.hpp>
using namespace boost;

#include <cxxabi.h>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string get(const std::string& url)
{
    urdl::istream is;
    is.exceptions(ios_base::failbit|ios_base::badbit);
    is.open(url);
    is.exceptions(ios_base::badbit);

    string result;

    while (is)
    {   
        char buffer[2048] = "";
        is.read(buffer, sizeof(buffer));
        result.append(buffer, is.gcount());
    }
    return result;
}

int main(int argc, char* argv[])
{
    try
    {
        string result;

        cout << "Reading project list...";

        string projects = get("http://todoist.com/API/getProjects?token=" TOKEN);
        result += "{\n";
        result += "\"projects\": ";
        result += projects;

        json::grammar<char>::variant v
            = json::parse(projects.begin(), projects.end());

        if(v->type() != typeid(json::grammar<char>::array))
        {
            throw runtime_error("Unexpected data for project list recieved");
        }

        json::grammar<char>::array const & a = 
            any_cast< json::grammar<char>::array >(*v);

        cout << "\nReading tasks...";
        
        progress_display progress(a.size());

        for(json::grammar<char>::array::const_iterator it = a.begin(); 
            it != a.end(); ++it)
        {
            if ((*it)->type() != typeid(json::grammar<char>::object))
            {            
                int status;
                string name;
                char *xname = abi::__cxa_demangle((*it)->type().name(), 0, 0, &status);                 
                if(status) {
                    name = v->type().name();
                } else {
                    name = xname;
                    free(xname); 
                }
                throw logic_error("Unexpected type: " + name);
            }

            json::grammar<char>::object p = 
                any_cast< json::grammar<char>::object >(**it);
            
            assert(p.count("id"));
            int id = any_cast<int>(*p["id"]);

            result += ",\n";

            string items = get((format(
                                    "http://todoist.com/API/getUncompletedItems?project_id=%1%&token=%2%") % id % TOKEN).str());
            result += (format("\"%1%\": ") % id).str();
            result += items;

            progress += 1;
        }
        result += "\n}\n";

        ofstream out("todoist.txt");
        out << result;
        cout << "All data was saved to 'todoist.txt'\n";
    }
    catch (std::exception& e)
    {
        cerr << "error: " << e.what() << "\n";
    }
}
