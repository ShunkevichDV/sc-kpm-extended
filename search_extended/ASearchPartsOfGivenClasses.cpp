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

ScAddr ASearchPartsOfGivenClasses::msAgentKeynode;
ScAddr ASearchPartsOfGivenClasses::agent_question;

SC_AGENT_IMPLEMENTATION(ASearchPartsOfGivenClasses)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr quest = ms_context->GetArcEnd(edgeAddr);

    if (!ms_context->HelperCheckArc(agent_question, quest, ScType::EdgeAccessConstPosPerm))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr elem_set, class_set;

    ScIterator5Ptr iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
    if (iter->Next())
        elem_set = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
    if (iter->Next())
        class_set = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    return SC_RESULT_OK;
}

} // namespace
