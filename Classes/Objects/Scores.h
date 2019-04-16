#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"

class Character;

struct KDA
{
    int kill;
    int die;
    
    KDA():kill(0),die(0){}
    void reset()
    {
        kill = 0;
        die = 0;
    }
    
    void operator=(const KDA& newkda)
    {
        kill = newkda.kill;
        die = newkda.die;
    }
};

class Scores
{
    using score = pair<weak_ptr<Character>, KDA>;
    
    vector<score> _listScores;
public:
    Scores();
    ~Scores();
    
    void addCharacter(const shared_ptr<Character>& character);
    void update(const shared_ptr<Character>& character, KDA newkda);
    void reset();
    const vector<score>& getScores() const;
};
