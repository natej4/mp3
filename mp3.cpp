#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

class Song {
public:
    string title;
    int time;
    int track;      // Primary key
    bool operator < (Song another) const { return title < another.title;}
    friend ostream & operator<<(ostream& out, const Song& l);
};

ostream & operator << (ostream& out, Song & l) {     
    cout << "\t\t" << l.track << ".  " << l.title << " " << l.time/100 << ":";
    if ((l.time % 100) < 10) cout << "0";
    cout << l.time % 100;
    return out;
}

class Album {
    public :
        mutable map<int, Song> songs;
        string name;  // Album's name
        mutable string artist;
        mutable string genre;
        mutable int time;     // Total time of all songs on album
        mutable int nsongs;   // Total number of songs on album
        bool operator < (Album another) const { return name < another.name;}
    bool operator == (string albumName) const { return name == albumName;}
    friend std::ostream& operator<<(std::ostream& out, const Album& al);
};

ostream& operator<<(ostream& out, const Album& al)
{
  cout << "\t" << al.name << ": " << al.nsongs << ", " << al.time/100 << ":";
  if ((al.time % 100) < 10) cout << "0";
  cout << al.time % 100 << endl;
  for (map<int, Song>::const_iterator it=al.songs.begin(); it!=al.songs.end(); ++it){
    Song curSong = it->second;
    cout << curSong << endl;
  }
  return out;
}

class Artist {
   public :
        string name; // Artist's name
        mutable set<Album> albums;
        mutable int time;    // Total time of all songs on all albums by this artist
        mutable int nsongs;  // Total number of songs on all albums by this artist
        bool operator < (Artist another) const { return name < another.name;}
        bool operator == (string artistName) const { return name == artistName;}
        friend std::ostream& operator<<(std::ostream& out, const Artist& ar);
};

ostream& operator<<(ostream& out, const Artist& ar)
{
  cout << ar.name << ": " << ar.nsongs << ", " << ar.time/100 << ":";
  if ((ar.time % 100) < 10) cout << "0";
  cout << ar.time % 100 << endl;
  for (set<Album>::iterator it=ar.albums.begin(); it!=ar.albums.end(); ++it)
    cout << *it << endl;
  return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//function headers
void fillSets (fstream& in, set<Artist>& artists, set<Album>& albums);
void display (set<Artist>& artists);
string removeUnderscore(string text);

int main(int argc, char* argv[]){
    if (argc == 1){ //check to make sure a file is specified
        cout << "mp3: missing filename" << endl;
        return -1;
    }

    else{
        string fn = argv[1];
        fstream in(fn.c_str());

        set<Artist> artists;
        set<Album> albums;
        fillSets(in, artists, albums);
        display(artists);
    }
}

void fillSets (fstream& in, set<Artist>& artists, set<Album>& albums){
    string title, art, album, gen;
    float minutes,seconds;
    int track;
    while (in >> title){
        //reading in file one line at a time
        in >> minutes;
        in.ignore();
        in >> seconds >> art >> album >> gen >> track;
        Song s;
        //fixing format of time
        minutes *= 100;
        if(seconds >= 60){
            seconds -= 60;
            minutes += 100;
        }
        minutes += seconds;

        //reformatting strings
        art = removeUnderscore(art);
        album = removeUnderscore(album);
        title = removeUnderscore(title);

        s.title = title;
        s.time = minutes;
        s.track = track;
        
        //adding the song to an album and inserting that album in a set
        Album a;
        a.name = album;
        if(albums.count(a) == 0) albums.insert(a);
        set<Album>::iterator it = albums.find(a);
        it->artist = art;
        it->genre = gen;
        it->nsongs++;
        it->time += minutes;
        it->songs.insert(pair<int,Song>(track, s));

        //adding the artist of each song to a set
        Artist c;
        c.name = art;
        if(artists.count(c)==0) artists.insert(c);
        

    }
    //going through the two sets and finding all the albums by that artist
    for(set<Artist>::iterator iu = artists.begin(); iu != artists.end(); iu++){
        set<Album>::iterator it = albums.begin();
        for(it; it != albums.end(); it++){
            if(it->artist == iu->name){
                iu->nsongs += it->nsongs;
                if(it->time % 10 >= 6){
                    it->time += 40;
                }
                iu->time += it->time;
                iu->albums.insert(*it);
            }
        }
    }
}

//runs through set of artists and uses operators overloaded above to neatly print all information
void display (set<Artist>& artists){
    for(set<Artist>::iterator it = artists.begin(); it != artists.end(); it++){
        cout << *it << endl;
    }    
}

//short function that replaces underscores in a string with spaces
string removeUnderscore(string text) {
    for(std::string::iterator it = text.begin(); it != text.end(); ++it) {
        if(*it == '_') {
            *it = ' ';
        }
    }
    return text;
}