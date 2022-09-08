#include <iostream>
#include <string>
#include <map>

using namespace std;

int main()
{
  string cmd, name, phone;
  map< string, string > book;

  while( cin >> cmd )
  {
    if ( cmd == "add" )
    {
      cin >> name >> phone;
      book[ name ] = phone;
      cout << "Added" << endl;
    }
    else if ( cmd == "find" )
    {
      cin >> name;
      map< string, string >::const_iterator ifind = book.find( name );

      if ( ifind != book.end() )
        cout << ifind->first << "'s phone is " << ifind->second << endl;
      else
        cout << name << " not found" << endl;
    }
    else if ( cmd == "del" )
    {
      cin >> name;
      book.erase( name );
      cout << "Deleted" << endl;
    }
    else if ( cmd == "view" )
    {
      map< string, string >::const_iterator i;

      for( i = book.begin(); i != book.end() ; ++i )
        cout << i->first << "\t " << i->second << endl;
    }
    else if ( cmd == "quit" )
      return 0;
    else
      cerr << "Bad command '" << cmd << "'" << endl;
  } // while( cin >> cmd )

  return 0;
} // int main()