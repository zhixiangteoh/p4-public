#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <list>
#include <string>
#include <utility>  // std::pair

#include "List.hpp"

class Editor {
    // using TextBuffer = List<char>;
    // using Iterator = List<char>::Iterator;

    using TextBuffer = std::list<char>;
    using Iterator = std::list<char>::iterator;

   public:
    // EFFECTS: Creates a new editor with an empty text buffer, with the
    //          current position at row 1 and column 0.
    Editor() : buffer(), row(1), column(0), index(0) {
        buffer.push_back(0);
        end_sentinel = buffer.begin();
        buffer.push_front(0);
        start_sentinel = buffer.begin();

        cursor = end_sentinel;
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor one position forward and updates the
    //           row and column, unless the cursor is already at the end
    //           of the buffer, in which case this does nothing. Returns
    //           true if the position changed, or false if it did not
    //           (i.e. if the cursor was already at the end of the
    //           buffer).
    bool forward() {
        if (is_at_end()) {
            return false;
        }

        assert(cursor != buffer.end());
        if (*cursor == '\n') {
            ++row;
            column = 0;
        } else {
            ++column;
        }
        ++cursor;
        ++index;
        return true;
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor one position backward and updates the
    //           row and column, unless the cursor is already at the
    //           start of the buffer, in which case this does nothing.
    //           Returns true if the position changed, or false if it did
    //           not (i.e. if the cursor was already at the start of the
    //           buffer).
    bool backward() {
        // assert(cursor != start_sentinel);
        if (is_at_start()) {
            return false;
        }

        --cursor;
        --index;
        assert(cursor != buffer.end());
        if (cursor != start_sentinel && *cursor == '\n') {
            --row;
            column = compute_column();
        } else {
            --column;
        }
        return true;
    }

    // MODIFIES: *this
    // EFFECTS:  Inserts a character in the buffer at the cursor and
    //           updates the current row and column.
    void insert(char c) {
        cursor = buffer.insert(cursor, c);
        ++cursor;
        ++index;
        if (c == '\n') {  // <ENTER>
            ++row;
            column = 0;
        } else {
            ++column;
        }
    }

    // MODIFIES: *this
    // EFFECTS:  Deletes the character from the buffer that is
    //           at cursor. Does nothing if the cursor is at the
    //           end of the buffer. Returns true if a character was
    //           removed, or false if not (i.e. if the cursor was at the
    //           end of the buffer).
    bool remove() {
        // assert(cursor != start_sentinel);
        if (is_at_end()) {
            return false;
        }

        assert(cursor != buffer.end());
        if (*cursor == '\n') {
            column = compute_column();
        }

        cursor = buffer.erase(cursor);
        return true;
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor to the start of the current row (column
    //           0).
    void move_to_row_start() {
        while (column > 0) {
            backward();
        }
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor to the end of the current row (the
    //           newline character that ends the row, or the end of the
    //           buffer if the row is the last one in the buffer).
    void move_to_row_end() {
        while (*cursor != '\n') {
            forward();
        }
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor to the given column in the current row,
    //           or to the end of the row if the row does not have that
    //           many columns.
    void move_to_column(int new_column) {
        assert(new_column >= 0);
        while (*cursor != '\n' && column < new_column) {
            forward();
        }
        while (column > new_column) {
            backward();
        }
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor to the previous row, retaining the
    //           current column if possible. If the previous row is
    //           shorter than the current column, moves to the end of the
    //           previous row (the newline character that ends the row).
    //           Does nothing if the cursor is already in the first row.
    //           Returns true if the position changed, or false if it did
    //           not (i.e. if the cursor was already in the first row).
    bool up() {
        if (row == 1) {
            return false;
        }

        int btm_col = column;
        move_to_row_start();
        backward();
        column = compute_column();
        move_to_column(btm_col);
        return true;
    }

    // MODIFIES: *this
    // EFFECTS:  Moves the cursor to the next row, retaining the current
    //           column if possible. If the next row is shorter than the
    //           current column, moves to the end of the next row (the
    //           newline character that ends the row, or the end of the
    //           buffer if it is the last row). Does nothing if the
    //           cursor is already in the last row.
    //           Returns true if the position changed, or false if it did
    //           not (i.e. if the cursor was already in the last row).
    bool down() {
        if (is_at_end()) {
            return false;
        }
        int top_col = column;
        move_to_row_end();
        forward();
        column = compute_column();
        move_to_column(top_col);
        return true;
    }

    // EFFECTS:  Returns whether the cursor is at the end of the buffer.
    bool is_at_end() const {
        return cursor == end_sentinel;
    }

    // REQUIRES: the cursor is not at the end of the buffer
    // EFFECTS:  Returns the character at the current cursor
    char data_at_cursor() const {
        assert(!is_at_end());
        return *cursor;
    }

    // EFFECTS:  Returns the row of the character at the current cursor.
    int get_row() const {
        return row;
    }

    // EFFECTS:  Returns the column of the character at the current
    //           cursor.
    int get_column() const {
        return column;
    }

    // EFFECTS:  Returns the index of the character at the current cursor
    //           with respect to the entire contents. If the cursor is at
    //           the end of the buffer, returns size() as the index.
    // HINT: Traversing a list is really slow. Instead, use the index
    //       private member variable to keep track of the index and
    //       update it as necessary.
    int get_index() const {
        return index;
    }

    // EFFECTS:  Returns the number of characters in the buffer.
    int size() const {
        return buffer.size();
    }

    // EFFECTS:  Returns the contents of the text buffer as a string.
    std::string stringify() const {
        std::string result;
        auto it = start_sentinel;
        ++it;
        for (; it != end_sentinel; ++it) {
            result.push_back(*it);
        }
        return result;
    }

   private:
    TextBuffer buffer;        // linked list that contains the characters
    Iterator cursor;          // current position within the list
    int row;                  // current row
    int column;               // current column
    int index;                // current index
    Iterator start_sentinel;  // sentinel node at the start of the list
    Iterator end_sentinel;    // sentinel node at the end of the list
    // INVARIANT: cursor points at an actual character in the text, or to
    //            the end sentinel—i.e., cursor \in (start_sentinel, end_sentinel]
    // INVARIANT: row and column are the row and column numbers of the
    //            character the cursor is pointing at
    // INVARIANT: index is the index within the buffer of the character
    //            the cursor is pointing at with respect to the entire
    //            contents, or size() if the cursor is at the end of
    //            the buffer; 0 <= index <= size()

    // EFFECTS: Computes the column of the cursor within the current
    //          row.
    // NOTE: This does not assume that the "column" member variable has
    //       a correct value (i.e. the second INVARIANT can be broken).
    int compute_column() const {
        int col = 0;
        auto cursor_aux = cursor;
        if (cursor_aux == start_sentinel) {
            return 0;
        }

        while (cursor_aux != start_sentinel && *(--cursor_aux) != '\n') {
            ++col;
        }
        if (cursor_aux == start_sentinel) {
            return col - 1;  // start_sentinel is one-past-start
        } else {
            return col;
        }
    }

    // helpers
    bool is_at_start() const {
        return row == 1 && column == 0;
    }
};

#endif
