#pragma once

class Person
{
public:
    int getHealth();
    int getItem(int idx);

private:
    int health = 100;
    vector<int> item;
};

int Person::getHealth()
{
    return health;
}

int getItem(int idx)
{
    if(-1 < idx  && idx < item.size() )
    {
        return item.at(idx);
    }
    else
    {
        return -1;
    }
}

/*
item list
    0: 
    1: 
*/