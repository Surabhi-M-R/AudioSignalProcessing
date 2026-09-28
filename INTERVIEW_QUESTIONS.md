# Music Playlist Manager — Interview Questions & Complete Explanation Guide

This document covers **every concept** used in the `Audio.cpp` program. Each section maps a topic to potential interview questions, model answers, and references to the exact lines of code in the program where the concept is applied.

---

## Table of Contents

1. [Data Structures](#1-data-structures)
    - 1.1 [Linked Lists (Singly vs Doubly)](#11-linked-lists-singly-vs-doubly)
    - 1.2 [Doubly Linked List Operations](#12-doubly-linked-list-operations)
    - 1.3 [Nested / Hierarchical Data Structures (List of Lists)](#13-nested--hierarchical-data-structures-list-of-lists)
2. [Object-Oriented Programming (OOP)](#2-object-oriented-programming-oop)
    - 2.1 [Classes and Objects](#21-classes-and-objects)
    - 2.2 [Constructors and Destructors](#22-constructors-and-destructors)
    - 2.3 [Encapsulation](#23-encapsulation)
    - 2.4 [Composition (Has-A Relationship)](#24-composition-has-a-relationship)
3. [Pointers and Dynamic Memory](#3-pointers-and-dynamic-memory)
    - 3.1 [Pointers Basics](#31-pointers-basics)
    - 3.2 [new and delete (Heap Allocation)](#32-new-and-delete-heap-allocation)
    - 3.3 [nullptr and Null Checks](#33-nullptr-and-null-checks)
    - 3.4 [Memory Leaks and Prevention](#34-memory-leaks-and-prevention)
4. [File I/O](#4-file-io)
    - 4.1 [Writing to Files (ofstream)](#41-writing-to-files-ofstream)
    - 4.2 [Reading from Files (ifstream)](#42-reading-from-files-ifstream)
    - 4.3 [String Parsing (Delimiters)](#43-string-parsing-delimiters)
5. [String Manipulation](#5-string-manipulation)
    - 5.1 [std::string Operations](#51-stdstring-operations)
    - 5.2 [Case-Insensitive Comparison](#52-case-insensitive-comparison)
    - 5.3 [String to Integer Conversion](#53-string-to-integer-conversion)
6. [Input/Output and Stream Handling](#6-inputoutput-and-stream-handling)
    - 6.1 [cin, cout, getline](#61-cin-cout-getline)
    - 6.2 [Input Validation and Error Recovery](#62-input-validation-and-error-recovery)
    - 6.3 [Buffer Flushing (cin.ignore)](#63-buffer-flushing-cinignore)
7. [Control Flow](#7-control-flow)
    - 7.1 [switch-case Statements](#71-switch-case-statements)
    - 7.2 [do-while Loop](#72-do-while-loop)
    - 7.3 [Event Loop / Game Loop Pattern](#73-event-loop--game-loop-pattern)
8. [Windows API and Multimedia](#8-windows-api-and-multimedia)
    - 8.1 [MCI (Media Control Interface)](#81-mci-media-control-interface)
    - 8.2 [Keyboard Input (_kbhit, _getch)](#82-keyboard-input-_kbhit-_getch)
    - 8.3 [SetConsoleOutputCP and Encoding](#83-setconsoleoutputcp-and-encoding)
    - 8.4 [#pragma comment(lib)](#84-pragma-commentlib)
9. [Algorithm Design Patterns](#9-algorithm-design-patterns)
    - 9.1 [Linear Search](#91-linear-search)
    - 9.2 [Traversal](#92-traversal)
    - 9.3 [State Machine (Playback States)](#93-state-machine-playback-states)
10. [Miscellaneous C++ Concepts](#10-miscellaneous-c-concepts)
    - 10.1 [Preprocessor Directives](#101-preprocessor-directives)
    - 10.2 [using namespace std](#102-using-namespace-std)
    - 10.3 [Ternary Operator](#103-ternary-operator)
    - 10.4 [Type Casting with to_string](#104-type-casting-with-to_string)

---

## 1. Data Structures

### 1.1 Linked Lists (Singly vs Doubly)

**Q: What is a linked list? How is it different from an array?**

**A:** A linked list is a linear data structure where elements (nodes) are stored in non-contiguous memory locations. Each node contains data and a pointer to the next node. Unlike arrays, linked lists:
- Don't require contiguous memory allocation
- Can grow/shrink dynamically at runtime
- Have O(1) insertion/deletion at known positions (no shifting needed)
- Have O(n) access time (no random indexing like `arr[5]`)

**Q: What is the difference between a singly and a doubly linked list?**

**A:**

| Feature | Singly Linked List | Doubly Linked List |
|---------|-------------------|-------------------|
| Pointers per node | 1 (`next`) | 2 (`next` + `prev`) |
| Traversal | Forward only | Forward and backward |
| Deletion | Need previous node reference | Self-sufficient |
| Memory | Less per node | More per node |
| Use case | Simple sequential access | Navigation (prev/next) |

**Where in code:** The `SongNode` class (lines 19-34) implements a doubly linked list node:
```cpp
class SongNode {
public:
    string title;
    string filePath;
    int playCount;
    SongNode* next;  // forward link
    SongNode* prev;  // backward link
};
```

**Q: Why was a doubly linked list chosen over a singly linked list for this program?**

**A:** Because the music player needs **bidirectional navigation** — playing the next song (`next` pointer) AND the previous song (`prev` pointer). With a singly linked list, going backward would require re-traversing from the head, which is O(n). With a doubly linked list, `playPrevious()` is O(1).

---

### 1.2 Doubly Linked List Operations

**Q: Explain how insertion at the end (tail) works in a doubly linked list.**

**A:** The `addSong()` method (lines 60-73) demonstrates this:
```cpp
void addSong(string title, string path) {
    SongNode* newSong = new SongNode(title, path);

    if (head == nullptr) {       // Case 1: List is empty
        head = tail = newSong;   // New node is both head and tail
    }
    else {                       // Case 2: List has nodes
        tail->next = newSong;    // Old tail points forward to new node
        newSong->prev = tail;    // New node points backward to old tail
        tail = newSong;          // Update tail pointer
    }
    songCount++;
}
```

**Step-by-step for Case 2 (adding "Song C" to [A <-> B]):**
1. `tail->next = newSong` → B's next now points to C
2. `newSong->prev = tail` → C's prev now points to B
3. `tail = newSong` → tail pointer now points to C
4. Result: `A <-> B <-> C`, tail = C

**Q: Explain how deletion works in a doubly linked list. What edge cases must be handled?**

**A:** The `deleteSong()` method (lines 248-281) handles three cases:

```cpp
// Case 1: Deleting the HEAD node
if (temp == head) {
    head = temp->next;
    if (head != nullptr) head->prev = nullptr;
}
// Case 2: Deleting the TAIL node
else if (temp == tail) {
    tail = temp->prev;
    if (tail != nullptr) tail->next = nullptr;
}
// Case 3: Deleting a MIDDLE node
else {
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
}
delete temp;
songCount--;
```

**The four edge cases:**
1. **Deleting the head** — Update head to the next node, set its `prev` to nullptr
2. **Deleting the tail** — Update tail to the previous node, set its `next` to nullptr
3. **Deleting a middle node** — Bypass the node by connecting its neighbors to each other
4. **Deleting the currently playing song** — Must also stop audio playback first (line 254)

**Q: What is the time complexity of insertion, deletion, and search in a doubly linked list?**

**A:**

| Operation | Time Complexity | Explanation |
|-----------|----------------|-------------|
| Insert at tail | O(1) | Direct access via `tail` pointer |
| Insert at head | O(1) | Direct access via `head` pointer |
| Delete by value | O(n) | Must search (traverse) to find the node first |
| Search by value | O(n) | Must traverse from head to find matching node |
| Access by index | O(n) | No random access, must traverse |
| Next/Previous | O(1) | Direct pointer follow |

---

### 1.3 Nested / Hierarchical Data Structures (List of Lists)

**Q: What is a "list of lists" and when would you use one?**

**A:** A list of lists is a hierarchical data structure where each node in the outer list contains (or points to) an entire inner list. It's used when you need to group related collections together.

**In this program:**
```
MasterPlaylist (outer doubly linked list)
├── PlaylistNode "Chill_Vibes" → SmallPlaylist (inner doubly linked list)
│   ├── SongNode "Sleepy Cat"
│   ├── SongNode "Serene View"
│   └── SongNode "Lullaby Night"
├── PlaylistNode "Nature_Sounds" → SmallPlaylist
│   ├── SongNode "Forest Walk"
│   ├── SongNode "Forest Treasure"
│   └── SongNode "Zanarkand Forest"
└── PlaylistNode "Island_Party" → SmallPlaylist
    ├── SongNode "Island Beat"
    ├── SongNode "Piano Reflections"
    └── SongNode "Voxscape"
```

**Where in code:** `PlaylistNode` (lines 358-376) contains a pointer to a `SmallPlaylist`:
```cpp
class PlaylistNode {
public:
    string name;
    SmallPlaylist* songs;     // Pointer to an ENTIRE inner linked list
    PlaylistNode* next;
    PlaylistNode* prev;
};
```

**Q: What is the difference between this nested structure and a 2D array?**

**A:**

| Feature | List of Lists | 2D Array |
|---------|--------------|----------|
| Size | Dynamic per list | Fixed dimensions |
| Memory | Non-contiguous | Contiguous |
| Inner sizes | Can differ (jagged) | All rows same length |
| Insert/Delete | O(1) at known positions | O(n) due to shifting |

---

## 2. Object-Oriented Programming (OOP)

### 2.1 Classes and Objects

**Q: What is a class in C++? What is the difference between a class and an object?**

**A:** A class is a blueprint/template that defines the properties (data members) and behaviors (member functions) of a type. An object is a specific instance of that class created at runtime.

**In this program, there are 4 classes:**

| Class | Purpose | Lines |
|-------|---------|-------|
| `SongNode` | Represents a single audio track | 19-34 |
| `SmallPlaylist` | Manages a doubly linked list of songs | 37-355 |
| `PlaylistNode` | Links a playlist name to its SmallPlaylist | 358-376 |
| `MasterPlaylist` | Manages a doubly linked list of playlists | 379-636 |

**Object creation example (line 657):**
```cpp
MasterPlaylist myMusic;  // 'myMusic' is an OBJECT of class MasterPlaylist
```

### 2.2 Constructors and Destructors

**Q: What is a constructor? What is a destructor? Why are both important?**

**A:**
- A **constructor** is a special function called automatically when an object is created. It initializes the object's state.
- A **destructor** (prefixed with `~`) is called automatically when an object goes out of scope or is deleted. It cleans up resources.

**Constructor example — `SmallPlaylist()` (lines 48-57):**
```cpp
SmallPlaylist() {
    head = nullptr;
    tail = nullptr;
    currentPlaying = nullptr;
    songCount = 0;
    isPlaying = false;
    isPaused = false;
    isLooped = false;
    volume = 100;
}
```

**Destructor example — `~SmallPlaylist()` (lines 346-354):**
```cpp
~SmallPlaylist() {
    stopPlayback();                    // Stop any audio playback
    SongNode* current = head;
    while (current != nullptr) {       // Walk through every node
        SongNode* next = current->next;
        delete current;                // Free each node's memory
        current = next;
    }
}
```

**Q: What happens if you don't write a destructor for a class that uses `new`?**

**A:** Memory leak. The heap memory allocated by `new` will never be freed. The program will consume increasingly more RAM over time. In this program, if `~SmallPlaylist()` didn't exist, every `SongNode` created by `addSong()` would leak.

**Q: What is the difference between a parameterized and a default constructor?**

**A:**
- **Default constructor**: No parameters. Example: `SmallPlaylist()` (line 48)
- **Parameterized constructor**: Takes arguments. Example: `SongNode(string t, string path)` (line 27)

```cpp
// Parameterized constructor
SongNode(string t, string path) {
    title = t;
    filePath = path;
    playCount = 0;
    next = nullptr;
    prev = nullptr;
}
```

---

### 2.3 Encapsulation

**Q: What is encapsulation? Is it used properly in this program?**

**A:** Encapsulation is bundling data and the methods that operate on that data into a single unit (class), and restricting direct access to some of the object's components. 

In this program, all class members are `public:`, which means encapsulation is **partial**. The data and methods are grouped together (good), but there's no access restriction (could be improved). In a production codebase, member variables like `head`, `tail`, and `volume` should be `private`, with public getter/setter methods.

**Improved version (how an interviewer would expect it):**
```cpp
class SmallPlaylist {
private:
    SongNode* head;          // Hidden from outside
    SongNode* tail;
    int volume;
public:
    void addSong(...);       // Controlled interface
    int getVolume() { return volume; }
    void setVolume(int v) { if(v >= 0 && v <= 100) volume = v; }
};
```

---

### 2.4 Composition (Has-A Relationship)

**Q: What is composition in OOP? Give an example from this codebase.**

**A:** Composition is when one class **contains an instance of another class** as a member. It represents a "has-a" relationship.

```cpp
class PlaylistNode {
    SmallPlaylist* songs;   // PlaylistNode HAS-A SmallPlaylist
};

class MasterPlaylist {
    PlaylistNode* head;     // MasterPlaylist HAS PlaylistNodes
};
```

The `PlaylistNode` *has-a* `SmallPlaylist`. When a `PlaylistNode` is destroyed, its destructor (line 373) also destroys the `SmallPlaylist`:
```cpp
~PlaylistNode() {
    delete songs;   // Destroying the owner destroys the owned object
}
```

**Q: What is the difference between composition and inheritance?**

**A:**
- **Composition (Has-A):** `PlaylistNode` *has-a* `SmallPlaylist`. Used in this program.
- **Inheritance (Is-A):** `Dog` *is-a* `Animal`. Not used in this program.

---

## 3. Pointers and Dynamic Memory

### 3.1 Pointers Basics

**Q: What is a pointer? Why are pointers essential for linked lists?**

**A:** A pointer is a variable that stores the memory address of another variable. Pointers are essential for linked lists because nodes are scattered across heap memory — the only way to connect them is by storing each node's address in the previous node's pointer.

```cpp
SongNode* next;    // Stores the ADDRESS of the next SongNode
SongNode* prev;    // Stores the ADDRESS of the previous SongNode
```

**Q: What is the `->` (arrow) operator?**

**A:** The arrow operator dereferences a pointer and accesses a member in one step. `ptr->member` is equivalent to `(*ptr).member`.

```cpp
temp->title        // Access the 'title' member of the node pointed to by 'temp'
temp->next         // Access the 'next' pointer of the node pointed to by 'temp'
```

---

### 3.2 new and delete (Heap Allocation)

**Q: What is the difference between stack and heap memory?**

**A:**

| Feature | Stack | Heap |
|---------|-------|------|
| Allocation | Automatic (compiler managed) | Manual (`new` / `delete`) |
| Lifetime | Until function returns | Until explicitly `delete`d |
| Speed | Faster | Slower |
| Size | Limited (typically 1-8 MB) | Large (limited by RAM) |
| Example | `int x = 5;` | `SongNode* s = new SongNode(...);` |

**In this program:** All nodes (`SongNode`, `PlaylistNode`) are allocated on the **heap** using `new` because their lifetime must extend beyond the function call that created them:
```cpp
SongNode* newSong = new SongNode(title, path);   // Heap allocation (line 61)
// ...
delete temp;                                       // Heap deallocation (line 271)
```

**Q: What would happen if you used stack allocation instead of heap for linked list nodes?**

**A:** The nodes would be destroyed when the function returns, leaving dangling pointers. Example:
```cpp
void addSong() {
    SongNode node("title", "path");   // Stack allocated
    tail->next = &node;               // Pointer to stack variable
}  // 'node' is destroyed here — tail->next is now a DANGLING POINTER!
```

---

### 3.3 nullptr and Null Checks

**Q: What is `nullptr`? Why is checking for `nullptr` important?**

**A:** `nullptr` is the C++11 keyword representing a null pointer (a pointer that doesn't point to any valid memory). Checking for it prevents **segmentation faults** (crashes from accessing invalid memory).

**Critical null checks in this program:**
```cpp
if (head == nullptr)             // Is the list empty? (line 63)
if (currentPlaying == nullptr)   // Is anything playing? (line 132)
if (temp->next == nullptr)       // Is this the last song? (line 138)
if (temp->prev == nullptr)       // Is this the first song? (line 161)
```

**Q: What is a dangling pointer? How does this program avoid it?**

**A:** A dangling pointer points to memory that has been freed. This program avoids it by setting pointers to `nullptr` after deletion:
```cpp
if (temp == currentPlaying) {
    stopPlayback();
    currentPlaying = nullptr;    // Avoid dangling pointer (line 255)
}
```

---

### 3.4 Memory Leaks and Prevention

**Q: What is a memory leak? How does this program prevent them?**

**A:** A memory leak occurs when heap-allocated memory is never freed. Over time, the program consumes more and more RAM.

**Prevention in this program — Destructors walk through and delete every node:**
```cpp
~SmallPlaylist() {               // Lines 346-354
    stopPlayback();
    SongNode* current = head;
    while (current != nullptr) {
        SongNode* next = current->next;    // Save next BEFORE deleting
        delete current;                     // Free current node
        current = next;                     // Move to saved next
    }
}
```

**Key insight:** You must save `current->next` BEFORE deleting `current`, because after deletion, accessing `current->next` is undefined behavior.

---

## 4. File I/O

### 4.1 Writing to Files (ofstream)

**Q: How do you write data to a file in C++?**

**A:** Using `ofstream` (output file stream). The `saveToFile()` method (lines 284-301):
```cpp
bool saveToFile(string filename) {
    ofstream file(filename);           // Create/open file for writing
    if (!file.is_open()) return false; // Check if file opened successfully

    file << songCount << endl;         // Write song count
    file << isLooped << endl;          // Write loop state

    SongNode* temp = head;
    while (temp) {
        // Write each song as: title|path|playCount
        file << temp->title << "|" << temp->filePath << "|" << temp->playCount << endl;
        temp = temp->next;
    }

    file.close();                      // Close the file
    return true;
}
```

**Output file format:**
```
3
0
Sleepy Cat|C:\Users\...\mixkit-sleepy-cat-135.mp3|5
Serene View|C:\Users\...\mixkit-serene-view-443.mp3|3
Lullaby Night|C:\Users\...\mixkit-lullaby-night-531.mp3|1
```

---

### 4.2 Reading from Files (ifstream)

**Q: How do you read structured data from a file in C++?**

**A:** Using `ifstream` (input file stream). The `loadFromFile()` method (lines 304-344):
```cpp
bool loadFromFile(string filename) {
    ifstream file(filename);            // Open file for reading
    if (!file.is_open()) return false;

    int count;
    file >> count;                      // Read integer (song count)
    file >> isLooped;                   // Read boolean (loop state)
    file.ignore();                      // Discard leftover newline

    for (int i = 0; i < count; i++) {
        string line;
        getline(file, line);            // Read entire line
        // Parse the line using '|' delimiter...
    }

    file.close();
    return true;
}
```

---

### 4.3 String Parsing (Delimiters)

**Q: How do you split a string by a delimiter in C++?**

**A:** Using `find()` and `substr()`. The file loader parses `title|path|playCount` lines (lines 319-325):
```cpp
string line = "Sleepy Cat|C:\\Music\\song.mp3|5";

size_t pos1 = line.find('|');     // First '|' position = 10
size_t pos2 = line.rfind('|');    // Last '|' position = 30

string title = line.substr(0, pos1);                    // "Sleepy Cat"
string path  = line.substr(pos1 + 1, pos2 - pos1 - 1);  // "C:\\Music\\song.mp3"
int plays    = stoi(line.substr(pos2 + 1));              // 5
```

**Q: What is the difference between `find()` and `rfind()`?**

**A:** `find()` searches forward (left-to-right) and returns the **first** occurrence. `rfind()` searches backward (right-to-left) and returns the **last** occurrence. This distinction is critical when the delimiter appears multiple times.

---

## 5. String Manipulation

### 5.1 std::string Operations

**Q: What string operations are used in this program?**

**A:**

| Operation | Example | Line |
|-----------|---------|------|
| Concatenation (`+`) | `"open \"" + filePath + "\" type mpegvideo alias SONG"` | 79 |
| Comparison (`==`) | `temp->title == title` | 107 |
| Substring (`substr`) | `line.substr(0, pos1)` | 323 |
| Find (`find`) | `line.find('\|')` | 319 |
| Reverse Find (`rfind`) | `line.rfind('\|')` | 320 |
| Length (`length`) | `filename.length() < 4` | 569 |
| c_str() | `openCmd.c_str()` | 80 |

**Q: Why is `.c_str()` needed when calling `mciSendStringA()`?**

**A:** `mciSendStringA()` is a C-style Windows API function that expects a `const char*` (C-string), not a `std::string`. The `.c_str()` method converts a C++ string to a null-terminated C character array.

---

### 5.2 Case-Insensitive Comparison

**Q: How do you do case-insensitive string comparison in C++?**

**A:** Convert both strings to lowercase (or uppercase) before comparing. The `toLower()` helper function (lines 13-16) uses `std::transform`:
```cpp
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Usage in findPlaylist() (lines 409-420):
if (toLower(temp->name) == toLower(searchName))  // "Chill_Vibes" == "chill_vibes"
```

**Q: What does `std::transform` do?**

**A:** `std::transform` applies a function to each element in a range. Here it applies `::tolower` to every character from `s.begin()` to `s.end()`, writing results back starting at `s.begin()`.

---

### 5.3 String to Integer Conversion

**Q: How do you convert a string to an integer in C++?**

**A:** Using `stoi()` (string-to-integer), introduced in C++11:
```cpp
int plays = stoi(line.substr(pos2 + 1));   // "5" → 5  (line 325)
```

And the reverse — integer to string using `to_string()`:
```cpp
string volCmd = "setaudio SONG volume to " + to_string(volume * 10);  // line 91
```

---

## 6. Input/Output and Stream Handling

### 6.1 cin, cout, getline

**Q: What is the difference between `cin >>` and `getline()`?**

**A:**

| Feature | `cin >>` | `getline()` |
|---------|----------|-------------|
| Reads | Single word (stops at whitespace) | Entire line (stops at newline) |
| Delimiter | Space, tab, newline | Newline only |
| Use case | Numbers, single words | Full sentences, file paths |

**In this program:**
```cpp
cin >> choice;                        // Read a single integer (line 700)
getline(cin, playlistName);           // Read full playlist name (line 711)
getline(cin, path);                   // Read full file path with spaces (line 721)
```

---

### 6.2 Input Validation and Error Recovery

**Q: What happens when `cin >> int` receives a string? How do you handle it?**

**A:** The stream enters a **fail state** (`failbit` is set). All subsequent `cin` operations will fail silently. You must:
1. Call `cin.clear()` to reset the error flags
2. Call `cin.ignore()` to discard the bad input from the buffer

**Implementation (lines 700-705):**
```cpp
if (!(cin >> choice)) {                    // Did the read fail?
    cin.clear();                           // Reset error flags
    cin.ignore(10000, '\n');               // Discard garbage input
    cout << "[!] Please enter a number (0-8), not text!" << endl;
    continue;                              // Re-show the menu
}
```

**Q: What would happen WITHOUT this validation?**

**A:** Typing a word like "hello" would cause `cin` to enter a fail state. The `do-while` loop would run infinitely, printing the menu endlessly without waiting for input — the program would freeze/spam.

---

### 6.3 Buffer Flushing (cin.ignore)

**Q: Why is `cin.ignore()` needed after `cin >>`?**

**A:** When you type `4` and press Enter, `cin >>` reads "4" but leaves the `\n` (newline) character in the input buffer. The next `getline()` call would immediately read that leftover `\n` and return an empty string, skipping the user's actual input.

```cpp
cin >> choice;       // Reads "4", leaves "\n" in buffer
cin.ignore();        // Discards the leftover "\n"  (line 706)
getline(cin, name);  // Now correctly waits for user input
```

---

## 7. Control Flow

### 7.1 switch-case Statements

**Q: What is a switch-case? When should you use it over if-else?**

**A:** A `switch` evaluates a single expression and branches to the matching `case` label. It's cleaner than chained if-else when comparing one variable against many constant values.

**Usage (lines 708-830):**
```cpp
switch (choice) {
    case 1: /* Add playlist */     break;
    case 2: /* Add song */         break;
    case 3: /* Display */          break;
    case 4: /* Play */             break;
    case 5: /* Delete song */      break;
    case 6: /* Delete playlist */  break;
    case 7: /* Save */             break;
    case 8: /* Load */             break;
    case 0: /* Exit */             break;
    default: /* Invalid input */
}
```

**Q: What happens if you forget `break` in a switch case?**

**A:** **Fall-through** — execution continues into the next case without checking the condition. This is usually a bug but occasionally used intentionally.

---

### 7.2 do-while Loop

**Q: What is a do-while loop? How is it different from a while loop?**

**A:** A `do-while` loop executes the body **at least once** before checking the condition. A `while` loop checks the condition first.

```cpp
do {
    printMainMenu();          // Always shows menu at least once
    cin >> choice;
    // ... handle choice ...
} while (choice != 0);        // Keep looping until user picks 0
```

**Why do-while here?** The menu should display at least once before checking if the user wants to exit.

---

### 7.3 Event Loop / Game Loop Pattern

**Q: What is an event loop? How is it implemented here?**

**A:** An event loop is a programming pattern that continuously:
1. Checks for events (keyboard input)
2. Processes events
3. Updates state
4. Sleeps briefly to prevent CPU overuse

**The playback loop (lines 498-542) implements this pattern:**
```cpp
while ((isPlaying || isPaused) && !exitRequested) {
    // 1. CHECK STATE: Query MCI for current song status
    mciSendStringA("status SONG mode", status, ...);

    // 2. AUTO-ADVANCE: If song finished, play next
    if (strcmp(status, "stopped") == 0 && !isPaused) {
        playNext();
    }

    // 3. HANDLE INPUT: Check for keyboard events
    if (_kbhit()) {
        int key = _getch();
        if (key == 75) playPrevious();      // Left Arrow
        else if (key == 77) playNext();      // Right Arrow
        else if (key == ' ') togglePause();  // Space
        else if (key == 27) exit;            // ESC
    }

    // 4. SLEEP: Prevent 100% CPU usage
    Sleep(100);  // 100ms = check 10 times per second
}
```

**Q: Why is `Sleep(100)` important in the loop?**

**A:** Without it, the while loop would run millions of times per second, consuming 100% CPU. `Sleep(100)` pauses for 100 milliseconds between iterations, reducing CPU usage to near 0% while still being responsive enough (10 checks/second).

---

## 8. Windows API and Multimedia

### 8.1 MCI (Media Control Interface)

**Q: What is MCI and how is it used to play audio?**

**A:** MCI (Media Control Interface) is a Windows API for controlling multimedia devices. Commands are sent as text strings via `mciSendStringA()`.

**Audio lifecycle in this program:**
```cpp
// 1. OPEN: Load the audio file
mciSendStringA("open \"file.mp3\" type mpegvideo alias SONG", ...);

// 2. PLAY: Start playback
mciSendStringA("play SONG", ...);

// 3. CONTROL: Pause, resume, set volume
mciSendStringA("pause SONG", ...);
mciSendStringA("resume SONG", ...);
mciSendStringA("setaudio SONG volume to 500", ...);

// 4. QUERY: Check playback status
mciSendStringA("status SONG mode", status, sizeof(status), ...);

// 5. STOP & CLOSE: Clean up
mciSendStringA("stop SONG", ...);
mciSendStringA("close SONG", ...);
```

**Q: What does the `alias` keyword do in MCI?**

**A:** An alias gives the opened media file a short reference name. Instead of referring to the full file path every time, you refer to it as `"SONG"`. This is crucial because the program always closes the old alias before opening a new one.

---

### 8.2 Keyboard Input (_kbhit, _getch)

**Q: What are `_kbhit()` and `_getch()`? Why are they used instead of `cin`?**

**A:**
- `_kbhit()` — Returns true if a key was pressed (non-blocking check)
- `_getch()` — Reads a single character without waiting for Enter, and without echoing it

**Why not `cin`?** `cin` blocks execution and waits for Enter. During music playback, you need **non-blocking** input — the program must keep running (checking playback status, auto-advancing songs) while simultaneously listening for keypresses.

**Arrow key handling (lines 513-521):**
```cpp
if (key == 0 || key == 224) {   // Arrow keys send TWO values
    key = _getch();              // Read the actual key code
    if (key == 75)  playPrevious();  // Left Arrow = 75
    if (key == 77)  playNext();      // Right Arrow = 77
}
```

**Q: Why do arrow keys require two `_getch()` calls?**

**A:** Arrow keys are **extended keys** that send a two-byte sequence. The first byte is `0` or `224` (a prefix), and the second byte is the actual key code (75=Left, 77=Right, 72=Up, 80=Down).

---

### 8.3 SetConsoleOutputCP and Encoding

**Q: What is `SetConsoleOutputCP(CP_UTF8)` and why is it needed?**

**A:** It sets the console's output code page to UTF-8 (code page 65001). Without it, non-ASCII characters (like Unicode box-drawing characters `╔═║╚` or checkmarks `✓`) display as garbled gibberish (e.g., `ΓòöΓòÉ...`) because the Windows console defaults to a legacy code page.

```cpp
SetConsoleOutputCP(CP_UTF8);  // Line 656 — first line of main()
```

---

### 8.4 #pragma comment(lib)

**Q: What does `#pragma comment(lib, "winmm.lib")` do?**

**A:** It tells the MSVC linker to automatically link the `winmm.lib` library (Windows Multimedia). Without it, functions like `mciSendStringA()` would cause "unresolved external symbol" linker errors.

```cpp
#pragma comment(lib, "winmm.lib")  // Line 9
```

**Note:** This is MSVC-specific. For GCC/MinGW, you must pass `-lwinmm` on the command line instead.

---

## 9. Algorithm Design Patterns

### 9.1 Linear Search

**Q: What search algorithm is used in this program? What is its time complexity?**

**A:** **Linear search** — traversing the list from head to tail, comparing each node. Time complexity: **O(n)**.

**Example — `playSong()` (lines 103-117):**
```cpp
void playSong(string title) {
    SongNode* temp = head;
    while (temp != nullptr) {
        if (temp->title == title) {   // Compare each node
            currentPlaying = temp;
            playAudioFile(...);
            return;                    // Found — exit early
        }
        temp = temp->next;             // Move to next node
    }
    cout << "Song not found!" << endl;  // Exhausted list without finding
}
```

**Q: Could you improve the search performance?**

**A:** For this application (small playlists of 5-50 songs), O(n) linear search is perfectly adequate. For large collections (10,000+ items), you could:
- Use a `std::unordered_map` (hash map) for O(1) average lookup
- Sort the list and use binary search for O(log n) lookup
- Use a balanced BST (like `std::map`) for O(log n) lookup with ordered traversal

---

### 9.2 Traversal

**Q: What is list traversal? Show the pattern.**

**A:** Traversal is visiting every node in the list exactly once. The standard pattern:

```cpp
SongNode* temp = head;           // Start at the beginning
while (temp != nullptr) {         // Continue until end
    // Process temp->data
    temp = temp->next;            // Move to next node
}
```

**Used in:** `displaySongs()` (line 236), `deleteSong()` (line 251), `saveToFile()` (line 294), every destructor.

---

### 9.3 State Machine (Playback States)

**Q: What is a state machine? How does the playback system work as one?**

**A:** A state machine is a model where the system is always in one of several defined states, and transitions between states based on events.

**Playback states and transitions:**
```
            play()           pause()
  STOPPED ---------> PLAYING ---------> PAUSED
     ^                  |                  |
     |     stop()       |     resume()     |
     +------------------+                  |
     |                                     |
     +-------------------------------------+
                    stop()
```

**State tracking (lines 43-45):**
```cpp
bool isPlaying;   // true when audio is actively playing
bool isPaused;    // true when audio is paused
// Both false = stopped
```

**State transitions in code:**
```cpp
// STOPPED → PLAYING (playAudioFile, line 94-95)
isPlaying = true; isPaused = false;

// PLAYING → PAUSED (togglePause, line 186-187)
isPaused = true; isPlaying = false;

// PAUSED → PLAYING (togglePause, line 180-181)
isPaused = false; isPlaying = true;

// ANY → STOPPED (stopPlayback, line 222-223)
isPlaying = false; isPaused = false;
```

---

## 10. Miscellaneous C++ Concepts

### 10.1 Preprocessor Directives

**Q: What are `#include` and `#pragma`? What happens before compilation?**

**A:** The preprocessor runs BEFORE the compiler. `#include` copies the entire contents of a header file into your source file. `#pragma` gives compiler-specific instructions.

| Directive | Purpose |
|-----------|---------|
| `#include <iostream>` | Standard I/O (cout, cin) |
| `#include <string>` | std::string class |
| `#include <fstream>` | File I/O (ofstream, ifstream) |
| `#include <algorithm>` | std::transform |
| `#include <windows.h>` | Windows API (Sleep, SetConsoleOutputCP) |
| `#include <mmsystem.h>` | Multimedia API (mciSendStringA) |
| `#include <conio.h>` | Console I/O (_kbhit, _getch) |
| `#include <cstring>` | C-string functions (strcmp) |

---

### 10.2 using namespace std

**Q: What does `using namespace std;` do? Is it good practice?**

**A:** It allows you to use standard library names like `cout`, `string`, `endl` without the `std::` prefix. 

**Without it:** `std::cout << std::string("hello") << std::endl;`
**With it:** `cout << string("hello") << endl;`

**In production code**, it's considered bad practice because it can cause **name collisions** (two libraries might define the same name). In academic projects and competitive programming, it's widely used for convenience.

---

### 10.3 Ternary Operator

**Q: What is the ternary operator?**

**A:** A shorthand for `if-else` that returns a value: `condition ? value_if_true : value_if_false`

```cpp
// Line 195
cout << "[LOOP] " << (isLooped ? "ON" : "OFF") << endl;
// Equivalent to:
if (isLooped) cout << "[LOOP] ON"; else cout << "[LOOP] OFF";

// Line 566
string playlistName = (pos != string::npos) ? filename.substr(0, pos) : filename;
```

---

### 10.4 Type Casting with to_string

**Q: How do you convert numeric types to strings in C++?**

**A:** Use `to_string()`:
```cpp
string volCmd = "setaudio SONG volume to " + to_string(volume * 10);  // Line 91
// If volume = 80, result: "setaudio SONG volume to 800"
```

---

## Quick-Fire Interview Questions (Rapid Round)

| # | Question | Short Answer |
|---|----------|-------------|
| 1 | What data structure is used for playlists? | Doubly Linked List |
| 2 | What is the time complexity of `addSong()`? | O(1) — direct tail insertion |
| 3 | What is the time complexity of `deleteSong()`? | O(n) — search + O(1) delete |
| 4 | What is the time complexity of `playNext()`? | O(1) — follow `next` pointer |
| 5 | How many classes are there? | 4 (SongNode, SmallPlaylist, PlaylistNode, MasterPlaylist) |
| 6 | What design pattern does the playback loop use? | Event loop / Game loop |
| 7 | What prevents memory leaks? | Destructors with node-by-node deletion |
| 8 | Why is `cin.ignore()` needed? | To flush leftover `\n` from the input buffer |
| 9 | What API plays audio? | Windows MCI via `mciSendStringA()` |
| 10 | Why doubly linked and not singly? | Need bidirectional navigation (prev/next) |
| 11 | What is `nullptr`? | C++11 null pointer constant |
| 12 | What is the `->` operator? | Pointer dereference + member access |
| 13 | What file format is used for saving? | Pipe-delimited text (`.txt`) |
| 14 | What happens if `cin >> int` gets a string? | Stream enters fail state |
| 15 | Why is `Sleep(100)` in the event loop? | Prevents 100% CPU usage |

---

## Summary: Complete Class Hierarchy Diagram

```
+------------------+         +------------------+
|    SongNode      |         |   PlaylistNode   |
+------------------+         +------------------+
| - title          |    +--->| - name           |
| - filePath       |    |    | - songs ---------|----> SmallPlaylist
| - playCount      |    |    | - next           |         |
| - next --------->|    |    | - prev           |         |
| - prev           |    |    +------------------+         |
+------------------+    |                                  |
       ^                |    +------------------+          |
       |                |    |  MasterPlaylist  |          |
       +---- nodes of --+----|  - head          |          |
                             |  - tail          |          |
                         +-->|  - currentPL     |          |
                             |  - playlistCount |          |
                             +------------------+          |
                                                           |
                             +------------------+          |
                             |  SmallPlaylist   |<---------+
                             +------------------+
                             | - head (SongNode)|
                             | - tail           |
                             | - currentPlaying |
                             | - songCount      |
                             | - isPlaying      |
                             | - isPaused       |
                             | - isLooped       |
                             | - volume         |
                             +------------------+
                             | + addSong()      |
                             | + deleteSong()   |
                             | + playAudioFile()|
                             | + playNext()     |
                             | + playPrevious() |
                             | + togglePause()  |
                             | + toggleLoop()   |
                             | + increaseVol()  |
                             | + decreaseVol()  |
                             | + stopPlayback() |
                             | + displaySongs() |
                             | + saveToFile()   |
                             | + loadFromFile() |
                             +------------------+
```

---

*Generated for the AudioSignalProcessing project — covering all 835 lines of Audio.cpp.*
