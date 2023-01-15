#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <vector>
#include <algorithm>
#include "OOP5EventException.h"

using namespace std;

template <typename T>
class Subject
{

    vector<Observer<T>*> observers;

public:
    Subject()
    {
        observers = vector<Observer<T>*>();
    }

    virtual ~Subject() = default;

    void notify(const T &event)
    {
        for (Observer<T>* o : observers)
        {
            o->handleEvent(event);
        }
    }

    void addObserver(Observer<T> &o)
    {
        if (find(observers.begin(), observers.end(), &o) != observers.end())
        {
            throw ObserverAlreadyKnownToSubject();
        }

        observers.emplace_back(&o);
    }

    void removeObserver(Observer<T> &o)
    {
        auto it = find(observers.begin(), observers.end(), &o);
        if (it != observers.end())
        {
            observers.erase(it);
            return;
        }

        throw ObserverUnknownToSubject();
    }

    Subject<T> &operator+=(Observer<T> &o)
    {
        this->addObserver(o);
        return *this;
    }

    Subject<T> &operator-=(Observer<T> &o)
    {
        this->removeObserver(o);
        return *this;
    }

    Subject<T> &operator()(const T &event)
    {
        this->notify(event);
        return *this;
    }
};

#endif