#include "Entry.h"
#include <time.h>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

std::mt19937_64 mtrand = std::mt19937_64(std::chrono::high_resolution_clock::now().time_since_epoch().count());

Entry::Entry(uint64_t parentId, uint64_t id, uint64_t created)
{
    m_username = "";
    m_password = "";
    m_url = "";
    m_notes = "";
    m_created = (created == 0 ? DateTime(getCurrentLocalTime()) : created);
    m_modified = m_created;
    m_id = (id == 0 ? mtrand() : id);
    m_parent = parentId;

    m_dateFormatString = "%Y-%M-%D %H:%I:%S";
}

Entry::~Entry()
{

}
