#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <cstring>
#pragma comment(lib, "winmm.lib")
using namespace std;

// Song Node in Small Playlist
class SongNode {
public:
    string title;
    string filePath;
    int playCount;
    SongNode* next;
    SongNode* prev;

    SongNode(string t, string path) {
        title = t;
        filePath = path;
        playCount = 0;
        next = nullptr;
        prev = nullptr;
    }
};

// Small Playlist (Doubly Linked List)
class SmallPlaylist {
public:
    SongNode* head;
    SongNode* tail;
    SongNode* currentPlaying;
    int songCount;
    bool isPlaying;
    bool isPaused;
    bool isLooped;
    int volume;

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

    // Add song at the end
    void addSong(string title, string path) {
        SongNode* newSong = new SongNode(title, path);

        if (head == nullptr) {
            head = tail = newSong;
        }
        else {
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        }
        songCount++;
        cout << "[+] Added song: " << title << endl;
    }

    // Play audio file using MCI
    bool playAudioFile(string filePath, string title) {
        stopPlayback();

        string openCmd = "open \"" + filePath + "\" type mpegvideo alias SONG";
        if (mciSendStringA(openCmd.c_str(), NULL, 0, NULL) != 0) {
            cout << "[!] Error opening file!" << endl;
            return false;
        }

        if (mciSendStringA("play SONG", NULL, 0, NULL) != 0) {
            cout << "[!] Error playing file!" << endl;
            return false;
        }

        // Set volume
        string volCmd = "setaudio SONG volume to " + to_string(volume * 10);
        mciSendStringA(volCmd.c_str(), NULL, 0, NULL);

        isPlaying = true;
        isPaused = false;

        cout << "\n[PLAYING] " << title << endl;
        cout << "[FILE] " << filePath << "\n" << endl;
        return true;
    }

    // Play specific song
    void playSong(string title) {
        SongNode* temp = head;

        while (temp != nullptr) {
            if (temp->title == title) {
                currentPlaying = temp;
                playAudioFile(temp->filePath, temp->title);
                temp->playCount++;
                return;
            }
            temp = temp->next;
        }

        cout << "[!] Song not found!" << endl;
    }

    // Play first song
    void playFirst() {
        if (head == nullptr) {
            cout << "[!] Playlist is empty!" << endl;
            return;
        }
        currentPlaying = head;
        playAudioFile(head->filePath, head->title);
        head->playCount++;
    }

    // Play next song
    void playNext() {
        if (currentPlaying == nullptr) {
            cout << "[!] No song is currently playing!" << endl;
            playFirst();
            return;
        }

        if (currentPlaying->next == nullptr) {
            if (isLooped) {
                cout << "[~] Looping back to start..." << endl;
                playFirst();
            }
            else {
                cout << "[!] This is the last song!" << endl;
            }
            return;
        }

        currentPlaying = currentPlaying->next;
        playAudioFile(currentPlaying->filePath, currentPlaying->title);
        currentPlaying->playCount++;
    }

    // Play previous song
    void playPrevious() {
        if (currentPlaying == nullptr) {
            cout << "[!] No song is currently playing!" << endl;
            return;
        }

        if (currentPlaying->prev == nullptr) {
            cout << "[!] This is the first song!" << endl;
            return;
        }

        currentPlaying = currentPlaying->prev;
        playAudioFile(currentPlaying->filePath, currentPlaying->title);
        currentPlaying->playCount++;
    }

    // Pause/Resume playback
    void togglePause() {
        if (!isPlaying && !isPaused) {
            cout << "[!] No song is playing!" << endl;
            return;
        }

        if (isPaused) {
            mciSendStringA("resume SONG", NULL, 0, NULL);
            isPaused = false;
            isPlaying = true;
            cout << "\n[RESUMED] Playback resumed" << endl;
        }
        else {
            mciSendStringA("pause SONG", NULL, 0, NULL);
            isPaused = true;
            isPlaying = false;
            cout << "\n[PAUSED] Playback paused" << endl;
        }
    }

    // Toggle Loop
    void toggleLoop() {
        isLooped = !isLooped;
        cout << "\n[LOOP] " << (isLooped ? "ON" : "OFF") << endl;
    }

