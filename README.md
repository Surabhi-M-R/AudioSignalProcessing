# Quran Playlist Manager (Data Structures Project) 🎧

A high-performance C++ implementation focused on managing audio data streams through advanced data structures. Originally conceptualized as a general Audio Signal Processor.

## 🚀 Academic Context
This project serves as a practical application of **Data Structure** concepts, specifically designed to handle real-time audio playback, memory-efficient data management, and nested structures.

## 🛠️ Technical Specifications
* **IDE:** Visual Studio 2026 (Next-Gen Environment).
* **Solution Format:** Modern `.slnx` format for faster loading and cleaner project hierarchy.
* **Language:** C++23 / C++26 Ready.
* **Architecture Support:** Native configurations for `x64` and `x86`.
* **External Libraries:** Windows Multimedia API (`winmm.lib`).

---

## 📖 What This Program is Doing
The program is a console-based **Playlist Manager** built entirely in C++. It allows users to manage multiple playlists, where each playlist contains multiple audio files (songs or recitations). 

Under the hood, it uses an advanced **Lists of Lists** architecture to store data:
1. **Master Playlist (Outer list)**: A doubly linked list representing a collection of playlists. Each node (`PlaylistNode`) points to a specific `SmallPlaylist`.
2. **Small Playlist (Inner list)**: A doubly linked list representing individual audio files. Each node (`SongNode`) holds the title, file path, play count, and pointers to the previous and next audio tracks.

**Core functionalities include:**
- **Playback Control:** Playing, pausing, stopping, looping, and controlling the volume of audio files directly from the console using MCI (Media Control Interface).
- **Navigation:** Next and Previous track tracing using the links in the doubly linked list.
- **File Management:** Saving playlists (track lists) to a text file on disk, and loading them dynamically.
- **Interactive Console:** Keyboard-based interactive playback menu (using `_kbhit()` and `_getch()`) that runs concurrently to the playback process.

## 💻 How to Run the Program

### Visual Studio (Recommended)
1. Double-click the `Audio.slnx` file to open the solution in Visual Studio.
2. Press **Ctrl + F5** (Start without Debugging) or simply **F5** to build and run the program natively. The project is already configured to link against `winmm.lib` due to the `#pragma` declaration inside the source code.

### Command Line / Terminal (GCC or MSVC)
If you are compiling directly from a Command Prompt or PowerShell, you must link the Windows Multimedia Library `winmm.lib` (MSVC) or `-lwinmm` (GCC), as it's required for audio playback.

**Using MSVC (Visual Studio Developer Command Prompt):**
```bash
cd Audio
cl Audio.cpp /EHsc /link winmm.lib
.\Audio.exe
```

**Using GCC/MinGW:**
```bash
cd Audio
g++ Audio.cpp -o Audio.exe -lwinmm
.\Audio.exe
```

## 🔍 Line-by-Line Execution Walkthrough
Here is how the program operates intrinsically when executed:

1. **Imports & Setup (Lines 1-10):** The program loads `<iostream>`, `<windows.h>`, `<mmsystem.h>` (for audio MCI playback) and uses `#pragma comment(lib, "winmm.lib")` to tell the MSVC linker to include the Windows multimedia API functions.
2. **Objects Instantiation (Lines 647-649):** Inside `main()`, `MasterPlaylist myMusic;` is instantiated. This creates a fresh outer doubly linked list for handling all playlists, initializing the `head` and `tail` pointers to `nullptr`.
3. **Seeding Default Data (Lines 662-680):** Default playlists (e.g., `Al_Munshawy`, `Maher Al_muaiqly`) are programmatically appended using `addPlaylist()` and `addSongToPlaylist()`. This constructs `PlaylistNode` and `SongNode` objects in the heap memory dynamically.
   * *Note: The hardcoded absolute paths (like `C:\Users\Lenovo\Music\001.mp3`) must be modified or removed based on your system, otherwise they will fail to play because those paths don't exist on your PC.*
4. **Main Loop Execution (Lines 684-813):** A `do-while` loop starts. The user is prompted with the Main Menu. Based on inputs captured by `cin >> choice`, the `switch` statement resolves the desired operation.
5. **Inner Playback Loop Integration (Lines 465-535):** When playing an audio file, it launches `startPlaybackLoop()`. This initiates playback using `mciSendStringA("open ... alias SONG")`. To provide an interactive player rather than freezing, a `while` loop periodically sleeps (`Sleep(100)`) and checks keyboard input via `_kbhit()` enabling real-time adjustments (Space to pause, Right Arrow to skip forward, +/- for volume).
6. **Graceful Exit (Lines 799-807):** Upon hitting `0` on the main menu, current playback stops. The loop concludes, the `MasterPlaylist` and `SmallPlaylist` destructors automatically fire freeing up all memory allocations for the linked lists, and `return 0` concludes the application cleanly.

## 🛠️ How to Tackle Errors & Troubleshooting

### 1. "Error opening file!" or "Error playing file!" 
* **Cause**: This happens when Windows MCI fails to find the MP3/WAV path, or the provided path doesn't point to a supported music file. This will immediately happen with the initial mock items when you test them, due to paths like `C:\Users\Lenovo\Music\...`.
* **Solution**: Ensure your provided file path is absolute, perfectly capitalized, and visually existent on your current PC. Do not use quotes inside the input when adding a file. Either remove lines 662-680 in `Audio.cpp` to start with a blank database, or change them to a valid `.mp3` document you own.

### 2. Audio plays but doesn't transition to the next song automatically
* **Cause**: The `strcmp(status, "stopped") == 0` check in `startPlaybackLoop` relies on the internal MCI status query correctly returning a string. A glitched audio file handle could fail to register as stopped.
* **Solution**: You can manually skip using the `<-` and `->` keys. Make sure your MP3 files are standard and not corrupted.

### 3. Build Error / `mciSendStringA` Unresolved External Symbol
* **Cause**: The compiler is missing the instructions linking `winmm.lib`.
* **Solution**: Make sure you include the link flag when compiling (`-lwinmm` for MinGW, `winmm.lib` for MSVC). If you are using Visual Studio, it should be natively resolved by `#pragma comment`, however setting up linking inside Linker Properties >> Input >> Additional Dependencies solves misconfigurations.

### 4. Memory Leaks or Access Violations on Closure
* **Cause**: Safely deleting a deeply nested data structure can throw pointer exceptions if you interrupt memory clearance.
* **Solution**: Rely on the existing `~MasterPlaylist` Destructor loop, which safely walks nodes. Ensure that you never manually clear a playlist without correctly unlinking its `prev` and `next` connections first.

### 5. `getline` skipping inputs (Input Buffer Glitch)
* **Cause**: Lingering `\n` characters in the C++ `cin` input stream skip subsequent text fetching.
* **Solution**: The code actively tackles this with `cin.ignore()` placed carefully after standard integer `cin >>` commands (like in line 687). If you add custom menus and string parsing starts overlapping, simply insert `cin.ignore()` to flush the buffer before requesting the string.
