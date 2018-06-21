/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "searchUtils.hpp"
#include "searchKeynodes.hpp"

namespace search_extended
{

void Utils::addToAnswer(ScMemoryContext &ctx, const ScAddr &answer, const ScAddr &elem, sc_bool sys_off = SC_TRUE, sc_bool key_element = SC_FALSE)
{
    if (SC_TRUE == sys_off)
    {
        if (IS_SYSTEM_ELEMENT(ctx, elem))
            return;
    }

    if (!ctx.HelperCheckArc(answer, elem, ScType::EdgeAccessConstPosPerm))
    {
        ScAddr arc = ctx.CreateArc(ScType::EdgeAccessConstPosPerm, answer, elem);
        if (SC_TRUE == key_element)
        {
            ctx.CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_key_sc_element, arc);
        }
    }
}

sc_bool Utils::isElementOfUnion(ScMemoryContext & ctx, ScAddr const &elem, ScAddr const &class_set)
{
    ScIterator3Ptr iter = ctx.Iterator3(class_set, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iter->Next())
    {
        if (ctx.HelperCheckArc(iter->Get(2), elem, ScType::EdgeAccessConstPosPerm))
            return SC_TRUE;
    }
    return SC_FALSE;
}

sc_bool Utils::isSubset(ScMemoryContext & ctx, ScAddr const &parent_set, ScAddr const &child_set)
{
    if (parent_set == child_set)
        return SC_TRUE;

    ScIterator5Ptr iter = ctx.Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, child_set, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_inclusion);
    while (iter->Next())
    {
        sc_bool result = isSubset(ctx, iter->Get(0), child_set);
        if (SC_TRUE == result)
            return SC_TRUE;
    }

    ScIterator3Ptr iter3 = ctx.Iterator3(ScType::NodeConst, ScType::EdgeAccessConstPosPerm, child_set);
    while (iter3->Next())
    {
        ScIterator5Ptr iter5 = ctx.Iterator5(iter3->Get(0), ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, Keynodes::nrel_subdividing);
        if (iter5->Next())
        {
            sc_bool result = isSubset(ctx, iter5->Get(2), child_set);
            if (SC_TRUE == result)
                return SC_TRUE;
        }
    }

    return SC_FALSE;
}

void Utils::finishSearchAction(ScMemoryContext & ctx, ScAddr const &action, ScAddr const &answer)
{
    ScAddr arc = ctx.CreateArc(ScType::EdgeDCommonConst, action, answer);
    ctx.CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_answer, arc);
    ctx.CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::system_element, arc);
    ctx.CreateArc(ScType::EdgeAccessConstPosPerm, Keynodes::question_finished, action);
}

}
