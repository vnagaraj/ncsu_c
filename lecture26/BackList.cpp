#include <iostream>

using namespace std;

/** Resizable list that reports items backward, where the most
    recently pushed item shows up at index zero and the oldest item
    shows up at index size-1.
*/
class BackList {
  /* Number of items actually in the list.. */
  int count;

  /* Capacity of the array used to store the list (typically larger
     than count). */
  int capacity;

  /* Dynamically allocated memory for the list of values. */
  int *seq;

public:
  /** Make an empty list. */
  BackList()
  {
    // Make an empty list, with room for 5 items.
    capacity = 5;
    count = 0;

    // Look, we're using the new operator.
    seq = new int [ capacity ];
  }

  /** Copy constructor, to make a BackList given a BackList.
      Fill in the fields of this new BackList, so it looks just like
      the given list, a. */
  BackList( BackList const &a )
  {
    delete [] seq;
    seq = new int [a.capacity];
    for (int i=0; i< a.count; i++){
      seq[i] = a.seq[i];
    }
    count = a.count;
  }

  /** Free memory for this list. */
  ~BackList()
  {
    delete [] seq;
  }

  /** Return number of items in the list. */
  int size()
  {
    return count;
  }

  /** Push a new item onto the end of this list (although the [] operator will make
      it look like it got pushed at the start. */
  void push( int v )
  {
    if ( count >= capacity ) {
      capacity = 2 * capacity;
      int * temp = new int [capacity];
      for (int i=0; i< count; i++){
        temp[i] = seq[i];
      }
      delete [] seq;
      seq = temp;
    }

    seq[ count++ ] = v;
  }

  /** Return a reference to the item at the given index.  The most recently pushed itme
      will appear at index 0. */
  int &operator[]( int idx )
  {
    return seq[ count - idx - 1 ];
  }
};

/** Let us print a list using the << operator.  This isn't part of the
    BackList class, so it can't access the fileds of list directly.  You'll
    have to print out the list contents using the member functions and
    overloaded operators provided by list. */
ostream &operator<<( ostream &out, BackList &list )
{
  for (int i=0; i < list.size(); i++){
    out << list[i] << " ";
  }
  return out;
}

int main()
{
  // Make a list with a few items on it.
  BackList list1;

  list1.push( 10 );
  list1.push( 15 );
  list1.push( 20 );
  list1.push( 25 );
  list1.push( 30 );
  list1.push( 35 );
  list1.push( 40 );

  // Print the list.
  cout << "List1: " << list1 << endl;

  {
    // Use the copy constructor to make a copy of list1.
    BackList list2 = list1;

    // See, return by reference lets read or write elements via the [] operator.
    list2[ 0 ] = 41;
    list1[ 0 ] = 39;

    list2.push( 45 );

    // list2 is a copy of list1, so changes to one don't affect the other.
    cout << endl;
    cout << "List1: " << list1 << endl;
    cout << "List2: " << list2 << endl;
  }

  // Now, list2 is gone, but list is still around
  cout << endl;
  cout << "List1: " << list1 << endl;


  return 0;
}
