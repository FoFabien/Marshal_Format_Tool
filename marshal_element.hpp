#ifndef MARSHAL_ELEMENT_HPP
#define MARSHAL_ELEMENT_HPP

#include <vector>
#include <utility>
#include <stdint.h>

class MaElemBase // base class
{
    public:
        virtual ~MaElemBase() {};
        char getType() const {return type;}
        virtual int getSize() const {return 0;}
    protected:
        MaElemBase():type(-1) {}
        char type;
};

class MaElemTrue: public MaElemBase
{
    public:
        MaElemTrue();
        virtual ~MaElemTrue() {};
        virtual int getSize() const {return 1;}
    protected:
};

class MaElemFalse: public MaElemBase
{
    public:
        MaElemFalse();
        virtual ~MaElemFalse() {};
        virtual int getSize() const {return 1;}
    protected:
};

class MaElemNil: public MaElemBase
{
    public:
        MaElemNil();
        virtual ~MaElemNil() {};
        virtual int getSize() const {return 1;}
    protected:
};

class MaElemHash: public MaElemBase
{
    public:
        MaElemHash();
        virtual ~MaElemHash();
        virtual int getSize() const;
        void add(std::pair<MaElemBase*, MaElemBase*> &p);
        const std::vector<std::pair<MaElemBase*, MaElemBase*> >& getHashs() const;
    protected:
        std::vector<std::pair<MaElemBase*, MaElemBase*> > hashs;
};

class MaElemItems: public MaElemBase
{
    public:
        MaElemItems();
        virtual ~MaElemItems();
        virtual int getSize() const;
        void add(std::pair<MaElemBase*, MaElemBase*> &p);
        const std::vector<std::pair<MaElemBase*, MaElemBase*> >& getItems() const;
    protected:
        std::vector<std::pair<MaElemBase*, MaElemBase*> > items;
};

class MaElemRaw: public MaElemBase // base class
{
    public:
        MaElemRaw();
        virtual ~MaElemRaw();
        virtual int getSize() const;
        void add(uint8_t c);
        void swapRaw(std::vector<uint8_t>& r); // perform a swap
        const std::vector<uint8_t>& getRaw() const;
    protected:
        std::vector<uint8_t> raw;
};

class MaElemSymbol: public MaElemRaw
{
    public:
        MaElemSymbol();
        virtual ~MaElemSymbol();
        virtual int getSize() const;
};

class MaElemString: public MaElemRaw
{
    public:
        MaElemString();
        virtual ~MaElemString();
        virtual int getSize() const;
};

class MaElemVar: public MaElemBase // base class
{
    public:
        MaElemVar();
        virtual ~MaElemVar();
        virtual int getSize() const;
        void setMain(MaElemBase* m);
        const MaElemBase* getMain() const;
        void addSub(MaElemBase* s);
        const std::vector<MaElemBase*>& getSub() const;
        virtual MaElemItems* convertSubToItems(); // sub will be empty
    protected:
        MaElemBase* main;
        std::vector<MaElemBase*> sub;
};

class MaElemObject: public MaElemVar
{
    public:
        MaElemObject();
        virtual ~MaElemObject();
        virtual int getSize() const;
};

class MaElemIVar: public MaElemVar
{
    public:
        MaElemIVar();
        virtual ~MaElemIVar();
        virtual int getSize() const;
};

class MaElemUClass: public MaElemVar
{
    public:
        MaElemUClass();
        virtual ~MaElemUClass();
        virtual int getSize() const;
};

class MaElemArray: public MaElemVar
{
    public:
        MaElemArray();
        virtual ~MaElemArray();
        virtual int getSize() const;
};

class MaElemNumber: public MaElemBase // base class
{
    public:
        MaElemNumber();
        virtual ~MaElemNumber();
        virtual int getSize() const;
        void setValue(int i);
        int getValue() const;
    protected:
        int n;
};

class MaElemSymlink: public MaElemNumber
{
    public:
        MaElemSymlink();
        virtual ~MaElemSymlink();
        virtual int getSize() const;
};

class MaElemObjlink: public MaElemNumber
{
    public:
        MaElemObjlink();
        virtual ~MaElemObjlink();
        virtual int getSize() const;
};

class MaElemInteger: public MaElemNumber
{
    public:
        MaElemInteger();
        virtual ~MaElemInteger();
        virtual int getSize() const;
};

class MaElemUserdef: public MaElemBase
{
    public:
        MaElemUserdef();
        virtual ~MaElemUserdef();
        virtual int getSize() const;
        void setMain(MaElemBase* m);
        const MaElemBase* getMain() const;
        void addChar(uint8_t c);
        const std::vector<uint8_t>& getContent() const;
    protected:
        MaElemBase* main;
        std::vector<uint8_t> content;
};

#endif // MARSHAL_ELEMENT_HPP
