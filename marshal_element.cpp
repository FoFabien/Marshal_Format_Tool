#include "marshal_element.hpp"
#include <iostream>

MaElemTrue::MaElemTrue()
{
    type = 'T';
}

MaElemFalse::MaElemFalse()
{
    type = 'F';
}

MaElemNil::MaElemNil()
{
    type = '0';
}

//------------------------------------------------

MaElemHash::MaElemHash()
{
    type = '{';
}

MaElemHash::~MaElemHash()
{
    for(size_t i = 0; i < hashs.size(); ++i)
    {
        delete hashs[i].first;
        delete hashs[i].second;
    }
}

int MaElemHash::getSize() const
{
    int r = 0;
    for(size_t i = 0; i < hashs.size(); ++i)
    {
        r += hashs[i].first->getSize();
        r += hashs[i].second->getSize();
    }
    return r;
}

void MaElemHash::add(std::pair<MaElemBase*, MaElemBase*> &p)
{
    hashs.push_back(p);
}

const std::vector<std::pair<MaElemBase*, MaElemBase*> >& MaElemHash::getHashs() const
{
    return hashs;
}

//------------------------------------------------

MaElemItems::MaElemItems()
{
    type = -2;
}

MaElemItems::~MaElemItems()
{
    for(size_t i = 0; i < items.size(); ++i)
    {
        delete items[i].first;
        delete items[i].second;
    }
}

int MaElemItems::getSize() const
{
    int r = 0;
    for(size_t i = 0; i < items.size(); ++i)
    {
        r += items[i].first->getSize();
        r += items[i].second->getSize();
    }
    return r;
}

void MaElemItems::add(std::pair<MaElemBase*, MaElemBase*> &p)
{
    items.push_back(p);
}

const std::vector<std::pair<MaElemBase*, MaElemBase*> >& MaElemItems::getItems() const
{
    return items;
}

//------------------------------------------------

MaElemRaw::MaElemRaw()
{
    type = -1;
}

MaElemRaw::~MaElemRaw()
{

}

int MaElemRaw::getSize() const
{
    return 1;
}

void MaElemRaw::add(uint8_t c)
{
    raw.push_back(c);
}

void MaElemRaw::swapRaw(std::vector<uint8_t>& r)
{
    raw.swap(r);
}

const std::vector<uint8_t>& MaElemRaw::getRaw() const
{
    return raw;
}

//------------------------------------------------

MaElemSymbol::MaElemSymbol()
{
    type = ':';
}

MaElemSymbol::~MaElemSymbol()
{

}

int MaElemSymbol::getSize() const
{
    return 1;
}

//------------------------------------------------

MaElemString::MaElemString()
{
    type = '"';
}

MaElemString::~MaElemString()
{

}

int MaElemString::getSize() const
{
    return 1;
}

//------------------------------------------------

MaElemVar::MaElemVar()
{
    type = -1;
    main = NULL;
}

MaElemVar::~MaElemVar()
{
    delete main;
    for(size_t i = 0; i < sub.size(); ++i)
        delete sub[i];
}

int MaElemVar::getSize() const
{
    int r = 0;
    if(main) r += main->getSize();
    for(size_t i = 0; i < sub.size(); ++i)
        r += sub[i]->getSize();
    return r;
}

void MaElemVar::setMain(MaElemBase* m)
{
    main = m;
}

const MaElemBase* MaElemVar::getMain() const
{
    return main;
}

void MaElemVar::addSub(MaElemBase* m)
{
    sub.push_back(m);
}

const std::vector<MaElemBase*>& MaElemVar::getSub() const
{
    return sub;
}

MaElemItems* MaElemVar::convertSubToItems()
{
    MaElemItems* items = new MaElemItems();
    std::pair<MaElemBase*, MaElemBase*> p;
    bool state = false;
    for(size_t i = 0; i < sub.size(); ++i)
    {
        if(!state)
        {
            p.first = sub[i];
            state = true;
        }
        else
        {
            p.second = sub[i];
            items->add(p);
            state = false;
        }
    }
    if(state) delete p.first;
    sub.clear();
    return items;
}

//------------------------------------------------

MaElemObject::MaElemObject()
{
    type = 'o';
}

MaElemObject::~MaElemObject()
{

}

int MaElemObject::getSize() const
{
    return MaElemVar::getSize();
}

//------------------------------------------------

MaElemIVar::MaElemIVar()
{
    type = 'I';
}

MaElemIVar::~MaElemIVar()
{

}

int MaElemIVar::getSize() const
{
    return MaElemVar::getSize();
}

//------------------------------------------------

MaElemUClass::MaElemUClass()
{
    type = 'C';
}

MaElemUClass::~MaElemUClass()
{

}

int MaElemUClass::getSize() const
{
    return MaElemVar::getSize();
}

//------------------------------------------------

MaElemArray::MaElemArray()
{
    type = '[';
}

MaElemArray::~MaElemArray()
{

}

int MaElemArray::getSize() const
{
    return MaElemVar::getSize();
}

//------------------------------------------------

MaElemNumber::MaElemNumber()
{
    type = -1;
    n = 0;
}

MaElemNumber::~MaElemNumber()
{

}

int MaElemNumber::getSize() const
{
    return 1;
}

void MaElemNumber::setValue(int i)
{
    n = i;
}

int MaElemNumber::getValue() const
{
    return n;
}

//------------------------------------------------

MaElemSymlink::MaElemSymlink()
{
    type = ';';
}

MaElemSymlink::~MaElemSymlink()
{

}

int MaElemSymlink::getSize() const
{
    return 1;
}

//------------------------------------------------

MaElemObjlink::MaElemObjlink()
{
    type = '@';
}

MaElemObjlink::~MaElemObjlink()
{

}

int MaElemObjlink::getSize() const
{
    return 1;
}

//------------------------------------------------

MaElemInteger::MaElemInteger()
{
    type = 'i';
}

MaElemInteger::~MaElemInteger()
{

}

int MaElemInteger::getSize() const
{
    return 1;
}

//------------------------------------------------

MaElemUserdef::MaElemUserdef()
{
    type = 'u';
    main = NULL;
}

MaElemUserdef::~MaElemUserdef()
{
    delete main;
}

int MaElemUserdef::getSize() const
{
    return main->getSize();
}

void MaElemUserdef::setMain(MaElemBase* m)
{
    main = m;
}

const MaElemBase* MaElemUserdef::getMain() const
{
    return main;
}

void MaElemUserdef::addChar(uint8_t c)
{
    content.push_back(c);
}

const std::vector<uint8_t>& MaElemUserdef::getContent() const
{
    return content;
}
