/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "searchUtils.hpp"
#include "sc-memory/cpp/sc_stream.hpp"

#include "ASearchElementsConnectedWithRelation.hpp"

#include <iostream>
#include <sstream>
#include <thread>

namespace search_extended
{

ScAddr ASearchElementsConnectedWithRelation::msAgentKeynode;
ScAddr ASearchElementsConnectedWithRelation::agent_question;

SC_AGENT_IMPLEMENTATION(ASearchElementsConnectedWithRelation)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr quest = ms_context->GetArcEnd(edgeAddr);

    ScAddr elem, relation;

    ScIterator5Ptr iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
    if (iter->Next())
        elem = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
    if (iter->Next())
        relation = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr answer = ms_context->CreateNode(ScType::NodeConst);
    ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::system_element, answer);

    sc_bool sys_off = SC_TRUE;
    if (IS_SYSTEM_ELEMENT(((ScMemoryContext&)ms_context), elem))
        sys_off = SC_FALSE;


    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, elem, sys_off, SC_FALSE);

    if  (ms_context->HelperCheckArc(Keynodes::quasybinary_relation, relation, ScType::EdgeAccessConstPosPerm) || ms_context->HelperCheckArc(Keynodes::decomposition_relation, relation, ScType::EdgeAccessConstPosPerm))
    {
        ScIterator5Ptr iter5 = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter5->Next())
        {
            if (Utils::isSubset((ScMemoryContext&)ms_context, relation, iter5->Get(4)))
            {
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(0), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(3), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(4), sys_off, SC_FALSE);

                ScIterator3Ptr iter3 = ms_context->Iterator3(iter5->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
                while (iter3->Next())
                {
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter3->Get(1), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter3->Get(2), sys_off, SC_FALSE);
                }
            }
        }

        ScIterator3Ptr iter3_1 = ms_context->Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, elem);
        while (iter3_1->Next())
        {
            ScIterator5Ptr iter5_1 = ms_context->Iterator5(iter3_1->Get(0), ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
            while (iter5_1->Next())
            {
                if (Utils::isSubset((ScMemoryContext&)ms_context, relation, iter5_1->Get(4)))
                {
                    std::cout << "STEP4" << std::endl;
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5_1->Get(1), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5_1->Get(2), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5_1->Get(3), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5_1->Get(4), sys_off, SC_FALSE);

                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter3_1->Get(0), sys_off, SC_FALSE);
                    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter3_1->Get(1), sys_off, SC_FALSE);
                }
            }
        }

    }
    else
    {
        ScIterator5Ptr iter5 = ms_context->Iterator5(elem, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter5->Next())
        {
            if (Utils::isSubset((ScMemoryContext&)ms_context, relation, iter5->Get(4)))
            {
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(2), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(3), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(4), sys_off, SC_FALSE);
            }
        }

        iter5 = ms_context->Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter5->Next())
        {
            if (Utils::isSubset((ScMemoryContext&)ms_context, relation, iter5->Get(4)))
            {
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(0), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(3), sys_off, SC_FALSE);
                Utils::addToAnswer((ScMemoryContext&)ms_context, answer, iter5->Get(4), sys_off, SC_FALSE);
            }
        }
    }

    Utils::finishSearchAction((ScMemoryContext&)ms_context, quest, answer);

    return SC_RESULT_OK;
}

} // namespace
