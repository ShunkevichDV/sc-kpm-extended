/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "searchUtils.hpp"
#include "sc-memory/cpp/sc_stream.hpp"

#include "ASearchPartsByRelations.hpp"

#include <iostream>
#include <sstream>
#include <thread>

namespace search_extended
{

ScAddr ASearchPartsByRelations::msAgentKeynode;
ScAddr ASearchPartsByRelations::agent_question;
ScAddr ASearchPartsByRelations::agent_question_count;

SC_AGENT_IMPLEMENTATION(ASearchPartsByRelations)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr quest = ms_context->GetArcEnd(edgeAddr);

    sc_bool count_only = SC_FALSE;
    count = 0;

    if (!ms_context->HelperCheckArc(agent_question, quest, ScType::EdgeAccessConstPosPerm))
    {
        if (ms_context->HelperCheckArc(agent_question_count, quest, ScType::EdgeAccessConstPosPerm))
            count_only = SC_TRUE;
        else
            return SC_RESULT_ERROR_INVALID_PARAMS;
    }

    ScAddr elem_set, relation_set;

    ScIterator5Ptr iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
    if (iter->Next())
        elem_set = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
    if (iter->Next())
        relation_set = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr answer = ms_context->CreateNode(ScType::NodeConst);
    ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::system_element, answer);

    sc_bool sys_off = SC_TRUE;
    ScIterator3Ptr elem_iter = ms_context->Iterator3(elem_set, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (elem_iter->Next())
    {
        if (IS_SYSTEM_ELEMENT(((ScMemoryContext&)ms_context), elem_iter->Get(2)))
            sys_off = SC_FALSE;
        if (SC_FALSE == count_only)
        {
            Utils::addToAnswer((ScMemoryContext&)ms_context, answer, elem_iter->Get(2), sys_off, SC_TRUE);
        }
        searchByRelationsRec((ScMemoryContext&)ms_context, elem_iter->Get(2), relation_set, answer, sys_off, count_only);
    }

    if (count_only)
    {
        ScAddr count_link = ms_context->CreateLink();
        std::stringstream ss;
        ss << count;
        std::string str = ss.str();
        ScStream write_stream((sc_char*)str.c_str(), str.length(), SC_STREAM_FLAG_READ);
        ms_context->SetLinkContent(count_link, write_stream);
        Utils::addToAnswer((ScMemoryContext&)ms_context, answer, count_link, sys_off, SC_TRUE);
    }

    Utils::finishSearchAction((ScMemoryContext&)ms_context, quest, answer);

    return SC_RESULT_OK;
}


sc_bool ASearchPartsByRelations::searchByRelationsRec(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &class_set, ScAddr const &answer, sc_bool &sys_off, sc_bool &count_only)
{
    sc_bool result = SC_FALSE;

    ScIterator5Ptr iter = ctx.Iterator5(elem, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter->Next())
    {
        if (!ctx.HelperCheckArc(Keynodes::taxonomy_relation, iter->Get(4), ScType::EdgeAccessConstPosPerm))
            continue;

        sc_bool curr_result = searchByRelationsRec(ctx, iter->Get(2), class_set, answer, sys_off, count_only);
        sc_bool found = Utils::isSubsetOfAny(ctx, class_set, iter->Get(4));
        if (SC_TRUE == found && SC_TRUE == count_only)
        {
            count++;
            continue;
        }
        if (found || SC_TRUE == curr_result)
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

        sc_bool found = Utils::isSubsetOfAny(ctx, class_set, iter5->Get(4));

        sc_bool flag = SC_FALSE;
        ScIterator3Ptr iter3 = ctx.Iterator3(iter5->Get(0), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (iter3->Next())
        {
            sc_bool curr_result = searchByRelationsRec(ctx, iter3->Get(2), class_set, answer, sys_off, count_only);

            if (SC_TRUE == found && SC_TRUE == count_only)
            {
                count++;
                continue;
            }

            if (found || SC_TRUE == curr_result)
            {
                result = SC_TRUE;
                flag = SC_TRUE;
                Utils::addToAnswer(ctx, answer, iter3->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter3->Get(2), sys_off, SC_FALSE);
            }
        }

        if (SC_FALSE == flag || SC_TRUE == count_only)
            continue;

        Utils::addToAnswer(ctx, answer, iter5->Get(0), sys_off, SC_FALSE);
        Utils::addToAnswer(ctx, answer, iter5->Get(1), sys_off, SC_FALSE);
        Utils::addToAnswer(ctx, answer, iter5->Get(3), sys_off, SC_FALSE);
        Utils::addToAnswer(ctx, answer, iter5->Get(4), sys_off, SC_FALSE);
    }

    return result;
}

} // namespace
