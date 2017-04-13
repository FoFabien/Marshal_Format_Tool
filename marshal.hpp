#ifndef MARSHAL_HPP
#define MARSHAL_HPP

#include <string>
#include <map>
#include "marshal_element.hpp"

struct CommonEvent
{
    std::vector<uint8_t> name;
    bool has_name;
    std::vector<MaElemBase*> list;
    bool has_list;
    int trigger;
    bool has_trigger;
    int switch_id;
    bool has_switch_id;
    int id;
    bool has_id;
    bool error;
};

struct EventCommand
{
    std::vector<MaElemBase*> parameters;
    bool has_parameters;
    int indent;
    bool has_indent;
    int code;
    bool has_code;
    bool error;
};

class Marshal
{
    public:
        Marshal();
        ~Marshal();
        void clear();
        int read(const std::string& file);
        int write(const std::string& file);
        int getElemCount() const;
        void print() const;
        bool dump(const std::string& file) const;
        bool dumpCommonEvents(const std::string& file) const;
        bool compile(const std::string& file);
    protected:
        // functions
        int readFile(std::istream &f);
        MaElemBase* readRubyValue(std::istream &f);
        MaElemBase* readRubyHash(std::istream &f);
        MaElemBase* readRubySymbol(std::istream &f);
        MaElemBase* readRubyString(std::istream &f);
        MaElemBase* readRubyObject(std::istream &f);
        MaElemBase* readRubyIvar(std::istream &f);
        MaElemBase* readRubyUclass(std::istream &f);
        MaElemBase* readRubySymlink(std::istream &f);
        MaElemBase* readRubyItems(std::istream &f);
        MaElemBase* readRubyInteger(std::istream &f);
        MaElemBase* readRubyArray(std::istream &f);
        MaElemBase* readRubyObjlink(std::istream &f);
        MaElemBase* readRubyUserdef(std::istream &f);
        int readRubyLong(std::istream &f);

        int writeFile(std::ostream &f);
        bool writeRubyValue(std::ostream &f, const MaElemBase* e);
        bool writeRubyHash(std::ostream &f, const MaElemBase* e);
        bool writeRubySymbol(std::ostream &f, const MaElemBase* e);
        bool writeRubyString(std::ostream &f, const MaElemBase* e);
        bool writeRubyObject(std::ostream &f, const MaElemBase* e);
        bool writeRubyIvar(std::ostream &f, const MaElemBase* e);
        bool writeRubyUclass(std::ostream &f, const MaElemBase* e);
        bool writeRubySymlink(std::ostream &f, const MaElemBase* e);
        bool writeRubyItems(std::ostream &f, const MaElemBase* e);
        bool writeRubyInteger(std::ostream &f, const MaElemBase* e);
        bool writeRubyArray(std::ostream &f, const MaElemBase* e);
        bool writeRubyObjlink(std::ostream &f, const MaElemBase* e);
        bool writeRubyUserdef(std::ostream &f, const MaElemBase* e);
        bool writeRubyLong(std::ostream &f, int l);

        void print_elem(const MaElemBase* e, int space = 0) const;
        bool dump_elem(std::ostream &f, const MaElemBase* e, int space = 0) const;

        const std::vector<uint8_t> searchSymbol(int id) const;
        const std::vector<uint8_t> searchObject(int id) const;
        const MaElemSymbol* searchSymbolPtr(int id) const;
        const MaElemObject* searchObjectPtr(int id) const;
        const MaElemBase* search_elem(const MaElemBase* e, int &id_target, int &id_current, char type) const;

        CommonEvent getCommonEvent(const MaElemBase* e) const;
        EventCommand getEventCommand(const MaElemBase* e) const;
        bool dumpEventCommand(std::ostream &f, const MaElemBase* e) const;
        bool writeContent(std::ostream &f, const MaElemBase* e) const;

        std::streampos getLine(std::istream& f, std::string& str);
        MaElemBase* getSymbol(const std::string& sym);
        size_t removeTabulation(std::string& s);
        size_t removeSpace(std::string& s);

        // parser
        MaElemObject* parseEvent(std::istream& f);
        bool parseCommand(std::istream& f, MaElemArray* list);
        void parseParameter(const std::string& params, MaElemVar* list);
        void addEmptyCommand(MaElemArray* list);
        std::string getBracketContent(std::vector<std::string> &tokens, size_t &start, int &perr, char cBeg, char cEnd);

        // attributes
        std::vector<MaElemBase*> elems;
        // common events dump
        std::map<int, std::string> code_table;
        std::map<std::string, int> inv_code_table;
        // compilation
        int line_count;
        int sym_count;
        std::map<std::string, int> symbol_table;
        bool err_flag;
};

#endif // MARSHAL_HPP
