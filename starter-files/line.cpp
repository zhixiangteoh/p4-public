/*
 * Visualization for testing the text buffer implementation.
 *
 * Note that this is just for testing! This code will only work once
 * you have all the functionality of the Editor class
 * Originally written for 15-122 Principles of Imperative Computation
 * Ported to C++ by Saquib Razak for EECS 280. */

#include <stdio.h>
#include <iostream>
#include <string>
#include "Editor.hpp"

using namespace std;

void visualize_gapbuf(Editor &editor) {
  int left = 0;
  while (editor.backward()) {
    --left;
  }
  while (!editor.is_at_end()) {
    if (left == 0) {
      cout << "|";
    }
    char c = editor.data_at_cursor();
    if (c == '\n') {
      cout << "\\n";
    } else {
      cout << c;
    }
    ++left;
    editor.forward();
  }

  for (int i = 0; i < left; ++i) {
    editor.backward();
  }
  if (editor.is_at_end()) {
    cout << "|";
  }
  cout << "\t:(" << editor.get_row() << "," << editor.get_column() << " )";
}

void process_char(Editor &editor, char c)  {
  if (c == '<') {
    cout << "left  : ";
    editor.backward();
  } else if (c == '>') {
    cout << "right : ";
    editor.forward();
  } else if (c == '^') {
    cout << "up    : ";
    editor.up();
  } else if (c == '!') {
    cout << "down  : ";
    editor.down();
  } else if (c == '#') {
    cout << "del   : ";
    if (editor.backward()) {
      editor.forward();
      editor.remove();
    }
  } else if (c == '[') {
    cout << "home  : ";
    editor.move_to_row_start();
  } else if (c == ']') {
    cout << "end   : ";
    editor.move_to_row_end();
  } else if (c == '@') {
    cout << "enter : ";
    editor.insert('\n');
  } else {
    cout << "add   : ";
    editor.insert(c);
  }
}

void process_string(Editor &editor, string s) {
  int limit = s.size();
  for (int i = 0; i < limit; i++) {
    process_char(editor, s[i]);
    visualize_gapbuf(editor);
    cout << "\n";
  }
}

void test() {
  Editor editor;
  cout << "LINE Is Not an Editor -- it visualizes the Editor contents"
       << " in a single line.\n"
       << "The '<' character mimics going backwards (left arrow key)\n"
       << "The '>' character mimics going forwards (right arrow key)\n"
       << "The '#' character mimics deletion (backspace key)\n"
       << "The '^' character mimics going up (up arrow key)\n"
       << "The '!' character mimics going down (down arrow key)\n"
       << "The '[' character mimics going to the start of the line"
       << " (home key)\n"
       << "The ']' character mimics going to the end of the line"
       << " (end key)\n"
       << "The '@' character mimics a newline (enter key)\n"
       << "All other characters just insert that character\n\n"
       << "Give initial input (empty line quits):"
       << endl;

  string s;
  while (getline(cin, s) && s != "") {
    cout << "STARTING\nstart : ";
    visualize_gapbuf(editor);
    cout << "\n";
    process_string(editor, s);
    cout << "\n";

    cout << "Done. More input? (empty line quits):" << endl;
  }
}

int main() {
  test();
  cout << "Goodbye." << endl;
}
