/***************************************
 ** E0: a hacky text editor front-end **
 **                                   **
 **         by William Lovas          **
 **         and Robert Simmons        **
 **          ported to C++ by         **
 **          Saquib Razak             **
 **                                   **
 ***************************************/

#include <iostream>
#include <string>
#include <utility> // pair
#include <ncurses.h>
#include "Editor.hpp"

using namespace std;

void render_topbar(WINDOW *window) {
  werase(window);
  for (int i = getbegx(window); i < getmaxx(window); ++i)
    waddch(window, ' ');
  wmove(window, 0, 1);
  waddstr(window, "E0, the minimalist editor -- ^X to exit, ^L to refresh");
}

void render_botbar(Editor &editor, WINDOW *window) {
  werase(window);
  for (int i = getbegx(window); i < getmaxx(window); ++i)
    waddch(window, ' '|A_STANDOUT);
  wmove(window, 0, 1);
  wattron(window, A_REVERSE);

  string msg = "Position (";
  msg = msg + std::to_string(editor.get_row());
  msg = msg +  ",";
  msg = msg + std::to_string(editor.get_column());
  msg = msg+ ")";
  waddstr(window, msg.c_str());
  wattroff(window, A_REVERSE);
}

void render_buf(Editor &editor, WINDOW *window) {
  wmove(window, 0, 0);
  werase(window);

  int cursor = 0;
  pair<string, int> data = editor.stringify();
  cursor = data.second;
  for (int i = 0; i < static_cast<int>(data.first.size()); ++i) {
    char c = data.first[i];
    // The display character is either ' ' (if it's a newline) or the char
    // The display character is what gets highlighted if we're at the point
    int display = c == '\n' ? ' ' : c;
    if (i == cursor) display = display|A_STANDOUT;
    int x, y;
    getyx(window, y, x);
    if (y == getmaxy(window) - 1) {
      // Special corner cases: last line of the buffer
      if (c != '\n' && x < getmaxx(window) - 1) {
        waddch(window, display); // Show a regular character (common case)
      } else {
        if (c == '\n') waddch(window, display);
        getyx(window,y,x);
        while (x != getmaxx(window) - 1){
          waddch(window, ' ');
          getyx(window, y, x);
        }
        waddch(window, '>');
        return;
      }
    } else {
      // Normal cases: in the buffer
      getyx(window, y, x);
      if (c != '\n' && x < getmaxx(window) - 1) {
        waddch(window, display); // Show a regular character (common case)
      } else if (c == '\n' && x < getmaxx(window) - 1) {
        waddch(window, display); // Newline (common case)
        waddch(window, '\n');
      } else if (c == '\n') {
        waddch(window, display); // Newline (edge case, newline at end of line)
      } else {
        waddch(window, '\\');
        waddch(window, display); // Wrap to the next line
      }
    }
  }

  // We're at the end of the buffer. This only matters if end = cursor
  if (cursor == -1) {
    waddch(window,' '|A_STANDOUT);
  }
}

/** main: look the other way if you've ever programmed using curses **/
int main() {
  cout << "Starting\n";
  WINDOW *mainwin = initscr();
  cbreak();
  noecho();
  keypad(mainwin, true);
  int vis = curs_set(0);

  int ncols = getmaxx(mainwin);
  int nlines = getmaxy(mainwin);
  int begx = getbegx(mainwin);
  int begy = getbegy(mainwin);

  WINDOW *canvas = subwin(mainwin,
                          nlines - 3  /* lines: save 3 for status info */,
                          ncols       /* cols: same as main window */,
                          begy + 1    /* begy: skip top status bar */,
                          begx        /* begx: same as main window */);
  WINDOW *topbar = subwin(mainwin, 1 /* lines */, ncols, begy, begx);
  WINDOW *botbar = subwin(mainwin, 1 /* lines */, ncols, nlines - 2, begx);
  Editor editor;
  render_topbar(topbar);

  while (true) {
    render_buf(editor, canvas);
    wrefresh(canvas);
    render_botbar(editor, botbar);
    wrefresh(botbar);

    int c = getch();
    if (c == 24) /* ^X */ { break; }
    else if (c == 12) /* ^L */ {
      wclear(mainwin);
      render_topbar(topbar);
      wrefresh(mainwin);
    }
    else if (c == KEY_BACKSPACE || c == 127) { editor.remove(); }
    else if (c == KEY_LEFT)  { editor.backward(); }
    else if (c == KEY_RIGHT) { editor.forward(); }
    else if (c == KEY_ENTER) { editor.insert('\n'); }
    else if (c == KEY_UP) { editor.up();}
    else if (c == KEY_DOWN) { editor.down();}
    else if (c == KEY_HOME) { editor.move_to_row_start();}
    else if (c == KEY_END) { editor.move_to_row_end();}
    else if (0 < c && c < 127) { editor.insert(c);}
  }

  curs_set(vis);
  endwin();

  cout << "thanks for flying E !" << endl; // <- wing commander homage -wjl
}
