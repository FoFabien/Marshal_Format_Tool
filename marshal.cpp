#include "marshal.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "func.hpp"

std::ofstream fout; // for debug
//#define DEBUG

Marshal::Marshal()
{
    #ifdef DEBUG
    if(!fout.is_open()) fout.open("log.txt", std::ios::out | std::ios::trunc);
    #endif

	// for RMXP common events file
    code_table[0]   = "[Empty]";
    code_table[101] = "[Message]";
    code_table[102] = "[Choice]";
    code_table[103] = "[Input Value]";
    code_table[104] = "[Message Option]";
    code_table[105] = "[Key Input]";
    code_table[106] = "[Wait]";
    code_table[108] = "[Comment]";
    code_table[111] = "[Condition]";
    code_table[112] = "[Loop]";
    code_table[113] = "[Break Loop]";
    code_table[115] = "[Stop Event Processing]";
    code_table[116] = "[Erase Event]";
    code_table[117] = "[Call Event]";
    code_table[118] = "[Label]";
    code_table[119] = "[Jump]";
    code_table[121] = "[Switch]";
    code_table[122] = "[Variable]";
    code_table[123] = "[Self Switch]";
    code_table[124] = "[Timer]";
    code_table[125] = "[Gold]";
    code_table[126] = "[Items]";
    code_table[127] = "[Weapons]";
    code_table[128] = "[Armor]";
    code_table[129] = "[Party Member]";
    code_table[131] = "[Windowskin]";
    code_table[132] = "[Battle BGM]";
    code_table[132] = "[Battle End ME]";
    code_table[133] = "[Battle End ME]";
    code_table[134] = "[Save Access]";
    code_table[135] = "[Menu Access]";
    code_table[136] = "[Encounter]";
    code_table[201] = "[Move Player]";
    code_table[202] = "[Move Event]";
    code_table[203] = "[Scroll Map]";
    code_table[204] = "[Map Settings]";
    code_table[205] = "[Fog Color Tone]";
    code_table[206] = "[Fog Opacity]";
    code_table[207] = "[Show Animation]";
    code_table[208] = "[Transparent Flag]";
    code_table[209] = "[Move Route]";
    code_table[210] = "[Wait for Movevement]";
    code_table[221] = "[Prepare Transition]";
    code_table[222] = "[Execute Transition]";
    code_table[223] = "[Screen Color Tone]";
    code_table[224] = "[Screen Flash]";
    code_table[225] = "[Screen Shake]";
    code_table[231] = "[Show Picture]";
    code_table[232] = "[Move Picture]";
    code_table[233] = "[Rotate Picture]";
    code_table[234] = "[Picture Color Tone]";
    code_table[235] = "[Erase Picture]";
    code_table[236] = "[Set Weather Effects]";
    code_table[241] = "[Play BGM]";
    code_table[242] = "[Fade Out BGM]";
    code_table[245] = "[Play BGS]";
    code_table[246] = "[Fade Out BGS]";
    code_table[247] = "[Memorize BGM/BGS]";
    code_table[248] = "[Restore BGM/BGS]";
    code_table[249] = "[Play ME]";
    code_table[250] = "[Play SE]";
    code_table[251] = "[Stop SE]";
    code_table[301] = "[Battle Processing]";
    code_table[302] = "[Shop Processing]";
    code_table[303] = "[Name Input Processing]";
    code_table[311] = "[Change HP]";
    code_table[312] = "[Change SP]";
    code_table[313] = "[Change State]";
    code_table[314] = "[Recover All]";
    code_table[315] = "[Change EXP]";
    code_table[316] = "[Change Level]";
    code_table[317] = "[Change Parameters]";
    code_table[318] = "[Change Skills]";
    code_table[319] = "[Change Equipment]";
    code_table[320] = "[Change Actor Name]";
    code_table[321] = "[Change Actor Class]";
    code_table[322] = "[Change Actor Graphic]";
    code_table[331] = "[Change Enemy HP]";
    code_table[332] = "[Change Enemy SP]";
    code_table[333] = "[Change Enemy State]";
    code_table[334] = "[Enemy Recover All]";
    code_table[335] = "[Enemy Appearance]";
    code_table[336] = "[Enemy Transform]";
    code_table[337] = "[Show Battle Animation]";
    code_table[338] = "[Deal Damage]";
    code_table[339] = "[Force Action]";
    code_table[340] = "[Abort Battle]";
    code_table[351] = "[Call Menu Screen]";
    code_table[352] = "[Call Save Screen]";
    code_table[353] = "[Game Over]";
    code_table[354] = "[Return to Title Screen]";
    code_table[355] = "[Script]";
    code_table[401] = "[Message+]";
    code_table[402] = "[When]";
    code_table[403] = "[When Cancel]";
    code_table[404] = "[Choices End]";
    code_table[408] = "[Comment+]";
    code_table[411] = "[Else]";
    code_table[412] = "[Branch End]";
    code_table[413] = "[Repeat Above]";
    code_table[509] = "[Move Command]";
    code_table[601] = "[If Win]";
    code_table[602] = "[If Escape]";
    code_table[603] = "[If Lose]";
    code_table[604] = "[Battle Processing End]";
    code_table[605] = "[Shop Item]";
    code_table[655] = "[Script+]";

    for(std::map<int, std::string>::iterator it = code_table.begin(); it != code_table.end(); ++it)
        inv_code_table[it->second] = it->first;
}

Marshal::~Marshal()
{
    for(size_t i = 0; i < elems.size(); ++i) delete elems[i];
}

void Marshal::clear()
{
    for(size_t i = 0; i < elems.size(); ++i) delete elems[i];
    elems.clear();
}

int Marshal::read(const std::string& file)
{
    std::ifstream f(file.c_str(), std::ios::in | std::ios::binary);

    if(!f) return -1;

    return readFile(f);
}

