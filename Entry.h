#ifndef ENTRY_H
#define ENTRY_H
#include <string>
#include <random>
#include <chrono>
#include "DateTime.h"
#include "PasswordPolicy.h"

/*TODO:
 * Find a place to store preference for date formating string
 *     other than here more than likely. Each entry doesn't need
 *     its own copy
 */

using namespace DateAndTime;

class Entry
{
private:
    uint64_t m_id;
    uint64_t m_parent;
    std::string m_title;
    std::string m_username;
    std::string m_password;
    std::string m_url;
    std::string m_notes;
    DateTime m_created;
    DateTime m_modified;
    std::string m_dateFormatString;
    PasswordPolicy m_passwordPolicy;
    static std::random_device rd;
    static std::mt19937_64 mtrand_64;
public:
    void setId(uint64_t id) {m_id = id;}
    void setParent(uint64_t parent) {m_parent = parent;}
    void setTitle(std::string title) {m_title = title;}
    void setUsername(std::string username) {m_username = username;}
    void setPassword(std::string password) {m_password = password;}
    void setUrl(std::string url) {m_url = url;}
    void setNotes(std::string notes) {m_notes = notes;}
    void setCreated(DateTime time = getCurrentLocalTime()) {m_created = time;}
    void setModified(DateTime time = getCurrentLocalTime()) {m_modified = time;}
    //void setDateFormatString(std::string format) {m_dateFormatString = format;}
    void setPasswordPolicy(PasswordPolicy policy) {m_passwordPolicy = policy;}
    uint64_t getId() const {return m_id;}
    uint64_t getParent() {return m_parent;}
    std::string getTitle() {return m_title;}
    std::string getUsername() {return m_username;}
    std::string getPassword() {return m_password;}
    std::string getUrl() {return m_url;}
    std::string getNotes() {return m_notes;}
    std::string getCreated() {return m_created.format(m_dateFormatString);}
    std::string getModified() {return m_modified.format(m_dateFormatString);}
    //std::string getDateFormatString() //... shouldn't ever be needed, eh?
    PasswordPolicy getPasswordPolicy() {return m_passwordPolicy;}
    uint64_t getCreatedUInt64() {return m_created.asUInt64();}
    uint64_t getModifiedUInt64() {return m_modified.asUInt64();}
    std::string generatePassword() {return m_passwordPolicy.generate();}
    std::string print();
    Entry();
    ~Entry();

    inline bool operator==(const Entry& that) const
    {
        return this->getId() == that.getId();
    }
    inline bool operator!=(const Entry& that) const
    {
        return !(*this == that);
    }
};

#endif // ENTRY_H
