#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <chrono>
#include <random>
#include <sstream>

using namespace std;

enum Commands_e {
    STOP = 0,
    PLAY,
    PAUSE,
};

class Track {
    string name;
    tm creationDate;
    int duration;

public:

    void setName(const string &val) {
        name = val;
    }

    string getName() {
        return name;
    }

    void setCreationDate(const tm &val) {
        creationDate = val;
    }

    tm getCreationDate() {
        return creationDate;
    }

    void setDuration(const int &val) {
        duration = val;
    }

    int getDuration() const {
        return duration;
    }
};

static void FillPlaylist(vector<Track *> &playList) {
#define NUM_OF_TRACKS 7
    static const string names[NUM_OF_TRACKS] = {
            "Track1",
            "Track2",
            "Track3",
            "Track4",
            "Track5",
            "Track6",
            "Track7",
    };
    static const string dates[NUM_OF_TRACKS] = {
            "05/21/2000",
            "06/21/2000",
            "06/21/2000",
            "05/22/2012",
            "07/21/2021",
            "08/22/2014",
            "08/22/2014",
    };
    static const int duration[NUM_OF_TRACKS] = {
            120,
            130,
            140,
            150,
            160,
            170,
            180
    };

    for (int i = 0; i < NUM_OF_TRACKS; ++i) {
        auto *track = new Track();
        track->setName(names[i]);
        stringstream s;
        s << dates[i];

        tm date;
        memset(&date, 0, sizeof(date));
        s >> get_time(&date, "%m/%d/%Y");
        track->setCreationDate(date);
        track->setDuration(duration[i]);
        playList.push_back(track);
    }
}

class AudioPlayer {

    vector<Track *> tracks;
    mt19937 mt_rand;
    Commands_e currentCmd = STOP;
    int currentTrack = 0;

    int generateRandom(uint32_t min, uint32_t max) {
        return (float) min + ((float) mt_rand() / (float) mt19937::max()) * ((float) max + 0.999f);
    }

    void startTrackPlaying(Track *track) {
        cout << "Playing is started..." << endl;
        cout << "   Track name: " << track->getName() << endl;
        tm date = track->getCreationDate();
        cout << "   Creation date: " << put_time(&date, "%m/%d/%Y") << endl;
        cout << "   Duration: " << track->getDuration() << " s" << endl;
    }

public:
    void setupPlaylist(const vector<Track *> &playList) {
        mt_rand = mt19937(chrono::high_resolution_clock::now().time_since_epoch().count());
        tracks = playList;
    };

    void play() {
        if (tracks.empty()) {
            cout << "Playlist is empty" << endl;
            return;
        }

        if (currentCmd != STOP) {
            cout << "Track: " << tracks[currentTrack]->getName() << " is already started" << endl;
            return;
        }

        string name;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter track name: " << endl;
        getline(cin, name);

        int i = 0;
        for (const auto &track : tracks) {
            if (name == track->getName()) {
                currentCmd = PLAY;
                currentTrack = i;
                startTrackPlaying(track);
                return;
            }
            ++i;
        }
        cout << "This track has not found" << endl;
    }

    void pause() {
        if (currentCmd == STOP) {
            cout << "No track is played" << endl;
            return;
        } else if (currentCmd != PLAY) {
            return;
        }

        currentCmd = PAUSE;
        cout << "Playing is paused..." << endl;
    }

    void next() {
        if (tracks.empty()) {
            cout << "Playlist is empty" << endl;
            return;
        }

        bool trackSelected = false;

        while (!trackSelected) {
            int trackNum = generateRandom(0, tracks.size() - 1);
            if (currentCmd != PLAY || trackNum != currentTrack) {
                currentTrack = trackNum;
                startTrackPlaying(tracks[trackNum]);
                trackSelected = true;
            }
        }
        currentCmd = PLAY;
    }

    void stop() {
        if (currentCmd != STOP) {
            cout << "Playing is stopped" << endl;
        }

        currentCmd = STOP;
    }
};

int main() {
    string cmd;
    time_t currTime = time(nullptr);

    vector<Track *> playList;
    FillPlaylist(playList);
    auto *player = new AudioPlayer();
    player->setupPlaylist(playList);

    while (true) {
        cout << "Enter command (play|pause|next|stop|exit): " << endl;
        cin >> cmd;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (cmd == "play") {
            player->play();
        } else if (cmd == "pause") {
            player->pause();
        } else if (cmd == "next") {
            player->next();
        } else if (cmd == "stop") {
            player->stop();
        } else if (cmd == "exit") {
            break;
        } else {
            cout << "Error. Incorrect command" << endl;
        }
    }

    for (auto &track : playList) {
        delete track;
    }
    delete player;
    return 0;
}
