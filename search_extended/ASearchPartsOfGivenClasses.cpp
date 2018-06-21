/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "searchUtils.hpp"
#include "sc-memory/cpp/sc_stream.hpp"

#include "ASearchPartsOfGivenClasses.hpp"

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

    ScAddr answer = ms_context->CreateNode(ScType::NodeConst);
    ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::system_element, answer);

    ScIterator3Ptr elem_iter = ms_context->Iterator3(elem_set, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (elem_iter->Next())
    {
        sc_bool sys_off = SC_TRUE;
        if (IS_SYSTEM_ELEMENT(((ScMemoryContext&)ms_context), elem_iter->Get(2)))
            sys_off = SC_FALSE;

        Utils::addToAnswer((ScMemoryContext&)ms_context, answer, elem_iter->Get(2), sys_off, SC_TRUE);
        searchPartsRec((ScMemoryContext&)ms_context, elem_iter->Get(2), class_set, answer, sys_off);
    }

    Utils::finishSearchAction((ScMemoryContext&)ms_context, quest, answer);

    return SC_RESULT_OK;
}


sc_bool ASearchPartsOfGivenClasses::searchPartsRec(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &class_set, ScAddr const &answer, sc_bool sys_off)
{
    sc_bool result = SC_FALSE;

    ScIterator5Ptr iter = ctx.Iterator5(elem, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter->Next())
    {
        if (!ctx.HelperCheckArc(Keynodes::taxonomy_relation, iter->Get(4), ScType::EdgeAccessConstPosPerm))
            continue;

        sc_bool curr_result = searchPartsRec(ctx, iter->Get(2), class_set, answer, sys_off);

        if (Utils::isElementOfUnion(ctx, iter->Get(2), class_set) || SC_TRUE == curr_result)
        {
            result = SC_TRUE;
            Utils::addToAnswer(ctx, answer, iter->Get(1), sys_off, SC_FALSE);
            Utils::addToAnswer(ctx, answer, iter->Get(2), sys_off, SC_FALSE);
            Utils::addToAnswer(ctx, answer, iter->Get(3), sys_off, SC_FALSE);
            Utils::addToAnswer(ctx, answer, iter->Get(4), sys_off, SC_FALSE);
        }
    }

    //Quasybinary case
    ScIterator5Ptr iter5 = ctx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, elem, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter5->Next())
    {
        if (!ctx.HelperCheckArc(Keynodes::quasybinary_relation, iter5->Get(4), ScType::EdgeAccessConstPosPerm))
            continue;
        if (!(ctx.HelperCheckArc(Keynodes::taxonomy_relation, iter5->Get(4), ScType::EdgeAccessConstPosPerm) || ctx.HelperCheckArc(Keynodes::decomposition_relation, iter->Get(4), ScType::EdgeAccessConstPosPerm)))
            continue;

        sc_bool flag = SC_FALSE;
        ScIterator3Ptr iter3 = ctx.Iterator3(iter5->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter3->Next())
        {
            sc_bool curr_result = searchPartsRec(ctx, iter3->Get(2), class_set, answer, sys_off);

            if (Utils::isElementOfUnion(ctx, iter3->Get(2), class_set) || SC_TRUE == curr_result)
            {
                result = SC_TRUE;
                flag = SC_TRUE;
                Utils::addToAnswer(ctx, answer, iter3->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter3->Get(2), sys_off, SC_FALSE);
            }
        }

        if (SC_FALSE == flag)
            continue;

        Utils::addToAnswer(ctx, answer, iter5->Get(0), sys_off, SC_FALSE);
        Utils::addToAnswer(ctx, answer, iter5->Get(1), sys_off, SC_FALSE);
        Utils::addToAnswer(ctx, answer, iter5->Get(3), sys_off, SC_FALSE);
        Utils::addToAnswer(ctx, answer, iter5->Get(4), sys_off, SC_FALSE);
    }

    return result;
}

} // namespace