    // Volume Control
    void increaseVolume() {
        if (volume < 100) {
            volume += 10;
            string cmd = "setaudio SONG volume to " + to_string(volume * 10);
            mciSendStringA(cmd.c_str(), NULL, 0, NULL);
            cout << "\n[VOLUME] " << volume << "%" << endl;
        }
    }

    void decreaseVolume() {
        if (volume > 0) {
            volume -= 10;
            string cmd = "setaudio SONG volume to " + to_string(volume * 10);
            mciSendStringA(cmd.c_str(), NULL, 0, NULL);
            cout << "\n[VOLUME] " << volume << "%" << endl;
        }
    }

    // Stop playback
    void stopPlayback() {
        if (isPlaying || isPaused) {
            mciSendStringA("stop SONG", NULL, 0, NULL);
            mciSendStringA("close SONG", NULL, 0, NULL);
            isPlaying = false;
            isPaused = false;
        }
    }

    // Display all songs
    void displaySongs() {
        if (head == nullptr) {
            cout << "  [Empty]" << endl;
            return;
        }

        SongNode* temp = head;
        int num = 1;
        while (temp != nullptr) {
            cout << "  " << num << ". " << temp->title;
            if (temp == currentPlaying) {
                cout << " [NOW PLAYING]";
            }
            cout << " (Played: " << temp->playCount << " times)" << endl;
            temp = temp->next;
            num++;
        }
    }

    // Delete song
    bool deleteSong(string title) {
        SongNode* temp = head;

        while (temp != nullptr) {
            if (temp->title == title) {
                if (temp == currentPlaying) {
                    stopPlayback();
                    currentPlaying = nullptr;
                }

                if (temp == head) {
                    head = temp->next;
                    if (head != nullptr) head->prev = nullptr;
                }
                else if (temp == tail) {
                    tail = temp->prev;
                    if (tail != nullptr) tail->next = nullptr;
                }
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }

                delete temp;
                songCount--;
                cout << "[-] Deleted song: " << title << endl;
                return true;
            }
            temp = temp->next;
        }

        cout << "[!] Song not found!" << endl;
        return false;
    }

    // Save playlist to file
    bool saveToFile(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        file << songCount << endl;
        file << isLooped << endl;

        SongNode* temp = head;
        while (temp) {
            file << temp->title << "|" << temp->filePath << "|" << temp->playCount << endl;
            temp = temp->next;
        }

        file.close();
        return true;
    }

    // Load playlist from file
    bool loadFromFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        int count;
        file >> count;
        file >> isLooped;
        file.ignore();

        for (int i = 0; i < count; i++) {
            string line;
            getline(file, line);

            size_t pos1 = line.find('|');
            size_t pos2 = line.rfind('|');

            if (pos1 != string::npos && pos2 != string::npos) {
                string title = line.substr(0, pos1);
                string path = line.substr(pos1 + 1, pos2 - pos1 - 1);
                int plays = stoi(line.substr(pos2 + 1));

                SongNode* newSong = new SongNode(title, path);
                newSong->playCount = plays;

                if (!head) {
                    head = tail = newSong;
                }
                else {
                    tail->next = newSong;
                    newSong->prev = tail;
                    tail = newSong;
                }
                songCount++;
            }
        }

        file.close();
        return true;
    }

    ~SmallPlaylist() {
        stopPlayback();
        SongNode* current = head;
        while (current != nullptr) {
            SongNode* next = current->next;
            delete current;
            current = next;
        }
    }
};

// Playlist Node in Master Playlist
class PlaylistNode {
public:
    string name;
    SmallPlaylist* songs;
    PlaylistNode* next;
    PlaylistNode* prev;

    PlaylistNode(string n) {
        name = n;
        songs = new SmallPlaylist();
        next = nullptr;
        prev = nullptr;
    }


    ~PlaylistNode() {
        delete songs;
    }
};

// Master Playlist (Big Doubly Linked List)
class MasterPlaylist {
public:
    PlaylistNode* head;
    PlaylistNode* tail;
    PlaylistNode* currentPlaylist;
    int playlistCount;

    MasterPlaylist() {
        head = nullptr;
        tail = nullptr;
        currentPlaylist = nullptr;
        playlistCount = 0;
    }

    // Add new playlist
    void addPlaylist(string name) {
        PlaylistNode* newPlaylist = new PlaylistNode(name);

        if (head == nullptr) {
            head = tail = newPlaylist;
        }
        else {
            tail->next = newPlaylist;
            newPlaylist->prev = tail;
            tail = newPlaylist;
        }
        playlistCount++;
        cout << "\n[+] Created Playlist: " << name << endl;
    }

