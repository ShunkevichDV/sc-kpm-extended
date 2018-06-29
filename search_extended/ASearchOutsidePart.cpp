/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "searchUtils.hpp"
#include "sc-memory/cpp/sc_stream.hpp"

#include "ASearchOutsidePart.hpp"

#include <iostream>
#include <sstream>
#include <thread>

namespace search_extended
{

ScAddr ASearchOutsidePart::msAgentKeynode;
ScAddr ASearchOutsidePart::agent_question;
ScAddr ASearchOutsidePart::nrel_external_to_internal_clamping;

SC_AGENT_IMPLEMENTATION(ASearchOutsidePart)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr quest = ms_context->GetArcEnd(edgeAddr);

    ScAddr elem;
    ScIterator3Ptr iter = ms_context->Iterator3(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    if (iter->Next())
        elem = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr answer = ms_context->CreateNode(ScType::NodeConst);
    ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::system_element, answer);

    sc_bool sys_off = SC_TRUE;
    if (IS_SYSTEM_ELEMENT(((ScMemoryContext&)ms_context), elem))
        sys_off = SC_FALSE;

    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, elem, sys_off, SC_FALSE);

    ScIterator5Ptr iter5 = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter5->Next())
    {
        if (ms_context->HelperCheckArc(Keynodes::taxonomy_relation, iter5->Get(4), ScType::EdgeAccessConstPosPerm) || ms_context->HelperCheckArc(Keynodes::decomposition_relation, iter5->Get(4), ScType::EdgeAccessConstPosPerm))
        {
            ScIterator3Ptr iter3 = ms_context->Iterator3(iter5->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
            while (iter3->Next())
            {
                sc_bool flag = SC_TRUE;
                ScIterator5Ptr iter_clamp = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, iter3->Get(2), ScType::EdgeAccessConstPosPerm, nrel_external_to_internal_clamping);
                while (iter_clamp->Next())
                {
                    if (ms_context->HelperCheckArc(iter5->Get(0), iter_clamp->Get(0), ScType::EdgeAccessConstPosPerm))
                    {
                        flag = SC_FALSE;
                        break;
                    }
                }
                if (SC_TRUE == flag)
                {
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(0), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(1), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(3), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(4), sys_off, SC_FALSE);

                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter3->Get(1), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter3->Get(2), sys_off, SC_FALSE);
                }
            }
        }
    }

    Utils::finishSearchAction((ScMemoryContext&)ms_context, quest, answer);

    return SC_RESULT_OK;
}

} // namespace