int Marshal::write(const std::string& file)
{
   std::ofstream f(file.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

    if(!f) return -1;

    return writeFile(f);
}

int Marshal::getElemCount() const
{
    int r = 0;
    for(size_t i = 0; i < elems.size(); ++i) r += elems[i]->getSize();
    return r;
}

void Marshal::print() const
{
    for(size_t i = 0; i < elems.size(); ++i)
    {
        print_elem(elems[i]);
    }
}

bool Marshal::dump(const std::string& file) const
{
    std::ofstream f(file.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
    if(!f) return false;

    uint8_t c = 0xef;
    f.write((char*)&c, 1);
    c = 0xbb;
    f.write((char*)&c, 1);
    c = 0xbf;
    f.write((char*)&c, 1);

    for(size_t i = 0; i < elems.size(); ++i)
    {
        if(!dump_elem(f, elems[i])) return false;
    }
    f.close();
    return true;
}

int Marshal::readFile(std::istream &f)
{
    int8_t tmp8;
    f.read((char*)&tmp8, 1);
    if(tmp8 != 0x04) return 1;
    f.read((char*)&tmp8, 1);
    if(tmp8 != 0x08) return 2;

    while(true)
    {
        MaElemBase* e = readRubyValue(f);
        if(e == NULL)
        {
            if(f.good()) return 1; // something bad happened
            else return 0; // end of file
        }
        else elems.push_back(e);
    }
    return 0;
}

MaElemBase* Marshal::readRubyValue(std::istream &f)
{
    char c;
    f.read((char*)&c, 1);
    if(!f.good()) return NULL;

    MaElemBase* e = NULL;
    switch(c)
    {
        default:
            std::cout << "* (0x" << dec2hex(((int)f.tellg()) - 1) << ") Error" << std::endl;
            break;
        case '0':
            e = new MaElemNil();
            break;
        case 'T':
            e = new MaElemTrue();
            break;
        case 'F':
            e = new MaElemFalse();
            break;
        case '{':
            e = readRubyHash(f);
            break;
        case ':':
            e = readRubySymbol(f);
            break;
        case ';':
            e = readRubySymlink(f);
            break;
        case 'I':
            e = readRubyIvar(f);
            break;
        case '"':
            e = readRubyString(f);
            break;
        case 'C':
            e = readRubyUclass(f);
            break;
        case 'o':
            e = readRubyObject(f);
            break;
        case 'i':
            e = readRubyInteger(f);
            break;
        case '[':
            e = readRubyArray(f);
            break;
        case '@':
            e = readRubyObjlink(f);
            break;
        case 'u':
            e = readRubyUserdef(f);
            break;
        case 'e':
            break;
        case 'd':
            break;
        case 'U':
            break;
        case 'f':
            break;
        case 'l':
            break;
        case '/':
            break;
        case '}':
            break;
        case 'S':
            break;
        case 'M':
            break;
        case 'm':
            break;
    }
    return e;
}

MaElemBase* Marshal::readRubyHash(std::istream &f)
{
    int l = readRubyLong(f);
    if(!f.good()) return NULL;

    MaElemHash* e = new MaElemHash();

    for(; l > 0; --l)
    {
        std::pair<MaElemBase*, MaElemBase*> p;
        p.first = readRubyValue(f);
        if(!p.first)
        {
            delete e;
            return NULL;
        }
        p.second = readRubyValue(f);
        if(!p.second)
        {
            delete e;
            delete p.first;
            return NULL;
        }
        e->add(p);
    }

    return e;
}

MaElemBase* Marshal::readRubySymbol(std::istream &f)
{
    int l = readRubyLong(f);
    if(!f.good()) return NULL;

    MaElemSymbol* e = new MaElemSymbol();

    for(; l > 0; --l)
    {
        uint8_t c;
        f.read((char*)&c, 1);
        if(!f.good())
        {
            delete e;
            return NULL;
        }
        e->add(c);
    }
    return e;
}

MaElemBase* Marshal::readRubyString(std::istream &f)
{
    int l = readRubyLong(f);
    if(!f.good()) return NULL;

    MaElemString* e = new MaElemString();

    for(; l > 0; --l)
    {
        uint8_t c;
        f.read((char*)&c, 1);
        if(!f.good())
        {
            delete e;
            return NULL;
        }
        e->add(c);
    }
    return e;
}

MaElemBase* Marshal::readRubyObject(std::istream &f)
{
    MaElemBase* p = readRubyValue(f);
    if(!p) return NULL;
    MaElemObject* e = new MaElemObject();
    e->setMain(p);
    p = readRubyItems(f);
    if(!p)
    {
        delete e;
        return NULL;
    }
    e->addSub(p);
    return e;
}

MaElemBase* Marshal::readRubyIvar(std::istream &f)
{
    MaElemBase* p = readRubyValue(f);
    if(!p) return NULL;
    MaElemIVar* e = new MaElemIVar();
    e->setMain(p);
    p = readRubyItems(f);
    if(!p)
    {
        delete e;
        return NULL;
    }
    e->addSub(p);
    return e;
}

MaElemBase* Marshal::readRubyUclass(std::istream &f)
{
    MaElemBase* p = readRubyValue(f);
    if(!p) return NULL;
    MaElemUClass* e = new MaElemUClass();
    e->setMain(p);
    p = readRubyValue(f);
    if(!p)
    {
        delete e;
        return NULL;
    }
    e->addSub(p);
    return e;
}

MaElemBase* Marshal::readRubySymlink(std::istream &f)
{
    MaElemSymlink* e = new MaElemSymlink();
    e->setValue(readRubyLong(f));
    if(!f.good())
    {
        delete e;
        return NULL;
    }
    return e;
}

MaElemBase* Marshal::readRubyItems(std::istream &f)
{
    int l = readRubyLong(f);
    if(!f.good()) return NULL;

    MaElemItems* e = new MaElemItems();

    for(; l > 0; --l)
    {
        std::pair<MaElemBase*, MaElemBase*> p;
        p.first = readRubyValue(f);
        if(!p.first)
        {
            delete e;
            return NULL;
        }
        p.second = readRubyValue(f);
        if(!p.second)
        {
            delete e;
            delete p.first;
            return NULL;
        }
        e->add(p);
    }
    return e;
}

MaElemBase* Marshal::readRubyInteger(std::istream &f)
{
    MaElemInteger* e = new MaElemInteger();
    e->setValue(readRubyLong(f));
    if(!f.good())
    {
        delete e;
        return NULL;
    }
    return e;
}

MaElemBase* Marshal::readRubyArray(std::istream &f)
{
    MaElemArray* e = new MaElemArray();
    for(int l = readRubyLong(f); l > 0; --l)
    {
        MaElemBase* p = readRubyValue(f);
        if(!p)
        {
            delete e;
            return NULL;
        }
        e->addSub(p);
    }
    return e;
}

MaElemBase* Marshal::readRubyObjlink(std::istream &f)
{
    MaElemObjlink* e = new MaElemObjlink();
    e->setValue(readRubyLong(f));
    if(!f.good())
    {
        delete e;
        return NULL;
    }
    return e;
}

MaElemBase* Marshal::readRubyUserdef(std::istream &f)
{
    MaElemBase* p = readRubyValue(f);
    if(!p) return NULL;
    MaElemUserdef* e = new MaElemUserdef();
    e->setMain(p);

    for(int l = readRubyLong(f); l > 0; --l)
    {
        uint8_t c;
        f.read((char*)&c, 1);
        if(!f.good())
        {
            delete e;
            return NULL;
        }
        e->addChar(c);
    }
    return e;
}

int Marshal::readRubyLong(std::istream &f)
{
    char tmp;
    int r = 0;
    f.read(&tmp, 1);
    if(!f.good()) return 1;
    char loop = tmp;
    switch(tmp)
    {
        case 0: return 0; // zero
        case 1: // positive multiple bytes
        case 2:
        case 3:
        case 4:
            for(int i = 0; i < loop; ++i)
            {
                f.read(&tmp, 1);
                if(!f.good()) return 1;
                r |= ((uint8_t)tmp) << (8 * i);
            }
            break;
        case -1: // negative multiple bytes
        case -2:
        case -3:
        case -4:
        {
            loop = -loop;
            int i;
            for(i = 0; i < loop; ++i)
            {
                f.read(&tmp, 1);
                if(!f.good()) return 1;
                r |= ((uint8_t)tmp) << (8 * i);
            }
            for(; i < 4; ++i)
            {
                tmp = 0xff;
                r |= ((uint8_t)tmp) << (8 * i);
            }
            break;
        }
        default: // single byte
            if(tmp > 0) r = tmp - 5;
            else r = tmp + 5;
            break;
    }
    return r;
}

int Marshal::writeFile(std::ostream &f)
{
    int8_t tmp8;
    tmp8 = 0x04;
    f.write((char*)&tmp8, 1);
    tmp8 = 0x08;
    f.write((char*)&tmp8, 1);

    for(size_t i = 0; i < elems.size(); ++i)
    {
        if(!writeRubyValue(f, elems[i])) return 1;
    }

    return 0;
}
bool Marshal::writeRubyValue(std::ostream &f, const MaElemBase* e)
{
    if(!e) return false;
    uint8_t c = e->getType();
    f.write((char*)&c, 1);
    if(!f.good()) return false;

    switch(e->getType())
    {
        default: // error
            return false;
        case '0':
            return true;
        case 'T':
            return true;
        case 'F':
            return true;
        case '{':
            return writeRubyHash(f, e);
        case ':':
            return writeRubySymbol(f, e);
        case ';':
            return writeRubySymlink(f, e);
        case 'I':
            return writeRubyIvar(f, e);
        case '"':
            return writeRubyString(f, e);
        case 'C':
            return writeRubyUclass(f, e);
        case 'o':
            return writeRubyObject(f, e);
        case 'i':
            return writeRubyInteger(f, e);
        case '[':
            return writeRubyArray(f, e);
        case '@':
            return writeRubyObjlink(f, e);
        case 'u':
            return writeRubyUserdef(f, e);
        case 'e':
        case 'd':
        case 'U':
        case 'f':
        case 'l':
        case '/':
        case '}':
        case 'S':
        case 'M':
        case 'm':
            return false;
    }
    return false;
}

bool Marshal::writeRubyHash(std::ostream &f, const MaElemBase* e)
{
    const std::vector<std::pair<MaElemBase*, MaElemBase*> > p = ((MaElemHash*)e)->getHashs();
    if(!writeRubyLong(f, (int)p.size())) return false;

    for(size_t i = 0; i < p.size(); ++i)
    {
        if(!writeRubyValue(f, p[i].first)) return false;
        if(!writeRubyValue(f, p[i].second)) return false;
    }
    return true;
}

bool Marshal::writeRubySymbol(std::ostream &f, const MaElemBase* e)
{
    std::vector<uint8_t> r = ((MaElemSymbol*)e)->getRaw();
    if(!writeRubyLong(f, (int)r.size())) return false;

    for(size_t i = 0; i < r.size(); ++i)
        f.write((char*)&(r[i]), 1);
    return true;
}

bool Marshal::writeRubyString(std::ostream &f, const MaElemBase* e)
{
    std::vector<uint8_t> r = ((MaElemString*)e)->getRaw();
    if(!writeRubyLong(f, (int)r.size())) return false;

    for(size_t i = 0; i < r.size(); ++i)
        f.write((char*)&(r[i]), 1);
    return true;
}

bool Marshal::writeRubyObject(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyValue(f, ((MaElemObject*)e)->getMain())) return false;
    const std::vector<MaElemBase*> s = ((MaElemObject*)e)->getSub();
    if(s.empty()) return false;
    if(s[0]->getType() != -2) return false;
    if(!writeRubyItems(f, s[0])) return false;
    return true;
}

bool Marshal::writeRubyIvar(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyValue(f, ((MaElemIVar*)e)->getMain())) return false;
    const std::vector<MaElemBase*> s = ((MaElemIVar*)e)->getSub();
    if(s.empty()) return false;
    if(!writeRubyValue(f, s[0])) return false;
    return true;
}

