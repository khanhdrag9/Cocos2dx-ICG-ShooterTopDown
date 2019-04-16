//
//  Scores.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/16/19.
//

#include "Scores.h"
#include "../Characters/Character.h"

Scores::Scores()
{
}

Scores::~Scores()
{
    _listScores.clear();
}

void Scores::addCharacter(const shared_ptr<Character>& character)
{
    _listScores.push_back(score(character, KDA()));
}

void Scores::update(const shared_ptr<Character>& character, KDA newkda)
{
    for(auto& s : _listScores)
    {
        auto score = s.first.lock();
        if(score)
        {
            if(&(*score.get()) == &(*character.get()))
            {
                s.second = newkda;
                break;
            }
        }
    }
}

void Scores::reset()
{
    for(auto& s : _listScores)
    {
        s.second.reset();
    }
}

const vector<Scores::score>& Scores::getScores() const
{
    return _listScores;
}