    // Find playlist
    PlaylistNode* findPlaylist(string name) {
        PlaylistNode* temp = head;
        while (temp != nullptr) {
            if (temp->name == name) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // Select playlist for playback
    void selectPlaylist(string name) {
        PlaylistNode* playlist = findPlaylist(name);
        if (playlist != nullptr) {
            currentPlaylist = playlist;
            cout << "\n[+] Selected Playlist: " << name << endl;
        }
        else {
            cout << "[!] Playlist not found!" << endl;
        }
    }

    // Add song to specific playlist
    void addSongToPlaylist(string playlistName, string songTitle, string path) {
        PlaylistNode* playlist = findPlaylist(playlistName);
        if (playlist != nullptr) {
            playlist->songs->addSong(songTitle, path);
        }
        else {
            cout << "[!] Playlist not found!" << endl;
        }
    }

    // Display all playlists
    void displayAllPlaylists() {
        if (head == nullptr) {
            cout << "\n[!] No playlists!" << endl;
            return;
        }

        cout << "\n=============== ALL PLAYLISTS ===============" << endl;
        PlaylistNode* temp = head;
        int num = 1;
        while (temp != nullptr) {
            if (temp == currentPlaylist) {
                cout << "\n> " << num << ". [" << temp->name << "]";
            }
            else {
                cout << "\n  " << num << ". [" << temp->name << "]";
            }
            cout << " (" << temp->songs->songCount << " songs)";
            if (temp->songs->isLooped) cout << " [LOOP ON]";
            cout << endl;
            temp->songs->displaySongs();
            temp = temp->next;
            num++;
        }
        cout << "============================================\n" << endl;
    }

    // Start playback loop with keyboard controls
    void startPlaybackLoop() {
        if (currentPlaylist == nullptr) {
            cout << "[!] Select a playlist first!" << endl;
            return;
        }

        if (currentPlaylist->songs->songCount == 0) {
            cout << "[!] Playlist is empty!" << endl;
            return;
        }

        currentPlaylist->songs->playFirst();

        cout << "\n========== PLAYBACK CONTROLS ==========" << endl;
        cout << "  <- (Left)   : Previous Song" << endl;
        cout << "  -> (Right)  : Next Song" << endl;
        cout << "  SPACE       : Pause/Resume" << endl;
        cout << "  L           : Toggle Loop" << endl;
        cout << "  +           : Volume Up" << endl;
        cout << "  -           : Volume Down" << endl;
        cout << "  ESC         : Stop & Exit" << endl;
        cout << "========================================\n" << endl;

        bool exitRequested = false;

        while ((currentPlaylist->songs->isPlaying || currentPlaylist->songs->isPaused) && !exitRequested) {
            // Check if current song has finished
            char status[128];
            mciSendStringA("status SONG mode", status, sizeof(status), NULL);

            if (strcmp(status, "stopped") == 0 && !currentPlaylist->songs->isPaused) {
                if (currentPlaylist->songs->currentPlaying != nullptr) {
                    cout << "\n[AUTO] Playing next song..." << endl;
                    currentPlaylist->songs->playNext();
                }
            }

            if (_kbhit()) {
                int key = _getch();

                if (key == 0 || key == 224) {
                    key = _getch();

                    if (key == 75) {  // Left Arrow
                        currentPlaylist->songs->playPrevious();
                    }
                    else if (key == 77) {  // Right Arrow
                        currentPlaylist->songs->playNext();
                    }
                }
                else if (key == ' ') {  // Space
                    currentPlaylist->songs->togglePause();
                }
                else if (key == 'l' || key == 'L') {  // Loop
                    currentPlaylist->songs->toggleLoop();
                }
                else if (key == '+' || key == '=') {  // Volume Up
                    currentPlaylist->songs->increaseVolume();
                }
                else if (key == '-' || key == '_') {  // Volume Down
                    currentPlaylist->songs->decreaseVolume();
                }
                else if (key == 27) {  // ESC
                    currentPlaylist->songs->stopPlayback();
                    cout << "\n[+] Exiting playback mode..." << endl;
                    exitRequested = true;
                }
            }
            Sleep(100);
        }
    }

    // Save playlist to file
    void savePlaylist(string playlistName) {
        PlaylistNode* playlist = findPlaylist(playlistName);
        if (!playlist) {
            cout << "[!] Playlist not found!" << endl;
            return;
        }

        string filename = playlistName + ".txt";
        if (playlist->songs->saveToFile(filename)) {
            cout << "\n[+] Playlist saved to: " << filename << endl;
        }
        else {
            cout << "\n[!] Error saving playlist!" << endl;
        }
    }

    // Load playlist from file
    void loadPlaylist(string filename) {
        // Extract playlist name from filename
        size_t pos = filename.find(".txt");
        string playlistName = (pos != string::npos) ? filename.substr(0, pos) : filename;

        // Check if filename ends with .txt
        if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".txt") {
            filename += ".txt";
        }

        PlaylistNode* newPlaylist = new PlaylistNode(playlistName);

        if (newPlaylist->songs->loadFromFile(filename)) {
            if (head == nullptr) {
                head = tail = newPlaylist;
            }
            else {
                tail->next = newPlaylist;
                newPlaylist->prev = tail;
                tail = newPlaylist;
            }
            playlistCount++;
            cout << "\n[+] Playlist loaded: " << playlistName << endl;
        }
        else {
            delete newPlaylist;
            cout << "\n[!] Error loading playlist!" << endl;
        }
    }

    // Delete playlist
    bool deletePlaylist(string name) {
        PlaylistNode* temp = head;

        while (temp != nullptr) {
            if (temp->name == name) {
                if (temp == currentPlaylist) {
                    currentPlaylist = nullptr;
                }

                if (temp == head) {
                    head = temp->next;
                    if (head != nullptr) head->prev = nullptr;
                }
                else if (temp == tail) {
                    tail = temp->prev;
                    if (tail != nullptr) tail->next = nullptr;
                }
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }

                delete temp;
                playlistCount--;
                cout << "[-] Deleted Playlist: " << name << endl;
                return true;
            }
            temp = temp->next;
        }

        cout << "[!] Playlist not found!" << endl;
        return false;
    }

