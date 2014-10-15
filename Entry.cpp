#include "Entry.h"
#include <time.h>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

std::mt19937_64 mtrand = std::mt19937_64(std::chrono::high_resolution_clock::now().time_since_epoch().count());

Entry::Entry()
{
    m_username = "";
    m_password = "";
    m_url = "";
    m_notes = "";
    m_created = getCurrentLocalTime();
    m_modified = m_created;
    m_id = mtrand();
    m_parent = 0;

    m_dateFormatString = "%Y-%M-%D %H:%I:%S";
}

Entry::~Entry()
{

}
