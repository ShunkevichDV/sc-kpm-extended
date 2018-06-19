/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "ASearchPartsOfGivenClasses.hpp"
#include "sc-memory/cpp/sc_memory.hpp"
#include "sc-memory/cpp/sc_stream.hpp"

#include <iostream>
#include <sstream>
#include <thread>

namespace search_extended
{

ScAddr ASearchPartsOfGivenClasses::question_initiated;
ScAddr ASearchPartsOfGivenClasses::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASearchPartsOfGivenClasses)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    return SC_RESULT_OK;
}

} // namespace
