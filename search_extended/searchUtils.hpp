/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_memory.h"
#include "sc-memory/cpp/sc_module.hpp"

#include "searchUtils.generated.hpp"

#define IS_SYSTEM_ELEMENT(ctx, elem) (SC_TRUE == ctx.HelperCheckArc(Keynodes::system_element, elem, ScType::EdgeAccessConstPosPerm))

namespace search_extended
{

class Utils : public ScObject
{
    SC_CLASS()
    SC_GENERATED_BODY()

public:

    static sc_bool isSubset(ScMemoryContext & ctx, ScAddr const &parent_set, ScAddr const &child_set);
    static void addToAnswer(ScMemoryContext & ctx, ScAddr const &answer, ScAddr const &elem, sc_bool sys_off);
};

} //namespace