bool Marshal::writeRubyUclass(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyValue(f, ((MaElemUClass*)e)->getMain())) return false;
    const std::vector<MaElemBase*> s = ((MaElemUClass*)e)->getSub();
    if(s.empty()) return false;
    if(!writeRubyValue(f, s[0])) return false;
    return true;
}

bool Marshal::writeRubySymlink(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyLong(f, ((MaElemSymlink*)e)->getValue())) return false;
    return true;
}

bool Marshal::writeRubyItems(std::ostream &f, const MaElemBase* e)
{
    const std::vector<std::pair<MaElemBase*, MaElemBase*> > p = ((MaElemItems*)e)->getItems();
    if(!writeRubyLong(f, (int)p.size())) return false;

    for(size_t i = 0; i < p.size(); ++i)
    {
        if(!writeRubyValue(f, p[i].first)) return false;
        if(!writeRubyValue(f, p[i].second)) return false;
    }
    return true;
}

bool Marshal::writeRubyInteger(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyLong(f, ((MaElemInteger*)e)->getValue())) return false;
    return true;
}

bool Marshal::writeRubyArray(std::ostream &f, const MaElemBase* e)
{
    const std::vector<MaElemBase*> v = ((MaElemArray*)e)->getSub();
    if(!writeRubyLong(f, (int)v.size())) return false;

    for(size_t i = 0; i < v.size(); ++i)
        if(!writeRubyValue(f, v[i])) return false;

    return true;
}

bool Marshal::writeRubyObjlink(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyLong(f, ((MaElemObjlink*)e)->getValue())) return false;
    return true;
}

bool Marshal::writeRubyUserdef(std::ostream &f, const MaElemBase* e)
{
    if(!writeRubyValue(f, ((MaElemUserdef*)e)->getMain())) return false;
    const std::vector<uint8_t> c = ((MaElemUserdef*)e)->getContent();
    if(!writeRubyLong(f, (int)c.size())) return false;

    for(size_t i = 0; i < c.size(); ++i)
        f.write((const char*)&(c[i]), 1);

    return true;
}

bool Marshal::writeRubyLong(std::ostream &f, int l)
{
    if(l >= -123 && l <= 122)
    {
        if(l > 0) l+= 5;
        else if(l < 0) l -= 5;
        f.write((char*)&l, 1);
    }
    else if(l > 0)
    {
        char n = 0;
        for(char i = 0; i < 4; ++i)
        {
            if(((uint8_t)((l >> (8 * i)) % 0x100)) == 0) n++;
            else n = 0;
        }
        n = 4 - n;
        f.write(&n, 1);
        for(char i = 0; i < n; ++i)
        {
            uint8_t c = (uint8_t)((l >> (8 * i)) % 0x100);
            f.write((char*)&c, 1);
        }
    }
    else
    {
        char n = 0;
        for(char i = 0; i < 4; ++i)
        {
            if(((uint8_t)((l >> (8 * i)) % 0x100)) == 0xFF) n++;
            else n = 0;
        }
        n = 4 - n;
        n = -n;
        f.write(&n, 1);
        n = -n;
        for(char i = 0; i < n; ++i)
        {
            uint8_t c = (uint8_t)((l >> (8 * i) % 0x100));
            f.write((char*)&c, 1);
        }
    }
    return true;
}