    ~MasterPlaylist() {
        PlaylistNode* current = head;
        while (current != nullptr) {
            PlaylistNode* next = current->next;
            delete current;
            current = next;
        }
    }
};

void printMainMenu() {
    cout << "\n============================================" << endl;
    cout << "       QURAN PLAYLIST MANAGER" << endl;
    cout << "============================================" << endl;
    cout << " 1. Add New Playlist" << endl;
    cout << " 2. Add Song to Playlist" << endl;
    cout << " 3. Display All Playlists" << endl;
    cout << " 4. Select & Play Playlist" << endl;
    cout << " 5. Delete Song from Playlist" << endl;
    cout << " 6. Delete Playlist" << endl;
    cout << " 7. Save Playlist to File" << endl;
    cout << " 8. Load Playlist from File" << endl;
    cout << " 0. Exit" << endl;
    cout << "============================================" << endl;
    cout << "Your choice: ";
}

int main() {
    MasterPlaylist myMusic;
    int choice;
    string playlistName, songTitle, path, filename;

    cout << "\n╔════════════════════════════════════════════╗" << endl;
    cout << "║   QURAN PLAYLIST MANAGER - ENHANCED        ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    cout << "\nNew Features Added:" << endl;
    cout << "  ✓ Loop Mode" << endl;
    cout << "  ✓ Volume Control" << endl;
    cout << "  ✓ Play Count Statistics" << endl;
    cout << "  ✓ Save/Load Playlists" << endl;
    cout << "\nSupported formats: MP3, WAV, WMA\n" << endl;

    // Creating default playlists
    myMusic.addPlaylist("Al_Munshawy");
    myMusic.addSongToPlaylist("Al_Munshawy", "Al-Fatihah",
        "C:\\Users\\Lenovo\\Music\\001.mp3");
        myMusic.addSongToPlaylist("Al_Munshawy", "Al-Baqarah",
            "C:\\Users\\Lenovo\\Music\\002.mp3");
        myMusic.addSongToPlaylist("Al_Munshawy", "Aal-E-Imran",
            "C:\Users\Lenovo\Music\003.mp3");
     myMusic.addPlaylist("Maher Al_muaiqly");
    myMusic.addSongToPlaylist("Maher Al_muaiqly", "Al-Kahf",
        "C:\\Users\\mohamed\\Downloads\\maherKahf.mp3");
    myMusic.addSongToPlaylist("Maher Al_muaiqly", "Yasin",
        "C:\\Users\\mohamed\\Downloads\\maheryasen.mp3");

    myMusic.addPlaylist("Yaser Al_dosary");
    myMusic.addSongToPlaylist("Yaser Al_dosary", "Al-Fajr",
        "C:\\Users\\Ali Hassan\\Desktop\\menshawy_Quran\\003.mp3");
    myMusic.addSongToPlaylist("Yaser Al_dosary", "Qaf",
        "C:\\Users\\mohamed\\Downloads\\yaserKaf.mp3");

    cout << "[INFO] Default playlists loaded successfully!\n" << endl;

    do {
        printMainMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Playlist name: ";
            getline(cin, playlistName);
            myMusic.addPlaylist(playlistName);
            break;

        case 2:
            cout << "Playlist name: ";
            getline(cin, playlistName);
            cout << "Song title: ";
            getline(cin, songTitle);
            cout << "Full path to audio file: ";
            getline(cin, path);
            myMusic.addSongToPlaylist(playlistName, songTitle, path);
            break;

        case 3:
            myMusic.displayAllPlaylists();
            cout << "Press Enter to continue...";
            cin.get();
            break;

        case 4: {
            cout << "Playlist name: ";
            getline(cin, playlistName);
            myMusic.selectPlaylist(playlistName);

            if (myMusic.currentPlaylist != nullptr) {
                int playlistChoice;
                do {
                    cout << "\n============================================" << endl;
                    cout << "    PLAYLIST: [" << myMusic.currentPlaylist->name << "]" << endl;
                    cout << "============================================" << endl;
                    cout << " 1. Play from Beginning" << endl;
                    cout << " 2. Search & Play Specific Song" << endl;
                    cout << " 3. Add Song to This Playlist" << endl;
                    cout << " 4. Toggle Loop Mode" << endl;
                    cout << " 0. Back to Main Menu" << endl;
                    cout << "============================================" << endl;
                    cout << "Your choice: ";
                    cin >> playlistChoice;
                    cin.ignore();

                    switch (playlistChoice) {
                    case 1:
                        myMusic.startPlaybackLoop();
                        break;

                    case 2:
                        cout << "Song title: ";
                        getline(cin, songTitle);
                        myMusic.currentPlaylist->songs->playSong(songTitle);
                        myMusic.startPlaybackLoop();
                        break;

                    case 3:
                        cout << "Song title: ";
                        getline(cin, songTitle);
                        cout << "Full path to audio file: ";
                        getline(cin, path);
                        myMusic.currentPlaylist->songs->addSong(songTitle, path);
                        break;

                    case 4:
                        myMusic.currentPlaylist->songs->toggleLoop();
                        break;

                    case 0:
                        cout << "\n[+] Back to main menu..." << endl;
                        break;

                    default:
                        cout << "[!] Invalid choice!" << endl;
                    }

                } while (playlistChoice != 0);
            }
            break;
        }

        case 5:
            cout << "Playlist name: ";
            getline(cin, playlistName);
            myMusic.selectPlaylist(playlistName);
            if (myMusic.currentPlaylist) {
                cout << "Song title to delete: ";
                getline(cin, songTitle);
                myMusic.currentPlaylist->songs->deleteSong(songTitle);
            }
            break;

        case 6:
            cout << "Playlist name: ";
            getline(cin, playlistName);
            myMusic.deletePlaylist(playlistName);
            break;

        case 7:
            cout << "Playlist name to save: ";
            getline(cin, playlistName);
            myMusic.savePlaylist(playlistName);
            break;

        case 8:
            cout << "Filename to load (without .txt): ";
            getline(cin, filename);
            myMusic.loadPlaylist(filename);
            break;

        case 0:
            if (myMusic.currentPlaylist != nullptr) {
                myMusic.currentPlaylist->songs->stopPlayback();
            }
            cout << "\n╔════════════════════════════════════════════╗" << endl;
            cout << "║  Thanks for using Quran Playlist Manager!  ║" << endl;
            cout << "║  May Allah bless you. Goodbye!              ║" << endl;
            cout << "╚════════════════════════════════════════════╝\n" << endl;
            break;

        default:
            cout << "[!] Invalid choice!" << endl;
        }

    } while (choice != 0);

    return 0;
}