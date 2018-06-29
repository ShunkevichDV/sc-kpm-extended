/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/


#include "searchUtils.hpp"
#include "sc-memory/cpp/sc_stream.hpp"

#include "ASearchPartsContainingPartWithCharacteristic.hpp"

#include <iostream>
#include <sstream>
#include <thread>

namespace search_extended
{

ScAddr ASearchPartsContainingPartWithCharacteristic::msAgentKeynode;
ScAddr ASearchPartsContainingPartWithCharacteristic::agent_question;

SC_AGENT_IMPLEMENTATION(ASearchPartsContainingPartWithCharacteristic)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr quest = ms_context->GetArcEnd(edgeAddr);

    ScAddr elem;

    ScIterator5Ptr iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_1);
    if (iter->Next())
        elem = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_2);
    if (iter->Next())
        class1 = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_3);
    if (iter->Next())
        class2 = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_4);
    if (iter->Next())
        nrel_characteristic = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter = ms_context->Iterator5(quest, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::rrel_5);
    if (iter->Next())
        value = iter->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr answer = ms_context->CreateNode(ScType::NodeConst);
    ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::system_element, answer);

    sc_bool sys_off = SC_TRUE;
    if (IS_SYSTEM_ELEMENT(((ScMemoryContext&)ms_context), elem))
        sys_off = SC_FALSE;

    Utils::addToAnswer((ScMemoryContext&)ms_context, answer, elem, sys_off, SC_FALSE);

    searchPartsClass1((ScMemoryContext&)ms_context, elem, answer, sys_off);

    Utils::finishSearchAction((ScMemoryContext&)ms_context, quest, answer);

    return SC_RESULT_OK;
}

sc_bool ASearchPartsContainingPartWithCharacteristic::searchPartsClass1(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &answer, sc_bool &sys_off)
{
    sc_bool result = SC_FALSE;

    ScIterator5Ptr iter = ctx.Iterator5(elem, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter->Next())
    {
        if (!ctx.HelperCheckArc(Keynodes::taxonomy_relation, iter->Get(4), ScType::EdgeAccessConstPosPerm))
            continue;

        sc_bool found = SC_FALSE;

        if (ctx.HelperCheckArc(class1, iter->Get(2), ScType::EdgeAccessConstPosPerm))
        {
            found = searchPartsClass2(ctx, iter->Get(2), answer, sys_off);
        }
        else
        {
            found = searchPartsClass1(ctx, iter->Get(2), answer, sys_off);
        }

        if (SC_TRUE == found)
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
            sc_bool found = SC_FALSE;

            if (ctx.HelperCheckArc(class1, iter3->Get(2), ScType::EdgeAccessConstPosPerm))
            {
                found = searchPartsClass2(ctx, iter3->Get(2), answer, sys_off);
            }
            else
            {
                found = searchPartsClass1(ctx, iter3->Get(2), answer, sys_off);
            }

            if (SC_TRUE == found)
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

sc_bool ASearchPartsContainingPartWithCharacteristic::searchPartsClass2(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &answer, sc_bool &sys_off)
{
    sc_bool result = SC_FALSE;

    ScIterator5Ptr iter = ctx.Iterator5(elem, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter->Next())
    {
        if (!ctx.HelperCheckArc(Keynodes::taxonomy_relation, iter->Get(4), ScType::EdgeAccessConstPosPerm))
            continue;

        sc_bool found = SC_FALSE;

        if (ctx.HelperCheckArc(class2, iter->Get(2), ScType::EdgeAccessConstPosPerm))
        {
            found = checkCharacteristicValue(ctx, iter->Get(2), answer, sys_off);
        }
        else
        {
            found = searchPartsClass2(ctx, iter->Get(2), answer, sys_off);
        }

        if (SC_TRUE == found)
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
            sc_bool found = SC_FALSE;

            if (ctx.HelperCheckArc(class2, iter3->Get(2), ScType::EdgeAccessConstPosPerm))
            {
                found = checkCharacteristicValue(ctx, iter3->Get(2), answer, sys_off);
            }
            else
            {
                found = searchPartsClass2(ctx, iter3->Get(2), answer, sys_off);
            }

            if (found)
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

sc_bool ASearchPartsContainingPartWithCharacteristic::checkCharacteristicValue(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &answer, sc_bool &sys_off)
{
    sc_bool result = SC_FALSE;
    ScIterator5Ptr iter = ctx.Iterator5(elem, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, nrel_characteristic);
    while (iter->Next())
    {
        ScIterator3Ptr iter3 = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, iter->Get(2));
        while (iter3->Next())
        {
            if (iter3->Get(0) == value)
            {
                result = SC_TRUE;
                Utils::addToAnswer(ctx, answer, iter3->Get(0), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter3->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter3->Get(2), sys_off, SC_FALSE);

                Utils::addToAnswer(ctx, answer, iter->Get(1), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter->Get(2), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter->Get(3), sys_off, SC_FALSE);
                Utils::addToAnswer(ctx, answer, iter->Get(4), sys_off, SC_FALSE);
                break;
            }
        }
    }
    return result;
}

} // namespace