void Marshal::print_elem(const MaElemBase* e, int space) const
{
    if(!e)
    {
        std::cout << "Error: Null pointer" << std::endl;
        return;
    }
    for(int i = 0; i < space && e->getType() != -2; ++i) std::cout << "\t";

    switch(e->getType())
    {
        default:
            std::cout << "Error: " << e->getType() << std::endl;
            break;
        case -2: // items list
        {
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemItems*)e)->getItems();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                print_elem(tab[i].first, space);
                print_elem(tab[i].second, space);
            }
            break;
        }
        case '0':
            std::cout << "Nil" << std::endl;
            break;
        case 'T':
            std::cout << "True" << std::endl;
            break;
        case 'F':
            std::cout << "False" << std::endl;
            break;
        case '{':
        {
            std::cout << "Hash (";
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemHash*)e)->getHashs();
            std::cout << tab.size() << ")" << std::endl;
            for(size_t i = 0; i < tab.size(); ++i)
            {
                print_elem(tab[i].first, space+1);
                print_elem(tab[i].second, space+1);
            }
            break;
        }
        case ':':
        {
            std::cout << "Symbol: ";
            const std::vector<uint8_t> tab = ((MaElemSymbol*)e)->getRaw();
            for(size_t i = 0; i < tab.size(); ++i) std::cout << (char)tab[i];
            std::cout << std::endl;
            break;
        }
        case ';':
        {
            std::cout << "Symbol Link: " << ((MaElemSymlink*)e)->getValue() << " (";
            std::vector<uint8_t> tab = searchSymbol(((MaElemSymlink*)e)->getValue());
            for(size_t i = 0; i < tab.size(); ++i) std::cout << (char)tab[i];
            std::cout << ")" << std::endl;
            break;
        }
        case 'I':
        {
            std::cout << "IVar" << std::endl;
            print_elem(((MaElemIVar*)e)->getMain(), space+1);
            const std::vector<MaElemBase*> tab = ((MaElemIVar*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                print_elem(tab[i], space+1);
            }
            break;
        }
        case '"':
        {
            std::cout << "String (";
            const std::vector<uint8_t> tab = ((MaElemSymbol*)e)->getRaw();
            std::cout << tab.size() << "): ";
            if(tab.size() > 30) std::cout << "<Hidden, too big>" << std::endl;
            else
            {
                for(size_t i = 0; i < tab.size(); ++i)
                    if(tab[i] > 31 && tab[i] < 128) std::cout << (char)tab[i];
                    else std::cout << '?';
                std::cout << std::endl;
            }
            break;
        }
        case 'C':
        {
            std::cout << "UClass" << std::endl;
            print_elem(((MaElemUClass*)e)->getMain(), space+1);
            const std::vector<MaElemBase*> tab = ((MaElemUClass*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                print_elem(tab[i], space+1);
            }
            break;
        }
        case 'o':
        {
            std::cout << "Object" << std::endl;
            print_elem(((MaElemObject*)e)->getMain(), space+1);
            const std::vector<MaElemBase*> tab = ((MaElemObject*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                print_elem(tab[i], space+1);
            }
            break;
        }
        case 'i':
            std::cout << "Integer: " << ((MaElemInteger*)e)->getValue() << std::endl;
            break;
        case '[':
        {
            std::cout << "Array (";
            const std::vector<MaElemBase*> tab = ((MaElemArray*)e)->getSub();
            std::cout << tab.size() << ")" << std::endl;
            for(size_t i = 0; i < tab.size(); ++i)
            {
                print_elem(tab[i], space+1);
            }
            break;
        }
        case '@':
        {
            std::cout << "Object Link: " << ((MaElemObjlink*)e)->getValue() << " (";
            std::vector<uint8_t> tab = searchObject(((MaElemObjlink*)e)->getValue());
            for(size_t i = 0; i < tab.size(); ++i) std::cout << (char)tab[i];
            std::cout << ")" << std::endl;
            break;
        }
        case 'u':
        {
            std::cout << "User Defined" << std::endl;
            print_elem(((MaElemUserdef*)e)->getMain(), space+1);
            const std::vector<uint8_t> tab = ((MaElemUserdef*)e)->getContent();
            for(int i = 0; i < space+1; ++i) std::cout << "\t";
            std::cout << "Content (" << tab.size() << "): ";
            if(tab.size() > 30) std::cout << "<Hidden, too big>" << std::endl;
            else
            {
                for(size_t i = 0; i < tab.size(); ++i)
                    if(tab[i] > 31 && tab[i] < 128) std::cout << (char)tab[i];
                    else std::cout << '?';
                std::cout << std::endl;
            }
            break;
        }
    }
}

bool Marshal::dump_elem(std::ostream &f, const MaElemBase* e, int space) const
{
    if(!e) return false;

    for(int i = 0; i < space && e->getType() != -2; ++i) f.write("\t", 1);

    switch(e->getType())
    {
        default:
            return false;
        case -2: // items list
        {
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemItems*)e)->getItems();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                dump_elem(f, tab[i].first, space);
                dump_elem(f, tab[i].second, space);
            }
            break;
        }
        case '0':
            f.write("Nil\n", 4);
            break;
        case 'T':
            f.write("True\n", 5);
            break;
        case 'F':
            f.write("False\n", 6);
            break;
        case '{':
        {
            f.write("Hash (", 6);
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemHash*)e)->getHashs();
            std::string s = dec2str(tab.size()) + ")\n";
            f.write(s.c_str(), s.size());
            for(size_t i = 0; i < tab.size(); ++i)
            {
                dump_elem(f, tab[i].first, space+1);
                dump_elem(f, tab[i].second, space+1);
            }
            break;
        }
        case ':':
        {
            f.write("Symbol: ", 8);
            const std::vector<uint8_t> tab = ((MaElemSymbol*)e)->getRaw();
            for(size_t i = 0; i < tab.size(); ++i) f.write((char*)&tab[i], 1);
            f.write("\n", 1);
            break;
        }
        case ';':
        {
            std::string s = "Symbol Link: " + dec2str(((MaElemSymlink*)e)->getValue()) + " (";
            f.write(s.c_str(), s.size());
            std::vector<uint8_t> tab = searchSymbol(((MaElemSymlink*)e)->getValue());
            for(size_t i = 0; i < tab.size(); ++i) f.write((char*)&tab[i], 1);
            f.write(")\n", 2);
            break;
        }
        case 'I':
        {
            f.write("IVar\n", 5);
            dump_elem(f, ((MaElemIVar*)e)->getMain(), space+1);
            const std::vector<MaElemBase*> tab = ((MaElemIVar*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                dump_elem(f, tab[i], space+1);
            }
            break;
        }
        case '"':
        {
            f.write("String (", 8);
            const std::vector<uint8_t> tab = ((MaElemSymbol*)e)->getRaw();
            std::string s = dec2str(tab.size()) + ")=\"";
            f.write(s.c_str(), s.size());
            for(size_t i = 0; i < tab.size(); ++i)
                f.write((char*)&tab[i], 1);
            f.write("\"\n", 2);
            break;
        }
        case 'C':
        {
            f.write("UClass\n", 7);
            dump_elem(f, ((MaElemUClass*)e)->getMain(), space+1);
            const std::vector<MaElemBase*> tab = ((MaElemUClass*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                dump_elem(f, tab[i], space+1);
            }
            break;
        }
        case 'o':
        {
            f.write("Object: ", 8);
            dump_elem(f, ((MaElemObject*)e)->getMain());
            const std::vector<MaElemBase*> tab = ((MaElemObject*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                dump_elem(f, tab[i], space+1);
            }
            break;
        }
        case 'i':
        {
            std::string s = "Integer: " + dec2str(((MaElemInteger*)e)->getValue()) + "\n";
            f.write(s.c_str(), s.size());
            break;
        }
        case '[':
        {
            f.write("Array (", 7);
            const std::vector<MaElemBase*> tab = ((MaElemArray*)e)->getSub();
            std::string s = dec2str(tab.size()) + ")\n";
            f.write(s.c_str(), s.size());
            for(size_t i = 0; i < tab.size(); ++i)
            {
                dump_elem(f, tab[i], space+1);
            }
            break;
        }
        case '@':
        {
            std::string s = "Object Link: " + dec2str(((MaElemObjlink*)e)->getValue()) + " (";
            f.write(s.c_str(), s.size());
            std::vector<uint8_t> tab = searchObject(((MaElemObjlink*)e)->getValue());
            for(size_t i = 0; i < tab.size(); ++i) f.write((char*)&tab[i], 1);
            f.write(")\n", 2);
            break;
        }
        case 'u':
        {
            f.write("User Defined\n", 13);
            dump_elem(f, ((MaElemUserdef*)e)->getMain(), space+1);
            const std::vector<uint8_t> tab = ((MaElemUserdef*)e)->getContent();
            for(int i = 0; i < space+1; ++i) f.write("\t", 1);

            std::string s = "Content (" + dec2str(tab.size()) + "): ";
            f.write(s.c_str(), s.size());

            for(size_t i = 0; i < tab.size(); ++i)
            {
                s = dec2hex(tab[i]);
                f.write(s.c_str(), s.size());
            }
            f.write("\n", 1);
            break;
        }
    }
    return true;
}

bool Marshal::dumpCommonEvents(const std::string& file) const
{
    std::ofstream f(file.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

    if(!f) return false;
    if(elems.empty()) return false;
    if(elems[0]->getType() != '[') return false;

    uint8_t c = 0xef;
    f.write((char*)&c, 1);
    c = 0xbb;
    f.write((char*)&c, 1);
    c = 0xbf;
    f.write((char*)&c, 1);

    MaElemArray* events = (MaElemArray*)elems[0];
    const std::vector<MaElemBase*> &event_list = events->getSub();
    std::vector<CommonEvent> ces;
    int max_id = -1;

    for(size_t i = 1; i < event_list.size(); ++i) // element 0 should be Nil, we start at 1
    {
        ces.push_back(getCommonEvent(((MaElemObject*)event_list[i])->getSub()[0]));
        if(ces.back().id > max_id) max_id = ces.back().id;
    }

    for(size_t i = 0; i < ces.size(); ++i)
    {
        if(!ces[i].error)
        {
            std::string tmp = wstr2str(getNumber(ces[i].id, max_id)) + "=\"";
            f.write(tmp.c_str(), tmp.size());
            for(size_t j = 0; j < ces[i].name.size(); ++j)
                f.write((char*)&ces[i].name[j], 1);
            f.write("\"\n", 2);

            if(ces[i].has_trigger)
            {
                f.write("\ttrigger=", 9);
                tmp = dec2str(ces[i].trigger);
                f.write(tmp.c_str(), tmp.size());
                f.write("\n", 1);
            }
            if(ces[i].has_switch_id)
            {
                f.write("\tswitch_id=", 11);
                tmp = dec2str(ces[i].switch_id);
                f.write(tmp.c_str(), tmp.size());
                f.write("\n", 1);
            }
            if(ces[i].has_list)
            {
                f.write("\tcommand_list\n", 14);
                for(size_t j = 0; j < ces[i].list.size(); ++j)
                    if(!dumpEventCommand(f, ces[i].list[j])) return false;
            }
        }
    }
    return true;
}

bool Marshal::compile(const std::string& file)
{
    std::ifstream f(file.c_str(), std::ios::in | std::ios::binary);

    if(!f) return false;

    uint32_t bom = 0;
    f.read((char*)&bom, 3);
    if(bom != 0xbfbbef) f.seekg(f.beg); // check for UTF8 bom

    clear();
    MaElemArray* mainArray = new MaElemArray();
    elems.push_back(mainArray);
    mainArray->addSub(new MaElemNil()); // first element is Nil

    symbol_table.clear(); // clear symbol table
    sym_count = -1;
    line_count = 0;

    err_flag = false;
    while(f.good() && !err_flag)
    {
        MaElemObject* ptr = parseEvent(f);
        if(ptr) mainArray->addSub(ptr);
    }
    if(err_flag) return false;

    if(write("CommonEvents.rxdata"))
    {
        std::cout << "failed to write" << std::endl;
        return false;
    }

    return true;
}

const std::vector<uint8_t> Marshal::searchSymbol(int id) const
{
    int pos = -1;
    for(size_t i = 0; i < elems.size(); ++i)
    {
        const MaElemBase* r = search_elem(elems[i], id, pos, ':');
        if(r && r->getType() == ':')
        {
            return ((MaElemSymbol*)r)->getRaw();
        }
    }
    return std::vector<uint8_t>();
}

const std::vector<uint8_t> Marshal::searchObject(int id) const
{
    int pos = -1;
    for(size_t i = 0; i < elems.size(); ++i)
    {
        const MaElemBase* r = search_elem(elems[i], id, pos, 'o');
        if(r && r->getType() == 'o')
        {
            const MaElemBase* m = ((MaElemObject*)r)->getMain();
            if(!m) return std::vector<uint8_t>();
            switch(m->getType())
            {
                case ':': return ((const MaElemSymbol*)m)->getRaw();
                case ';': return searchSymbol(((const MaElemSymlink*)m)->getValue());
                default: return std::vector<uint8_t>();
            }
        }
    }
    return std::vector<uint8_t>();
}

const MaElemSymbol* Marshal::searchSymbolPtr(int id) const
{
    int pos = -1;
    for(size_t i = 0; i < elems.size(); ++i)
    {
        const MaElemBase* r = search_elem(elems[i], id, pos, ':');
        if(r && r->getType() == ':')
        {
            return ((const MaElemSymbol*)r);
        }
    }
    return NULL;
}

const MaElemObject*  Marshal::searchObjectPtr(int id) const
{
    int pos = -1;
    for(size_t i = 0; i < elems.size(); ++i)
    {
        const MaElemBase* r = search_elem(elems[i], id, pos, 'o');
        if(r && r->getType() == 'o')
        {
            return ((const MaElemObject*)r);
        }
    }
    return NULL;
}

const MaElemBase* Marshal::search_elem(const MaElemBase* e, int &id_target, int &id_current, char type) const
{
    if(!e) return NULL;

    if(e->getType() == type)
    {
        id_current++;
        if(id_current == id_target) return e;
    }
    const MaElemBase* r = NULL;

    switch(e->getType())
    {
        default: return NULL;
        case -2: // items list
        {
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemItems*)e)->getItems();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                r = search_elem(tab[i].first, id_target, id_current, type);
                if(r) return r;
                r = search_elem(tab[i].second, id_target, id_current, type);
                if(r) return r;
            }
            break;
        }
        case '0':
            return NULL;
        case 'T':
            return NULL;
        case 'F':
            return NULL;
        case '{':
        {
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemHash*)e)->getHashs();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                r = search_elem(tab[i].first, id_target, id_current, type);
                if(r) return r;
                r = search_elem(tab[i].second, id_target, id_current, type);
                if(r) return r;
            }
            break;
        }
        case ':':
            return NULL;
        case ';':
            return NULL;
        case 'I':
        {
            r = search_elem(((MaElemIVar*)e)->getMain(), id_target, id_current, type);
            if(r) return r;
            const std::vector<MaElemBase*> tab = ((MaElemIVar*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                r = search_elem(tab[i], id_target, id_current, type);
                if(r) return r;
            }
            break;
        }
        case '"':
            return NULL;
        case 'C':
        {
            r = search_elem(((MaElemUClass*)e)->getMain(), id_target, id_current, type);
            if(r) return r;
            const std::vector<MaElemBase*> tab = ((MaElemUClass*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                r = search_elem(tab[i], id_target, id_current, type);
                if(r) return r;
            }
            break;
        }
        case 'o':
        {
            r = search_elem(((MaElemObject*)e)->getMain(), id_target, id_current, type);
            if(r) return r;
            const std::vector<MaElemBase*> tab = ((MaElemObject*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                r = search_elem(tab[i], id_target, id_current, type);
                if(r) return r;
            }
            break;
        }
        case 'i':
            return NULL;
        case '[':
        {
            const std::vector<MaElemBase*> tab = ((MaElemArray*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                r = search_elem(tab[i], id_target, id_current, type);
                if(r) return r;
            }
            break;
        }
        case '@':
            return NULL;
        case 'u':
            r = search_elem(((MaElemUserdef*)e)->getMain(), id_target, id_current, type);
            if(r) return r;
            break;
    }
    return NULL;
}

CommonEvent Marshal::getCommonEvent(const MaElemBase* e) const
{
    CommonEvent ce;
    // init
    ce.error = true;
    ce.has_name = false;
    ce.has_list = false;
    ce.has_trigger = false;
    ce.has_switch_id = false;
    ce.has_id = false;
    // error safety
    if(e == NULL) return ce;
    if(e->getType() != -2) return ce;
    const std::vector<std::pair<MaElemBase*, MaElemBase*> > &items = ((MaElemItems*)e)->getItems();
    if(items.size() < 5) return ce; // must contain 5 elements at least

    std::string tmp;
    for(size_t i = 0; i < items.size(); ++i)
    {
        tmp = "";
        switch(items[i].first->getType())
        {
            case ':': // symbol
            {
                const std::vector<uint8_t> &raw = ((MaElemSymbol*)items[i].first)->getRaw();
                for(size_t i = 0; i < raw.size(); ++i)
                    tmp += (char)raw[i];
                break;
            }
            case ';': // symbol link
            {
                const std::vector<uint8_t> &raw = searchSymbol(((MaElemSymlink*)items[i].first)->getValue());
                for(size_t i = 0; i < raw.size(); ++i)
                    tmp += (char)raw[i];
                break;
            }
            default: return ce;
        }

        // check which symbol it is
        if(tmp == "@name")
        {
            if(items[i].second->getType() != '"') return ce;
            ce.name = ((MaElemString*)items[i].second)->getRaw();
            ce.has_name = true;
        }
        else if(tmp == "@list")
        {
            if(items[i].second->getType() != '[') return ce;
            ce.list = ((MaElemArray*)items[i].second)->getSub();
            if(!ce.list.empty()) ce.has_list = true;
        }
        else if(tmp == "@trigger")
        {
            if(items[i].second->getType() != 'i') return ce;
            ce.trigger = ((MaElemInteger*)items[i].second)->getValue();
            ce.has_trigger = true;
        }
        else if(tmp == "@switch_id")
        {
            if(items[i].second->getType() != 'i') return ce;
            ce.switch_id = ((MaElemInteger*)items[i].second)->getValue();
            ce.has_switch_id = true;
        }
        else if(tmp == "@id")
        {
            if(items[i].second->getType() != 'i') return ce;
            ce.id = ((MaElemInteger*)items[i].second)->getValue();
            ce.has_id = true;
        }
    }
    ce.error = false;
    return ce;
}

EventCommand Marshal::getEventCommand(const MaElemBase* e) const
{
    EventCommand ec;
    // init
    ec.error = true;
    ec.has_parameters = false;
    ec.has_indent = false;
    ec.has_code = false;
    // error safety
    if(e == NULL) return ec;
    if(e->getType() != -2) return ec; // must be an array
    const std::vector<std::pair<MaElemBase*, MaElemBase*> > &items = ((MaElemItems*)e)->getItems();
    if(items.size() < 3) return ec; // must contain 3 elements at least

    std::string tmp;
    for(size_t i = 0; i < items.size(); ++i)
    {
        tmp = "";
        switch(items[i].first->getType())
        {
            case ':': // symbol
            {
                const std::vector<uint8_t> &raw = ((MaElemSymbol*)items[i].first)->getRaw();
                for(size_t i = 0; i < raw.size(); ++i)
                    tmp += (char)raw[i];
                break;
            }
            case ';': // symbol link
            {
                const std::vector<uint8_t> &raw = searchSymbol(((MaElemSymlink*)items[i].first)->getValue());
                for(size_t i = 0; i < raw.size(); ++i)
                    tmp += (char)raw[i];
                break;
            }
            default: return ec;
        }

        // check which symbol it is
        if(tmp == "@parameters")
        {
            if(items[i].second->getType() != '[') return ec;
            ec.parameters = ((MaElemArray*)items[i].second)->getSub();
            if(!ec.parameters.empty()) ec.has_parameters = true;
        }
        else if(tmp == "@indent")
        {
            if(items[i].second->getType() != 'i') return ec;
            ec.indent = ((MaElemInteger*)items[i].second)->getValue();
            ec.has_indent = true;
        }
        else if(tmp == "@code")
        {
            if(items[i].second->getType() != 'i') return ec;
            ec.code = ((MaElemInteger*)items[i].second)->getValue();
            ec.has_code = true;
        }
    }
    ec.error = false;
    return ec;
}

bool Marshal::dumpEventCommand(std::ostream &f, const MaElemBase* e) const
{
    if(!e) return false;
    if(e->getType() != 'o') return false;
    // check the symbol
    {
        const MaElemBase* sym = ((const MaElemObject*)e)->getMain();
        if(!sym) return false;
        std::string tmp;
        switch(sym->getType())
        {
            case ':': // symbol
            {
                const std::vector<uint8_t> &raw = ((const MaElemSymbol*)sym)->getRaw();
                for(size_t i = 0; i < raw.size(); ++i)
                    tmp += (char)raw[i];
                break;
            }
            case ';': // symbol link
            {
                const std::vector<uint8_t> &raw = searchSymbol(((const MaElemSymlink*)sym)->getValue());
                for(size_t i = 0; i < raw.size(); ++i)
                    tmp += (char)raw[i];
                break;
            }
            default: return false;
        }

        if(tmp != "RPG::EventCommand") return false;
    }

    const std::vector<MaElemBase*> command_items = ((const MaElemObject*)e)->getSub();
    if(command_items.empty() || command_items[0]->getType() != -2) return false; // must be an Items type

    EventCommand ec = getEventCommand(command_items[0]);
    if(!ec.error)
    {
        f.write("\t\t", 2);
        for(int i = 0; i < ec.indent; ++i) f.write("\t", 1);
        std::string tmp;
        std::map<int, std::string>::const_iterator it = code_table.find(ec.code);
        if(it != code_table.end()) tmp = it->second;
        else tmp = "[" + dec2str(ec.code) + "]";
        f.write(tmp.c_str(), tmp.size());

        if(ec.parameters.empty())
        {
            f.write("\n", 1);
        }
        else
        {
            size_t t = (tmp.size() / 4);
            t = 7 - t;
            for(size_t i = 0; i < t; ++i) f.write("\t", 1);

            for(size_t i = 0; i < ec.parameters.size(); ++i)
            {
                writeContent(f, ec.parameters[i]);

                if(i < ec.parameters.size() - 1) f.write(", ", 2);
            }
            f.write("\n", 1);
        }
    }
    else return false;

    return true;
}

bool Marshal::writeContent(std::ostream &f, const MaElemBase* e) const
{
    if(!e) return false;

    switch(e->getType())
    {
        default: return false;
        case -2: // items list
        {
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemItems*)e)->getItems();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                writeContent(f, tab[i].first);
                f.write(", ", 2);
                writeContent(f, tab[i].second);
                if(i < tab.size()-1) f.write(", ", 2);
            }
            break;
        }
        case '0':
            f.write("Nil", 3);
            break;
        case 'T':
            f.write("True", 4);
            break;
        case 'F':
            f.write("False", 5);
            break;
        case '{':
        {
            f.write("hash(", 5);
            const std::vector<std::pair<MaElemBase*, MaElemBase*> > tab = ((MaElemHash*)e)->getHashs();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                f.write("{ ", 2);
                writeContent(f, tab[i].first);
                f.write(", ", 2);
                writeContent(f, tab[i].second);
                f.write(" }", 2);
                if(i < tab.size()-1) f.write(", ", 2);
            }
            f.write(")", 1);
            break;
        }
        case ':':
        {
            f.write("s(", 2);
            const std::vector<uint8_t> tab = ((MaElemSymbol*)e)->getRaw();
            for(size_t i = 0; i < tab.size(); ++i) f.write((char*)&tab[i], 1);
            f.write(")", 1);
            break;
        }
        case ';':
        {
            f.write("s(", 2);
            std::vector<uint8_t> tab = searchSymbol(((MaElemSymlink*)e)->getValue());
            for(size_t i = 0; i < tab.size(); ++i) f.write((char*)&tab[i], 1);
            f.write(")", 1);
            break;
        }
        case 'I':
        {
            f.write("ivar(", 5);
            writeContent(f, ((MaElemIVar*)e)->getMain());
            f.write(" ; ", 3);
            const std::vector<MaElemBase*> tab = ((MaElemIVar*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                writeContent(f, tab[i]);
                if(i < tab.size()-1) f.write(", ", 2);
            }
            f.write(")", 1);
            break;
        }
        case '"':
        {
            f.write("`", 1);
            const std::vector<uint8_t> tab = ((MaElemSymbol*)e)->getRaw();
            for(size_t i = 0; i < tab.size(); ++i)
                f.write((char*)&tab[i], 1);
            f.write("`", 1);
            break;
        }
        case 'C':
        {
            f.write("class(", 5);
            writeContent(f, ((MaElemUClass*)e)->getMain());
            f.write(" ; ", 3);
            const std::vector<MaElemBase*> tab = ((MaElemIVar*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                writeContent(f, tab[i]);
                if(i < tab.size()-1) f.write(", ", 2);
            }
            f.write(")", 1);
            break;
        }
        case 'o':
        {
            f.write("obj(", 4);
            writeContent(f, ((MaElemUClass*)e)->getMain());
            f.write(" ; ", 3);
            const std::vector<MaElemBase*> tab = ((MaElemIVar*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                writeContent(f, tab[i]);
                if(i < tab.size()-1) f.write(", ", 2);
            }
            f.write(")", 1);
            break;
        }
        case 'i':
        {
            std::string s = dec2str(((MaElemInteger*)e)->getValue());
            f.write(s.c_str(), s.size());
            break;
        }
        case '[':
        {
            f.write("{", 1);
            const std::vector<MaElemBase*> tab = ((MaElemArray*)e)->getSub();
            for(size_t i = 0; i < tab.size(); ++i)
            {
                writeContent(f, tab[i]);
                if(i < tab.size()-1) f.write(", ", 2);
            }
            f.write("}", 1);
            break;
        }
        case '@':
        {
            f.write("objlink(", 8);
            //std::vector<uint8_t> tab = searchObject(((MaElemObjlink*)e)->getValue());
            //for(size_t i = 0; i < tab.size(); ++i) f.write((char*)&tab[i], 1);
            //f.write(", ", 2);
            std::string s = dec2str(((MaElemObjlink*)e)->getValue()) + ")";
            f.write(s.c_str(), s.size());
            break;
        }
        case 'u':
        {
            f.write("udef(", 5);
            writeContent(f, ((MaElemUserdef*)e)->getMain());
            const std::vector<uint8_t> tab = ((MaElemUserdef*)e)->getContent();

            f.write(", 0x", 4);

            for(size_t i = 0; i < tab.size(); ++i)
            {
                std::string s = dec2hex(tab[i]);
                f.write(s.c_str(), s.size());
                if(i < tab.size() - 1) f.write(".", 1);
            }
            f.write(")", 1);
            break;
        }
    }
    return true;
}

std::streampos Marshal::getLine(std::istream& f, std::string& str)
{
    str.clear();
    std::streampos r = f.tellg();
    char c = 0;
    while(true)
    {
        if(f.eof()) return r;
        f.read(&c, 1);
        if(f.fail()) return r;
        switch(c)
        {
            case '\r': break;
            case '\n': return r;
            default:
                str += c;
                if(f.eof()) return r;
                break;
        }
    }
}

MaElemBase* Marshal::getSymbol(const std::string& sym)
{
    if(sym.empty()) return NULL;

    std::map<std::string, int>::iterator it = symbol_table.find(sym);
    if(it != symbol_table.end())
    {
        MaElemSymlink* symbol = new MaElemSymlink();
        symbol->setValue(it->second);
        return symbol;
    }
    else
    {
        MaElemSymbol* symbol = new MaElemSymbol();
        for(size_t i = 0; i < sym.size(); ++i)
            symbol->add((uint8_t)sym[i]);
        sym_count++;
        symbol_table[sym] = sym_count;
        return symbol;
    }
}

size_t Marshal::removeTabulation(std::string& s)
{
    size_t i = 0;
    if(s.empty()) return 0;
    do
    {
        if(s[0] == '\t')
        {
            s.erase(s.begin());
            i++;
        }
    }while(s[0] == '\t');
    return i;
}

size_t Marshal::removeSpace(std::string& s)
{
    size_t i = 0;
    if(s.empty()) return 0;
    do
    {
        if(s[0] == '\t' || s[0] == ' ')
        {
            s.erase(s.begin());
            i++;
        }
    }while(s[0] == '\t' || s[0] == ' ');
    return i;
}

MaElemObject* Marshal::parseEvent(std::istream& f)
{
    std::string line;
    std::streampos linepos = getLine(f, line);
    line_count++;
    if(f.eof()) return NULL;
    if(f.fail())
    {
        std::cout << "line " << line_count << " : Reading error (badbit or failbit is set)" << std::endl;
        std::cout << "Debug: good()=" << f.good();
        std::cout << " eof()=" << f.eof();
        std::cout << " fail()=" << f.fail();
        std::cout << " bad()=" << f.bad() << std::endl;
        err_flag = true;
        return NULL;
    }

    if(line.empty()) return NULL; // empty, we skip

    std::vector<std::string> strs;
    for(size_t i = 0; i < line.size() && strs.empty(); ++i)
    {
        if(line[i] == '=')
        {
            strs.push_back(line.substr(0, i));
            strs.push_back(line.substr(i+1, line.size()));
        }
    }

    if(strs.size() < 2 || !isNumber(strs[0]))
    {
        std::cout << "line " << line_count << " : Error, expecting ID=\"Event Name\"" << std::endl;
        err_flag = true;
        return NULL;
    }
    if(strs[1].size() < 2 || strs[1][0] != '"' || strs[1][strs[1].size()-1] != '"')
    {
        std::cout << "line " << line_count << " : Error, Event name isn't between quotation marks" << std::endl;
        err_flag = true;
        return NULL;
    }
    strs[1].erase(strs[1].begin());
    strs[1].erase(strs[1].begin()+strs[1].size()-1);

    MaElemObject* event = new MaElemObject();
    event->setMain(getSymbol("RPG::CommonEvent"));
    MaElemItems* items = new MaElemItems();
    event->addSub(items);
    std::pair<MaElemBase*, MaElemBase*> p;

    // @name
    p.first = getSymbol("@name");
    p.second = ((MaElemBase*)new MaElemString());
    for(size_t i = 0; i < strs[1].size(); ++i)
        ((MaElemString*)p.second)->add((uint8_t)strs[1][i]);
    items->add(p);

    // @id
    p.first = getSymbol("@id");
    p.second = ((MaElemBase*)new MaElemInteger());
    ((MaElemInteger*)p.second)->setValue(str2int(strs[0]));
    items->add(p);

    // build object
    bool trigger = false;
    bool switch_id = false;
    bool commands = false;
    while(true)
    {
        linepos = getLine(f, line);
        line_count++;
        // error check
        if(!f.good()) return NULL;
        if(line.empty())
        {
            std::cout << "line " << line_count << " : Warning, empty line" << std::endl;
            continue;
        }
        removeTabulation(line);
        if(!trigger && line.find("trigger") == 0)
        {
            size_t i = line.find("=");
            for(; i+1 < line.size() && line[i+1] == ' '; ++i);
            for(; i < line.size() && line[i] < '0' && line[i] > '9'; ++i);
            if(i == line.size())
            {
                std::cout << "line " << line_count << " : Can't parse trigger" << std::endl;
                err_flag = true;
                delete event;
                return NULL;
            }
            p.first = getSymbol("@trigger");
            p.second = ((MaElemBase*)new MaElemInteger());
            ((MaElemInteger*)p.second)->setValue(str2int(line.substr(i+1, line.size())));
            items->add(p);
            trigger = true;
        }
        else if(!switch_id && line.find("switch_id") == 0)
        {
            size_t i = line.find("=");
            for(; i+1 < line.size() && line[i+1] == ' '; ++i);
            for(; i < line.size() && line[i] < '0' && line[i] > '9'; ++i);
            if(i == line.size())
            {
                std::cout << "line " << line_count << " : Can't parse switch_id" << std::endl;
                err_flag = true;
                delete event;
                return NULL;
            }
            p.first = getSymbol("@switch_id");
            p.second = ((MaElemBase*)new MaElemInteger());
            ((MaElemInteger*)p.second)->setValue(str2int(line.substr(i+1, line.size())));
            items->add(p);
            switch_id = true;
        }
        else if(!commands && line == "command_list")
        {
            p.first = getSymbol("@list");
            p.second = ((MaElemBase*)new MaElemArray());
            while(parseCommand(f, (MaElemArray*)p.second));
            if(err_flag)
            {
                delete event;
                return NULL;
            }
            if(p.second->getSize() == 0)
            {
                std::cout << "line " << line_count << " : Warning, no commands in object. Added an empty one" << std::endl;
                addEmptyCommand((MaElemArray*)p.second);
            }
            items->add(p);
            commands = true;
        }
        else
        {
            if(!trigger) std::cout << "line " << line_count << " : Warning, End of object, trigger is missing ?" << std::endl;
            if(!switch_id) std::cout << "line " << line_count << " : Warning, End of object, switch_id is missing ?" << std::endl;
            if(!commands) std::cout << "line " << line_count << " : Warning, End of object, command_list is missing ?" << std::endl;
            f.seekg(linepos); // go back to beginning of the line
            line_count--;
            return event;
        }
        if(trigger && switch_id && commands)
            return event;
    }
}

bool Marshal::parseCommand(std::istream& f, MaElemArray* list)
{
    std::string line;
    std::streampos linepos = getLine(f, line);
    line_count++;

    // error check
    if(f.eof()) return false;
    if(f.fail())
    {
        std::cout << "line " << line_count << " : Reading error (badbit or failbit is set)" << std::endl;
        std::cout << "Debug: good()=" << f.good();
        std::cout << " eof()=" << f.eof();
        std::cout << " fail()=" << f.fail();
        std::cout << " bad()=" << f.bad() << std::endl;
        err_flag = true;
        return false;
    }
    int indent = removeTabulation(line) - 2;
    if(line.empty())
    {
        std::cout << "line " << line_count << " : Warning, empty line" << std::endl;
        return true;
    }

    // find command
    size_t pos;
    for(pos = 0; pos < line.size() && line[pos] != ']'; ++pos);
    if(line[pos] != ']' || line[0] != '[')
    {
        f.seekg(linepos);
        line_count--;
        return false;
    }


    // do stuff
    std::map<std::string, int>::iterator it = inv_code_table.find(line.substr(0, pos+1));
    if(it == inv_code_table.end())
    {
        std::cout << "line " << line_count << " : unknown command: " << line << std::endl;
        err_flag = true;
        return false;
    }

    if(indent < 0)
    {
        std::cout << "line " << line_count << " : Error, indentation is missing ?" << std::endl;
        err_flag = true;
        return false;
    }

    MaElemObject* command = new MaElemObject();
    command->setMain(getSymbol("RPG::EventCommand"));
    MaElemItems* items = new MaElemItems();
    command->addSub(items);
    std::pair<MaElemBase*, MaElemBase*> p;

    // @parameters
    p.first = getSymbol("@parameters");
    p.second = ((MaElemBase*)new MaElemArray());
    if(pos + 2 < line.size())
    {
        line = line.substr(pos+2, line.size());
        removeTabulation(line);
        parseParameter(line, (MaElemArray*)p.second);
        if(err_flag)
        {
            items->add(p);
            delete command;
            return false;
        }
    }
    items->add(p);

    // @indent
    p.first = getSymbol("@indent");
    p.second = ((MaElemBase*)new MaElemInteger());
    ((MaElemInteger*)p.second)->setValue(indent);
    items->add(p);

    // @code
    p.first = getSymbol("@code");
    p.second = ((MaElemBase*)new MaElemInteger());
    ((MaElemInteger*)p.second)->setValue(it->second);
    items->add(p);

    list->addSub(command);

    return true;
}

void Marshal::parseParameter(const std::string& params, MaElemVar* list)
{
    std::vector<char> delim;
    delim.push_back(',');
    std::vector<std::string> tokens = split(params, delim);

    for(size_t i = 0; i < tokens.size(); ++i)
    {
        removeSpace(tokens[i]);
        if(!tokens[i].empty())
        {
            switch(tokens[i][0])
            {
                case '`':
                {
                    size_t j = i;
                    size_t k = 1;
                    bool found = false;
                    for(; j < tokens.size() && !found; ++j)
                    {
                        for(; k < tokens[j].size() && !found; ++k)
                        {
                            if(tokens[j][k] == '`') found = true;
                        }

                        if(!found) k = 0;
                    }
                    j--;
                    k--;
                    if(!found)
                    {
                        std::cout << "line " << line_count << " : Error, end of string '`' not found" << std::endl;
                        err_flag = true;
                        return;
                    }
                    else
                    {
                        MaElemString* str = new MaElemString();
                        size_t y = 1;
                        for(size_t x = i; x <= j; ++x)
                        {
                            for(;  (x == j && y < k) || (x != j && y < tokens[x].size()); ++y)
                            {
                                str->add((uint8_t)tokens[x][y]);
                            }
                            y = 0;
                            if(x < j) str->add((uint8_t)',');
                        }
                        list->addSub((MaElemBase*)str);
                        i = j;
                    }
                    break;
                }
                case 'o':
                {
                    if(tokens[i].find("objlink") == 0)
                    {
                        size_t first = tokens[i].find("(");
                        if(first == std::string::npos)
                        {
                            std::cout << "line " << line_count << " : Error, objlink syntax error: " << tokens[i] << std::endl;
                            err_flag = true;
                            return;
                        }
                        size_t last = tokens[i].find(")", first);
                        if(last == std::string::npos)
                        {
                            std::cout << "line " << line_count << " : Error, objlink syntax error: " << tokens[i] << std::endl;
                            err_flag = true;
                            return;
                        }
                        std::string s = tokens[i].substr(first+1, last-first-1);
                        if(isNumber(s))
                        {
                            MaElemObjlink* num = new MaElemObjlink();
                            num->setValue(str2int(s));
                            list->addSub((MaElemBase*)num);
                        }
                        else
                        {
                            std::cout << "line " << line_count << " : Error, objlink syntax error, expected a numeric value: " << tokens[i] << std::endl;
                            err_flag = true;
                            return;
                        }
                    }
                    else if(tokens[i].find("obj") == 0)
                    {
                        int perr;
                        std::string s = getBracketContent(tokens, i, perr, '(', ')');
                        switch(perr)
                        {
                            case 1: std::cout << "line " << line_count << " : Error, object syntax error" << std::endl; err_flag = true; return;
                            case 2: std::cout << "line " << line_count << " : Error, object syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                        }
                        MaElemArray tmp;
                        size_t pos = s.find(";");
                        if(pos == std::string::npos)
                        {
                             std::cout << "line " << line_count << " : Error, object content syntax error: " << std::endl;
                             err_flag = true;
                             return;
                        }
                        parseParameter(s.substr(0, pos), &tmp);
                        if(err_flag) return;
                        if(tmp.getSize() == 0)
                        {
                            err_flag = true;
                            return;
                        }

                        // check if element is symbol
                        MaElemObject* obj = new MaElemObject();
                        const std::vector<MaElemBase*> sub = tmp.getSub();
                        switch(sub[0]->getType())
                        {
                            case ';':
                            {
                                MaElemSymlink *symbol = new MaElemSymlink();
                                symbol->setValue(((MaElemSymlink*)sub[0])->getValue());
                                obj->setMain(symbol);
                                break;
                            }
                            case ':':
                            {
                                MaElemSymbol *symbol = new MaElemSymbol();
                                const std::vector<uint8_t>&raw = ((MaElemSymbol*)sub[0])->getRaw();
                                for(size_t x = 0; x < raw.size(); ++x)
                                    symbol->add(raw[x]);
                                obj->setMain(symbol);
                                break;
                            }
                        }
                        parseParameter(s.substr(pos+1, s.size()), obj);
                        obj->addSub((MaElemBase*)(obj->convertSubToItems()));
                        list->addSub((MaElemBase*)obj);
                        if(err_flag) return;
                    }
                    else
                    {
                        std::cout << "line " << line_count << " : Error, syntax error, unknown parameter: " << tokens[i] << std::endl;
                        err_flag = true;
                        return;
                    }
                    break;
                }
                case 'u':
                {
                    if(tokens[i].find("udef") == 0)
                    {
                        int perr;
                        std::string s = getBracketContent(tokens, i, perr, '(', ')');
                        switch(perr)
                        {
                            case 1: std::cout << "line " << line_count << " : Error, userdef syntax error" << std::endl; err_flag = true; return;
                            case 2: std::cout << "line " << line_count << " : Error, userdef syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                        }
                        std::vector<std::string> udef_content = split(s, delim);
                        if(udef_content.size() < 2)
                        {
                            std::cout << "line " << line_count << " : Error, invalid userdef content" << std::endl;
                            err_flag = true;
                            return;
                        }

                        MaElemArray tmp;
                        parseParameter(udef_content[0], &tmp);
                        if(err_flag) return;
                        if(tmp.getSize() == 0)
                        {
                            err_flag = true;
                            return;
                        }

                        // check if element is symbol
                        MaElemUserdef* udef = new MaElemUserdef();
                        const std::vector<MaElemBase*> sub = tmp.getSub();
                        switch(sub[0]->getType())
                        {
                            case ';':
                            {
                                MaElemSymlink *symbol = new MaElemSymlink();
                                symbol->setValue(((MaElemSymlink*)sub[0])->getValue());
                                udef->setMain(symbol);
                                break;
                            }
                            case ':':
                            {
                                MaElemSymbol *symbol = new MaElemSymbol();
                                const std::vector<uint8_t>&raw = ((MaElemSymbol*)sub[0])->getRaw();
                                for(size_t x = 0; x < raw.size(); ++x)
                                    symbol->add(raw[x]);
                                udef->setMain(symbol);
                                break;
                            }
                        }
                        size_t pos = udef_content[1].find("0x");
                        if(pos == std::string::npos)
                        {
                            std::cout << "line " << line_count << " : Error, invalid userdef content" << std::endl;
                            err_flag = true;
                            delete udef;
                            return;
                        }
                        else
                        {
                            s.clear();
                            for(size_t j = pos + 2; j < udef_content[1].size(); ++j)
                                if(udef_content[1][j] != ' ') s += udef_content[1][j];
                            std::vector<char> Udelim;
                            Udelim.push_back('.');
                            udef_content = split(s, Udelim);
                            for(size_t j = 0; j < udef_content.size(); ++j)
                                udef->addChar((uint8_t)strtol(udef_content[j].c_str(), NULL, 16));
                            list->addSub((MaElemBase*)udef);
                        }
                    }
                    else
                    {
                        std::cout << "line " << line_count << " : Error, syntax error, unknown parameter: " << tokens[i] << std::endl;
                        err_flag = true;
                        return;
                    }
                    break;
                }
                case 's':
                {
                    int perr;
                    std::string s = getBracketContent(tokens, i, perr, '(', ')');
                    switch(perr)
                    {
                        case 1: std::cout << "line " << line_count << " : Error, symbol syntax error" << std::endl; err_flag = true; return;
                        case 2: std::cout << "line " << line_count << " : Error, symbol syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                    }
                    list->addSub(getSymbol(s));
                    break;
                }
                case 'h':
                {
                    if(tokens[i].find("hash") == 0)
                    {
                        int perr;
                        std::string s = getBracketContent(tokens, i, perr, '(', ')');
                        switch(perr)
                        {
                            case 1: std::cout << "line " << line_count << " : Error, hash syntax error" << std::endl; err_flag = true; return;
                            case 2: std::cout << "line " << line_count << " : Error, hash syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                        }
                        //DO SOMETHING
                        std::cout << "line " << line_count << " : Hash isn't implemented" << std::endl;
                        err_flag = true;
                        return;
                    }
                    else
                    {
                        std::cout << "line " << line_count << " : Error, syntax error, unknown parameter: " << tokens[i] << std::endl;
                        err_flag = true;
                        return;
                    }
                    break;
                }
                case '{':
                {
                    int perr;
                    std::string s = getBracketContent(tokens, i, perr, '{', '}');
                    switch(perr)
                    {
                        case 1: std::cout << "line " << line_count << " : Error, array syntax error" << std::endl; err_flag = true; return;
                        case 2: std::cout << "line " << line_count << " : Error, array syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                    }
                    MaElemArray* _array = new MaElemArray();
                    parseParameter(s, _array);
                    list->addSub(_array);
                    if(err_flag) return;
                    break;
                }
                case 'i':
                {
                    if(tokens[i].find("ivar") == 0)
                    {
                        int perr;
                        std::string s = getBracketContent(tokens, i, perr, '(', ')');
                        switch(perr)
                        {
                            case 1: std::cout << "line " << line_count << " : Error, ivar syntax error" << std::endl; err_flag = true; return;
                            case 2: std::cout << "line " << line_count << " : Error, ivar syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                        }
                        //DO SOMETHING
                        std::cout << "line " << line_count << " : Ivar isn't implemented" << std::endl;
                        err_flag = true;
                        return;
                    }
                    else
                    {
                        std::cout << "line " << line_count << " : Error, syntax error, unknown parameter: " << tokens[i] << std::endl;
                        err_flag = true;
                        return;
                    }
                    break;
                }
                case 'c':
                {
                    if(tokens[i].find("class") == 0)
                    {
                        int perr;
                        std::string s = getBracketContent(tokens, i, perr, '(', ')');
                        switch(perr)
                        {
                            case 1: std::cout << "line " << line_count << " : Error, class syntax error" << std::endl; err_flag = true; return;
                            case 2: std::cout << "line " << line_count << " : Error, class syntax error, missing bracket ?" << std::endl; err_flag = true; return;
                        }
                        //DO SOMETHING
                        std::cout << "line " << line_count << " : Class isn't implemented" << std::endl;
                        err_flag = true;
                        return;
                    }
                    else
                    {
                        std::cout << "line " << line_count << " : Error, syntax error, unknown parameter: " << tokens[i] << std::endl;
                        err_flag = true;
                        return;
                    }
                    break;
                }
                case 'T':
                    list->addSub(new MaElemTrue());
                    break;
                case 'N':
                    list->addSub(new MaElemNil());
                    break;
                case 'F':
                    list->addSub(new MaElemFalse());
                    break;
                default:
                {
                    do
                    {
                        if(tokens[i][tokens[i].size()-1] == ' ')
                            tokens[i].erase(tokens[i].begin()+tokens[i].size()-1);
                    }while(!tokens[i].empty() && tokens[i][tokens[i].size()-1] == ' ');

                    if(isNumber(tokens[i]))
                    {
                        MaElemInteger* num = new MaElemInteger();
                        num->setValue(str2int(tokens[i]));
                        list->addSub((MaElemBase*)num);
                    }
                    else
                    {
                        std::cout << "line " << line_count << " : Error, syntax error, unknown parameter: " << tokens[i] << std::endl;
                        err_flag = true;
                        return;
                    }
                    break;
                }
            }
        }
    }
}

void Marshal::addEmptyCommand(MaElemArray* list)
{
    if(list == NULL) return;
    MaElemObject* command = new MaElemObject();
    command->setMain(getSymbol("RPG::EventCommand"));
    MaElemItems* items = new MaElemItems();
    command->addSub(items);
    std::pair<MaElemBase*, MaElemBase*> p;

    // @parameters
    p.first = getSymbol("@parameters");
    p.second = ((MaElemBase*)new MaElemArray());
    items->add(p);

    // @indent
    p.first = getSymbol("@indent");
    p.second = ((MaElemBase*)new MaElemInteger());
    ((MaElemInteger*)p.second)->setValue(0);
    items->add(p);

    // @code
    p.first = getSymbol("@code");
    p.second = ((MaElemBase*)new MaElemInteger());
    ((MaElemInteger*)p.second)->setValue(0);
    items->add(p);

    list->addSub(command);
}

std::string Marshal::getBracketContent(std::vector<std::string> &tokens, size_t &start, int &perr, char cBeg, char cEnd)
{
    perr = 0;
    size_t first = tokens[start].find(cBeg);
    if(first == std::string::npos)
    {
        perr = 1;
        return std::string("");
    }
    std::string s;
    int count = 1;
    size_t j = start;
    size_t k = first+1;
    for(;j < tokens.size() && count > 0; ++j)
    {
        for(; k < tokens[j].size() && count > 0; ++k)
        {
            if(tokens[j][k] == cBeg) count++;
            else if(tokens[j][k] == cEnd) count--;
        }
        if(count > 0) k = 0;
    }
    j--;
    k--;
    if(count > 0)
    {
        perr = 2;
        return std::string("");
    }
    if(j == start) s = tokens[start].substr(first+1, k-first-1);
    else
    {
        s = tokens[start].substr(first+1, tokens[start].size());
        for(size_t x = start + 1; x < j; x++)
        {
            s += ",";
            s += tokens[x];
        }
        s += ",";
        s += tokens[j].substr(0, k);
    }
    start = j;
    return s;
}
